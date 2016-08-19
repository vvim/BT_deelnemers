#include <QDebug>
#include <QtWebKit>
#include "deelnemer_location.h"
#include "ui_deelnemer_location.h"

#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"

DeelnemerLocation::DeelnemerLocation(SDeelnemerMarker *_deelnemer, int zoom, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::deelnemer_location)
{
    settings = new QSettings("settings.ini", QSettings::IniFormat);
    deelnemerMarker = _deelnemer;

    vvimDebug() << "[TODO]" << "should we test if latitude / longitude are valid coordinates?";

    // read HTML file containing Google Maps
    QFile f(":/html/google_maps.html");
    if (!f.open(QFile::ReadOnly | QFile::Text))
    {
        vvimDebug() << "whoops, cannot read HTML-file";
    }
    QTextStream in(&f);
    QString str = in.readAll();
    f.close();

    ui->setupUi(this);
    ui->webView->setHtml(str.arg(settings->value("apiKey").toString()).arg(deelnemerMarker->lat).arg(deelnemerMarker->lng).arg(zoom).arg(deelnemerMarker->caption())  );
    ui->label_deelnemer_location->setText(deelnemerMarker->name);

    qDebug() << endl << str.arg(settings->value("apiKey").toString()).arg(deelnemerMarker->lat).arg(deelnemerMarker->lng).arg(zoom).arg(deelnemerMarker->caption());
}

DeelnemerLocation::~DeelnemerLocation()
{
    delete deelnemerMarker;
    delete settings;
    delete ui;
}
