#ifndef GLOBALFUN_H
#define GLOBALFUN_H

#include "define.h"

#include <QRect>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QStringList>
#include <QDomDocument>

#include <QImage>
#include <QTime>



#define D_DEFAULT_PREFIX      QString("rc")
#define D_PREFIX              QString("prefix")
#define D_RCINVALID      QString("invalid")
#define D_EFFECTTYPE     QString("effecttype")
#define D_EFFECTPARA     QString("parameter")
#define D_TIPTEXT       QString("text")

typedef enum{
    DM_NORMAL = 0,
    DM_STANDBY,
    DM_FULLSCREEN,  //屏显示文本信息
    DM_UDISK,
    DM_NET,
    DM_SLEEP,
    DM_DIAL,
    DM_INIT  //>@主题初始化
} DEV_MODE;

class NUM_INFO
{
public:
    NUM_INFO()
    {
        mNum = 0;
        mMinNum = 0;
        mMaxNum = 0;
    }
    bool setnum(int pNum)
    {
        if(pNum < mMinNum)
        {
            mNum = mMinNum;
            return false;
        }
        else if(pNum > mMaxNum)
        {
            mNum = mMaxNum;
            return false;
        }
        else
        {
            mNum = pNum;
            return true;
        }
        return false;
    }
    void setnum(int pNum, int pMinNum, int pMaxNum)
    {
        mMinNum = pMinNum;
        mMaxNum = pMaxNum;
        if(pNum < mMinNum)
            mNum = mMinNum;
        else if(pNum > mMaxNum)
            mNum = mMaxNum;
        else
            mNum = pNum;
    }
public:
    int mNum;
    int mMinNum;
    int mMaxNum;
};

typedef enum{
    ROTATE_INVALID=-1,
    ROTATE_0=0,
    ROTATE_90=90,
    ROTATE_180=180,
    ROTATE_270=270,
} ROTATE_TYPE;

typedef enum{
    COM_INVALID=0,
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

typedef enum{
    P_INVALID = 0,  ///0
    P_WD,
    P_CPU,
    P_MEM,
    P_LED,
    P_DISK,
    P_MDISK,
    P_LCD,
    P_Touch,
    P_RTC,
    P_Player,
    P_LANC,
    P_LANR,
    P_LANH,
    P_VP,
    P_COM,
    P_SAF,
    P_DIAL,
    ///
    P_UDP,
    P_TCP,
    P_TEST
} PH_Type;



typedef enum{
    L_INVALID = 0,
    L_ContentManager,
    L_ThemeManager
} LG_Type;


typedef enum{
    Vertical=0,
    Horizontal=1,
    HVBoth=2,
    HVNone
} DIRECTION;///横向 纵向

typedef enum{
    PATH_TYPENONE = 0,
    PATH_TYPERC,
    PATH_TYPENETADDR,
    PATH_TYPEABS,
    PATH_TYPEQRC
} FILEPATH_TYPE;

//图片转换优先级
typedef enum{
    PRIOR_WIDTH = 0,
    PRIOR_HEIGHT
} PRIOR;




///文件类型
typedef enum{
    TYPE_INVALID=0,
    TYPE_START,
    TYPE_PRO,
    TYPE_MANUFACTURE,
    TYPE_UPGRADE,
    TYPE_NETWORKMANAGER,
    TYPE_VIDEOEDITOR,
    TYPE_IMAGEEDITOR,
    TYPE_AUDIOEDITOR,
    TYPE_FONTEDITOR,
    TYPE_ASCIICODEC,
    TYPE_EDITOR,
    TYPE_INI,
    TYPE_LOG,
    TYPE_FACEDETECT,
    TYPE_RC,
    TYPE_FTP,
    TYPE_SMS,
    TYPE_SQLSERVER,
    TYPE_SQLDESIGN,
    TYPE_SQLMANUFACTURE,
    Type_MONITOR,
    TYPE_CHAT,
    TYPE_DEVINFO,
    TYPE_MAPS,
    TYPE_SEV,
    TYPE_MDP,
    TYPE_UI,
    TYPE_MAP,
    TYPE_STC,
    TYPE_STO
}FILE_TYPE;

typedef enum{
    RC_INVALID=0,
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

class RC_MAP
{
public:
    RC_MAP(){}
    bool operator==(const RC_MAP &s)
    {
        if(mSrc.compare(s.mSrc))
            return false;
        if(mMap.compare(s.mMap))
            return false;
        return true;
    }
    bool operator!=(const RC_MAP &s)  { return !operator==(s); }
    RC_MAP & operator=(const RC_MAP &s)
    {
        mSrc = s.mSrc;
        mMap = s.mMap;
        return *this;
    }
public:
    QString mSrc;
    QString mMap;
};


typedef enum{
    OPERATE_NONE = 0,
    ///
    OPERATE_FORM,
    OPERATE_RC,
    OPERATE_MAP,
    ///
    OPERATE_INIT,
    OPERATE_RELEASE,
    ///
    OPERATE_PROGRESS,
    ///动作参数
    OPERATE_CHANGE,
    OPERATE_SLIP,   ///滑动
    OPERATE_TURN, ///转向
    OPERATE_ADD,
    OPERATE_MODIFY,
    OPERATE_DEL,
    ///
    OPERATE_CLEAR,
    OPERATE_SHOW,
    OPERATE_HIDE,
    ///播放参数
    OPERATE_RUN,
    OPERATE_HALT,
    OPERATE_PLAY,
    OPERATE_PAUSE,
    OPERATE_STOP,
    OPERATE_STEADY,
    OPERATE_VOCAL,  ///有声
    OPERATE_MUTE,   ///静音
    OPERATE_RESUME,
    OPERATE_REPEAT,
    ///用于测试
    OPERATE_KEYPRESS,
    OPERATE_KEYLIGHT,
    OPERATE_KEYCANCEL,
    OPERATE_KEYMOVE,
    OPERATE_KEYRELEASE
} AREA_OPERATE;

typedef enum{
    UI_UNKNOW=0,
    UI_STRING,
    UI_CHECK,
    UI_ENUM,
    UI_NUM,
    UI_BOOL
}UI_TYPE;

UI_TYPE getUiType(QString pName);
QString getUiString(UI_TYPE pType);


QString getOperateString(AREA_OPERATE pType);
AREA_OPERATE getOperateType(QString pType);


int getKey(QString pString, QString pPrefix = D_DEFAULT_PREFIX);

ROTATE_TYPE getRotateType(int);
int getRotateAngle(ROTATE_TYPE toType);


bool creatPath(QString pPath);
bool copyFolder(QString pSrcDir, QString pDstDir, bool pSubfolder=true, bool pCopyFile=true, bool pIdel=false);
bool moveFolder(QString pSrc, QString pDst);
bool delFolder(QString pDir, bool pIdel=false);
bool clearFolder(QString pDir);
bool cutFile(QString srcFile, QString dstFile);
bool copyFile(QString srcFile, QString dstFile, QString pPrefix=QString());
bool delFile(QString pPath);
bool creatFile(QString pPath);


QStringList getAllInFloders(bool pSubDir, QString pDir, bool pSize);
QStringList getAllFileInFloders(QString pDir);
QStringList getAllSubfolderInFloders(QString pDir);
QStringList getAllFilesAndSize(QString pDir);

bool isFileExist(QString FileName);
QStringList ergodicDirectory(QString d);
QString getFileName(QString pFilePath);
QString getFileDirectory(QString pFilePath);
QString getParentDirectory(QString pDirPath);
QString getDirectoryName(QString pDirPath);
QRect getRectFromString(QString pRectString);
QStringList findFilesInFloder(QString DirName, QString fileName, bool subfolder);
QStringList searchFiles(QString pDirPath, QString pSuffix, bool pSubfolder);
QStringList findFiles(const QDir &dir,const QString &fileName,const QString &txt,Qt::CaseSensitivity sensitive);

bool convertImage(QString pSrcFileName,QRect pValidRect,QString pDstFileName,QSize pResolution);
bool convertImage(QString pFileName, QSize pSize, PRIOR pPrior);
bool convertImage(QString pSrcFileName,QString pDstFileName,QSize pResolution);
bool convertAudio(QString pSrcFileName, QString pDstFileName);
QString getFileSuffix(QString pFileName);
QImage getScaledImage(QString pFileName, QSize pSize, PRIOR pPrior);
QSize getScaledImageSize(QString pFileName, QSize pSize, PRIOR pPrior);
QString getStrBehindKey(QString pString, QString pKey,quint8 pStart, quint8 pLenth,Qt::CaseSensitivity pCaps);
QString getStrBetweenPreAndAfter(QString pString,QString pPre, QString pAfter,int pPreindent, int pAfterindent,Qt::CaseSensitivity pCaps);


DIRECTION getDirection(QString pDirection);
QString getDirectionString(DIRECTION pDirection);
COM_TYPE getComponetType(QString ComponentName);
QString getComponetName(COM_TYPE ComponentType);
QString getComponetPath(COM_TYPE pType);

FILEPATH_TYPE getRcFileType(QString pFileName);
RC_TYPE getRcType(QString FileName);
COM_TYPE getParentComType(COM_TYPE pType);
COM_TYPE getParentComType(QString ComPath);
COM_TYPE getCurComType(QString ComPath);
QString getCurComName(QString ComPath);



PH_Type getPHType(QString pPara);
PH_Type getElementPHType(QDomElement pDevElement);
LG_Type getLGType(QString pPara);

typedef enum {
    DEV_NONE=0,
    DEV_MIDDLEWARE,
    DEV_PHYSICAL,
    DEV_LOGIC
} DEV_TYPE;

DEV_TYPE getDevType(QString pDevName);

inline void QtSleep(unsigned int msec, QEventLoop::ProcessEventsFlags flags = QEventLoop::AllEvents)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents(flags, 1);
    }
}



#endif // GLOBALFUN_H


