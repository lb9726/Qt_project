#-------------------------------------------------
#
# Project created by QtCreator 2018-01-15T19:42:47
#
#-------------------------------------------------

QT       += core gui uitools xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestRotate
TEMPLATE = app
DESTDIR = $$PWD/usrfs

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    mygraphicview.cpp \
    themeparser.cpp \
    uivideo.cpp \
    uitext.cpp \
    uiicon.cpp \
    uifloor.cpp \
    uidesktop.cpp \
    uiclock.cpp \
    uiarrow.cpp \
    xmlparse.cpp \
    bootscreen.cpp \
    cmysplashscreen.cpp

HEADERS  += mainwindow.h \
    mygraphicview.h \
    themeparser.h \
    uivideo.h \
    uitext.h \
    uiicon.h \
    uifloor.h \
    uidesktop.h \
    uiclock.h \
    uiarrow.h \
    xmlparse.h \
    define.h \
    bootscreen.h \
    cmysplashscreen.h

FORMS    += \
    theme.ui \
    vertical_Theme.ui \
    bootscreen.ui

DISTFILES += \
    ui.pri
