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

    // Remember the indexes of the columns
    // invalid index => -1
    contactVoorkeurIdx = model_deelnemers->fieldIndex("contactvoorkeur");
    doelgroepIdx = model_deelnemers->fieldIndex("doelgroep");
    domeinIdx = model_deelnemers->fieldIndex("domein");
    geslachtIdx = model_deelnemers->fieldIndex("geslacht");
    hoeLerenKennenIdx = model_deelnemers->fieldIndex("bt_leren_kennen");
    ingeschrevenDoorIdx = model_deelnemers->fieldIndex("ingeschreven_door");
    niveauNlIdx = model_deelnemers->fieldIndex("niveau_nl");
    soortDeelnemerIdx = model_deelnemers->fieldIndex("soort_deelnemer");
    statuutIdx = model_deelnemers->fieldIndex("statuut");


    /** voorlopig nog geen relaties zetten. Er zijn namelijk veel cellen met NULL als waarde, en die worden dan uit de Join geweerd

    // Set the relations to the other database tables
    model_deelnemers->setRelation(contactVoorkeurIdx, QSqlRelation("t_deelnemer_contact_voorkeur", "id", "voorkeur"));
    // doelgroep -> multiple select...
    // domein -> multiple select...
    model_deelnemers->setRelation(geslachtIdx, QSqlRelation("t_deelnemer_geslacht", "id", "geslacht"));
    model_deelnemers->setRelation(hoeLerenKennenIdx, QSqlRelation("t_deelnemer_hoe_leren_kennen", "id", "info"));
    model_deelnemers->setRelation(ingeschrevenDoorIdx, QSqlRelation("t_deelnemer_ingeschreven_door", "id", "medewerker"));
    model_deelnemers->setRelation(niveauNlIdx, QSqlRelation("t_deelnemer_niv_nederlands", "id", "niveau"));
    model_deelnemers->setRelation(soortDeelnemerIdx, QSqlRelation("t_deelnemer_soort_deelnemer", "id", "soort"));
    model_deelnemers->setRelation(statuutIdx, QSqlRelation("t_deelnemer_statuut", "id", "statuut"));
    **/
    // Populate the model
    if (!model_deelnemers->select())
    {
        showError(model_deelnemers->lastError());
        return;
    }

    ui->deelnemersTable->setModel(model_deelnemers);

    //  ui.bookTable->setColumnHidden(model_deelnemers->fieldIndex("id"), true);

    QDataWidgetMapper *mapper = new QDataWidgetMapper(this);
    mapper->setModel(model_deelnemers);
    //mapper->setItemDelegate(new BookDelegate(this));
    mapper->addMapping(ui->le_naam, model_deelnemers->fieldIndex("naam"));
    mapper->addMapping(ui->le_familieNaam, model_deelnemers->fieldIndex("familienaam"));
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
    mapper->addMapping(ui->dateEdit_inschrijfdatum, model_deelnemers->fieldIndex("inschrijf_datum"));
    mapper->addMapping(ui->comboBox_ingeschreven_door, model_deelnemers->fieldIndex("ingeschreven_door"),"currentIndex");
    mapper->addMapping(ui->dateEdit_laatstecontact, model_deelnemers->fieldIndex("laatste_contact"));
    mapper->addMapping(ui->comboBox_geslacht, model_deelnemers->fieldIndex("geslacht"),"currentIndex");
    mapper->addMapping(ui->dateEdit_geboortedatum, model_deelnemers->fieldIndex("geboortedatum"));
    mapper->addMapping(ui->le_afkomst, model_deelnemers->fieldIndex("afkomst"));
    //mapper->addMapping(ui->le_, model_deelnemers->fieldIndex(""));

    connect(ui->deelnemersTable->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            mapper, SLOT(setCurrentModelIndex(QModelIndex)));

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
    // populate QComboBoxes
    ui->comboBox_geslacht->addItem("man");
    ui->comboBox_geslacht->addItem("vrouw");
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
    // !! zodra je van fiche verandert , vraag aan de gebruiker "wijzigingen opslaan"? submitAll / Rollback
    //  if(QMessageBox("save changes?"))
    //  {
    //      commit();
    //      mapper->setCurrentModelIndex(new_index);
    //  }
    //  else
    //  {
    //      rollback();
    //      return;
    //  }

    int currentRow = ui->deelnemersTable->currentIndex().row();
    int geslachtIdx = model_deelnemers->fieldIndex("geslacht");
    int naamIdx = model_deelnemers->fieldIndex("naam");
    int ingeschrdoorIdx = model_deelnemers->fieldIndex("ingeschreven_door");
    qDebug() << "row changed" << currentRow << new_index.row();
    qDebug() << "naam:" << model_deelnemers->data(model_deelnemers->index(currentRow,naamIdx)).toString();
    qDebug() << "geslacht:" << model_deelnemers->data(model_deelnemers->index(currentRow,geslachtIdx)).toInt();
    qDebug() << "    is NULL?" << model_deelnemers->data(model_deelnemers->index(currentRow,geslachtIdx)).isNull();
    qDebug() << "ingeschreven door:" << model_deelnemers->data(model_deelnemers->index(currentRow,ingeschrdoorIdx)).toInt();

/*

    if(model_deelnemers->data(model_deelnemers->index(currentRow,geslachtIdx)).isNull()) // NULL.toInt() == 0 ! so this can be misleading
    {
        // geslacht is NULL
        ui->comboBox_geslacht->setCurrentIndex(-1);
    }
    else
    {
        ui->comboBox_geslacht->setCurrentIndex(model_deelnemers->data(model_deelnemers->index(currentRow,geslachtIdx)).toInt());
    }
*/
    // mapper->setCurrentModelIndex(new_index); => crash. Why??
}
