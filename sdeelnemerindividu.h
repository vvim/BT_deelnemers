#ifndef SDEELNEMERINDIVIDU_H
#define SDEELNEMERINDIVIDU_H

#include <QDebug>

//structure containing markers data when typeOf(deelnemer) = individu
struct SDeelnemerIndividu
{
    SDeelnemerIndividu()
    {
        voornaam = "";
        familienaam = "";
    }

    SDeelnemerIndividu(QString _voornaam, QString _familienaam)
    {
        voornaam = _voornaam;
        familienaam = _familienaam;
    }

    QString getName()
    {
        QString fullname = QString("%1 %2").arg(voornaam).arg(familienaam);
        return fullname.trimmed();
    }

    void PrintInformation()
    {
        qDebug() << "... subtype: Individu";
        qDebug() << "..... voornaam   :" << voornaam;
        qDebug() << "..... familienaam:" << familienaam;
    }

    QString voornaam;
    QString familienaam;
};

#endif // SDEELNEMERINDIVIDU_H
