#-------------------------------------------------
#
# Project created by QtCreator 2017-06-29T13:41:00
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = canopenDemo
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    open/display.c \
    open/openclient.cpp \
    open/opencallback.cpp

HEADERS  += widget.h \
    open/display.h \
    open/openclient.h \
    open/opencallback.h

FORMS    += widget.ui


unix:!macx: LIBS += -L$$PWD/lib/ -lcanfestival
INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
unix:!macx: LIBS += -L$$PWD/lib/ -lcanfestival_unix

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

unix:!macx: LIBS += -L$$PWD/lib/ -lcanfestival_can_socket
INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

LIBS            += -ldl
QMAKE_CXXFLAGS +=  -Wno-unused-parameter
