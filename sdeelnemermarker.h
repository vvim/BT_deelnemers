#ifndef SDEELNEMERMARKER_H
#define SDEELNEMERMARKER_H

#include <QDebug>
#include "saddress.h"

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
		// add later if useful:
		//    individu = false;
		//    organisatie = false;
    }

    SDeelnemerMarker(int _id, double _lat, double _lng, QString _name, SAddress _address, QString _email, QString _telnr, QString _gsm)
    {
        id = _id; lat = _lat; lng = _lng; name = _name; address = _address;
        email = _email; telnr = _telnr; gsm = _gsm;
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

    QString getName()
    {
        /*
          if (individual)
              return Individual.getName();
         */
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
        return QString("%1<br/>%2").arg(name).arg(address.getAddress());
    }


    QString getNameAndAddress()
    {
        QString nameandaddress = "";

        if(!name.trimmed().isEmpty())
            nameandaddress.append(name).append(", ");

        nameandaddress.append(address.getAddress());

        return nameandaddress;
    }

    QString contactInformationInOneLine()
    {
        return QString("%1\t%2\t%3\t%4\t%5").arg(name).arg(email).arg(telnr).arg(gsm).arg(address.getAddress());
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
