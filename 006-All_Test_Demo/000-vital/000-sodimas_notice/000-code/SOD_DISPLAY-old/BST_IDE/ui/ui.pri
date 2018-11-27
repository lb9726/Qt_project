DEPENDPATH +=   $$PWD/ \
                $$PWD/..

INCLUDEPATH +=  $$PWD/ \
                $$PWD/graphics/ \
                $$PWD/eventmap

contains(DEFINES, IDE) | contains(DEFINES, SETTING) | contains(DEFINES, DISPLAY) {
    HEADERS += $$PWD/rccontainer.h
    SOURCES += $$PWD/rccontainer.cpp

    HEADERS += $$PWD/uicantainer.h
    SOURCES += $$PWD/uicantainer.cpp

    HEADERS += $$PWD/eventmap/eventmap.h
    SOURCES += $$PWD/eventmap/eventmap.cpp

    contains(DEFINES, SUPPORT_SCENE) {
        SOURCES += $$PWD/uiscene.cpp
        HEADERS  += $$PWD/uiscene.h
    }
}

contains(DEFINES, SUPPORT_SCENE) | contains(DEFINES, SUPPORT_COMEDIT) {
    HEADERS +=  $$PWD/graphics/graphicscomponent.h \
                $$PWD/graphics/graphicsmedia.h \
                $$PWD/graphics/graphicsregion.h \
#                $$PWD/graphics/graphicsbutton.h \
#                $$PWD/graphics/graphicsclock.h \
                $$PWD/graphics/graphicstext.h \
                $$PWD/graphics/graphicsarrow.h \
                $$PWD/graphics/graphicsfloor.h \
                $$PWD/graphics/graphicsmessage.h \
                $$PWD/graphics/graphicsprogress.h \
#                $$PWD/graphics/graphicskeyboard.h \
#                $$PWD/graphics/graphicswebview.h \
#                $$PWD/graphics/graphicsqml.h \
#                $$PWD/graphics/graphicsanimal.h \
#                $$PWD/graphics/graphicsopengl.h \
                $$PWD/graphics/graphicsoperate.h \
                $$PWD/graphics/graphicsscene.h

    SOURCES +=  $$PWD/graphics/graphicscomponent.cpp \
                $$PWD/graphics/graphicsregion.cpp  \
                $$PWD/graphics/graphicsmedia.cpp \
#                $$PWD/graphics/graphicsbutton.cpp \
#                $$PWD/graphics/graphicsclock.cpp \
                $$PWD/graphics/graphicstext.cpp \
                $$PWD/graphics/graphicsarrow.cpp \
                $$PWD/graphics/graphicsfloor.cpp \
                $$PWD/graphics/graphicsmessage.cpp \
                $$PWD/graphics/graphicsprogress.cpp \
#                $$PWD/graphics/graphicskeyboard.cpp \
#                $$PWD/graphics/graphicswebview.cpp \
#                $$PWD/graphics/graphicsqml.cpp \
#                $$PWD/graphics/graphicsanimal.cpp \
#                $$PWD/graphics/graphicsopengl.cpp \
                $$PWD/graphics/graphicsoperate.cpp \
                $$PWD/graphics/graphicsscene.cpp

    HEADERS  += $$PWD/graphicsinc.h
}
