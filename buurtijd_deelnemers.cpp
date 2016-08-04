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
        QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

        QLocale curLocale(QLocale("nl_NL"));
        QLocale::setDefault(curLocale);
    **/

    // when using a config file, use -->  settings.value("db/host").toString()
    qDebug() << QObject::tr(" ++ 0) trying to connect to:") << HOST << PASSWORD;
    db.setHostName(HOST);
    db.setDatabaseName(DATABASE);
    db.setUserName(USER);
    db.setPassword(PASSWORD);
    qDebug() << QObject::tr(" ++ 1) original connect options:") << db.connectOptions();
    db.setConnectOptions( "MYSQL_OPT_RECONNECT=true;" ) ;
    qDebug() << QObject::tr(" ++ 2) new connect options:") << db.connectOptions();

    if( !db.open() )
    {
        QMessageBox::critical(0, QObject::tr("Cannot open database"),
            QObject::tr("Unable to establish a database connection.\n"
                     "This is the error recieved: \n\n").append(db.lastError().text()), QMessageBox::Cancel);
        qCritical( "Failed to connect to database" );
        return false;
    }

    qDebug() << " ++ 3) connect options after opening:" << db.connectOptions();
    qDebug() << "Connected to database at " << HOST; // settings.value("db/host").toString();

    return true;
}

