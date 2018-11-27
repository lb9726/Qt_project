TEMPLATE = app

QT += qml quick serialport core gui
CONFIG += c++11

SOURCES += main.cpp \
    protocoldeal.cpp \
    qdevicewatcher.cpp \
    qdevicewatcher_linux.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    images/zn_lift_network.png \
    images/network.png \
    images/lift_network.png \
    images/kone_arrow_up.png \
    images/kone_arrow_null.png \
    images/kone_arrow_down.png

HEADERS += \
    protocoldeal.h \
    qdevicewatcher.h \
    qdevicewatcher_p.h

