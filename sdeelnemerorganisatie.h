#ifndef SDEELNEMERORGANISATIE_H
#define SDEELNEMERORGANISATIE_H

#include <QDebug>

// I could make an enum class to save the gender of this individual, but
// as the UI works with a combobox, an 'int' seems more practical
//
//      enum class Geslacht { man, vrouw };

//structure containing markers data when typeOf(deelnemer) = individu
struct SDeelnemerOrganisatie
{
    SDeelnemerOrganisatie()
    {
    }

/*    SDeelnemerIndividu(QString _voornaam, QString _familienaam, int _geslacht, QDate _geboortedatum, QString _afkomst, int _statuut, int _niveauNederlands)
    {
        voornaam = _voornaam;
        familienaam = _familienaam;
        geslacht = _geslacht;
        geboortedatum = _geboortedatum;
        afkomst = _afkomst;
        statuut = _statuut;
        niveauNederlands = _niveauNederlands;
    }
*/

    void PrintInformation()
    {
        qDebug() << "... subtype: Organisatie";
        qDebug() << "..... idk:" /* << voornaam */ ;
    }

    QString voornaam;
    QString familienaam;
    int geslacht;
    QDate geboortedatum;
    QString afkomst;
    int statuut;
    int niveauNederlands;
};

#endif // SDEELNEMERORGANISATIE_H
