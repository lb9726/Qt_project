INCLUDEPATH +=  $$PWD/

INCLUDEPATH += $$PWD/qdevicewatcher/

HEADERS +=  $$PWD/qdevicewatcher/qdevicewatcher.h \
            $$PWD/qdevicewatcher/qdevicewatcher_p.h

SOURCES +=  $$PWD/qdevicewatcher/qdevicewatcher.cpp

unix {
    HEADERS +=  $$PWD/UsbFormat/usbhelper_linux.h
    SOURCES +=  $$PWD/qdevicewatcher/qdevicewatcher_linux.cpp \
                $$PWD/UsbFormat/usbhelper_linux.cpp
}
else {
    win32 {
        SOURCES +=  $$PWD/qdevicewatcher/qdevicewatcher_win32.cpp
        INCLUDEPATH += $$PWD/UsbFormat/

            HEADERS +=  $$PWD/UsbFormat/usbeventfilter_win32.h \
                    $$PWD/UsbFormat/usbhelper_win32.h

            SOURCES +=  $$PWD/UsbFormat/usbeventfilter_win32.cpp \
                    $$PWD/UsbFormat/usbhelper_win32.cpp
    }
}


