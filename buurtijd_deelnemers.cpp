#include "buurtijd_deelnemers.h"
#include "ui_buurtijd_deelnemers.h"
#include "sdeelnemermarker.h"
#include <QClipboard>

#define DEELNEMER_SOORT_is_INDIVIDU 1
#define DEELNEMER_SOORT_is_ORGANISATIE 2

#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"

Buurtijd_deelnemers::Buurtijd_deelnemers(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Buurtijd_deelnemers)
{
    completer = NULL;
    location = NULL;
    notes = NULL;
    newindividu = NULL;
    last_known_deelnemer = SDeelnemerMarker();
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
    connect(mapper, SIGNAL(currentIndexChanged(int)), this, SLOT(updateNavigationalButtons(int)));

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
    ui->dateEdit_stop_datum->setDisplayFormat("dd MMM yyyy");
    ui->dateEdit_stop_datum->setLocale(QLocale::Dutch);
    ui->dateEdit_stop_datum->setCalendarPopup(true);  //zie http://stackoverflow.com/questions/7031962/qdateedit-calendar-popup

    ui->le_zoekDeelnemer->setFocus();
}

Buurtijd_deelnemers::~Buurtijd_deelnemers()
{
    db.close();
    vvimDebug() << "database closed";

    delete completer;
    delete settings;

    delete location;
    delete notes;
    delete newindividu;
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

    vvimDebug() << "Wijzigingen opslaan? deelnemer:" << last_known_deelnemer.id;
    // [DEBUG] last_known_deelnemer.PrintInformation();
    if(UserMadeChangesToDeelnemer())
    {
        vvimDebug() << ".. changes have been made";
    /* !! zodra je van fiche verandert , vraag aan de gebruiker "wijzigingen opslaan"? submitAll / Rollback
     *
     * 1) testen of er iets is gewijzigd bij de gebruiker -> UserMadeChangesToDeelnemer()
     *
     * 2) indien nodig: messagebox laten zien
     *
     * 3) verder doen met de werking van het programma
     */


        vvimDebug() << ".. show QMessageBox";
        /// !! TODO na "SaveChanges", wordt de nieuwe informatie in SDeelnemer geladen?
        int reply = QMessageBox::question(this, tr("Wijzigingen opslaan?"), tr("Bij deelnemer %1 zijn gegevens gewijzigd. Wil je deze opslaan vooraleer we de fiche van een andere deelnemer openen?").arg(last_known_deelnemer.getName()),
                                        QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            vvimDebug() << ".... user clicked on 'Yes' to save the changes";
            on_saveButton_clicked();
            // commit();
        }
        else if (reply == QMessageBox::No)
        {
            on_cancelButton_clicked();
            // rollback();
            vvimDebug() << ".... NO was clicked by user => ignore changes";
        }
        else
        {
            // de gebruiker heeft op ESC gedrukt of het kruisje aangeklikt: geldt niet als input
            vvimDebug() << ".... user pressed ESC or closed the QMessageBox => exit this function";
            return;
        }

    }

    vvimDebug() << ".. changing current row";
    int currentRow = new_index.row();

    mapper->setCurrentModelIndex(new_index);
    last_known_index = new_index;
    vvimDebug() << "huidige index gewijzigd naar" << last_known_index;

    /* 4) kiezen wat zichtbaar is en wat niet */

    /// -> 1) information for all participants -> is always visible, so we do not need to make any changes here

    /// -> 2) information for official members only
    /// info: invalid index => -1

    last_known_deelnemer = readDeelnemer();

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
        vvimDebug() << "reload information in last_known_deelnemer so we can still test if the user changed information since the last save";
        last_known_deelnemer = readDeelnemer();
        QString feedback = QString(ui->le_naam->text());
        feedbackSuccess(feedback.append(" opgeslagen"));
    }
    else
    {
        vvimDebug() << "submitAll FAILED, rollback";
        vvimDebug() << "error:" << model_deelnemers->lastError();
        model_deelnemers->database().rollback();
        feedbackWarning("Er ging iets mis, wijzigingen niet opgeslagen");
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
    int plaatsIdx = model_deelnemers->fieldIndex("plaats");
    int gsmIdx = model_deelnemers->fieldIndex("gsm");
    int telefoonIdx = model_deelnemers->fieldIndex("telefoon");
    vvimDebug() << "[CAVEAT]"
                << "we expect the combination [naam] [familienaam] to be unique, but can we guarantee that?"
                << "else we could mix the address of telephonenumber in the mix?"
                << "currently I add there ID-number to make every entry unique";

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
            dlnmr.append(QString(" (id %1) %2 - %3 - %4").arg(model_deelnemers->index( i, idIdx ).data().toString()).arg(model_deelnemers->index( i, plaatsIdx ).data().toString()).arg(model_deelnemers->index( i, gsmIdx ).data().toString()).arg(model_deelnemers->index( i, telefoonIdx ).data().toString()));
        }
        dlnmr = dlnmr.simplified();
        while(dlnmr.endsWith(" -"))
        {
            dlnmr.chop(2);
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
    vvimDebug() << "... Cancel button pressed" << "undo all changes";

    vvimDebug() << "... rolling back, will also trigger ChangeRow()";
    if(!model_deelnemers->database().rollback())
        vvimDebug() << "rollback FAILED" << model_deelnemers->lastError().text();


    vvimDebug() << "... reload by using select() , see https://forum.qt.io/topic/2981/how-to-reload-the-tableview-to-reload-its-data/4";
    if(!model_deelnemers->select())
        vvimDebug() << "select FAILED" << model_deelnemers->lastError().text();

    vvimDebug() << "... we have to reset the CurrentModelIndex, or the currentindex will be invalid (-1) and no changes are possible";
    mapper->setCurrentModelIndex(last_known_index);
    vvimDebug() << "... end of function on_cancelButton_clicked() DONE";
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
    QString profile = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("varia"))).toString();
    QDate lastcontact = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("laatste_contact"))).toDate();
    QString when_best_to_contact = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("wanneer_best_contacteren"))).toString();
    int preferred_way_to_contact = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("contactvoorkeur"))).toInt();
    int how_first_contact = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("bt_leren_kennen"))).toInt();

    SAddress address = readAddress();
    SContacts contact = readContacts();

    SDeelnemerMarker _deelnemer = SDeelnemerMarker(deelnemersId, latitude, longitude, name, profile, lastcontact, when_best_to_contact, preferred_way_to_contact, how_first_contact, address, contact);

    if(ThisRowContainsAnIndividual(currentRow))
    {
        vvimDebug() << "this deelnemer is an individual";
        QString familienaam = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("familienaam"))).toString();
        int geslacht = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("geslacht"))).toInt();
        QDate geboortedatum = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("geboortedatum"))).toDate();
        QString afkomst = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("afkomst"))).toString();
        int statuut = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("statuut"))).toInt();
        int niveauNederlands = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("niveau_nl"))).toInt();
        bool fam_verzekering = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("fam_verzekering"))).toBool();
        bool brand_verzekering = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("brand_verzekering"))).toBool();

        _deelnemer.AddIndividu(SDeelnemerIndividu(name, familienaam, geslacht, geboortedatum, afkomst, statuut, niveauNederlands, fam_verzekering, brand_verzekering));
    }

    if(ThisRowContainsAnOrganisation(currentRow))
    {
        vvimDebug() << "this deelnemer is an organisation";
        QString contactpersoon_voornaam = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("contactpersoon_voornaam"))).toString();
        QString contactpersoon_familienaam = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("contactpersoon_familienaam"))).toString();
        bool vrijw_verzekering = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("vrijwilligers_verzekering"))).toBool();
        QString doelgroep = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("doelgroep"))).toString();
        QString domein = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("domein"))).toString();

        _deelnemer.AddOrganisatie(SDeelnemerOrganisatie(contactpersoon_voornaam, contactpersoon_familienaam, vrijw_verzekering, doelgroep, domein));
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

SContacts Buurtijd_deelnemers::readContacts()
{
    vvimDebug() << "readContacts() called";
    int currentRow = last_known_index.row();
    QString email1 = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("email1"))).toString();
    QString email2 = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("email2"))).toString();
    QString telnr = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("telefoon"))).toString();
    QString gsm = model_deelnemers->data(model_deelnemers->index(currentRow,model_deelnemers->fieldIndex("gsm"))).toString();

    return SContacts(email1,email2,telnr,gsm);
}

bool Buurtijd_deelnemers::ThisRowContainsAnIndividual(int row)
{
    return( !(model_deelnemers->data(model_deelnemers->index(row,model_deelnemers->fieldIndex("soort_deelnemer"))).isNull())
                && model_deelnemers->data(model_deelnemers->index(row,model_deelnemers->fieldIndex("soort_deelnemer"))).toInt() == DEELNEMER_SOORT_is_INDIVIDU);
}

bool Buurtijd_deelnemers::ThisRowContainsAnOrganisation(int row)
{
    return( !(model_deelnemers->data(model_deelnemers->index(row,model_deelnemers->fieldIndex("soort_deelnemer"))).isNull())
                && model_deelnemers->data(model_deelnemers->index(row,model_deelnemers->fieldIndex("soort_deelnemer"))).toInt() == DEELNEMER_SOORT_is_ORGANISATIE);
}

bool Buurtijd_deelnemers::UserMadeChangesToDeelnemer()
{
    if(last_known_deelnemer.id < 0)
    {
        vvimDebug() << "last_known_deelnemer.id < 0, this is normal at the startup of the program as no deelnemer has been loaded yet";
        return false;
    }

    int id = last_known_deelnemer.id;
    //lat = 0;
    //lng = 0;

    if(ui->le_naam->text() != last_known_deelnemer.name)
    {
        vvimDebug() << "deelnemer" << id << "name has changed. DB:" << last_known_deelnemer.name << "now:" << ui->le_naam->text();
        return true;
    }

    if(ui->plainTextEdit_varia->toPlainText() != last_known_deelnemer.profile)
    {
        vvimDebug() << "deelnemer" << id << "profile has changed. DB:" << last_known_deelnemer.profile << "now:" << ui->plainTextEdit_varia->toPlainText();
        return true;
    }

    if(ui->dateEdit_laatstecontact->date() != last_known_deelnemer.lastcontact)
    {
        vvimDebug() << "deelnemer" << id << "lastcontact has changed. DB:" << last_known_deelnemer.lastcontact.toString() << "now:" << ui->dateEdit_laatstecontact->date().toString();
        return true;
    }

    if(ui->plainTextEdit_wanneer_best_contacteren->toPlainText() != last_known_deelnemer.when_best_to_contact)
    {
        vvimDebug() << "deelnemer" << id << "when_best_to_contact has changed. DB:" << last_known_deelnemer.when_best_to_contact << "now:" << ui->plainTextEdit_wanneer_best_contacteren->toPlainText();
        return true;
    }

    if(ui->comboBox_contactvoorkeur->currentIndex() != last_known_deelnemer.preferred_way_to_contact)
    {
        vvimDebug() << "deelnemer" << id << "preferred_way_to_contact has changed. DB:" << last_known_deelnemer.preferred_way_to_contact << "now:" << ui->comboBox_contactvoorkeur->currentIndex() << ui->comboBox_contactvoorkeur->currentText();
        return true;
    }

    if(ui->comboBox_hoe_bt_leren_kennen->currentIndex() != last_known_deelnemer.how_first_contact)
    {
        vvimDebug() << "deelnemer" << id << "how_first_contact_BT has changed. DB:" << last_known_deelnemer.how_first_contact << "now:" << ui->comboBox_hoe_bt_leren_kennen->currentIndex() << ui->comboBox_hoe_bt_leren_kennen->currentText();
        return true;
    }

    if(UserMadeChangesToDeelnemerAddress())
    {
        vvimDebug() << "deelnemer" << id << "address has changed.";
        vvimDebug() << "[TODO] Address has changed => recalculate LAT and LNG!";
        return true;
    }

    if(UserMadeChangesToDeelnemerContacts())
    {
        vvimDebug() << "deelnemer" << id << "contacts have changed.";
        return true;
    }

    if(UserMadeChangesToDeelnemerIndividu())
    {
        vvimDebug() << "deelnemer" << id << "individu has changed.";
        return true;
    }

    if(UserMadeChangesToDeelnemerOrganisatie())
    {
        vvimDebug() << "deelnemer" << id << "organisatie has changed.";
        return true;
    }

    return false;
}


bool Buurtijd_deelnemers::UserMadeChangesToDeelnemerAddress()
{
    if(last_known_deelnemer.id < 0)
    {
        vvimDebug() << "last_known_deelnemer.id < 0, this is normal at the startup of the program as no deelnemer has been loaded yet";
        return false;
    }

    if(!last_known_deelnemer.address_present)
    {
        vvimDebug() << "[WARNING]" << "last_known_deelnemer has no defined address, something went wrong";
        return true;
    }

    if(ui->le_straat->text() != last_known_deelnemer.Address.street)
    {
        vvimDebug() << "...street has changed. DB:" << last_known_deelnemer.Address.street << "now:" << ui->le_straat->text();
        return true;
    }

    if(ui->le_huisnr->text() != last_known_deelnemer.Address.housenr)
    {
        vvimDebug() << "...housenr has changed. DB:" << last_known_deelnemer.Address.housenr << "now:" << ui->le_huisnr->text();
        return true;
    }

    if(ui->le_busnr->text() != last_known_deelnemer.Address.busnr)
    {
        vvimDebug() << "...busnr has changed. DB:" << last_known_deelnemer.Address.busnr << "now:" << ui->le_busnr->text();
        return true;
    }

    if(ui->le_postcode->text() != last_known_deelnemer.Address.postalcode)
    {
        vvimDebug() << "...postalcode has changed. DB:" << last_known_deelnemer.Address.postalcode << "now:" << ui->le_postcode->text();
        return true;
    }

    if(ui->le_plaats->text() != last_known_deelnemer.Address.plaats)
    {
        vvimDebug() << "...plaats has changed. DB:" << last_known_deelnemer.Address.plaats << "now:" << ui->le_plaats->text();
        return true;
    }

    return false;
}

bool Buurtijd_deelnemers::UserMadeChangesToDeelnemerContacts()
{
    if(last_known_deelnemer.id < 0)
    {
        vvimDebug() << "last_known_deelnemer.id < 0, this is normal at the startup of the program as no deelnemer has been loaded yet";
        return false;
    }

    if(!last_known_deelnemer.contacts_present)
    {
        vvimDebug() << "[WARNING]" << "last_known_deelnemer has no defined contacts, something went wrong";
        return true;
    }

    if(ui->le_email1->text() != last_known_deelnemer.Contacts.email1)
    {
        vvimDebug() << "...email1 has changed. DB:" << last_known_deelnemer.Contacts.email1 << "now:" << ui->le_email1->text();
        return true;
    }

    if(ui->le_email2->text() != last_known_deelnemer.Contacts.email2)
    {
        vvimDebug() << "...email2 has changed. DB:" << last_known_deelnemer.Contacts.email2 << "now:" << ui->le_email2->text();
        return true;
    }

    if(ui->le_telefoon->text() != last_known_deelnemer.Contacts.telnr)
    {
        vvimDebug() << "...telnr has changed. DB:" << last_known_deelnemer.Contacts.telnr << "now:" << ui->le_telefoon->text();
        return true;
    }

    if(ui->le_gsm->text() != last_known_deelnemer.Contacts.gsm)
    {
        vvimDebug() << "...gsm has changed. DB:" << last_known_deelnemer.Contacts.gsm << "now:" << ui->le_gsm->text();
        return true;
    }

    return false;
}

bool Buurtijd_deelnemers::UserMadeChangesToDeelnemerIndividu()
{
    if(last_known_deelnemer.id < 0)
    {
        vvimDebug() << "last_known_deelnemer.id < 0, this is normal at the startup of the program as no deelnemer has been loaded yet";
        return false;
    }

    if( ui->comboBox_soort->currentIndex() == DEELNEMER_SOORT_is_INDIVIDU)
    {
        if(!last_known_deelnemer.individu_present )
        {
            vvimDebug() << "...deelnemer in UI is 'individu', but in SDeelnemer it is not => changes have been made.";
            return true;
        }

        // the deelnemer in the program is an INDIVIDU and the deelnemer in SDeelnemer is an INDIVIDU
        // so we should test all attributes

        if(ui->le_familieNaam->text() != last_known_deelnemer.Individu.familienaam)
        {
            vvimDebug() << "...familienaam has changed. DB:" << last_known_deelnemer.Individu.familienaam << "now:" << ui->le_familieNaam->text();
            return true;
        }

        if(ui->comboBox_geslacht->currentIndex() != last_known_deelnemer.Individu.geslacht)
        {
            vvimDebug() << "...geslacht has changed. DB:" << last_known_deelnemer.Individu.geslacht << last_known_deelnemer.Individu.getGeslacht() << "now:" << ui->comboBox_geslacht->currentIndex() << ui->comboBox_geslacht->currentText();
            return true;
        }

        if(ui->dateEdit_geboortedatum->date() != last_known_deelnemer.Individu.geboortedatum)
        {
            vvimDebug() << "...geboortedatum has changed. DB:" << last_known_deelnemer.Individu.geboortedatum << "now:" << ui->dateEdit_geboortedatum->date();
            return true;
        }

        if(ui->le_afkomst->text() != last_known_deelnemer.Individu.afkomst)
        {
            vvimDebug() << "...afkomst has changed. DB:" << last_known_deelnemer.Individu.afkomst << "now:" << ui->le_afkomst->text();
            return true;
        }

        if(ui->comboBox_statuut->currentIndex() != last_known_deelnemer.Individu.statuut)
        {
            vvimDebug() << "...statuut has changed. DB:" << last_known_deelnemer.Individu.statuut /** << last_known_deelnemer.Individu.getStatuut() **/ << "now:" << ui->comboBox_statuut->currentIndex() << ui->comboBox_statuut->currentText();
            return true;
        }

        if(ui->comboBox_niveau_nederlands->currentIndex() != last_known_deelnemer.Individu.niveauNederlands)
        {
            vvimDebug() << "...niveau Nederlands has changed. DB:" << last_known_deelnemer.Individu.niveauNederlands /** << last_known_deelnemer.Individu.getNiveauNederlands() **/ << "now:" << ui->comboBox_niveau_nederlands->currentIndex() << ui->comboBox_niveau_nederlands->currentText();
            return true;
        }

        if( ui->checkBox_familiale_verzekering->isChecked() != last_known_deelnemer.Individu.familialeverzekering)
        {
            vvimDebug() << "...familiale verzekering has changed. DB:" << last_known_deelnemer.Individu.familialeverzekering << "now:" << ui->checkBox_familiale_verzekering->isChecked();
            return true;
        }

        if( ui->checkBox_brandverzekering->isChecked() != last_known_deelnemer.Individu.brandverzekering)
        {
            vvimDebug() << "...brandverzekering has changed. DB:" << last_known_deelnemer.Individu.brandverzekering << "now:" << ui->checkBox_brandverzekering->isChecked();
            return true;
        }
    }
    else
    {
        // the deelnemer in the program is NOT an INDIVIDU
        // so we should test if the deelnemer in SDeelnemer is one
        // return TRUE if there is an INDIVIDU in SDeelnemer, return FALSE if there isn't
        if(last_known_deelnemer.individu_present)
        {
            vvimDebug() << "...deelnemer in UI is NOT 'individu', but in SDeelnemer it is => changes have been made.";
            return true;
        }
    }

    return false;
}

bool Buurtijd_deelnemers::UserMadeChangesToDeelnemerOrganisatie()
{
    if(last_known_deelnemer.id < 0)
    {
        vvimDebug() << "last_known_deelnemer.id < 0, this is normal at the startup of the program as no deelnemer has been loaded yet";
        return false;
    }

    if( ui->comboBox_soort->currentIndex() == DEELNEMER_SOORT_is_ORGANISATIE)
    {
        if(!last_known_deelnemer.organisatie_present )
        {
            vvimDebug() << "...deelnemer in UI is 'organisatie', but in SDeelnemer it is not => changes have been made.";
            return true;
        }

        // the deelnemer in the program is an ORGANISATIE and the deelnemer in SDeelnemer is an ORGANISATIE
        // so we should test all attributes

        if(ui->le_contactpersoon_organisatie_voornaam->text() != last_known_deelnemer.Organisatie.contactpersoon_voornaam)
        {
            vvimDebug() << "...contactpersoon-voornaam has changed. DB:" << last_known_deelnemer.Organisatie.contactpersoon_voornaam << "now:" << ui->le_contactpersoon_organisatie_voornaam->text();
            return true;
        }

        if(ui->le_contactpersoon_organisatie_familienaam->text() != last_known_deelnemer.Organisatie.contactpersoon_familienaam)
        {
            vvimDebug() << "...contactpersoon-voornaam has changed. DB:" << last_known_deelnemer.Organisatie.contactpersoon_familienaam << "now:" << ui->le_contactpersoon_organisatie_familienaam->text();
            return true;
        }

        if( ui->checkBox_vrijwilligers_verzekering->isChecked() != last_known_deelnemer.Organisatie.vrijwilligersverzekering)
        {
            vvimDebug() << "...vrijwilligersverzekering has changed. DB:" << last_known_deelnemer.Organisatie.vrijwilligersverzekering << "now:" << ui->checkBox_vrijwilligers_verzekering->isChecked();
            return true;
        }

        if(ui->listView_doelgroep->selectedItemsList() != last_known_deelnemer.Organisatie.doelgroep)
        {
            vvimDebug() << "...doelgroep has changed. DB:" << last_known_deelnemer.Organisatie.doelgroep << "now:" << ui->listView_doelgroep->selectedItemsList();
            return true;
        }

        if(ui->listView_domein->selectedItemsList() != last_known_deelnemer.Organisatie.domein)
        {
            vvimDebug() << "...domein has changed. DB:" << last_known_deelnemer.Organisatie.domein << "now:" << ui->listView_domein->selectedItemsList();
            return true;
        }

    }
    else
    {
        // the deelnemer in the program is NOT an ORGANISATIE
        // so we should test if the deelnemer in SDeelnemer is one
        // return TRUE if there is an ORGANISATIE in SDeelnemer, return FALSE if there isn't
        if(last_known_deelnemer.organisatie_present)
        {
            vvimDebug() << "...deelnemer in UI is NOT 'organisatie', but in SDeelnemer it is => changes have been made.";
            return true;
        }
    }

    return false;
}

void Buurtijd_deelnemers::on_pushButton_CreateNewIndividu_clicked()
{
    vvimDebug() << "show NewBuurtijderIndividu";
    if(newindividu)
        delete newindividu;

    newindividu = new NewDeelnemerIndividu();
    connect(newindividu, SIGNAL(addNewIndividu(QString,QString,QString,QString,QString,QString,QString)),
            this, SLOT(addNewIndividuToDatabase(QString,QString,QString,QString,QString,QString,QString))); // wat met NULL ?
    newindividu->show();
}

void Buurtijd_deelnemers::addNewIndividuToDatabase(QString naam,QString familienaam,QString straat,QString huisnr,QString busnr,QString postcode,QString plaats)
{
    vvimDebug() << "will add to database:" << QString("\n..%1 %2\n..%3 %4 bus %5\n..%6 %7").arg(naam).arg(familienaam).arg(straat).arg(huisnr).arg(busnr).arg(postcode).arg(plaats);
    ui->le_zoekDeelnemer->clear();

    QString SQLquery_add_new_individu = "INSERT INTO `t_deelnemers` ( `naam`, `familienaam`, `straat`, `huisnr`, `busnr`, `postcode`, `plaats`, `soort_deelnemer`, `geslacht`, `geboortedatum`, `statuut`, `bt_leren_kennen`, `niveau_nl`, `contactvoorkeur`, `ingeschreven_door`, `laatste_contact`, `inschrijf_datum`) VALUES (:naam, :familienaam, :straat, :huisnr, :busnr, :postcode, :plaats, :soort_deelnemer, 0 , '1800-01-01' , 0 , 0 , 0 , 0 , 0 , '1800-01-01' , '1800-01-01' )";

    QSqlQuery query_add_new_individu;
    query_add_new_individu.prepare(SQLquery_add_new_individu);
    query_add_new_individu.bindValue(":naam", naam);
    query_add_new_individu.bindValue(":familienaam", familienaam);
    query_add_new_individu.bindValue(":straat", straat);
    query_add_new_individu.bindValue(":huisnr", huisnr);
    query_add_new_individu.bindValue(":busnr", busnr);
    query_add_new_individu.bindValue(":postcode", postcode);
    query_add_new_individu.bindValue(":plaats", plaats);
    query_add_new_individu.bindValue(":soort_deelnemer", DEELNEMER_SOORT_is_INDIVIDU);

    QString feedback = QString("%1 %2").arg(naam).arg(familienaam);
    if(query_add_new_individu.exec())
    {
        vvimDebug() << "adding new individual" << "success";
        feedbackSuccess(feedback.append(" opgeslagen"));
    }
    else
    {
        vvimDebug() << "adding new individual" << "[FAILED]" <<     query_add_new_individu.lastQuery();
        feedbackWarning(QString("Er ging iets mis, %1 niet opgeslagen").arg(feedback));
    }

    //update the model
    model_deelnemers->database().transaction();
    model_deelnemers->submitAll();

    //reload the completer
    loadCompleter();

    //show the newly added individual:
    vvimDebug() << "[WARNING] we should first test if we need to save made changes to the shown user before showing newly added individual?";
    // show individual at the bottom of the model??
    int lastRow = model_deelnemers->rowCount() - 1;
    QModelIndex deelnemer_idx = model_deelnemers->index( lastRow, 0);
    vvimDebug() << deelnemer_idx << model_deelnemers->rowCount() << lastRow;
    ChangeRow(deelnemer_idx);
}

void Buurtijd_deelnemers::feedbackSuccess(QString message)
{
    ui->label_feedback->setText(message);
    ui->label_feedback->setStyleSheet("font-style: italic; color: green");
}

void Buurtijd_deelnemers::feedbackWarning(QString message)
{
    ui->label_feedback->setText(message);
    ui->label_feedback->setStyleSheet("font-weight: bold; color: red");
}

void Buurtijd_deelnemers::updateNavigationalButtons(int row)
{
    ui->pushButton_previous->setEnabled(row > 0);
    ui->pushButton_next->setEnabled(row < model_deelnemers->rowCount() - 1);
}

void Buurtijd_deelnemers::on_pushButton_previous_clicked()
{
    vvimDebug() << "button 'previous' pressed";
    ui->label_feedback->clear();
    ui->le_zoekDeelnemer->clear();

    int row = last_known_index.row() - 1;

    QModelIndex deelnemer_idx = model_deelnemers->index(row,0);

    if (deelnemer_idx.isValid()) // else no match
    {
        vvimDebug() << "toon user" << deelnemer_idx.data().toInt();
        ChangeRow(deelnemer_idx);
    }
    else
    {
        vvimDebug() << "no such user at index" << deelnemer_idx;
    }
}

void Buurtijd_deelnemers::on_pushButton_next_clicked()
{
    vvimDebug() << "button 'previous' pressed";
    ui->label_feedback->clear();
    ui->le_zoekDeelnemer->clear();

    int row = last_known_index.row() + 1;

    QModelIndex deelnemer_idx = model_deelnemers->index(row,0);

    if (deelnemer_idx.isValid()) // else no match
    {
        vvimDebug() << "toon user" << deelnemer_idx.data().toInt();
        ChangeRow(deelnemer_idx);
    }
    else
    {
        vvimDebug() << "no such user at index" << deelnemer_idx;
    }
}

void Buurtijd_deelnemers::keyPressEvent( QKeyEvent *k )
{
    switch(k->key())
    {
        case Qt::Key_PageDown:
            on_pushButton_next_clicked();
            break;
        case Qt::Key_PageUp:
            on_pushButton_previous_clicked();
            break;
    }
}

void Buurtijd_deelnemers::on_pushButton_lastcontact_is_today_clicked()
{
    int row = -1;

    if(last_known_index.isValid())
        row = last_known_index.row();
    else
    {
        vvimDebug() << "last_known_index is not valid, so I have no idea which row. RETURN";
        return;
    }

    int last_contact_column = model_deelnemers->fieldIndex("laatste_contact");
    QModelIndex j = model_deelnemers->index(row,last_contact_column);

    /**

Add icon for 'today'

icon downloaded from free library at http://www.iconarchive.com/show/ios7-icons-by-icons8/Time-And-Date-Today-icon.html


      **/

    // change the _model_
    vvimDebug() << "last contact was:" << model_deelnemers->data(j).toDate().toString() << row << last_contact_column;
    model_deelnemers->setData(j,QDate::currentDate());

    // change the _view_
    ui->dateEdit_laatstecontact->setDate(QDate::currentDate());

/*  vvim TODO: er zou toch een manier moeten zijn om gewoon dateEdit_laatstecontact te herladen, ipv het manueel te doen??
            iets zoals:
    model_deelnemers->->select();

    !! ipv mijn voorstel onder "change the _view_" */

}

void Buurtijd_deelnemers::on_pushButton_first_clicked()
{
    vvimDebug() << "button 'first' pressed";
    ui->label_feedback->clear();
    ui->le_zoekDeelnemer->clear();

    int row = 0;

    QModelIndex deelnemer_idx = model_deelnemers->index(row,0);

    if (deelnemer_idx.isValid()) // else no match
    {
        vvimDebug() << "toon user" << deelnemer_idx.data().toInt();
        ChangeRow(deelnemer_idx);
    }
    else
    {
        vvimDebug() << "no such user at index" << deelnemer_idx;
    }
}

void Buurtijd_deelnemers::on_pushButton_last_clicked()
{
    vvimDebug() << "button 'last' pressed";
    ui->label_feedback->clear();
    ui->le_zoekDeelnemer->clear();

    int row = model_deelnemers->rowCount() - 1;

    QModelIndex deelnemer_idx = model_deelnemers->index(row,0);

    if (deelnemer_idx.isValid()) // else no match
    {
        vvimDebug() << "toon user" << deelnemer_idx.data().toInt();
        ChangeRow(deelnemer_idx);
    }
    else
    {
        vvimDebug() << "no such user at index" << deelnemer_idx;
    }

}
