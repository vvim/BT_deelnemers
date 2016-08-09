#ifndef BUURTIJD_DEELNEMERS_H
#define BUURTIJD_DEELNEMERS_H

#include <QWidget>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlRelationalTableModel>
#include <QDataWidgetMapper>
#include <QComboBox>

namespace Ui {
class Buurtijd_deelnemers;
}

class Buurtijd_deelnemers : public QWidget
{
    Q_OBJECT

public:
    explicit Buurtijd_deelnemers(QWidget *parent = 0);
    ~Buurtijd_deelnemers();

private:
    void showError(const QSqlError &err);
    Ui::Buurtijd_deelnemers *ui;
    bool connectToDatabase();
    QSqlDatabase db;
    QSqlRelationalTableModel *model_deelnemers, *model_contactVoorkeur, *model_doelgroep, *model_domein, *model_geslacht, *model_hoeLerenKennen, *model_ingeschrevenDoor, *model_niveauNl, *model_soortDeelnemer, *model_statuut;
    QDataWidgetMapper *mapper;
    int contactVoorkeurIdx, doelgroepIdx, domeinIdx, geslachtIdx, hoeLerenKennenIdx, ingeschrevenDoorIdx, niveauNlIdx, soortDeelnemerIdx, statuutIdx;
    void mapComboboxAndTableModel(QComboBox *combobox, QSqlRelationalTableModel *model, QString table);

private slots:
    void ChangeRow(QModelIndex new_index);
};

#endif // BUURTIJD_DEELNEMERS_H
