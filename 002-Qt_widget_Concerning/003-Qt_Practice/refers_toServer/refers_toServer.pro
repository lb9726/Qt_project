TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    serdata.c \
    server.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    serdata.h \
    server.h

DISTFILES += \
    chat.txt \
    personinf.txt

