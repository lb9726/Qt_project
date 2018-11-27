#-------------------------------------------------
#
# Project created by QtCreator 2017-04-27T21:42:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FileCopy
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    copythread.cpp

HEADERS  += mainwindow.h \
    copythread.h

FORMS    += mainwindow.ui
