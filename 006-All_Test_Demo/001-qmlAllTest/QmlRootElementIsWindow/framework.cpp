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
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    mApp = new QGuiApplication(argc, argv);

    mEngine = new QQmlApplicationEngine();

    if (mEngine)
    {
        mQmlContext = mEngine->rootContext();
    }
    if (mQmlContext)
    {
        mQmlContext->setContextProperty("FrameWorkContext", this);
    }

    QQmlComponent component(mEngine);
    component.loadUrl(QUrl(QLatin1String("qrc:/main.qml")));

    if (component.isReady())
    {
        mCurrentWindow = qobject_cast<QQuickWindow*>(component.create());
        qDebug()<<"w = "<<mCurrentWindow;
        mCurrentWindow->show();
    }
    else
    {
        qWarning() << "warning:" << component.errorString();
    }
    return mApp->exec();
//    mEngine->deleteLater();
//    mApp->deleteLater();
}

void FrameWork::changeUi(int index)
{
    QQuickWindow* tmpWindow = mCurrentWindow;
    QQmlComponent component(mEngine);
    mQmlContext = mEngine->rootContext();
    mQmlContext->setContextProperty("FrameWorkContext", this);

    if (0 == index)
    {
        component.loadUrl(QUrl(QLatin1String("qrc:/main.qml")));

        if (component.isReady())
        {
            mCurrentWindow = qobject_cast<QQuickWindow*>(component.create());
            qDebug()<<"mCurrentWindow = "<<mCurrentWindow;
            mCurrentWindow->show();
        }
        else
        {
            qWarning() << "warning:" << component.errorString();
        }
    }
    else if (1 == index)
    {
        component.loadUrl(QUrl(QLatin1String("qrc:/Page1.qml")));

        if (component.isReady())
        {
            mCurrentWindow = qobject_cast<QQuickWindow*>(component.create());
            qDebug()<<"mCurrentWindow = "<<mCurrentWindow;
            mCurrentWindow->show();
        }
        else
        {
            qWarning() << "warning:" << component.errorString();
        }
    }
    tmpWindow->deleteLater();
}

