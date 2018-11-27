INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/..

SOURCES +=  setting/devicesetbase.cpp \
            setting/imageselectdialog.cpp \
            setting/simulatorform.cpp

HEADERS +=  setting/devicesetbase.h \
            setting/imageselectdialog.h \
            setting/simulatorform.h

FORMS +=    setting/imageselectdialog.ui \
            setting/simulatorform.ui

SOURCES +=  $$PWD/devicesetting.cpp \
            $$PWD/flrmapbar.cpp

HEADERS +=  $$PWD/devicesetting.h \
            $$PWD/flrmapbar.h

FORMS +=    $$PWD/devicesetting.ui \
            $$PWD/flrmapbar.ui

RESOURCES += $$PWD/setting.qrc




