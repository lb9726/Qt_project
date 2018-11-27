#include <QApplication>
#include <QTextCodec>
#include "devicesetting.h"
#include "splashscreen.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec*codec=QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
#ifndef QT5
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif

    GetApplicationDirectory();

    //Splash
    QPixmap tmpPixmap = QPixmap(GlobalPara.m_SoftwareDir + D_ETC_DIR + QString("/splash.png"));
    SplashScreen tmpScreen(tmpPixmap);
    tmpScreen.show();
    a.processEvents();

    QtSleep(500);
    tmpScreen.updateProgressBar(25);

    iniParse *tmpParse = new iniParse(GlobalPara.m_SoftwareDir + D_ETC_DIR + QString("/boot.ini"));
    QString tmpLang;
    QTranslator translator;
    QString qmFile;
    if(tmpParse->getProperty("Language", tmpLang))
        qmFile = GlobalPara.m_SoftwareDir + D_ETC_DIR + QString("/lang/") + QString("SETTING_%1.qm").arg(tmpLang);
    if(translator.load(qmFile))   //将刚才发布的.qm文件加载到程序中
    {
        IDE_TRACE_STR(qmFile);
        a.installTranslator(&translator);
    }
    tmpParse->deleteLater();

    QtSleep(500);
    tmpScreen.updateProgressBar(50);

    DeviceManager w;
    w.setWindowTitle(QObject::tr("Sodimas display designer"));

    QtSleep(500);
    tmpScreen.updateProgressBar(75);

    tmpScreen.updateProgressBar(99);
    tmpScreen.finish(&w);

    w.show();

    return a.exec();
}
