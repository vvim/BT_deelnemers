#ifndef SDEELNEMERORGANISATIE_H
#define SDEELNEMERORGANISATIE_H

#include <QDebug>

//structure containing markers data when typeOf(deelnemer) = organisatie
struct SDeelnemerOrganisatie
{
    SDeelnemerOrganisatie()
    {
    }

/*    SDeelnemerOrganisatie(QString _xx)
    {
        xx = _xx;
    }
*/

    void PrintInformation()
    {
        qDebug() << "... subtype: Organisatie";
        qDebug() << "..... xx:" /* << xx */ ;
    }

    /*
    QString xx;
    */
};

#endif // SDEELNEMERORGANISATIE_H
