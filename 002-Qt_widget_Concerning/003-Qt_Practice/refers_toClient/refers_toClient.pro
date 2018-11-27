TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.c \
    log.c \
    client.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    client.h \
    main.h \
    log.h

DISTFILES += \
    chat/libo.txt \
    chat/xiao.txt \
    chat/zhao.txt \
    chat/zhaox.txt \
    chat/zhx.txt \
    chat/赵鑫.txt

