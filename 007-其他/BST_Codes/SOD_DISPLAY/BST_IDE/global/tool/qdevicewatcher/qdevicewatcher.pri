INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/..

contains(DEFINES, SURRPORT_USBWATCHE)    {
    INCLUDEPATH += $$PWD/

    HEADERS +=  $$PWD/qdevicewatcher.h \
                $$PWD/qdevicechangeevent.h \
                $$PWD/qdevicewatcher_global.h \
                $$PWD/qdevicewatcher_p.h

    SOURCES +=  $$PWD/qdevicechangeevent.cpp \
                $$PWD/qdevicewatcher.cpp

    contains(DEFINES, WINDOWS) {
        SOURCES +=  $$PWD/qdevicewatcher_win32.cpp
    }
    contains(DEFINES, UBUNTU) {
        SOURCES +=  $$PWD/qdevicewatcher_linux.cpp
    }
    contains(DEFINES, LINUX) {
        SOURCES +=  $$PWD/qdevicewatcher_linux.cpp
    }
}

