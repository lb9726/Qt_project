#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "dateandtime.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QQmlContext* tmpContent = engine.rootContext();
    DateAndTime tmpDateAndTime;
    if (tmpContent)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"tmpContent is OK";
        tmpContent->setContextProperty("DateAndTime", &tmpDateAndTime);
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"tmpContent is NULL";
    }
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
