#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QObject>
#include "protocoldeal.h"
#include <QQuickView>
#include <QQmlContext>
#include <QQuickItem>
#include <QScreen>
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
//    qmlRegisterType<Protocoldeal>("lb2616.tools.Protocoldeal", 1, 0, "Protocoldeal");
    QQmlApplicationEngine engine;
    Protocoldeal *pro = Protocoldeal::GetInstance();
    engine.rootContext()->setContextProperty("Mypro", pro);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    QObject * topLevel = engine.rootObjects().value(0);
    QQuickWindow* window = qobject_cast<QQuickWindow*>(topLevel);
//    QScreen screen;
//    screen.setOrientationUpdateMask(Qt::LandscapeOrientation);
//    window->showFullScreen();
    window->show();
//    QQuickItem *win = qobject_cast<QQuickItem*>(topLevel);
//    win->setRotation(90);
//    win->sh
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
//    QGuiApplication app(argc, argv);
//    QQuickView view;
//    Protocoldeal *pro = Protocoldeal::GetInstance();
//    view.rootContext()->setContextProperty("Mypro", pro);
//    view.setSource(QUrl(QStringLiteral("qrc:///main.qml")));
//    view.show();
    return app.exec();
}

