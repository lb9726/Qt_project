#-------------------------------------------------
#
# Project created by QtCreator 2017-06-21T15:05:53
#
#-------------------------------------------------

TARGET = Addlibtest
TEMPLATE = lib

DEFINES += ADDLIBTEST_LIBRARY

SOURCES += addlibtest.cpp

HEADERS += addlibtest.h\
        addlibtest_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
