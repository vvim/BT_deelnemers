#include "overzichtvraagaanbod.h"
#include "ui_overzichtvraagaanbod.h"
#include <QTime>

#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"

OverzichtVraagAanbod::OverzichtVraagAanbod(QSqlRelationalTableModel *_model_vraag_aanbod_overzicht, QSqlRelationalTableModel *_model_deelnemers, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OverzichtVraagAanbod)
{
    ui->setupUi(this);

    ui->label_feedback->clear();
    ui->saveButton->setAutoFillBackground(true);
    ui->saveButton->setStyleSheet("background-color: rgb(255, 0, 0); color: rgb(255, 255, 255)");


    model_vraag_aanbod_overzicht = _model_vraag_aanbod_overzicht;

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
    int categorieIdx_t_categorie = model_categorie->fieldIndex("categorie"); // which column do we need?

    for ( int i = 0 ; i < model_categorie->rowCount() ; ++i )
    {
        categories.push_back(model_categorie->index( i ,categorieIdx_t_categorie).data().toString().toStdString());
    }

    categories_combobox = new ComboBoxDelegate(categories, this);

    deelnemer_completer = new CompleterDelegate(_model_deelnemers, this);

    int idIdx = model_vraag_aanbod_overzicht->fieldIndex("id");
    int timestampIdx = model_vraag_aanbod_overzicht->fieldIndex("timestamp");
    int deelnemerIdx = model_vraag_aanbod_overzicht->fieldIndex("deelnemer");
    int vraagIdx = model_vraag_aanbod_overzicht->fieldIndex("vraag");
    int categorieIdx_t_vraag_aanbod = model_vraag_aanbod_overzicht->fieldIndex("categorie");
    int inhoudIdx = model_vraag_aanbod_overzicht->fieldIndex("inhoud");

    ui->tableView->setModel(model_vraag_aanbod_overzicht);
    ui->tableView->setColumnHidden(idIdx,1); // hide column with "id"
    ui->tableView->setColumnHidden(timestampIdx,1); // hide column with "timestamp"
    ui->tableView->setItemDelegateForColumn(vraagIdx,vraag_aanbod_combobox);
    ui->tableView->setItemDelegateForColumn(categorieIdx_t_vraag_aanbod,categories_combobox);
    ui->tableView->setItemDelegateForColumn(deelnemerIdx,deelnemer_completer);

    // make delegates readable (maybe better to implement virtual sizeHint()
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setColumnWidth(vraagIdx,80);
    ui->tableView->setColumnWidth(categorieIdx_t_vraag_aanbod,150);
    ui->tableView->setColumnWidth(deelnemerIdx,150);
}

OverzichtVraagAanbod::~OverzichtVraagAanbod()
{
    delete ui;
    delete vraag_aanbod_combobox;
    delete categories_combobox;
    delete deelnemer_completer;
}

void OverzichtVraagAanbod::feedbackSuccess(QString message)
{
    // or if you want to include the date as well: QDateTime currentTime = QDateTime::currentDateTime();
    QTime currentTime = QTime::currentTime();
    QString message_with_timestamp = QString("%1 (%2)").arg(message).arg(currentTime.toString());
    ui->label_feedback->setText(message_with_timestamp);
    ui->label_feedback->setStyleSheet("font-style: italic; color: green");
}

void OverzichtVraagAanbod::feedbackWarning(QString message)
{
    QTime currentTime = QTime::currentTime();
    QString message_with_timestamp = QString("%1 (%2)").arg(message).arg(currentTime.toString());
    ui->label_feedback->setText(message_with_timestamp);
    ui->label_feedback->setStyleSheet("font-weight: bold; color: red");
}

void OverzichtVraagAanbod::feedbackNeutral(QString message)
{
    QTime currentTime = QTime::currentTime();
    QString message_with_timestamp = QString("%1 (%2)").arg(message).arg(currentTime.toString());
    ui->label_feedback->setText(message_with_timestamp);
    ui->label_feedback->setStyleSheet("");
}

void OverzichtVraagAanbod::on_saveButton_clicked()
{
    feedbackSuccess("succes klik!");
}
