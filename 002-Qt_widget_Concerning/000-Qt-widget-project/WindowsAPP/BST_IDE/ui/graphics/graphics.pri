INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/..

contains(DEFINES, SURRPORT_3D) {
    contains(DEFINES, WINDOWS) {
        INCLUDEPATH +=  $$PWD/OpenGL/Windows/include/
        LIBS += $$PWD/OpenGL/Windows/lib/glut32.lib \
                $$PWD/OpenGL/Windows/lib/GlU32.Lib \
                $$PWD/OpenGL/Windows/lib/lib3ds-1_3.lib
    }
    contains(DEFINES, UBUNTU) {
        INCLUDEPATH +=  $$PWD/OpenGL/Ubuntu/include/
        LIBS += -l3ds -lglut -lGLU  #GLU一定要大写
    }
    contains(DEFINES, LINUX) {
        INCLUDEPATH +=  $$PWD/OpenGL/Linux/include/
    }
}

HEADERS +=  $$PWD/graphicsrc.h \
            $$PWD/graphicscomponent.h \
            $$PWD/graphicsaudio.h \
            $$PWD/graphicsvideo.h \
            $$PWD/graphicsregion.h \
            $$PWD/graphicsbutton.h \
            $$PWD/graphicsclock.h \
            $$PWD/graphicstext.h \
            $$PWD/graphicsarrow.h \
            $$PWD/graphicsfloor.h \
            $$PWD/graphicssetbar.h \
            $$PWD/graphicsmessage.h \
            $$PWD/graphicsprogress.h \
            $$PWD/graphicskeyboard.h \
            $$PWD/graphicsinc.h \
            $$PWD/graphicswebview.h \
            $$PWD/graphicsqml.h \
            $$PWD/graphicsanimal.h \
            $$PWD/graphicsopengl.h \
            $$PWD/graphicsoperate.h \
            $$PWD/graphicsscene.h


SOURCES +=  $$PWD/graphicsrc.cpp \
            $$PWD/graphicscomponent.cpp \
            $$PWD/graphicsregion.cpp  \
            $$PWD/graphicsaudio.cpp \
            $$PWD/graphicsvideo.cpp \
            $$PWD/graphicsbutton.cpp \
            $$PWD/graphicsclock.cpp \
            $$PWD/graphicstext.cpp \
            $$PWD/graphicsarrow.cpp \
            $$PWD/graphicsfloor.cpp \
            $$PWD/graphicssetbar.cpp \
            $$PWD/graphicsmessage.cpp \
            $$PWD/graphicsprogress.cpp \
            $$PWD/graphicskeyboard.cpp \
            $$PWD/graphicswebview.cpp \
            $$PWD/graphicsqml.cpp \
            $$PWD/graphicsanimal.cpp \
            $$PWD/graphicsopengl.cpp \
            $$PWD/graphicsoperate.cpp \
            $$PWD/graphicsscene.cpp

RESOURCES += \
    graphics/graphics.qrc


