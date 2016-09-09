#ifndef SADDRESS_H
#define SADDRESS_H

#include <QDebug>

//structure containing an address

struct SAddress
{
    SAddress()
    {
        street = "";
        housenr = "";
        busnr = "";
        postalcode = "";
        plaats = "";
        country = "";
    }

    SAddress(QString _street, QString _housenr, QString _busnr, QString _postalcode, QString _plaats, QString _country = "België")
    {
        street = _street;
        housenr = _housenr;
        busnr = _busnr;
        postalcode = _postalcode;
        plaats = _plaats;
        country = _country;
    }

    QString getAddress()
    {
        QString address = "";

        address.append(street).append(" ").append(housenr);

        if(!busnr.trimmed().isEmpty())
            address.append(" bus ").append(busnr);

        address.append(", ").append(postalcode).append(" ").append(plaats);

        return address;
    }

    QString getAddressAndCountry()
    {
        QString address = getAddress();

        address.append(", ").append(country);

        return address;
    }

    void PrintInformation()
    {
        qDebug() << ". type: Address";
        qDebug() << "... street     :" << street;
        qDebug() << "... housenr    :" << housenr;
        qDebug() << "... busnr      :" << busnr;
        qDebug() << "... postalcode :" << postalcode;
        qDebug() << "... plaats     :" << plaats;
        qDebug() << "... country    :" << country;
        qDebug() << "... ... getAddress() :" << getAddress();
        qDebug() << "... ... getAddressAndCountry() :" << getAddressAndCountry();
    }

    QString street;
    QString housenr;
    QString busnr;
    QString postalcode;
    QString plaats;
    QString country;
};



#endif // SADDRESS_H
