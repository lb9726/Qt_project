#include "usbaction.h"
#include <QIODevice>
#include <QDebug>
#include <QCoreApplication>
#include <QThread>
#include <QFileInfoList>
#include <QFileInfo>
#include <QDir>

UsbAction::UsbAction(QObject *parent) : QObject(parent)
  , mFileList(QStringList())
  , runPath(QCoreApplication::applicationDirPath())
  , fileLength(0)
  , fileNum(0)
{
    connect(this, &UsbAction::sigStartProgress, this, &UsbAction::sendUpdateProgress);
}

QStringList UsbAction::readFile()
{
    QString tmpPath = runPath + "/copyFile.txt";
    QFile file(tmpPath);
    mCpFileList.clear();
    if (file.open(QIODevice::ReadOnly))
    {
        while(!file.atEnd())
        {
            QByteArray line = file.readLine();
            qDebug()<<"read line = "<< line;
            QString tmp(line);
            qDebug()<<"tmp = "<< tmp;
            QString spl = tmp.split("->").at(0);
            qDebug()<<"spl = "<< spl;
            if (spl != QString('\n') && spl != QString(" "))
            {
                spl = spl.split("/").last();    // 取出文件的文件名
                spl = spl.left(spl.length() - 2); // 去掉最后一个字符’
                mCpFileList.append("Updating " + spl);
            }
        }
        file.close();
        return mCpFileList;
    }
    else
    {
        qDebug()<<"QStringList UsbAction:: "<<__FUNCTION__<<"() "<<"lines "<<__LINE__<<" "<<tmpPath<<" is open error";
        return QStringList();
    }
    return QStringList();
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
        qDebug()<<"fileInfo.name = "<</*path + "/" + */fileInfo.fileName();
        mFileList.append("Updating Disk " + fileInfo.fileName());
    }
    //dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot)返回所有子目录，并进行过滤
    foreach(QString subDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        fileLength += dirFileSize(path + QDir::separator() + subDir); //若存在子目录，则递归调用dirFileSize()函数
    }
    qDebug()<<"fileNum = "<< fileNum <<" fileLenght = "<< fileLength;
    return fileLength;
}

void UsbAction::resetFileLenghtAndFileNum()
{
    fileLength = 0;
    fileNum = 0;
}

int UsbAction::getFileNum()
{
    return fileNum;
}

int UsbAction::getFileLength()
{
    return fileLength;
}

int UsbAction::copyFile(QString src, QString dest)
{
//    dest = " /usr/bst/usrfs/ ";
//    src = " /meida/sodimas/ ";
    dest = " /home/libo/Desktop/emulation/ ";
    src = " /home/libo/Desktop/1243/ ";
    QString cmd = "cp -vrf " + src + dest + " > /home/libo/Desktop/test.txt";
    int flag = system(cmd.toLatin1().data());
    qDebug()<<"cmd.toLatin1().data()"<<cmd.toLatin1().data();
    this->moveToThread(&mProgressThread);
    mProgressThread.start();
    emit sigStartProgress();
    qDebug()<<__FUNCTION__<<"()"<<" flag = "<< flag;
    return flag;
}

QStringList UsbAction::getFileList()
{
    qDebug()<< mFileList;
    return mFileList;
}

void UsbAction::sendUpdateProgress()
{
    int num = 0;
    while(1)
    {
        if (num < 100)
        {
            num = num + 100/getFileNum();
            if (num >= 98)
            {
                emit sigUpdateProgress(100);
                break;
            }
            qDebug()<<"num = "<<num;
            emit sigUpdateProgress(num);
        }
        QThread::msleep(1000);
    }
}


