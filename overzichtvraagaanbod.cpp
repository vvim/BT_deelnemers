#include "overzichtvraagaanbod.h"
#include "ui_overzichtvraagaanbod.h"
#include <QTime>
#include <QDebug>

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
}

OverzichtVraagAanbod::~OverzichtVraagAanbod()
{
    delete ui;
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
