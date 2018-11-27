TEMPLATE = app

QT += qml quick widgets
QT       += core gui multimedia

SOURCES += main.cpp \
    musicplayer.cpp \
    volumebutton.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    volumebutton.h \
    musicplayer.h
