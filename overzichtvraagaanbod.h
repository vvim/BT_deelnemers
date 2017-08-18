#ifndef OVERZICHTVRAAGAANBOD_H
#define OVERZICHTVRAAGAANBOD_H

#include <QDialog>
#include <QSqlRelationalTableModel>

namespace Ui {
class OverzichtVraagAanbod;
}

class OverzichtVraagAanbod : public QDialog
{
    Q_OBJECT

public:
    explicit OverzichtVraagAanbod(QSqlRelationalTableModel *_model_vraag_aanbod, QWidget *parent = 0);
    ~OverzichtVraagAanbod();

private:
    Ui::OverzichtVraagAanbod *ui;
    QSqlRelationalTableModel *model_vraag_aanbod;
};

#endif // OVERZICHTVRAAGAANBOD_H
