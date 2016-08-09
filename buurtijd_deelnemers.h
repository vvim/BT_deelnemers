#ifndef BUURTIJD_DEELNEMERS_H
#define BUURTIJD_DEELNEMERS_H

#include <QWidget>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlRelationalTableModel>
#include <QDataWidgetMapper>

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
    QSqlRelationalTableModel *model_deelnemers, *contactVoorkeur_model, *doelgroep_model, *domein_model, *geslacht_model, *hoeLerenKennen_model, *ingeschrevenDoor_model, *niveauNl_model, *soortDeelnemer_model, *statuut_model;
    QDataWidgetMapper *mapper;
    int contactVoorkeurIdx, doelgroepIdx, domeinIdx, geslachtIdx, hoeLerenKennenIdx, ingeschrevenDoorIdx, niveauNlIdx, soortDeelnemerIdx, statuutIdx;

private slots:
    void ChangeRow(QModelIndex new_index);
};

#endif // BUURTIJD_DEELNEMERS_H
