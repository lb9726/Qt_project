#include "zipfunction.h"
#include <QString>
#include "define.h"
#include "global.h"
#include "quazip.h"
#include "quazipfile.h"

ZipFunction::ZipFunction(QObject *parent) : QObject(parent)
{
    mContainDir = true;
    connect(this, &ZipFunction::startZip, this, &ZipFunction::Zip, Qt::QueuedConnection);
    connect(this, &ZipFunction::startUnZip, this, &ZipFunction::Unzip, Qt::QueuedConnection);
    this->moveToThread(&mZipTaskThread);
    mZipTaskThread.start();
}

//bool ZipFunction::Zip(QString pSrcPath, QString pDstPath)
//{
//    QString tmpString = GetDirectoryName(pSrcPath);
//    if(pDstPath.endsWith('/') == false)
//        pDstPath.append('/');
//    return Zip(pSrcPath, pDstPath + tmpString, true);
//}

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
        emit start();
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);

        //>@开始压缩
        quint64 count = inFile.size();
        qDebug()<<__PRETTY_FUNCTION__<<"count = "<<count;
        quint64 i = 0;
        while(i < count)
        {
            if(i)
            {
                emit progress(i*100/count);
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
        emit end();
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
            emit start();
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
                        emit progress(j*100/count);
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
                    emit progress(tmpProgress);
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
                emit progress(100);
            }
            //>@发送结束信号
            emit end();
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
    emit start();
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
                emit progress(j*100/count);
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
        emit progress(++i*100/total); IDE_TRACE_INT(i*100/total);
        emit message(QString("Unzip %1 success!").arg(tmpFileName));
        QtSleep(1);
    }
    zip.close();
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
    emit end();
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    return true;
}
