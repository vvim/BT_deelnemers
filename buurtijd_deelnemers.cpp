#include "buurtijd_deelnemers.h"
#include "ui_buurtijd_deelnemers.h"
#include "sdeelnemermarker.h"
#include <QClipboard>

#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"

Buurtijd_deelnemers::Buurtijd_deelnemers(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Buurtijd_deelnemers)
{
    completer = NULL;
    location = NULL;
    notes = NULL;
    settings = new QSettings("settings.ini", QSettings::IniFormat);

    ui->setupUi(this);
    ui->deelnemersTable->setVisible(false); // only keeping it for debugging reasons, it has no use of the user
    ui->label_feedback->clear();

    ui->saveButton->setAutoFillBackground(true);
    ui->saveButton->setStyleSheet("background-color: rgb(255, 0, 0); color: rgb(255, 255, 255)");

    QPalette p = ui->plainTextEdit_varia->palette();
    p.setColor(QPalette::Active, QPalette::Base, QColor("#ffffb2"));
    p.setColor(QPalette::Inactive, QPalette::Base, QColor("#ffffb2"));
    ui->plainTextEdit_varia->setPalette(p);

    db = QSqlDatabase::addDatabase("QMYSQL");
    connectToDatabase();

    // Create the data model
    model_deelnemers = new QSqlRelationalTableModel(ui->deelnemersTable);
    model_deelnemers->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_deelnemers->setTable("t_deelnemers");

    // Populate the model
    if (!model_deelnemers->select())
    {
        showError(model_deelnemers->lastError());
        return;
    }

    loadCompleter();
    ui->deelnemersTable->setModel(model_deelnemers);

    mapper = new QDataWidgetMapper(this);
    mapper->setModel(model_deelnemers);

    // Populate the comboboxes
    mapComboboxAndTableModel(ui->comboBox_statuut,model_statuut,"t_deelnemer_statuut",model_deelnemers->fieldIndex("statuut"));
    mapComboboxAndTableModel(ui->comboBox_geslacht,model_geslacht,"t_deelnemer_geslacht",model_deelnemers->fieldIndex("geslacht"));
    mapComboboxAndTableModel(ui->comboBox_contactvoorkeur,model_contactVoorkeur,"t_deelnemer_contact_voorkeur",model_deelnemers->fieldIndex("contactvoorkeur"));
    mapComboboxAndTableModel(ui->comboBox_ingeschreven_door,model_ingeschrevenDoor,"t_deelnemer_ingeschreven_door",model_deelnemers->fieldIndex("ingeschreven_door"));
    mapComboboxAndTableModel(ui->comboBox_hoe_bt_leren_kennen,model_hoeLerenKennen,"t_deelnemer_hoe_leren_kennen",model_deelnemers->fieldIndex("bt_leren_kennen"));
    mapComboboxAndTableModel(ui->comboBox_niveau_nederlands,model_niveauNl,"t_deelnemer_niv_nederlands",model_deelnemers->fieldIndex("niveau_nl"));
    mapComboboxAndTableModel(ui->comboBox_soort,model_soort,"t_deelnemer_soort",model_deelnemers->fieldIndex("soort_deelnemer"));
    mapListviewAndTableModel(ui->listView_doelgroep,model_doelgroep,"t_deelnemer_doelgroep",model_deelnemers->fieldIndex("doelgroep"));
    mapListviewAndTableModel(ui->listView_domein,model_domein,"t_deelnemer_domein",model_deelnemers->fieldIndex("domein"));
    /// I do not use SetRelation() because several columns have NULL as a value,
    /// and then the entire row gets ignored during the JOIN. So small workaround:
    ///     -> as we do not use relations amongst the tables, maybe better to use the simpler QSqlTableModel ???

    //  ui.bookTable->setColumnHidden(model_deelnemers->fieldIndex("id"), true);

    //mapper->setItemDelegate(new BookDelegate(this));

    // mapping database t_deelnemers to ui->...
    /// -> 1) information for all participants
    mapper->addMapping(ui->le_naam, model_deelnemers->fieldIndex("naam"));
    mapper->addMapping(ui->le_straat, model_deelnemers->fieldIndex("straat"));
    mapper->addMapping(ui->le_huisnr, model_deelnemers->fieldIndex("huisnr"));
    mapper->addMapping(ui->le_busnr, model_deelnemers->fieldIndex("busnr"));
    mapper->addMapping(ui->le_postcode, model_deelnemers->fieldIndex("postcode"));
    mapper->addMapping(ui->le_plaats, model_deelnemers->fieldIndex("plaats"));
    mapper->addMapping(ui->le_telefoon, model_deelnemers->fieldIndex("telefoon"));
    mapper->addMapping(ui->le_gsm, model_deelnemers->fieldIndex("gsm"));
    mapper->addMapping(ui->le_email1, model_deelnemers->fieldIndex("email1"));
    mapper->addMapping(ui->le_email2, model_deelnemers->fieldIndex("email2"));
    mapper->addMapping(ui->checkbox_lid, model_deelnemers->fieldIndex("lid"));
    mapper->addMapping(ui->dateEdit_laatstecontact, model_deelnemers->fieldIndex("laatste_contact"));
    mapper->addMapping(ui->plainTextEdit_wanneer_best_contacteren, model_deelnemers->fieldIndex("wanneer_best_contacteren"));
    mapper->addMapping(ui->plainTextEdit_varia, model_deelnemers->fieldIndex("varia"));
    /** comboboxes:
     *    "contactvoorkeur"
     *    "hoe_buurtijd_leren_kennen"
     *    "soort" -> voor ComboBox gekozen omdat dit eenvoudig ter programmeren was, een radiobutton brengt niet echt een voordeel op
     **/


    /// -> 2) information for official members only
    mapper->addMapping(ui->dateEdit_inschrijfdatum, model_deelnemers->fieldIndex("inschrijf_datum"));
    mapper->addMapping(ui->checkBox_hr_goedgekeurd, model_deelnemers->fieldIndex("hr_goedgekeurd"));
    mapper->addMapping(ui->checkBox_contact_delen, model_deelnemers->fieldIndex("contact_delen"));
    mapper->addMapping(ui->checkBox_fotomateriaal_gebruiken, model_deelnemers->fieldIndex("fotomateriaal_gebruiken"));
    mapper->addMapping(ui->checkBox_was_lid_is_nu_gestopt, model_deelnemers->fieldIndex("was_lid_is_nu_gestopt"));
    /** comboboxes:
     *    "ingeschreven_door"
     **/
    /// -> 2b) information for official members that have quit
    mapper->addMapping(ui->dateEdit_stop_datum, model_deelnemers->fieldIndex("stop_datum"));
    mapper->addMapping(ui->plainTextEdit_stop_reden, model_deelnemers->fieldIndex("stop_reden"));


    /// -> 3) information for individuals only
    mapper->addMapping(ui->le_familieNaam, model_deelnemers->fieldIndex("familienaam"));
    mapper->addMapping(ui->dateEdit_geboortedatum, model_deelnemers->fieldIndex("geboortedatum"));
    mapper->addMapping(ui->le_afkomst, model_deelnemers->fieldIndex("afkomst"));
    mapper->addMapping(ui->checkBox_brandverzekering, model_deelnemers->fieldIndex("brand_verzekering"));
    mapper->addMapping(ui->checkBox_familiale_verzekering, model_deelnemers->fieldIndex("fam_verzekering"));
    /** comboboxes:
     *    "geslacht"
     *    "statuut"
     *    "niv_nederlands"
     **/

    /// -> 4) information for organisations only
    mapper->addMapping(ui->le_contactpersoon_organisatie_familienaam, model_deelnemers->fieldIndex("contactpersoon_familienaam"));
    mapper->addMapping(ui->le_contactpersoon_organisatie_voornaam, model_deelnemers->fieldIndex("contactpersoon_voornaam"));
    mapper->addMapping(ui->checkBox_vrijwilligers_verzekering, model_deelnemers->fieldIndex("vrijwilligers_verzekering"));
    /** listviews:
     *    "domein"
     *    "doelgroep"
     **/

    connect(ui->deelnemersTable->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(ChangeRow(QModelIndex)));

    ui->deelnemersTable->setCurrentIndex(model_deelnemers->index(0, 0));


    // make QDateEdits readable
    ui->dateEdit_geboortedatum->setDisplayFormat("dd MMM yyyy");
    ui->dateEdit_geboortedatum->setLocale(QLocale::Dutch);
    ui->dateEdit_geboortedatum->setCalendarPopup(true);  //zie http://stackoverflow.com/questions/7031962/qdateedit-calendar-popup
    ui->dateEdit_inschrijfdatum->setDisplayFormat("dd MMM yyyy");
    ui->dateEdit_inschrijfdatum->setLocale(QLocale::Dutch);
    ui->dateEdit_inschrijfdatum->setCalendarPopup(true);  //zie http://stackoverflow.com/questions/7031962/qdateedit-calendar-popup
    ui->dateEdit_laatstecontact->setDisplayFormat("dd MMM yyyy");
    ui->dateEdit_laatstecontact->setLocale(QLocale::Dutch);
    ui->dateEdit_laatstecontact->setCalendarPopup(true);  //zie http://stackoverflow.com/questions/7031962/qdateedit-calendar-popup

    ui->deelnemersTable->setFocus();
}

Buurtijd_deelnemers::~Buurtijd_deelnemers()
{
    db.close();
    vvimDebug() << "database closed";

    delete completer;
    delete settings;

    delete location;
    delete notes;
    delete ui;

    vvimDebug() << "[TODO]" << "delete alle modellen";
    vvimDebug() << "[TODO]" << "delete mapping!";
}

bool Buurtijd_deelnemers::connectToDatabase()
{
     vvimDebug() << "drivers: "<< QSqlDatabase::drivers();
    /**
        ** maybe needed to be added later: **

        QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

        QLocale curLocale(QLocale("nl_NL"));
        QLocale::setDefault(curLocale);
    **/

    db.setHostName(settings->value("db/host").toString());
    db.setDatabaseName(settings->value("db/databasename").toString());
    db.setUserName(settings->value("db/username").toString() );
    db.setPassword(settings->value("db/password").toString());
    db.setConnectOptions( "MYSQL_OPT_RECONNECT=true;" ) ;

    if( !db.open() )
    {
        showError( db.lastError() );
        qCritical( "Failed to connect to database" );
        return false;
    }

    vvimDebug() << "Connected to database at " << settings->value("db/host").toString();

    return true;
}


void Buurtijd_deelnemers::showError(const QSqlError &err)
{
    QMessageBox::critical(this, "Unable to initialize Database",
                "Error initializing database: " + err.text());
}

void Buurtijd_deelnemers::ChangeRow(QModelIndex new_index)
{
    if(!new_index.isValid())
    {
        vvimDebug() << "invalid index" << new_index;

        if(last_known_index.isValid())
        {
            // when new_index is not a valid index, but last_known_index is, we will continue with last_known_index (the last known valid index)
            vvimDebug() << "changed index to ``" << last_known_index << "and show that entry";
            new_index = last_known_index;
        }
        else
        {
            vvimDebug() << "last_known_index is also not valid" << last_known_index << "ABORT function";
            return;
        }
    }
    int currentRow = new_index.row();

    /* !! zodra je van fiche verandert , vraag aan de gebruiker "wijzigingen opslaan"? submitAll / Rollback
     *
     * 1) testen of er iets is gewijzigd bij de gebruiker
     *          hoe? houden we een BOOL bij met "heeft iets aangeklikt"
     *               of vergelijken we de inhoud van elke widget met de inhoud van het overeenkomstige model?
     *
     * 2) indien nodig: messagebox laten zien
     *
     * 3) verder doen met de werking van het programma
    int reply = QMessageBox::question(this, "Wijzigingen opslaan?", "Wijzigingen opslaan?",
                                    QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        // commit();
    }
    else if (reply == QMessageBox::No)
    {
        // rollback();
        qDebug() << "Yes was *not* clicked";
    }
    else
    {
        // de gebruiker heeft op ESC gedrukt of het kruisje aangeklikt: geldt niet als input
        return;
    }
     *
     * 3) verder doen met de werking van het programma
     *
    **/

    mapper->setCurrentModelIndex(new_index);
    last_known_index = new_index;
    vvimDebug() << "huidige index gewijzigd naar" << last_known_index;

    /* 4) kiezen wat zichtbaar is en wat niet */

    /// -> 1) information for all participants -> is always visible, so we do not need to make any changes here

    /// -> 2) information for official members only
    /// info: invalid index => -1

    showInformationForOfficialMember(
                model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("lid"))).toBool()
                );

    /// -> 2b) information for official members that have quit
    /// TODO: testen of 2b) getoond kan worden als 2) false is, want dat heeft geen zin
    showInformationForOfficialMemberHasQuit(
                model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("was_lid_is_nu_gestopt"))).toBool()
                );

    /// info: NULL.toInt() == 0 ! so this can be misleading , you can test with ().isNull()
    if(ThisRowContainsAnIndividual(currentRow))
    {
        /// -> 3) information for individuals only
        vvimDebug() << model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("naam"))).toString() << "is een individu";
        showInformationForIndividual(true);
        showInformationForOrganisation(false);
    }
    else
    {
        /// -> 4) information for organisations only
        vvimDebug() << model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("naam"))).toString() << "is een organisatie";
        showInformationForIndividual(false);
        showInformationForOrganisation(true);
    }
}

void Buurtijd_deelnemers::mapComboboxAndTableModel(QComboBox *combobox,QSqlRelationalTableModel *model, QString table_name, int t_deelnemers_fieldindex)
{
    model = new QSqlRelationalTableModel(combobox);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable(table_name);
    if (!model->select())
    {
        showError(model->lastError());
        return;
    }
    combobox->setModel(model);
    combobox->setModelColumn(1);
    mapper->addMapping(combobox, t_deelnemers_fieldindex,"currentIndex");
}

void Buurtijd_deelnemers::mapListviewAndTableModel(BTListView *listview, BTSqlTableModel *model, QString table_name, int t_deelnemers_fieldindex)
{
    model = new BTSqlTableModel(listview);
    //model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    listview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    model->setTable(table_name);
    if (!model->select())
    {
        showError(model->lastError());
        return;
    }
    listview->setModel(model);
    listview->setModelColumn(1);
    listview->setSelectionBehavior(QAbstractItemView::SelectRows);     // http://doc.qt.io/qt-4.8/qabstractitemview.html#SelectionBehavior-enum
    //listview->setSelectionMode(QAbstractItemView::MultiSelection);      //http://doc.qt.io/qt-4.8/qabstractitemview.html#SelectionMode-enum
    listview->setSelectionMode(QAbstractItemView::ExtendedSelection);      //http://doc.qt.io/qt-4.8/qabstractitemview.html#SelectionMode-enum
    // see issue #1 https://github.com/vvim/BT_deelnemers/issues/1
    mapper->addMapping(listview, t_deelnemers_fieldindex,"selectedItemsList");
}

void Buurtijd_deelnemers::showInformationForOfficialMember(bool make_visible)
{
    /// -> 2) information for official members only
    ui->dateEdit_inschrijfdatum->setVisible(make_visible);
    ui->label_inschrijfdatum->setVisible(make_visible);
    ui->checkBox_hr_goedgekeurd->setVisible(make_visible);
    ui->checkBox_contact_delen->setVisible(make_visible);
    ui->checkBox_fotomateriaal_gebruiken->setVisible(make_visible);
    ui->checkBox_was_lid_is_nu_gestopt->setVisible(make_visible);
    ui->comboBox_ingeschreven_door->setVisible(make_visible);
    ui->label_ingeschreven_door->setVisible(make_visible);
}

void Buurtijd_deelnemers::showInformationForOfficialMemberHasQuit(bool make_visible)
{
    /// -> 2b) information for official members that have quit
    ui->dateEdit_stop_datum->setVisible(make_visible);
    ui->label_stop_datum->setVisible(make_visible);
    ui->plainTextEdit_stop_reden->setVisible(make_visible);
    ui->label_stop_reden->setVisible(make_visible);
}

void Buurtijd_deelnemers::showInformationForIndividual(bool make_visible)
{
    /// -> 3) information for individuals only
    ui->le_familieNaam->setVisible(make_visible);
    ui->label_familieNaam->setVisible(make_visible);
    ui->dateEdit_geboortedatum->setVisible(make_visible);
    ui->label_geboortedatum->setVisible(make_visible);
    ui->le_afkomst->setVisible(make_visible);
    ui->label_afkomst->setVisible(make_visible);
    ui->checkBox_brandverzekering->setVisible(make_visible);
    ui->checkBox_familiale_verzekering->setVisible(make_visible);
    ui->comboBox_geslacht->setVisible(make_visible);
    ui->label_geslacht->setVisible(make_visible);
    ui->comboBox_statuut->setVisible(make_visible);
    ui->label_statuut->setVisible(make_visible);
    ui->comboBox_niveau_nederlands->setVisible(make_visible);
    ui->label_niveau_nederlands->setVisible(make_visible);
}

void Buurtijd_deelnemers::showInformationForOrganisation(bool make_visible)
{
    /// -> 4) information for organisations only
    ui->le_contactpersoon_organisatie_familienaam->setVisible(make_visible);
    ui->label_contactpersoon_organisatie->setVisible(make_visible);
    ui->le_contactpersoon_organisatie_voornaam->setVisible(make_visible);
    ui->checkBox_vrijwilligers_verzekering->setVisible(make_visible);
    ui->label_domein->setVisible(make_visible);
    ui->listView_domein->setVisible(make_visible);
    ui->label_doelgroep->setVisible(make_visible);
    ui->listView_doelgroep->setVisible(make_visible);
}

void Buurtijd_deelnemers::on_saveButton_clicked()
{
    vvimDebug() << "saving";
    model_deelnemers->database().transaction();

    if(model_deelnemers->submitAll())
    {
        vvimDebug() << "submitAll is successful, committing";
        model_deelnemers->database().commit();
        vvimDebug() << "reload the completer";
        loadCompleter();
        QString feedback = QString(ui->le_naam->text());
        ui->label_feedback->setText(feedback.append(" opgeslagen"));
        ui->label_feedback->setStyleSheet("font-style: italic; color: green");
    }
    else
    {
        vvimDebug() << "submitAll FAILED, rollback";
        vvimDebug() << "error:" << model_deelnemers->lastError();
        model_deelnemers->database().rollback();
        ui->label_feedback->setText("Er ging iets mis, wijzigingen niet opgeslagen");
        ui->label_feedback->setStyleSheet("font-weight: bold; color: red");
    }

    if(last_known_index.isValid())
        ChangeRow(last_known_index);
    else
        vvimDebug() << "last known index invalid?" << last_known_index;
}

void Buurtijd_deelnemers::loadCompleter()
{
    if(completer)
        delete completer;

    deelnemers_map.clear();
    QStringList deelnemers_list;

    int idIdx = model_deelnemers->fieldIndex("id");
    int familieNaamIdx = model_deelnemers->fieldIndex("familienaam");
    int naamIdx = model_deelnemers->fieldIndex("naam");
    vvimDebug() << "[CAVEAT]" << "we expect the combination [naam] [familienaam] to be unique, but can we guarantee that?" << "else we could mix the address of telephonenumber in the mix?" << "currently I add there ID-number to make every entry unique";

    for ( int i = 0 ; i < model_deelnemers->rowCount() ; ++i )
    {
        QString dlnmr;
        if( model_deelnemers->index( i, familieNaamIdx ).data().isNull())
        {
            dlnmr = model_deelnemers->index( i, naamIdx ).data().toString();
            dlnmr.append(QString(" (id %1)").arg(model_deelnemers->index( i, idIdx ).data().toString()));
        }
        else
        {
            dlnmr = model_deelnemers->index( i, naamIdx ).data().toString();
            dlnmr.append(" ");
            dlnmr.append(model_deelnemers->index( i, familieNaamIdx ).data().toString());
            dlnmr.append(QString(" (id %1)").arg(model_deelnemers->index( i, idIdx ).data().toString()));
        }
        deelnemers_list << dlnmr;
        deelnemers_map[dlnmr] = model_deelnemers->index( i, idIdx );
    }

    deelnemers_list.sort();
    completer = new MyCompleter(deelnemers_list, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->le_zoekDeelnemer->setCompleter(completer);
    vvimDebug() << "done, completer (re)loaded.";
}

void Buurtijd_deelnemers::on_pushButton_showDeelnemer_clicked()
{
    ui->label_feedback->clear();
    QModelIndex deelnemer_idx = deelnemers_map[ui->le_zoekDeelnemer->text()];
    if (deelnemer_idx.isValid()) // else no match
    {
        vvimDebug() << "toon user" << deelnemer_idx.data().toInt();
        ChangeRow(deelnemer_idx);
    }
    else
    {
        vvimDebug() << "no such user" << ui->le_zoekDeelnemer->text() << "invalid QModelIndex:" << deelnemer_idx << "should look like (QModelIndex(-1,-1,)";
    }
}

void Buurtijd_deelnemers::on_pushButton_showMaps_clicked()
{
    // !! IF lat/lng are valid? what if they are NULL? QPushButton should not been enabled

    vvimDebug() << "show GoogleMaps";
    if(location)
        delete location;

    SDeelnemerMarker deelnemer = readDeelnemer();

    location = new DeelnemerLocation(deelnemer, model_deelnemers);
    location->show();
}

void Buurtijd_deelnemers::on_cancelButton_clicked()
{
    QMessageBox::information(this, "Knop werkt nog niet",
                "De knop 'Bewerkingen annuleren' werkt nog niet, dit komt later.");
}

void Buurtijd_deelnemers::on_pushButton_showNotes_clicked()
{
    vvimDebug() << "show Notes";
    if(notes)
        delete notes;

    // populate QListView A with the different notes from this user -> new model? Filter(user_id)

    // B will be empty, but you can create a _new_ note or read/adapt a previous one
    //  -> see http://qt.apidoc.info/4.8.5/demos-textedit.html
    //  -> see http://stackoverflow.com/questions/6413901/how-to-implement-editor-using-qtextedit-with-toolbar

    int deelnemersId = model_deelnemers->data(model_deelnemers->index(last_known_index.row(),model_deelnemers->fieldIndex("id"))).toInt();

    QSqlRelationalTableModel *model_deelnemernotes = new QSqlRelationalTableModel();
    model_deelnemernotes->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_deelnemernotes->setTable("t_deelnemer_notas");

    // Populate the model
    if (!model_deelnemernotes->select())
    {
        showError(model_deelnemernotes->lastError());
        return;
    }

    model_deelnemernotes->setFilter(QString("deelnemer_id = %1").arg(deelnemersId));

    notes = new DeelnemerNotes(readDeelnemer(), model_deelnemernotes);
    notes->show();
}

void Buurtijd_deelnemers::on_pushButton_copyContactInformation_clicked()
{
    SDeelnemerMarker deelnemer = readDeelnemer();
    vvimDebug() << "copy user information to System Clipboard" << "user" << deelnemer.id;

    // https://contingencycoder.wordpress.com/2013/07/22/quick-tip-copy-any-text-to-the-system-clipboard-in-qt/
    // http://stackoverflow.com/a/15742175

    QApplication::clipboard()->setText(deelnemer.contactInformationInOneLine());
}

SDeelnemerMarker Buurtijd_deelnemers::readDeelnemer()
{
    vvimDebug() << "readDeelnemer() called";
    int currentRow = last_known_index.row();
    int deelnemersId = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("id"))).toInt();
    vvimDebug() << "for user" << deelnemersId;
    double latitude = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("lat"))).toDouble();
    double longitude = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("lng"))).toDouble();
    QString name = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("naam"))).toString();
    SAddress address = readAddress();

    QString email = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("email1"))).toString();
    QString telnr = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("telefoon"))).toString();
    QString gsm = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("gsm"))).toString();

    SDeelnemerMarker _deelnemer = SDeelnemerMarker(deelnemersId, latitude, longitude, name, address, email, telnr, gsm);

    if(ThisRowContainsAnIndividual(currentRow))
    {
        vvimDebug() << "this deelnemer is an individual";
        QString familienaam = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("familienaam"))).toString();
        _deelnemer.AddIndividu(SDeelnemerIndividu(name, familienaam));
    }


    return _deelnemer;
}

SAddress Buurtijd_deelnemers::readAddress()
{
    vvimDebug() << "readAddress() called";
    int currentRow = last_known_index.row();
    QString straat = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("straat"))).toString();
    QString huisnr = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("huisnr"))).toString();
    QString busnr = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("busnr"))).toString();
    QString postcode = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("postcode"))).toString();
    QString plaats = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("plaats"))).toString();

    return SAddress(straat,huisnr,busnr,postcode,plaats);
}

bool Buurtijd_deelnemers::ThisRowContainsAnIndividual(int row)
{
    return( !(model_deelnemers->data(model_deelnemers->index(row,model_deelnemers->fieldIndex("soort_deelnemer"))).isNull())
                && model_deelnemers->data(model_deelnemers->index(row,model_deelnemers->fieldIndex("soort_deelnemer"))).toInt() == 0);
}
