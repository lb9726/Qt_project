#include "global.h"


QString GetDirectoryName(QString pDirPath)
{
#if 0
    QFileInfo tmpFileInfo(pDirPath);
    if(tmpFileInfo.isDir())
    {
        return tmpFileInfo.dir().dirName();
    }
    return QString();
#else
    if (pDirPath.isEmpty())
        return QString();
    pDirPath.replace("\\", "/");
    if(!pDirPath.endsWith('/'))
        pDirPath.append('/');
    int index1 = pDirPath.lastIndexOf('/');
    if(index1 < 0)
        return QString();
    int index2 = pDirPath.lastIndexOf('/', index1-1);
    if(index2 < 0)
        return QString();
    return pDirPath.mid(index2 + 1, index1 - index2 - 1);
#endif
}

QString getFileDirectory(QString pFilePath)
{
#if 0
    QFileInfo tmpFileInfo(pFilePath);
    if(tmpFileInfo.isFile())
    {
        return tmpFileInfo.dir().path();
    }
    return QString();
#else
    if (pFilePath.isEmpty())
        return QString();
    pFilePath.replace("\\", "/");
    if(pFilePath.endsWith('/'))
        return QString();
    int index = pFilePath.lastIndexOf('/');
    if(index < 0)
        return QString();
    return  pFilePath.mid(0, index + 1);
#endif
}

bool CreatPath(QString pPath)
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

QStringList ErgodicDirectory(QString d)
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

QString getFileName(QString pFilePath)
{
#if 1
    QFileInfo tmpFileInfo(pFilePath);
    return tmpFileInfo.fileName();
#else
    if (pFilePath.isEmpty())
        return QString();
    QString tmpFileName;
    int index = pFilePath.lastIndexOf("/");
    if(index >= 0)
        tmpFileName =  pFilePath.mid(index + 1);
    else
        tmpFileName = pFilePath;
    return tmpFileName;
#endif
}

QString getParentDirectory(QString pDirPath)
{
#if 0
    QFileInfo tmpFileInfo(pDirPath);
    if(tmpFileInfo.isDir())
    {
        QDir tmpDir = tmpFileInfo.dir();
        if(tmpDir.cdUp())
            return tmpDir.dirName();
    }
    return QString();
#else
    if (pDirPath.isEmpty())
        return QString();
    pDirPath.replace("\\", "/");
    if(!pDirPath.endsWith('/'))
        pDirPath.append('/');
    int index1 = pDirPath.lastIndexOf('/');
    if(index1 < 0)
        return QString();
    int index2 = pDirPath.lastIndexOf('/', index1-1);
    if(index2 < 0)
        return QString();
    return pDirPath.mid(0, index2 + 1);
#endif
}

QString  BiteorMega(int peso)
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





