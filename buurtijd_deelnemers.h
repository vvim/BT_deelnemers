#ifndef BUURTIJD_DEELNEMERS_H
#define BUURTIJD_DEELNEMERS_H

#include <QWidget>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>

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
    Ui::Buurtijd_deelnemers *ui;
    bool connectToDatabase();
    QSqlDatabase db;
};

#endif // BUURTIJD_DEELNEMERS_H
