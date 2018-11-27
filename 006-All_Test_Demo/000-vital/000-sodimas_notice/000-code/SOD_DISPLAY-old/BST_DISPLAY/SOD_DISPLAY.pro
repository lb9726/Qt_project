QT       += core gui

TARGET = SOD_DISPLAY
TEMPLATE = app
CONFIG += thread

greaterThan(QT_MAJOR_VERSION, 4) { QT += widgets }



DEFINES +=  SODIMAS
DEFINES +=  QT_V453 #QT_V453
DEFINES +=  N329     # N329 AM335X DM3730 IMX6X
DEFINES +=  LINUX  # WINDOWS LINUX UBUNTU FEDERO
DEFINES +=  GCC     # GCC MSVC
DEFINES +=  SHARED #STATIC SHARED
DEFINES +=  DISPLAY SURRPORT_RGB888 # LVDS输出格式：RGB565 RGB666 RGB888
DEFINES += SUPPORT_SCENE #SUPPORT_WIDGET # #SUPPORT_PLUGIN

DEFINES += SURRPORT_XML
DEFINES += SURRPORT_NETWORK
DEFINES += SURRPORT_MULTIMEDIA
DEFINES += OSS  # 视频驱动：QtAV MPLAYER GSTREAMER FFPLAY OSS ALSA

#DEFINES += SURRPORT_USBWATCHE
DEFINES += SURRPORT_ZIP
DEFINES += SURRPORT_TOOL

contains(DEFINES, SURRPORT_NETWORK)     { QT += network }
#contains(DEFINES, SURRPORT_QML)         { QT += declarative }

include(../BST_IDE/global/global.pri)
include(../BST_IDE/mdp/mdp.pri)
include(../BST_IDE/ui/ui.pri)

SOURCES += ../BST_DISPLAY/main.cpp




win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-SOD_DISPLAY-Desktop-Debug/release/ -lz
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-SOD_DISPLAY-Desktop-Debug/debug/ -lz
else:unix: LIBS += -L$$PWD/../build-SOD_DISPLAY-Desktop-Debug/ -lz

INCLUDEPATH += $$PWD/../build-SOD_DISPLAY-Desktop-Debug
DEPENDPATH += $$PWD/../build-SOD_DISPLAY-Desktop-Debug
