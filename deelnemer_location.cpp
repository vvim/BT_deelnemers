#include "deelnemer_location.h"
#include "ui_deelnemer_location.h"
#include <QDebug>

#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"

DeelnemerLocation::DeelnemerLocation(QString latitude, QString longitude, int zoom, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::deelnemer_location)
{
    settings = new QSettings("settings.ini", QSettings::IniFormat);

    vvimDebug() << "[TODO]" << "should we test if latitude / longitude are valid coordinates?";

    ui->setupUi(this);
    ui->webView->setHtml(QString("<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"initial-scale=1.0, user-scalable=no\" /><style type=\"text/css\">html { height: 100% } body { height: 100%; margin: 0; padding: 0 } #map_canvas { height: 100% } </style> <script src=\"https://maps.googleapis.com/maps/api/js?v=3&key=%1\"> </script> <script> var map; var markers = []; function initialize() { var myOptions = { center: new google.maps.LatLng(%2, %3), zoom: %4, mapTypeId: google.maps.MapTypeId.ROADMAP, panControl: true }; map = new google.maps.Map(document.getElementById(\"map_canvas\"), myOptions); } </script> </head> <body onload=\"initialize()\"> <div id=\"map_canvas\" style=\"width:100%; height:100%\"></div> </body></html>").arg(settings->value("General\apiKey").toString()).arg(latitude).arg(longitude).arg(zoom)  );
}

DeelnemerLocation::~DeelnemerLocation()
{
    delete settings;
    delete ui;
}
