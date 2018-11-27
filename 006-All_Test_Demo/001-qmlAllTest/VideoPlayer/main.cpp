#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include "dateandtime.h"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    DateAndTime dat;
    engine.rootContext()->setContextProperty("GetTimeAndDate", &dat);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

