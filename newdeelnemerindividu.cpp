#include "newdeelnemerindividu.h"
#include "ui_newdeelnemerindividu.h"
#include <QDebug>

#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"

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

void NewDeelnemerIndividu::on_buttonBox_accepted()
{
    vvimDebug() << "emit addNewIndividu() to put information in database";
    emit addNewIndividu(ui->le_naam->text(),
                        ui->le_straat->text(),ui->le_huisnr->text(),ui->le_busnr->text(),
                        ui->le_postcode->text(),ui->le_plaats->text(),ui->rb_individu->isChecked(),ui->le_familieNaam->text());
}

void NewDeelnemerIndividu::on_rb_individu_toggled(bool checked)
{
    if(checked)
    {
        //vvimDebug() << "individu checked!";
        ui->label_familieNaam->setVisible(true);
        ui->le_familieNaam->setVisible(true);
        ui->label_naam->setText("Voornaam");
    }
    else
    {
        //vvimDebug() << "organisation checked!";
        ui->label_familieNaam->setVisible(false);
        ui->le_familieNaam->setVisible(false);
        ui->label_naam->setText("Organisatie");
    }
}
