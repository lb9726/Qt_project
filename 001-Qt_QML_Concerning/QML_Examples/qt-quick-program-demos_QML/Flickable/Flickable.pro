TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    fileupdate2.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    images/ArrowUp.png

HEADERS += \
    fileupdate2.h

