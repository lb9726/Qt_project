TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    src/channel.c \
    src/closechannel.c \
    src/dataspec.c \
    src/decdecode.c \
    src/getoptv.c \
    src/hexdecode.c \
    src/hexdump.c \
    src/openchannel.c \
    src/putoptv.c \
    src/readpacket.c \
    src/sendpacket.c \
    src/todigit.c \
    src/version.c

HEADERS += \
    include/channel.h \
    include/error.h \
    include/ether.h \
    include/flags.h \
    include/getoptv.h \
    include/memory.h \
    include/number.h \
    include/putoptv.h \
    include/symbol.h \
    include/types.h \
    include/version.h
