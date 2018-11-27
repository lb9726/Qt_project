#include "qtquick1applicationviewer.h"
#include <QApplication>
#include "globalsetting.h"

extern GlobalSetting* appGlobalSetting;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //qmlRegisterType<ArrowItem>("ArrowElement", 1, 0, "ArrowItem");
    appGlobalSetting = new GlobalSetting();
    QtQuick1ApplicationViewer viewer;
    appGlobalSetting->setQuickView(&viewer);
    appGlobalSetting->initApp();
    int ret = app.exec();
    appGlobalSetting->clearData();
    return ret;
}
