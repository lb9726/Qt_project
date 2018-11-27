#-------------------------------------------------
#
# Project created by QtCreator 2017-07-24T09:49:08
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SplitBlockFileCopy
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    workthread.cpp

HEADERS  += widget.h \
    workthread.h \
    define.h


FORMS    += widget.ui
