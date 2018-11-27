INCLUDEPATH +=  $$PWD/

HEADERS += \
    $$PWD/zipfunction.h \
    $$PWD/zip/quazip.h \
    $$PWD/zip/quazipfile.h \
    $$PWD/zip/quazipnewinfo.h \
    $$PWD/zip/ioapi.h \
    $$PWD/zip/unzip.h \
    $$PWD/zip/zip.h \
    $$PWD/zipandunzip.h

SOURCES += \
    $$PWD/zipfunction.cpp \
    $$PWD/zip/quazip.cpp \
    $$PWD/zip/quazipfile.cpp \
    $$PWD/zip/quazipnewinfo.cpp \
    $$PWD/zip/ioapi.c \
    $$PWD/zip/unzip.c \
    $$PWD/zip/zip.c \
    $$PWD/zipandunzip.cpp

unix:!macx: LIBS += -L$$PWD/lib -lz

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
