INCLUDEPATH +=  $$PWD/

HEADERS += \
    common/define.h \
    $$PWD/themeparser.h \
    $$PWD/xmlparse.h \
    $$PWD/qdevicewatcher/usbaction.h \
    $$PWD/mdpparser.h

INCLUDEPATH += $$PWD/qdevicewatcher/

HEADERS +=  $$PWD/qdevicewatcher/qdevicewatcher.h \
            $$PWD/qdevicewatcher/qdevicechangeevent.h \
            $$PWD/qdevicewatcher/qdevicewatcher_global.h \
            $$PWD/qdevicewatcher/qdevicewatcher_p.h

SOURCES +=  $$PWD/qdevicewatcher/qdevicechangeevent.cpp \
            $$PWD/qdevicewatcher/qdevicewatcher.cpp \
    $$PWD/themeparser.cpp \
    $$PWD/xmlparse.cpp \
    $$PWD/qdevicewatcher/usbaction.cpp \
    $$PWD/mdpparser.cpp

linux {
    SOURCES +=  $$PWD/qdevicewatcher/qdevicewatcher_linux.cpp
} else {
    SOURCES +=  $$PWD/qdevicewatcher/qdevicewatcher_win32.cpp
}
