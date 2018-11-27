#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QMessageBox>
#include <QQmlContext>
#include <QFont>
#include "usbeventfilter.h"
#include "configureholder.h"

#if defined Q_OS_WIN32
#include <windows.h>
bool checkOnly()
{
    HANDLE m_hMutex  =  CreateMutex(NULL, FALSE,  L"MediascreenHelper_2017_06_04" );
    if  (GetLastError()  ==  ERROR_ALREADY_EXISTS)  {
        CloseHandle(m_hMutex);
        m_hMutex  =  NULL;
        return  false;
    }
    else{
        return true;
    }
}
#endif

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    if(!checkOnly()){
        QMessageBox msgBox;
        msgBox.setText("An instance already running, this will quit.");
        msgBox.exec();
        return 1;
    }

    QFont font("Microsoft YaHei");
    app.setFont(font);

    //install usb driver action event filter
    QQmlApplicationEngine engine;
    UsbEventFilter *nativeFilter = new UsbEventFilter();
    app.installNativeEventFilter(nativeFilter);
    ConfigureHolder *configureSerialer = new ConfigureHolder(&engine);
    QQmlContext *content = engine.rootContext();

    content->setContextProperty("UsbHelper",nativeFilter->usbHelper);
    content->setContextProperty("ConfigureSerialer",configureSerialer);

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    int ret = app.exec();

    delete nativeFilter;
    configureSerialer->deleteLater();

    return ret;
}
