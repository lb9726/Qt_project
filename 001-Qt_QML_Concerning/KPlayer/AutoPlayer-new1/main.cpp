#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/Media.qml")));
//    QQuickView *view = new QQuickView;
//    view->engine()
//    view->setSource(QUrl(QStringLiteral("qrc:/Media.qml")));
//    QQuickView view;
//    QObject::connect(view->engine(), SIGNAL(quit()), view, SLOT(close()));
//    view->showFullScreen();
//    view->show();
//    engine.showFullScreen();

//    QQuickView view;
//    view.setSource(QUrl(QStringLiteral("qrc:/Media.qml")));
//    view.showFullScreen();
    return app.exec();
}
