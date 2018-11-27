#include "qtquick1applicationviewer.h"
#include <QApplication>
#include <QWSServer>
#include "globalsetting.h"

extern GlobalSetting* appGlobalSetting;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //qmlRegisterType<ArrowItem>("ArrowElement", 1, 0, "ArrowItem");
    QWSServer::setCursorVisible(FALSE);
    appGlobalSetting = new GlobalSetting();
    QtQuick1ApplicationViewer viewer;
    viewer.setWindowFlags(Qt::FramelessWindowHint);
    appGlobalSetting->setQuickView(&viewer);
    appGlobalSetting->initApp();
    int ret = app.exec();
    appGlobalSetting->clearData();
    return ret;
}
