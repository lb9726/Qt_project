#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    QQuickView mview;
    QObject::connect(mview.engine(), SIGNAL(quit()), &app, SLOT(quit()));
    //不这样写，quit会直接提示没有接受者处理这个handle
    //Signal QQmlEngine::quit() emitted, but no receivers connected to handle it.
    mview.setSource(QUrl(QStringLiteral("qrc:/Main2.qml")));
    mview.show();
    return app.exec();
}
