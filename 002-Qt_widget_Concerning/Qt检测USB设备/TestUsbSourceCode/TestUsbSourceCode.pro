#-------------------------------------------------
#
# Project created by QtCreator 2017-06-22T09:18:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestUsbSourceCode
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    qdevicewatcher.cpp \
    qdevicewatcher_linux.cpp

HEADERS  += widget.h \
    qdevicewatcher.h \
    qdevicewatcher_p.h

FORMS    += widget.ui
