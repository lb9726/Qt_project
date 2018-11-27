#-------------------------------------------------
#
# Project created by QtCreator 2017-05-27T10:24:15
#
#-------------------------------------------------

QT       += core gui testlib
QT += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Protocol_test
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    protocoldeal.cpp

HEADERS  += widget.h \
    protocoldeal.h

FORMS    += widget.ui
