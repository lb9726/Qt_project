#-------------------------------------------------
#
# Project created by QtCreator 2018-03-12T08:09:36
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestReleaseZipFile
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


INCLUDEPATH += $$PWD/zip/
HEADERS += $$PWD/zip/crypt.h \
           $$PWD/zip/ioapi.h \
           $$PWD/zip/quazip.h \
           $$PWD/zip/quazipfile.h \
           $$PWD/zip/quazipfileinfo.h \
           $$PWD/zip/quazipnewinfo.h \
           $$PWD/zip/unzip.h \
           $$PWD/zip/zconf.h \
           $$PWD/zip/zip.h \
           $$PWD/zip/zlib.h \
    zipfunction.h

SOURCES += $$PWD/zip/ioapi.c \
           $$PWD/zip/quazip.cpp \
           $$PWD/zip/quazipfile.cpp \
           $$PWD/zip/quazipnewinfo.cpp \
           $$PWD/zip/unzip.c \
           $$PWD/zip/zip.c \
    zipfunction.cpp

HEADERS += $$PWD/zipfun.h
SOURCES += $$PWD/zipfun.cpp


SOURCES += main.cpp\
        widget.cpp \
    global.cpp \
    testzip.cpp

HEADERS  += widget.h \
    define.h \
    global.h \
    testzip.h

FORMS    += widget.ui

#unix:!macx: LIBS += -L$$PWD/lib -lz

#INCLUDEPATH += $$PWD/.
#DEPENDPATH += $$PWD/.

unix:!macx: LIBS += -L$$PWD/lib_system -lz

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
