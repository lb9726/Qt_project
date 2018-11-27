#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "fileupdate2.h"
#include <QtQml>
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<FileUpdate2>("lb2616.tools.FileUpdate2", 1, 0, "FileUpdate2");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

