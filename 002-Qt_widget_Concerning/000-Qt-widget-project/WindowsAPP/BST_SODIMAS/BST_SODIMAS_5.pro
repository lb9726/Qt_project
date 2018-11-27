#-------------------------------------------------
#
# Project created by QtCreator 2014-01-10T14:55:54
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SOD_SETTING
TEMPLATE = app
CONFIG += thread

DEFINES +=  SODIMAS  #BST SODIMAS KONE OTIS
DEFINES +=  WINDOWS  # WINDOWS LINUX UBUNTU PHONE
contains(DEFINES, PHONE) {
    DEFINES +=  ANDROID  # IOS BBOS
}
DEFINES +=  GCC     # GCC MSVC
DEFINES +=  SETTING

DEFINES +=  QT5   #QT4.8.2 QT4.5.3

contains(DEFINES, WINDOWS)         {
    CONFIG(release, debug|release) {
        DESTDIR = ./distribute/WinAPP/
    #如果是windows下的release编译代码，则直接将程序生成到EXE目录，然后修改“项目”中的“运行配置”为自定义到EXE下的BST_IDE.exe即可
    }
}

DEFINES +=  SUPPORT_LOCALTEST   #是否允许本机内部测试
DEFINES +=  SUPPORT_MUTEX   #是否使用Mutex实现资源互斥

DEFINES += SURRPORT_WINSDK

DEFINES += SURRPORT_XML
DEFINES += SURRPORT_NETWORK

DEFINES += SURRPORT_MULTIMEDIA
DEFINES += PHONON          # 视频驱动：QtAV PHONON MPLAYER GSTREAMER FFPLAY OSS ALSA QPLAYER

DEFINES += SURRPORT_USBWATCHE
DEFINES += SURRPORT_ZIP
DEFINES += SURRPORT_TOOL
#DEFINES += SURRPORT_LOG

contains(DEFINES, SURRPORT_MULTIMEDIA)  {
    QT += multimedia
}
contains(DEFINES, SURRPORT_NETWORK)     { QT += network }
contains(DEFINES, SURRPORT_WEBKIT)      { QT += webkit}

include(../BST_IDE/global/global.pri)
include(../BST_IDE/network/network.pri)
include(../BST_IDE/mdp/mdp.pri)
include(../BST_SODIMAS/setting_sodimas/setting.pri)

SOURCES += ../BST_SODIMAS/main.cpp

INCLUDEPATH += ../BST_SODIMAS/

TRANSLATIONS += ../BST_SODIMAS/SETTING.ts

CONFIG += mobility
MOBILITY =

VERSION = 1.0.0.7



