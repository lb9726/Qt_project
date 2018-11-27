#-------------------------------------------------
#
# Project created by QtCreator 2017-05-15T09:27:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DFS_ADB
TEMPLATE = app

DEFINES += LINUX #LINUX WINDOWS

include(driver/qtuart.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    #menubutton.cpp \
    menuitem.cpp

HEADERS  += mainwindow.h \
    #menubutton.h \
    menuitem.h

FORMS    += mainwindow.ui \
    menuitem.ui

RESOURCES += \
    rc.qrc
