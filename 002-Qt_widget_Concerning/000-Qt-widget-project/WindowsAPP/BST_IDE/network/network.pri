INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/..

contains(DEFINES, SURRPORT_WINSDK) {
    #LIBS  += wlanapi/wlanapi.lib
    #INCLUDEPATH += $$PWD/wlanapi
    #直接使用指令
}

!contains(DEFINES, DISPLAY) {
    HEADERS += $$PWD/networkmanager.h \
               $$PWD/networkitem.h

    SOURCES += $$PWD/networkmanager.cpp \
               $$PWD/networkitem.cpp
}

contains(DEFINES, IDE) {
    HEADERS += $$PWD/bearercloud.h \
               $$PWD/cloud.h \
               $$PWD/networkmanagerform.h
    SOURCES += $$PWD/bearercloud.cpp \
               $$PWD/cloud.cpp \
               $$PWD/networkmanagerform.cpp
    FORMS +=   $$PWD/networkmanagerform.ui

    RESOURCES += $$PWD/network.qrc
}


