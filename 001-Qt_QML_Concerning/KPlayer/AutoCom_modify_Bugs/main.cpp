#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/Media.qml")));
//    QQuickView view;
//    view.setSource(QUrl(QStringLiteral("qrc:/Media.qml")));
//    view.show();
    return app.exec();
}
