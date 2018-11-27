#include "usbaction.h"
#include <QIODevice>
#include <QDebug>
#include <QCoreApplication>
#include <QThread>
#include <QFileInfoList>
#include <QFileInfo>
#include <QDir>
#include "common/define.h"
#include <stdlib.h>
UsbAction::UsbAction(QObject *parent) : QObject(parent)
  , runPath(QCoreApplication::applicationDirPath())
  , fileLength(0)
  , fileNum(0)
  , mFileList(QStringList())
  , mCpFileList(QStringList())
{
    connect(this, &UsbAction::sigStartProgress, this, &UsbAction::sendUpdateProgress);
}

// 获取用户选择文件夹下的所有文件的大小，调用QFileInfo的size方法实现
quint64 UsbAction::dirFileSize(const QString &path)
{
    QDir dir(path);
    //dir.entryInfoList(QDir::Files)返回文件信息
    foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::Files))
    {
        fileLength += fileInfo.size() / 1024; //计算文件大小
        ++fileNum;
//        qDebug()<<"fileInfo.name = "<</*path + "/" + */fileInfo.fileName();
        mFileList.append(fileInfo.fileName());
    }
    //dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot)返回所有子目录，并进行过滤
    foreach(QString subDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        fileLength += dirFileSize(path + QDir::separator() + subDir); //若存在子目录，则递归调用dirFileSize()函数
    }
//    qDebug()<<"fileNum = "<< fileNum <<" fileLenght = "<< fileLength;
    return fileLength;
}

void UsbAction::resetCopyParameter()
{
    fileLength = 0;
    fileNum = 0;
    mFileList.clear();
    mCpFileList.clear();
}

int UsbAction::getFileNum()
{
    return fileNum;
}

int UsbAction::getFileLength()
{
    return fileLength;
}

QStringList UsbAction::getFileList(bool isBackUp)
{
    QString str;
    if (isBackUp)
    {
        foreach (str, mFileList)
        {
            mCpFileList.append("BackUp " + str);
        }
    }
    else
    {
        foreach (str, mFileList)
        {
            mCpFileList.append("Updating " + str);
        }
    }
//    qDebug()<< mCpFileList;
    return mCpFileList;
}

void UsbAction::removeFile(QString src)
{
    QString cmd = "rm -rf " + src;
    int flag = system(cmd.toLatin1().data());
    qDebug()<<__FUNCTION__<<" cmd.toLatin1().data() "<<cmd.toLatin1().data()<<" flag = "<<flag;
    system("sync");
    QtSleep(1500);
}

int UsbAction::copyFile(QString src, QString dest)
{
    // cmd = "cp -vrf " + src + " " + dest + " > /usr/bst/usrfs/copyFile.txt";
    QString cmd = "cp -rf " + src + " " + dest;
    int flag = system(cmd.toLatin1().data());
    qDebug()<<"cmd.toLatin1().data()"<<cmd.toLatin1().data();
    qDebug()<<__FUNCTION__<<"() flag = "<<flag;
    QtSleep(1500);
    this->moveToThread(&mProgressThread);
    mProgressThread.start();
    return flag;
}

void UsbAction::sendUpdateProgress(bool isBackUp)
{
    qDebug()<<" void UsbAction::" <<__FUNCTION__;
    int num = 0;
    while(1)
    {
        if (num < 100)
        {
            if (100 / (fileNum+1) < 1)
            {
                num = num + 1;
            }
            else
            {
                num = num + 100 /(fileNum +1);
            }
            if (num >= 98)
            {
                emit sigUpdateProgress(100);
                break;
            }
            qDebug()<<"num = "<<num;
            emit sigUpdateProgress(num);
        }
        QThread::msleep(1500);
    }
    if (isBackUp)
    {
        emit sigBackUpEnded();
        qDebug()<<" will send BackUp ended signal!";
    }
    else
    {
        emit sigUpdateEnded();
        qDebug()<<" will send Updating ended signal!";
    }
}
