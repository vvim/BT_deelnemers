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
        vvimDebug() << "error selecting model_categorie" << model_categorie->lastError().text();
        QMessageBox::critical(this, "Unable to lookup categories in database",
                    "Unable to lookup categories in database: " + model_categorie->lastError().text());
        return;
    }

    vvimDebug() << "categorieën geladen voor overzicht t_vraag_aanbod:" << model_categorie->rowCount();
    std::vector<std::string> categories;
    int categorieIdx_t_categorie = model_categorie->fieldIndex("categorie"); // which column do we need?

    for ( int i = 0 ; i < model_categorie->rowCount() ; ++i )
    {
        categories.push_back(model_categorie->index( i ,categorieIdx_t_categorie).data().toString().toStdString());
    }
    categories_combobox = new ComboBoxDelegate(categories, this);
    delete model_categorie;

    QSqlRelationalTableModel *model_transactie_statussen = new QSqlRelationalTableModel();
    model_transactie_statussen->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_transactie_statussen->setTable("t_va_transactie_status");

    // Populate the model
    if (!model_transactie_statussen->select())
    {
        vvimDebug() << "error selecting model_transactie_statussen" << model_transactie_statussen->lastError().text();
        QMessageBox::critical(this, "Unable to lookup transactie statussen in database",
                    "Unable to lookup transactie statussen in database: " + model_transactie_statussen->lastError().text());
        return;
    }
    vvimDebug() << "transactie_statussen geladen voor overzicht t_vraag_aanbod:" << model_transactie_statussen->rowCount();
    std::vector<std::string> transactie_status;
    int transactiestatusIdx_t_va_transactie_status = model_transactie_statussen->fieldIndex("transactie_status"); // which column do we need?

    for ( int i = 0 ; i < model_transactie_statussen->rowCount() ; ++i )
    {
        QString status = QString("%1 %2").arg(i).arg(model_transactie_statussen->index( i ,transactiestatusIdx_t_va_transactie_status).data().toString());
        transactie_status.push_back(status.toStdString());
    }
    transactie_status_combobox = new ComboBoxDelegate(transactie_status, this);
    delete model_transactie_statussen;

    deelnemer_completer = new CompleterDelegate(_model_deelnemers, this);

    int idIdx = model_vraag_aanbod_overzicht->fieldIndex("id");
    int timestampIdx = model_vraag_aanbod_overzicht->fieldIndex("timestamp");
    int deelnemerIdx = model_vraag_aanbod_overzicht->fieldIndex("deelnemer");
    int vraagIdx = model_vraag_aanbod_overzicht->fieldIndex("vraag");
    int categorieIdx_t_vraag_aanbod = model_vraag_aanbod_overzicht->fieldIndex("categorie");
    int transactiestatusIdx_t_vraag_aanbod = model_vraag_aanbod_overzicht->fieldIndex("transactie_status");
    int inhoudIdx = model_vraag_aanbod_overzicht->fieldIndex("inhoud");

    ui->tableView->setModel(model_vraag_aanbod_overzicht);
    ui->tableView->setColumnHidden(idIdx,1); // hide column with "id"
    ui->tableView->setColumnHidden(timestampIdx,1); // hide column with "timestamp"
    ui->tableView->setItemDelegateForColumn(vraagIdx,vraag_aanbod_combobox);
    ui->tableView->setItemDelegateForColumn(categorieIdx_t_vraag_aanbod,categories_combobox);
    ui->tableView->setItemDelegateForColumn(transactiestatusIdx_t_vraag_aanbod,transactie_status_combobox);
    ui->tableView->setItemDelegateForColumn(deelnemerIdx,deelnemer_completer);

    // make delegates readable (maybe better to implement virtual sizeHint()
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setColumnWidth(vraagIdx,60);
    ui->tableView->setColumnWidth(categorieIdx_t_vraag_aanbod,120);
    ui->tableView->setColumnWidth(deelnemerIdx,140);
    ui->tableView->setColumnWidth(transactiestatusIdx_t_vraag_aanbod,120);
}

OverzichtVraagAanbod::~OverzichtVraagAanbod()
{
    delete ui;
    delete vraag_aanbod_combobox;
    delete categories_combobox;
    delete transactie_status_combobox;
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
    vvimDebug() << "saving";
    model_vraag_aanbod_overzicht->database().transaction();

    if(model_vraag_aanbod_overzicht->submitAll())
    {
        vvimDebug() << "submitAll is successful, committing";
        model_vraag_aanbod_overzicht->database().commit();
        feedbackSuccess("opgeslagen");
    }
    else
    {
        vvimDebug() << "submitAll FAILED, rollback";
        vvimDebug() << "error:" << model_vraag_aanbod_overzicht->lastError();
        model_vraag_aanbod_overzicht->database().rollback();
        feedbackWarning("Er ging iets mis, wijzigingen niet opgeslagen");
    }
}

void OverzichtVraagAanbod::on_cancelButton_clicked()
{
    vvimDebug() << "... Cancel button pressed" << "undo all changes";

    vvimDebug() << "... rolling back";
    if(!model_vraag_aanbod_overzicht->database().rollback())
    {
        vvimDebug() << "rollback FAILED" << model_vraag_aanbod_overzicht->lastError().text();
        feedbackWarning(QString("annuleren faalde: %1").arg(model_vraag_aanbod_overzicht->lastError().text()));
    }


    vvimDebug() << "... reload by using select() , see https://forum.qt.io/topic/2981/how-to-reload-the-tableview-to-reload-its-data/4";
    if(!model_vraag_aanbod_overzicht->select())
    {
        vvimDebug() << "select FAILED" << model_vraag_aanbod_overzicht->lastError().text();
        feedbackWarning(QString("annuleren is gelukt, maar daarna ging er iets mis bij het ophalen van de gegevens: %1").arg(model_vraag_aanbod_overzicht->lastError().text()));
    }

    feedbackNeutral("Bewerkingen annuleren is gelukt");
}
