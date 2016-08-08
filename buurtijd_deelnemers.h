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
    QSqlRelationalTableModel *model;
    QDataWidgetMapper *mapper;
    int contactVoorkeurIdx, doelgroepIdx, domeinIdx, geslachtIdx, hoeLerenKennenIdx, ingeschrevenDoorIdx, niveauNlIdx, soortDeelnemerIdx, statuutIdx;

};

#endif // BUURTIJD_DEELNEMERS_H
