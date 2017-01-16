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
#include <QSettings>
#include "btlistview.h"
#include "btsqltablemodel.h"
#include "mylineedit.h"
#include "deelnemer_location.h"
#include "deelnemernotes.h"
#include "newdeelnemerindividu.h"

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
    void loadCompleter();
    SDeelnemerMarker readDeelnemer();
    SAddress readAddress();
    SContacts readContacts();
    bool ThisRowContainsAnIndividual(int row);
    bool ThisRowContainsAnOrganisation(int row);
    bool UserMadeChangesToDeelnemer();
    bool UserMadeChangesToDeelnemerAddress();
    bool UserMadeChangesToDeelnemerContacts();
    bool UserMadeChangesToDeelnemerIndividu();
    void feedbackSuccess(QString message);
    void feedbackWarning(QString message);

    MyCompleter *completer;
    QMap<QString, QModelIndex> deelnemers_map;
    QModelIndex last_known_index;
    SDeelnemerMarker last_known_deelnemer;
    DeelnemerLocation *location;
    DeelnemerNotes *notes;
    NewDeelnemerIndividu *newindividu;
    QSettings *settings;

private slots:
    void ChangeRow(QModelIndex new_index);
    void on_saveButton_clicked();
    void on_pushButton_showDeelnemer_clicked();
    void on_pushButton_showMaps_clicked();
    void on_cancelButton_clicked();
    void on_pushButton_showNotes_clicked();
    void on_pushButton_copyContactInformation_clicked();
    void on_pushButton_CreateNewIndividu_clicked();
    void addNewIndividuToDatabase(QString naam,QString familienaam,QString straat,QString huisnr,QString busnr,QString postcode,QString plaats);
};

#endif // BUURTIJD_DEELNEMERS_H
