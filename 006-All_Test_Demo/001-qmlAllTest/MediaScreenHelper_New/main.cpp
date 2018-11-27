#include <QGuiApplication>
#include <QFont>

#include "mediascreen.h"
#include <QFontDatabase>
#include <QCoreApplication>
#include <QDebug>
#include <QIcon>

#if defined(Q_OS_WIN)
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

    app.setWindowIcon(QIcon("icon.ico"));

    MediaScreen *mediascreen = new MediaScreen();
    mediascreen->init();

#if defined(Q_OS_WIN)
    app.installNativeEventFilter(mediascreen->getPoint());
#elif defined(Q_OS_LINUX)
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
    mediascreen->usbDetected();
#endif

    int ret = app.exec();
    delete mediascreen;
    mediascreen = 0;
    return ret;
}
