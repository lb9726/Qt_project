DEPENDPATH += $$PWD/..
INCLUDEPATH +=  $$PWD/

HEADERS += $$PWD/plugininterface.h \
    interface/pluginwidget.h \
    ../BST_IDE/interface/stackedmenu.h \
    ../BST_IDE/interface/menuclass.h

SOURCES += \
    interface/pluginwidget.cpp \
    ../BST_IDE/interface/stackedmenu.cpp \
    ../BST_IDE/interface/menuclass.cpp

contains(DEFINES, SIZE_800x480) {
    FORMS += ../BST_IDE/interface/stackedmenu_h400.ui
}
contains(DEFINES, SIZE_800x600) {
    FORMS += ../BST_IDE/interface/stackedmenu_h400.ui
}
contains(DEFINES, SIZE_1024x768) {
    FORMS += ../BST_IDE/interface/stackedmenu_h600.ui
}
contains(DEFINES, SIZE_1280x800) {
    FORMS += ../BST_IDE/interface/stackedmenu_h600.ui
}
contains(DEFINES, SIZE_1366x768) {
    FORMS += ../BST_IDE/interface/stackedmenu_h600.ui
}
contains(DEFINES, SIZE_1920x540) {
    FORMS += ../BST_IDE/interface/stackedmenu_h400.ui
}

RESOURCES += ../BST_IDE/interface/interface.qrc
