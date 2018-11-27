#include "framework.h"
#include <QDebug>

FrameWork* FrameWork::_instance = 0;

FrameWork::FrameWork(QObject *parent) : QObject(parent)
{
}

FrameWork::~FrameWork()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
    if (mEngine)
    {
        mEngine->deleteLater();
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
    }
    if (mApp) // mApp 要在最后delete才行，否则出现异常退出
    {
        mApp->deleteLater();
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
    }
}

void FrameWork::registerMyQmlType()
{

}

FrameWork *FrameWork::getInstance()
{
    if (0 == _instance)
    {
        _instance = new FrameWork();
    }
    return _instance;
}

int FrameWork::initScene(int argc, char *argv[])
{
    mApp = new QGuiApplication(argc, argv);
    mEngine = new QQuickView();

    if (mEngine)
    {
        mQmlContext = mEngine->rootContext();
    }
    if (mQmlContext)
    {
        mQmlContext->setContextProperty("FrameWorkContext", this);
    }

    mEngine->setSource(QStringLiteral("qrc:/main.qml"));
    mEngine->show();
    return mApp->exec();
}

void FrameWork::changeUi(int index)
{
    QQuickView* tmpQuickView = mEngine;

    mEngine = new QQuickView();
    mQmlContext = mEngine->rootContext();
    mQmlContext->setContextProperty("FrameWorkContext", this);

    if (0 == index)
    {
        mEngine->setSource(QStringLiteral("qrc:/main.qml"));
        mEngine->show();
    }
    else if (1 == index)
    {
        mEngine->setSource(QStringLiteral("qrc:/Page1.qml"));
        mEngine->show();

    }
//    tmpQuickView->rootObject()->deleteLater();
    tmpQuickView->deleteLater();
}

