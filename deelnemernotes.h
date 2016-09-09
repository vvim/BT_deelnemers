#ifndef DEELNEMERNOTES_H
#define DEELNEMERNOTES_H

#include <QWidget>
#include <QSqlRelationalTableModel>
#include <QDataWidgetMapper>
#include <QPushButton>
#include "notassortfilterproxymodel.h"
#include "sdeelnemermarker.h"

namespace Ui {
class DeelnemerNotes;
}

class DeelnemerNotes : public QWidget
{
    Q_OBJECT

public:
    explicit DeelnemerNotes(SDeelnemerMarker _deelnemer, QSqlRelationalTableModel *_model_deelnemernotes, QWidget *parent = 0);
    ~DeelnemerNotes();

private:
    Ui::DeelnemerNotes *ui;
    QPushButton *newNoteButton, *removeNoteButton;
    SDeelnemerMarker deelnemer;
    QSqlRelationalTableModel *model_deelnemernotes;
    QDataWidgetMapper *mapper;
    NotasSortFilterProxyModel *notasSortedModel;
    bool SaveToDatabase();

private slots:
    void createNewNote();
    void removeSelectedNote();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // DEELNEMERNOTES_H
