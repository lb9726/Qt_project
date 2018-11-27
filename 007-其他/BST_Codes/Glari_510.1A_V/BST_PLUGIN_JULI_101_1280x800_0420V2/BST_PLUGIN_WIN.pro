#-------------------------------------------------
#
# Project created by QtCreator 2014-03-10T19:13:03
#
#-------------------------------------------------

QT       += core gui xml network

TARGET = BST_PLUGIN
TEMPLATE = app
CONFIG += thread

DEFINES +=  X86     # N329 AM335X DM3730 IMX6X X86
DEFINES +=  WINDOWS   # WINDOWS LINUX UBUNTU FEDERO
DEFINES +=  GCC     # GCC MSVC
DEFINES +=  SHARED     # STATIC SHARED

DEFINES +=  SIZE_1280x800    # SIZE_800x480  SIZE_800x600  SIZE_1024x768 SIZE_1280x800  SIZE_1366x768  SIZE_1920x540

DEFINES +=  DISPLAY \
            SURRPORT_RGB888

DEFINES +=  PIXBUFFER 
DEFINES += SURRPORT_XML
DEFINES += SURRPORT_NETWORK
DEFINES += MPLAYER

include(../BST_IDE/global/global.pri)
include(../BST_IDE/multimedia/multimedia.pri)
include(../BST_IDE/interface/interface.pri)
include(../BST_IDE/effect/effect.pri)
include(./plugin/plugin.pri)

SOURCES += main.cpp

