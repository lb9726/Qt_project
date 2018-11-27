INCLUDEPATH += $$PWD/.. \
               $$PWD/ \

DEPENDPATH += $$PWD/..

QT += svg xml
contains(DEFINES, IDE) { QT += xmlpatterns }

HEADERS += $$PWD/define.h \
           $$PWD/globalfun.h
SOURCES += $$PWD/globalfun.cpp

contains(DEFINES, WINDOWS) {
    LIBS  += $$PWD/lib/odbc32.lib\
             $$PWD/lib/odbccp32.lib\
             $$PWD/lib/winmm.lib\
             $$PWD/lib/setupapi.lib\
             $$PWD/lib/wsock32.lib \
             $$PWD/lib/ws2_32.lib\
             $$PWD/lib/kernel32.lib\
             $$PWD/lib/user32.lib\
             $$PWD/lib/gdi32.lib\
             $$PWD/lib/winspool.lib\
             $$PWD/lib/comdlg32.lib\
             $$PWD/lib/advapi32.lib\
             $$PWD/lib/lmusbdll.lib\
             $$PWD/lib/usbd.lib \
             $$PWD/lib/Psapi.lib \
             $$PWD/lib/libiphlpapi.a \
             $$PWD/lib/Mpr.lib
}

contains(DEFINES, SURRPORT_LOG)
{
    INCLUDEPATH += $$PWD/log/  \
                   $$PWD/log/helpers \
                   $$PWD/log/log/  \
                   $$PWD/log/spi/ \
                   $$PWD/log/varia/ \
                   $$PWD/log/android/

    contains(DEFINES, ANDROID) {
        HEADERS += $$PWD/log/android/logcat.h
        SOURCES += $$PWD/log/android/logcat.cpp
    }
    !contains(DEFINES, QT5) {
        HEADERS +=  $$PWD/log/appender.h \
                    $$PWD/log/appenderskeleton.h \
                    $$PWD/log/basicconfigurator.h \
                    $$PWD/log/consoleappender.h \
                    $$PWD/log/dailyrollingfileappender.h \
                    $$PWD/log/fileappender.h \
                    $$PWD/log/helpers/classlogger.h \
                    $$PWD/log/helpers/configuratorhelper.h \
                    $$PWD/log/helpers/datetime.h \
                    $$PWD/log/helpers/factory.h \
                    $$PWD/log/helpers/initialisationhelper.h \
                    $$PWD/log/helpers/logerror.h \
                    $$PWD/log/helpers/logobject.h \
                    $$PWD/log/helpers/logobjectptr.h \
                    $$PWD/log/helpers/optionconverter.h \
                    $$PWD/log/helpers/patternformatter.h \
                    $$PWD/log/helpers/properties.h \
                    $$PWD/log/hierarchy.h \
                    $$PWD/log/layout.h \
                    $$PWD/log/level.h \
                    $$PWD/log/log4qt.h \
                    $$PWD/log/logger.h \
                    $$PWD/log/loggerrepository.h \
                    $$PWD/log/loggingevent.h \
                    $$PWD/log/logmanager.h \
                    $$PWD/log/mdc.h \
                    $$PWD/log/ndc.h \
                    $$PWD/log/patternlayout.h \
                    $$PWD/log/propertyconfigurator.h \
                    $$PWD/log/rollingfileappender.h \
                    $$PWD/log/simplelayout.h \
                    $$PWD/log/spi/filter.h \
                    $$PWD/log/ttcclayout.h \
                    $$PWD/log/writerappender.h \
                    $$PWD/log/varia/debugappender.h \
                    $$PWD/log/varia/denyallfilter.h \
                    $$PWD/log/varia/nullappender.h \
                    $$PWD/log/varia/levelmatchfilter.h \
                    $$PWD/log/varia/levelrangefilter.h \
                    $$PWD/log/varia/listappender.h \
                    $$PWD/log/varia/stringmatchfilter.h

        SOURCES +=  $$PWD/log/appenderskeleton.cpp \
                    $$PWD/log/basicconfigurator.cpp \
                    $$PWD/log/consoleappender.cpp \
                    $$PWD/log/dailyrollingfileappender.cpp \
                    $$PWD/log/fileappender.cpp \
                    $$PWD/log/helpers/classlogger.cpp \
                    $$PWD/log/helpers/configuratorhelper.cpp \
                    $$PWD/log/helpers/datetime.cpp \
                    $$PWD/log/helpers/factory.cpp \
                    $$PWD/log/helpers/initialisationhelper.cpp \
                    $$PWD/log/helpers/logerror.cpp \
                    $$PWD/log/helpers/logobject.cpp \
                    $$PWD/log/helpers/logobjectptr.cpp \
                    $$PWD/log/helpers/optionconverter.cpp \
                    $$PWD/log/helpers/patternformatter.cpp \
                    $$PWD/log/helpers/properties.cpp \
                    $$PWD/log/hierarchy.cpp \
                    $$PWD/log/layout.cpp \
                    $$PWD/log/level.cpp \
                    $$PWD/log/log4qt.cpp \
                    $$PWD/log/logger.cpp \
                    $$PWD/log/loggerrepository.cpp \
                    $$PWD/log/loggingevent.cpp \
                    $$PWD/log/logmanager.cpp \
                    $$PWD/log/mdc.cpp \
                    $$PWD/log/ndc.cpp \
                    $$PWD/log/patternlayout.cpp \
                    $$PWD/log/propertyconfigurator.cpp \
                    $$PWD/log/rollingfileappender.cpp \
                    $$PWD/log/simplelayout.cpp \
                    $$PWD/log/spi/filter.cpp \
                    $$PWD/log/ttcclayout.cpp \
                    $$PWD/log/writerappender.cpp \
                    $$PWD/log/varia/debugappender.cpp \
                    $$PWD/log/varia/denyallfilter.cpp \
                    $$PWD/log/varia/nullappender.cpp \
                    $$PWD/log/varia/levelmatchfilter.cpp \
                    $$PWD/log/varia/levelrangefilter.cpp \
                    $$PWD/log/varia/listappender.cpp \
                    $$PWD/log/varia/stringmatchfilter.cpp
    }

    HEADERS += $$PWD/logfun.h
    SOURCES += $$PWD/logfun.cpp
}

contains(DEFINES, SURRPORT_ZIP) {
    INCLUDEPATH += $$PWD/zip/

    contains(DEFINES, GCC) {
        HEADERS += $$PWD/zip/crypt.h \
                   $$PWD/zip/ioapi.h \
                   $$PWD/zip/quazip.h \
                   $$PWD/zip/quazipfile.h \
                   $$PWD/zip/quazipfileinfo.h \
                   $$PWD/zip/quazipnewinfo.h \
                   $$PWD/zip/unzip.h \
                   $$PWD/zip/zconf.h \
                   $$PWD/zip/zip.h \
                   $$PWD/zip/zlib.h

        SOURCES += $$PWD/zip/ioapi.c \
                   $$PWD/zip/quazip.cpp \
                   $$PWD/zip/quazipfile.cpp \
                   $$PWD/zip/quazipnewinfo.cpp \
                   $$PWD/zip/unzip.c \
                   $$PWD/zip/zip.c
    }

    HEADERS += $$PWD/zipfun.h
    SOURCES += $$PWD/zipfun.cpp
}

contains(DEFINES, SURRPORT_MULTIMEDIA) {
    INCLUDEPATH += $$PWD/media/

    SOURCES +=   $$PWD/media/wav_parser.cpp \
                 $$PWD/media/mp3_parser.cpp \
                 $$PWD/media/basemedia.cpp

    HEADERS  +=  $$PWD/media/wav_parser.h \
                 $$PWD/media/mp3_parser.h \
                 $$PWD/media/basemedia.h \
                 $$PWD/media/playerbase.h

    contains(DEFINES, IDE){
        SOURCES +=   $$PWD/media/fft.cpp \
                     $$PWD/media/baseaudiocodec.cpp

        HEADERS  +=  $$PWD/media/fft.h \
                     $$PWD/media/baseaudiocodec.h
    }

    contains(DEFINES, PHONON) {
        QT += multimedia
        SOURCES +=   $$PWD/media/phonon.cpp
        HEADERS  +=  $$PWD/media/phonon.h
    }

    contains(DEFINES, QPLAYER){
        QT += multimedia
        SOURCES +=   $$PWD/media/qplayer.cpp
        HEADERS  +=  $$PWD/media/qplayer.h
    }

    contains(DEFINES, OSS){
        SOURCES +=   $$PWD/media/ossplayer.cpp
        HEADERS  +=  $$PWD/media/ossplayer.h
    }

}

DEFINES += SURRPORT_UI
contains(DEFINES, SURRPORT_UI)  {
    INCLUDEPATH += $$PWD/ui/

    HEADERS += $$PWD/ui/bstui.h  \
               $$PWD/ui/bggen.h

    SOURCES += $$PWD/ui/bstui.cpp  \
               $$PWD/ui/bggen.cpp

    RESOURCES += $$PWD/ui/bstui.qrc

    HEADERS += $$PWD/ui/messagebox.h
    SOURCES += $$PWD/ui/messagebox.cpp
    FORMS +=   $$PWD/ui/messagebox.ui
    RESOURCES += $$PWD/ui/messagebox.qrc

    HEADERS += $$PWD/ui/splashscreen.h
    SOURCES += $$PWD/ui/splashscreen.cpp
    FORMS +=   $$PWD/ui/splashscreen.ui
    RESOURCES += $$PWD/ui/splashscreen.qrc

    contains(DEFINES, SETTING) {
        HEADERS += $$PWD/ui/rclistwidget.h
        SOURCES += $$PWD/ui/rclistwidget.cpp
    }

    !contains(DEFINES, DISPLAY)     {
        SOURCES +=  $$PWD/ui/catalogtreewidget.cpp \
                    $$PWD/ui/playlabel.cpp

        HEADERS +=  $$PWD/ui/catalogtreewidget.h \
                    $$PWD/ui/playlabel.h
    }
}

contains(DEFINES, SURRPORT_TOOL)  {
    INCLUDEPATH += $$PWD/tool/

    contains(DEFINES, SURRPORT_USB)     {
        INCLUDEPATH +=  $$PWD/tool/usb/  \
                        $$PWD/tool/usb/lib/

        SOURCES +=  $$PWD/tool/usbport.cpp
        HEADERS +=  $$PWD/tool/usbport.h
    }

    !contains(DEFINES, IOS)     {
        SOURCES +=  $$PWD/tool/serialport.cpp \
                    $$PWD/tool/modem.cpp
        HEADERS +=  $$PWD/tool/serialport.h \
                    $$PWD/tool/modem.h
    }

    SOURCES +=  $$PWD/tool/tooltcp.cpp \
                $$PWD/tool/tooludp.cpp \
                $$PWD/tool/toolhttp.cpp

    HEADERS +=  $$PWD/tool/tooltcp.h \
                $$PWD/tool/tooludp.h \
                $$PWD/tool/toolhttp.h

    contains(DEFINES, SURRPORT_USBWATCHE)    {
        INCLUDEPATH += $$PWD/tool/qdevicewatcher/

        HEADERS +=  $$PWD/tool/qdevicewatcher/qdevicewatcher.h \
                    $$PWD/tool/qdevicewatcher/qdevicechangeevent.h \
                    $$PWD/tool/qdevicewatcher/qdevicewatcher_global.h \
                    $$PWD/tool/qdevicewatcher/qdevicewatcher_p.h

        SOURCES +=  $$PWD/tool/qdevicewatcher/qdevicechangeevent.cpp \
                    $$PWD/tool/qdevicewatcher/qdevicewatcher.cpp

        SOURCES +=  $$PWD/tool/qdevicewatcher/qdevicewatcher_win32.cpp
    }
}

HEADERS += $$PWD/uifun.h
SOURCES += $$PWD/uifun.cpp

HEADERS += $$PWD/devfun.h \
           $$PWD/deviceinc.h

SOURCES += $$PWD/devfun.cpp

SOURCES += $$PWD/global.cpp
HEADERS += $$PWD/global.h

