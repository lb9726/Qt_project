#-------------------------------------------------
#
# Project created by QtCreator 2017-06-19T15:43:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testMultiUI
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    test.cpp \
    update.cpp

HEADERS  += widget.h \
    test.h \
    update.h

FORMS    += widget.ui \
    test.ui \
    update.ui
