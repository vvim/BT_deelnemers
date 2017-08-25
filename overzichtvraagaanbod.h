#ifndef OVERZICHTVRAAGAANBOD_H
#define OVERZICHTVRAAGAANBOD_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include <QSqlError>
#include <QMessageBox>
#include "comboboxdelegate.h"
#include "completerdelegate.h"

namespace Ui {
class OverzichtVraagAanbod;
}

class OverzichtVraagAanbod : public QDialog
{
    Q_OBJECT

public:
    explicit OverzichtVraagAanbod(QSqlRelationalTableModel *_model_vraag_aanbod_overzicht, QWidget *parent = 0);
    ~OverzichtVraagAanbod();

private:
    Ui::OverzichtVraagAanbod *ui;
    QSqlRelationalTableModel *model_vraag_aanbod_overzicht;
    ComboBoxDelegate *vraag_aanbod_combobox, *categories_combobox;
    CompleterDelegate *deelnemer_completer;
};

#endif // OVERZICHTVRAAGAANBOD_H
