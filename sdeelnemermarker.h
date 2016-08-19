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
        lat = 0;
        lng = 0;
        name = "";
        address = "";
		// add later if useful:
		//    individu = false;
		//    organisatie = false;
    };

    SDeelnemerMarker(double _lat, double _lng, QString _name, QString _address)
    {
        lat = _lat; lng = _lng; name = _name; address = _address;
		// add later if useful:
		//    individu = false;
		//    organisatie = false;
    };

	/** add later if useful:
    SDeelnemerMarker(double _lat, double _lng, SDeelnemerIndividu _Individu)
    {
        lat = _lat; lng = _lng; caption = _Individu.getNameAndAddress(); Individu = _Individu;
        individu = true; organisatie = false;
    };
    **/

    void PrintInformation()
    {
        qDebug() << "." << name << address << "(" << lat << lng << ")";
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
    };

    QString caption()
    {
        QString cap;
        cap = QString("%1<br/>%2").arg(name).arg(address);
        // escape QString to be used as a caption in JavaScript. Should not contain '
        // regular Qt::escape() from #include <QtGui/qtextdocument.h> will not escape the '
        // should we also check for \\\\' ? (as this could result in a \\' , escaping the \ but not the ')
        return cap.replace("'", "\\'");
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
