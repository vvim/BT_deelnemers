#include "overzichtvraagaanbod.h"
#include "ui_overzichtvraagaanbod.h"

#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"

OverzichtVraagAanbod::OverzichtVraagAanbod(QSqlRelationalTableModel *_model_vraag_aanbod, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OverzichtVraagAanbod)
{
    ui->setupUi(this);
    vraag_aanbod_checkbox = new ComboBoxVADelegate(this);
    model_vraag_aanbod = _model_vraag_aanbod;
    ui->tableView->setModel(model_vraag_aanbod);
    ui->tableView->setColumnHidden(0,1); // hide column with "id"
    ui->tableView->setColumnHidden(1,1); // hide column with "timestamp"
    ui->tableView->setItemDelegateForColumn(3,vraag_aanbod_checkbox);
}

OverzichtVraagAanbod::~OverzichtVraagAanbod()
{
    delete ui;
}
