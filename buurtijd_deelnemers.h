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
#include <QMap>
#include "btlistview.h"
#include "btsqltablemodel.h"
#include "mylineedit.h"

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
    // as we do not use relations amongst the tables, maybe better to use the simpler QSqlTableModel ???
    QSqlRelationalTableModel *model_deelnemers, *model_contactVoorkeur, *model_geslacht, *model_hoeLerenKennen, *model_ingeschrevenDoor, *model_niveauNl, *model_soortDeelnemer, *model_statuut, *model_soort;
    BTSqlTableModel *model_doelgroep, *model_domein;
    QDataWidgetMapper *mapper;
    void mapComboboxAndTableModel(QComboBox *combobox, QSqlRelationalTableModel *model, QString table_name, int t_deelnemers_fieldindex);
    void mapListviewAndTableModel(BTListView *listview, BTSqlTableModel *model, QString table_name, int t_deelnemers_fieldindex);
    void showInformationForOfficialMember(bool make_visible);
    void showInformationForOfficialMemberHasQuit(bool make_visible);
    void showInformationForIndividual(bool make_visible);
    void showInformationForOrganisation(bool make_visible);
    MyCompleter *completer;
    void loadCompleter();
    QMap<QString, QModelIndex> deelnemers_map;
    QModelIndex last_known_index;
    bool deelnemer_information_changed;

private slots:
    void ChangeRow(QModelIndex new_index);
    void on_saveButton_clicked();
    void on_pushButton_showDeelnemer_clicked();
    void userChangesInformation();
};

#endif // BUURTIJD_DEELNEMERS_H
