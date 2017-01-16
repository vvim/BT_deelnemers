#ifndef SDEELNEMERORGANISATIE_H
#define SDEELNEMERORGANISATIE_H

#include <QDebug>

//structure containing markers data when typeOf(deelnemer) = organisatie
struct SDeelnemerOrganisatie
{
    SDeelnemerOrganisatie()
    {
        contactpersoon_voornaam = "";
        contactpersoon_familienaam = "";
        vrijwilligersverzekering = false;
        doelgroep = "";
        domein = "";
    }

    SDeelnemerOrganisatie(QString _contactpersoon_voornaam, QString _contactpersoon_familienaam, bool _vrijwilligersverzekering, QString _doelgroep, QString _domein)
    {
        contactpersoon_voornaam = _contactpersoon_voornaam;
        contactpersoon_familienaam = _contactpersoon_familienaam;
        vrijwilligersverzekering = _vrijwilligersverzekering;
        doelgroep = _doelgroep;
        domein = _domein;
    }


    void PrintInformation()
    {
        qDebug() << "... subtype: Organisatie";
        qDebug() << "..... contactpersoon voornaam   :" << contactpersoon_voornaam;
        qDebug() << "..... contactpersoon familienaam:" << contactpersoon_familienaam;
        qDebug() << "..... vrijwilligersverzekering?:" << vrijwilligersverzekering;
        qDebug() << "..... doelgroep:" << doelgroep /** << getDoelgroep() **/ ;
        qDebug() << "..... domein   :" << domein /** << getDomein() **/ ;
    }

    QString contactpersoon_voornaam, contactpersoon_familienaam, doelgroep, domein;
    bool vrijwilligersverzekering;
};

#endif // SDEELNEMERORGANISATIE_H
