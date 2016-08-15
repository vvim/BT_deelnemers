#-------------------------------------------------
#
# Project created by QtCreator 2016-08-02T16:37:29
#
#-------------------------------------------------

QT       += core gui network
QT           += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QTPLUGIN += qsqlmysql

TARGET = BT_deelnemers
TEMPLATE = app

LIBS += /usr/lib/x86_64-linux-gnu/qt4/plugins/sqldrivers/libqsqlmysql.so

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
