#include "newdeelnemerindividu.h"
#include "ui_newdeelnemerindividu.h"

NewDeelnemerIndividu::NewDeelnemerIndividu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewDeelnemerIndividu)
{
    ui->setupUi(this);
    QPalette Pal(palette());

    // change the background color to sky blue to attract attention http://www.tayloredmktg.com/rgb/
    Pal.setColor(QPalette::Background, QColor(135,206,250));
    this->setAutoFillBackground(true);
    this->setPalette(Pal);
}

NewDeelnemerIndividu::~NewDeelnemerIndividu()
{
    delete ui;
}
