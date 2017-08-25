#include "overzichtvraagaanbod.h"
#include "ui_overzichtvraagaanbod.h"

#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"

OverzichtVraagAanbod::OverzichtVraagAanbod(QSqlRelationalTableModel *_model_vraag_aanbod_overzicht, QSqlRelationalTableModel *_model_deelnemers, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OverzichtVraagAanbod)
{
    ui->setupUi(this);

    model_vraag_aanbod_overzicht = _model_vraag_aanbod_overzicht;
    model_deelnemers = _model_deelnemers;

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

    completer = NULL;
    loadCompleter();
    deelnemer_completer = new CompleterDelegate(deelnemers_map, id_map, this);

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

void OverzichtVraagAanbod::loadCompleter()
{
    if(completer)
        delete completer;

    deelnemers_map.clear();
    QStringList deelnemers_list;

    int idIdx = model_deelnemers->fieldIndex("id");
    int familieNaamIdx = model_deelnemers->fieldIndex("familienaam");
    int naamIdx = model_deelnemers->fieldIndex("naam");
    vvimDebug() << "[CAVEAT]"
                << "we expect the combination [naam] [familienaam] to be unique, but can we guarantee that?"
                << "else we could mix the address of telephonenumber in the mix?"
                << "currently I add there ID-number to make every entry unique";

    for ( int i = 0 ; i < model_deelnemers->rowCount() ; ++i )
    {
        QString dlnmr;
        if( model_deelnemers->index( i, familieNaamIdx ).data().isNull())
        {
            dlnmr = model_deelnemers->index( i, naamIdx ).data().toString();
        }
        else
        {
            dlnmr = model_deelnemers->index( i, naamIdx ).data().toString();
            dlnmr.append(" ");
            dlnmr.append(model_deelnemers->index( i, familieNaamIdx ).data().toString());
        }
        dlnmr = dlnmr.simplified();
        while(dlnmr.endsWith(" -"))
        {
            dlnmr.chop(2);
        }
        deelnemers_list << dlnmr;
        deelnemers_map[dlnmr] = idIdx;

        int id_deelnemer = model_deelnemers->index( i, idIdx ).data().toInt();
        id_map[id_deelnemer] = dlnmr;
    }

    deelnemers_list.sort();
    completer = new MyCompleter(deelnemers_list, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    vvimDebug() << "done, completer (re)loaded." << deelnemers_map.count() << id_map.count() << "rows";
}
