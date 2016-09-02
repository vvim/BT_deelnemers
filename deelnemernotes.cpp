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


}

DeelnemerNotes::~DeelnemerNotes()
{
    delete ui;
    delete model_deelnemernotes;
    // if the program crashes when we delete model_deelnemernotes, then maybe it is still used in buurtijd_deelnemers.cpp ?
}
