#include <QApplication>
#include <QGraphicsView>
#include <QWSServer>
#include <QTimer>
#include "devicemanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv, QApplication::GuiServer);

    GetApplicationDirectory();
    IDE_LOGGER(QString("> -- APP %1 started!").arg(GlobalPara.m_SoftwareDir));

    GlobalPara.m_DeviceManager = new DeviceManager(QRect(0,0,a.desktop()->width(),a.desktop()->height()));
    GlobalPara.m_DeviceManager->show();
    GlobalPara.m_DeviceManager->setWindowTitle(QObject::tr("BST_DISPLAY"));

    int ret = a.exec();

    sync();
    IDE_LOGGER("> -- APP exited!");
    return ret;
}
