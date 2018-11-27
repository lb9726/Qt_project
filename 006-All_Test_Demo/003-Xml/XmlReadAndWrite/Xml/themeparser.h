#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QImage>
#include "define.h"
#include "xmlparse.h"

typedef enum {
    Vertical = 0,
    Horizontal = 1,
    HVBoth = 2,
    HVNone
} DIRECTION;///横向 纵向

inline DIRECTION getDirection(QString pDirection)
{
    if(pDirection.isEmpty())
    {
        return HVNone;
    }
    if(pDirection.compare("V") == 0)
    {
        return Vertical;
    }
    else if(pDirection.compare("H") == 0)
    {
        return Horizontal;
    }
    else if(pDirection.compare("HV") == 0)
    {
        return HVBoth;
    }
    return HVNone;
}

inline QString getDirectionString(DIRECTION pDirection)
{
    QString tmpDirString;
    if(pDirection == Vertical)
    {
        tmpDirString = "v";
    }
    else if(pDirection == Horizontal)
    {
        tmpDirString = "h";
    }
    else if(pDirection == HVBoth)
    {
        tmpDirString = "hv";
    }
    return tmpDirString;
}

typedef enum {
    PATH_TYPENONE = 0,
    PATH_TYPERC,
    PATH_TYPENETADDR,
    PATH_TYPEABS,
    PATH_TYPEQRC
} FILEPATH_TYPE;

typedef enum {
    ROTATE_INVALID = -1,
    ROTATE_0 = 0,
    ROTATE_90 = 90,
    ROTATE_180 = 180,
    ROTATE_270 = 270
} ROTATE_TYPE;

typedef enum{
    COM_INVALID = 0,
    COM_DESKTOP,
    COM_STARTDEF,
    COM_FLOOR,
    COM_REALTIMETEXT,
    COM_ARROW,
    COM_FUNCTION,
    COM_MEDIA,
    COM_FULLTEXT,
    COM_MESSAGEBAR,
    COM_PROGRESSBAR,
    COM_LOGO,
    COM_USBLOGO,
    COM_WIFILOGO,
    COM_ENDDEF
} COM_TYPE;

inline COM_TYPE getComponetType(QString ComponentName)
{
    COM_TYPE tmpComType = COM_INVALID;
    if (ComponentName.isNull())
    {
        return tmpComType;
    }
    else if (ComponentName.isEmpty()
        || ComponentName.compare(QString("desktop"), Qt::CaseInsensitive) == 0
        || ComponentName.compare(QString("/"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = COM_DESKTOP;

    }
    else if (ComponentName.compare(QString("messagebar"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = COM_MESSAGEBAR;
    }
    else if (ComponentName.compare(QString("progressbar"), Qt::CaseInsensitive) == 0)
    {
        tmpComType = COM_PROGRESSBAR;
    }
    else if(!ComponentName.compare(QString("floor"), Qt::CaseInsensitive))
    {
        tmpComType = COM_FLOOR;
    }
    else if (!ComponentName.compare(QString("arrow"), Qt::CaseInsensitive))
    {
        tmpComType = COM_ARROW;
    }
    else if (!ComponentName.compare(QString("function"), Qt::CaseInsensitive))
    {
        tmpComType = COM_FUNCTION;
    }
    else if (!ComponentName.compare(QString("media"), Qt::CaseInsensitive))
    {
        tmpComType = COM_MEDIA;
    }
    else if (!ComponentName.compare(QString("fulltext"), Qt::CaseInsensitive))
    {
        tmpComType = COM_FULLTEXT;
    }
    else if (!ComponentName.compare(QString("realtimetext"), Qt::CaseInsensitive))
    {
        tmpComType = COM_REALTIMETEXT;
    }
    else if (!ComponentName.compare(QString("logo"), Qt::CaseInsensitive))
    {
        tmpComType = COM_LOGO;
    }
    else
    {

    }
    return tmpComType;
}

inline QString getComponetName(COM_TYPE ComponentType)
{
    switch(ComponentType)
    {
    case COM_DESKTOP:
    {
        return QString("desktop");
    }
    case COM_MESSAGEBAR:
    {
        return QString("messagebar");
    }
    case COM_PROGRESSBAR:
    {
        return QString("progressbar");
    }
    case COM_FLOOR:
    {
        return QString("floor");
    }
    case COM_ARROW:
    {
        return QString("arrow");
    }
    case COM_FUNCTION:
    {
        return QString("function");
    }
    case COM_MEDIA:
    {
        return QString("media");
    }
    case COM_FULLTEXT:
    {
        return QString("fulltext");
    }
    case COM_REALTIMETEXT:
    {
        return QString("realtimetext");
    }
    case COM_LOGO:
    {
        return QString("logo");
    }
    default:
    {
        return QString();
    }
    }
    return QString();
}

inline QString getComponetPath(COM_TYPE pType)
{
    if(pType == COM_DESKTOP)
    {
        return QString("/");
    }
    QString tmpName = getComponetName(pType);
    if(!tmpName.isEmpty())
    {
        return tmpName.prepend('/');
    }
    return tmpName;
}

inline COM_TYPE getParentComType(COM_TYPE pType)
{
    if(pType > COM_STARTDEF && pType < COM_ENDDEF)
    {
        return COM_DESKTOP;
    }
    return COM_INVALID;
}

inline COM_TYPE getParentComType(QString ComPath)
{
    if(ComPath.isEmpty())
    {
        return COM_INVALID;
    }
    int index1, index2;
    QString tmpString;
    index1 = ComPath.indexOf('/', Qt::CaseInsensitive);
    if(index1 < 0)
    {
        return COM_INVALID;
    }
    index2 = ComPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2 < 0) ///desktop and rect
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
        tmpString = ComPath.mid(index1 + 1, index2 - index1 - 1);
    }
    return getComponetType(tmpString);
}

inline QString getCurComName(QString ComPath)
{
    if(ComPath.isEmpty())
    {
        return QString();
    }
    int index1, index2;
    index1 = ComPath.indexOf('/', Qt::CaseInsensitive);
    if(index1 < 0)
    {
        if(ComPath.compare(QString("desktop"), Qt::CaseInsensitive) == 0)
        {
            return QString("desktop");
        }
        else
        {
            return QString();
        }
    }
    QString tmpString;
    index2 = ComPath.indexOf('/', index1+1, Qt::CaseInsensitive);
    if(index2 < 0)
    {
        tmpString = ComPath.mid(index1+1);
        if(tmpString.isEmpty())
        {
            return QString("desktop");     ///Empty表示Desktop
        }
        else
        {
            return tmpString;
        }
    }
    else
    {
        tmpString = ComPath.mid(index2 + 1);
    }
    return tmpString;
}

inline COM_TYPE getCurComType(QString ComPath)
{
    QString tmpString;
    int index1, index2;
    index1 = ComPath.indexOf('/', Qt::CaseInsensitive);
    if(index1 < 0)
    {
        return getComponetType(ComPath);
    }
    index2 = ComPath.indexOf('/', index1 + 1, Qt::CaseInsensitive);
    if(index2 < 0)
    {
        tmpString = ComPath.mid(index1 + 1);
        if(tmpString.isEmpty())
        {
            return COM_DESKTOP;     ///Empty表示Desktop
        }
    }
    else
    {
        tmpString = ComPath.mid(index2 + 1);
    }
    return getComponetType(tmpString);
}

inline QRect getRectFromString(QString pRectString)
{
    if(pRectString.isEmpty())
    {
        return QRect();
    }
    QStringList tmpList = pRectString.split(",");
    if(tmpList.count() != 4)
    {
        return QRect();
    }
    int tmpX = tmpList.at(0).toInt();
    int tmpY = tmpList.at(1).toInt();
    int tmpW = tmpList.at(2).toInt();
    int tmpH = tmpList.at(3).toInt();
    if(tmpW <= 0 || tmpH <= 0)
    {
        return QRect();
    }
    return QRect(tmpX, tmpY, tmpW, tmpH);
}

typedef enum {
    RC_INVALID = 0,
    RC_BMP,
    RC_JPG,
    RC_PNG,
    RC_GIF,
    RC_SVG,
    RC_QML,
    RC_FONT,
    RC_MP3,
    RC_WAV,
    RC_MP4,
    RC_AVI,
    RC_RMVB,
    RC_FLV,
    RC_3DS,
    RC_HTML,
    RC_XML
}RC_TYPE;

inline RC_TYPE getRcType(QString FileName)
{
    if (FileName.isEmpty())
    {
        return RC_INVALID;
    }
    if(FileName.endsWith(".bmp", Qt::CaseInsensitive))
    {
        return RC_BMP;
    }
    else if(FileName.endsWith(".jpg", Qt::CaseInsensitive))
    {
        return RC_JPG;
    }
    else if(FileName.endsWith(".png", Qt::CaseInsensitive))
    {
        return RC_PNG;
    }
    else if(FileName.endsWith(".gif", Qt::CaseInsensitive))
    {
        return RC_GIF;
    }
    else if(FileName.endsWith(".svg", Qt::CaseInsensitive))
    {
        return RC_SVG;
    }
    else if(FileName.endsWith(".qml", Qt::CaseInsensitive))
    {
        return RC_QML;
    }
    else if(FileName.endsWith(".avi", Qt::CaseInsensitive))
    {
        return RC_AVI;
    }
    else if(FileName.endsWith(".wav", Qt::CaseInsensitive))
    {
        return RC_WAV;
    }
    else if(FileName.endsWith(".mp3", Qt::CaseInsensitive))
    {
        return RC_MP3;
    }
    else if(FileName.endsWith(".mp4", Qt::CaseInsensitive))
    {
        return RC_MP4;
    }
    else if(FileName.endsWith(".flv", Qt::CaseInsensitive))
    {
        return RC_FLV;
    }
    else if(FileName.endsWith(".rmvb", Qt::CaseInsensitive))
    {
        return RC_RMVB;
    }
    else if(FileName.endsWith(".font", Qt::CaseInsensitive))
    {
        return RC_FONT;
    }
    else if(FileName.endsWith(".3ds", Qt::CaseInsensitive))
    {
        return RC_3DS;
    }
    else if(FileName.endsWith(".html", Qt::CaseInsensitive))
    {
        return RC_HTML;
    }
    else if(FileName.endsWith(".xml", Qt::CaseInsensitive))
    {
        return RC_XML;
    }
    return RC_INVALID;
}

inline FILEPATH_TYPE getRcFileType(QString pFileName)
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
    if(pFileName.startsWith("http://")
         || pFileName.startsWith("https://")
         ||   pFileName.startsWith("rtmp://")
         ||  pFileName.startsWith("rtsp://")
         ||  pFileName.startsWith("mms://"))
    {
        return PATH_TYPENETADDR;
    }
    ///3、查看是否为绝对/相对路径下的文件
    if(QFile::exists(pFileName))
    {
        return PATH_TYPEABS;
    }
    return PATH_TYPENONE;
}

//如果pResolution无效，则不缩放
inline bool convertImage(QString pSrcFileName, QString pDstFileName, QSize pResolution)
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
    QString suffix = QFileInfo(pDstFileName).suffix();
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
        {
            tmpDstFile.setPermissions(tmpPermissions | QFile::WriteUser);
        }
        tmpDstFile.remove();
    }
    else
    {
        tmpDstString = QFileInfo(pDstFileName).path();
        QDir tmpDir;
        tmpDir.mkpath(tmpDstString);
    }

    QImage dstImage;
    //检查目标图片分辨率是否合理，如果大小无效或者大于1920x1920，则保持原大小不变。
    if(pResolution.isEmpty() || pResolution.isValid() == false
            || pResolution.width() > 1920
            || pResolution.height() > 1920)
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

inline bool convertAudio(QString pSrcFileName, QString pDstFileName)
{
    QString tmpString = QFileInfo(pSrcFileName).suffix();
    if(tmpString.isEmpty())
    {
        return false;
    }
    if(pDstFileName.endsWith(".wav", Qt::CaseInsensitive) == false)
    {
        pDstFileName.append(".wav");
    }
    if(tmpString.compare("wav", Qt::CaseInsensitive) == 0)
    {
        return QFile::copy(pSrcFileName, pDstFileName);
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

#define D_DEFAULT_PREFIX      QString("rc")
#define D_PREFIX              QString("prefix")
#define D_RCINVALID           QString("invalid")
#define D_EFFECTTYPE          QString("effecttype")
#define D_EFFECTPARA          QString("parameter")
#define D_TIPTEXT             QString("text")

class ThemeParser : public XmlParse
{
    Q_OBJECT
public:
    ThemeParser(QString fileName);

    static const QString UiSuffix;
    static const QString UiFileType;

    ///操作ui文件
    bool createUi();
    bool openUi();
    bool closeUi();
    bool delUi();
    bool saveUi();
    bool renameUi(QString FileName);

    bool isValid();

    QString getSolution();
    bool setSolution(QString pSolution);
    DIRECTION getDirectionPermisson();
    bool setDirectionPermisson(DIRECTION pDirection);
    bool canSwitch() { return (m_DirectionPermisson == HVBoth); }

    ///操作文件中的元素
    void getComponentGroup();

    ///查找文件的全路径
    FILEPATH_TYPE getRcFileType(QString pFileName);
    FILEPATH_TYPE regulatRcFile(QString& pFileName);
    ///查看是否为相对路径
    bool isRelativePath(QString pFileName);
    ///规整资源到目标路径
    bool regularFile(COM_TYPE pComType, QString pRcName, QString &pRcFile, QString pDstPath, QSize pSize);

    bool getComRect(int pLayout, QDomElement pComElement, QRect &pHRect, QRect &pVRect);
    QRect getComRect(int pLayout, QDomElement pComElement, DIRECTION pDirection);
    bool modifyComRect(int pLayout, QDomElement pComElement, QRect pRect, DIRECTION pDirection, bool pForceNew = false);

    QDomElement getComRcElement(QDomElement pComElement);
    QDomNodeList getComRcList(QDomElement pComElement);
    QDomNodeList getComStateList(QDomElement pComElement);
    QDomElement getComState(QDomElement pComElement, QString pStateName, bool pCreateFlag = false); ///pCreateFlag如果不存在则自动创建
    bool modifyEffectType(QDomElement pStateElement, QString pEffectType);
    QDomElement modifyComState(QDomElement pComElement, QString pStateName, QString pEffectType, QString pEffectPara);

    bool modifyEffectPara(QDomElement pStateElement, QString pEffectPara);

    ///获取某个组件的父QDomElement，同时查看是否为规整的资源路径，否则进行创建此路径
    bool getParentElement(QDomElement &pParentElement, QString pComPath);
    ///获取组件的父组件
    QDomElement getParComDom(QDomElement pComElement);
    QDomElement getParComDom(QString pComPath);

    //    bool addMap(QDomElement pMapStateElement, RC_MAP pMap, QString pPrefix=D_DEFAULT_PREFIX);
    //    bool delMap(QDomElement pMapStateElement, RC_MAP pMap, QString pPrefix=D_DEFAULT_PREFIX);
    //    QList<RC_MAP> getMapList(QDomElement pComElement, QString &pMapType, QString pPrefix=D_DEFAULT_PREFIX);
    //    QHash<QString, QString> getMapHash(QDomElement pComElement, QString &pMapType, QString pPrefix=D_DEFAULT_PREFIX);

    ///添加一个控件的Dom资源
    QDomElement addComDom(COM_TYPE pComType, QRect pRect, DIRECTION pDirection);
    ///在Dom中删除一个控件的Dom资源
    bool deleteComDom(COM_TYPE pComType);

    ///将资源全部保存为相对路径，并存储在UI对应的文件夹下
    bool saveRelativeComDom(QDomElement pComElement);

public:
    ///xx.ui文件中的组件（subui和control）的Dom元素(xx.ui)
    QHash<COM_TYPE, QDomElement>  m_ComElementGroup;
    DIRECTION                     m_DirectionPermisson;
};

#endif // THEMEMANAGER_H
