#-------------------------------------------------
#
# Project created by QtCreator 2017-05-27T10:24:15
#
#-------------------------------------------------

QT       += core gui testlib dbus
QT += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Protocol_test
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    protocoldeal.cpp \
    fileupdate.cpp \
    qdevicewatcher.cpp \
    qdevicewatcher_linux.cpp \
    showscreen.cpp

HEADERS  += widget.h \
    protocoldeal.h \
    fileupdate.h \
    qdevicewatcher.h \
    qdevicewatcher_p.h \
    showscreen.h

FORMS    += widget.ui \
    fileupdate.ui

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
    images/ArrowDn.png \
    images/bground.png

RESOURCES += \
    images.qrc
