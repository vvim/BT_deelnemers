#include "overzichtvraagaanbod.h"
#include "ui_overzichtvraagaanbod.h"

#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"

OverzichtVraagAanbod::OverzichtVraagAanbod(QSqlRelationalTableModel *_model_vraag_aanbod, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OverzichtVraagAanbod)
{
    ui->setupUi(this);
    model_vraag_aanbod = _model_vraag_aanbod;
    ui->tableView->setModel(model_vraag_aanbod);
    ui->tableView->setColumnHidden(0,1); // hide column with "id"
    ui->tableView->setColumnHidden(1,1); // hide column with "timestamp"
}

OverzichtVraagAanbod::~OverzichtVraagAanbod()
{
    delete ui;
}
