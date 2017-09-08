#ifndef OVERZICHTVRAAGAANBOD_H
#define OVERZICHTVRAAGAANBOD_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include <QSqlError>
#include <QMessageBox>
#include <QComboBox>
#include <QDataWidgetMapper>
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
    void on_cancelButton_clicked();
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_pushButton_transactie_opslaan_clicked();

private:
    Ui::OverzichtVraagAanbod *ui;
    QSqlRelationalTableModel *model_vraag_aanbod_overzicht;
    ComboBoxDelegate *vraag_aanbod_combobox, *categories_combobox, *transactie_status_combobox;
    CompleterDelegate *deelnemer_completer;
    OverzichtVraagAanbod *vraag_aanbod;
    QMap<QString, int> deelnemers_map;
    QMap<int, QString> id_map;
    QDataWidgetMapper *mapper;

    void feedbackSuccess(QString message);
    void feedbackWarning(QString message);
    void feedbackNeutral(QString message);
    void toonOverzicht(bool overzicht_visible);
    void mapComboboxAndTableModel(QComboBox *combobox,QSqlRelationalTableModel *model, QString table_name, int t_deelnemers_fieldindex);
};

#endif // OVERZICHTVRAAGAANBOD_H
