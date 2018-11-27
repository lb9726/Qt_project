INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/..

HEADERS += $$PWD/eventmap.h
SOURCES += $$PWD/eventmap.cpp

contains(DEFINES, IDE) {
    HEADERS += $$PWD/eventmapform.h
    SOURCES += $$PWD/eventmapform.cpp
    FORMS += $$PWD/eventmapform.ui
}








