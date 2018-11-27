INCLUDEPATH += setting_sodimas/

SOURCES +=  ../BST_IDE/setting/devicesetbase.cpp \
            ../BST_IDE/setting/flrmapbar.cpp \
            ../BST_IDE/setting/textinput.cpp \
            ../BST_IDE/setting/imageselectdialog.cpp \
            setting_sodimas/devicesetting.cpp

HEADERS +=  ../BST_IDE/setting/devicesetbase.h \
            ../BST_IDE/setting/flrmapbar.h \
            ../BST_IDE/setting/textinput.h \
            ../BST_IDE/setting/imageselectdialog.h \
            setting_sodimas/devicesetting.h

FORMS +=    ../BST_IDE/setting/flrmapbar.ui \
            ../BST_IDE/setting/textinput.ui \
            ../BST_IDE/setting/imageselectdialog.ui

FORMS +=   setting_sodimas/devicesetting.ui

RESOURCES += setting_sodimas/setting.qrc




