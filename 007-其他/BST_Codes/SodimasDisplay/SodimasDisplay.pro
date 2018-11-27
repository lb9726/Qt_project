# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =
QT += xml
QT += network

DEFINES += LINUX  #WINDOWS LINUX

INCLUDEPATH += $$PWD/global/ \
            $$PWD/configure/ \
            $$PWD/component/ui/ \
            $$PWD/component/physic/ \
            $$PWD/component/logic/ \
            $$PWD/..

DEPENDPATH += $$PWD/ \
              $$PWD/..

# The .cpp file which was generated for your project. Feel free to hack it.


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
    util/media/wav_parser.h




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
    util/media/wav_parser.cpp


RESOURCES += qml.qrc

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick1applicationviewer/qtquick1applicationviewer.pri)

# Default rules for deployment.
#include(deployment.pri)

