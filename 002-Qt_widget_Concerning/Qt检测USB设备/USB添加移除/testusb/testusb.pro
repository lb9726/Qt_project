#-------------------------------------------------
#
# Project created by QtCreator 2017-06-21T11:00:18
#
#-------------------------------------------------

QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testusb
TEMPLATE = app

INCLUDEPATH += /home/devin/Desktop/qdevicewatcher-master/src/
LIBS += -L /home/devin/Desktop/qdevicewatcher-master/out/lib_linux_ -lQDeviceWatcher
DESTDIR=$$/home/devin/Desktop/qdevicewatcher-master/out/lib_linux_/

SOURCES += main.cpp\
        widget.cpp \
    usbmanager.cpp \
    haldevice.cpp

HEADERS  += widget.h \
    usbmanager.h \
    haldevice.h

FORMS    += widget.ui
