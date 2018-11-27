TEMPLATE = app

QT += qml quick widgets core gui

SOURCES += main.cpp \
    showgifpicture.cpp

RESOURCES += qml.qrc
TARGET = KPLayer

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES +=
RC_FILE += icon.rc

HEADERS += \
    showgifpicture.h
