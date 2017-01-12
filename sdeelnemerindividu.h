#ifndef SDEELNEMERINDIVIDU_H
#define SDEELNEMERINDIVIDU_H

#include <QDebug>

// I could make an enum class to save the gender of this individual, but
// as the UI works with a combobox, an 'int' seems more practical
//
//      enum class Geslacht { man, vrouw };

//structure containing markers data when typeOf(deelnemer) = individu
struct SDeelnemerIndividu
{
    SDeelnemerIndividu()
    {
        voornaam = "";
        familienaam = "";
    }

    SDeelnemerIndividu(QString _voornaam, QString _familienaam, int _geslacht, QDate _geboortedatum, QString _afkomst, int _statuut, int _niveauNederlands)
    {
        voornaam = _voornaam;
        familienaam = _familienaam;
        geslacht = _geslacht;
        geboortedatum = _geboortedatum;
        afkomst = _afkomst;
        statuut = _statuut;
        niveauNederlands = _niveauNederlands;
    }

    QString getName()
    {
        QString fullname = QString("%1 %2").arg(voornaam).arg(familienaam);
        return fullname.trimmed();
    }

    QString getGeslacht()
    {
        if(geslacht == 0)
            return "man";
        else
            return "vrouw";
    }

    void PrintInformation()
    {
        qDebug() << "... subtype: Individu";
        qDebug() << "..... voornaam   :" << voornaam;
        qDebug() << "..... familienaam:" << familienaam;
        qDebug() << "..... geslacht:" << geslacht << getGeslacht();
        qDebug() << "..... geboortedatum:" << geboortedatum;
        qDebug() << "..... afkomst:" << afkomst;
        qDebug() << "..... statuut:" << statuut /** << getStatuut() **/;
        qDebug() << "..... niveau Nederlands:" << niveauNederlands /** << getNiveauNederlands() **/;
    }

    QString voornaam;
    QString familienaam;
    int geslacht;
    QDate geboortedatum;
    QString afkomst;
    int statuut;
    int niveauNederlands;
};

#endif // SDEELNEMERINDIVIDU_H
