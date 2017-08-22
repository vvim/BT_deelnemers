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
#include "overzichtvraagaanbod.h"
#include "deelnemerbhoverzicht.h"

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
    QSqlRelationalTableModel *model_deelnemers, *model_contactVoorkeur, *model_geslacht, *model_hoeLerenKennen, *model_ingeschrevenDoor, *model_niveauNl, *model_soortDeelnemer, *model_statuut, *model_soort, *model_vraag_aanbod;
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
    bool UserMadeChangesToDeelnemerOrganisatie();
    void feedbackSuccess(QString message);
    void feedbackWarning(QString message);

    MyCompleter *completer;
    OverzichtVraagAanbod *vraag_aanbod;
    QMap<QString, QModelIndex> deelnemers_map;
    QModelIndex last_known_index;
    SDeelnemerMarker last_known_deelnemer;
    DeelnemerLocation *location;
    DeelnemerNotes *notes;
    NewDeelnemerIndividu *newindividu;
    DeelnemerBHOverzicht *bh_overzicht;
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
    void addNewParticipantToDatabase(QString naam, QString straat, QString huisnr, QString busnr, QString postcode, QString plaats, bool is_individu, QString familienaam);
    void updateNavigationalButtons(int row);
    void on_pushButton_previous_clicked();
    void on_pushButton_next_clicked();
    void keyPressEvent( QKeyEvent *k );
    void on_pushButton_lastcontact_is_today_clicked();
    void on_pushButton_first_clicked();
    void on_pushButton_last_clicked();
    void on_checkbox_lid_toggled(bool checked);
    void on_checkBox_was_lid_is_nu_gestopt_toggled(bool checked);
    void on_comboBox_soort_currentIndexChanged(int index);
    void on_pushButton_participantsWithoutEmail_clicked();
    void on_pushButton_vraag_aanbod_clicked();
    void on_pushButton_BH_clicked();
};

#endif // BUURTIJD_DEELNEMERS_H
