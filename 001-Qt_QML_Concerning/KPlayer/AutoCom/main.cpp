#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include <showgifpicture.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QQmlContext *content = engine.rootContext();
    showGifPicture sgp;
    content->setContextProperty("ShowGif", &sgp);
    engine.load(QUrl(QStringLiteral("qrc:/Media.qml")));
    return app.exec();
}
