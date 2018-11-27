#include "zipandunzip.h"
#include <QString>
#include "define.h"
#include "zip/quazipfile.h"
#include "zip/quazip.h"
#include <QDir>
#include <QDateTime>

ZipAndUnZip::ZipAndUnZip(QThread *parent) : QThread(parent)
{
    m_UnZipProcess = 0;
    m_UnzipTimer = 0;
    mIsFinished = -1;  // -1 default -2 failed 0 success
}

void ZipAndUnZip::run()
{
    qDebug()<<__PRETTY_FUNCTION__<<QThread::currentThreadId();
    if (m_ZipFlag)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"m_ZipFlag = "<<m_ZipFlag;
        Zip(m_DirPath, m_ZipFile, true, true);
        m_ZipFlag = false;
    }
}

ZipAndUnZip::~ZipAndUnZip()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
}

void ZipAndUnZip::setZipFilePath(QString dirNamePath, QString zipFilePath)
{
    qDebug()<<__PRETTY_FUNCTION__<<dirNamePath<<zipFilePath;
    m_DirPath = dirNamePath;
    m_ZipFile = zipFilePath;
}

void ZipAndUnZip::setZipFlag(bool zipFlag, bool unZipFlag)
{
    m_ZipFlag = zipFlag;
    m_UnZipFlag = unZipFlag;
}

bool ZipAndUnZip::Zip(QString pSrcPath, QString pZipFile, bool pCover, bool pIngore)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<pSrcPath<<pZipFile;
    quint32 tmpProgress = 0;
    if(pZipFile.isEmpty())
    {
        IDE_TRACE();
        emit error();
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
        return false;
    }
    pSrcPath.replace('\\', '/');
    pZipFile.replace('\\', '/');
    qDebug()<<__PRETTY_FUNCTION__<<pSrcPath<<pZipFile;
    if(pZipFile.endsWith(".zip", Qt::CaseInsensitive) == false)
    {
        pZipFile.append(".zip");
    }
    QFile tmpFile(pZipFile);
    if(tmpFile.exists())
    {
        if(pCover)
        {
            IDE_TRACE();
            tmpFile.remove();
        }
        else
        {
            IDE_TRACE();
            emit error();
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            return true;
        }
    }

    //>@确保目的路径存在。
    QString tmpDstPath = getFileDirectory(pZipFile);
//    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"tmpDstPath = "<<tmpDstPath;
    CreatPath(tmpDstPath);
//    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"after CreatPath(tmpDstPath) = ";
    QFileInfo tmpInfo(pSrcPath);
//    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"tmpInfo";
//    qDebug()<<tmpInfo.isFile()<<tmpInfo.isDir()<<"lines = "<<__LINE__;
    if(tmpInfo.isFile())
    {
//        qDebug()<<__PRETTY_FUNCTION__<<"is file";
        if(!tmpInfo.exists())
        {
            IDE_TRACE_STR(pSrcPath);
            emit error();
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            return false;
        }
        if(pSrcPath.endsWith(".zip", Qt::CaseInsensitive)) //>@不可以压缩zip文件
        {
            IDE_TRACE();
            emit error();
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            return false;
        }
//    qDebug()<<__PRETTY_FUNCTION__<<"start zip";
        //>@打开文件准备开始压缩
        QuaZip zip(pZipFile);
        if(!zip.open(QuaZip::mdCreate))
        {
            IDE_TRACE();
            emit error();
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            return false;
        }
        QuaZipFile outFile(&zip);
        QString tmpNameOfZip = getFileName(pSrcPath);//>@压缩包内的此文件的文件名
        IDE_TRACE_STR(tmpNameOfZip);
        QFile inFile(pSrcPath);
        if(!inFile.open(QIODevice::ReadOnly))
        {
            IDE_TRACE();
            zip.close();
            emit error();
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            return false;
        }
        if(!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo(tmpNameOfZip, inFile.fileName())))
        {
            IDE_TRACE();
            inFile.close();
            zip.close();
            emit error();
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            return false;
        }

        //>@发送开始信号
        emit workStart("zip");
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);

        //>@开始压缩
        quint64 count = inFile.size();
//        qDebug()<<__PRETTY_FUNCTION__<<"count = "<<count;
        quint64 i = 0;
        while(i < count)
        {
            if(i)
            {
                emit zipProgress(i*100/count);
                QtSleep(1);
                //QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            }
            i += outFile.write(inFile.read(MAXFILEFRAME));
        }
        if(outFile.getZipError() != UNZ_OK)
        {
            IDE_TRACE();
            inFile.close();
            outFile.close();
            zip.close();
            emit message(QString("Zip %1 to %2 error!").arg(pSrcPath).arg(pZipFile));
            emit error();
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            return false;
        }
        outFile.close();
        if (outFile.getZipError() != UNZ_OK)
        {
            IDE_TRACE();
            inFile.close();
            zip.close();
            emit message(QString("Zip %1 to %2 error!").arg(pSrcPath).arg(pZipFile));
            emit error();
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            return false;
        }
        inFile.close();
        zip.close();
        if(zip.getZipError() != UNZ_OK)
        {
            IDE_TRACE();
            emit message(QString("Zip %1 to %2 error!").arg(pSrcPath).arg(pZipFile));
            emit error();
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            return false;
        }

        //>@发送结束信号
        emit end("zip");
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
        return true;
    }
    else if(tmpInfo.isDir())
    {
        if(!tmpInfo.exists())
        {
            IDE_TRACE_STR(pSrcPath);
            emit error();
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            return false;
        }
        QStringList tmpFileList = ErgodicDirectory(pSrcPath);
//        qDebug()<<__PRETTY_FUNCTION__<<"tmpFileList = "<<tmpFileList;
        if (tmpFileList.size() > 0)
        {
            QuaZip zip(pZipFile);
            if(!zip.open(QuaZip::mdCreate))
            {
                IDE_TRACE();
                emit error();
                QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
                return false;
            }
            QuaZipFile outFile(&zip);

            //>@发送开始信号
            emit workStart("zip");
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);

            //>@开始压缩
            int totalf = tmpFileList.size();
            QString tmpParentSrcDir;
            if(mContainDir)
            {
                tmpParentSrcDir = getParentDirectory(pSrcPath);//>@计算pSrcDir的父路径
            }
            else
            {
                tmpParentSrcDir = pSrcPath;
            }
            if(tmpParentSrcDir.endsWith("/") == false)
            {
                tmpParentSrcDir.append("/");
            }
            for (int i = 0; i < totalf; ++i)
            {
                QString tmpSrcPath = tmpFileList.at(i);
                QString tmpString = tmpSrcPath;
                //>@千万注意大小写，否则会在压缩时无法去除绝对路径，导致压缩包内不是相对路径。
                QString tmpNameOfZip = tmpString.replace(tmpParentSrcDir, "", Qt::CaseInsensitive);//>@压缩包内的此文件的文件名
                QFile inFile(tmpSrcPath);
                if(!inFile.open(QIODevice::ReadOnly))
                {
                    IDE_TRACE();
                    emit message(QString("Zip %1 error!").arg(tmpNameOfZip));
                    QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
                    continue;
                }
                if(!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo(tmpNameOfZip, inFile.fileName())))
                {
                    IDE_TRACE();
                    emit message(QString("Zip %1 error!").arg(tmpNameOfZip));
                    QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
                    continue;
                }
                quint64 count = inFile.size();
                quint64 j = 0;
                while(j < count)
                {
                    if(j && !pIngore)
                    {
                        emit zipProgress(j*100/count);
                        QtSleep(1);
                        //QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
                    }
                    j += outFile.write(inFile.read(MAXFILEFRAME));
                }
                if(outFile.getZipError() != UNZ_OK)
                {
                    IDE_TRACE();
                    emit message(QString("Zip %1 error!").arg(tmpNameOfZip));
                    QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
                    continue;
                }
                outFile.close();
                if(outFile.getZipError() != UNZ_OK)
                {
                    IDE_TRACE();
                    emit message(QString("Zip %1 error!").arg(tmpNameOfZip));
                    QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
                    continue;
                }
                inFile.close();

                //>@计算传输的百分比
                quint32 tmpPrg = i*100/totalf;
                if(tmpProgress != tmpPrg)
                {
                    tmpProgress = tmpPrg;
                    emit zipProgress(tmpProgress);
                    emit message(QString("Zip %1 %%2 success").arg(tmpNameOfZip)
                                 .arg(BiteorMega(inFile.size())));
                    QtSleep(1);
                }
            }
            zip.close();
            if(zip.getZipError() != UNZ_OK)
            {
                IDE_TRACE();
                emit message(QString("Zip %1 to %2 error!").arg(pSrcPath).arg(pZipFile));
                emit error();
                QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
                return false;
            }

            if (tmpInfo.isDir() && tmpProgress != 100 &&tmpProgress != 0)
            {
                emit zipProgress(100);
            }
            //>@发送结束信号
            emit end("zip");
            QtSleep(1);
#if (defined(UBUNTU) || defined(LINUX))
            system("sync");
#endif
            return true;
        }
        emit error();
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
        return false;
    }
    emit error();
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"after emit error";
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    return false;
}

void ZipAndUnZip::Unzip(QString str)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
    if (!m_UnZipProcess)
    {
        m_UnZipProcess = new QProcess();
        m_UnzipTimer = new QTimer();
        if (m_UnZipProcess)
        {
            generateAscendRandomNumber();
            connect(m_UnZipProcess, SIGNAL(finished(int)), this, SLOT(sltUnZipFinished(int)));
            connect(m_UnZipProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(sltOcurrErrors(QProcess::ProcessError)));
            QString cmd = QString("unzip -q /home/ftp/pub/%1 -o -d /usr/bst/").arg(str);
            QtSleep(50);
            m_UnZipProcess->start(cmd);  // executable
            if (m_UnzipTimer)
            {
                int seed = QDateTime::currentDateTime().time().second();
                srand(seed);
                int interval = rand()%500 + 800;
                m_UnzipTimer->setInterval(interval);
                connect(m_UnzipTimer, &QTimer::timeout, this, &ZipAndUnZip::sltTimeOut);
                m_UnzipTimer->start();
            }
        }
    }
}

void ZipAndUnZip::sltUnZipFinished(int code)
{
    qDebug()<<__PRETTY_FUNCTION__<<"cmd is executable finished"<<" code = "<<code;
    if (0 == code)
    {
        mIsFinished = 0;  // -1 default -2 failed 0 success
        if (m_UnzipTimer)
        {
            connect(m_UnzipTimer, &QTimer::timeout, this, &ZipAndUnZip::sltTimeOut2);
            m_UnzipTimer->start(600);
        }
    }
    else
    {
        mIsFinished = -2;
        emit error();
    }
    if (m_UnZipProcess)
    {
        QtSleep(100);
        m_UnZipProcess->close();
        disconnect(m_UnZipProcess, 0, 0, 0);
        m_UnZipProcess->deleteLater();
        m_UnZipProcess = 0;
        if (0 == mIsFinished)
        {
            mIsFinished = -1;
        }
        else if (-2 == mIsFinished)
        {
            mIsFinished = -1;
            if (m_UnzipTimer)
            {
                m_UnzipTimer->stop();
                m_UnzipTimer->deleteLater();
            }
        }
    }
}

void ZipAndUnZip::sltOcurrErrors(QProcess::ProcessError error)
{
    qDebug()<<__PRETTY_FUNCTION__<<"error = "<<error;
}

void ZipAndUnZip::sltTimeOut()
{
    static int num = 0;
    ++num;
    if (num < 80)
    {
        emit unZipProgress(numbersList.at(num));
    }
    else
    {
        if (m_UnzipTimer)
        {
            m_UnzipTimer->stop();
            disconnect(m_UnzipTimer, 0, 0, 0);
        }
        num = 0;
    }
}

void ZipAndUnZip::sltTimeOut2()
{
    static int tmpNum = 0;
    if (0 == tmpNum)
    {
        int seed = QDateTime::currentDateTime().time().second();
        srand(seed);
        tmpNum = rand()%10 + 80;
    }
    ++tmpNum;
    emit unZipProgress(tmpNum);
    if (100 == tmpNum)
    {
        tmpNum = 0;
        if (m_UnzipTimer)
        {
            disconnect(m_UnzipTimer, 0, 0, 0);
            m_UnzipTimer->stop();
            m_UnzipTimer->deleteLater();
            m_UnzipTimer = 0;
        }
    }
}


QString ZipAndUnZip::GetDirectoryName(QString pDirPath)
{
    if (pDirPath.isEmpty())
    {
        return QString();
    }
    pDirPath.replace("\\", "/");
    if(!pDirPath.endsWith('/'))
    {
        pDirPath.append('/');
    }
    int index1 = pDirPath.lastIndexOf('/');
    if(index1 < 0)
    {
        return QString();
    }
    int index2 = pDirPath.lastIndexOf('/', index1 - 1);
    if(index2 < 0)
    {
        return QString();
    }
    return pDirPath.mid(index2 + 1, index1 - index2 - 1);
}

QString ZipAndUnZip::getFileDirectory(QString pFilePath)
{
    if (pFilePath.isEmpty())
    {
        return QString();
    }
    pFilePath.replace("\\", "/");
    if(pFilePath.endsWith('/'))
    {
        return QString();
    }
    int index = pFilePath.lastIndexOf('/');
    if(index < 0)
    {
        return QString();
    }
    return  pFilePath.mid(0, index + 1);
}

bool ZipAndUnZip::CreatPath(QString pPath)
{
    QDir dir(pPath);
    if(!dir.exists())
    {
        if(!dir.mkpath(pPath))
        {
            dir.cdUp();
            IDE_TRACE();
            return false;
        }
        dir.cdUp();
    }
    return true;
}

QStringList ZipAndUnZip::ErgodicDirectory(QString d)
{
    QFileInfo tmpFileInfo(d);
    if(!tmpFileInfo.isDir())
    {
        IDE_TRACE();
        return QStringList();
    }
    if(!d.endsWith('/'))
    {
        d.append('/');
    }

    QStringList tmpFileList;
    QDir dir(d);
    if (dir.exists())
    {
        const QFileInfoList list = dir.entryInfoList();
        QFileInfo fi;
        for (int l = 0; l < list.size(); ++l)
        {
            fi = list.at(l);
            if (fi.isDir() && fi.fileName() != "." && fi.fileName() != "..")
            {
                tmpFileList << ErgodicDirectory(fi.absoluteFilePath());
            }
            else if(fi.isFile())
            {
                tmpFileList.append(fi.absoluteFilePath());
            }
        }
    }
    return tmpFileList;
}

QString ZipAndUnZip::getFileName(QString pFilePath)
{
    if (pFilePath.isEmpty())
    {
        return QString();
    }
    QString tmpFileName;
    int index = pFilePath.lastIndexOf("/");
    if(index >= 0)
    {
        tmpFileName =  pFilePath.mid(index + 1);
    }
    else
    {
        tmpFileName = pFilePath;
    }
    return tmpFileName;
}

QString ZipAndUnZip::getParentDirectory(QString pDirPath)
{
    if (pDirPath.isEmpty())
    {
        return QString();
    }
    pDirPath.replace("\\", "/");
    if(!pDirPath.endsWith('/'))
    {
        pDirPath.append('/');
    }
    int index1 = pDirPath.lastIndexOf('/');
    if(index1 < 0)
    {
        return QString();
    }
    int index2 = pDirPath.lastIndexOf('/', index1 - 1);
    if(index2 < 0)
    {
        return QString();
    }
    return pDirPath.mid(0, index2 + 1);
}

QString ZipAndUnZip::BiteorMega(int peso)
{
    QString humanread;
    double canno = peso / 1024;
    int sale = canno;
    if (peso > 0)
    {
        if (canno < 1)
        {
           sale = 1;
        }
    }

   if (sale < 1025)
   {
       humanread = QString("%1.KB").arg(sale);
       return humanread;
   }

   float megad = sale / 1024;

   if (megad < 1025)
   {
       humanread = QString("%1.MB").arg(megad);
       return humanread;
   }
   else
   {
       humanread = QString("1.GB+");
   }
   return humanread;
}

void ZipAndUnZip::generateAscendRandomNumber()
{
    int i;
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    for(i = 0; i < 80; ++i) //生成100个大小在[0, 100]之间的随机数
    {
        numbersList.append(qrand()%80);
    }
    //递增排序
    std::stable_sort(numbersList.begin(), numbersList.end());
    for(i = 0; i < 80; ++i) //生成80个大小在[0, 80]之间的随机数
    {
        qDebug()<<__PRETTY_FUNCTION__<<"num = "<<numbersList.at(i);
    }
}

