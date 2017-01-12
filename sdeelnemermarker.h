#ifndef SDEELNEMERMARKER_H
#define SDEELNEMERMARKER_H

#include <QDebug>
#include <QDate>
#include "saddress.h"
#include "scontacts.h"
#include "sdeelnemerindividu.h"
#include "sdeelnemerorganisatie.h"

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
        profile = "";
        lastcontact = QDate();
        when_best_to_contact = "";
        preferred_way_to_contact = -1;
        how_first_contact = -1;
        individu_present = false;

		// add later if useful:
		//    organisatie = false;

        address_present = false;
        contacts_present = false;
    }

    SDeelnemerMarker(int _id, double _lat, double _lng, QString _name, QString _profile,
                     QDate _lastcontact, QString _when_best_to_contact, int _preferred_way_to_contact,
                     int _how_first_contact, SAddress _Address, SContacts _Contacts)
    {
        id = _id; lat = _lat; lng = _lng; name = _name;
        profile = _profile;
        lastcontact = _lastcontact;
        when_best_to_contact = _when_best_to_contact;
        preferred_way_to_contact = _preferred_way_to_contact;
        how_first_contact = _how_first_contact;

        Address = _Address; Contacts = _Contacts;
        address_present = true; contacts_present = true;
        individu_present = false;

		// add later if useful:
		//    organisatie = false;
    }

    void AddIndividu(SDeelnemerIndividu _Individu)
    {
        organisatie_present = false; // mutually exclusive: a deelnemer is either an individual XOR an organisation
        individu_present = true;
        Individu = _Individu;
    }

    void AddOrganisatie(SDeelnemerOrganisatie _Organisatie)
    {
        individu_present = false; // mutually exclusive: a deelnemer is either an individual XOR an organisation
        organisatie_present = true;
        Organisatie = _Organisatie;
    }

    QString getName()
    {
        if(individu_present)
            return Individu.getName();

        return name;
    }

    void PrintInformation()
    {
        qDebug() << ". type: DeelnemerMarker";
        qDebug() << "... id        :" << id;
        qDebug() << "... name      :" << name;
        qDebug() << "... getName() :" << getName();
        qDebug() << "... (lat, lng):" << lat << lng;
        qDebug() << "... lastcontact:" << lastcontact.toString();
        qDebug() << "... how know BT:" << how_first_contact;
        qDebug() << "... best contact:" << preferred_way_to_contact << when_best_to_contact;
        qDebug() << "... profile:" << profile;

        if(address_present)
            Address.PrintInformation();
        if(contacts_present)
            Contacts.PrintInformation();
        if(individu_present)
            Individu.PrintInformation();
        if(organisatie_present)
            Organisatie.PrintInformation();
        /** add later if useful:
        if((!individu) && (!organisatie))
        {
            qDebug() << ". type: Adres";
        }
        **/
    }

    QString captionForGoogleMapsInfoWindow()
    {
        if(address_present)
            return QString("%1<br/>%2").arg(getName()).arg(Address.getAddress());
        else
            return QString("%1").arg(getName());
    }


    QString getNameAndAddress()
    {
        QString nameandaddress = getName();

        if(!nameandaddress.trimmed().isEmpty())
            nameandaddress.append(", ");

        if(address_present)
            nameandaddress.append(Address.getAddress());

        return nameandaddress;
    }

    QString contactInformationInOneLine()
    {
        QString contactinformation = getName();
        if(contacts_present)
            contactinformation.append(QString("\t%1\t%2\t%3\t%4").arg(Contacts.email1).arg(Contacts.email2).arg(Contacts.telnr).arg(Contacts.gsm));
        if(address_present)
            contactinformation.append(QString("\t%1").arg(Address.getAddress()));
        return contactinformation;
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
    QString profile;
    QDate lastcontact;
    QString when_best_to_contact;
    int preferred_way_to_contact;
    int how_first_contact;

    bool individu_present;
    SDeelnemerIndividu Individu;

    bool address_present;
    SAddress Address;
    bool contacts_present;
    SContacts Contacts;

    bool organisatie_present;
    SDeelnemerOrganisatie Organisatie;

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
