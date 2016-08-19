#include <QDebug>
#include <QtWebKit>
#include <QMessageBox>
#include "deelnemer_location.h"
#include "ui_deelnemer_location.h"

#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"

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
    ui->webView->setHtml(htmlToLoad.arg(settings->value("apiKey").toString()).arg(deelnemerMarker->lat).arg(deelnemerMarker->lng).arg(zoom).arg(deelnemerMarker->caption())  );
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

    // [TODO] markers_js.append(str.arg(latitude).arg(longitude).arg(title).arg(iconcolor).arg(id));

    QFile f(":/html/google_maps_markers.html");
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

    ui->webView->setHtml(htmlToLoad.arg(settings->value("apiKey").toString()).arg(deelnemerMarker->lat).arg(deelnemerMarker->lng).arg(zoom).arg(deelnemerMarker->caption()).arg(markers_js)  );
}
