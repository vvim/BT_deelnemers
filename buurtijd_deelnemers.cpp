#include "buurtijd_deelnemers.h"
#include "ui_buurtijd_deelnemers.h"

#define HOST "localhost"
#define DATABASE "buurtijd_test"
#define USER "testuser"
#define PASSWORD "HiDrNick!"

Buurtijd_deelnemers::Buurtijd_deelnemers(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Buurtijd_deelnemers)
{
    ui->setupUi(this);
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
    /// I do not use SetRelation() because several columns have NULL as a value,
    /// and then the entire row gets ignored during the JOIN. So small workaround:
    ///     -> as we do not use relations amongst the tables, maybe better to use the simpler QSqlTableModel ???

    // Remember the indexes of the columns
    // invalid index => -1
    doelgroepIdx = model_deelnemers->fieldIndex("doelgroep");
    domeinIdx = model_deelnemers->fieldIndex("domein");
    soortDeelnemerIdx = model_deelnemers->fieldIndex("soort_deelnemer");

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
    mapper->addMapping(ui->rb_individu, model_deelnemers->fieldIndex("soort"));
    /** comboboxes:
     *    "contactvoorkeur"
     *    "hoe_buurtijd_leren_kennen"
     **/
//TODO: individu of organisatie . Radiobutton, zie http://www.qtcentre.org/threads/21860-QRadioButton-and-QDataWidgetMapper qwidgetmapper radiobutton

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
//TODO: domein
//TODO: doelgroep

    //mapper->addMapping(ui->le_, model_deelnemers->fieldIndex(""));

    //connect(ui->deelnemersTable->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
    //        mapper, SLOT(setCurrentModelIndex(QModelIndex)));

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
    qDebug() << "database closed";
    delete ui;
}

bool Buurtijd_deelnemers::connectToDatabase()
{
     qDebug() << "drivers: "<< QSqlDatabase::drivers();
    /**
        ** maybe needed to be added later: **

        QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

        QLocale curLocale(QLocale("nl_NL"));
        QLocale::setDefault(curLocale);
    **/

    // when using a config file, use -->  settings.value("db/host").toString()
    db.setHostName(HOST);
    db.setDatabaseName(DATABASE);
    db.setUserName(USER);
    db.setPassword(PASSWORD);
    db.setConnectOptions( "MYSQL_OPT_RECONNECT=true;" ) ;

    if( !db.open() )
    {
        showError( db.lastError() );
        qCritical( "Failed to connect to database" );
        return false;
    }

    qDebug() << "Connected to database at " << HOST; // settings.value("db/host").toString();

    return true;
}


void Buurtijd_deelnemers::showError(const QSqlError &err)
{
    QMessageBox::critical(this, "Unable to initialize Database",
                "Error initializing database: " + err.text());
}

void Buurtijd_deelnemers::ChangeRow(QModelIndex new_index)
{
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

    // if(model_deelnemers->data(model_deelnemers->index(currentRow,geslachtIdx)).isNull()) // NULL.toInt() == 0 ! so this can be misleading
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
