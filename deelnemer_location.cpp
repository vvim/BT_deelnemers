#include <QDebug>
#include <QtWebKit>
#include <QMessageBox>
#include "deelnemer_location.h"
#include "ui_deelnemer_location.h"

#define model_INDIVIDUAL 0
#define model_LID 1

#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"


QString JavaScriptEscape(QString plaintext)
{
    // escape QString to be used as a string in JavaScript. Should not contain '
    // regular Qt::escape() from #include <QtGui/qtextdocument.h> will not escape the '

    QString javascriptversion = plaintext.replace("\n"," ").replace("'","\\'");
    javascriptversion.replace("\\\\'","\\'");
    return javascriptversion;
}

DeelnemerLocation::DeelnemerLocation(SDeelnemerMarker *_deelnemer, QSqlRelationalTableModel *_model_deelnemers, int _zoom, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::deelnemer_location)
{
    settings = new QSettings("settings.ini", QSettings::IniFormat);
    deelnemerMarker = _deelnemer;
    model_deelnemers = _model_deelnemers;
    zoom = _zoom;

    vvimDebug() << "[TODO]" << "should we test if latitude / longitude are valid coordinates?";

    // read HTML file containing Google Maps
    QFile f(":/html/google_maps.html");
    if (!f.open(QFile::ReadOnly | QFile::Text))
    {
        vvimDebug() << "whoops, cannot read HTML-file";
        QMessageBox messageBox;
        messageBox.critical(0,"Error",tr("Kan HTML-bestand niet lezen"));
        messageBox.setFixedSize(500,200);
        return;
    }
    QTextStream in(&f);
    QString htmlToLoad = in.readAll();
    f.close();

    ui->setupUi(this);
    ui->webView->setHtml(htmlToLoad.arg(settings->value("apiKey").toString()).arg(deelnemerMarker->lat).arg(deelnemerMarker->lng).arg(zoom).arg(JavaScriptEscape(deelnemerMarker->caption()))  );
    ui->label_deelnemer_location->setText(deelnemerMarker->name);
}

DeelnemerLocation::~DeelnemerLocation()
{
    delete deelnemerMarker;
    delete settings;
    delete ui;
    // DO NOT delete _model_deelnemers, we still need it in buurtijd_deelnemers.cpp
}

void DeelnemerLocation::on_pushButton_showAllDeelnemers_clicked()
{
    /* 1) query DB for deelnemers that are official members but don't have their LAT/LNG coordinates yet:
     *    Google Maps API doesn't allow more than 15 queries in one go => maximum = 15
     *    QSqlQuery query_no_lat_lng("SELECT * FROM t_deelnemers WHERE lid = 1  AND (lat is NULL OR lng is NULL) LIMIT 15");
     */


    /// bestaande marker is deelnemerMarker

    //                [id, lat, lng, 'title', icon, zIndex],  // icon: see http://stackoverflow.com/questions/7095574/google-maps-api-3-custom-marker-color-for-default-dot-marker/18623391#18623391
    QString str = "\n\t[%5, %1, %2,   '%3', 'http://maps.google.com/mapfiles/ms/icons/%4-dot.png'],";

    QString markers_js = "";

    int lidIdx = model_deelnemers->fieldIndex("lid");
    int latIdx = model_deelnemers->fieldIndex("lat");
    int lngIdx = model_deelnemers->fieldIndex("lng");
    int nameIdx = model_deelnemers->fieldIndex("naam");
    int familieNaamIdx = model_deelnemers->fieldIndex("familienaam");
    int idIdx = model_deelnemers->fieldIndex("id");
    int soortDeelnemerIdx = model_deelnemers->fieldIndex("soort_deelnemer");

    for ( int i = 0 ; i < model_deelnemers->rowCount() ; ++i )
    {
        // [TODO] markers_js.append(str.arg(latitude).arg(longitude).arg(title).arg(iconcolor).arg(id));
        if( ( model_deelnemers->index( i, lidIdx ).data().toInt() == model_LID )  // NULL toInt() == 0
                && (model_deelnemers->index( i, idIdx ).data().toInt() != deelnemerMarker->id) ) // we do not have to show the main marker twice
        {
            // we only show markers if the corresponding deelnemer is an official member, so lidIdx should be 1
            double latitude = model_deelnemers->index( i, latIdx ).data().toDouble();
            double longitude = model_deelnemers->index( i, lngIdx ).data().toDouble();
            QString title = model_deelnemers->index( i, nameIdx ).data().toString();
            int id = model_deelnemers->index( i, idIdx ).data().toInt();
            QString iconcolor;

            if( model_deelnemers->index( i, soortDeelnemerIdx ).data().toInt() == model_INDIVIDUAL) // deelnemer is an individual
            {
                iconcolor = "blue";
                title.append(" ").append(model_deelnemers->index( i, familieNaamIdx ).data().toString());
            }
            else // deelnemer is an organisation
            {
                iconcolor = "green";
            }

            markers_js.append(str.arg(latitude).arg(longitude).arg(JavaScriptEscape(title)).arg(iconcolor).arg(id));
        }

    }

    if(markers_js.length() < 1) // => no deelnemers to be marked
    {
        vvimDebug() << "no deelnemers found to be marked, did something go wrong?";
        return;
    }

    // chop last ',' from markers_js
    markers_js.chop(1);

    QFile f(":/html/google_maps_markers.html");
    if (!f.open(QFile::ReadOnly | QFile::Text))
    {
        vvimDebug() << "whoops, cannot read HTML-file for the markers";
        QMessageBox messageBox;
        messageBox.critical(0,"Error",tr("Kan HTML-bestand met de markers niet lezen"));
        messageBox.setFixedSize(500,200);
        return;
    }
    QTextStream in(&f);
    QString htmlToLoad = in.readAll();
    f.close();

    ui->webView->setHtml(htmlToLoad.arg(settings->value("apiKey").toString()).arg(deelnemerMarker->lat).arg(deelnemerMarker->lng).arg(zoom).arg(deelnemerMarker->caption()).arg(markers_js)  );
}
