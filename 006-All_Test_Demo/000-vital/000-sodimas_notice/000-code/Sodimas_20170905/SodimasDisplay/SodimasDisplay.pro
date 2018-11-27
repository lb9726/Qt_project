# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =
QT += xml
QT += network

DEFINES += LINUX  #WINDOWS LINUX
DEFINES += CANOPEN
#DEFINES += CANOPEN


INCLUDEPATH += $$PWD/global/ \
            $$PWD/configure/ \
            $$PWD/component/ui/ \
            $$PWD/component/physic/ \
            $$PWD/component/logic/ \
            $$PWD/..

DEPENDPATH += $$PWD/ \
              $$PWD/..

QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-variable

HEADERS += \
    qmlitem/arrowitem.h \
    global/define.h \
    global/globalfun.h \
    globalsetting.h \
    configure/xmlparse.h \
    configure/themeparser.h \
    configure/mdpparser.h \
    component/ui/uibase.h \
    component/ui/desktopui.h \
    component/ui/messagebarui.h \
    component/ui/progressbarui.h \
    component/ui/floorui.h \
    component/ui/functionui.h \
    component/ui/mediaui.h \
    component/ui/logoui.h \
    component/ui/arrowui.h \
    component/physic/playerph.h \
    component/physic/ledph.h \
    component/physic/lcdph.h \
    component/physic/lancph.h \
    component/physic/comph.h \
    component/physic/physicbase.h \
    component/logic/logicbase.h \
    component/logic/themelg.h \
    component/ui/fulltextui.h \
    component/ui/realtimetextui.h \
    component/bootsplash.h \
    component/ui/wifilogo.h \
    component/ui/usblogo.h \
    util/media/mediaplayer.h \
    util/media/playerbase.h \
    util/media/wav_parser.h \
    component/customlogo.h \
    button/buttoncontroller.h \
    button/iddailer.h \
    button/backlightctrl.h \
    util/media/mplayer.h \
    wifi/appclient.h \
    wifi/qjson/FlexLexer.h \
    wifi/qjson/json_parser.hh \
    wifi/qjson/json_scanner.h \
    wifi/qjson/location.hh \
    wifi/qjson/parser.h \
    wifi/qjson/parser_p.h \
    wifi/qjson/parserrunnable.h \
    wifi/qjson/position.hh \
    wifi/qjson/qjson_debug.h \
    wifi/qjson/qjson_export.h \
    wifi/qjson/qobjecthelper.h \
    wifi/qjson/serializer.h \
    wifi/qjson/serializerrunnable.h \
    wifi/qjson/stack.hh





SOURCES += main.cpp \
    qmlitem/arrowitem.cpp \
    global/globalfun.cpp \
    globalsetting.cpp \
    configure/xmlparse.cpp \
    configure/themeparser.cpp \
    configure/mdpparser.cpp \
    component/ui/uibase.cpp \
    component/ui/desktopui.cpp \
    component/ui/messagebarui.cpp \
    component/ui/progressbarui.cpp \
    component/ui/floorui.cpp \
    component/ui/functionui.cpp \
    component/ui/mediaui.cpp \
    component/ui/logoui.cpp \
    component/ui/arrowui.cpp \
    component/physic/playerph.cpp \
    component/physic/ledph.cpp \
    component/physic/lcdph.cpp \
    component/physic/lancph.cpp \
    component/physic/comph.cpp \
    component/physic/physicbase.cpp \
    component/logic/logicbase.cpp \
    component/logic/themelg.cpp \
    component/ui/fulltextui.cpp \
    component/ui/realtimetextui.cpp \
    component/bootsplash.cpp \
    component/ui/wifilogo.cpp \
    component/ui/usblogo.cpp \
    util/media/mediaplayer.cpp \
    util/media/wav_parser.cpp \
    component/customlogo.cpp \
    button/buttoncontroller.cpp \
    button/iddailer.cpp \
    button/backlightctrl.cpp \
    util/media/mplayer.cpp \
    wifi/appclient.cpp \
    wifi/qjson/json_parser.cc \
    wifi/qjson/json_scanner.cc \
    wifi/qjson/json_scanner.cpp \
    wifi/qjson/parser.cpp \
    wifi/qjson/parserrunnable.cpp \
    wifi/qjson/qobjecthelper.cpp \
    wifi/qjson/serializer.cpp \
    wifi/qjson/serializerrunnable.cpp

RESOURCES += qml.qrc


contains(DEFINES,CANOPEN){

    HEADERS += \
    canopen/display.h \
    canopen/opencallback.h \
    canopen/openclient.h
    SOURCES += \
    canopen/opencallback.cpp \
    canopen/openclient.cpp \
    canopen/display.c


    unix:!macx: LIBS += -L$$PWD/lib -lcanfestival
    INCLUDEPATH += $$PWD/include
    DEPENDPATH += $$PWD/include
    unix:!macx: LIBS += -L$$PWD/lib -lcanfestival_unix

    INCLUDEPATH += $$PWD/include
    DEPENDPATH += $$PWD/include

    unix:!macx: LIBS += -L$$PWD/lib -lcanfestival_can_socket
    INCLUDEPATH += $$PWD/include
    DEPENDPATH += $$PWD/include

    LIBS            += -ldl
}

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick1applicationviewer/qtquick1applicationviewer.pri)

# Default rules for deployment.
#include(deployment.pri)

