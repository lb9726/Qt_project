#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

/*    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));*/
    QQuickView quickView;
    quickView.setSource(QStringLiteral("qrc:/main.qml"));
    quickView.show();
    return app.exec();
}
