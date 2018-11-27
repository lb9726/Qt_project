//#include <QGuiApplication>
//#include <QQuickView>

#include "framework.h"

int main(int argc, char *argv[])
{
//    QGuiApplication app(argc, argv);

//    QQuickView engine;
//    engine.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
//    engine.show();
//    return app.exec();
    if (FrameWork::getInstance())
    {
        FrameWork::getInstance()->initScene(argc, argv);
        delete FrameWork::getInstance();
    }
    return 0;
}
