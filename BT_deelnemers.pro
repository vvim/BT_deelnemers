#-------------------------------------------------
#
# Project created by QtCreator 2016-08-02T16:37:29
#
#-------------------------------------------------

QT       += core gui webkit network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += ../qjson/include

TARGET = BT_deelnemers
TEMPLATE = app

linux* {

    SMTP_LIBRARY_LOCATION = $$PWD/../bluetiger9/build-SMTPEmail-Desktop-Debug

    LIBS += /usr/lib/x86_64-linux-gnu/qt4/plugins/sqldrivers/libqsqlmysql.so
    LIBS += /usr/lib/libqjson.so.0
    LIBS += -L$$SMTP_LIBRARY_LOCATION -lSMTPEmail
}

win32-g++ {
    DEFINES += GOOGLEMAPS_IN_BROWSER
    LIBS += ../qjson/build/lib/qjson0.dll
    LIBS += ../qjson/build/lib/libqjson.dll
    LIBS += C:\\Wim\\MySQL\\MySQLServer55\\lib\\libmysql.dll
    LIBS += C:\\Wim\\MySQL\\MySQLServer55\\lib\\libmysql.lib

# email support
#    SMTP_LIBRARY_LOCATION = $$PWD/../../../build-SMTPEmail-Desktop-Debug/
#
#    win32:CONFIG(release, debug|release): LIBS += -L$$SMTP_LIBRARY_LOCATION/release/ -lSMTPEmail
#    else:win32:CONFIG(debug, debug|release): LIBS += -L$$SMTP_LIBRARY_LOCATION/debug/ -lSMTPEmail
}

win32-msvc* {
    LIBS += ../qjson/build/lib/qjson0.lib
    LIBS += ../qjson/build/lib/libqjson.dll
    LIBS += C:\\Wim\\MySQL\\MySQLServer55\\lib\\libmysql.dll
    LIBS += C:\\Wim\\MySQL\\MySQLServer55\\lib\\libmysql.lib
}

SOURCES += main.cpp\
        buurtijd_deelnemers.cpp \
    btlistview.cpp \
    btsqltablemodel.cpp \
    mylineedit.cpp \
    deelnemer_location.cpp \
    deelnemernotes.cpp \
    notassortfilterproxymodel.cpp \
    newdeelnemerindividu.cpp \
    overzichtvraagaanbod.cpp \
    comboboxdelegate.cpp \
    deelnemerbhoverzicht.cpp

HEADERS  += buurtijd_deelnemers.h \
    btlistview.h \
    btsqltablemodel.h \
    mylineedit.h \
    deelnemer_location.h \
    sdeelnemermarker.h \
    deelnemernotes.h \
    notassortfilterproxymodel.h \
    saddress.h \
    sdeelnemerindividu.h \
    scontacts.h \
    newdeelnemerindividu.h \
    sdeelnemerorganisatie.h \
    overzichtvraagaanbod.h \
    comboboxdelegate.h \
    deelnemerbhoverzicht.h

FORMS    += buurtijd_deelnemers.ui \
    deelnemer_location.ui \
    deelnemernotes.ui \
    newdeelnemerindividu.ui \
    overzichtvraagaanbod.ui \
    deelnemerbhoverzicht.ui

RESOURCES += \
    buurtijd_deelnemers.qrc
