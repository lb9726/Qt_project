#include <QApplication>
//#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QtQml>
#include "RGBGame.h"
#include "mysingleton.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<RGBGame>("lb2616.tools.RGBGame", 1, 0, "RGBGame");
    qmlRegisterSingletonType<MySingleton>("lb2616.tools.MySingleton",
                                          1, 0, "MySingleton", singletonProvider);
//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    QQuickView view;
    QObject::connect(view.engine(), SIGNAL(quit()), &app, SLOT(quit()));
    //不这样写，quit会直接提示没有接受者处理这个handle
    view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    view.show();
//    view.showFullScreen();
    return app.exec();
}

/*
 * main.cpp 将 RGBGame 类注册为在 QML 中可以使用的 RGBGame 类型,主版本为 1,
次版本为 0,库的名字是 lb2616.tools.RGBGame。
注册动作必须在 QML 上下文创建之前,否则无效。
另外:QQuickView 为 Qt Quick UI 提供了一个窗口,可以方便地加载 QML 文件并显示
其 界 面 。 QApplication 派 生 自 QGuiApplication , 而 QGuiApplication 又 派 生 自
QCoreApplication,这三个类是常见的管理 Qt 应用程序的类。QQmlApplicationEngine 可
以方便地从一个单一的 QML 文件中加载应用程序,它派生自 QQmlEngine,QQmlEngine
则提供了加载 QML 组件的环境,可以与 QQmlComponent、QQmlContext 等一起使用。
单例注册:注册单例要用 qmlRegisterSingletonType()。有三个重载函数,单例类型可
以是 QObject 或 QJSValue,也可以在 QML 文件中添加”pragma Singleton”,以这个
QML 文件的路径注册一个单例。注册成功后,我们不必亲自去构造一个实例,却可以直接
使用一个类对象。这里介绍一个单例类型为 QObject 的例子。
首先,自定义一个派生自 QObject 的 MySingelton 类,参照 MySingleton.h:
 */
