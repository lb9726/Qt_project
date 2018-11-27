#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "findqmlcomponent.h"
#include <QObject>
#include "protocoldeal.h"
//#include <QDeclarativeComponent>
//#include <QDeclarativeEngine>
//#include <QDeclarativeContext>
//#include <QtQuick1/QDeclarativeView>
//#include <QtQuick1/QDeclarativeItem>
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<Protocoldeal>("lb2616.tools.Protocoldeal", 1, 0, "Protocoldeal");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    QObject * topLevel = engine.rootObjects().value(0);
    QQuickWindow* window = qobject_cast<QQuickWindow*>(topLevel);
//    window->showFullScreen();
    window->show();

//    QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:///main.qml")));
//    QObject *obj = component.create();
//    QObject *ArrowDirection = obj->findChild<QObject *>("arrowdownpicture");
//    if (ArrowDirection)
//    {
//        ArrowDirection->setProperty("visble", false);
//    }
//    QDeclarativeView *declarativeView = new QDeclarativeView;
//    QDeclarativeEngine  *m_declarativeEngine = declarativeView->engine();
//    QDeclarativeContext * rootContent = m_declarativeEngine->rootContext();
//    rootContent->setContextProperty("GlobalSetting",this);
//    QDeclarativeView view;
//    view.setSource(QUrl(QStringLiteral("qrc:///main.qml")));
//    view.showFullScreen();
    return app.exec();
}

