DEPENDPATH +=   $$PWD/ \
                $$PWD/..

INCLUDEPATH +=  $$PWD/ \
                $$PWD/graphics/ \
                $$PWD/eventmap/ \
                $$PWD/effect/ \
                $$PWD/setmenu/ \
                $$PWD/widget/ \
                $$PWD/edit/

contains(DEFINES, IDE) {
    SOURCES += $$PWD/uipage.cpp\
               $$PWD/diagramscene.cpp \
               $$PWD/uipageview.cpp \
               $$PWD/diagramcom.cpp \
               $$PWD/diagramframe.cpp \
               $$PWD/diagramsetting.cpp \
               $$PWD/propertytree.cpp

    HEADERS  += $$PWD/uipage.h\
                $$PWD/diagramscene.h\
                $$PWD/uipageview.h \
                $$PWD/diagramcom.h \
                $$PWD/diagramframe.h \
                $$PWD/diagramsetting.h \
                $$PWD/propertytree.h

    FORMS   += $$PWD/uipage.ui \
               $$PWD/diagramsetting.ui

    RESOURCES += $$PWD/graphics.qrc
}

contains(DEFINES, IDE)  {
    HEADERS += $$PWD/eventmap/eventmapform.h
    SOURCES += $$PWD/eventmap/eventmapform.cpp
    FORMS += $$PWD/eventmap/eventmapform.ui
}

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

    contains(DEFINES, SUPPORT_WIDGET) {
        HEADERS += $$PWD/uiwidget.h
        SOURCES += $$PWD/uiwidget.cpp
    }

    contains(DEFINES, SUPPORT_PLUGIN) {
        HEADERS += $$PWD/uiplugin.h
        SOURCES += $$PWD/uiplugin.cpp
    }

    #SettingMenu
    contains(DEFINES, SUPPORT_MENU) {
        contains(DEFINES, SUPPORT_TOUCH) {
            HEADERS +=  $$PWD/setmenuinc.h \
                        $$PWD/setmenu/setmenu.h \
                        $$PWD/setmenu/flrbtnmapbox.h
            SOURCES +=  $$PWD/setmenu/setmenu.cpp \
                        $$PWD/setmenu/flrbtnmapbox.cpp
            FORMS +=    $$PWD/setmenu/setmenu.ui \
                        $$PWD/setmenu/flrbtnmapbox.ui
            RESOURCES += $$PWD/setmenu/setmenu.qrc
        }
        contains(DEFINES, SUPPORT_KEY) {
            HEADERS +=  $$PWD/setmenu/settab.h \
                        $$PWD/setmenu/settabclass.h
            SOURCES +=  $$PWD/setmenu/settab.cpp \
                        $$PWD/setmenu/settabclass.cpp
            FORMS +=    $$PWD/setmenu/settab.ui
            RESOURCES += $$PWD/setmenu/settab.qrc
        }
    }
}

contains(DEFINES, SURRPORT_3D) {
    contains(DEFINES, WINDOWS) {
        INCLUDEPATH +=  $$PWD/graphics/OpenGL/Windows/include/
        LIBS += $$PWD/graphics/OpenGL/Windows/lib/glut32.lib \
                $$PWD/graphics/OpenGL/Windows/lib/GlU32.Lib \
                $$PWD/graphics/OpenGL/Windows/lib/lib3ds-1_3.lib
    }
    contains(DEFINES, UBUNTU) {
        INCLUDEPATH +=  $$PWD/graphics/OpenGL/Ubuntu/include/
        LIBS += -l3ds -lglut -lGLU  #GLU一定要大写
    }
    contains(DEFINES, LINUX) {
        INCLUDEPATH +=  $$PWD/graphics/OpenGL/Linux/include/
    }
}

contains(DEFINES, SUPPORT_SCENE) | contains(DEFINES, SUPPORT_COMEDIT) {
    HEADERS +=  $$PWD/graphics/graphicscomponent.h \
                $$PWD/graphics/graphicsmedia.h \
                $$PWD/graphics/graphicsregion.h \
                $$PWD/graphics/graphicsbutton.h \
                $$PWD/graphics/graphicsclock.h \
                $$PWD/graphics/graphicstext.h \
                $$PWD/graphics/graphicsarrow.h \
                $$PWD/graphics/graphicsfloor.h \
                $$PWD/graphics/graphicsmessage.h \
                $$PWD/graphics/graphicsprogress.h \
                $$PWD/graphics/graphicskeyboard.h \
                $$PWD/graphics/graphicswebview.h \
                $$PWD/graphics/graphicsqml.h \
                $$PWD/graphics/graphicsanimal.h \
                $$PWD/graphics/graphicsopengl.h \
                $$PWD/graphics/graphicsoperate.h \
                $$PWD/graphics/graphicsscene.h

    SOURCES +=  $$PWD/graphics/graphicscomponent.cpp \
                $$PWD/graphics/graphicsregion.cpp  \
                $$PWD/graphics/graphicsmedia.cpp \
                $$PWD/graphics/graphicsbutton.cpp \
                $$PWD/graphics/graphicsclock.cpp \
                $$PWD/graphics/graphicstext.cpp \
                $$PWD/graphics/graphicsarrow.cpp \
                $$PWD/graphics/graphicsfloor.cpp \
                $$PWD/graphics/graphicsmessage.cpp \
                $$PWD/graphics/graphicsprogress.cpp \
                $$PWD/graphics/graphicskeyboard.cpp \
                $$PWD/graphics/graphicswebview.cpp \
                $$PWD/graphics/graphicsqml.cpp \
                $$PWD/graphics/graphicsanimal.cpp \
                $$PWD/graphics/graphicsopengl.cpp \
                $$PWD/graphics/graphicsoperate.cpp \
                $$PWD/graphics/graphicsscene.cpp

    HEADERS  += $$PWD/graphicsinc.h
}

contains(DEFINES, SUPPORT_WIDGET) {
    contains(DEFINES, IMX6X) {
        INCLUDEPATH += $$PWD/widget/lib/uitools/IMX6/QtUiTools
        LIBS += $$PWD/widget/lib/uitools/IMX6/libQtUiTools.a
    }

    HEADERS +=  $$PWD/effect/effectbase.h
    SOURCES +=  $$PWD/effect/effectbase.cpp

    HEADERS +=  $$PWD/effectinc.h

    HEADERS +=  $$PWD/widget/stackedmenu.h \
                $$PWD/widget/widgetbase.h \
                $$PWD/widget/widgetregion.h \
                $$PWD/widget/widgetbutton.h \
                $$PWD/widget/widgetarrow.h \
                $$PWD/widget/widgetmedia.h \
                $$PWD/widget/widgetfloor.h \
                $$PWD/widget/widgetclock.h \
                $$PWD/widget/widgetmessage.h \
                $$PWD/widget/widgetprogress.h \
                $$PWD/widget/widgetstatus.h \
                $$PWD/widget/widgettext.h \
                $$PWD/widget/widgetweather.h \
                $$PWD/widget/widgetrencode.h


    SOURCES +=  $$PWD/widget/stackedmenu.cpp \
                $$PWD/widget/widgetbase.cpp \
                $$PWD/widget/widgetregion.cpp \
                $$PWD/widget/widgetbutton.cpp \
                $$PWD/widget/widgetarrow.cpp \
                $$PWD/widget/widgetmedia.cpp \
                $$PWD/widget/widgetfloor.cpp \
                $$PWD/widget/widgetclock.cpp \
                $$PWD/widget/widgetmessage.cpp \
                $$PWD/widget/widgetprogress.cpp \
                $$PWD/widget/widgetstatus.cpp \
                $$PWD/widget/widgettext.cpp \
                $$PWD/widget/widgetweather.cpp \
                $$PWD/widget/widgetrencode.cpp

    HEADERS +=  $$PWD/widgetinc.h

    HEADERS += $$PWD/setmenu/menubase.h \
               $$PWD/setmenu/menuclock.h \
               $$PWD/setmenu/menuflrbtn.h \
               $$PWD/setmenu/menuweather.h

    SOURCES += $$PWD/setmenu/menubase.cpp \
               $$PWD/setmenu/menuclock.cpp \
               $$PWD/setmenu/menuflrbtn.cpp \
               $$PWD/setmenu/menuweather.cpp

    FORMS +=   $$PWD/setmenu/menuclock.ui \
               $$PWD/setmenu/menuflrbtn.ui \
               $$PWD/setmenu/menuweather.ui
}
