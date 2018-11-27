TEMPLATE = app

QT += core gui qml quick xml widgets
DEFINES += WINDOWS
CONFIG += c++11
CONFIG += X86 # X86 X64

RC_FILE = icon.rc

SOURCES += main.cpp \
    netclient.cpp \
    multmedia.cpp \
    pubulishpackage.cpp \
    exeupdate.cpp \
    templeteload.cpp \
    taid.cpp \
    util/filefunc.cpp \
    util/uiparser.cpp \
    util/xmlparser.cpp \
    uieditor.cpp \
    publish/bstsendupdate.cpp \
    publish/client.cpp \
    publish/clientread.cpp \
    publish/clientwrite.cpp \
    publish/readsession.cpp \
    publish/session.cpp \
    publish/tftp.cpp \
    publish/writesession.cpp \
    compressclient.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
#include(./lib/zip/zip.pri)
HEADERS += \
    netclient.h \
    multmedia.h \
    taid.h \
    pubulishpackage.h \
    exeupdate.h \
    templeteload.h \
    util/define.h \
    util/filefunc.h \
    util/uiparser.h \
    util/xmlparser.h \
    uieditor.h \
    publish/bstsendupdate.h \
    publish/client.h \
    publish/clientread.h \
    publish/clientwrite.h \
    publish/readsession.h \
    publish/session.h \
    publish/tftp.h \
    publish/utils.h \
    publish/writesession.h \
    compressclient.h

LIBS += -lWs2_32



DISTFILES += \
    icon.ico \
    icon.rc

X64{
    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/quazip64/ -lquazip
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/quazip64/ -lquazipd
    else:unix: LIBS += -L$$PWD/lib/quazip64/ -lquazip

    INCLUDEPATH += $$PWD/lib/quazip64
    DEPENDPATH += $$PWD/lib/quazip64
}

X86{
    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/quazip32/ -lquazip
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/quazip32/ -lquazipd
    else:unix: LIBS += -L$$PWD/lib/quazip32/ -lquazip

    INCLUDEPATH += $$PWD/lib/quazip32
    DEPENDPATH += $$PWD/lib/quazip32
}
