#include "deelnemer_location.h"
#include "ui_deelnemer_location.h"

DeelnemerLocation::DeelnemerLocation(QString address, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::deelnemer_location)
{
    ui->setupUi(this);
    ui->webView->setHtml(QString("<!DOCTYPE html><html><body><h1>Hello World!</h1>My address is: %1</body></html>").arg(address));
}

DeelnemerLocation::~DeelnemerLocation()
{
    delete ui;
}
