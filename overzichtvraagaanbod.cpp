#include "overzichtvraagaanbod.h"
#include "ui_overzichtvraagaanbod.h"
#include <QTime>
#include <QDebug>
#include <QSqlRelationalDelegate>

#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"

OverzichtVraagAanbod::OverzichtVraagAanbod(QSqlRelationalTableModel *_model_vraag_aanbod_overzicht, QSqlRelationalTableModel *_model_deelnemers, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OverzichtVraagAanbod)
{
    ui->setupUi(this);
    toonOverzicht(true);

    ui->label_feedback->clear();
    ui->saveButton->setAutoFillBackground(true);
    ui->saveButton->setStyleSheet("background-color: rgb(255, 0, 0); color: rgb(255, 255, 255)");


    model_vraag_aanbod_overzicht = _model_vraag_aanbod_overzicht;

    int idIdx = model_vraag_aanbod_overzicht->fieldIndex("id");
    int timestampIdx = model_vraag_aanbod_overzicht->fieldIndex("timestamp");
    int deelnemerIdx = model_vraag_aanbod_overzicht->fieldIndex("deelnemer");
    int vraagIdx = model_vraag_aanbod_overzicht->fieldIndex("vraag");
    int categorieIdx= model_vraag_aanbod_overzicht->fieldIndex("categorie");
    int transactiestatusIdx = model_vraag_aanbod_overzicht->fieldIndex("transactie_status");
    int inhoudIdx = model_vraag_aanbod_overzicht->fieldIndex("inhoud");

    model_vraag_aanbod_overzicht->setRelation(categorieIdx,
                                              QSqlRelation("t_categorie","id", "categorie")  );
    model_vraag_aanbod_overzicht->setRelation(transactiestatusIdx,
                                              QSqlRelation("t_va_transactie_status","id", "transactie_status")  );
    model_vraag_aanbod_overzicht->setRelation(deelnemerIdx,
                                              QSqlRelation("t_deelnemers","id", "naam")  );
    model_vraag_aanbod_overzicht->select();

    ui->tableView->setModel(model_vraag_aanbod_overzicht);
    ui->tableView->setColumnHidden(idIdx,1); // hide column with "id"
    ui->tableView->setColumnHidden(timestampIdx,1); // hide column with "timestamp"

    // make delegates readable (maybe better to implement virtual sizeHint()
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setColumnWidth(vraagIdx,60);
    ui->tableView->setColumnWidth(categorieIdx,120);
    ui->tableView->setColumnWidth(deelnemerIdx,140);
    ui->tableView->setColumnWidth(transactiestatusIdx,120);

    QSqlTableModel *rel_categorie = model_vraag_aanbod_overzicht->relationModel(categorieIdx);
    ui->comboBox_categorie->setModel(rel_categorie);
    ui->comboBox_categorie->setModelColumn(rel_categorie->fieldIndex("categorie"));

    QSqlTableModel *rel_status = model_vraag_aanbod_overzicht->relationModel(transactiestatusIdx);
    ui->comboBox_status->setModel(rel_status);
    ui->comboBox_status->setModelColumn(rel_status->fieldIndex("transactie_status"));

    mapper = new QDataWidgetMapper(this);
    mapper->setModel(model_vraag_aanbod_overzicht);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(ui->le_initiator, deelnemerIdx);
    // combobox vraag/aanbod
    mapper->addMapping(ui->textEdit_inhoud, inhoudIdx);
    mapper->addMapping(ui->comboBox_categorie, categorieIdx);
    mapper->addMapping(ui->comboBox_status, transactiestatusIdx);
}

OverzichtVraagAanbod::~OverzichtVraagAanbod()
{
    delete ui;
    delete mapper;
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

void OverzichtVraagAanbod::on_tableView_doubleClicked(const QModelIndex &index)
{
    vvimDebug() << "show" << index ;
/*    vvimDebug() << "\n\nclickety on" << index ;
    int inhoudIdx = model_vraag_aanbod_overzicht->fieldIndex("inhoud");
    QString inhoud = model_vraag_aanbod_overzicht->index( index.row(), inhoudIdx).data().toString();
    vvimDebug() << inhoud << "\n\n";*/
    toonOverzicht(false);

    ui->label_feedback->clear();
    mapper->setCurrentModelIndex(index);
}

void OverzichtVraagAanbod::on_pushButton_transactie_opslaan_clicked()
{
    vvimDebug() << "pushed";
    toonOverzicht(true);
}

void OverzichtVraagAanbod::toonOverzicht(bool overzicht_visible)
{
    vvimDebug() << overzicht_visible;
    ui->groupBox->setVisible(!overzicht_visible);
    ui->tableView->setVisible(overzicht_visible);
    ui->saveButton->setVisible(overzicht_visible);
    ui->cancelButton->setVisible(overzicht_visible);
}

void OverzichtVraagAanbod::mapComboboxAndTableModel(QComboBox *combobox,QSqlRelationalTableModel *model, QString table_name, int t_deelnemers_fieldindex)
{
//////////////    zie http://doc.qt.io/archives/qq/qq21-datawidgetmapper.html


    model = new QSqlRelationalTableModel(combobox);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable(table_name);
    if (!model->select())
    {
////////////        showError(model->lastError());
        return;
    }
    combobox->setModel(model);
    combobox->setModelColumn(1);
    mapper->addMapping(combobox, t_deelnemers_fieldindex,"currentIndex");
}

