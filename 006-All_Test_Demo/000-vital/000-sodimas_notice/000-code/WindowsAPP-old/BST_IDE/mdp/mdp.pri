INCLUDEPATH += $$PWD/  \
               $$PWD/middleware/ \
               $$PWD/physical/ \
               $$PWD/logic/

DEPENDPATH +=  $$PWD/..

HEADERS +=  $$PWD/physical/srnsaveform.h
SOURCES +=  $$PWD/physical/srnsaveform.cpp

FORMS +=    $$PWD/physical/srnsaveform.ui

HEADERS +=  $$PWD/physical/physicalbase.h \
            $$PWD/physical/physicalprotocol.h \
            $$PWD/physical/physicalelevator.h \
            $$PWD/physical/physicalplayer.h \
            $$PWD/physical/physicallcd.h \
            $$PWD/physical/physicalnet.h \
            $$PWD/physical/physicalvp.h

SOURCES +=  $$PWD/physical/physicalbase.cpp \
            $$PWD/physical/physicalprotocol.cpp \
            $$PWD/physical/physicalelevator.cpp \
            $$PWD/physical/physicalplayer.cpp \
            $$PWD/physical/physicallcd.cpp \
            $$PWD/physical/physicalnet.cpp \
            $$PWD/physical/physicalvp.cpp

HEADERS +=  $$PWD/middleware/middlewareinc.h \
            $$PWD/middleware/middlewarebase.h
SOURCES +=  $$PWD/middleware/middlewarebase.cpp

HEADERS +=  $$PWD/logic/logicbase.h \
            $$PWD/logic/logicthememanager.h \
            $$PWD/logic/logiccontentmanager.h
SOURCES +=  $$PWD/logic/logicbase.cpp \
            $$PWD/logic/logicthememanager.cpp \
            $$PWD/logic/logiccontentmanager.cpp

contains(DEFINES, IDE) {
    HEADERS +=  $$PWD/treeitemclass.h \
                $$PWD/deviceclass.h

    SOURCES +=  $$PWD/treeitemclass.cpp \
                $$PWD/deviceclass.cpp
}

!contains(DEFINES, SETTING) {
    HEADERS +=  $$PWD/physical/physicalrtc.h \
                $$PWD/physical/physicaldisk.h \
                $$PWD/physical/physicalmem.h \
                $$PWD/physical/physicalled.h \
                $$PWD/physical/physicalwd.h \
                $$PWD/physical/physicaldial.h \
                $$PWD/physical/physicalcpu.h \
                $$PWD/physical/physicaltouch.h \
                $$PWD/physical/physicalinc.h

    SOURCES +=  $$PWD/physical/physicalrtc.cpp \
                $$PWD/physical/physicaldisk.cpp \
                $$PWD/physical/physicalmem.cpp \
                $$PWD/physical/physicalled.cpp \
                $$PWD/physical/physicalwd.cpp \
                $$PWD/physical/physicaldial.cpp \
                $$PWD/physical/physicaltouch.cpp \
                $$PWD/physical/physicalcpu.cpp

    HEADERS +=  $$PWD/logic/logicinc.h \
                $$PWD/logic/logicssh.h \
                $$PWD/logic/logicmonitor.h \
                $$PWD/logic/logicftp.h \
                $$PWD/logic/logicdlna.h \
                $$PWD/logic/logiccardreader.h \
                $$PWD/logic/logicbonjour.h \
                $$PWD/logic/logicnfcp2p.h

    SOURCES +=  $$PWD/logic/logicssh.cpp \
                $$PWD/logic/logicmonitor.cpp \
                $$PWD/logic/logicftp.cpp \
                $$PWD/logic/logicdlna.cpp \
                $$PWD/logic/logiccardreader.cpp \
                $$PWD/logic/logicbonjour.cpp \
                $$PWD/logic/logicnfcp2p.cpp
}

contains(DEFINES, SURRPORT_NFC){
    contains(DEFINES, GCC) {
        contains(DEFINES, WINDOWS) {
            LIBS += $$PWD/physical/nfc/Windows/lib/libnfc.dll.a \
                    $$PWD/physical/nfc/Windows/lib/libnfcutils.a
        }
        contains(DEFINES, UBUNTU) {
            LIBS += $$PWD/physical/nfc/Ubuntu/lib/libnfc.a \
                    $$PWD/physical/nfc/Ubuntu/lib/libnfc.so.4.0.0
        }
        contains(DEFINES, LINUX) {
            LIBS += $$PWD/physical/nfc/Linux/lib/libnfc.a \
                    $$PWD/physical/nfc/Linux/lib/libnfc.so.4.0.0
        }
    }
}

contains(DEFINES, DISPLAY) {
    HEADERS += $$PWD/devicemanager.h \
               $$PWD/bootlogo.h
    SOURCES += $$PWD/devicemanager.cpp \
               $$PWD/bootlogo.cpp
}

contains(DEFINES, IDE) {
    HEADERS +=  $$PWD/deviceparaform.h \
                $$PWD/deviceitem.h \
                $$PWD/mdppage.h \
                $$PWD/devicescene.h \
                $$PWD/bootgenform.h

    SOURCES +=  $$PWD/deviceparaform.cpp \
                $$PWD/mdppage.cpp \
                $$PWD/devicescene.cpp \
                $$PWD/deviceitem.cpp \
                $$PWD/bootgenform.cpp

    FORMS +=  $$PWD/deviceparaform.ui \
              $$PWD/mdppage.ui \
              $$PWD/bootgenform.ui

    RESOURCES += $$PWD/device.qrc
}


