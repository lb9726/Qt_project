#-------------------------------------------------
#
# Project created by QtCreator 2017-06-15T16:52:43
#
#-------------------------------------------------

QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UsbMountTest
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    haldevice.cpp \
    usbmanager.cpp

HEADERS  += widget.h \
    haldevice.h \
    usbmanager.h
