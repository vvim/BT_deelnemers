#include "overzichtvraagaanbod.h"
#include "ui_overzichtvraagaanbod.h"

#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"

OverzichtVraagAanbod::OverzichtVraagAanbod(QSqlRelationalTableModel *_model_vraag_aanbod, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OverzichtVraagAanbod)
{
    ui->setupUi(this);

    // we should get this information from a database, but "vraag-aanbod" which is actually a Boolean, should be ok
    std::vector<std::string> vraag_aanbod_items_for_combobox;
    vraag_aanbod_items_for_combobox.push_back("Aanbod"); // value="0"
    vraag_aanbod_items_for_combobox.push_back("Vraag"); // value="1"
    vraag_aanbod_combobox = new ComboBoxDelegate(vraag_aanbod_items_for_combobox, this);

    // to get the categories, we should have a table T_CATEGORIES in the database >TODO<
    std::vector<std::string> categories;
    categories.push_back("Administratie"); // value=0
    categories.push_back("Babysit"); // value=1
    categories.push_back("Boeken"); // value=2
    categories.push_back("Boodschappen"); // value=3
    categories.push_back("Computers & GSM"); // value=4
    categories.push_back("Cultuur"); // value=5
    categories.push_back("Dieren"); // value=6
    categories.push_back("Donatie aan deelnemers of het Sociaal Fonds"); // value=7
    categories.push_back("Donatie Boekenmarktje"); // value=8
    categories.push_back("Donatie Bureaumarktje"); // value=9
    categories.push_back("Donatie Kledingmarktje"); // value=10
    categories.push_back("Donatie Speelgoedmarktje"); // value=11
    categories.push_back("Donatie Voedingsmarktje"); // value=12
    categories.push_back("Feesten"); // value=13
    categories.push_back("Fietsen"); // value=14
    categories.push_back("Flyeren"); // value=15
    categories.push_back("Hergebruik"); // value=16
    categories.push_back("Huishoudelijke hulp"); // value=17
    categories.push_back("Huur tuinproject"); // value=18
    categories.push_back("Kleding & Creatieve vaardigheden"); // value=19
    categories.push_back("Kledingmarktje - bezoek aan kledingmarktje"); // value=20
    categories.push_back("Kledingmarktje - hulp"); // value=21
    categories.push_back("Klusjes & Verhuizen"); // value=22
    categories.push_back("Koken & Bakken"); // value=23
    categories.push_back("Lenen"); // value=24
    categories.push_back("Lokaal"); // value=25
    categories.push_back("Materiaal"); // value=26
    categories.push_back("Organisatie"); // value=27
    categories.push_back("Sociaal contact & Burenhulp"); // value=28
    categories.push_back("Speelgoedmarktje - bezoek"); // value=29
    categories.push_back("Speelgoedmarktje - hulp"); // value=30
    categories.push_back("Sport"); // value=31
    categories.push_back("Tolken & Vertalen"); // value=32
    categories.push_back("Tuin & Plantjes"); // value=33
    categories.push_back("Vervoer"); // value=34
    categories.push_back("Voedingsmarktje - bezoek aan voedingsmarktje"); // value=35
    categories.push_back("Voedingsmarktje - hulp"); // value=36
    categories.push_back("Voedingsmarktje - ophalen"); // value=37
    categories.push_back("Volkskeuken - bezoek aan volkskeuken"); // value=38
    categories.push_back("Volkskeuken - hulp"); // value=39
    categories.push_back("Vorming - huiswerkbegeleiding"); // value=40
    categories.push_back("Vorming - hulp sollicitatie"); // value=41
    categories.push_back("Vorming - Nederlands oefenen"); // value=42
    categories.push_back("Vorming - nieuwe taal inoefenen"); // value=43
    categories.push_back("Vorming - vorming allerhande"); // value=44
    categories.push_back("Workshop"); // value=45

    categories_combobox = new ComboBoxDelegate(categories, this);


    model_vraag_aanbod = _model_vraag_aanbod;
    ui->tableView->setModel(model_vraag_aanbod);
    ui->tableView->setColumnHidden(0,1); // hide column with "id"
    ui->tableView->setColumnHidden(1,1); // hide column with "timestamp"
    ui->tableView->setItemDelegateForColumn(3,vraag_aanbod_combobox);
    ui->tableView->setItemDelegateForColumn(4,categories_combobox);
}

OverzichtVraagAanbod::~OverzichtVraagAanbod()
{
    delete ui;
}
