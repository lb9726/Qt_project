QT += qml quick xml

CONFIG += c++11
TARGET = mediaScreen
DESTDIR = $$PWD/000_EXEPath

SOURCES += main.cpp \
    configureholder.cpp \
    ctranslator.cpp \
    mediascreen.cpp

RESOURCES += qml.qrc

TRANSLATIONS = language_english.ts\
            language_china.ts\
            language_traditionchina.ts\
            language_Russian.ts

RESOURCES += qml.qrc
RC_FILE = icon.rc

include($$PWD/Common/Common.pri)

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    configureholder.h \
    ctranslator.h \
    define.h \
    mediascreen.h
