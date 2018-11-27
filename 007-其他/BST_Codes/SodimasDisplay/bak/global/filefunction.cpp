#include "filefunction.h"

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

bool Match(QString pData, QString pPattern)
{
    QString pattern(pPattern);
    QRegExp rx(pattern);
    bool match = rx.exactMatch(pData);
    return match;
}

QStringList SplitString(QString pString, QString pKey)
{
    QStringList tmpStringList;
    tmpStringList.clear();
    if(pString.isEmpty())
        return tmpStringList;
    int index1 = 0, index2 = 0;
    int count = pString.length();
    while(index1<count)
    {
        index2 = pString.indexOf(pKey, index1);
        if(index2 < 0)
        {
            tmpStringList << pString.mid(index1);
            return tmpStringList;
        }
        tmpStringList << pString.mid(index1, index2-index1);
        index1 = index2 + 1;
    }
    return tmpStringList;
}

FILEPATH_TYPE GetFilePathType(QString pFile)
{
    if(pFile.isEmpty())
    {
        return PATH_TYPENONE;
    }
    //>@1、查看是否为qrc中的文件
    if(pFile.startsWith(":"))
    {
        return PATH_TYPEQRC;
    }
    //>@2、查看是否为网络流媒体文件
    if(pFile.startsWith("http://") ||
            pFile.startsWith("https://") ||
            pFile.startsWith("rtmp://") ||
            pFile.startsWith("rtsp://") ||
            pFile.startsWith("mms://"))
    {
        return PATH_TYPENETADDR;
    }
    //>@3、查看是否为绝对/相对路径下的文件
    if(QFile::exists(pFile))
    {
        return PATH_TYPEABS;
    }
    return PATH_TYPENONE;
}

QString GetFileName(QString pFilePath)
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

QString GetFileBaseName(QString pFilePath)
{
    QFileInfo tmpFileInfo(pFilePath);
    return tmpFileInfo.baseName();
}

bool IsSupportImg(QFileInfo info)
{
    QString suffix = info.suffix();
    if(suffix.compare("png",Qt::CaseInsensitive)==0){
        return true;
    }else if(suffix.compare("bmp",Qt::CaseInsensitive)==0){
        return true;
    }else if(suffix.compare("jpg",Qt::CaseInsensitive)==0){
        return true;
    }else{
        return false;
    }
}

QString GetFileSuffix(QString pFileName)
{
    QFileInfo tmpFileInfo(pFileName);
    return tmpFileInfo.suffix();
}

QString GetFilePath(QString pFilePath)
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
    return pFilePath.mid(0, index + 1);
#endif
}

//>@ e:/xx/xx/
QString GetParentPath(QString pDirPath)
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

QString GetDirName(QString pDirPath)
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

bool CreatFile(QString pPath)
{
    QFile file(pPath);
    if(file.exists())
        return true;
    CreatPath(GetFilePath(pPath));
    if(!file.open(QFile::WriteOnly))
    {
        IDE_TRACE();
        return false;
    }
    file.close();
    return true;
}

bool CreatPath(QString pPath)
{
    QDir dir(pPath);
    if(!dir.exists())
    {
        if(!dir.mkpath(pPath))
        {
            return false;
        }
    }
    return true;
}

qint64 GetPathSize(QString pDir)
{
    qint64 tmpSize = 0;
    QFileInfo tmpFileInfo(pDir);
    if(tmpFileInfo.isDir())
    {
        if(!pDir.endsWith("/"))
            pDir.append("/");
        QDir tmpDir(pDir);
        QStringList folders = tmpDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (int i = 0; i < folders.size(); ++i)
        {
            QString tmpPath = QString("%1%2").arg(pDir).arg(folders[i]);
            tmpSize += GetPathSize(tmpPath);
        }
        QFileInfoList files = tmpDir.entryInfoList(QDir::Files | QDir::NoSymLinks);
        for (int i = 0; i < files.size(); ++i)
        {
            tmpSize += files.at(i).size();
        }
    }
    else
    {
        tmpSize = tmpFileInfo.size();
    }
    return tmpSize;
}

QStringList GetFileList(QFileInfoList pFileList, bool pContainSize, bool pContainVer)
{
    QStringList tmpFileList;
    foreach(QFileInfo tmpFile, pFileList)
    {
        if(!tmpFile.exists())
            continue;
        QString tmpFilePath = tmpFile.filePath();
        QString tmpFileInfo = tmpFilePath;
        if(pContainSize)
            tmpFileInfo.append(QString("#%1").arg(tmpFile.size()));
        if(pContainVer)
            tmpFileInfo.append(QString("#%1").arg(GetSelfAppVersion(tmpFilePath)));
        tmpFileList.append(tmpFileInfo);
    }
    return tmpFileList;
}

//>@包括子文件夹下的文件。
QFileInfoList ListFolder(QString pDir, bool pSubDir, qint64 &pTotalSize, int *pControl, pShowInfo *pShowFunc)
{
    if(pDir.isEmpty())
        return QFileInfoList();
    pDir.replace("\\", "/");
    if(!pDir.endsWith('/'))
        pDir.append('/');

    return ListFolder(QFileInfo(pDir), pSubDir, pTotalSize, pControl, pShowFunc);
}

QFileInfoList ListFolder(QFileInfo pDirInfo, bool pSubDir, qint64 &pTotalSize, int *pControl, pShowInfo *pShowFunc)
{
    QFileInfoList tmpFileList;
    if(!pDirInfo.isDir())
        return tmpFileList;
    QDir tmpDir(pDirInfo.filePath());
    //>@先处理文件夹.
    if(pSubDir)
    {
        QFileInfoList tmpFolders = tmpDir.entryInfoList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        int count = tmpFolders.count();
        qreal tmpdiff;
        if(pShowFunc){
            tmpdiff = 100.0 / (qreal)count;
        }

        for (int i = 0; i < count; ++i)
        {
            //            if(*pControl == 0){
            //                IDE_TRACE();
            //                return tmpFileList;
            //            }
            QFileInfo tmpSubFolder = tmpFolders.at(i);
            qint64 tmpSize = 0;
            tmpFileList.append(ListFolder(tmpSubFolder, pSubDir, tmpSize, pControl, pShowFunc));
            pTotalSize += tmpSize;
            if(pShowFunc){
                (**pShowFunc)(D_SCANINFO_PRG, tmpdiff * i, QVariant());
            }
        }
        if(pShowFunc)
            (**pShowFunc)(D_SCANINFO_PRG, 100, QVariant());
    }
    //>@再处理文件
    QFileInfoList tmpFiles = tmpDir.entryInfoList(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    for(int i = 0;i<tmpFiles.count();i++)
    {
        //        if(pControl == 0 || *pControl == 0)
        //            return tmpFileList;
        QFileInfo tmpFileInfo = tmpFiles.at(i);
        if(tmpFileInfo.isFile())
        {
            qint64 tmpSize = tmpFileInfo.size();
            //if(tmpSize > 0)  //>@不论文件是否为空，都加入列表
            {
                pTotalSize += tmpSize;
                tmpFileList.append(tmpFileInfo);
            }
        }
    }
    tmpFileList.append(pDirInfo);  //>@包括文件夹，但文件夹没有大小
    return tmpFileList;
}

QFileInfoList SearchFiles(QDir pDir, QString pFile,  QString pSuffix, QString pTxt, Qt::CaseSensitivity pSensitive, bool pSubfolder)
{
    QString tmpDirPath = pDir.path();
    if(!tmpDirPath.endsWith('/'))
        tmpDirPath.append('/');

    QStringList tmpFilter;
    bool tmpEmpty[2];
    tmpEmpty[0] = pFile.isEmpty();
    tmpEmpty[1] = pSuffix.isEmpty();
    if(!tmpEmpty[0])
    {
        if(!tmpEmpty[1])
        {
            QFileInfo tmpInfo(pFile);
            if(tmpInfo.suffix().compare(pSuffix, Qt::CaseInsensitive))
                return QFileInfoList();
        }
        tmpFilter = QStringList(pFile);
    }
    else if(!tmpEmpty[1])
    {
        tmpFilter.append(QString("*.%1").arg(pSuffix));
    }
    else
        return QFileInfoList();

    QFileInfoList tmpFoundFiles = pDir.entryInfoList(tmpFilter, QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    if(!pTxt.isEmpty())
    {
        QFileInfoList tmpRetList;
        for(int i=0; i<tmpFoundFiles.size(); ++i)
        {
            QFileInfo tmpFileInfo = tmpFoundFiles.at(i);
            QFile file(tmpFileInfo.absoluteFilePath());
            if(file.open(QIODevice::ReadOnly))
            {
                QString line;
                QTextStream in(&file);
                while(!in.atEnd())
                {
                    line = in.readLine();
                    if(line.contains(pTxt, pSensitive))
                    {
                        tmpRetList.append(tmpFileInfo);
                        break;
                    }
                }
            }
        }
        return tmpRetList;
    }
    return QFileInfoList();
}

bool CutFile(QString srcFile, QString dstFile, int *pControl, pShowInfo *pShowFunc)
{
    if(!TCopyFile(srcFile, dstFile, pControl, pShowFunc))
        return false;
    return DelFile(srcFile, pControl);
}

bool TCopyFile(QString srcFile, QString dstFile, int *pControl, pShowInfo *pShowFunc)
{
    //>@如果路径相同，则不进行拷贝
#ifdef WINDOWS
    if(srcFile.compare(dstFile, Qt::CaseInsensitive) == 0)
        return true;
#else
    if(srcFile.compare(dstFile) == 0)
        return true;
#endif
    QFileInfo tmpSrcFileInfo(srcFile);
    if(!tmpSrcFileInfo.isFile())
    {
        IDE_TRACE();
        return false;
    }
    QString tmpDstString = GetFilePath(dstFile);
    if(tmpDstString.isEmpty())
    {
        IDE_TRACE();
        return false;
    }
    QDir tmpDstDir;
    if(!tmpDstDir.mkpath(tmpDstString))
    {
        IDE_TRACE();
        return false;
    }
    if(!DelFile(dstFile, pControl))
    {
        IDE_TRACE();
        return false;
    }
    bool flag = false;
    qint64 tmpSize = tmpSrcFileInfo.size();
    if(tmpSize > D_FILEPACKAGE_SIZE)
    {
        QFile tmpSrcFile(srcFile);
        QFile tmpDstFile(dstFile);
        if(!tmpSrcFile.open(QIODevice::ReadOnly))
        {
            IDE_TRACE();
            return false;
        }
        if(!tmpDstFile.open(QIODevice::WriteOnly))
        {
            IDE_TRACE();
            return false;
        }
        flag = true;
        qreal tmpDiff = 0;
        if(pShowFunc)
            tmpDiff = (D_FILEPACKAGE_SIZE * 100) / (qreal)tmpSize;
        int count = 0;
        while(!tmpSrcFile.atEnd())
        {
            tmpDstFile.write(tmpSrcFile.read(100000));
            count++;
            if(pShowFunc)
                (**pShowFunc)(D_FILEINFO_PRG, count*tmpDiff, QVariant());
            QCoreApplication::processEvents();
        }
        tmpSrcFile.close();
        tmpDstFile.close();
    }
    else
    {
        flag = QFile::copy(srcFile, dstFile);
    }
    return flag;
}

bool DelFile(QString pPath, int *pControl, pShowInfo *pShowFunc)
{
    QFileInfo tmpFileInfo(pPath);
    if(!tmpFileInfo.exists())
        return true;
    if(!tmpFileInfo.isFile())
    {
        IDE_TRACE();
        return false;
    }
    QFile file(pPath);
    if(!file.remove())
    {
        IDE_TRACE();
        return false;
    }
    return true;
}

bool CopyFolder(QString pSrcDir, QString pDstDir, bool pSubfolder, int *pControl, pShowInfo *pShowFunc)
{
    QFileInfo tmpFileInfo(pSrcDir);
    if(!tmpFileInfo.isDir())
    {
        IDE_TRACE();
        return false;
    }

    pSrcDir.replace(QString("//"),QString("/"));
    pDstDir.replace(QString("//"),QString("/"));
    if(!pSrcDir.endsWith('/'))
        pSrcDir.append('/');
    if(!pDstDir.endsWith('/'))
        pDstDir.append('/');
    //>@如果路径相同，则不进行拷贝
#ifdef WINDOWS
    if(pSrcDir.compare(pDstDir, Qt::CaseInsensitive) == 0)
        return true;
#else
    if(pSrcDir.compare(pDstDir) == 0)
        return true;
#endif
    if(!CreatPath(pDstDir))
    {
        IDE_TRACE_STR(pDstDir);
        return false;
    }
    qint64 tmpTotalSize = 0;
    qint64 tmpUsedSize = 0;
    QFileInfoList tmpFileList = ListFolder(pSrcDir, pSubfolder, tmpTotalSize, pControl, pShowFunc);
    if(tmpFileList.isEmpty())
        return false;
    foreach(QFileInfo tmpFileInfo, tmpFileList)
    {
        //        if(*pControl == 0)
        //            return false;
        if(!tmpFileInfo.isFile())
            continue;
        QString tmpSrcFilePath = tmpFileInfo.filePath();
        QString tmpSrcFileName = tmpFileInfo.fileName();
        QString tmpDstFilePath = pDstDir + tmpSrcFileName;
        TCopyFile(tmpSrcFilePath, tmpDstFilePath, pControl, pShowFunc);
        if(pShowFunc)
        {
            tmpUsedSize += tmpFileInfo.size();
            int tmpPrg = tmpUsedSize * 100 / tmpTotalSize;
            (**pShowFunc)(D_DIRINFO_PRG, tmpPrg, QVariant());
        }
    }
    return true;
}

bool DeleteDirectory(const QString &path)
{
    if (path.isEmpty())
        return false;

    QDir dir(path);
    if(!dir.exists()){
        //IDE_TRACE();
        return true;
    }

    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    foreach (QFileInfo fi, fileList)
    {
        if (fi.isFile())
            fi.dir().remove(fi.fileName());
        else
            DeleteDirectory(fi.absoluteFilePath());
    }
    return dir.rmpath(dir.absolutePath());
}


bool DelFolder(QString pDir, int *pControl, pShowInfo *pShowFunc)
{
    if(!ClearFolder(pDir, pControl, pShowFunc)){
        IDE_TRACE();
        return false;
    }
    QDir tmpDir;
    if(!tmpDir.rmdir(pDir))
    {
        IDE_TRACE();
        tmpDir.cdUp();
        bool flag = tmpDir.rmdir(pDir);
        //qDebug()<<flag;
        return flag;
    }
    //IDE_TRACE();
    return true;
}

bool ClearFolder(QString pDir, int *pControl, pShowInfo *pShowFunc)  //>@清空文件夹内文件
{
    pDir.replace(QString("//"),QString("/"));
    if(!pDir.endsWith('/'))
        pDir.append('/');
    QFileInfo tmpFileInfo(pDir);
    if(!tmpFileInfo.isDir())
    {
        IDE_TRACE();
        return false;
    }
    qint64 tmpTotalSize = 0;
    qint64 tmpUsedSize = 0;
    QFileInfoList tmpFileList = ListFolder(pDir, true, tmpTotalSize, pControl, pShowFunc);
    if(tmpFileList.isEmpty())
        return false;
    for(int i=0;i<tmpFileList.count()-1;i++)
    {
        //        if(*pControl == 0)
        //            return false;
        QFileInfo tmpFileInfo = tmpFileList.at(i);
        if(tmpFileInfo.isFile())
        {
            DelFile(tmpFileInfo.filePath(), pControl);
            if(pShowFunc)
            {
                tmpUsedSize += tmpFileInfo.size();
                int tmpPrg = tmpUsedSize * 100 / tmpTotalSize;
                (**pShowFunc)(D_DIRINFO_PRG, tmpPrg, QVariant());
            }
        }
        else if(tmpFileInfo.isDir())
        {
            if(tmpFileInfo.exists())
            {
                QString tmpPath = tmpFileInfo.path();
                QDir tmpDir;
                if(!tmpDir.rmdir(tmpPath))
                {
                    tmpDir.cdUp();
                    tmpDir.rmdir(tmpPath);
                }
            }
        }
    }
    return true;
}

bool MoveFolder(QString pSrcDir, QString pDstDir, int *pControl, pShowInfo *pShowFunc)
{
    QFileInfo tmpFileInfo(pSrcDir);
    if(!tmpFileInfo.isDir())
    {
        IDE_TRACE();
        return false;
    }
    pSrcDir.replace(QString("//"),QString("/"));
    pDstDir.replace(QString("//"),QString("/"));
    if(!pSrcDir.endsWith('/'))
        pSrcDir.append('/');
    if(!pDstDir.endsWith('/'))
        pDstDir.append('/');
    //>@如果路径相同，则不进行拷贝
#ifdef WINDOWS
    if(pSrcDir.compare(pDstDir, Qt::CaseInsensitive) == 0)
        return true;
#else
    if(pSrcDir.compare(pDstDir) == 0)
        return true;
#endif
    //>@如果在同级目录，则仅仅是修改文件夹名。
    QString tmpSrcParDir = GetParentPath(pSrcDir);
    QString tmpDstParDir = GetParentPath(pDstDir);
#ifdef WINDOWS
    if(tmpSrcParDir.compare(tmpDstParDir, Qt::CaseInsensitive) == 0)
#else
    if(tmpSrcParDir.compare(tmpDstParDir) == 0)
#endif
    {
        QString tmpSrcName = GetDirName(pSrcDir);
        QString tmpDstName = GetDirName(pDstDir);
        QDir tmpDir;
        return tmpDir.rename(tmpSrcName, tmpDstName);
    }
    if(!CreatPath(pDstDir))
    {
        IDE_TRACE_STR(pDstDir);
        return false;
    }
    qint64 tmpTotalSize = 0;
    qint64 tmpUsedSize = 0;
    QFileInfoList tmpFileList = ListFolder(pSrcDir, true, tmpTotalSize, pControl, pShowFunc);
    if(tmpFileList.isEmpty())
        return false;
    foreach(QFileInfo tmpFileInfo, tmpFileList)
    {
        //        if(*pControl == 0)
        //            return false;
        if(!tmpFileInfo.isFile())
            continue;
        QString tmpSrcFilePath = tmpFileInfo.filePath();
        QString tmpSrcFileName = tmpFileInfo.fileName();
        QString tmpDstFilePath = pDstDir + tmpSrcFileName;
        CutFile(tmpSrcFilePath, tmpDstFilePath, pControl, pShowFunc);
        if(pShowFunc)
        {
            tmpUsedSize += tmpFileInfo.size();
            int tmpPrg = tmpUsedSize * 100 / tmpTotalSize;
            (**pShowFunc)(D_DIRINFO_PRG, tmpPrg, QVariant());
        }
    }
    return true;
}

bool CopyDirectory(const QDir& fromDir, const QDir& toDir, bool bCoverIfFileExists)
{
    QDir formDir_ = fromDir;
    QDir toDir_ = toDir;

    if(!toDir_.exists())
    {
        if(!toDir_.mkdir(toDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = formDir_.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        //拷贝子目录
        if(fileInfo.isDir())
        {
            //递归调用拷贝
            if(!CopyDirectory(fileInfo.filePath(), toDir_.filePath(fileInfo.fileName()),true))
                return false;
        }
        //拷贝子文件
        else
        {
            if(bCoverIfFileExists && toDir_.exists(fileInfo.fileName()))
            {
                toDir_.remove(fileInfo.fileName());
            }
            if(!QFile::copy(fileInfo.filePath(), toDir_.filePath(fileInfo.fileName())))
            {
                return false;
            }
        }
    }
    return true;
}


FileOperate::FileOperate(QObject *parent) :
    QThread(parent)
{
    m_State = S_NONE;
    m_StopFlag = 1;
}

void FileOperate::Stop()
{
    m_StopFlag = 0;
}

void FileOperate::Start(FileOperate::State pType)
{
    if(isBusy())
    {
        Stop();
        wait(1000);
    }
    m_StopFlag = 1;
    m_State = pType;
    start(LowestPriority);
}

bool FileOperate::isBusy()
{
    return isRunning();
}

bool FileOperate::WaitForFinished(quint32 pTimeout)
{
    while(isBusy() && pTimeout)
    {
        pTimeout -= 100;
        QtSleep(100);
    }
    return !isBusy();
}

void FileOperate::CopyFolder(QString pSrcDir, QString pDstDir, bool pSubfolder, pShowInfo *pShowFunc)
{
    m_Src = pSrcDir;
    m_Dst = pDstDir;
    m_SubFolder = pSubfolder;
    m_ShowFunc = pShowFunc;
    Start(S_FOLDER_COPY);
}

void FileOperate::MoveFolder(QString pSrc, QString pDst, pShowInfo *pShowFunc)
{
    m_Src = pSrc;
    m_Dst = pDst;
    m_ShowFunc = pShowFunc;
    Start(S_FOLDER_MOVE);
}

void FileOperate::DelFolder(QString pDir, pShowInfo *pShowFunc)
{
    m_Src = pDir;
    m_ShowFunc = pShowFunc;
    Start(S_FOLDER_DEL);
}

void FileOperate::ClearFolder(QString pDir, pShowInfo *pShowFunc)
{
    m_Src = pDir;
    m_ShowFunc = pShowFunc;
    Start(S_FOLDER_CLR);
}

void FileOperate::CutFile(QString srcFile, QString dstFile, pShowInfo *pShowFunc)
{
    m_Src = srcFile;
    m_Dst = dstFile;
    m_ShowFunc = pShowFunc;
    Start(S_FILE_CUT);
}

void FileOperate::TCopyFile(QString srcFile, QString dstFile, pShowInfo *pShowFunc)
{
    m_Src = srcFile;
    m_Dst = dstFile;
    m_ShowFunc = pShowFunc;
    Start(S_FILE_COPY);
}

void FileOperate::DelFile(QString pPath)
{
    m_Src = pPath;
    m_ShowFunc = (pShowInfo*)0;
    Start(S_FILE_DEL);
}

void FileOperate::run()
{
    bool tmpRet = false;
    emit sChanged(m_State);
    switch(m_State)
    {
    case S_FOLDER_COPY:  //>@CopyFolder
    {
        tmpRet = ::CopyFolder(m_Src, m_Dst, m_SubFolder, &m_StopFlag, m_ShowFunc);
        break;
    }
    case S_FOLDER_MOVE:   //>@MoveFolder
    {
        tmpRet = ::MoveFolder(m_Src, m_Dst, &m_StopFlag, m_ShowFunc);
        break;
    }
    case S_FOLDER_DEL:   //>@DelFolder
    {
        tmpRet = ::DelFolder(m_Src, &m_StopFlag, m_ShowFunc);
        break;
    }
    case S_FOLDER_CLR:   //>@ClearFolder
    {
        tmpRet = ::ClearFolder(m_Src, &m_StopFlag, m_ShowFunc);
        break;
    }
    case S_FILE_CUT:   //>@CutFile
    {
        tmpRet = ::CutFile(m_Src, m_Dst, &m_StopFlag, m_ShowFunc);
        break;
    }
    case S_FILE_COPY:   //>@TCopyFile
    {
        tmpRet = ::TCopyFile(m_Src, m_Dst, &m_StopFlag, m_ShowFunc);
        break;
    }
    case S_FILE_DEL:   //>@DelFile
    {
        tmpRet = ::DelFile(m_Src, &m_StopFlag);
        break;
    }
    default:
    {
        break;
    }
    }
    if(tmpRet)
        m_State = S_NONE;
    else
        m_State = S_ERROR;
    emit sChanged(m_State);
}

QString GetSelfSysVersion()
{
    QFile tmpFile;
    tmpFile.setFileName("/version");
    if(!tmpFile.open(QIODevice::ReadWrite))
    {
        IDE_TRACE();
        return QString();
    }
    QString tmpVersion = tmpFile.readAll();
    tmpFile.close();
    return tmpVersion;
}

QString GetSelfAppVersion(QString pFile)
{
    int tmpVersionPos[2] = {0, 0};
    QFile tmpFile;
    tmpFile.setFileName(pFile);
    IDE_TRACE_STR(pFile);
    if(!tmpFile.open(QIODevice::ReadOnly))
    {
        IDE_TRACE();
        return QString();
    }
    if(tmpFile.size() < 1024)
    {
        tmpFile.close();
        IDE_TRACE();
        return QString();
    }
    int index = tmpFile.size()-100;
    tmpFile.seek(index);
    QByteArray tmpExtend = tmpFile.read(1024);
    tmpFile.close();
    int index1 = tmpExtend.indexOf("VERSION:", 0); //VERSION:1.0.0.1;
    if(index1 < 0)
    {
        // 文件还未设置版本号
        IDE_TRACE();
        return QString();
    }
    index1 += 8;
    tmpVersionPos[0] = index + index1;
    QString tmpVersion;
    int index2 = tmpExtend.indexOf(";", index1);
    if(index2 < 0)
    {
        tmpVersionPos[1] = -1;
        tmpVersion = tmpExtend.mid(index1);
    }
    else
    {
        tmpVersionPos[1] = index + index2;
        tmpVersion = tmpExtend.mid(index2-index1);
    }
    return tmpVersion;
}

