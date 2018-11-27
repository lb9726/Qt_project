#-------------------------------------------------
#
# Project created by QtCreator 2015-08-19T10:42:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mine1_0
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myscence.cpp \
    myitem.cpp \
    newwiner.cpp \
    mygame.cpp \
    query.cpp \
    introduce.cpp

HEADERS  += mainwindow.h \
    myscence.h \
    myitem.h \
    newwiner.h \
    mygame.h \
    query.h \
    introduce.h

FORMS    += mainwindow.ui \
    newwiner.ui \
    mygame.ui \
    query.ui \
    introduce.ui

RESOURCES += \
    image.qrc
