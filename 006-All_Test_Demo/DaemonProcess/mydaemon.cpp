#include "mydaemon.h"
#include <QDebug>
#include <QCoreApplication>

MyDaemon::MyDaemon(QObject *parent) : QObject(parent)
{
    mProcess = nullptr;
    mRunStatus = QString();
    mRunPath = QCoreApplication::applicationDirPath();
}

void MyDaemon::initDaemon()
{

}

void MyDaemon::startExternProcess()
{
    QString runPath = mRunPath;
    mProcess = new QProcess();
    if (mProcess)
    {
        int res = mProcess->execute(runPath + "/ChangePicture");
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"res = "<<res;
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"after execute";
    }
}

void MyDaemon::killExternProcess()
{

}

void MyDaemon::rebootSystem()
{
    QString cmd = QString("reboot");
    int flag = system(cmd.toLatin1().data());
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"flag = "<<flag;
}

QString MyDaemon::getExternProcessRunStatus()
{

    return QString();
}
