INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/..

contains(DEFINES, SURRPORT_USB)     {
    INCLUDEPATH +=  $$PWD/usb/  \
                    $$PWD/usb/lib/
}

!contains(DEFINES, IOS)     {
    SOURCES +=  $$PWD/serialport.cpp \
                $$PWD/modem.cpp
    HEADERS +=  $$PWD/serialport.h \
                $$PWD/modem.h
}

SOURCES +=  $$PWD/gifplayer.cpp \
            $$PWD/usbport.cpp \
            $$PWD/tooltcp.cpp \
            $$PWD/tooludp.cpp \
            $$PWD/toolhttp.cpp \
            $$PWD/upitem.cpp

HEADERS +=  $$PWD/gifplayer.h \
            $$PWD/usbport.h \
            $$PWD/tooltcp.h \
            $$PWD/tooludp.h \
            $$PWD/toolhttp.h \
            $$PWD/upitem.h

contains(DEFINES, SURRPORT_HIGHLIGHT)   {
    contains(DEFINES, WINDOWS) {
        INCLUDEPATH += $$PWD/highlight/Windows/include/
        LIBS += $$PWD/highlight/Windows/lib/libboost_regex-mgw44-1_46.a \
                $$PWD/highlight/Windows/lib/libsource-highlight3.a \
                $$PWD/highlight/Windows/lib/libsource-highlight-qt42.a
    }
    contains(DEFINES, UBUNTU) {
        INCLUDEPATH += $$PWD/highlight/Ubuntu/include/
        LIBS += -L$$PWD/highlight/Ubuntu/lib/ -lsource-highlight3 -lsource-highlight-qt42
    }
    contains(DEFINES, LINUX) {
        INCLUDEPATH += $$PWD/highlight/Linux/include/
        LIBS += -L$$PWD/highlight/Linux/lib/ -lsource-highlight3 -lsource-highlight-qt42
    }
}

SOURCES += $$PWD/highlighter.cpp \
           $$PWD/editorbasic.cpp

HEADERS  += $$PWD/highlighter.h \
            $$PWD/editorbasic.h

!contains(DEFINES, DISPLAY)     {
    SOURCES +=  $$PWD/completertool.cpp \
                $$PWD/findfileform.cpp

    HEADERS +=  $$PWD/completertool.h \
                $$PWD/findfileform.h

    FORMS   +=  $$PWD/findfileform.ui
    RESOURCES += $$PWD/completer.qrc

    SOURCES +=  $$PWD/screenshot.cpp
    HEADERS +=  $$PWD/screenshot.h

    SOURCES +=  $$PWD/toolbar.cpp \
                $$PWD/dieselinstrument.cpp\
                $$PWD/speedinstrument.cpp\
                $$PWD/timeinstrument.cpp \
                $$PWD/pieinstrument.cpp \
                $$PWD/catalogtreewidget.cpp \
                $$PWD/previewlabel.cpp \
                $$PWD/playlabel.cpp

    HEADERS +=  $$PWD/toolbar.h \
                $$PWD/dieselinstrument.h\
                $$PWD/speedinstrument.h\
                $$PWD/timeinstrument.h \
                $$PWD/pieinstrument.h \
                $$PWD/catalogtreewidget.h \
                $$PWD/previewlabel.h \
                $$PWD/playlabel.h

    RESOURCES += $$PWD/tool.qrc
}

contains(DEFINES, SURRPORT_WEBKIT) {
    SOURCES +=  $$PWD/webview.cpp
    HEADERS +=  $$PWD/webview.h
}

contains(DEFINES, SURRPORT_UDT)     {
    contains(DEFINES, WINDOWS) {
        contains(DEFINES, MSVC) {
            INCLUDEPATH += $$PWD/udt/Windows/include/
            HEADERS +=  $$PWD/tooludt.h
            SOURCES +=  $$PWD/tooludt.cpp
            LIBS += $$PWD/udt/Windows/lib/udt.lib  ws2_32.lib
        }
    }
    contains(DEFINES, UBUNTU) {
        HEADERS +=  $$PWD/tooludt.h
        SOURCES +=  $$PWD/tooludt.cpp
        LIBS += $$PWD/udt/Ubuntu/lib/libudt.a \
                $$PWD/udt/Ubuntu/lib/libudt.so
    }
    contains(DEFINES, LINUX) {
        HEADERS +=  $$PWD/tooludt.h
        SOURCES +=  $$PWD/tooludt.cpp
        LIBS += $$PWD/udt/Linux/lib/libudt.a \
                $$PWD/udt/Linux/lib/libudt.so
    }
}

contains(DEFINES, SURRPORT_PINYIN) {
    HEADERS +=  $$PWD/keyboard.h
    SOURCES +=  $$PWD/keyboard.cpp
    FORMS += $$PWD/keyboard.ui
    RESOURCES += $$PWD/keyboard.qrc
}

contains(DEFINES, SURRPORT_ACTIVEX) {
    HEADERS +=  $$PWD/axwidget.h
    SOURCES +=  $$PWD/axwidget.cpp
}

contains(DEFINES, SUPPORT_RENCODE)  {
    INCLUDEPATH += $$PWD/rencode/Linux/include/
    SOURCES +=  $$PWD/rencode.cpp
    HEADERS +=  $$PWD/rencode.h
    LIBS += /workspace/IMX6/libpng-1.2.52/lib/lib/libpng12.a
    #LIBS +=  -Wl,--rpath-link /workspace/IMX6/zlib-1.2.3/lib/lib -L/workspace/IMX6/zlib-1.2.3/lib/lib -lz -L/workspace/IMX6/libpng-1.6.14/lib/lib -lpng
}

