#include "overzichtvraagaanbod.h"
#include "ui_overzichtvraagaanbod.h"

#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"

OverzichtVraagAanbod::OverzichtVraagAanbod(QSqlRelationalTableModel *_model_vraag_aanbod_overzicht, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OverzichtVraagAanbod)
{
    ui->setupUi(this);

    // we should get this information from a database, but "vraag-aanbod" which is actually a Boolean, should be ok
    std::vector<std::string> vraag_aanbod_items_for_combobox;
    vraag_aanbod_items_for_combobox.push_back("Aanbod"); // value="0"
    vraag_aanbod_items_for_combobox.push_back("Vraag"); // value="1"
    vraag_aanbod_combobox = new ComboBoxDelegate(vraag_aanbod_items_for_combobox, this);

    QSqlRelationalTableModel *model_categorie = new QSqlRelationalTableModel();
    model_categorie->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_categorie->setTable("t_categorie");

    // Populate the model
    if (!model_categorie->select())
    {
        //vvimDebug() << "[TODO]" << "show error" << ;
        // !! TODO //showError(model_categorie->lastError());
        QMessageBox::critical(this, "Unable to lookup categories in database",
                    "Unable to lookup categories in database: " + model_categorie->lastError().text());
        return;
    }


    vvimDebug() << "\n\n\t!! categorie geladen:" << model_categorie->rowCount() <<"\n\n";
    // to get the categories, we should have a table T_CATEGORIES in the database >TODO<
    std::vector<std::string> categories;
    int categorieIdx = model_categorie->fieldIndex("categorie"); // which column do we need?

    for ( int i = 0 ; i < model_categorie->rowCount() ; ++i )
    {
        categories.push_back(model_categorie->index( i ,categorieIdx).data().toString().toStdString());
    }

    categories_combobox = new ComboBoxDelegate(categories, this);

    deelnemer_completer = new CompleterDelegate(this);

    model_vraag_aanbod_overzicht = _model_vraag_aanbod;
    ui->tableView->setModel(model_vraag_aanbod_overzicht);
    ui->tableView->setColumnHidden(0,1); // hide column with "id"
    ui->tableView->setColumnHidden(1,1); // hide column with "timestamp"
    ui->tableView->setItemDelegateForColumn(3,vraag_aanbod_combobox);
    ui->tableView->setItemDelegateForColumn(4,categories_combobox);
    ui->tableView->setItemDelegateForColumn(2,deelnemer_completer);
}

OverzichtVraagAanbod::~OverzichtVraagAanbod()
{
    delete ui;
    delete vraag_aanbod_combobox;
    delete categories_combobox;
    delete deelnemer_completer;
}
