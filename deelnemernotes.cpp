#include "deelnemernotes.h"
#include "ui_deelnemernotes.h"

DeelnemerNotes::DeelnemerNotes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeelnemerNotes)
{
    ui->setupUi(this);
}

DeelnemerNotes::~DeelnemerNotes()
{
    delete ui;
}
