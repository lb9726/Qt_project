#-------------------------------------------------
#
# Project created by QtCreator 2014-10-21T20:05:22
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MineSweeper
TEMPLATE = app


SOURCES += main.cpp\
        ms_mainwindow.cpp \
    herodialog.cpp \
    myitem.cpp \
    myscene.cpp \
    playerdialog.cpp \
    setdialog.cpp

HEADERS  += ms_mainwindow.h \
    common.h \
    herodialog.h \
    myitem.h \
    myscene.h \
    playerdialog.h \
    setdialog.h

RESOURCES += \
    images.qrc \
    sounds.qrc
