#include "zipfunction.h"
#include <QString>
#include "define.h"
#include "zip/quazipfile.h"
#include "zip/quazip.h"
#include <QDir>
ZipFunction::ZipFunction(QThread *parent) : QThread(parent)
{
    mContainDir = true;
}

void ZipFunction::run()
{
    qDebug()<<__PRETTY_FUNCTION__<<QThread::currentThreadId();
    if (m_ZipFlag)
    {
        Zip(m_DirPath, m_ZipFile, true, true);
        m_ZipFlag = false;
    }
    else if (m_UnZipFlag)
    {
        Unzip(m_ZipFile, m_DirPath, true, true);
        m_UnZipFlag = false;
    }
}

void ZipFunction::setZipFilePath(QString dirNamePath, QString zipFilePath)
{
    qDebug()<<__PRETTY_FUNCTION__<<dirNamePath<<zipFilePath;
    m_DirPath = dirNamePath;
    m_ZipFile = zipFilePath;
}

void ZipFunction::setZipFlag(bool zipFlag, bool unZipFlag)
{
    m_ZipFlag = zipFlag;
    m_UnZipFlag = unZipFlag;
}

bool ZipFunction::Zip(QString pSrcPath, QString pZipFile, bool pCover, bool pIngore)
{
    qDebug()<<__PRETTY_FUNCTION__<<pSrcPath<<pZipFile;
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
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"tmpDstPath = "<<tmpDstPath;
    CreatPath(tmpDstPath);
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"after CreatPath(tmpDstPath) = ";
    QFileInfo tmpInfo(pSrcPath);
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"tmpInfo";
    qDebug()<<tmpInfo.isFile()<<tmpInfo.isDir()<<"lines = "<<__LINE__;
    if(tmpInfo.isFile())
    {
        qDebug()<<__PRETTY_FUNCTION__<<"is file";
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
    qDebug()<<__PRETTY_FUNCTION__<<"start zip";
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
        emit workStart();
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);

        //>@开始压缩
        quint64 count = inFile.size();
        qDebug()<<__PRETTY_FUNCTION__<<"count = "<<count;
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
        qDebug()<<__PRETTY_FUNCTION__<<"is dir";
        if(!tmpInfo.exists())
        {
            IDE_TRACE_STR(pSrcPath);
            emit error();
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            return false;
        }
        QStringList tmpFileList = ErgodicDirectory(pSrcPath);
        qDebug()<<__PRETTY_FUNCTION__<<"tmpFileList = "<<tmpFileList;
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
            emit workStart();
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

bool ZipFunction::Unzip(QString pZipFile, QString pDstPath, bool pCover, bool pIngore)
{
    if(pDstPath.endsWith("/") == false)
    {
        pDstPath.append("/");
    }
    pDstPath.replace('\\', '/');
    pZipFile.replace('\\', '/');
    CreatPath(pDstPath);

    if(!QFile::exists(pZipFile))
    {
        IDE_TRACE();
        emit error();
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
        return false;
    }

    QuaZip zip(pZipFile);
    if(!zip.open(QuaZip::mdUnzip))
    {
        IDE_TRACE();
        emit error();
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
        return false;
    }

    //>@发送开始信号
    emit workStart();
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1);

    //>@计算压缩文件中的总文件数
    int total = 0;
    for(bool more = zip.goToFirstFile(); more; more = zip.goToNextFile())
    {
        ++total;
    }
    int i = 0;
    QuaZipFile infile(&zip);
    for(bool more = zip.goToFirstFile(); more; more = zip.goToNextFile())
    {
        QString tmpFileName = zip.getCurrentFileName();
        QString tmpString = pDstPath + tmpFileName;
        IDE_TRACE_STR(tmpString);
        QFile outfile(tmpString);/* extract to path ....... */
        if(outfile.exists())
        {
            if(pCover)
            {
                if(!outfile.setPermissions(QFile::WriteOther))
                {
                    IDE_TRACE();
                    continue;
                }
                if(!outfile.remove())
                {
                    IDE_TRACE();
                    continue;
                }
            }
            else
            {
                IDE_TRACE();
                continue;
            }
        }
        QFileInfo infofile(outfile);
        if(!CreatPath(infofile.absolutePath()))
        {
            IDE_TRACE_STR(infofile.absolutePath());
            continue;
        }
        if(!infile.open(QIODevice::ReadOnly))
        {
            IDE_TRACE_STR(tmpFileName);
            continue;
        }
        if(!outfile.open(QIODevice::WriteOnly))
        {
            IDE_TRACE_STR(tmpString);
            infile.close();
            continue;
        }
        quint64 count = infile.size();
        quint64 j = 0;
        while(j < count)
        {
            if(j && !pIngore)
            {
                emit unZipProgress(j*100/count);
                QtSleep(1);
                //QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            }
            j += outfile.write(infile.read(MAXFILEFRAME));
        }
        if(infile.getZipError() != UNZ_OK)
        {
            IDE_TRACE();
            emit message(QString("Unzip %1 error!").arg(tmpFileName));
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            infile.close();
            outfile.close();
            continue;
        }
        outfile.close();
        infile.close();
        if(infile.getZipError() != UNZ_OK)
        {
            IDE_TRACE();
            emit message(QString("Unzip %1 error!").arg(tmpFileName));
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            continue;
        }

        //>@计算传输的百分比
        emit unZipProgress(++i*100/total); IDE_TRACE_INT(i*100/total);
        emit message(QString("Unzip %1 success!").arg(tmpFileName));
        QtSleep(1);
    }
    zip.close();
    if (i*100 > 100 && (i * 100 /total))
    {
        emit unZipProgress(100);
    }
    if(zip.getZipError() != UNZ_OK)
    {
        IDE_TRACE();
        emit message(QString("Unzip %1 to %2 error!").arg(pZipFile).arg(pDstPath));
        emit error();
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
        return false;
    }

#if (defined(UBUNTU) || defined(LINUX))
    system("sync");
#endif

    //>@发送结束信号
    IDE_TRACE();
    emit end("unZip");
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    return true;
}

QString ZipFunction::GetDirectoryName(QString pDirPath)
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

QString ZipFunction::getFileDirectory(QString pFilePath)
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

bool ZipFunction::CreatPath(QString pPath)
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

QStringList ZipFunction::ErgodicDirectory(QString d)
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

QString ZipFunction::getFileName(QString pFilePath)
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

QString ZipFunction::getParentDirectory(QString pDirPath)
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

QString ZipFunction::BiteorMega(int peso)
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
