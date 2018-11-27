TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/main.c \
    src/raw_socket_channel.c

HEADERS += \
    inc/db_types.h \
    inc/raw_socket_channel.h
