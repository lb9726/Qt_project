contains(DEFINES, LINUX){
    HEADERS += \
        $$PWD/canopen/display.h \
        $$PWD/canopen/opencallback.h \
        $$PWD/canopen/openclient.h

    SOURCES += \
        $$PWD/canopen/opencallback.cpp \
        $$PWD/canopen/openclient.cpp \
        $$PWD/canopen/display.c

#    INCLUDEPATH += $$PWD/include \
#                    $$PWD/canopen
#    DEPENDPATH += $$PWD/include \
#                  $$PWD/

#    LIBS += -L$$PWD/lib -lcanfestival
#    LIBS += -L$$PWD/lib -lcanfestival_unix

#    LIBS += -L$$PWD/lib -lcanfestival_can_socket

#    LIBS += -L/opt/qt_4.8.6_imx6ul/tslib/lib
#    LIBS            += -ldl -lrt

    unix:!macx: LIBS += -L$$PWD/lib -lcanfestival

    INCLUDEPATH += $$PWD/include
    DEPENDPATH += $$PWD/include

    unix:!macx: PRE_TARGETDEPS += $$PWD/lib/libcanfestival.a

    unix:!macx: LIBS += -L$$PWD/lib -lcanfestival_unix

    INCLUDEPATH += $$PWD/include
    DEPENDPATH += $$PWD/include

    unix:!macx: PRE_TARGETDEPS += $$PWD/lib/libcanfestival_unix.a

    unix:!macx: LIBS += -L$$PWD/lib -lcanfestival_can_socket

    INCLUDEPATH += $$PWD/include
    DEPENDPATH += $$PWD/include

    LIBS += -ldl -lrt
}

HEADERS += \
    $$PWD/canopen/canopenclientfunction.h \
    $$PWD/saf/safthread.h

SOURCES += \
    $$PWD/canopen/canopenclientfunction.cpp \
    $$PWD/saf/safthread.cpp


