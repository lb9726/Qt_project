#include <QGuiApplication>
#include <QFont>

#include "mediascreen.h"
#include <QFontDatabase>
#include <QCoreApplication>
#include <QDebug>

#if defined(Q_OS_WIN32)
#include <windows.h>
#include "UsbFormat/usbeventfilter_win32.h"
#endif

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QString fontDir = QCoreApplication::applicationDirPath() + "/font/KONE Information_v12.otf";
    QFontDatabase::addApplicationFont(fontDir);
    QFont font("KONE Information_v12");
    app.setFont(font);

    MediaScreen *mediascreen = new MediaScreen();
    mediascreen->init();
#if defined(Q_OS_WIN32)
    app.installNativeEventFilter(mediascreen->getPoint());
//#elif defined(Q_OS_LINUX)
//    mediascreen->usbDetected();
#endif
    int ret = app.exec();
    mediascreen->deleteLater();
    return ret;
}
