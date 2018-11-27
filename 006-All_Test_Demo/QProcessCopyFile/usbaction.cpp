#include "usbaction.h"
#include <QDebug>
#include <QDir>
#include <QByteArray>

UsbAction::UsbAction(QObject *parent) : QObject(parent)
{
    cpProcess = nullptr;
    isBackUpFlag = false;
    mCmd = QString();
}

void UsbAction::initProcess()
{
    cpProcess = new QProcess;
    if (cpProcess)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"cpProcess is not null";
        connect(cpProcess, SIGNAL(started()), this, SLOT(sltStart()));
        connect(cpProcess, SIGNAL(finished(int)), this, SLOT(sltFinished(int)));
        connect(cpProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(sltError(QProcess::ProcessError)));
        connect(cpProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(sltReadStandardOutPut()));
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"cpProcess is null";
    }
}

void UsbAction::startWork()
{
    if (cpProcess)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"cmd = "<<mCmd;
        cpProcess->start(mCmd);
//        cpProcess->execute(mCmd);
    }
}

void UsbAction::sltError(QProcess::ProcessError error)
{
    if (isBackUpFlag)
    {
        emit cmdError(QString("BackUp"));
    }
    else
    {
        emit cmdError(QString("Updating"));
    }
    if (error == QProcess::FailedToStart)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"QProcess::FailedToStart";
    }
    else if (error == QProcess::Crashed)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"QProcess::Crashed";
    }
    else if (error == QProcess::Timedout)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"QProcess::Timedout";
    }
    else if (error == QProcess::WriteError)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"QProcess::WriteError";
    }
    else if (error == QProcess::ReadError)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"QProcess::ReadError";

    }
    else if (error == QProcess::UnknownError)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"QProcess::UnknownError";
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"Others error!";
    }
//    switch (error) {
//    case QProcess::FailedToStart:
//        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"QProcess::FailedToStart";
//        break;
//    case QProcess::Crashed:
//        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"QProcess::Crashed";
//        break;
//    case QProcess::Timedout:
//        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"QProcess::Timedout";
//        break;
//    case QProcess::WriteError:
//        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"QProcess::WriteError";
//        break;
//    case QProcess::ReadError:
//        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"QProcess::ReadError";
//        break;
//    case QProcess::UnknownError:
//        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"QProcess::UnknownError";
//        break;
//    default:
//        break;
//    }
}

void UsbAction::sltFinished(int code)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"code = "<<code;
    if (isBackUpFlag)
    {
        emit cmdFinished(QString("BackUp"));
    }
    else
    {
        emit cmdFinished(QString("Updating"));
    }
}

int UsbAction::copyFile(QString src, QString dest)
{
//    QDir tmpDir(dest);
//    if (!tmpDir.exists())
//    {
//        QString mkdir = QString("mkdir -p %1").arg(dest);
//        qDebug()<<__PRETTY_FUNCTION__<<"mkdir = "<<mkdir;
//        system(mkdir.toLatin1().data());
//    }
    QDir tmpDir(dest);
    if (tmpDir.exists())
    {
        QString rmdir = QString("rm -rf %1").arg(dest);
        qDebug()<<__PRETTY_FUNCTION__<<"rm = "<<rmdir;
        system(rmdir.toLatin1().data());
    }
    QString cmd = "cp -vrf " + src + " " + dest;
    mCmd = cmd;
    return 0;
}

void UsbAction::sltStart()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
    if (isBackUpFlag)
    {
        if (cpProcess)
        {
            emit cmdStart(QString("BackUp"));
        }
    }
    else
    {
        if (cpProcess)
        {
            emit cmdStart(QString("Updating"));
        }
    }
}

void UsbAction::sltReadStandardOutPut()
{
    QByteArray pStandardOutput;
    if (cpProcess)
    {
        pStandardOutput = cpProcess->readAllStandardOutput();
    }
    QString tmpString = QString(pStandardOutput);
    QStringList tmplist = tmpString.split("->");
    foreach (QString tmp, tmplist) {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<tmp.split("/").last();
    }
}

void UsbAction::setIsBackUpFlag(bool value)
{
    isBackUpFlag = value;
}

