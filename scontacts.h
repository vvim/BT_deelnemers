#ifndef SCONTACTS_H
#define SCONTACTS_H

#include <QDebug>

//structure containing other contactinformation

struct SContacts
{
    SContacts()
    {
        email1 = "";
        email2 = "";
        telnr = "";
        gsm = "";
    }

    SContacts(QString _email1, QString _email2, QString _telnr, QString _gsm)
    {
        email1 = _email1;
        email2 = _email2;
        telnr = _telnr;
        gsm = _gsm;
    }

    void PrintInformation()
    {
        qDebug() << ". type: Contacts";
        qDebug() << "... email1     :" << email1;
        qDebug() << "... email2     :" << email2;
        qDebug() << "... telnr      :" << telnr;
        qDebug() << "... gsm        :" << gsm;
    }

    QString email1, email2, telnr, gsm;
};



#endif // SCONTACTS_H
