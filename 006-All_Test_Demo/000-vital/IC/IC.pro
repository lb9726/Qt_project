#-------------------------------------------------
#
# Project created by QtCreator 2016-08-08T21:23:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#RC_FILE = myico.rc

TARGET = IC
TEMPLATE = app

QT       += sql network

DEFINES += SURRPORT_SQL
DEFINES += SQLITE          # SQLITE MSSQL MYSQL SQLACCESS
include(./sqlserver/sqlserver.pri)

include(./icreader/icreader.pri)

include(./login/Login.pri)

SOURCES += main.cpp\
        icmanager.cpp \
    usritem.cpp \
    adminitem.cpp \
    carditem.cpp \
    icdata.cpp \
    flickcharm.cpp \
    managerdialog.cpp

HEADERS  += icmanager.h \
    usritem.h \
    adminitem.h \
    carditem.h \
    icdata.h \
    flickcharm.h \
    managerdialog.h

FORMS    += icmanager.ui \
    usritem.ui \
    adminitem.ui \
    carditem.ui \
    managerdialog.ui

RESOURCES += \
    ic.qrc

