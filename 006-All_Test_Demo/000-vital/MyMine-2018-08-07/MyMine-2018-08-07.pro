#-------------------------------------------------
#
# Project created by QtCreator 2018-08-07T08:48:55
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyMine-2018-08-07
TEMPLATE = app

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
    introducemine.cpp \
    mygraphicspixmapitem.cpp \
    mygraphicsscene.cpp \
    myselfdefinegame.cpp \
    gamewinner.cpp \
    historywinner.cpp \
    mymsgbox.cpp

HEADERS  += mainwindow.h \
    introducemine.h \
    mygraphicspixmapitem.h \
    mygraphicsscene.h \
    myselfdefinegame.h \
    gamewinner.h \
    historywinner.h \
    logcat.h \
    mymsgbox.h

FORMS    += mainwindow.ui \
    introducemine.ui \
    myselfdefinegame.ui \
    gamewinner.ui \
    historywinner.ui \
    mymsgbox.ui

RESOURCES += \
    images.qrc
