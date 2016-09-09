#ifndef SDEELNEMERMARKER_H
#define SDEELNEMERMARKER_H

#include <QDebug>
#include "saddress.h"
#include "sdeelnemerindividu.h"

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
        //address = "";
        email = "";
        telnr = "";
        gsm = "";
        individu = false;

		// add later if useful:
		//    organisatie = false;
    }

    SDeelnemerMarker(int _id, double _lat, double _lng, QString _name, SAddress _address, QString _email, QString _telnr, QString _gsm)
    {
        id = _id; lat = _lat; lng = _lng; name = _name; address = _address;
        email = _email; telnr = _telnr; gsm = _gsm;
        individu = false;

		// add later if useful:
		//    organisatie = false;
    }

    void AddIndividu(SDeelnemerIndividu _individu)
    {
        individu = true;
        Individu = _individu;
    }

    QString getName()
    {
        if(individu)
            return Individu.getName();

        return name;
    }

    void PrintInformation()
    {
        qDebug() << ". type: DeelnemerMarker";
        qDebug() << "... id        :" << id;
        qDebug() << "... name      :" << name;
        qDebug() << "... getName() :" << getName();
        qDebug() << "... email     :" << email;
        qDebug() << "... telnr     :" << telnr;
        qDebug() << "... gsm       :" << gsm;
        qDebug() << "... (lat, lng):" << lat << lng;
        address.PrintInformation();
        if(individu)
            Individu.PrintInformation();
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

    QString captionForGoogleMapsInfoWindow()
    {
        return QString("%1<br/>%2").arg(getName()).arg(address.getAddress());
    }


    QString getNameAndAddress()
    {
        QString nameandaddress = getName();

        if(!nameandaddress.trimmed().isEmpty())
            nameandaddress.append(", ");

        nameandaddress.append(address.getAddress());

        return nameandaddress;
    }

    QString contactInformationInOneLine()
    {
        return QString("%1\t%2\t%3\t%4\t%5").arg(getName()).arg(email).arg(telnr).arg(gsm).arg(address.getAddress());
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
    SAddress address;
    QString email;
    QString telnr;
    QString gsm;

    bool individu;
    SDeelnemerIndividu Individu;

// add later if useful:
//
//    bool organisatie;
//    bool lid;
//    bool liduitgeschreven; // of deze struct can in de struct 'lid' genest zijn

//    SDeelnemerOrganisatie Organisatie;
//    SDeelnemerLid Lid;
//    SDeelnemerUitgeschrevenLid UitgeschrevenLid;
};


#endif // SDEELNEMERMARKER_H
