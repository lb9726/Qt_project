INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/..

HEADERS += $$PWD/define.h \
            $$PWD/istacked.h \
	    ../BST_IDE/global/tooltcp.h \
    ../BST_IDE/global/xmlparser.h \
    ../BST_IDE/global/uiparser.h \
    ../BST_IDE/global/mdpparser.h \
    ../BST_IDE/global/filefunc.h

contains(DEFINES, WINDOWS) {
    LIBS  += $$PWD/lib/odbc32.lib\
             $$PWD/lib/odbccp32.lib\
             $$PWD/lib/winmm.lib\
             $$PWD/lib/usbd.lib\
             $$PWD/lib/setupapi.lib\
             $$PWD/lib/lmusbdll.lib\
             $$PWD/lib/wsock32.lib
}

SOURCES += $$PWD/global.cpp\
           $$PWD/globalfun.cpp \
            $$PWD/istacked.cpp \
    ../BST_IDE/global/tooltcp.cpp \
    ../BST_IDE/global/xmlparser.cpp \
    ../BST_IDE/global/uiparser.cpp \
    ../BST_IDE/global/mdpparser.cpp \
    ../BST_IDE/global/filefunc.cpp

HEADERS += $$PWD/global.h \
           $$PWD/globalfun.h

HEADERS += $$PWD/serialport.h
SOURCES += $$PWD/serialport.cpp

HEADERS += $$PWD/ilistview.h
SOURCES += $$PWD/ilistview.cpp

INCLUDEPATH += $$PWD/tool/qdevicewatcher/  \
                $$PWD/tool/

HEADERS +=  $$PWD/tool/qdevicewatcher/qdevicewatcher.h \
            $$PWD/tool/qdevicewatcher/qdevicechangeevent.h \
            $$PWD/tool/qdevicewatcher/qdevicewatcher_global.h \
            $$PWD/tool/qdevicewatcher/qdevicewatcher_p.h

SOURCES +=  $$PWD/tool/qdevicewatcher/qdevicechangeevent.cpp \
            $$PWD/tool/qdevicewatcher/qdevicewatcher.cpp

contains(DEFINES, WINDOWS) {
    SOURCES +=  $$PWD/tool/qdevicewatcher/qdevicewatcher_win32.cpp
}
contains(DEFINES, UBUNTU) {
    SOURCES +=  $$PWD/tool/qdevicewatcher/qdevicewatcher_linux.cpp
}
contains(DEFINES, LINUX) {
    SOURCES +=  $$PWD/tool/qdevicewatcher/qdevicewatcher_linux.cpp
}
