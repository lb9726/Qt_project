#-------------------------------------------------
#
# Project created by QtCreator 2017-10-14T19:32:59
#
#-------------------------------------------------

QT       += core gui uitools serialport xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += WINDOWS #WINDOWS LINUX

TARGET = TFT
TEMPLATE = app

DESTDIR = $$PWD/usrfs

DEPENDPATH += $$PWD/ \
              $$PWD/..

QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-variable

include(./driver/driver.pri)
include(./protocol/protocol.pri)
include(./ui/ui.pri)
include(./common/common.pri)
include(./media/media.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    splashform.cpp

HEADERS  += mainwindow.h \
    splashform.h

OTHER_FILES += \
    common/protocol.pri

FORMS += \
    splashform.ui
