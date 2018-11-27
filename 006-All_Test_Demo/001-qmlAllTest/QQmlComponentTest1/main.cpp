#include <QCoreApplication>
#include <QQmlEngine>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QDebug>
#include <QQuickWindow>

//int main(int argc, char *argv[])
//{
//    QCoreApplication a(argc, argv);
////    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
////    QQmlEngine engine;
////    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
//    QQmlApplicationEngine engine;
//    QQmlComponent component(&engine);
////    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
//    component.loadUrl(QStringLiteral("qrc:/main.qml"));
//    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
////    qDebug()<<__PRETTY_FUNCTION__<<component.errors();
//    QObject *myObject;
//    if (component.isReady())
//    {
//        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<component.errors();
//        myObject = component.create();
//        if (myObject)
//        {
//            qDebug()<<"myObject = "<<myObject;
//        }
//        else
//        {
//            qDebug()<<"myObject is NULL";
//        }
//        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<component.errors();
//        QQuickWindow *window = qobject_cast<QQuickWindow*>(myObject);
//        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<component.errors();
//        if (window)
//        {
//            window->showNormal();
//        }
//        else
//        {
//            qDebug()<<"window is NULL";
//        }
//        //        QQuickItem *item = qobject_cast<QQuickItem*>(myObject);
////        int width = item->width();
////        qDebug() << item->position();
////        qDebug() << width;
//    }
//    else
//    {
//        qDebug()<<__PRETTY_FUNCTION__<<component.errors();
//        qDebug() << "component is not ready!!!!!!!!!";
//    }
//    return a.exec();
//}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
//    qmlRegisterType<gemini>("Union.Lotto.Gemini", 1, 0, "Gemini");
    QQmlEngine engine;

    QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:///main.qml")));
    QObject *myObject = component.create();
    if (myObject)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
        QQuickItem *item = qobject_cast<QQuickItem*>(myObject);
        int width = item->width();
        qDebug() << item->position();
        qDebug() << "width = "<<width<<"height = "<< item->height();
        QQuickItem * rect = item->findChild<QQuickItem *>("rect1");
        if (rect)
        {
            qDebug() <<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<< rect->position();
            qDebug() << "width = "<<rect->width()<<"height = "<< rect->height();
        }
        else
        {
            qDebug() << "rect is NULL";
        }
    }
    else
    {
        qDebug()<<"myObject is NULL";
    }
    return app.exec();
}
