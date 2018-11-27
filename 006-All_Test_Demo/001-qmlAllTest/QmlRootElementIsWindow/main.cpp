//#include <QGuiApplication>
//#include <QQmlApplicationEngine>
//#include <QQmlComponent>
//#include <QQuickWindow>
//#include <QDebug>

#include "framework.h"

int main(int argc, char *argv[])
{
//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//    QGuiApplication app(argc, argv);

//    QQmlApplicationEngine engine;
//    QQmlComponent component(&engine);
//    component.loadUrl(QUrl(QLatin1String("qrc:/main.qml")));
////    component.loadUrl(QUrl::fromLocalFile("qrc:/main.qml"));
////    warning: "file:///home/libo/qrc:/main.qml:-1 File not found\n"

//    if (component.isReady())
//    {
//        QQuickWindow* w = qobject_cast<QQuickWindow*>(component.create());
//        qDebug()<<"w = "<<w;
//        w->show();
//    }
//    else
//    {
//        qWarning() << "warning:" << component.errorString();
//    }

//    return app.exec();
    if (FrameWork::getInstance())
    {
        FrameWork::getInstance()->initScene(argc, argv);
        delete FrameWork::getInstance();
    }
    return 0;
}
