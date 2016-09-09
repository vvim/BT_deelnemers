#include "deelnemernotes.h"
#include "ui_deelnemernotes.h"

#include <QDebug>
#include <QSqlError>
#include <QMessageBox>

#define NOTE_TIMESTAMP_ID 0
#define NOTE_TIMESTAMP_COLUMN 1
#define NOTE_TIMESTAMP_NOTE 2
#define NOTE_DEELNEMERID_COLUMN 3

#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"

DeelnemerNotes::DeelnemerNotes(SDeelnemerMarker _deelnemer, QSqlRelationalTableModel *_model_deelnemernotes, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeelnemerNotes)
{
    deelnemer = _deelnemer;
    if(deelnemer.id < 0)
    {
        vvimDebug() << "[ERROR] invalid deelnemer-id!" << deelnemer.id << "db will refuse when user tries to add notes";
    }
    model_deelnemernotes = _model_deelnemernotes;

    ui->setupUi(this);
    ui->label_deelnemer_notes->setText(QString("Nota's van deelnemer %1").arg(deelnemer.getName()));

    newNoteButton = new QPushButton(tr("Nieuwe Nota"));
    removeNoteButton = new QPushButton(tr("Wis Nota"));
    ui->buttonBox->addButton(newNoteButton,QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(removeNoteButton,QDialogButtonBox::DestructiveRole);

    notasSortedModel = new NotasSortFilterProxyModel(this);
    notasSortedModel->setDynamicSortFilter(true);
    notasSortedModel->setSourceModel(model_deelnemernotes);

    ui->listViewOfAllNotes->setModel(notasSortedModel);
    ui->listViewOfAllNotes->setModelColumn(NOTE_TIMESTAMP_COLUMN); // do not show column 0 ('id') but column 1 ('timestamp')
    notasSortedModel->sort(NOTE_TIMESTAMP_COLUMN, Qt::DescendingOrder);

    mapper = new QDataWidgetMapper(this);
    mapper->setModel(notasSortedModel);

    mapper->addMapping(ui->textEditCurrentNote, NOTE_TIMESTAMP_NOTE);

    connect(ui->listViewOfAllNotes->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                mapper, SLOT(setCurrentModelIndex(QModelIndex)));

    connect(newNoteButton, SIGNAL(pressed()), this, SLOT(createNewNote()));
    connect(removeNoteButton, SIGNAL(pressed()), this, SLOT(removeSelectedNote()));

    // renaming buttons from QButtonBox to make their use clear
    // see http://stackoverflow.com/a/31291548/
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Wijzigingen opslaan"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Wijzigingen annuleren"));

    if(model_deelnemernotes->rowCount() > 0)
    {
        vvimDebug() << "notes are present, so select the first one";
        ui->listViewOfAllNotes->setCurrentIndex(notasSortedModel->index(0,NOTE_TIMESTAMP_COLUMN));
    }
}

DeelnemerNotes::~DeelnemerNotes()
{
    delete newNoteButton;
    delete removeNoteButton;
    delete ui;
    delete notasSortedModel;
    delete model_deelnemernotes;
    // if the program crashes when we delete model_deelnemernotes, then maybe it is still used in buurtijd_deelnemers.cpp ?
}

void DeelnemerNotes::createNewNote()
{
    int row_to_insert = model_deelnemernotes->rowCount();
    vvimDebug() << "Creating new note at row" << row_to_insert;
    vvimDebug() << model_deelnemernotes->insertRow(row_to_insert);

    model_deelnemernotes->setData(model_deelnemernotes->index(row_to_insert,NOTE_TIMESTAMP_COLUMN),QDateTime::currentDateTime());
    model_deelnemernotes->setData(model_deelnemernotes->index(row_to_insert,NOTE_DEELNEMERID_COLUMN),deelnemer.id);

    SaveToDatabase(); // save newly created note

    /* we have to change selection to a valid row of QListView, else the user can start to type a
     * note, but it will be discarded even if the user clicks on "save note", because the selected
     * row will be '-1'.
     * -> see issue #9 https://github.com/vvim/BT_deelnemers/issues/9
     */
    ui->listViewOfAllNotes->setCurrentIndex(notasSortedModel->index(0,NOTE_TIMESTAMP_COLUMN));
}

void DeelnemerNotes::removeSelectedNote()
{
    QModelIndex index_from_id = notasSortedModel->index(ui->listViewOfAllNotes->currentIndex().row(),NOTE_TIMESTAMP_ID);
    vvimDebug() << "Removing row" << ui->listViewOfAllNotes->currentIndex().row() << "table id:" << notasSortedModel->data(index_from_id).toInt() << ". Are you sure?";
    if(ui->listViewOfAllNotes->currentIndex().row() < 0)
    {
        vvimDebug() << "rownr < 0 => not valid id";
        vvimDebug() << "id valid?" << notasSortedModel->data(index_from_id).isValid();
        vvimDebug() << "show messagebox";
        QMessageBox::information(this, tr("Selecteer nota om te verwijderen"),
                    tr("1. selecteer de nota die je wilt verwijderen\n2. druk op 'Wis Nota'"));
        return;
    }
    vvimDebug() << "id valid?" << notasSortedModel->data(index_from_id).isValid();
    vvimDebug() << "will request confirmation from the user before throwing this note into the fire";

    // feedback from the user requested -> http://stackoverflow.com/a/13111762/
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Nota verwijderen?"),
                                  tr("Deze actie kan niet ongedaan gemaakt worden. Weg is weg...\nBent u zeker dat u de geselecteerde nota wilt verwijderen?\n"),
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        vvimDebug() << "user confirmed to delete selected note";
        notasSortedModel->removeRow(ui->listViewOfAllNotes->currentIndex().row());
        // must commit to database;
        SaveToDatabase();

        /** after removing a note from the model, the note stays visible in the QTextEdit, and QListView's selected row is invalid (-1)
         **     vvimDebug() << "which row is selected now?" << ui->listViewOfAllNotes->currentIndex().row();
         **
         ** To avoid the user's confusion: better to clear the QTextEdit **/
        ui->textEditCurrentNote->clear();
    }
    else
    {
        vvimDebug() << "user said 'No', note will not be deleted";
    }
}

void DeelnemerNotes::on_buttonBox_accepted()
{
    vvimDebug() << "OK button pressed" << "saving";
    SaveToDatabase();
}

void DeelnemerNotes::on_buttonBox_rejected()
{
    vvimDebug() << "Cancel button pressed" << "undo changes";
    qDebug() << model_deelnemernotes->database().rollback();
    // reload by using select() , see https://forum.qt.io/topic/2981/how-to-reload-the-tableview-to-reload-its-data/4
    qDebug() << model_deelnemernotes->select();
}

bool DeelnemerNotes::SaveToDatabase()
{
    model_deelnemernotes->database().transaction();

    if(model_deelnemernotes->submitAll())
    {
        vvimDebug() << "submitAll is successful, committing";
        if(model_deelnemernotes->database().commit())
        {
            vvimDebug() << "commit is successful, done!";
            /* feedback
            QString feedback = QString(ui->le_naam->text());
            ui->label_feedback->setText(feedback.append(" opgeslagen"));
            ui->label_feedback->setStyleSheet("font-style: italic; color: green");
            */
            return true;
        }
        else
        {
            vvimDebug() << "[FAIL] commit failed";
            vvimDebug() << "error:" << model_deelnemernotes->lastError();
            return false;
        }
    }
    else
    {
        vvimDebug() << "[FAIL] submitAll failed, rollback";
        vvimDebug() << "error:" << model_deelnemernotes->lastError();
        model_deelnemernotes->database().rollback();
        /* feedback
        ui->label_feedback->setText("Er ging iets mis, wijzigingen niet opgeslagen");
        ui->label_feedback->setStyleSheet("font-weight: bold; color: red");
        */
    }
    return false;
}
