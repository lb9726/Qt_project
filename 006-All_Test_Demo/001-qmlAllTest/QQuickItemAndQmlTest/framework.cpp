#include "framework.h"
#include <QDebug>

FrameWork* FrameWork::_instance = 0;

FrameWork::FrameWork(QObject *parent) : QObject(parent)
{

}

void FrameWork::registerMyQmlType()
{
    qmlRegisterType<QmlScreenA>("lb2616.tools.QmlScreenA", 1, 0, "QmlScreenA");
    qmlRegisterType<QmlScreenB>("lb2616.tools.QmlScreenB", 1, 0, "QmlScreenB");
    qmlRegisterType<QmlScreenC>("lb2616.tools.QmlScreenC", 1, 0, "QmlScreenC");
}

FrameWork *FrameWork::getInstance()
{
    if (0 == _instance)
    {
        _instance = new FrameWork();
    }
    return _instance;
}

int FrameWork::runApplication(int argc, char *argv[])
{
    mapp = new QGuiApplication(argc, argv);

//    QQmlApplicationEngine engine; // 加载以window和appwindown为根节点的qml
    mview = new QQuickView(); // 加载以Item为根节点的元素
    mQmlContext = mview->rootContext();
    mQmlContext->setContextProperty("FrameWork", this);
    mview->setSource(QStringLiteral("qrc:/QmlTestA.qml"));
    mview->show();
    getObject();
    int ret = mapp->exec();
    mapp->deleteLater();
    mview->deleteLater();
    qDebug()<<__PRETTY_FUNCTION__<<__LINE__<<"will delete pointer";
    return ret;
}

void FrameWork::getObject()
{
    if (mview)
    {
        QQuickItem* tmpItem = mview->rootObject();
        if (tmpItem)
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"tmpItem = "<<tmpItem;
        }
    }
}

void FrameWork::changeUi(int index)
{
    mview->hide();
    QQuickView* tmpLastView = mview;
    mview = new QQuickView();
//    QQuickItem* tmpItem = tmpLastView->rootObject();
//    delete tmpItem;
    bool flag = tmpLastView->close();
    qDebug()<<__PRETTY_FUNCTION__<<"flag = "<<flag;
    tmpLastView->deleteLater();
    qDebug()<<__PRETTY_FUNCTION__<<"index = "<<index;
    mQmlContext = mview->rootContext();
    mQmlContext->setContextProperty("FrameWork", this);
    if(0 == index)
    {
        mview->setSource(QStringLiteral("qrc:/QmlTestA.qml"));
        mview->show();
    }
    else if (1 == index)
    {
        mview->setSource(QStringLiteral("qrc:/QmlTestB.qml"));
        qDebug()<<"after setSource";
        mview->show();
    }
    else if (2 == index)
    {
        mview->setSource(QStringLiteral("qrc:/QmlTestC.qml"));
        mview->show();
    }
    getObject();
}














