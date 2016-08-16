#-------------------------------------------------
#
# Project created by QtCreator 2016-08-02T16:37:29
#
#-------------------------------------------------

QT       += core gui webkit network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += ../qjson/include

QTPLUGIN += qsqlmysql

TARGET = BT_deelnemers
TEMPLATE = app

linux* {
    LIBS += /usr/lib/x86_64-linux-gnu/qt4/plugins/sqldrivers/libqsqlmysql.so
    LIBS += /usr/lib/libqjson.so.0
}

win32-g++ {
    LIBS += ../qjson/build/lib/qjson0.dll
    LIBS += ../qjson/build/lib/libqjson.dll
    #LIBS += ...\\MySQLServer55\\lib\\libmysql.dll
    #LIBS += ...\\MySQL\\MySQLServer55\\lib\\libmysql.lib
}

win32-msvc* {
    LIBS += ../qjson/build/lib/qjson0.lib
    LIBS += ../qjson/build/lib/libqjson.dll
    #LIBS += ...\\MySQLServer55\\lib\\libmysql.dll
    #LIBS += ...\\MySQL\\MySQLServer55\\lib\\libmysql.lib
}

SOURCES += main.cpp\
        buurtijd_deelnemers.cpp \
    btlistview.cpp \
    btsqltablemodel.cpp \
    mylineedit.cpp

HEADERS  += buurtijd_deelnemers.h \
    btlistview.h \
    btsqltablemodel.h \
    mylineedit.h

FORMS    += buurtijd_deelnemers.ui

RESOURCES += \
    buurtijd_deelnemers.qrc
