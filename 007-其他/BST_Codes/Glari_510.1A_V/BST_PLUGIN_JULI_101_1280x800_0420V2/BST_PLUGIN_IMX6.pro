#-------------------------------------------------
#
# Project created by QtCreator 2014-03-10T19:13:03
#
#-------------------------------------------------

QT       += core gui xml network

TARGET = BST_DISPLAY
TEMPLATE = app
CONFIG += thread

DESTDIR = ../usrfs/EXE

DEFINES +=  IMX6X     # N329 AM335X DM3730 IMX6X X86
DEFINES +=  LINUX   # WINDOWS LINUX UBUNTU FEDERO
DEFINES +=  GCC     # GCC MSVC
DEFINES +=  SHARED     # STATIC SHARED

DEFINES +=  DISPLAY \
            SURRPORT_RGB888
DEFINES +=  PIXBUFFER
DEFINES += SURRPORT_XML
DEFINES += SURRPORT_NETWORK
DEFINES += MPLAYER VIDEO

DEFINES +=  SIZE_1280x800    # SIZE_800x480  SIZE_800x600  SIZE_1024x768 SIZE_1280x800  SIZE_1366x768  SIZE_1920x540

include(../BST_IDE/global/global.pri)
include(../BST_IDE/multimedia/multimedia.pri)
include(../BST_IDE/interface/interface.pri)
include(../BST_IDE/effect/effect.pri)
include(./plugin/plugin.pri)

INCLUDEPATH += /usr/include/gstreamer-0.10 \
               /usr/include/glib-2.0   \
               /usr/lib/glib-2.0/include  \
               /usr/include/libxml2
LIBS += -L/workspace/IMX6/rootfs/usr/lib/ -lgstreamer-0.10 -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lrt -lglib-2.0 -lz -lxml2


SOURCES += main.cpp

