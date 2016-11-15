#include "newdeelnemerindividu.h"
#include "ui_newdeelnemerindividu.h"

NewDeelnemerIndividu::NewDeelnemerIndividu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewDeelnemerIndividu)
{
    ui->setupUi(this);
}

NewDeelnemerIndividu::~NewDeelnemerIndividu()
{
    delete ui;
}
