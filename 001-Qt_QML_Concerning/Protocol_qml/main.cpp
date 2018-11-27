#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QtQml>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    QObject * topLevel = engine.rootObjects().value(0);
    QQuickWindow* window = qobject_cast<QQuickWindow*>(topLevel);
    window->show();
//    window->showFullScreen();

//    QQuickView Qengine;
//    Qengine.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
