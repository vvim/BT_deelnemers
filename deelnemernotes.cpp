#include "deelnemernotes.h"
#include "ui_deelnemernotes.h"

#include <QDebug>

#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"

DeelnemerNotes::DeelnemerNotes(int _deelnemer_id, QSqlRelationalTableModel *_model_deelnemernotes, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeelnemerNotes)
{
    // get deelnemer_id from constructor
    deelnemer_id = _deelnemer_id;
    model_deelnemernotes = _model_deelnemernotes;

    ui->setupUi(this);
    ui->label_deelnemer_notes->setText(QString("Nota's van deelnemer %1").arg(_deelnemer_id));

    newNoteButton = new QPushButton(tr("Nieuwe Nota"));
    removeNoteButton = new QPushButton(tr("Wis Nota"));
    ui->buttonBox->addButton(newNoteButton,QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(removeNoteButton,QDialogButtonBox::DestructiveRole);

    notasSortedModel = new NotasSortFilterProxyModel(this);
    notasSortedModel->setDynamicSortFilter(true);
    notasSortedModel->setSourceModel(model_deelnemernotes);

    ui->listViewOfAllNotes->setModel(notasSortedModel);
    ui->listViewOfAllNotes->setModelColumn(model_deelnemernotes->fieldIndex("timestamp")); // do not show column 0 ('id') but column 1 ('timestamp')
    notasSortedModel->sort(model_deelnemernotes->fieldIndex("timestamp"), Qt::DescendingOrder);

    mapper = new QDataWidgetMapper(this);
    mapper->setModel(notasSortedModel);

    mapper->addMapping(ui->textEditCurrentNote, model_deelnemernotes->fieldIndex("nota"));

    connect(ui->listViewOfAllNotes->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                mapper, SLOT(setCurrentModelIndex(QModelIndex)));

    connect(newNoteButton, SIGNAL(pressed()), this, SLOT(createNewNote()));
    connect(removeNoteButton, SIGNAL(pressed()), this, SLOT(removeSelectedNote()));

    // renaming buttons from QButtonBox to make their use clear
    // see http://stackoverflow.com/a/31291548/
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Wijzigingen opslaan"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Wijzigingen annuleren"));
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
    vvimDebug() << "Creating new note";
}

void DeelnemerNotes::removeSelectedNote()
{
    QModelIndex index_from_id = notasSortedModel->index(ui->listViewOfAllNotes->currentIndex().row(),0);
    vvimDebug() << "Removing row" << ui->listViewOfAllNotes->currentIndex().row() << "table id:" << notasSortedModel->data(index_from_id).toInt() << ". Are you sure?";
}

void DeelnemerNotes::on_buttonBox_accepted()
{
    vvimDebug() << "OK button pressed" << "saving";

    model_deelnemernotes->database().transaction();

    if(model_deelnemernotes->submitAll())
    {
        vvimDebug() << "submitAll is successful, committing";
        model_deelnemernotes->database().commit();
        /* feedback
        QString feedback = QString(ui->le_naam->text());
        ui->label_feedback->setText(feedback.append(" opgeslagen"));
        ui->label_feedback->setStyleSheet("font-style: italic; color: green");
        */
    }
    else
    {
        vvimDebug() << "submitAll FAILED, rollback";
        model_deelnemernotes->database().rollback();
        /* feedback
        ui->label_feedback->setText("Er ging iets mis, wijzigingen niet opgeslagen");
        ui->label_feedback->setStyleSheet("font-weight: bold; color: red");
        */
    }
}

void DeelnemerNotes::on_buttonBox_rejected()
{
    vvimDebug() << "Cancel button pressed" << "undo changes";
    qDebug() << model_deelnemernotes->database().rollback();
    // reload by using select() , see https://forum.qt.io/topic/2981/how-to-reload-the-tableview-to-reload-its-data/4
    qDebug() << model_deelnemernotes->select();
}
