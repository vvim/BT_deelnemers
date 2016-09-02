#ifndef DEELNEMERNOTES_H
#define DEELNEMERNOTES_H

#include <QWidget>
#include <QSqlRelationalTableModel>
#include <QDataWidgetMapper>
#include <QPushButton>
#include "notassortfilterproxymodel.h"

namespace Ui {
class DeelnemerNotes;
}

class DeelnemerNotes : public QWidget
{
    Q_OBJECT

public:
    explicit DeelnemerNotes(int _deelnemer_id, QSqlRelationalTableModel *_model_deelnemernotes, QWidget *parent = 0);
    ~DeelnemerNotes();

private:
    Ui::DeelnemerNotes *ui;
    QPushButton *newNoteButton, *removeNoteButton;
    int deelnemer_id;
    QSqlRelationalTableModel *model_deelnemernotes;
    QDataWidgetMapper *mapper;
    NotasSortFilterProxyModel *notasSortedModel;

private slots:
    void createNewNote();
    void removeSelectedNote();
};

#endif // DEELNEMERNOTES_H
