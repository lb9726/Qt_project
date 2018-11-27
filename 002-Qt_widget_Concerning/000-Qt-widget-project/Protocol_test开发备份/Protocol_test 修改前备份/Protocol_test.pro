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

DISTFILES += \
    images/ArrowUp.png \
    images/emergency.png \
    images/logo.png \
    images/number0.png \
    images/number1.png \
    images/number2.png \
    images/number3.png \
    images/number4.png \
    images/number5.png \
    images/number6.png \
    images/number7.png \
    images/number8.png \
    images/number9.png \
    images/welcome_text.png \
    images/background.png \
    images/ArrowDn.png

RESOURCES += \
    images.qrc
