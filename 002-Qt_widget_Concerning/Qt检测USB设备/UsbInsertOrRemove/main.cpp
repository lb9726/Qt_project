#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "usbstatusdetected.h"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QQmlContext *cont = engine.rootContext();
    UsbStatusDetected *usbdetect = new UsbStatusDetected;
    cont->setContextProperty("Usbdetect", usbdetect);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
