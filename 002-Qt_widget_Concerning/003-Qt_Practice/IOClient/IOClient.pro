TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += client.c \
    data_client.c \
    logic_client.c \
    ui_client.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    client.h \
    data_client.h \
    logic_client.h \
    data_client.h \
    logic_client.h \
    ui_client.h
    ui_client.h

