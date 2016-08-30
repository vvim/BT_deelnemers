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

    ui->listViewOfAllNotes->setModel(model_deelnemernotes);
    ui->listViewOfAllNotes->setModelColumn(model_deelnemernotes->fieldIndex("timestamp")); // do not show column 0 ('id') but column 1 ('timestamp')

    /*can't get this to work*/ model_deelnemernotes->setSort(model_deelnemernotes->fieldIndex("timestamp"),Qt::DescendingOrder); // descending order from field "timestamp"
    // subclass QSortFilterProxyModel or have a look at https://developer.qt.nokia.com/wiki/QSqlRelationalDelegate_subclass_that_works_with_QSqlRelationalTableModel

    mapper = new QDataWidgetMapper(this);
    mapper->setModel(model_deelnemernotes);

    mapper->addMapping(ui->textEditCurrentNote, model_deelnemernotes->fieldIndex("nota"));

    connect(ui->listViewOfAllNotes->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                mapper, SLOT(setCurrentModelIndex(QModelIndex)));


}

DeelnemerNotes::~DeelnemerNotes()
{
    delete ui;
    delete model_deelnemernotes;
    // if the program crashes when we delete model_deelnemernotes, then maybe it is still used in buurtijd_deelnemers.cpp ?
}
