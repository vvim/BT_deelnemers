#ifndef SDEELNEMERMARKER_H
#define SDEELNEMERMARKER_H

#include <QDebug>

// add later if useful:
//      #include sdeelnemerindividu.h
//      #include sdeelnemerorganisatie.h
//      #include sdeelnemerlid.h
//      #include sdeelnemeruitgeschrevenlid.h

//structure to save markers data (deelnemer)
struct SDeelnemerMarker
{
    SDeelnemerMarker()
    {
        id = -1;
        lat = 0;
        lng = 0;
        name = "";
        address = "";
		// add later if useful:
		//    individu = false;
		//    organisatie = false;
    }

    SDeelnemerMarker(int _id, double _lat, double _lng, QString _name, QString _address)
    {
        id = _id; lat = _lat; lng = _lng; name = _name; address = _address;
		// add later if useful:
		//    individu = false;
		//    organisatie = false;
    }

	/** add later if useful:
    SDeelnemerMarker(int _id, double _lat, double _lng, SDeelnemerIndividu _Individu)
    {
        id = _id; lat = _lat; lng = _lng; caption = _Individu.getNameAndAddress(); Individu = _Individu;
        individu = true; organisatie = false;
    }
    **/

    void PrintInformation()
    {
        qDebug() << "." << id << name << address << "(" << lat << lng << ")";
		/** add later if useful:
        if((!individu) && (!organisatie))
        {
            qDebug() << ". type: Adres";
        }
        if(organisatie)
        {
            Organisatie.PrintInformation();
        }
        **/
    }

    QString caption()
    {
        return QString("%1<br/>%2").arg(name).arg(address);
    }

	/** add later if useful:
    int getOrganisatieId()
    {
        if(!organisatie)
        {
            return -1;
        }

        return Organisatie.getOrganisatieId();
    }
    **/

    int id;
    double lat;
    double lng;
    QString name;
    QString address;

// add later if useful:
//    bool individu;
//    bool organisatie;
//    bool lid;
//    bool liduitgeschreven; // of deze struct can in de struct 'lid' genest zijn

//    SDeelnemerIndividu Individu;
//    SDeelnemerOrganisatie Organisatie;
//    SDeelnemerLid Lid;
//    SDeelnemerUitgeschrevenLid UitgeschrevenLid;
};


#endif // SDEELNEMERMARKER_H
