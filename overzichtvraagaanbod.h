#ifndef OVERZICHTVRAAGAANBOD_H
#define OVERZICHTVRAAGAANBOD_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include <QSqlError>
#include <QMessageBox>
#include "comboboxdelegate.h"
#include "completerdelegate.h"
#include "mylineedit.h"

namespace Ui {
class OverzichtVraagAanbod;
}

class OverzichtVraagAanbod : public QDialog
{
    Q_OBJECT

public:
    explicit OverzichtVraagAanbod(QSqlRelationalTableModel *_model_vraag_aanbod_overzicht, QSqlRelationalTableModel *_model_deelnemers, QWidget *parent = 0);
    ~OverzichtVraagAanbod();
    void loadCompleter();

private:
    Ui::OverzichtVraagAanbod *ui;
    QSqlRelationalTableModel *model_vraag_aanbod_overzicht, *model_deelnemers;
    ComboBoxDelegate *vraag_aanbod_combobox, *categories_combobox;
    CompleterDelegate *deelnemer_completer;
    MyCompleter *completer;
    OverzichtVraagAanbod *vraag_aanbod;
    QMap<QString, int> deelnemers_map;
    QMap<int, QString> id_map;
};

#endif // OVERZICHTVRAAGAANBOD_H
