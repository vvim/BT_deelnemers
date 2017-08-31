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
    explicit OverzichtVraagAanbod(QSqlRelationalTableModel *_model_vraag_aanbod_overzicht, QSqlRelationalTableModel *_model_deelnemers, QWidget *parent = 0);
    ~OverzichtVraagAanbod();

private slots:
    void on_saveButton_clicked();

private:
    Ui::OverzichtVraagAanbod *ui;
    QSqlRelationalTableModel *model_vraag_aanbod_overzicht;
    ComboBoxDelegate *vraag_aanbod_combobox, *categories_combobox;
    CompleterDelegate *deelnemer_completer;
    OverzichtVraagAanbod *vraag_aanbod;
    QMap<QString, int> deelnemers_map;
    QMap<int, QString> id_map;
    void feedbackSuccess(QString message);
    void feedbackWarning(QString message);
};

#endif // OVERZICHTVRAAGAANBOD_H
