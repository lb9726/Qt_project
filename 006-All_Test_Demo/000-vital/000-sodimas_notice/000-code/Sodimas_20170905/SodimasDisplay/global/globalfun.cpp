#include "globalfun.h"


UI_TYPE getUiType(QString pName)
{
    if(pName.isEmpty())
        return UI_UNKNOW;
    else if(pName.compare(QString("enum"), Qt::CaseInsensitive) == 0)
        return UI_ENUM;
    else if(pName.compare(QString("string"), Qt::CaseInsensitive) == 0)
        return UI_STRING;
    else if(pName.compare(QString("num"), Qt::CaseInsensitive) == 0)
        return UI_NUM;
    else if(pName.compare(QString("check"), Qt::CaseInsensitive) == 0)
        return UI_CHECK;
    else if(pName.compare(QString("bool"), Qt::CaseInsensitive) == 0)
        return UI_BOOL;
    return UI_UNKNOW;
}

QString getUiString(UI_TYPE pType)
{
    if(UI_ENUM == pType)
        return QString("enum");
    else if(UI_STRING == pType)
        return QString("string");
    else if(UI_NUM == pType)
        return QString("num");
    else if(UI_CHECK == pType)
        return QString("check");
    else if(UI_BOOL == pType)
        return QString("bool");
    return QString();
}



ROTATE_TYPE getRotateType(int angle){
    if(angle == 0)
        return ROTATE_0;
    else if(angle == 90)
        return ROTATE_90;
    else if(angle == 180)
        return ROTATE_180;
    else if(angle == 270)
        return ROTATE_270;
    else{
        return ROTATE_INVALID;
    }
}

int getRotateAngle(ROTATE_TYPE toType){
    if(toType == ROTATE_0)
        return 0;
    else if(toType == ROTATE_90)
        return 90;
    else if(toType == ROTATE_180)
        return 180;
    else if(toType == ROTATE_270)
        return 270;
    else{
        return -1;
    }
}



AREA_OPERATE getOperateType(QString pType)
{
    if(pType.compare("RESOURCE", Qt::CaseInsensitive) == 0)
        return OPERATE_RC;
    else if(pType.compare("MAP", Qt::CaseInsensitive) == 0)
        return OPERATE_MAP;
    else if(pType.compare("FORM", Qt::CaseInsensitive) == 0)
        return OPERATE_FORM;
    else if(pType.compare("INIT", Qt::CaseInsensitive) == 0)
        return OPERATE_INIT;
    else if(pType.compare("RELEASE", Qt::CaseInsensitive) == 0)
        return OPERATE_RELEASE;
    else if(pType.compare("HASTATE", Qt::CaseInsensitive) == 0)
        return OPERATE_CHANGE;
    else if(pType.compare("SLIP", Qt::CaseInsensitive) == 0)
        return OPERATE_SLIP;
    else if(pType.compare("TURN", Qt::CaseInsensitive) == 0)
        return OPERATE_TURN;
    else if(pType.compare("ADD", Qt::CaseInsensitive) == 0)
        return OPERATE_ADD;
    else if(pType.compare("MODIFY", Qt::CaseInsensitive) == 0)
        return OPERATE_MODIFY;
    else if(pType.compare("DEL", Qt::CaseInsensitive) == 0)
        return OPERATE_DEL;
    else if(pType.compare("SHOW", Qt::CaseInsensitive) == 0)
        return OPERATE_SHOW;
    else if(pType.compare("HIDE", Qt::CaseInsensitive) == 0)
        return OPERATE_HIDE;
    else if(pType.compare("RUN", Qt::CaseInsensitive) == 0)
        return OPERATE_RUN;
    else if(pType.compare("HALT", Qt::CaseInsensitive) == 0)
        return OPERATE_HALT;
    else if(pType.compare("PLAY", Qt::CaseInsensitive) == 0)
        return OPERATE_PLAY;
    else if(pType.compare("PAUSE", Qt::CaseInsensitive) == 0)
        return OPERATE_PAUSE;
    else if(pType.compare("STOP", Qt::CaseInsensitive) == 0)
        return OPERATE_STOP;
    else if(pType.compare("STEADY", Qt::CaseInsensitive) == 0)
        return OPERATE_STEADY;
    else if(pType.compare("VOCAL", Qt::CaseInsensitive) == 0)
        return OPERATE_VOCAL;
    else if(pType.compare("MUTE", Qt::CaseInsensitive) == 0)
        return OPERATE_MUTE;
    else if(pType.compare("RESUME", Qt::CaseInsensitive) == 0)
        return OPERATE_RESUME;
    else if(pType.compare("REPEAT", Qt::CaseInsensitive) == 0)
        return OPERATE_REPEAT;
    else if(pType.compare("KEYPRESS", Qt::CaseInsensitive) == 0)
        return OPERATE_KEYPRESS;
    else if(pType.compare("KEYLIGHT", Qt::CaseInsensitive) == 0)
        return OPERATE_KEYLIGHT;
    else if(pType.compare("KEYCANCEL", Qt::CaseInsensitive) == 0)
        return OPERATE_KEYCANCEL;
    else if(pType.compare("KEYMOVE", Qt::CaseInsensitive) == 0)
        return OPERATE_KEYMOVE;
    else if(pType.compare("KEYRELEASE", Qt::CaseInsensitive) == 0)
        return OPERATE_KEYRELEASE;
    return OPERATE_NONE;
}
QString getOperateString(AREA_OPERATE pType)
{
    switch(pType)
    {
        case OPERATE_FORM:
            return QString("FORM");
        case OPERATE_RC:
            return QString("RESOURCE");
        case OPERATE_MAP:
            return QString("MAP");
        case OPERATE_INIT:
            return QString("INIT");
        case OPERATE_RELEASE:
            return QString("RELEASE");
        case OPERATE_CHANGE:
            return QString("HASTATE");
        case OPERATE_SLIP:
            return QString("SLIP");
        case OPERATE_TURN:
            return QString("TURN");
        case OPERATE_ADD:
            return QString("ADD");
        case OPERATE_MODIFY:
            return QString("MODIFY");
        case OPERATE_DEL:
            return QString("DEL");
        case OPERATE_SHOW:
            return QString("SHOW");
        case OPERATE_HIDE:
            return QString("HIDE");
        case OPERATE_RUN:
            return QString("RUN");
        case OPERATE_HALT:
            return QString("HALT");
        case OPERATE_PLAY:
            return QString("PLAY");
        case OPERATE_PAUSE:
            return QString("PAUSE");
        case OPERATE_STOP:
            return QString("STOP");
        case OPERATE_STEADY:
            return QString("STEADY");
        case OPERATE_VOCAL:
            return QString("VOCAL");
        case OPERATE_MUTE:
            return QString("MUTE");
        case OPERATE_RESUME:
            return QString("RESUME");
        case OPERATE_REPEAT:
            return QString("REPEAT");
        case OPERATE_KEYPRESS:
            return QString("KEYPRESS");
        case OPERATE_KEYLIGHT:
            return QString("KEYLIGHT");
        case OPERATE_KEYCANCEL:
            return QString("KEYCANCEL");
        case OPERATE_KEYMOVE:
            return QString("KEYMOVE");
        case OPERATE_KEYRELEASE:
            return QString("KEYRELEASE");
        default:
            break;
    }
    return QString("UNKNOW");
}




int getKey(QString pString, QString pPrefix)
{
    if(pString.isEmpty())
        return -1;
    int pos = pString.lastIndexOf('.');
    QString tmpName;
    if(pos < 0)
        tmpName = pString;
    else
        tmpName = pString.mid(0, pos);
    //IDE_TRACE_STR(tmpName);
    if(pPrefix.isEmpty())
        return tmpName.toInt();
    if(tmpName.startsWith(pPrefix,Qt::CaseInsensitive) == false)
        return tmpName.toInt();
    tmpName.remove(0, pPrefix.count());
    bool ok = false;
    int index = tmpName.toInt(&ok, 10);
    if(ok == false)
        return -1;
    return index;
}

DIRECTION getDirection(QString pDirection)
{
    if(pDirection.isEmpty())
        return HVNone;
    if(pDirection.compare("V") == 0)
        return Vertical;
    else if(pDirection.compare("H") == 0)
        return Horizontal;
    else if(pDirection.compare("HV") == 0)
        return HVBoth;
    return HVNone;
}

QString getDirectionString(DIRECTION pDirection)
{
    QString tmpDirString;
    if(pDirection == Vertical)
        tmpDirString = "v";
    else if(pDirection == Horizontal)
        tmpDirString = "h";
    else if(pDirection == HVBoth)
        tmpDirString = "hv";
    return tmpDirString;
}

COM_TYPE getComponetType(QString ComponentName)
{
    COM_TYPE tmpComType = COM_INVALID;
    if (ComponentName.isNull())
        return tmpComType;
    else if (ComponentName.isEmpty() ||
             ComponentName.compare(QString("desktop"), Qt::CaseInsensitive) == 0 ||
             ComponentName.compare(QString("/"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = COM_DESKTOP;

    }    
    else if(ComponentName.compare(QString("messagebar"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = COM_MESSAGEBAR;
    }
    else if(ComponentName.compare(QString("progressbar"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = COM_PROGRESSBAR;
    }
    else if(!ComponentName.compare(QString("floor"), Qt::CaseInsensitive))
    {
        tmpComType = COM_FLOOR;
    }
    else if(!ComponentName.compare(QString("arrow"), Qt::CaseInsensitive))
    {
        tmpComType = COM_ARROW;
    }
    else if(!ComponentName.compare(QString("function"), Qt::CaseInsensitive))
    {
        tmpComType = COM_FUNCTION;
    }
    else if(!ComponentName.compare(QString("media"), Qt::CaseInsensitive))
    {
        tmpComType = COM_MEDIA;
    }
    else if(!ComponentName.compare(QString("fulltext"), Qt::CaseInsensitive))
    {
        tmpComType = COM_FULLTEXT;
    }
    else if(!ComponentName.compare(QString("realtimetext"), Qt::CaseInsensitive))
    {
        tmpComType = COM_REALTIMETEXT;
    }
    else if(!ComponentName.compare(QString("logo"), Qt::CaseInsensitive))
    {
        tmpComType = COM_LOGO;
    }else{

    }
    return tmpComType;
}

QString getComponetName(COM_TYPE ComponentType)
{
    switch(ComponentType)
    {
    case COM_DESKTOP:
        return QString("desktop");
    case COM_MESSAGEBAR:
        return QString("messagebar");
    case COM_PROGRESSBAR:
        return QString("progressbar");
    case COM_FLOOR:
        return QString("floor");
    case COM_ARROW:
        return QString("arrow");
    case COM_FUNCTION:
        return QString("function");
    case COM_MEDIA:
        return QString("media");
    case COM_FULLTEXT:
        return QString("fulltext");
    case COM_REALTIMETEXT:
        return QString("realtimetext");
    case COM_LOGO:
        return QString("logo");
    default:
        return QString();
    }
    return QString();
}

QString getComponetPath(COM_TYPE pType)
{
    if(pType == COM_DESKTOP)
        return QString("/");
    QString tmpName = getComponetName(pType);
    if(!tmpName.isEmpty())
        return tmpName.prepend('/');
    return tmpName;
}

PH_Type getPHType(QString pPara)
{
    PH_Type PHType = P_INVALID;
    if(pPara.isEmpty())
        return PHType;
    if(pPara.compare("player", Qt::CaseInsensitive) == 0)
        PHType = P_Player;
    else if(pPara.compare("lanc", Qt::CaseInsensitive) == 0)
        PHType = P_LANC;
    else if(pPara.compare("lanr", Qt::CaseInsensitive) == 0)
        PHType = P_LANR;
    else if(pPara.compare("lcd", Qt::CaseInsensitive) == 0)
        PHType = P_LCD;
    else if(pPara.compare("led", Qt::CaseInsensitive) == 0)
        PHType = P_LED;
    else if(pPara.compare("com", Qt::CaseInsensitive) == 0)
        PHType = P_COM;
    return PHType;
}

PH_Type getElementPHType(QDomElement pDevElement)
{
    if(pDevElement.isNull())
        return P_INVALID;
    return getPHType(pDevElement.tagName());
}

LG_Type getLGType(QString pPara)
{
    LG_Type LGType = L_INVALID;
    if(pPara.isEmpty())
        return LGType;

    if(pPara.compare("thememanager", Qt::CaseInsensitive) == 0)
        LGType = L_ThemeManager;
    else if(pPara.compare("contentmanager", Qt::CaseInsensitive) == 0)
        LGType = L_ContentManager;
    return LGType;
}

DEV_TYPE getDevType(QString pDevName)
{
    if(pDevName.contains("physical"))
        return DEV_PHYSICAL;
    else if(pDevName.contains("logic"))
        return DEV_LOGIC;
    return DEV_NONE;
}


bool isFileExist(QString FileName)
{
    if(FileName.isEmpty())
        return false;
    QFile file(FileName);
    bool flag = file.exists();
    return flag;
}

COM_TYPE getParentComType(COM_TYPE pType)
{
    if(pType > COM_STARTDEF && pType < COM_ENDDEF)
    {
        return COM_DESKTOP;
    }
    return COM_INVALID;
}

COM_TYPE getParentComType(QString ComPath)
{
    if(ComPath.isEmpty())
        return COM_INVALID;
    int index1,index2;
    QString tmpString;
    index1 = ComPath.indexOf('/', Qt::CaseInsensitive);
    if(index1<0)
        return COM_INVALID;
    index2 = ComPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2<0) ///desktop and rect
    {
        tmpString = ComPath.mid(index1);
        if(tmpString.length() == 1)
        {
            return COM_INVALID;
        }
        return COM_DESKTOP;
    }
    else
    {
        tmpString = ComPath.mid(index1+1, index2-index1-1);
    }
    return getComponetType(tmpString);
}

QString getCurComName(QString ComPath)
{
    if(ComPath.isEmpty())
        return QString();
    int index1,index2;
    index1 = ComPath.indexOf('/', Qt::CaseInsensitive);
    if(index1<0)
    {
        if(ComPath.compare(QString("desktop"), Qt::CaseInsensitive) == 0)
            return QString("desktop");
        else
            return QString();
    }
    QString tmpString;
    index2 = ComPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2<0)
    {
        tmpString = ComPath.mid(index1+1);
        if(tmpString.isEmpty())
            return QString("desktop");     ///Empty表示Desktop
        else
            return tmpString;
    }
    else
    {
        tmpString = ComPath.mid(index2+1);
    }
    return tmpString;
}

COM_TYPE getCurComType(QString ComPath)
{
    QString tmpString;
    int index1,index2;
    index1 = ComPath.indexOf('/', Qt::CaseInsensitive);
    if(index1<0)
    {
        return getComponetType(ComPath);
    }
    index2 = ComPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2<0)
    {
        tmpString = ComPath.mid(index1+1);
        if(tmpString.isEmpty())
            return COM_DESKTOP;     ///Empty表示Desktop
    }
    else
    {
        tmpString = ComPath.mid(index2+1);
    }
    return getComponetType(tmpString);
}


RC_TYPE getRcType(QString FileName)
{
    if (FileName.isEmpty())
        return RC_INVALID;
    if(FileName.endsWith(".bmp",Qt::CaseInsensitive))
    {
        return RC_BMP;
    }
    else if(FileName.endsWith(".jpg",Qt::CaseInsensitive))
    {
        return RC_JPG;
    }
    else if(FileName.endsWith(".png",Qt::CaseInsensitive))
    {
        return RC_PNG;
    }
    else if(FileName.endsWith(".gif",Qt::CaseInsensitive))
    {
        return RC_GIF;
    }
    else if(FileName.endsWith(".svg",Qt::CaseInsensitive))
    {
        return RC_SVG;
    }
    else if(FileName.endsWith(".qml",Qt::CaseInsensitive))
    {
        return RC_QML;
    }
    else if(FileName.endsWith(".avi",Qt::CaseInsensitive))
    {
        return RC_AVI;
    }
    else if(FileName.endsWith(".wav",Qt::CaseInsensitive))
    {
        return RC_WAV;
    }
    else if(FileName.endsWith(".mp3",Qt::CaseInsensitive))
    {
        return RC_MP3;
    }
    else if(FileName.endsWith(".mp4",Qt::CaseInsensitive))
    {
        return RC_MP4;
    }
    else if(FileName.endsWith(".flv",Qt::CaseInsensitive))
    {
        return RC_FLV;
    }
    else if(FileName.endsWith(".rmvb",Qt::CaseInsensitive))
    {
        return RC_RMVB;
    }
    else if(FileName.endsWith(".font",Qt::CaseInsensitive))
    {
        return RC_FONT;
    }
    else if(FileName.endsWith(".3ds",Qt::CaseInsensitive))
    {
        return RC_3DS;
    }
    else if(FileName.endsWith(".html",Qt::CaseInsensitive))
    {
        return RC_HTML;
    }
    else if(FileName.endsWith(".xml",Qt::CaseInsensitive))
    {
        return RC_XML;
    }
    return RC_INVALID;
}

FILEPATH_TYPE getRcFileType(QString pFileName)
{
    if(pFileName.isEmpty())
    {
        return PATH_TYPENONE;
    }
    ///1、查看是否为qrc中的文件
    if(pFileName.startsWith(":"))
    {
        return PATH_TYPEQRC;
    }
    ///2、查看是否为网络流媒体文件
    if(pFileName.startsWith("http://") ||
       pFileName.startsWith("https://") ||
       pFileName.startsWith("rtmp://") ||
       pFileName.startsWith("rtsp://") ||
       pFileName.startsWith("mms://"))
    {
        return PATH_TYPENETADDR;
    }
    ///3、查看是否为绝对/相对路径下的文件
    if(isFileExist(pFileName))
    {
        return PATH_TYPEABS;
    }
    return PATH_TYPENONE;
}


QString getFileName(QString pFilePath)
{
    QFileInfo tmpFileInfo(pFilePath);
    return tmpFileInfo.fileName();
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

QString getParentDirectory(QString pDirPath)
{
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

}

QString getDirectoryName(QString pDirPath)
{
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
}

QRect getRectFromString(QString pRectString)
{
    if(pRectString.isEmpty())
        return QRect();
    QStringList tmpList = pRectString.split(",");
    if(tmpList.count() != 4)
    {
        return QRect();
    }
    int tmpX = tmpList.at(0).toInt();
    int tmpY = tmpList.at(1).toInt();
    int tmpW = tmpList.at(2).toInt();
    int tmpH = tmpList.at(3).toInt();
    if(tmpW<=0 || tmpH<=0)
    {
        return QRect();
    }
    return QRect(tmpX, tmpY, tmpW, tmpH);
}

QStringList findFilesInFloder(QString DirName, QString fileName, bool subfolder)
{
    QFileInfo tmpFileInfo(DirName);
    if(!tmpFileInfo.isDir())
    {
        IDE_TRACE();
        return QStringList();
    }
    if(!DirName.endsWith('/'))
        DirName.append('/');
    QDir dir = QDir(DirName);
    QStringList findFileList;
    if(subfolder)
    {
        QStringList folders;
        folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        if(folders.size() > 0)
        {
            for (int i = 0; i < folders.size(); ++i)
            {
                findFileList << findFilesInFloder(DirName + folders[i], fileName, true);
            }
        }
    }
    findFileList << findFiles(dir, fileName, QString(""), Qt::CaseSensitive);
    return findFileList;
}

QStringList findFiles(const QDir &dir,const QString &fileName,const QString &txt,Qt::CaseSensitivity sensitive)
{
    QString tmpDirPath = dir.path();
    if(!tmpDirPath.endsWith('/'))
        tmpDirPath.append('/');
    QStringList foundFiles;
    QStringList files;
    if(fileName.isEmpty())
        files = dir.entryList(QDir::Files | QDir::NoSymLinks);
    else
        files = dir.entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks);
    if(txt.isEmpty())
    {
        for(int i=0;i<files.count();i++)
        {
            foundFiles.append(tmpDirPath + files.at(i));
        }
    }
    else
    {
        for (int i = 0; i < files.size(); ++i)
        {
            QFile file(dir.absoluteFilePath(files[i]));
            if (file.open(QIODevice::ReadOnly))
            {
                QString line;
                QTextStream in(&file);
                while (!in.atEnd())
                {
                    line = in.readLine();
                    if (line.contains(txt, sensitive))
                    {
                        foundFiles.append(tmpDirPath + files[i]);
                        break;
                    }
                }
            }
        }
    }
    return foundFiles;
}

QStringList searchFiles(QString pDirPath, QString pSuffix, bool pSubfolder)
{
    QFileInfo tmpFileInfo(pDirPath);
    if(!tmpFileInfo.isDir())
    {
        IDE_TRACE();
        return QStringList();
    }
    if(!pDirPath.endsWith('/'))
        pDirPath.append('/');
    QDir dir = QDir(pDirPath);
    QStringList findFileList;
    if(pSubfolder)
    {
        QStringList folders;
        folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        if(folders.size() > 0)
        {
            for (int i = 0; i < folders.size(); ++i)
            {
                findFileList << searchFiles(pDirPath + folders[i], pSuffix, pSubfolder);
            }
        }
    }

    QStringList tmpFileList = dir.entryList(QStringList(QString("*.%1").arg(pSuffix)), QDir::Files | QDir::NoSymLinks);
    for(int i=0;i<tmpFileList.count();i++)
    {
        findFileList.append(pDirPath + tmpFileList.at(i));
    }
    return findFileList;
}

QStringList ergodicDirectory(QString d)
{
    QFileInfo tmpFileInfo(d);
    if(!tmpFileInfo.isDir())
    {
        IDE_TRACE();
        return QStringList();
    }
    if(!d.endsWith('/'))
        d.append('/');

    QStringList tmpFileList;
    QDir dir(d);
    if (dir.exists())
    {
        const QFileInfoList list = dir.entryInfoList();
        QFileInfo fi;
        for (int l = 0; l < list.size(); l++)
        {
            fi = list.at(l);
            if (fi.isDir() && fi.fileName() != "." && fi.fileName() != "..")
            {
                tmpFileList << ergodicDirectory(fi.absoluteFilePath());
            }
            else if(fi.isFile())
            {
                tmpFileList.append(fi.absoluteFilePath());
            }
        }
    }
    return tmpFileList;
}

bool cutFile(QString srcFile, QString dstFile)
{
    if(!copyFile(srcFile, dstFile))
        return false;
    return delFile(srcFile);
}

bool copyFile(QString srcFile, QString dstFile, QString pPrefix)
{
    //如果路径相同，则不进行拷贝
    if(srcFile.compare(dstFile, Qt::CaseInsensitive) == 0)
        return true;
    QFileInfo tmpFileInfo(srcFile);
    if(!tmpFileInfo.isFile())
    {
        IDE_TRACE();
        return false;
    }
    QString tmpDstString = getFileDirectory(dstFile);
    QDir tmpDstDir;
    if(!tmpDstDir.mkpath(tmpDstString))
    {
        IDE_TRACE();
        return false;
    }
    if(!pPrefix.isEmpty())
    {
        QString tmpName = getFileName(dstFile);
        if(!tmpName.startsWith(pPrefix))
        {
            if(!tmpDstString.endsWith('/'))
                tmpDstString.append('/');
            dstFile = tmpDstString + pPrefix + tmpName;
        }
    }
    if(!delFile(dstFile))
    {
        IDE_TRACE();
        return false;
    }
    bool flag = QFile::copy(srcFile, dstFile);
#if (defined(UBUNTU) || defined(LINUX))
    if(flag)
    {
        system("sync");
    }
#endif
    return flag;
}

bool delFile(QString pPath)
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
    if(!file.setPermissions(QFile::WriteOther))
    {
        IDE_TRACE();
    }
    if(!file.remove())
    {
        IDE_TRACE();
        return false;
    }
#if (defined(UBUNTU) || defined(LINUX))
    system("sync");
#endif
    return true;
}

bool creatFile(QString pPath)
{
    QFile file(pPath);
    if(file.exists())
        return true;
    creatPath(getFileDirectory(pPath));
    if(!file.open(QFile::WriteOnly))
    {
        IDE_TRACE();
        return false;
    }
    file.close();
#if (defined(UBUNTU) || defined(LINUX))
    system("sync");
#endif
    return true;
}

bool creatPath(QString pPath)
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

bool copyFolder(QString pSrcDir, QString pDstDir, bool pSubfolder, bool pCopyFile, bool pIdel)
{
    QFileInfo tmpFileInfo(pSrcDir);
    if(!tmpFileInfo.isDir())
    {
        IDE_TRACE();
        return false;
    }
    if(!pSrcDir.endsWith('/'))
        pSrcDir.append('/');
    if(!pDstDir.endsWith('/'))
        pDstDir.append('/');
    //如果路径相同，则不进行拷贝
    if(pSrcDir.compare(pDstDir, Qt::CaseInsensitive) == 0)
        return true;
    if(!creatPath(pDstDir))
    {
        IDE_TRACE_STR(pDstDir);
        return false;
    }
    QDir tmpSrcDir(pSrcDir);
    if(pSubfolder)
    {
        QStringList folders = tmpSrcDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (int i = 0; i < folders.size(); ++i)
        {
            QString tmpSrcPath = QString("%1%2").arg(pSrcDir).arg(folders[i]);
            QString tmpDstPath = QString("%1%2").arg(pDstDir).arg(folders[i]);
            copyFolder(tmpSrcPath, tmpDstPath, pSubfolder, pCopyFile, pIdel);
        }
    }
    if(pCopyFile)
    {
        QStringList files = findFiles(tmpSrcDir, QString(), QString(), Qt::CaseInsensitive);
        for(int i = 0; i < files.size(); ++i)
        {
            QString tmpDstFilePath = QString("%1%2").arg(pDstDir).arg(getFileName(files[i]));
            if(pIdel)
                QtSleep(10);
            copyFile(files[i], tmpDstFilePath);
        }
    }
    return true;
}
//清空文件夹内文件
bool clearFolder(QString pDir)
{
    if(!delFolder(pDir))
        return false;
    return creatPath(pDir);
}

bool delFolder(QString pDir, bool pIdel)
{
    QFileInfo tmpFileInfo(pDir);
    if(!tmpFileInfo.isDir())
    {
        IDE_TRACE();
        return false;
    }
    if(!pDir.endsWith('/'))
        pDir.append('/');
    QDir dir(pDir);
    QFileInfoList fileInfoList = dir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;
        if(fileInfo.isDir())
        {
            QString tmpdir = pDir + fileInfo.fileName();
            if(!delFolder(tmpdir))
            {
                return false;
            }
        }
        else if(fileInfo.isFile())
        {
            if(pIdel)
                QtSleep(10);
            if(!delFile(fileInfo.filePath()))
            {
                return false;
            }
        }
    }
    if(!dir.cdUp())            /**< 返回上级目录，因为只有返回上级目录，才可以删除这个目录 */
    {
        IDE_TRACE();
        return false;
    }
    if(dir.exists(pDir))
    {
        if(!dir.rmdir(pDir))
        {
            IDE_TRACE();
            return false;
        }
    }
    return true;
}

bool moveFolder(QString pSrcDir, QString pDstDir)
{
    QFileInfo tmpFileInfo(pSrcDir);
    if(!tmpFileInfo.isDir())
    {
        IDE_TRACE();
        return false;
    }
    if(!pSrcDir.endsWith('/'))
        pSrcDir.append('/');
    if(!pDstDir.endsWith('/'))
        pDstDir.append('/');
    //如果路径相同，则不进行拷贝
    if(pSrcDir.compare(pDstDir, Qt::CaseInsensitive) == 0)
        return true;
    QDir tmpDir(pSrcDir);
    if(!tmpDir.exists())
        return false;
    if(!creatPath(pDstDir))
        return false;
    //先移动子文件夹
    QDir tmpSrcDir(pSrcDir);
    QStringList folders = tmpSrcDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (int i = 0; i < folders.size(); ++i)
    {
        QString tmpSrcPath = QString("%1%2").arg(pSrcDir).arg(folders[i]);
        QString tmpDstPath = QString("%1%2").arg(pDstDir).arg(folders[i]);
        moveFolder(tmpSrcPath, tmpDstPath);
    }
    //再移动文件
    QStringList files = findFiles(tmpSrcDir, QString(), QString(), Qt::CaseInsensitive);
    for(int i = 0; i < files.size(); ++i)
    {
        QString tmpString = QString("%1%2").arg(pDstDir).arg(getFileName(files[i]));
        cutFile(files[i], tmpString);
    }
    return true;
}
//包括子文件夹下的文件。
QStringList getAllInFloders(bool pSubDir, QString pDir, bool pSize)
{
    if(pDir.isEmpty())
        return QStringList();
    pDir.replace("\\", "/");
    if(!pDir.endsWith('/'))
        pDir.append('/');
    QFileInfo tmpFileInfo(pDir);
    if(!tmpFileInfo.isDir())
    {
        IDE_TRACE();
        return QStringList();
    }
    QDir tmpDir = QDir(pDir);
    if(!tmpDir.exists())
        return QStringList();

    QStringList getAllList;
    //先处理文件夹
    if(pSubDir)
    {
        QStringList tmpFolders = tmpDir.entryList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        for (int i = 0; i < tmpFolders.count(); ++i)
        {
            QString tmpSubPath = pDir + tmpFolders.at(i);
            getAllList.append(getAllInFloders(pSubDir, tmpSubPath, pSize));
        }
    }
    //再处理文件
    QStringList tmpFiles = tmpDir.entryList(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    for(int i = 0;i<tmpFiles.count();i++)
    {
        QString tmpFile = pDir + tmpFiles.at(i);
        if(pSize)
        {
            QFileInfo tmpFileInfo(tmpFile);
            tmpFile.append(":");
            tmpFile.append(QString::number(tmpFileInfo.size()));
        }
        getAllList.append(tmpFile);
    }
    return getAllList;
}

//仅获取子文件夹。
QStringList getAllSubfolderInFloders(QString pDir)
{
    QFileInfo tmpFileInfo(pDir);
    if(!tmpFileInfo.isDir())
    {
        IDE_TRACE();
        return QStringList();
    }
    if(!pDir.endsWith('/'))
        pDir.append('/');
    QDir dir = QDir(pDir);
    QStringList getAllList,tmpList;
    QString tmpString;
    if(!dir.exists())
        return getAllList;
    tmpList = dir.entryList(QDir::Dirs|QDir::NoSymLinks|QDir::NoDotAndDotDot);
    for(int i = 0;i<tmpList.count();i++)
    {
        tmpString = QString("%1%2").arg(pDir).arg(tmpList.at(i));
        tmpString.replace(QString("//"),QString("/"));
        getAllList.append(tmpString);
    }
    return getAllList;
}

QStringList getAllFileInFloders(QString pDir)
{
    QFileInfo tmpFileInfo(pDir);
    if(!tmpFileInfo.isDir())
    {
        IDE_TRACE();
        return QStringList();
    }
    if(!pDir.endsWith('/'))
        pDir.append('/');
    QDir dir = QDir(pDir);
    QStringList getAllList,tmpList;
    QString tmpString;
    if(!dir.exists())
        return getAllList;
    tmpList = dir.entryList(QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot);
    for(int i = 0;i<tmpList.count();i++)
    {
        tmpString = QString("%1%2").arg(pDir).arg(tmpList.at(i));
        tmpString.replace(QString("//"),QString("/"));
        getAllList.append(tmpString);
    }
    return getAllList;
}
//从某一个文件夹中获取所有文件路径，包括子目录下的
QStringList getAllFilesAndSize(QString pDir)
{
    QDir dir = QDir(pDir);
    if(!dir.exists())
        return QStringList();
    if(!pDir.endsWith('/'))
        pDir.append('/');
    IDE_TRACE_STR(pDir);
    QStringList getAllList;
    QFileInfoList tmpList = dir.entryInfoList(QDir::Dirs|QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot);
    for(int i=0;i<tmpList.count();i++)
    {
        QFileInfo tmpFileInfo = tmpList.at(i);
        if(tmpFileInfo.isDir())
        {
            QString tmpPath = tmpFileInfo.filePath();
            getAllList.append(getAllFilesAndSize(tmpPath));
        }
        else if(tmpFileInfo.isFile())
        {
            getAllList.append(tmpFileInfo.filePath() + QString(":%1").arg(tmpFileInfo.size()));
        }
    }
    return getAllList;
}



//pValidRect:为0表示全部拷贝  pResolution:目标图片分辨率  先裁剪原图，在转化为目标分辨率。
//ConvertImage(QString("d:/a.jpg"), QRect(0,0,640,480), QString("d:/b.bmp"), QSize(320,240));
bool convertImage(QString pSrcFileName,QRect pValidRect,QString pDstFileName,QSize pResolution)
{
    if(pSrcFileName.isEmpty() || pSrcFileName.indexOf(QRegExp(".")) < 0)
    {
        IDE_TRACE();
        return false;
    }
    if(pDstFileName.isEmpty() || pDstFileName.indexOf(QRegExp(".")) < 0)
    {
        IDE_TRACE();
        return false;
    }
    bool flag = false;
    QImage tmpImage;
    tmpImage.load(pSrcFileName);
    QRect tmpRect = tmpImage.rect();
    if(pValidRect.left() != tmpRect.left() ||
       pValidRect.right() != tmpRect.right() ||
       pValidRect.top() != tmpRect.top() ||
       pValidRect.bottom() != tmpRect.bottom())
    {
        QImage newImage = tmpImage.copy(pValidRect);
        QImage dstImage = newImage.scaled(pResolution, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        flag = dstImage.save(pDstFileName);
    }
    else
    {
        QImage dstImage = tmpImage.scaled(pResolution, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        flag = dstImage.save(pDstFileName);
    }
    return flag;
}
QString getFileSuffix(QString pFileName)
{
    int index = pFileName.lastIndexOf(".");
    QString fileSuffix;
    if(index >= 0)
        fileSuffix = pFileName.mid(index+1);
    else
        return fileSuffix;
    if(fileSuffix.length() < 2)
        return QString();
    return fileSuffix;
}

//根据参数判断图片缩放大小
bool convertImage(QString pFileName, QSize pSize, PRIOR pPrior)
{
    QFile tmpFile(pFileName);
    if(tmpFile.exists() == false)
    {
        IDE_TRACE();
        return false;
    }
    QImage tmpImage, tmpNewImage;
    if(tmpImage.load(pFileName) == false)
    {
        IDE_TRACE();
        return false;
    }
    if(pPrior == PRIOR_WIDTH)
    {
        tmpNewImage = tmpImage.scaledToWidth(pSize.width(), Qt::SmoothTransformation);
    }
    else
    {
        tmpNewImage = tmpImage.scaledToHeight(pSize.height(), Qt::SmoothTransformation);
    }
    if(tmpFile.remove() == false)
    {
        IDE_TRACE();
        return false;
    }
    if(tmpNewImage.save(pFileName) == false)
    {
        IDE_TRACE();
        return false;
    }
    return true;
}

QImage getScaledImage(QString pFileName, QSize pSize, PRIOR pPrior)
{
    QFile tmpFile(pFileName);
    if(tmpFile.exists() == false)
    {
        return QImage();
    }
    QImage tmpImage, tmpNewImage;
    if(tmpImage.load(pFileName) == false)
    {
        return QImage();
    }
    if(pPrior == PRIOR_WIDTH)
    {
        tmpNewImage = tmpImage.scaledToWidth(pSize.width(), Qt::SmoothTransformation);
    }
    else
    {
        tmpNewImage = tmpImage.scaledToHeight(pSize.height(), Qt::SmoothTransformation);
    }
    return tmpNewImage;
}

QSize getScaledImageSize(QString pFileName, QSize pSize, PRIOR pPrior)
{
    QImage tmpImage= getScaledImage(pFileName, pSize, pPrior);
    if(tmpImage.isNull())
    {
        return QSize();
    }
    return tmpImage.size();
}

//如果pResolution无效，则不缩放
bool convertImage(QString pSrcFileName,QString pDstFileName,QSize pResolution)
{
    QFile tmpSrcFile(pSrcFileName);
    if(tmpSrcFile.exists() == false)
    {
        IDE_TRACE();
        return false;
    }
    QImage tmpImage;
    if(!tmpImage.load(pSrcFileName))
    {
        IDE_TRACE();
        return false;
    }
    if(pDstFileName.isEmpty())
    {
        IDE_TRACE();
        return false;
    }
    QString suffix = getFileSuffix(pDstFileName);
    if(suffix.isEmpty())
    {
        IDE_TRACE();
        return false;
    }
    QString tmpDstString;
    QFile tmpDstFile(pDstFileName);
    if(tmpDstFile.exists())
    {
        QFile::Permissions tmpPermissions = tmpDstFile.permissions(pDstFileName);
        if(!(tmpPermissions & QFile::WriteUser))
            tmpDstFile.setPermissions(tmpPermissions | QFile::WriteUser);
        tmpDstFile.remove();
    }
    else
    {
        tmpDstString = getFileDirectory(pDstFileName);
        QDir tmpDir;
        tmpDir.mkpath(tmpDstString);
    }

    QImage dstImage;
    //检查目标图片分辨率是否合理，如果大小无效或者大于1920x1920，则保持原大小不变。
    if(pResolution.isEmpty() || pResolution.isValid()==false ||
       pResolution.width() > 1920 ||
       pResolution.height() > 1920)
    {
        dstImage = tmpImage;
    }
    else
    {
        dstImage = tmpImage.scaled(pResolution, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    if(!dstImage.save(pDstFileName, suffix.toLatin1().data()))
    {
        IDE_TRACE();
        return false;
    }
    return true;
}

bool convertAudio(QString pSrcFileName, QString pDstFileName)
{
    QString tmpString = getFileSuffix(pSrcFileName);
    if(tmpString.isEmpty())
        return false;
    if(pDstFileName.endsWith(".wav", Qt::CaseInsensitive) == false)
        pDstFileName.append(".wav");
    if(tmpString.compare("wav", Qt::CaseInsensitive) == 0)
    {
        return copyFile(pSrcFileName, pDstFileName);
    }
    else if(tmpString.compare("mp3", Qt::CaseInsensitive) == 0)
    {
        //MULTIMEDIA_CONVERT tmpAudioConvert;
        //return tmpAudioConvert.ConvertAudio(pSrcFileName, pDstFileName, 100000);
    }
    else
    {
        IDE_TRACE();
        return false;
    }
    return true;
}

QString getStrBehindKey(QString pString, QString pKey,quint8 pStart, quint8 pLenth,Qt::CaseSensitivity pCaps)
{
    QString tmpString;
    tmpString.clear();

    if((pLenth==0)||(pString.isEmpty()))
        return tmpString;
    int tmpDex = pString.indexOf(pKey,pCaps);
    if(tmpDex==-1)
        return tmpString;
    if((tmpDex+pKey.length()+pStart,pLenth) > pString.length())
        return tmpString;

    tmpString = pString.mid(tmpDex+pKey.length()+pStart,pLenth);
    return tmpString;
}

QString getStrBetweenPreAndAfter(QString pString,QString pPre, QString pAfter,
     int pPreindent, int pAfterindent,Qt::CaseSensitivity pCaps)
{
    QString tmpString;
    tmpString.clear();

    int tmpPreDex = pString.indexOf(pPre,pCaps);
    if(tmpPreDex==-1)
        return tmpString;
    int tmpAfterDex = pString.indexOf(pAfter,pCaps);
    if(tmpAfterDex==-1)
        return tmpString;

    tmpString = pString.mid(tmpPreDex+pPre.length()+pPreindent,
                            tmpAfterDex-tmpPreDex-pPre.length()-pAfterindent);
    return tmpString;
}


