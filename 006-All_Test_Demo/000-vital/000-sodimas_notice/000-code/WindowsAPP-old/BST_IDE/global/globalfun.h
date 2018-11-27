#ifndef GLOBALFUN_H
#define GLOBALFUN_H

#include "define.h"

#ifdef WINDOWS
#include <conio.h>
#include <windows.h>
#include <WinBase.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <winver.h>
#include "tlhelp32.h"
#include <winbase.h>
#include <setupapi.h>
#include <commctrl.h>
#include <tchar.h>
#include <process.h>
#include <basetyps.h>
#include <initguid.h>
#include <devguid.h>

#include <WinNetWk.h>

//>@注册表常用宏定义
#include <RegStr.h>
//>@网络流量定义
#ifdef SURRPORT_NETMANAGER
#include  <iphlpapi.h>
#include  <ipexport.h>
#include  <iprtrmib.h>
#include  <iptypes.h>
#endif
#endif

#if (defined(UBUNTU) || defined(LINUX))
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/soundcard.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#endif

#ifdef QT5
#include <QtWidgets>
#endif

#include <QtGui>
#include <QtGui/QPixmap>
#ifdef QT5
#include <QSplashScreen>
#include <QApplication>
#else
#include <QtGui/QSplashScreen>
#include <QtGui/QApplication>
#endif

#include <QProcess>
#include <QTime>
#include <QHash>
#include <QPainter>
#include <QSharedPointer>
#include <QStyle>
#include <QTimeLine>
#include <QTimer>
#include <QImage>
#include <QFontMetrics>
#include <QPointF>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QToolBox>
#include <QDateTime>
#include <QMainWindow>
#include <QFontDialog>
#include <QTreeWidgetItem>
#include <QThread>
#include <QEvent>
#include <QReadWriteLock>
#include <QMessageBox>
#include <QGraphicsWidget>
#include <QGraphicsView>
#include <QGraphicsGridLayout>
#include <QGraphicsWidget>
#include <QPixmap>
#include <QSpacerItem>
#include <QIODevice>
#include <QListWidget>
#include <QProcess>
#include <QApplication>
#include <QSemaphore>
#include <QMutex>
#include <QMultiHash>
#include <QQueue>
#include <QVariant>

#ifdef SURRPORT_XML
#include <QDomDocument>
#endif

//>@Net
#ifdef SURRPORT_NETWORK
#include <QtNetwork>
#include <QNetworkInterface>
#include <QTcpServer>
#include <QTcpSocket>
#endif

#ifdef SURRPORT_WEBKIT
#ifdef QT5
#include <QtWebKitWidgets/qgraphicswebview>
#else
#include <QGraphicsWebView>
#endif
#endif

#ifdef  SURRPORT_SVG
#include <QSvgRenderer>
#endif

#ifdef SURRPORT_QML
#include <QtDeclarative/QDeclarativeComponent>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeItem>

#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>
#endif

#ifdef LINUX
#include <unistd.h>
#include <sys/reboot.h>
#endif

#if(QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
#include <QGraphicsBlurEffect>
#endif

#define D_LISTITEM_NAME  10000
#define D_LISTITEM_PATH  10001

//>@---------------测试函数-------------------
//>@TestBench
#define FUNCTIONFLAG            "function"
#define BRACKETFLAG             "("
#define RESULTSUFFIX            "_result"

bool FindSoftware(QString pName);
bool SetSoftwareBootrun(QString pPath, bool pEnable);
QString GetSoftwarePath(QString pName);

QString GetHostPCInfo();

inline QString GetFuncName(QByteArray pFuncString)
{
    if(pFuncString.isEmpty())
        return QString();
    if(!pFuncString.startsWith(FUNCTIONFLAG))
        return QString();
    int index = pFuncString.indexOf(BRACKETFLAG);
    if(index == -1)
        return QString();
    int flagsize = strlen(FUNCTIONFLAG)+1;
    QByteArray tmpFunName = pFuncString.mid(flagsize, index-flagsize);
    return QString::fromLatin1(tmpFunName.data());
}

inline QString GetFuncName(QString pFuncString)
{
    return GetFuncName(pFuncString.toLatin1());
}

//>@Other
#define StartPosX 20
#define StartPosY 20

//>@---------------基础函数-------------------

//>@重启
inline bool Reboot(bool pSync = true)
{
#if (defined(UBUNTU) || defined(LINUX))
    if(pSync)
        sync(); // 同步磁盘数据,将缓存数据回写到硬盘,以防数据丢失
    return reboot(RB_AUTOBOOT);
#else
    qApp->quit();
    QProcess::startDetached(qApp->applicationFilePath(), qApp->arguments());
    return true;
#endif
    return false;
}

inline void Restart()
{
    Reboot();
}

inline QString GetSysVersion()
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

inline QString GetAppVersion()
{
    QString tmpVersion;
    QFile tmpFile(":/Version/rc/Version/version.txt");
    if(tmpFile.open(QIODevice::ReadOnly))
    {
        tmpVersion = tmpFile.readLine(1024);
    }
    tmpFile.close();
    return tmpVersion;
}

inline QString GetAppVersion(QString pFile)
{
    int tmpVersionPos[2] = {-1, -1};
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

quint64 GetDiskFreeSpace(QString driver);

inline bool SetQwsSize(QString pFile, QSize pSize)
{
    if(pSize.isEmpty())
        return false;
    QString tmpOldFilePath = pFile;
    QString tmpNewFilePath = pFile + QString("_bk");
    QFile tmpFile(tmpOldFilePath);
    if(!tmpFile.open(QIODevice::ReadOnly))
        return false;
    QFile tmpNewFile(tmpNewFilePath);
    if(!tmpNewFile.open(QIODevice::WriteOnly))
        return false;
    while(!tmpFile.atEnd())
    {
        QByteArray tmpArray = tmpFile.readLine();
        if(tmpArray.isEmpty())
            continue;
        if(tmpArray.startsWith("export QWS_SIZE="))
        {
            tmpArray = QByteArray("export QWS_SIZE=")+QString("%1x%2").arg(pSize.width()).arg(pSize.height()).toLatin1()+QByteArray("\n");
        }
        tmpNewFile.write(tmpArray);
    }
    tmpNewFile.close();
    tmpFile.close();
    tmpFile.remove();
    tmpNewFile.rename(tmpOldFilePath);
    return true;
}

inline bool StrToInt(int& pNum, int pBase, QString pKey)
{
    bool flag = false;
    pNum = pKey.toInt(&flag, pBase);
    if(flag)
        return true;
    return false;
}

inline quint32 CharToU32(char pNum)
{
    return (quint32)(quint8)(pNum);
}

inline bool StrToLongLong(QString pStr, qint64 &pLongLong)
{
    bool ok = false;
    pLongLong = pStr.toLongLong(&ok,10);
    if(!ok)
        return false;
    return true;
}

inline bool PrintSence(QString pPath)
{
    if(pPath.isEmpty())
        return false;
    QPixmap Screen = QPixmap::grabWindow
            (QApplication::desktop()->winId(),
             0,
             0,
             QApplication::desktop()->size().width(),
             QApplication::desktop()->size().height());
    if(Screen.save(pPath,"PNG"))
        return true;
    return false;
}

inline QByteArray PrintSence()
{
    QPixmap Screen = QPixmap::grabWindow
            (QApplication::desktop()->winId(),
             0,
             0,
             QApplication::desktop()->size().width(),
             QApplication::desktop()->size().height());

    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    Screen.save(&buffer, "PNG");
    return byteArray;
}

inline bool isVisibleAscii(quint8 pValue)
{
    if(pValue > 32 && pValue < 127)
        return true;
    return false;
}

#define BUFSIZE 255
#define MAX_DRIVER_LEN 40

typedef struct{
    QString mPath;
    QString mLabel;
    QIcon   mIcon;
}DRIVER_INFO;

QString GetDiskLabel(QString pDiskPath);
QMultiHash<int, DRIVER_INFO> GetDiskList();

inline QString GetPcName()
{
    QStringList envVariables;
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
                 << "HOSTNAME.*" << "DOMAINNAME.*";
    QStringList environment = QProcess::systemEnvironment();
    for(int i=0;i<envVariables.count();i++)
    {
        QString string = envVariables.at(i);
        int index = environment.indexOf(QRegExp(string));
        if (index != -1)
        {
            QStringList stringList = environment.at(index).split('=');
            if (stringList.size() == 2)
                return stringList.at(1);
        }
    }
    return QString();
}

inline QString GetMachineName()
{
    QStringList envVariables;
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
                 << "HOSTNAME.*" << "DOMAINNAME.*";
    QStringList environment = QProcess::systemEnvironment();
    for(int i=0;i<envVariables.count();i++)
    {
        QString string = envVariables.at(i);
        int index = environment.indexOf(QRegExp(string));
        if (index != -1)
        {
            QStringList stringList = environment.at(index).split('=');
            if (stringList.size() == 2)
                return stringList.at(1);
        }
    }
    return QString();
}

//>@LoginNetDisk(QString("\\\\10.200.18.87\\FTP"), QString("m:"), QString("jinwenjie"), QString("bst123456,,,"));
bool LoginNetDisk(QString pNetAddr, QString pLoaclDisk, QString pName, QString pPasswd);

//>@AP模式下的DHCP服务器
bool SetDHCP(QString pFile, QString pIP);

//>@Client下的配置
bool SetNetwork(QString pNetName, bool pDHCP, QHostAddress pIP = QHostAddress(), QHostAddress pNetMask = QHostAddress(), QHostAddress pGateWay = QHostAddress(), QHostAddress pDNS = QHostAddress());

typedef enum{
    DM_NORMAL = 0,
    DM_STANDBY,
    DM_BIGSRN,  //>@大屏显示文本信息
    DM_MDISK,
    DM_NET,
    DM_SLEEP,
    DM_DIAL,
    DM_SETMENU,
    DM_INIT  //>@主题初始化
} E_DEVMODE;

Q_DECLARE_METATYPE(E_DEVMODE)
inline uint qHash(E_DEVMODE pType) { return (uint)pType; }

typedef struct{
    QString         mCpumodel;
    QString         mCpuLoad;
    QString         mMemModel;
    QString         mMemLoad;

    QString         mFsrcpath;
    QString         mTFlashpath;
    QString         mCurUiname;
    QString         mTFlashspace;

    QString         mUdisk0path;
    QString         mUdisk0space;
}DevInfo;

typedef enum{
    Failed = 0,
    Success,
    Start,
    Timeout,
    Stopped,
    Unstart
}TaskState;

typedef enum{
    PATH_TYPENONE = 0,
    PATH_TYPERC,
    PATH_TYPENETADDR,
    PATH_TYPEABS,
    PATH_TYPEQRC
} FILEPATH_TYPE;

typedef enum{
    e_TabNet=0,
    e_TabUsart=1,
    e_TabUsb=2
}CONNECT_TAB_TYPE;

typedef enum{
    Vertical=0,
    Horizontal=1,
    HVBoth=2,
    HVNone
} DIRECTION;        //>@横向 纵向

inline DIRECTION GetDirection(QString pDirection)
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

inline QString GetDirectionString(DIRECTION pDirection)
{
    QString tmpDirString;
    if(pDirection == Vertical)
        tmpDirString = "V";
    else if(pDirection == Horizontal)
        tmpDirString = "H";
    else if(pDirection == HVBoth)
        tmpDirString = "HV";
    return tmpDirString;
}

#ifdef IOS
#include "ocniclass.h"
#endif

inline void QtSleep(unsigned int msec, QEventLoop::ProcessEventsFlags flags = QEventLoop::AllEvents)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents(flags, 1);
#ifdef WINDOWS
        Sleep(1);
#elif UBUNTU
        usleep(1000);//sleep和usleep都已经obsolete，建议使用nanosleep代替
#elif LINUX
        usleep(1000);
#elif IOS
        ocdelay(1);
#else
    #if 1
        QMutex mutex;
        QWaitCondition sleep;
        mutex.lock();
        sleep.wait(&mutex, 10);
        mutex.unlock();
    #else
        QEventLoop eventloop;
        QTimer::singleShot(time, &eventloop, SLOT(quit()));
        eventloop.exec();
    #endif
#endif
    }
}

inline bool QtWait(bool &pFlag, unsigned int msec)
{
    if(msec == 0)
    {
        while(!pFlag)
            QtSleep(10);
    }
    else
    {
        int tmpNum = msec/10;
        int tmpCount = 0;
        while((tmpCount++<tmpNum) && !pFlag)
            QtSleep(10);
    }
    return pFlag;
}

inline bool QtWait(bool &pFlag, bool pEqlFlag, unsigned int msec)
{
    if(msec == 0)
    {
        while(pFlag!=pEqlFlag)
            QtSleep(10);
    }
    else
    {
        int tmpNum = msec/10;
        int tmpCount = 0;
        while((tmpCount++<tmpNum) && (pFlag!=pEqlFlag))
            QtSleep(10);
    }
    return (bool)(pFlag==pEqlFlag);
}

inline bool QtWait(int &pFlag, int pEqlFlag, unsigned int msec)
{
    if(msec == 0)
    {
        while(pFlag!=pEqlFlag)
            QtSleep(10);
    }
    else
    {
        int tmpNum = msec/10;
        int tmpCount = 0;
        while((tmpCount++<tmpNum) && (pFlag!=pEqlFlag))
            QtSleep(10);
    }
    return (bool)(pFlag==pEqlFlag);
}

inline bool QtWait(TaskState &pFlag, TaskState pEqlFlag, unsigned int msec)
{
    if(msec == 0)
    {
        while(pFlag!=pEqlFlag)
            QtSleep(10);
    }
    else
    {
        int tmpNum = msec/10;
        int tmpCount = 0;
        while((tmpCount++<tmpNum) && (pFlag!=pEqlFlag))
            QtSleep(10);
    }
    return (bool)(pFlag==pEqlFlag);
}

/*
    pType 1:ro
          2:rw
*/
bool isDevRW(QString pPath);
bool ReMount(QString pPath, int pType);

bool isDevExist(QString pPath);

typedef enum{
    Positive=0,
    Negative=1
} ORIENTATION;      //>@正序 逆序

typedef struct{
    DIRECTION mDirection;
    ORIENTATION mOrientation;
} CONSTRAINT;

/*!
 * Enums for port settings.
 */
typedef enum
{
    WIN_NAMES,
    IRIX_NAMES,
    HPUX_NAMES,
    SUN_NAMES,
    DIGITAL_NAMES,
    FREEBSD_NAMES,
    LINUX_NAMES
}NamingConvention;

typedef enum
{
    BAUD50 = 50,                //POSIX ONLY
    BAUD75 = 75,                //POSIX ONLY
    BAUD110 = 110,
    BAUD134 = 134,               //POSIX ONLY
    BAUD150 = 150,               //POSIX ONLY
    BAUD200 = 200,               //POSIX ONLY
    BAUD300 = 300,
    BAUD600 = 600,
    BAUD1200 = 1200,
    BAUD1800 = 1800,              //POSIX ONLY
    BAUD2400 = 2400,
    BAUD4800 = 4800,
    BAUD9600 = 9600,
    BAUD14400 = 14400,             //WINDOWS ONLY
    BAUD19200 = 19200,
    BAUD38400 = 38400,
    BAUD56000 = 56000,             //WINDOWS ONLY
    BAUD57600 = 57600,
    BAUD76800 = 76800,             //POSIX ONLY
    BAUD115200 = 115200,
    BAUD128000 = 128000,            //WINDOWS ONLY
    BAUD256000 = 256000             //WINDOWS ONLY
}BaudRateType;

typedef enum
{
    DATA_5 = 5,
    DATA_6 = 6,
    DATA_7 = 7,
    DATA_8 = 8
}DataBitsType;

typedef enum
{
    PAR_NONE,
    PAR_ODD,
    PAR_EVEN,
    PAR_MARK,               //WINDOWS ONLY
    PAR_SPACE
}ParityType;

typedef enum
{
    START_1 = 1
}StartBitsType;

typedef enum
{
    STOP_1 = 1,
    STOP_1_5 = 3,               //WINDOWS ONLY
    STOP_2 = 2
}StopBitsType;

typedef enum
{
    FLOW_OFF,
    FLOW_HARDWARE,
    FLOW_XONXOFF
}FlowType;

typedef enum{
    LCDIF = 0,
    LVDSIF,
    VGAIF,
    HDMIIF
} OUTPUTDEVICE;

typedef enum{
    TYPE_CH = 0,
    TYPE_EN,
    TYPE_FR,
    TYPE_DE,
    TYPE_JP
} LANGUAGE;

typedef enum{
    TYPE_LOWER=0,
    TYPE_UPPER
} TEXTCASE;

typedef enum{
    STATE_NONE = 0,
    STATE_ITEMUI,      //>@一级设置菜单
    STATE_CTRLUI,      //>@二级设置菜单
    STATE_FOCUSUI      //>@
} MANUAL_STATE;

typedef enum{
    KEY_SET = 0,
    KEY_UP,
    KEY_DN,
    KEY_BACK
} KEY_OPERATE;

#define D_INVALID_NUM       (0x7f)

enum _RC_INDEX{
    INVALIDRC   = 0x7f7f7f7f,
    SHOWRC      = 0x6f7f7f7f,
    HIDERC      = 0x5f7f7f7f,
    PREVRC      = 0x4f7f7f7f,
    NEXTRC      = 0x3f7f7f7f,
    HOLDRC      = 0x3e6f7f7f,
    REPEATONERC = 0x2f7f7f7f,
    REPEATALLRC = 0x1f7f7f7f,
    EXTRARC     = 0x7f7f7f00,
    //>@
    BGRC        = 100,
    TRANSPARENTRC = 10000,
    RCDISABILITY= 0x00000018
};

#define D_DEFAULT_PREFIX      QString("RC")
#define D_PREFIX              QString("Prefix")

inline int GetKey(QString pString, QString pPrefix = D_DEFAULT_PREFIX)
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

inline quint32 GetRcNum(QString pRc)
{
    bool ok=false;
    quint32 tmpNum = INVALIDRC;
    tmpNum = pRc.toUInt(&ok, 10);
    if(!ok)
    {
        if(pRc.compare("Next", Qt::CaseInsensitive) == 0)
            tmpNum = NEXTRC;
        else if(pRc.contains("Prev", Qt::CaseInsensitive))
            tmpNum =  PREVRC;
    }
    return tmpNum;
}

typedef enum{
    Image0=0,
    Image1=1,
    Image2=2,
    Image3=3,
    Image4=4,
    Image5=5,
    Image6=6,
    Image7=7,
    Image8=8,
    Image9=9,
    ImageA=10,
    ImageB=11,
    ImageC=12,
    ImageD=13,
    ImageE=14,
    ImageF=15,
    ImageG=16,
    ImageH=17,
    ImageI=18,
    ImageJ=19,
    ImageK=20,
    ImageL=21,
    ImageM=22,
    ImageN=23,
    ImageO=24,
    ImageP=25,
    ImageQ=26,
    ImageR=27,
    ImageS=28,
    ImageT=29,
    ImageU=30,
    ImageV=31,
    ImageW=32,
    ImageX=33,
    ImageY=34,
    ImageZ=35
}IMAGE_INDEX;

typedef enum{
    Text0=0,
    Text1=1,
    Text2=2,
    Text3=3,
    Text4=4,
    Text5=5,
    Text6=6,
    Text7=7,
    Text8=8,
    Text9=9,
    TextA=10,
    TextB=11,
    TextC=12,
    TextD=13,
    TextE=14,
    TextF=15,
    TextG=16,
    TextH=17,
    TextI=18,
    TextJ=19,
    TextK=20,
    TextL=21,
    TextM=22,
    TextN=23,
    TextO=24,
    TextP=25,
    TextQ=26,
    TextR=27,
    TextS=28,
    TextT=29,
    TextU=30,
    TextV=31,
    TextW=32,
    TextX=33,
    TextY=34,
    TextZ=35
}TEXT_INDEX;

//>@COM

#define SelectedZValueDiff    10000
#define ZValueDiff            500

#define VERTEXRADIUS    5                   //>@框顶点方块半径,指边长的一半
#define SIDERADIUS      4                    //>@框边方块半径
#define COM_MINSIZE     2

#define FRAMEVALIDWIDTH 10.0                  //>@框边和顶点的有效宽度，用于设置边框的灵敏度

//#define COMNAME         0
#define COMTYPE         1

typedef enum{
    DRAGLEFT,
    DRAGTOP,
    DRAGRIGHT,
    DRAGBOTTOM,
    DRAGLEFTTOP,
    DRAGRIGHTTOP,
    DRAGRIGHTBOTTOM,
    DRAGLEFTBOTTOM,
    DRAGALL,
    DRAGNONE
} COM_OPERTATE;

typedef enum{
    EffectTypeNone=0,
    EffectTypeTileRoll,
    EffectTypeRoll,
    EffectTypeFlip,
    EffectTypeBlink,    //>@闪烁
    EffectTypeRotate,
    EffectTypeTwirl,
    EffectTypeZoom,
    EffectTypeTransparent,
    EffectTypeSpread,
    EffectTypeWheel,
    EffectTypeSlipCycle,
    //>@还未实现的特效
    EffectTypeMove,
    EffectTypeGlide,
    EffectTypeFireworks,//>@烟花
    EffectTypeViewer,
    EffectTypeCube,
    EffectTypeCylinder,
    EffectTypeShake,    //>@震动
    EffectTypeBulge,    //>@凸出
    EffectTypeIndent,   //>@凹入
    EffectTypeSpectrum,
    EffectTypeShine,
    EffectTypeSwipe,
    EffectTypeBtnZoom
}EffectType;

inline EffectType GetEffectType(QString pType)
{
    if(pType.compare("Roll", Qt::CaseInsensitive) == 0)
        return EffectTypeRoll;
    else if(pType.compare("TileRoll", Qt::CaseInsensitive) == 0)
        return EffectTypeTileRoll;
    else if(pType.compare("Zoom", Qt::CaseInsensitive) == 0)//>@缩放
        return EffectTypeZoom;
    else if(pType.compare("Transparent", Qt::CaseInsensitive) == 0)//>@渐变透明
        return EffectTypeTransparent;
    else if(pType.compare("Spectrum", Qt::CaseInsensitive) == 0)//>@频谱
        return EffectTypeSpectrum;
    else if(pType.compare("Spread", Qt::CaseInsensitive) == 0)//>@扩散
        return EffectTypeSpread;
    else if(pType.compare("Wheel", Qt::CaseInsensitive) == 0)//>@光圈
        return EffectTypeWheel;
    else if(pType.compare("Rotate", Qt::CaseInsensitive) == 0)//>@旋转
        return EffectTypeRotate;
    else if(pType.compare("Twirl", Qt::CaseInsensitive) == 0)//>@转动
        return EffectTypeTwirl;
    else if(pType.compare("SlipCycle", Qt::CaseInsensitive) == 0)//>@回滚
        return EffectTypeSlipCycle;
    else if(pType.compare("Blink", Qt::CaseInsensitive) == 0)//>@闪烁
        return EffectTypeBlink;
    else if(pType.compare("Flip", Qt::CaseInsensitive) == 0)
        return EffectTypeFlip;
    else if(pType.compare("Move", Qt::CaseInsensitive) == 0)
        return EffectTypeMove;
    else if(pType.compare("Glide", Qt::CaseInsensitive) == 0)//>@滑动
        return EffectTypeGlide;
    else if(pType.compare("Fireworks", Qt::CaseInsensitive) == 0)//>@烟花
        return EffectTypeFireworks;
    else if(pType.compare("Viewer", Qt::CaseInsensitive) == 0)
        return EffectTypeViewer;
    else if(pType.compare("Cube", Qt::CaseInsensitive) == 0)
        return EffectTypeCube;
    else if(pType.compare("Cylinder", Qt::CaseInsensitive) == 0)
        return EffectTypeCylinder;
    else if(pType.compare("Shake", Qt::CaseInsensitive) == 0)//>@震动
        return EffectTypeShake;
    else if(pType.compare("Bulge", Qt::CaseInsensitive) == 0)//>@凸出
        return EffectTypeBulge;
    else if(pType.compare("Indent", Qt::CaseInsensitive) == 0)//>@凹入
        return EffectTypeIndent;
    else if(pType.compare("Shine", Qt::CaseInsensitive) == 0)//>@发光
        return EffectTypeShine;
    else if(pType.compare("Swipe", Qt::CaseInsensitive) == 0)//>@拨动
        return EffectTypeSwipe;
    else if(pType.compare("BtnZoom", Qt::CaseInsensitive) == 0)//>@拨动
        return EffectTypeBtnZoom;
    return EffectTypeNone;
}

typedef enum{
    Last=0,
    Current=1,
    Next=2
} ORDER;

typedef enum{
    Separator = 3,
    Hundred = 2,
    Ten = 1,
    Single = 0
} NumSite;

typedef enum{
    STEP0 = 0,
    STEP1 = 1
} OPERATE_STEP;

typedef struct{
    QPixmap  mLastPointer;       //>@使用过的资源指针（也即当前正在使用的）
    QPixmap  mCurPointer;        //>@待使用的资源指针
}PIXMAP_POINTER;

typedef struct{
    int       mLastPointer;       //>@使用过的资源指针（也即当前正在使用的）
    int       mCurPointer;        //>@待使用的资源指针
}RC_POINTER;

typedef enum{COMRC_UNKNOW=0,
             COMRC_IMAGE,
             COMRC_AUDIO,
             COMRC_FILM,
             COMRC_TEXT,
             COMRC_FORM,
             COMRC_SIGMAP} COMRC_TYPE;

inline COMRC_TYPE GetComRcType(QString pType)
{
    if(!pType.compare("Image", Qt::CaseInsensitive))
        return COMRC_IMAGE;
    else if(!pType.compare("Audio", Qt::CaseInsensitive))
        return COMRC_AUDIO;
    else if(!pType.compare("Film", Qt::CaseInsensitive))
        return COMRC_FILM;
    else if(!pType.compare("Text", Qt::CaseInsensitive))
        return COMRC_TEXT;
    else if(!pType.compare("Form", Qt::CaseInsensitive))
        return COMRC_FORM;
    else if(!pType.compare("SignalMap", Qt::CaseInsensitive))
        return COMRC_SIGMAP;
    return COMRC_UNKNOW;
}

class AREA_STYLE
{
public:
    AREA_STYLE() { mRcType = COMRC_UNKNOW; }
    QVariant GetPara(int pName) {return mParas.value(pName);}
    void SetPara(int pName, QVariant pValue) { mParas.insert(pName, pValue);}
public:
    COMRC_TYPE              mRcType;       //>@所使用的资源类型
    QHash<int, QVariant> mParas;
};
Q_DECLARE_METATYPE(AREA_STYLE)

class MAP_STYLE
{
public:
    MAP_STYLE() { mType = COMRC_UNKNOW; }
    QVariant GetPara(int pName) {return mParas.value(pName);}
    void SetPara(int pName, QVariant pValue) { mParas.insert(pName, pValue);}
public:
    COMRC_TYPE              mType;       //>@所使用的资源类型
    QHash<int, QVariant> mParas;
};
Q_DECLARE_METATYPE(MAP_STYLE)

class AREA_ANIMATE
{
public:
    AREA_ANIMATE()
    {
        mEffectType = EffectTypeNone;
        mDirection = Horizontal;
        mOrientation = Positive;
        mFrameStart = 0;
        mFrameEnd = 10;
        mFrameCount = mFrameEnd - mFrameStart -1;
        mAnimateTime = 200;
        mLoopCount = 1;        //>@循环次数，如果为0，则表示无限循环
        mDutyRatio = 50;        //>@占空比：0~100
        mAnimateType = QTimeLine::LinearCurve;
    }
public:
    EffectType              mEffectType;   //>@特效类型，默认为无特效
    DIRECTION               mDirection;    //>@图片特效方向
    ORIENTATION             mOrientation;  //>@特效正逆序
    int                     mLoopCount;    //>@为0表示永远循环
    int                     mFrameStart, mFrameEnd, mFrameCount;
    int                     mAnimateTime;
    int                     mDutyRatio;
    QTimeLine::CurveShape   mAnimateType;
};
Q_DECLARE_METATYPE(AREA_ANIMATE)

//>@基于TCP的服务类型
typedef struct{
    QString         mMajorType;
    QString         mMinorType;
    QHostAddress    mIP;
    QString         mDescription;
} TCPSERVICE_TYPE;
Q_DECLARE_METATYPE(TCPSERVICE_TYPE)

inline uint qHash(TCPSERVICE_TYPE pType)
{
    return qHash(pType.mMajorType+pType.mMinorType+pType.mIP.toString());
}

typedef enum{
    OPERATE_NONE = 0,
    //>@
    OPERATE_FORM,
    OPERATE_RC,
    OPERATE_MAP,
    //>@
    OPERATE_INIT,
    OPERATE_RELEASE,
    //>@
    OPERATE_PROGRESS,
    //>@动作参数
    OPERATE_CHANGE,
    OPERATE_SLIP,   //>@滑动
    OPERATE_TURN, //>@转向
    OPERATE_ADD,
    OPERATE_MODIFY,
    OPERATE_DEL,
    //>@
    OPERATE_CLEAR,
    OPERATE_SHOW,
    OPERATE_HIDE,
    //>@播放参数
    OPERATE_RUN,
    OPERATE_HALT,
    OPERATE_PLAY,
    OPERATE_PAUSE,
    OPERATE_STOP,
    OPERATE_STEADY,
    OPERATE_VOCAL,  //>@有声
    OPERATE_MUTE,   //>@静音
    OPERATE_RESUME,
    OPERATE_REPEAT,
    //>@用于测试
    OPERATE_KEYPRESS,
    OPERATE_KEYLIGHT,
    OPERATE_KEYCANCEL,
    OPERATE_KEYMOVE,
    OPERATE_KEYRELEASE
} AREA_OPERATE;

Q_DECLARE_METATYPE(AREA_OPERATE)   //>@自定义信号参数
Q_DECLARE_METATYPE(QProcess::ExitStatus)

inline AREA_OPERATE GetOperateType(QString pType)
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

inline QString GetOperateString(AREA_OPERATE pType)
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

typedef enum{
    MODE_NOREPEAT = 0,
    MODE_REPEATONE,
    MODE_REPEATALL
} REPEAT_MODE;

inline REPEAT_MODE GetRepeatMode(QString pMode)
{
    if(pMode.compare("RepeatOne"), Qt::CaseInsensitive == 0)
        return MODE_REPEATONE;
    else if(pMode.compare("RepeatAll"), Qt::CaseInsensitive == 0)
        return MODE_REPEATALL;
    return MODE_NOREPEAT;
}

typedef enum{
    RC_BGIMG = 0,
    //>@arrow
    RC_ARROWUP,
    RC_ARROWDOWN,
    RC_ARROWUPDOWN
} AREA_RC;

class OPERATE_INFO
{
public:
    OPERATE_INFO()
    {
        mValid = false;
        mOperate = OPERATE_NONE;
    }
public:
    bool            mValid;
    AREA_OPERATE    mOperate;
};

enum _UPDATE_PARA{
    PARA_START = 0,
    PARA_MANUAL = 1,
    PARA_LEFTBTN = 2,
    PARA_AUTO = 3
};

#define PARA_CURVESHAPE  QString("AnimateType")  //>@特效动画类型
#define PARA_FRAMERANGE  QString("FrameRange")   //>@特效帧频率
#define PARA_LOOPCOUNT   QString("LoopCount")    //>@循环次数
#define PARA_INTERVAL    QString("Interval")     //>@特效时长
#define PARA_DIRECTION   QString("Direction")    //>@特效方向
#define PARA_ORIENTATION QString("Orientation")  //>@特效正逆序
#define PARA_AMOUNT      QString("Amount")       //>@数量
#define PARA_DUTYRATIO   QString("DutyRatio")    //>@占空比
#define PARA_AUTOPLAY    QString("AutoPlay")
#define PARA_RCTYPE      QString("RcType")       //>@资源类型：Image/Text

#define PARA_LAYOUT      QString("Layout")       //>@布局
#define PARA_ROW         QString("Row")          //>@布局中行总数
#define PARA_COLUMN      QString("Column")       //>@布局中列总数
#define PARA_RSPACE      QString("RowSpace")     //>@布局中元素横向间隔
#define PARA_CSPACE      QString("ColumnSpace")  //>@布局中元素纵向间隔

#define PARA_STYLE       QString("Style")        //>@布局样式
#define PARA_ALIGN       QString("Align")
#define PARA_PATTERN     QString("Pattern")

//>@3D
#define PARA_ANGLEDIFF   QString("AngleDiff")       //>@一次旋转角度

typedef enum{
    ANGEL_0 = 0,
    ANGEL_90 = 90,
    ANGEL_180 = 180,
    ANGEL_270 = 270
} ROTATE;

//>@用于文件管理和UI组件管理的操作码
typedef enum{
    CodeNone=0,
    CodeInsert,
    CodeHide,
    CodeMap,
    CodePlay,
    CodeStop,
    CodeDelete,
    CodeUnActive,
    CodeActive,  //>@使之处于活动页面
    CodeMove,
    CodeZoom,
    CodeUpdate,
    CodeSelect,
    CodeRefresh,
    CodeSave,
    CodeSaveRC,
    CodeRegularRC
}ITEM_OPERATECODE;

//>@文件类型
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

typedef enum{
    CODEC_INVALID=0,
    CODEC_MPEG4,
    CODEC_H264
}CODEC_TYPE;

//>@组件类型
typedef enum{
    COM_INVALID=0,
    COM_FRAME,
    COM_LEFTFRAME,
    COM_TOPFRAME,
    COM_RIGHTFRAME,
    COM_BOTTOMFRAME,
    COM_NOTE,
    COM_LINE,
    //>@-------------桌面
    COM_DESKTOP,
    //>@-------------以下为区域
    COM_AREA,
    //>@
    COM_SETBAR,
    COM_MESSAGEBAR,
    COM_PROGRESSBAR,
    COM_ANIMAL,
    //>@Function Info Reginon
    COM_FLOOR,
    COM_FLRSIGN,
    COM_ARROW,
    COM_FUNCTION,
    //>@MultiMedia Reginon
    COM_MEDIA,
    COM_VIDEO,
    COM_MUSIC,
    COM_IMAGE,
    COM_WEBVIEW,
    COM_QML,
    COM_OPENGL,
    COM_AUDIO,
    COM_PHONE,
    //>@Text Bulletin Reginon
    COM_TEXT,
    //>@Integrated Info Reginon
    COM_LOGO,
    COM_WEATHER,
    COM_CLOCK,
    COM_NAMEPLATE,
    COM_RENCODE,
    //>@Button Operate Reginon
    COM_FLRBTN,
    COM_FLRLCD,
    COM_KEYBOARDBTN,    //>@键盘
    COM_CALLBTN,        //>@外呼按钮
    COM_PHONEBTN,
    COM_EMERGENCYBTN,   //>@警铃按钮
    COM_TALKBACKBTN,    //>@对讲按钮
    COM_OPENBTN,
    COM_CLOSEBTN,
    COM_SETBTN,
    COM_DISABILITYBTN,
    //>@检修
    COM_STATUS,
    //>@-------------以下为组件
    COM_COMPONENT,
    //>@-------------以下为子部件
    COM_FLRBTNTXT,
    COM_FLRBTNDESC,
    COM_FLRBTNANIM
} COM_TYPE;

typedef enum{
    ROTATE_0 = 0,
    ROTATE_90 = 90,
    ROTATE_180 = 180,
    ROTATE_270 = 270
} ROTATE_ANGLE;

typedef enum{
    STAGE_IN = 0,
    STAGE_OUT
} ZOOM_STAGE;

typedef enum{
    UI_UNKNOW=0,
    UI_LABEL,
    UI_STRING,
    UI_CHECK,
    UI_ENUM,
    UI_NUM,
    UI_BOOL,
    UI_MATRIX,
    UI_LINK,
    UI_ENETADDR,
    UI_TIMEEDIT,
    UI_BUTTON,
    UI_FILE,
    UI_MAP,
    UI_STACK
}UI_TYPE;

class FileTreeItem;
class TabPage;

class PAGE_INFO : public QObject
{
    Q_OBJECT
public:
    PAGE_INFO()
    {
        PageDialogPtr = 0;
        TreeItem = 0;
    }
    FILE_TYPE               FileType;
    QString                 FileDir;
    QString                 FileName;
    QVariant                Parameter;
    QTreeWidgetItem        *TreeItem;   //>@在项目列表中的树项
    QString                 IconName;
    TabPage                *PageDialogPtr;
};

typedef struct{
    QString CategoryType;
    QString ItemType;
    QString FileDir;
    QString FileName;
    QString IconName;
    bool BusyFlag;  //>@访问锁
} FILE_INFO;

typedef struct{
    QString Icon;
    QString Resolution;     //>@分辨率
    bool SupportVideo;      //>@是否支持视频
    bool SupportOpenGL;     //>@是否支持OpenGL
    QStringList Protocol;   //>@支持的接口协议
    QString Description;    //>@项目文件描述
} SolutionCategory;

typedef struct{
    QString Icon;
    QString Suffix;         //>@文件后缀
    QString Description;    //>@项目文件描述
} FileCategory;

typedef struct{
    QStringList NickNames;  //>@0表示RCName，>1表示NickName
    QStringList RcTypes;
}RESOURCE;

typedef struct{
    QString ParaName;
    QString ParaValue;
}EFFECTPARA;

typedef struct{
    QString EffectName;
    QList<EFFECTPARA*> EffectParas;
    QList<RESOURCE*> Resources;
}EFFECT;

typedef struct{
    QString     StateName;
    QList<EFFECT*> Effects;
}STATEMACHINE;

typedef struct{
    QString Icon;
    COM_TYPE CurType;
    COM_TYPE ParentType;
    QList<STATEMACHINE*> StateMachines;
} ComCategory;

typedef enum{
    ParaInvalid=0,
    ParaBool,
    ParaNum,
    ParaStr,
    ParaEnum,
    ParaCheck
} ParaType;

typedef struct{
    QString     mItemName;
    ParaType    mControlType;
    QVariant    mValues;
    QVariant    mCurValue;
} SubCfgItemInfo;

typedef struct{
    QString                 mPhysicsType;
    QList<SubCfgItemInfo*>  mSubItems;
} CfgItemInfo;

class RESOURCE_INFO
{
public:
    RESOURCE_INFO(){}
    ~RESOURCE_INFO(){}
public:
    QString RcName;
    QString RcFile;
public:
    bool operator==(const RESOURCE_INFO &s)
    {
        if(this->RcName.compare(s.RcName))
            return false;
        if(this->RcFile.compare(s.RcFile))
            return false;
        return true;
    }
    bool operator!=(const RESOURCE_INFO &s)  { return !operator==(s); }
};

class EFFECT_INFO
{
public:
    EFFECT_INFO(){RcFiles.clear();}
    ~EFFECT_INFO(){RcFiles.clear();}
    void clear()
    {
        EffectType.clear();
        EffectPara.clear();
        RcFiles.clear();
    }
public:
    QString EffectType;
    QString EffectPara;
    QList<RESOURCE_INFO> RcFiles;
public:
    bool operator==(const EFFECT_INFO &s)
    {
        if(this->EffectType.compare(s.EffectType))
            return false;
        if(this->EffectPara.compare(s.EffectPara))
            return false;
        if(RcFiles.count() != s.RcFiles.count())
            return false;
        for(int i=0;i<RcFiles.count();i++)
        {
            RESOURCE_INFO tmpSrc = RcFiles.at(i);
            RESOURCE_INFO tmpDst = s.RcFiles.at(i);
            if(tmpSrc != tmpDst)
                return false;
        }
        return true;
    }
    bool operator!=(const EFFECT_INFO &s)  { return !operator==(s); }
    EFFECT_INFO & operator=(EFFECT_INFO &s)
    {
        if(*this == s)
            return *this;
        EffectType = s.EffectType;
        EffectPara = s.EffectPara;
        RcFiles.clear();
        for(int i=0;i<s.RcFiles.count();i++)
        {
            RcFiles.insert(i, s.RcFiles.at(i));
        }
        return *this;
    }
};

class STATE_INFO
{
public:
    STATE_INFO(){}
    ~STATE_INFO(){}
public:
    QString      StateName;
    EFFECT_INFO  Effect;
public:
    bool operator==(const STATE_INFO &s)
    {
        if(this->StateName.compare(s.StateName))
            return false;
        if(Effect != s.Effect)
            return false;
        return true;
    }
    bool operator!=(const STATE_INFO &s)  { return !operator==(s); }
    STATE_INFO & operator=(STATE_INFO &s)
    {
        if(*this == s)
            return *this;
        StateName = s.StateName;
        Effect = s.Effect;
        return *this;
    }
};

typedef struct _Com_Struct{
    QGraphicsWidget      *ComponentPtr;
    QString               ComPath;        //>@/Function/Floor
    QRectF                ComRect;
    QList<STATE_INFO*>    StateMachine;       //>@状态机
}COM_STRUCT;

typedef enum{
    MODE_INVALID = 0,
    MODE_AUTO,      //>@自动运行模式
    MODE_MANUAL     //>@手动设置模式
} WORKMODE;

typedef enum{
    ITEM_BASE=65536,   //UserType=65536
    ITEM_LAY,
    ITEM_TEXT,
    ITEM_FRAME,
    ITEM_INVALD
} IMAGEITEM_TYPE;

typedef enum{
    //>@建断开连接状态
    S_OFF =0,//>@离线，比如Storage拔出，网线断掉
    //>@错误状态
    S_LOCK,
    S_ERROR,
    //>@建立连接状态
    S_ON,         //>@在线，只要是扫描到的都算在线，要与Netmanager中的硬/软连接区分开来
    S_SYNC,        //>@同步状态，只要是建立了软连接都算同步状态
    //>@其它
    S_NACK,            //>@设备有应答
    S_ACK,            //>@设备有应答
    S_PWM
} DEVICESTATE;

#define D_PHYSICSINTERFACE  QString("PID")
#define D_STATECODE         QString("SC")

//>@获取设备状态，返回是否Plug
inline DEVICESTATE GetDevStatus(QDomElement pDevElement)
{
    DEVICESTATE tmpState = S_OFF;
    if(pDevElement.isNull())
        return tmpState;
    QString tmpDevStatus = pDevElement.attribute(D_STATECODE);
    if(tmpDevStatus.compare("OFF", Qt::CaseInsensitive) == 0)
            tmpState = S_OFF;
    else if(tmpDevStatus.compare("ON", Qt::CaseInsensitive) == 0)
        tmpState = S_ON;
    else if(tmpDevStatus.compare("SYNC", Qt::CaseInsensitive) == 0)
        tmpState = S_SYNC;
    else if(tmpDevStatus.compare("LOCK", Qt::CaseInsensitive) == 0)
        tmpState = S_LOCK;
    else if(tmpDevStatus.compare("ERROR", Qt::CaseInsensitive) == 0)
        tmpState = S_ERROR;
    else if(tmpDevStatus.compare("ACK", Qt::CaseInsensitive) == 0)
        tmpState = S_ACK;
    else if(tmpDevStatus.compare("NACK", Qt::CaseInsensitive) == 0)
        tmpState = S_NACK;
    else if(tmpDevStatus.compare("PWM", Qt::CaseInsensitive) == 0)
        tmpState = S_PWM;
    return tmpState;
}

inline DEVICESTATE GetDevStatus(QDomElement pDevElement, QString &pDevPath)
{
    if(pDevElement.isNull())
        pDevPath.clear();
    else
        pDevPath = pDevElement.text();
    return GetDevStatus(pDevElement);
}

typedef enum{
    M_INVALID = 0,  //>@0
    M_QT,
    M_GTK,
    M_MiniGUI,
    M_Busybox,
    M_FTP,
    M_SSH,
    M_SDL,
    M_Mplayer,
    M_FFMPEG,
    M_Gstreamer,
    M_OpenGL,
    M_RMII,
    M_USBRMII,
    M_USBNET,
    M_GPRS,
    M_WIFI,
    M_BlueTooth,
    M_NFC,
    M_USBCOM,
    M_Camera,
    M_Mouse,
    M_Touch,
    M_Keyboard
} MW_Type;  //>@中间件类型

typedef enum{
    P_INVALID = 0,  //>@0
    P_WD,
    P_CPU,
    P_MEM,
    P_LED,
    P_DISK,
    P_MDISK,  //>@必须要在P_Player前面。
    P_LCD,
    P_Touch,
    P_RTC,
    P_Player,
    P_LANC,  //>@局域网内的客户端
    P_LANR,  //>@局域网内的路由器
    P_LANH,
    P_VP,
    P_COM,
    P_SAF,
    P_DIAL,  //>@必须在最后
    //>@
    P_UDP,
    P_TCP,
    P_TEST
} PH_Type;

Q_DECLARE_METATYPE(PH_Type)
inline uint qHash(PH_Type pType) { return (uint)pType; }

typedef enum{
    L_INVALID = 0,
    L_Monitor,
    L_CardReader,
    L_NFCP2P,
    L_ContentManager,
    L_ThemeManager
} LG_Type;

Q_DECLARE_METATYPE(LG_Type)
inline uint qHash(LG_Type pType) { return (uint)pType; }

typedef enum {
    DEV_NONE=0,
    DEV_MIDDLEWARE,
    DEV_PHYSICAL,
    DEV_LOGIC
} DEV_TYPE;

inline DEV_TYPE GetDevType(QString pDevName)
{
    if(pDevName.contains("Physical"))
        return DEV_PHYSICAL;
    else if(pDevName.contains("Logic"))
        return DEV_LOGIC;
    else if(pDevName.contains("Middleware"))
        return DEV_MIDDLEWARE;
    return DEV_NONE;
}

inline QString GetDevTypeString(DEV_TYPE pDevType)
{
    if(pDevType == DEV_PHYSICAL)
        return QString("Physical");
    else if(pDevType == DEV_LOGIC)
        return QString("Logic");
    else if(pDevType == DEV_MIDDLEWARE)
        return QString("Middleware");
    return QString();
}

inline MW_Type GetMWType(QString pPara)
{
    MW_Type MWType = M_INVALID;
    if(pPara.isEmpty())
        return MWType;
    if(pPara.compare("QT", Qt::CaseInsensitive) == 0)
        MWType = M_QT;
    else if(pPara.compare("GTK", Qt::CaseInsensitive) == 0)
        MWType = M_GTK;
    else if(pPara.compare("MiniGUI", Qt::CaseInsensitive) == 0)
        MWType = M_MiniGUI;
    else if(pPara.compare("Busybox", Qt::CaseInsensitive) == 0)
        MWType = M_Busybox;
    else if(pPara.compare("FTP", Qt::CaseInsensitive) == 0)
        MWType = M_FTP;
    else if(pPara.compare("SSH", Qt::CaseInsensitive) == 0)
        MWType = M_SSH;
    else if(pPara.compare("SDL", Qt::CaseInsensitive) == 0)
        MWType = M_SDL;
    else if(pPara.compare("Mplayer", Qt::CaseInsensitive) == 0)
        MWType = M_Mplayer;
    else if(pPara.compare("FFMPEG", Qt::CaseInsensitive) == 0)
        MWType = M_FFMPEG;
    else if(pPara.compare("Gstreamer", Qt::CaseInsensitive) == 0)
        MWType = M_Gstreamer;
    else if(pPara.compare("OpenGL", Qt::CaseInsensitive) == 0)
        MWType = M_OpenGL;
    else if(pPara.compare("RMII", Qt::CaseInsensitive) == 0)
        MWType = M_RMII;
    else if(pPara.compare("USBRMII", Qt::CaseInsensitive) == 0)
        MWType = M_USBRMII;
    else if(pPara.compare("USBNET", Qt::CaseInsensitive) == 0)
        MWType = M_USBNET;
    else if(pPara.compare("GPRS", Qt::CaseInsensitive) == 0)
        MWType = M_GPRS;
    else if(pPara.compare("WIFI", Qt::CaseInsensitive) == 0)
        MWType = M_WIFI;
    else if(pPara.compare("BlueTooth", Qt::CaseInsensitive) == 0)
        MWType = M_BlueTooth;
    else if(pPara.compare("NFC", Qt::CaseInsensitive) == 0)
        MWType = M_NFC;
    else if(pPara.compare("USBCOM", Qt::CaseInsensitive) == 0)
        MWType = M_USBCOM;
    else if(pPara.compare("Camera", Qt::CaseInsensitive) == 0)
        MWType = M_Camera;
    else if(pPara.compare("Mouse", Qt::CaseInsensitive) == 0)
        MWType = M_Mouse;
    else if(pPara.compare("Touch", Qt::CaseInsensitive) == 0)
        MWType = M_Touch;
    else if(pPara.compare("Keyboard", Qt::CaseInsensitive) == 0)
        MWType = M_Keyboard;
    return MWType;
}

inline PH_Type GetPHType(QString pPara)
{
    PH_Type PHType = P_INVALID;
    if(pPara.isEmpty())
        return PHType;
    if(pPara.compare("CPU", Qt::CaseInsensitive) == 0)
        PHType = P_CPU;
    else if(pPara.compare("MEM", Qt::CaseInsensitive) == 0)
        PHType = P_MEM;
    else if(pPara.compare("Disk", Qt::CaseInsensitive) == 0)
        PHType = P_DISK;
    else if(pPara.compare("RTC", Qt::CaseInsensitive) == 0)
        PHType = P_RTC;
    else if(pPara.compare("Player", Qt::CaseInsensitive) == 0)
        PHType = P_Player;
    else if(pPara.compare("LANC", Qt::CaseInsensitive) == 0)
        PHType = P_LANC;
    else if(pPara.compare("LANR", Qt::CaseInsensitive) == 0)
        PHType = P_LANR;
    else if(pPara.compare("LCD", Qt::CaseInsensitive) == 0)
        PHType = P_LCD;
    else if(pPara.compare("Touch", Qt::CaseInsensitive) == 0)
        PHType = P_Touch;
    else if(pPara.compare("LED", Qt::CaseInsensitive) == 0)
        PHType = P_LED;
    else if(pPara.compare("WD", Qt::CaseInsensitive) == 0)
        PHType = P_WD;
    else if(pPara.compare("Dial", Qt::CaseInsensitive) == 0)
        PHType = P_DIAL;
    else if(pPara.compare("MobileDisk", Qt::CaseInsensitive) == 0)
        PHType = P_MDISK;
    else if(pPara.compare("COM", Qt::CaseInsensitive) == 0)
        PHType = P_COM;
    else if(pPara.compare("SAF", Qt::CaseInsensitive) == 0)
        PHType = P_SAF;
    else if(pPara.compare("VP", Qt::CaseInsensitive) == 0)
        PHType = P_VP;
    return PHType;
}
inline PH_Type GetElementPHType(QDomElement pDevElement)
{
    if(pDevElement.isNull())
        return P_INVALID;
    return GetPHType(pDevElement.tagName());
}

inline LG_Type GetLGType(QString pPara)
{
    LG_Type LGType = L_INVALID;
    if(pPara.isEmpty())
        return LGType;
    if(pPara.compare("Monitor", Qt::CaseInsensitive) == 0)
        LGType = L_Monitor;
    else if(pPara.compare("CardReader", Qt::CaseInsensitive) == 0)
        LGType = L_CardReader;
    else if(pPara.compare("NFCP2P", Qt::CaseInsensitive) == 0)
        LGType = L_NFCP2P;
    else if(pPara.compare("ThemeManager", Qt::CaseInsensitive) == 0)
        LGType = L_ThemeManager;
    else if(pPara.compare("ContentManager", Qt::CaseInsensitive) == 0)
        LGType = L_ContentManager;
    return LGType;
}

/*****************************HTML*******************************/
/*****************************HTML*******************************/

inline void stringToHtmlFilter(QString &str)
{
    //注意这几行代码的顺序不能乱，否则会造成多次替换
    str.replace("&","&amp;");
    str.replace(">","&gt;");
    str.replace("<","&lt;");
    str.replace("\"","&quot;");
    str.replace("\'","&#39;");
    str.replace(" ","&nbsp;");
    str.replace("\n","<br>");
    str.replace("\r","<br>");
}

inline void stringToHtml(QString &str, QFont pFont, QColor pColor)
{
    QByteArray array;
    array.append(pColor.red());
    array.append(pColor.green());
    array.append(pColor.blue());
    QString strC(array.toHex());
    str = QString("<span style=\" font-family:'%1'; font-size:%2pt; font-weight:%3; color:#%4;\">%5</span>").arg(pFont.family()).arg(pFont.pointSize()).arg(pFont.weight()).arg(strC).arg(str);
}

inline void imgPathToHtml(QString &path)
{
    path = QString("<img src=\"%1\"/>").arg(path);
}

/*
    QString str(" < Hello Qt!>\n");
    QColor clrR(255,0,0);
    stringToHtmlFilter(str);
    stringToHtml(str,clrR);
    m_TextBrowser->insertHtml(str);

    QString imgPath = QString("D:/img/hello.png");
    imgPathToHtml(imgPath);
    m_TextBrowser->insertHtml(imgPath);
  */

typedef struct{
    QString     mParLabel;
    QString     mTagName;
    QString     mAtPara;
    QString     mAtList;
    QString     mText;
}DomInfo;

typedef enum{
    Type_SERIAL = 0,
    Type_SOCKET = 1,
    Type_USB = 2,
    Type_UnknowDevice = 3
} COMM_TYPE;

#define D_TFLASH    QString("TFLASH")
#define D_UDISK     QString("UDISK")
#define D_GPRS      QString("GPRS")
#define D_WIFI      QString("WIFI")
#define D_NFC       QString("NFC")
#define D_CAMERA    QString("CAMERA")
#define D_TOUCH     QString("TOUCH")
#define D_ETHERNET  QString("ETHERNET")

//>@存储器的分区格式，如果为F_UNKNOW则表示保持原来分区不变
typedef enum{
    F_UNKNOW,
    F_FAT,
    F_EXT
} FORMAT_TYPE;

/*************************************************************
//>@D_Global
**************************************************************/
#define D_INVALIDTIME               18446744073709551614    //0xFFFFFFFFFFFFFFFE
#define D_MAXSERIALBLOCKSIZE        3*1024
#define D_MAXSOCKETBLOCKSIZE        32*1024
#define D_MAXUSBBLOCKSIZE           32*1024
#define D_FRAMEHEADER               0x80
#define D_FRAMEFOOTER               0x81
#define D_FRAMEHEADER_ACK           0xc0
#define D_FRAMEFOOTER_ACK           0xc1
/*************************************************************
//>@D_IB
**************************************************************/
#define D_IBFRAMELEN                11
#define D_FloorAntiShakeNum         10
#define D_DoorAntiShakeNum          10
#define D_ArrowAntiShakeNum         10
#define D_ArriveAntiShakeNum        10
#define D_VoiceAntiShakeNum         10
#define D_ButtonAntiShakeNum        10
#define D_FunctionAntiShakeNum      10

#define D_ArrowArea                 0x07
#define D_ArrowUnknow               0x00
#define D_ArrowUp                   0x01
#define D_ArrowUpRun                0x05
#define D_ArrowDown                 0x02
#define D_ArrowDownRun              0x06
#define D_ArrowUpDown               0x03
#define D_ArrowUpDownRun            0x07

#define D_FloorFresh                0x01
#define D_ArrowFresh                0x02
#define D_FunctionFresh             0x04
#define D_AudioFresh                0x08

#define D_SignalArea                0x38
#define D_SignalArrive              0x08
#define D_SignalVoice               0x10
#define D_SignalLowPower            0x20

#define D_DoorArea                  0x30
#define D_DoorOpen                  0x20
#define D_DoorClose                 0x10

#define D_Fun1Area                  0x7f
#define D_Fun2Area                  0x7f00
#define D_Fun3Area                  0x7f0000

#define D_SignalFireControl         0x20
#define D_SignalMaintenance         0x10
#define D_SignalFullLoad            0x08
#define D_SignalOverLoad            0x04
#define D_SignalEarthquake          0x02
#define D_SignalNosmoke             0x01
#define D_SignalPrior               0x20
#define D_SignalFire                0x10
#define D_SignalDriver              0x08
#define D_SignalStop                0x04
#define D_SignalLock                0x02
#define D_SignalEmergencyCall       0x01
/*************************************************************
//>@D_NFD/HB
**************************************************************/
#define NOCRC   //(NOCRC|CRC)
#define NONFDHB   //(NONFDHB|NFDHB)
#define D_HBTime                    10000
#define D_LIFE                      5
#define D_DELAY                     10
/*************************************************************
//>@Global
**************************************************************/
typedef struct{
    int                 mLuminance;
    int                 mEnergySave;
    QString             mTheme;
    QStringList         mThemeList;
    QString             mRotate;
    QStringList         mRotateList;
} LCDSet;

#ifdef SURRPORT_NETWORK
typedef struct{
    QHostAddress        mIP;
    quint16             mPort;
    QString             mSubmask;
    QString             mGateway;
    QString             mDNS;
    QString             mDHCP;
    bool                mScan;
} EthernetSet;
#endif

typedef struct{
    QString             mPort;
    BaudRateType        mBuad;
    QStringList         mBuadList;
} Terminal;

typedef enum{
    Flag_Normal = 0,
    Flag_Ack = 1,
    Flag_Unknow = 2
} FRAME_FLAG;

typedef enum{
    State_Wait = 0,
    State_Header,
    State_CmdPara,
    State_AddrH,  //>@帧计数或者寄存器地址
    State_AddrL,
    State_DatalenH,
    State_DatalenL,
    State_Data,
    State_CRC,
    State_Footer
} FRAME_STATE;

typedef enum{
    Type_ScanDev = 0x5a,
    Type_Heartbeat = 0x10,
    Type_FloorStatus2 = 0x00,
    Type_FloorStatus = 0x01,
    Type_Urgency = 0x11,
    Type_CascadeButton = 0x12,
    Type_ButtonStatus = 0x02,
    Type_DisabilityButton = 0x03,
    Type_SetCmd = 0x04,
    Type_RegCmd = 0x08,
    Type_FileTrans = 0x05,
    Type_StreamTrans = 0x06,
    Type_NONE = 0xff
} FRAME_TYPE;

inline FRAME_TYPE ToFrameType(quint8 pType)
{
    switch(pType)
    {
        case Type_ScanDev:
            return Type_ScanDev;
        case Type_Heartbeat:
            return Type_Heartbeat;
        case Type_FloorStatus:
            return Type_FloorStatus;
        case Type_Urgency:
            return Type_Urgency;
        case Type_CascadeButton:
            return Type_CascadeButton;
        case Type_ButtonStatus:
            return Type_ButtonStatus;
        case Type_DisabilityButton:
            return Type_DisabilityButton;
        case Type_SetCmd:
            return Type_SetCmd;
        case Type_RegCmd:
            return Type_RegCmd;
        case Type_FileTrans:
            return Type_FileTrans;
        case Type_StreamTrans:
            return Type_StreamTrans;
        default:
            return Type_NONE;
    }
    return Type_NONE;
}

/*************************************************************
//>@IB
**************************************************************/
typedef enum{
    Type_Send = 0,
    Type_Rev = 1
}IBPORTEVENTTYPE;

typedef enum{
    MODE_NORMAL = 0,
    MODE_DEBUG,
    MODE_CONFIGURE,
    MODE_APPUPGRADE
} IBWORKMODE;

typedef enum{
    STATE_Start = 0,
    STATE_Run,
    STATE_Stop,
    STATE_Open
} IBRUNSTATE;

typedef enum{
    ArrowUnknow = 0x00,
    ArrowUp = 0x01,
    ArrowUproll = 0x05,
    ArrowUphalt = 0x09,
    ArrowDown = 0x02,
    ArrowDownroll = 0x06,
    ArrowDownhalt = 0x10,
    ArrowUpDown = 0x03,
    ArrowUpDownroll = 0x07,
    ArrowUpDownhalt = 0x11
} ARROWSTATE;

inline ARROWSTATE GetArrowState(QString pStateName)
{
    if(pStateName.isEmpty())
        return ArrowUnknow;
    if(pStateName.compare("UP") == 0)
    {
        return ArrowUp;
    }
    else if(pStateName.compare("UPROLL") == 0)
    {
        return ArrowUproll;
    }
    else if(pStateName.compare("UPHALT") == 0)
    {
        return ArrowUphalt;
    }
    else if(pStateName.compare("DOWN") == 0)
    {
        return ArrowDown;
    }
    else if(pStateName.compare("DOWNROLL") == 0)
    {
        return ArrowDownroll;
    }
    else if(pStateName.compare("DOWNHALT") == 0)
    {
        return ArrowDownhalt;
    }
    else if(pStateName.compare("UPDOWN") == 0)
    {
        return ArrowUpDown;
    }
    else if(pStateName.compare("UPDOWNROLL") == 0)
    {
        return ArrowUpDownroll;
    }
    else if(pStateName.compare("UPDOWNHALT") == 0)
    {
        return ArrowUpDownhalt;
    }
    return ArrowUnknow;
}

typedef enum{
    AUDIO_DING=101,
    AUDIO_DINGDANG,
    AUDIO_DINGDANGDONG,
    AUDIO_DOOROPEN,
    AUDIO_DOORCLOSE,
    AUDIO_FUNCTION=200
}AUDIO_RC;

typedef enum{
    DoorUnknow = 0,
    DoorOpen,
    DoorClose
} DOOR_STATE;

typedef enum{
    FunctionUnknow = 0,
    Function1,
    Function2,
    Function3
} FUNCTION_STATE;

typedef enum{
    STATE_NORMAL = 0,
    STATE_PRESS,
    STATE_RELEASE,
    STATE_LIGHT,
    STATE_MOVE
} BUTTONSTATE;

Q_DECLARE_METATYPE(BUTTONSTATE)   //>@自定义信号参数

typedef enum{
    Flag_None = 0x00,
    Flag_FileData = 0x01,
    Flag_FileFooter = 0x02,
    Flag_FileMD5 = 0x04,
    Flag_Stop = 0x40,
    Flag_OneTrans = 0x20,
    Flag_ReTrans = 0x10
} FILETRANS_FLAG;

typedef enum{
    Conn_PHDisconn = 0,
    Conn_PHConn,
    Conn_SFDisconn,
    Conn_SFConn
} CONN_STATE;

typedef enum{
    SC_Unknow = 0,
    //>@UnQueue
    SC_Init = 1,   //>@获取设备的基础参数
    SC_Release,
    SC_Connect,
    SC_Disconnect,
    SC_Set,        //>@设置寄存器参数
    SC_Get,        //>@获取寄存器参数
    SC_Cmd,        //>@设备可直接执行的命令，比如创建路径、移动文件/夹，删除文件夹等
    SC_Ls,         //>@查询某个路径的文件列表
    SC_Copy,
    SC_Cut,
    SC_Del,
    SC_Msg,        //>@消息或调试信息
    SC_Zip,
    SC_Unzip,
    //>@Queue
    SC_SendFileTest = 20,
    SC_GetFileTest,
    SC_StartGetFile,
    //>@Father
    SC_SendFile = 40,
    SC_GetFile,
    SC_SendDir,
    SC_GetDir
}SETCMD_TYPE;

inline SETCMD_TYPE StrToSCType(QString pStr)
{
    if(pStr.isEmpty())
        return SC_Unknow;
    if(pStr.compare("Set",Qt::CaseInsensitive) == 0)
        return SC_Set;
    else if(pStr.compare("Get",Qt::CaseInsensitive) == 0)
        return SC_Get;
    //
    else if(pStr.compare("Msg",Qt::CaseInsensitive) == 0)
        return SC_Msg;
    else if(pStr.compare("Cmd",Qt::CaseInsensitive) == 0)
        return SC_Cmd;
    //
    else if(pStr.compare("Ls",Qt::CaseInsensitive) == 0)
        return SC_Ls;
    //
    else if(pStr.compare("SendFileTest",Qt::CaseInsensitive) == 0)
        return SC_SendFileTest;
    else if(pStr.compare("GetFileTest",Qt::CaseInsensitive) == 0)
        return SC_GetFileTest;
    else if(pStr.compare("StartGetFile",Qt::CaseInsensitive) == 0)
        return SC_StartGetFile;
    //
    else if(pStr.compare("SendFile",Qt::CaseInsensitive) == 0)
        return SC_SendFile;
    else if(pStr.compare("GetFile",Qt::CaseInsensitive) == 0)
        return SC_GetFile;
    else if(pStr.compare("GetDir",Qt::CaseInsensitive) == 0)
        return SC_GetDir;
    else if(pStr.compare("SendDir",Qt::CaseInsensitive) == 0)
        return SC_SendDir;
    //
    else if(pStr.compare("Copy",Qt::CaseInsensitive) == 0)
        return SC_Copy;
    else if(pStr.compare("Cut",Qt::CaseInsensitive) == 0)
        return SC_Cut;
    else if(pStr.compare("Del",Qt::CaseInsensitive) == 0)
        return SC_Del;
    //
    else if(pStr.compare("Zip",Qt::CaseInsensitive) == 0)
        return SC_Zip;
    else if(pStr.compare("Unzip",Qt::CaseInsensitive) == 0)
        return SC_Unzip;
    //>
    else if(pStr.compare("Init",Qt::CaseInsensitive) == 0)
        return SC_Init;
    else if(pStr.compare("Release",Qt::CaseInsensitive) == 0)
        return SC_Release;
    else if(pStr.compare("Connect",Qt::CaseInsensitive) == 0)
        return SC_Connect;
    else if(pStr.compare("Disconnect",Qt::CaseInsensitive) == 0)
        return SC_Disconnect;
    return SC_Unknow;
}

inline QString SCTypeToStr(SETCMD_TYPE pType)
{
    switch(pType)
    {
        case SC_Init:return QString("Init");
        case SC_Release:return QString("Release");
        case SC_Connect:return QString("Connect");
        case SC_Disconnect:return QString("Disconnect");

        case SC_Set:return QString("Set");
        case SC_Get:return QString("Get");

        case SC_Cmd:return QString("Cmd");
        case SC_Msg:return QString("Msg");

        case SC_Copy:return QString("Copy");
        case SC_Cut:return QString("Cut");
        case SC_Del:return QString("Del");

        case SC_Zip:return QString("Zip");
        case SC_Unzip:return QString("Unzip");

        case SC_Ls:return QString("Ls");
        //>@
        case SC_SendFileTest:return QString("SendFileTest");
        case SC_GetFileTest:return QString("GetFileTest");
        case SC_StartGetFile:return QString("StartGetFile");
        //>@
        case SC_SendFile:return QString("SendFile");
        case SC_GetFile:return QString("GetFile");
        case SC_GetDir:return QString("GetDir");
        case SC_SendDir:return QString("SendDir");

        default:break;
    }
    return QString();
}

class SetCmdInfo  //>@用于CMD指令:set#xx#xxxx#xxxx#success
{
public:
    SetCmdInfo()
    {
        mFatherType =  SC_Unknow;
        mRatio = 1.0;
        mEndFlag = true;
        mEmitSig = false;
        mType = SC_Unknow;
        mPara.clear();
        mDietime = 0;
    }
    SetCmdInfo(QString pCMD)
    {
        mFatherType =  SC_Unknow;
        mRatio = 1.0;
        mEndFlag = true;
        mEmitSig = false;
        mDietime = 0;
        fromFrame(pCMD);
    }
    ~SetCmdInfo()
    {
        mPara.clear();
    }

    QString Result()
    {
        if(mPara.isEmpty())
            return QString();
        return mPara.last();
    }
    bool isSuccess()
    {
        QString tmpRet = Result();
        if(tmpRet.isEmpty())
            return false;
        return !(tmpRet.compare(D_Success, Qt::CaseInsensitive));
    }

    void SetSuccess(bool pFlag, bool pClear=false)  //>@pClear表示不保留原来参数
    {
        if(pClear)
            mPara.clear();
        if(pFlag)
        {
            if(!mPara.isEmpty())
            {
                QString tmpRet = mPara.last();
                if(!tmpRet.compare(D_Success, Qt::CaseInsensitive))
                    return;
                if(!tmpRet.compare(D_Failed, Qt::CaseInsensitive))
                    mPara.takeLast();
            }
            mPara << D_Success;
        }
        else
        {
            if(!mPara.isEmpty())
            {
                QString tmpRet = mPara.last();
                if(!tmpRet.compare(D_Failed, Qt::CaseInsensitive))
                    return;
                if(!tmpRet.compare(D_Success, Qt::CaseInsensitive))
                    mPara.takeLast();
            }
            mPara << D_Failed;
        }
    }

    bool fromFrame(QString pCMD)
    {
        QStringList tmpList = pCMD.split(D_CMDSUBSPLIT);
        if(tmpList.isEmpty())
            return false;
        mType = StrToSCType(tmpList.at(0));
        mPara = tmpList.mid(1);
        return true;
    }
    QString toFrame()
    {
        QString frame = SCTypeToStr(mType);
        if(!mPara.isEmpty())
        {
            frame.append(D_CMDSUBSPLIT);
            frame.append(mPara.join(D_CMDSUBSPLIT));
        }
        return frame;
    }
    QString toContent()  //>@不包含set(type)#
    {
        if(mPara.isEmpty())
            return QString();
        return mPara.join(D_CMDSUBSPLIT);
    }

    bool operator&(const SetCmdInfo &s) const
    {
        if(mType == s.mType)
            return true;
        return false;
    }
    bool operator==(const SetCmdInfo &s) const
    {
        if((mFatherType == s.mFatherType)&&
           (mRatio == s.mRatio)&&
           (mEndFlag == s.mEndFlag)&&
           (mEmitSig == s.mEmitSig)&&
           (mType == s.mType)&&
           (mPara == s.mPara)&&
           (mDietime == s.mDietime))
            return true;
        return false;
    }
    bool operator!=(const SetCmdInfo &s) const
    {
        return !operator==(s);
    }
    SetCmdInfo& operator=(const SetCmdInfo &s)
    {
        mFatherType = s.mFatherType;
        mRatio = s.mRatio;
        mEndFlag = s.mEndFlag;
        mEmitSig = s.mEmitSig;
        mType = s.mType;
        mPara = s.mPara;
        mDietime = s.mDietime;
        return *this;
    }
    SetCmdInfo& operator=(const QString &pCMD)
    {
        if(!fromFrame(pCMD))
            SetCmdInfo();
        return *this;
   }

public:
    SETCMD_TYPE         mFatherType;    //>@父指令类型
    qreal               mRatio;
    bool                mEndFlag;
    bool                mEmitSig;
    SETCMD_TYPE         mType;          //>@指令类型
    QStringList         mPara;          //>@指令参数
    quint32             mDietime;
};

typedef enum {
    MONITOR_AUDIO = 0,
    MONITOR_VIDEO,
    MONITOR_AV
}MONITOR_TYPE;

inline QString GetMonitorTypeString(MONITOR_TYPE pType)
{
    QString pAvType;
    if(pType == MONITOR_AUDIO)
        pAvType = QString("Audio");
    else if(pType == MONITOR_VIDEO)
        pAvType = QString("Video");
    else if(pType == MONITOR_AV)
        pAvType = QString("AV");
    return pAvType;
}

inline MONITOR_TYPE GetMonitorType(QString pTypeString)
{
    MONITOR_TYPE tmpAvType;
    if(pTypeString.compare(QString("Audio")) == 0)
        tmpAvType = MONITOR_AUDIO;
    else if(pTypeString.compare(QString("Video")) == 0)
        tmpAvType = MONITOR_VIDEO;
    else if(pTypeString.compare(QString("AV")) == 0)
        tmpAvType = MONITOR_AV;
    return tmpAvType;
}

//>@---------------基础类型-------------------

typedef void (*CallBackFunc)();

class PASSWD
{
public:
    PASSWD()
    {
        m_PassWD.clear();
    }
    PASSWD(QString pPassWD)
    {
        *this = pPassWD;
    }
public:
    QString m_PassWD;
public:
    bool operator==(const PASSWD &s)
    {
        if(this->m_PassWD.compare(s.m_PassWD) != 0)
            return false;
        return true;
    }
    bool operator!=(const PASSWD &s)  { return !operator==(s); }

    PASSWD & operator=(const PASSWD &s)
    {
        if(s.m_PassWD.length() < 8)
            m_PassWD.clear();
        m_PassWD = s.m_PassWD;
        return *this;
    }

    PASSWD & operator=(const QString &s)
    {
        if(s.length() < 8)
            m_PassWD.clear();
        m_PassWD = s;
        return *this;
    }
};

class ENET_ADDR
{
public:
    ENET_ADDR()
    {
        for(int i=0;i<4;i++)
            mAddr[i] = 0;
    }
    ENET_ADDR(QString pAddr)
    {
        *this = pAddr;
    }
    ENET_ADDR(quint8 *pAddr)
    {
        for(int i=0;i<4;i++)
            mAddr[i] = pAddr[i];
    }
    enum SUBMASK {SUBMASK_A=0, SUBMASK_B, SUBMASK_C};
public:
    quint8  mAddr[4];
public:
    QList<quint32> toQList()
    {
        QList<quint32> tmpList;
        tmpList << mAddr[0] << mAddr[1] << mAddr[2] << mAddr[3];
        return tmpList;
    }
    QString toQString()
    {
        return QString("%1.%2.%3.%4").arg(mAddr[0])
                                     .arg(mAddr[1])
                                     .arg(mAddr[2])
                                     .arg(mAddr[3]);
    }
    bool isValid()
    {
        for(int i=0;i<4;i++)
        {
            if(mAddr[i] != 0)
            {
                return true;
            }
        }
        return false;
    }
    static QString GetDefaultSubMask(QString pIP)
    {
        ENET_ADDR tmpIP = pIP;
        if(tmpIP.isValid() == false)
            return QString("0.0.0.0");
        if(tmpIP.mAddr[0] >= 1 && tmpIP.mAddr[0] <= 126 &&
           tmpIP.mAddr[1] >= 0 && tmpIP.mAddr[1] <= 255 &&
           tmpIP.mAddr[2] >= 0 && tmpIP.mAddr[2] <= 255 &&
           tmpIP.mAddr[3] >= 1 && tmpIP.mAddr[3] <= 254)
        {
            //>@A网段
            return QString("255.0.0.0");
        }
        else if(tmpIP.mAddr[0] >= 128 && tmpIP.mAddr[0] <= 191 &&
                tmpIP.mAddr[1] >= 0 && tmpIP.mAddr[1] <= 255 &&
                tmpIP.mAddr[2] >= 0 && tmpIP.mAddr[2] <= 255 &&
                tmpIP.mAddr[3] >= 1 && tmpIP.mAddr[3] <= 254)
        {
            //>@B网段
            return QString("255.255.0.0");
        }
        else if(tmpIP.mAddr[0] >= 192 && tmpIP.mAddr[0] <= 223 &&
                tmpIP.mAddr[1] >= 0 && tmpIP.mAddr[1] <= 255 &&
                tmpIP.mAddr[2] >= 0 && tmpIP.mAddr[2] <= 255 &&
                tmpIP.mAddr[3] >= 1 && tmpIP.mAddr[3] <= 254)
        {
            //>@C网段
            return QString("255.255.255.0");
        }
        return QString("0.0.0.0");
    }
    bool isOnSameNet(QString pIP)
    {
        ENET_ADDR tmpIP = pIP;
        ENET_ADDR tmpSubmask = GetDefaultSubMask(pIP);
        if(isValid() && tmpIP.isValid() && tmpSubmask.isValid())
        {
            for(int i=0;i<4;i++)
            {
                quint8 tmpSubIP = tmpSubmask.mAddr[i];
                if(tmpSubIP == 0)
                    return true;
                quint8 tmp1 = mAddr[i] & tmpSubIP;
                quint8 tmp2 = tmpIP.mAddr[i] & tmpSubIP;
                if(tmp1 != tmp2)
                    return false;
            }
        }
        return false;
    }
    bool operator==(const ENET_ADDR &s)
    {
        for(int i=0;i<4;i++)
        {
            if(this->mAddr[i] != s.mAddr[i])
                return false;
        }
        return true;
    }
    bool operator!=(const ENET_ADDR &s)  { return !operator==(s); }

    ENET_ADDR & operator=(const ENET_ADDR &s)
    {
        for(int i=0;i<4;i++)
        {
            this->mAddr[i] = s.mAddr[i];
        }
        return *this;
    }
    ENET_ADDR & operator=(const QString &s)
    {
        QStringList tmpAddrList = s.split('.');
        if(tmpAddrList.count() != 4)
        {
            for(int i=0;i<4;i++)
                mAddr[i] = 0;
        }
        else
        {
            for(int i=0;i<4;i++)
            {
                bool ok = false;
                QString tmpString = tmpAddrList.at(i);
                mAddr[i] = tmpString.toInt(&ok);
                if(ok == false)
                {
                    mAddr[i] = 0;
                }
            }
        }
        return *this;
    }
};

class VERSION
{
public:
    VERSION()
    {
        for(int i=0;i<4;i++)
            mAddr[i] = 0;
    }
    VERSION(QString pAddr)
    {
        *this = pAddr;
    }
    VERSION(quint8 *pAddr)
    {
        for(int i=0;i<4;i++)
            mAddr[i] = pAddr[i];
    }

public:
    quint8  mAddr[4];
public:
    QString toQString()
    {
        return QString("%1.%2.%3.%4").arg(mAddr[0])
                                     .arg(mAddr[1])
                                     .arg(mAddr[2])
                                     .arg(mAddr[3]);
    }
    bool isValid()
    {
        for(int i=0;i<4;i++)
        {
            if(mAddr[i] != 0)
            {
                return true;
            }
        }
        return false;
    }

    bool operator==(const VERSION &s)
    {
        for(int i=0;i<4;i++)
        {
            if(this->mAddr[i] != s.mAddr[i])
                return false;
        }
        return true;
    }
    bool operator!=(const VERSION &s)  { return !operator==(s); }

    VERSION & operator=(const VERSION &s)
    {
        for(int i=0;i<4;i++)
        {
            this->mAddr[i] = s.mAddr[i];
        }
        return *this;
    }
    VERSION & operator=(const QString &s)
    {
        QStringList tmpAddrList = s.split('.');
        if(tmpAddrList.count() != 4)
        {
            for(int i=0;i<4;i++)
                mAddr[i] = 0;
        }
        else
        {
            for(int i=0;i<4;i++)
            {
                bool ok = false;
                QString tmpString = tmpAddrList.at(i);
                mAddr[i] = tmpString.toInt(&ok);
                if(ok == false)
                {
                    mAddr[i] = 0;
                }
            }
        }
        return *this;
    }
};

class RANKS  //>@行列
{
public:
    RANKS()
    {
        mRow = mColumn = 0;
    }
    RANKS(QString pRanks)
    {
        *this = pRanks;
    }
    RANKS(quint32  pRow, quint32 pColumn)
    {
        mRow = pRow;
        mColumn = pColumn;
    }
public:
    quint32  mRow, mColumn;
public:
    QString toString()
    {
        return QString("%1x%2").arg(mRow).arg(mColumn);
    }
    bool operator==(const RANKS &s)
    {
        if(mRow != s.mRow)
            return false;
        if(mColumn != s.mColumn)
            return false;
        return true;
    }
    bool operator!=(const RANKS &s)  { return !operator==(s); }

    RANKS & operator=(const RANKS &s)
    {
        mRow = s.mRow;
        mColumn = s.mColumn;
        return *this;
    }
    RANKS & operator=(const QString &s)
    {
        QStringList tmpAddrList = s.split('x');
        if(tmpAddrList.count() != 2)
        {
            mRow = mColumn = 0;
        }
        else
        {
            bool ok = false;
            QString tmpString = tmpAddrList.at(0);
            mRow = tmpString.toInt(&ok);
            if(ok == false)
            {
                mRow = 0;
            }
            ok = false;
            tmpString = tmpAddrList.at(1);
            mColumn = tmpString.toInt(&ok);
            if(ok == false)
            {
                mColumn = 0;
            }
        }
        return *this;
    }
};

class RESOLUTION
{
public:
    RESOLUTION()
    {
        mWidth = mHeight = 0;
    }
    RESOLUTION(QString pResolution)
    {
        *this = pResolution;
    }
    RESOLUTION(quint32  pWidth, quint32 pHeight)
    {
        mWidth = pWidth;
        mHeight = pHeight;
    }
public:
    quint32  mWidth, mHeight;
public:
    QString toString()
    {
        return QString("%1x%2").arg(mWidth).arg(mHeight);
    }
    QSize toSize()
    {
        return QSize(mWidth, mHeight);
    }
    QSizeF toSizeF()
    {
        return QSizeF(mWidth, mHeight);
    }
    bool isValid()
    {
        if(mWidth <= 0)
            return false;
        if(mHeight <= 0)
            return false;
        return true;
    }
    bool operator==(const RESOLUTION &s)
    {
        if(mWidth != s.mWidth)
            return false;
        if(mHeight != s.mHeight)
            return false;
        return true;
    }
    bool operator!=(const RESOLUTION &s)  { return !operator==(s); }

    RESOLUTION & operator=(const RESOLUTION &s)
    {
        mWidth = s.mWidth;
        mHeight = s.mHeight;
        return *this;
    }
    RESOLUTION & operator=(const QString &s)
    {
        QStringList tmpAddrList = s.split('x');
        if(tmpAddrList.count() != 2)
        {
            mWidth = mHeight = 0;
        }
        else
        {
            bool ok = false;
            QString tmpString = tmpAddrList.at(0);
            mWidth = tmpString.toInt(&ok);
            if(ok == false)
            {
                mWidth = 0;
            }
            ok = false;
            tmpString = tmpAddrList.at(1);
            mHeight = tmpString.toInt(&ok);
            if(ok == false)
            {
                mHeight = 0;
            }
        }
        return *this;
    }
};

typedef enum{
    SLAVE_USBNET=0,
    SLAVE_STORAGE=1,
    SLAVE_INVALID
} USBSLAVE_TYPE;

inline USBSLAVE_TYPE GetUSBSlaveType(QString pSlaveName)
{
    if(pSlaveName.isEmpty())
        return SLAVE_INVALID;
    if(pSlaveName.compare("Net",Qt::CaseInsensitive) == 0)
        return SLAVE_USBNET;
    else if(pSlaveName.compare("Storage",Qt::CaseInsensitive) == 0)
        return SLAVE_STORAGE;
    return SLAVE_INVALID;
}

inline bool isNum(QString pString)
{
    bool ok=false;
    pString.toInt(&ok, 10);
    return ok;
}

/*****************Text*****************/

#define D_FILLCHAR      0
#define D_EMPTYCHAR     0x20

#define D_TEXT_MAXROW   4
#define D_TEXT_MAXCOL   24

#ifdef SUPPORT_WIDGET
typedef struct _TEXT_INFO{
    QPointer<QLabel>    mLabel;
    QString             mString;
} TEXT_INFO;
#else
typedef struct _TEXT_INFO{
    _TEXT_INFO()
    {
        mShowStart = 0;
        mShowEnd = 0;
        mString = 0;
    }
    QRectF              mRect;
    char               *mString;
    int                 mShowStart, mShowEnd;
} TEXT_INFO;
#endif

#define D_TEXT_MSG      0
#define D_TEXT_TIP      1

#define D_TEXT_MODE     2
#define D_TEXT_NUM      4

#ifdef WINDOWS
#ifndef QT5
typedef struct _MEMORYSTATUSEX {
  DWORD     dwLength;
  DWORD     dwMemoryLoad;
  DWORDLONG ullTotalPhys;
  DWORDLONG ullAvailPhys;
  DWORDLONG ullTotalPageFile;
  DWORDLONG ullAvailPageFile;
  DWORDLONG ullTotalVirtual;
  DWORDLONG ullAvailVirtual;
  DWORDLONG ullAvailExtendedVirtual;
} MEMORYSTATUSEX, *LPMEMORYSTATUSEX;
#endif
typedef BOOL (CALLBACK* GLOBALMEMORYSTATUSEX)(MEMORYSTATUSEX* pMemStatus);

// 时间格式转换
inline __int64 FileTimeToInt64(const FILETIME& time)
{
    ULARGE_INTEGER tt;
    tt.LowPart = time.dwLowDateTime;
    tt.HighPart = time.dwHighDateTime;
    return(tt.QuadPart);
}
#endif

typedef struct{
#ifdef WINDOWS
    DWORD mNetSpeed;
    DWORD mSendtotal;
    DWORD mRetotal;
#else
    quint16 mNetSpeed;
    quint16 mSendtotal;
    quint16 mRetotal;
#endif
}NET_FLOW; //>@网络流量

class SYSTEM_CMD : public QObject
{
    Q_OBJECT
public:
#ifdef WINDOWS
    //>@ example： GUID GUID_DEVINTERFACE_DISK ={ 0x53f56307L, 0xb6bf, 0x11d0, 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b };
    //>@            &GUID_DEVINTERFACE_DISK
    static bool EnumDeviceInterface(const GUID * InterfaceClassGUID)
    {
        char pcstr[128];
        HDEVINFO DeviceInfoSet;
        HDEVINFO NewDeviceInfoSet;
        SP_DEVICE_INTERFACE_DATA DeviceInterfaceData;
        PSP_DEVICE_INTERFACE_DETAIL_DATA lpDeviceInterfaceDetailData;
        DWORD dwBufferSize = 0;
        DWORD i;

        DeviceInfoSet = SetupDiCreateDeviceInfoList(NULL,NULL);
        if(DeviceInfoSet == INVALID_HANDLE_VALUE)
        {
            return 0;
        }
        NewDeviceInfoSet = SetupDiGetClassDevsEx(InterfaceClassGUID,NULL,NULL,DIGCF_DEVICEINTERFACE|DIGCF_PRESENT,DeviceInfoSet,NULL,NULL);
        if(NewDeviceInfoSet == INVALID_HANDLE_VALUE)
        {
            return 0;
        }
        DeviceInterfaceData.cbSize = sizeof(DeviceInterfaceData);
        for(i=0;;i++)
        {
            BOOL bResult = SetupDiEnumDeviceInterfaces(NewDeviceInfoSet,NULL,InterfaceClassGUID,i,&DeviceInterfaceData);
            if(!bResult)
            {
                if(bResult != NO_ERROR && GetLastError()!=ERROR_NO_MORE_ITEMS)
                {
                    return false;
                }
                break;
            }
            else
            {
                lpDeviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)HeapAlloc(GetProcessHeap(),0,sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA));
                lpDeviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
                dwBufferSize = lpDeviceInterfaceDetailData->cbSize;
                while(!SetupDiGetDeviceInterfaceDetail(NewDeviceInfoSet,&DeviceInterfaceData,lpDeviceInterfaceDetailData,dwBufferSize,&dwBufferSize,NULL))
                {
                    if(ERROR_INSUFFICIENT_BUFFER == GetLastError())
                    {
                        lpDeviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)HeapReAlloc(GetProcessHeap(),0,lpDeviceInterfaceDetailData,dwBufferSize);
                        lpDeviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
                    }
                }
                WideCharToMultiByte(CP_ACP,0,lpDeviceInterfaceDetailData->DevicePath,-1,pcstr,128,NULL,NULL);
                HeapFree(GetProcessHeap(),0,lpDeviceInterfaceDetailData);
            }
        }
        return true;
    }
#endif
    static bool ScanProcess(QString pProcess)
    {
#ifdef WINDOWS
        PROCESSENTRY32 pe32;
        // 在使用这个结构之前，先设置它的大小
        pe32.dwSize = sizeof(PROCESSENTRY32);
        // 给系统内的所有进程拍一个快照
        HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST | TH32CS_SNAPPROCESS | TH32CS_SNAPTHREAD | TH32CS_SNAPMODULE, 0);
        if(hProcessSnap == INVALID_HANDLE_VALUE)
        {
            return false;
        }
        // 遍历进程快照，轮流显示每个进程的信息
        BOOL bMore = Process32First(hProcessSnap, &pe32);
        char pcstr[128];
        QString m_processname = pProcess;
        QString procesname;
        while(bMore)
        {
            WideCharToMultiByte(CP_ACP,0,pe32.szExeFile,-1,pcstr,128,NULL,NULL);
            procesname = QString(pcstr);
            if(procesname.compare(m_processname,Qt::CaseInsensitive) == 0)
            {
                return true;
            }
            bMore = Process32Next(hProcessSnap, &pe32);
        }
        // 不要忘记清除掉snapshot对象
        CloseHandle(hProcessSnap);
        return false;
#else
        QString cmd = QString("ps aux | grep %1 | grep -v grep > scanProcess").arg(pProcess);
        system(cmd.toLatin1().data());
        QFile ProcessFile("scanProcess");
        if (!ProcessFile.exists())
        {
            return false;
        }
        char ProcessString[1024];
        if (!ProcessFile.open(QFile::ReadOnly))
        {
            ProcessFile.remove();
            return false;
        }
        ProcessFile.readLine(ProcessString, 1000);
        ProcessFile.close();
        ProcessFile.remove();
        QString ProcessInfo = QString(ProcessString);
        bool flag = (bool)(ProcessInfo.indexOf(pProcess, 0, Qt::CaseInsensitive) != -1);
        return flag;
#endif
    }
    //>@pProcess可以是某一个关键字，不一定是程序名,不要有空格
    static bool KillProcess(QString pProcess)
    {
#ifdef WINDOWS
        PROCESSENTRY32 pe32;
        // 在使用这个结构之前，先设置它的大小
        pe32.dwSize = sizeof(PROCESSENTRY32);
        // 给系统内的所有进程拍一个快照
        HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST | TH32CS_SNAPPROCESS | TH32CS_SNAPTHREAD | TH32CS_SNAPMODULE, 0);
        if(hProcessSnap == INVALID_HANDLE_VALUE)
            return false;
        // 遍历进程快照，轮流显示每个进程的信息
        BOOL bMore = Process32First(hProcessSnap, &pe32);
        char pcstr[128];
        QString m_processname = pProcess;
        QString procesname;
        while(bMore)
        {
            WideCharToMultiByte(CP_ACP,0,pe32.szExeFile,-1,pcstr,128,NULL,NULL);
            procesname = QString(pcstr);
            if(procesname.compare(m_processname,Qt::CaseInsensitive) == 0)
            {
                HANDLE hXplayer = OpenProcess(PROCESS_ALL_ACCESS, false, pe32.th32ProcessID);
                TerminateProcess(hXplayer, 0);
                CloseHandle(hXplayer);
            }
            bMore = Process32Next(hProcessSnap, &pe32);
        }
        // 不要忘记清除掉snapshot对象
        CloseHandle(hProcessSnap);
        return true;
#else
        QString cmd = QString("ps aux | grep %1 | grep -v grep > killProcess").arg(pProcess);
        system(cmd.toLatin1().data());
        QFile ProcessFile("killProcess");
        if (!ProcessFile.exists())
        {
            return false;
        }
        char ProcessString[1024];
        if (!ProcessFile.open(QFile::ReadOnly))
        {
            ProcessFile.remove();
            return false;
        }
        //>@提取所有相关进程的PID
        while(!(ProcessFile.atEnd()))
        {
            ProcessFile.readLine(ProcessString, 1000);
            QString ProcessInfo = QString(ProcessString);
            QString tmpPidString;
#ifdef LINUX
            tmpPidString = ProcessInfo.mid(0, 5);
#endif
#ifdef UBUNTU
            tmpPidString = ProcessInfo.mid(7, 7);
#endif
            if(tmpPidString.isEmpty())
                continue;
            bool ok = false;
            int tmpPid = tmpPidString.toInt(&ok, 10);
            if(ok)
            {
                QString tmpCmd = QString("kill -9 %1").arg(tmpPid);
                system(tmpCmd.toLatin1().data());
            }
        }
        ProcessFile.close();
        ProcessFile.remove();
        return true;
#endif
    }
    static bool SetNFC(bool pEnable)
    {
        if(pEnable)
        {
            QString tmpCmd = D_PCSCA_PATH;
#ifdef LINUX
            system(tmpCmd.toLatin1().data());
#endif
        }
        else
        {
            QString tmpCmd = QString("killall %1").arg(D_PCSCA_PATH);
#ifdef LINUX
            system(tmpCmd.toLatin1().data());
#endif
        }
        return true;
    }
    static bool SetSMS(bool pEnable, QString pConfFile=QString())
    {
        if(pEnable)
        {
            QFile tmpFile(pConfFile);
            if(tmpFile.exists() == false)
                return false;
#ifdef WINDOWS
            QString tmpString = QString("start /b %1 -f %2").arg(D_FFSERVER_PATH).arg(pConfFile);
            system(tmpString.toLatin1().data());
#else
            QString tmpString = QString("%1 -f %2 > %3ffserverInfo &").arg(D_FFSERVER_PATH).arg(pConfFile).arg(D_FSRCPATH);
            QProcess tmpProcess;
            tmpProcess.setProcessChannelMode(QProcess::MergedChannels);
            tmpProcess.startDetached(tmpString);
            QtSleep(100);
            QFile ffserverFile(QString("%1ffserverInfo").arg(D_FSRCPATH));
            if (!ffserverFile.exists())
            {
                return false;
            }
            char ffserverString[1024];
            if (!ffserverFile.open(QFile::ReadOnly))
            {
                ffserverFile.remove();
                return false;
            }
            ffserverFile.readLine(ffserverString, 1000);
            ffserverFile.close();
            QString ffserverinfo = QString(ffserverString);
            if(ffserverinfo.indexOf(QString("FFserver started"), Qt::CaseInsensitive) == -1)
            {
                ffserverFile.remove();
                return false;
            }
            ffserverFile.remove();
#endif
        }
        else
        {
#ifdef WINDOWS
            KillProcess(D_FFSERVER_PATH);
#else
            QString tmpCmd = QString("killall %1").arg(D_FFSERVER_PATH);
            system(tmpCmd.toLatin1().data());
#endif
        }
        return true;
    }
    static bool SetMonitor(bool pEnable, QString pCameraDev=QString(), int pFrameRate=10, RESOLUTION pSize=RESOLUTION())  //>@设置摄像头连续采集
    {
        if(pEnable)
        {
            if(pCameraDev.isEmpty() || pSize.isValid()==false)
            {
                return false;
            }
#ifdef WINDOWS
            QString tmpCmd;
            if(isNum(pCameraDev))
            {
                tmpCmd = QString("%1 -f vfwcap -s %2 -r %3 -i %4 http://127.0.0.1:8090/feed1.ffm")
                        .arg(D_FFMPEG_PATH)
                        .arg(pSize.toString())
                        .arg(pFrameRate)
                        .arg(pCameraDev);
            }
#else
            QFile tmpFile(pCameraDev);
            if(tmpFile.exists() == false)
            {
                return false;
            }
            QString tmpCmd;
            if(pCameraDev.contains("/dev/video", Qt::CaseInsensitive))
            {
                tmpCmd = QString("%1 -f video4linux2 -s %2 -r %3 -i %4 http://127.0.0.1:8090/feed1.ffm < /dev/null > /dev/null &")
                        .arg(D_FFMPEG_PATH)
                        .arg(pSize.toString())
                        .arg(pFrameRate)
                        .arg(pCameraDev);
            }
#endif
            else
            {
#ifdef WINDOWS
                tmpCmd = QString("start /b %1 -s %2 -r %3 -i %4 http://127.0.0.1:8090/feed1.ffm")
                        .arg(D_FFMPEG_PATH)
                        .arg(pSize.toString())
                        .arg(pFrameRate)
                        .arg(pCameraDev);
#else
                tmpCmd = QString("%1 -s %2 -r %3 -i %4 http://127.0.0.1:8090/feed1.ffm < /dev/null > /dev/null &")
                        .arg(D_FFMPEG_PATH)
                        .arg(pSize.toString())
                        .arg(pFrameRate)
                        .arg(pCameraDev);
#endif
            }
            SetMonitor(false);
            QProcess tmpProcess;
            tmpProcess.setProcessChannelMode(QProcess::MergedChannels);
            tmpProcess.startDetached(tmpCmd);
        }
        else
        {
#ifdef WINDOWS
            KillProcess(D_FFMPEG_PATH);
#else
            QString tmpCmd = QString("killall %1").arg(D_FFMPEG_PATH);
            system(tmpCmd.toLatin1().data());
#endif
        }
        return true;
    }
    static QPixmap GetIdentifyPix()  //>@设置摄像头单次采集用于后续的识别操作
    {

        return QPixmap();
    }
    static bool SetIP(QString pNetName, ENET_ADDR pIP)
    {
        if(pNetName.isEmpty() || pIP.isValid() == false)
        {
            return false;
        }
        QString tmpInfo;
#ifdef LINUX
        QString cmd = QString("ifconfig %1 %2 up").arg(pNetName).arg(pIP.toQString());
        QProcess tmpProcess;
        tmpProcess.setProcessChannelMode(QProcess::MergedChannels);
        tmpProcess.start(cmd);
        tmpProcess.waitForFinished();
        tmpInfo = tmpProcess.readAllStandardOutput();
        tmpProcess.kill();
#endif
        return !(tmpInfo.contains(D_NETERROR,Qt::CaseInsensitive));
    }
    static bool SetNetmask(QString pNetName, ENET_ADDR pNetmask)
    {
        QString tmpInfo;
#ifdef LINUX
        QString cmd = QString("ifconfig %1 netmask %2").arg(pNetName).arg(pNetmask.toQString());
        QProcess tmpProcess;
        tmpProcess.setProcessChannelMode(QProcess::MergedChannels);
        tmpProcess.start(cmd);
        tmpProcess.waitForFinished();
        tmpInfo = tmpProcess.readAllStandardOutput();
        tmpProcess.kill();
#endif
        return !(tmpInfo.contains(D_NETERROR,Qt::CaseInsensitive));
    }
    static bool SetGateway(ENET_ADDR pGateway)
    {
        if(pGateway.isValid() == false)
            return false;
        QString tmpInfo;
#ifdef LINUX
        QString cmd = QString("route add default gw ") + pGateway.toQString();
        QProcess tmpProcess;
        tmpProcess.setProcessChannelMode(QProcess::MergedChannels);
        tmpProcess.start(cmd);
        tmpProcess.waitForFinished();
        tmpInfo = tmpProcess.readAllStandardOutput();
        tmpProcess.kill();
#endif
        return !(tmpInfo.contains(D_GATEWAYERROR,Qt::CaseInsensitive));
    }
    static bool SetDNS(ENET_ADDR pDNS)
    {
        if(pDNS.isValid() == false)
            return false;
#ifdef LINUX
        QString DNSString;
        QStringList strlis;
        DNSString = QString("nameserver ") + pDNS.toQString();
        QFile DNSconffile(D_DNSCONF);
        QTextStream FileStream(&DNSconffile);
        if(!DNSconffile.open(QFile::ReadOnly))
        {
            return false;
        }
        while(!FileStream.atEnd())
        {
            QString lineNDS=FileStream.readLine();
            strlis<<lineNDS;
        }
        DNSconffile.close();

        QFile tmpconffile(D_DNSCONF);
        QTextStream tmpFileStream(&tmpconffile);
        if(!tmpconffile.open(QFile::WriteOnly))
        {
            return false;
        }
        int flag = 0;
        for (int i = 0; i < strlis.size(); i++)
        {
            if (strlis.at(i).compare(DNSString) == 0)
            {
                flag = 1;
            }
            tmpFileStream<<strlis.at(i)<<"\n";
        }
        if(flag == 0)
            tmpFileStream<<DNSString<<"\n";
        tmpconffile.close();
#endif
        return true;
    }
    static bool SetUSBWIFI(bool pEnable)
    {
#ifdef LINUX
        QProcess tmpProcess;
        if(pEnable == true)
        {
            tmpProcess.startDetached("wifi-start &");
        }
        else
        {
            tmpProcess.startDetached("wifi-stop &");
        }
        tmpProcess.waitForFinished();
        tmpProcess.kill();
#endif
        return true;
    }
    static bool SetWIFISSID(QString pSSID)
    {
        if(pSSID.isEmpty())
            return false;
        QString tmpOldFilePath = D_FSRCPATH + D_SYSTEMDIR + D_MIDDLEWAREDIR + QString("WIFI/RT2870AP.dat");
        QString tmpNewFilePath = QString("/etc/Wireless/RT2870AP/RT2870AP.dat");
        QFile tmpFile(tmpOldFilePath);
        if(!tmpFile.open(QIODevice::ReadOnly))
            return false;
        QFile tmpNewFile(tmpNewFilePath);
        if(!tmpNewFile.open(QIODevice::WriteOnly))
            return false;
        while(!tmpFile.atEnd())
        {
            QByteArray tmpArray = tmpFile.readLine();
            if(tmpArray.startsWith("SSID="))
            {
                IDE_TRACE_STR(pSSID);
                tmpArray = QByteArray("SSID=")+pSSID.toLatin1()+QByteArray("\n");
            }
            tmpNewFile.write(tmpArray);
        }
        tmpNewFile.close();
        tmpFile.close();
        return true;
    }
    static bool AddDefaultNet(QString pNetName)
    {
        if(pNetName.isEmpty())
            return false;
#ifdef LINUX
        QProcess tmpProcess;
        QString tmpString = QString("route add default dev %1").arg(pNetName);
        tmpProcess.start(tmpString);
        tmpProcess.waitForFinished();
        tmpProcess.kill();
#endif
        return true;
    }
    static bool SetNetCard(bool pEnable, QString pNetCardName)
    {
#ifdef LINUX
        if(pEnable == true)
        {
            QString cmd = QString("ifconfig %1 up").arg(pNetCardName);
            system(cmd.toLatin1().data());
        }
        else
        {
            QString cmd = QString("ifconfig %1 down").arg(pNetCardName);
            system(cmd.toLatin1().data());
        }
#endif
        return true;
    }
    static bool SetGPRS(bool pEnable)
    {
#ifdef LINUX
        if(pEnable == true)
        {
            QProcess tmpProcess;
            tmpProcess.start("pppd call wcdma &");
            tmpProcess.waitForFinished();
            tmpProcess.start(QString("route add default dev ppp0"));
            tmpProcess.waitForFinished();
            tmpProcess.kill();
            return true;
        }
        else
        {
            system("killall pppd");
        }
#endif
        return true;
    }
    //>@pConfFile不要有空格
    static bool SetDHCP(bool pDHCP, QString pConfFile)
    {
#ifdef LINUX
        if(pDHCP == true)
        {
            QFile tmpFile(pConfFile);
            if(tmpFile.exists() == false)
                return false;
            QProcess tmpProcess;
            QString cmd = QString("udhcpd %1 &").arg(pConfFile);
            tmpProcess.startDetached(cmd);
            tmpProcess.waitForFinished();
            tmpProcess.kill();
        }
        else
        {
            if(pConfFile.isEmpty())
                return false;
            KillProcess(pConfFile);
        }
#endif
        return true;
    }
    static bool SetFTP(bool pFTP)
    {
        return pFTP;
    }
    static bool SetTelnet(bool pTelnet)
    {
        return pTelnet;
    }
    static bool SetSSH(bool pSSH)
    {
#ifdef LINUX
        QProcess tmpProcess;
        if(pSSH == true)
        {
            tmpProcess.startDetached("/etc/init.d/dropbear start &");
        }
        else
        {
            tmpProcess.startDetached("/etc/init.d/dropbear stop &");
        }
        tmpProcess.waitForFinished();
        tmpProcess.kill();
#endif
        return true;
    }
#ifdef SURRPORT_NETMANAGER
    static QList<NET_FLOW> GetNetFlowInfo()
    {
        QList<NET_FLOW> tmpList;
#ifdef WINDOWS
        MIB_IFTABLE *pIfTable = NULL;   //网络信息表
        ULONG  dwSize = 0;              //接口个数
        DWORD dwRet = GetIfTable( pIfTable, &dwSize, TRUE);   //dwsize获得接口个数
        if(dwRet  ==  ERROR_INSUFFICIENT_BUFFER)
        {
            pIfTable = (MIB_IFTABLE  *)new  char[dwSize];
            if(pIfTable != NULL)
            {
                dwRet = GetIfTable( pIfTable, &dwSize, TRUE); //获得真实接口信息
                if( dwRet == NO_ERROR )
                {
                    IDE_DEBUG(QString("dwNumEntries = %1").arg(pIfTable->dwNumEntries));
                    for(int i=0;i<pIfTable->dwNumEntries;i++)
                    {
                        if((pIfTable->table[i]).dwType == 6 && (pIfTable->table[i]).dwInOctets > 0 && (pIfTable->table[i]).dwOutOctets > 0)
                        {
                            //只保留dwtype为6是网卡信息，接受发送了数据。默认服务器只能用一个网卡，
                            IDE_DEBUG(QString("table[%1].dwIndex   =   %2").arg(i).arg(pIfTable->table[i].dwIndex));
                            IDE_DEBUG(QString("    dwType     =   %1").arg(pIfTable->table[i].dwType));
                            IDE_DEBUG(QString("    dwSpeed   =   %1").arg(pIfTable->table[i].dwSpeed));
                            //added (pIfTable->table[i]).dwInOctets=0;
                            IDE_DEBUG(QString("/t received data is %1").arg(pIfTable->table[i].dwInOctets)); //接口接受总数据
                            IDE_DEBUG(QString("/t send  data is %1").arg(pIfTable->table[i].dwOutOctets));   // 接口发送总数据

                            NET_FLOW tmpNetFlow;
                            tmpNetFlow.mNetSpeed=(pIfTable->table[i]).dwSpeed;      //获得网卡数据类型，10M or100M
                            tmpNetFlow.mRetotal=(pIfTable->table[i]).dwInOctets;   //网卡接受到的总的数据字节
                            tmpNetFlow.mSendtotal=(pIfTable->table[i]).dwOutOctets;   //网卡发送的
                            tmpList.append(tmpNetFlow);
                        }
                    }
                }
            }
        }
        return tmpList;
#else
        return tmpList;
#endif
    }
#endif
    static bool SetUsbSlave(bool pEnable, USBSLAVE_TYPE tmpType, QString pPara=QString())
    {
#ifdef LINUX
        switch(tmpType)
        {
            case SLAVE_USBNET:       //>@pPara为IP地址
            {
                if(pEnable)
                {
                    QProcess tmpProcess;
                    tmpProcess.startDetached("rmmod g_file_storage");
                    tmpProcess.waitForFinished();
                    tmpProcess.startDetached("modprobe g_ether");
                    tmpProcess.waitForFinished();
                    tmpProcess.kill();
                }
                else
                {
                    system("rmmod g_ether");
                }
                break;
            }
            case SLAVE_STORAGE:       //>@pPara为待挂载的磁盘驱动地址, 可以为：/dev/mmcblk0p1
            {
                if(pEnable)
                {
                    QProcess tmpProcess;
                    tmpProcess.startDetached("rmmod g_ether");
                    tmpProcess.waitForFinished();
                    tmpProcess.startDetached("rmmod g_file_storage");
                    tmpProcess.waitForFinished();
                    tmpProcess.startDetached(QString("modprobe g_file_storage file=%1 stall=0 removable=1").arg(pPara));
                    tmpProcess.waitForFinished();
                    tmpProcess.kill();
                }
                else
                {
                    system("rmmod g_file_storage");
                }
                break;
            }
            default:
                return false;
        }
#endif
        return true;
    }
    static QString CreateSDPFile(QString pIP, QString pVideoPort, QString pAudioPort)
    {
        QString tmpFile = D_GrabScreenPath + QString("%1_%2.sdp").arg(pVideoPort).arg(pAudioPort);
        QFile file1(tmpFile);
        if(file1.exists())
            file1.remove();
        if(!file1.open(QFile::WriteOnly))
        {
            return QString();
        }
        QTextStream newInputStream(&file1);
        newInputStream<<"v=0"<<"\n";
        newInputStream<<"o=- 0 0 IN IP4 127.0.0.1"<<"\n";
        newInputStream<<"s=No Name"<<"\n";
        newInputStream<<"t=0 0"<<"\n";
        newInputStream<<"a=tool:libavformat 55.4.101"<<"\n";
        if(!pAudioPort.isEmpty())
        {
            newInputStream<<"m=audio "<< pAudioPort << " RTP/AVP 97" << "\n";
            newInputStream<<"c=IN IP4 "<< pIP << "\n";
            newInputStream<<"b=AS:128"<<"\n";
            newInputStream<<"a=rtpmap:97 MPEG4-GENERIC/44100/2"<<"\n";
            newInputStream<<"a=fmtp:97 profile-level-id=1;mode=AAC-hbr;sizelength=13;indexlength=3;indexdeltalength=3; config=121056E500"<<"\n";
        }
        if(!pAudioPort.isEmpty())
        {
            newInputStream<<"m=video " << pVideoPort <<" RTP/AVP 96"<<"\n";
            newInputStream<<"c=IN IP4 "<< pIP << "\n";
            newInputStream<<"b=AS:200"<<"\n";
            newInputStream<<"a=rtpmap:96 MP4V-ES/90000"<<"\n";
            newInputStream<<"a=fmtp:96 profile-level-id=1"<<"\n";
        }
        file1.close();
        return tmpFile;
    }
    static bool CreateDHCPConf(QString pConfFile, QString pNetName, QString pIP,
                               QString pSubmask, QString pGateway, QString pDNS)
    {
        QFile file1(pConfFile);
        if(file1.exists())
            file1.remove();
        if(!file1.open(QFile::WriteOnly))
        {
            return false;
        }
        QTextStream newInputStream(&file1);
        //>@添加网卡
        newInputStream<<"interface  "<<pNetName<<"  #"<<"\n";
        //>@添加IP范围
        ENET_ADDR tmpIP = pIP;
        tmpIP.mAddr[3] = 200;
        newInputStream<<"start  "<<tmpIP.toQString()<<"  #"<<"\n";
        tmpIP.mAddr[3] = 240;
        newInputStream<<"end  "<<tmpIP.toQString()<<"  #"<<"\n";
        //>@添加Submask
        newInputStream<<"option subnet  "<<pSubmask<<"  #"<<"\n";
        //>@添加Gateway
        newInputStream<<"opt router  "<<pGateway<<"  #"<<"\n";
        //>@添加DNS
        newInputStream<<"opt dns  "<<pDNS<<"  #"<<"\n";
        newInputStream<<"option		lease		864000"<<"\n";
        file1.close();
        return true;
    }
    static bool ChangeDHCPConf(QString pConfFile, QString str,QString content)
    {
        QFile file(pConfFile);
        if(!file.exists() || !file.open(QFile::ReadOnly))
        {
            return false;
        }
        QTextStream userFileStream(&file);
        QStringList strlis;
        while(!userFileStream.atEnd())
        {
            QString lineIni=userFileStream.readLine();
            strlis<<lineIni;
        }
        file.close();
        file.remove();
        QFile file1(pConfFile);
        if(!file1.open(QFile::WriteOnly))
        {
            return false;
        }
        QTextStream newInputStream(&file1);
        for (int i = 0;i < strlis.size();i++)
        {
            QString tmpstrlis = strlis.at(i);
            if(tmpstrlis.startsWith('#'))
                continue;
            if(tmpstrlis.startsWith(str))
            {
                newInputStream<<str<<" "<<content<<"\n";
            }
            else
            {
                newInputStream<<strlis.at(i)<<"\n";
            }
        }
        file1.close();
        return true;
    }
    static bool ChangeWifiConf(QString str,QString content)
    {
        QFile file(D_WIFICONF);
        if(!file.open(QFile::ReadWrite))
        {
            return false;
        }
        QTextStream userFileStream(&file);
        QStringList strlis;
        while(!userFileStream.atEnd())
        {
            QString lineIni=userFileStream.readLine();
            strlis<<lineIni;
        }
        file.close();
        file.remove();
        QFile file1(D_WIFICONF);
        if(!file1.open(QFile::WriteOnly))
        {
            return false;
        }
        QTextStream newInputStream(&file1);
        for (int i = 0;i < strlis.size();i++)
        {
            QString tmpstrlis = strlis.at(i);
            QStringList tmplist = tmpstrlis.split("=");
            if(tmplist.count() != 2)
            {
                continue;
            }
            if (tmplist.at(0).compare(str) == 0)
            {
                newInputStream<<str<<"="<<content<<"\n";
            }
            else
            {
                newInputStream<<strlis.at(i)<<"\n";
            }
        }
        file1.close();
        return true;
    }
    static bool SetSSID(QString pSSID)
    {
        return ChangeWifiConf("SSID", pSSID);
    }
    static bool SetPassWD(QString pPassWD)
    {
        return ChangeWifiConf("WPAPSK", pPassWD);
    }
    static bool SetLuminance(int pVaule)//>@pVaule 从0~100
    {
#ifdef LINUX
        QString tmpCmdString = QString("echo %1 > /sys/devices/platform/pwm-backlight/backlight/pwm-backligh/brightness").arg(pVaule);
        system(tmpCmdString.toLatin1().data());
#endif
        return true;
    }
    static bool SetLED(int id, bool light)
    {
#ifdef LINUX
        QString tmpString = QString("echo %1 > /sys/class/leds/led%2/brightness").arg(int(light)).arg(id);
        system(tmpString.toLatin1().data());
#endif
        return true;
    }
    static bool SetVolume(int pVaule) //>@0~127
    {
#ifdef LINUX
        QString tmpString = QString("amixer set PCM %1").arg(pVaule);
        system(tmpString.toLatin1().data());
#endif
        return true;
    }
    static bool GetCpuLoad(double &pTotal, double &pUsage, unsigned long pProcID = 0)
    {
#ifdef WINDOWS
        //上一次的时间
        qint64 last_time_ = 0;
        qint64 last_system_time_ = 0;
        FILETIME now;
        FILETIME creation_time;
        FILETIME exit_time;
        FILETIME kernel_time;
        FILETIME user_time;
        __int64 system_time;
        __int64 time;
        //>@cpu数量
        SYSTEM_INFO info;
        GetSystemInfo(&info);
        int processor_count_ =  (int)info.dwNumberOfProcessors;
        //>@
        GetSystemTimeAsFileTime(&now);
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION/*PROCESS_ALL_ACCESS*/, false, pProcID);
        if (!hProcess)
            return false;
        if (!GetProcessTimes(hProcess, &creation_time, &exit_time, &kernel_time, &user_time))
            return false;
        system_time = (FileTimeToInt64(kernel_time) + FileTimeToInt64(user_time)) / processor_count_;  //CPU使用时间
        time = FileTimeToInt64(now);		//现在的时间

        last_system_time_ = system_time;
        last_time_ = time;
        CloseHandle( hProcess );

        QtSleep(1000);

        hProcess = OpenProcess(PROCESS_QUERY_INFORMATION/*PROCESS_ALL_ACCESS*/, false, pProcID);
        if (!hProcess)
            return false;
        if (!GetProcessTimes(hProcess, &creation_time, &exit_time, &kernel_time, &user_time))
            return false;
        GetSystemTimeAsFileTime(&now);
        system_time = (FileTimeToInt64(kernel_time) + FileTimeToInt64(user_time)) / processor_count_;  //CPU使用时间
        time = FileTimeToInt64(now);		//现在的时间

        CloseHandle( hProcess );

        pUsage = ((double)(system_time - last_system_time_) / (double)(time - last_time_)) * 100;
        return true;
#else
        char tmpString[100];
        QFile tmpFile("/proc/stat");
        if(tmpFile.open(QFile::ReadOnly) == false)
            return false;
        tmpFile.readLine(tmpString,100);
        QString CpuInfo = QString(tmpString);
        QStringList CpuInfoList = CpuInfo.split(' ');
        double Total_1 = 0;
        for(int i=0;i<CpuInfoList.count();i++)
        {
            Total_1 = Total_1 + CpuInfoList.at(i).toInt();
        }
        double SYS_IDLE_1 = CpuInfoList.at(4).toInt();
        system("sleep 1");
        tmpFile.readLine(tmpString,100);
        CpuInfo = QString(tmpString);
        CpuInfoList = CpuInfo.split(" ");
        double Total_2 = 0;
        for(int i=0;i<CpuInfoList.count();i++)
        {
            Total_2 = Total_2 + CpuInfoList.at(i).toInt();
        }
        double SYS_IDLE_2 = CpuInfoList.at(4).toInt();
        pUsage = SYS_IDLE_2 - SYS_IDLE_1;
        pTotal =  Total_1 - Total_2;
#endif
        return true;
    }
    static bool GetMemSpace(double &pTotal, double &pUsage, double &pFree)
    {
#ifdef WINDOWS
        GLOBALMEMORYSTATUSEX pGlobalMemoryStatusEx;
        pGlobalMemoryStatusEx = (GLOBALMEMORYSTATUSEX)GetProcAddress(GetModuleHandleA(("Kernel32.dll")), ("GlobalMemoryStatusEx"));
        if (pGlobalMemoryStatusEx)
        {
            MEMORYSTATUSEX MemStatEx;
            MemStatEx.dwLength = sizeof(MEMORYSTATUSEX);
            pGlobalMemoryStatusEx(&MemStatEx);
            pTotal = MemStatEx.ullTotalPhys;
            pFree = MemStatEx.ullAvailPhys;
            pUsage = pTotal - pFree;
            return true;
        }
        return false;
#else
        bool ok1 = false, ok2 = false;
        system("free | grep 'buffers/cache' | awk '{print $3}'>tmpFile");
        system("free | grep 'buffers/cache' | awk '{print $4}'>>tmpFile");
        QFile tmpFile("tmpFile");
        if(!tmpFile.exists())
            return false;
        char tmpString[10];
        if(!tmpFile.open(QFile::ReadOnly))
        {
            tmpFile.remove();
            return false;
        }
        tmpFile.readLine(tmpString,10);
        QString Memory_Used = QString(tmpString);
        double tmpUsage = Memory_Used.toInt(&ok1, 10);
        if(ok1)
            pUsage = tmpUsage;
        tmpFile.readLine(tmpString,10);
        QString Memory_Free = QString(tmpString);
        double tmpFree = Memory_Free.toInt(&ok2, 10);
        if(ok2)
            pFree = tmpFree;
        if(ok1 && ok2)
            pTotal = pUsage +  pFree;
        tmpFile.remove();
        return (ok1 && ok2);
#endif
    }
    static bool GetDiskSpace(QString pDiskPath, double &pTotal, double &pUsage, double &pFree, double &pPercent)
    {
#ifdef WINDOWS
        ULARGE_INTEGER   uiFreeBytesAvailableToCaller;
        ULARGE_INTEGER   uiTotalNumberOfBytes;
        ULARGE_INTEGER   uiTotalNumberOfFreeBytes;
        TCHAR* tmpPath;
#ifdef UNICODE
        tmpPath = (TCHAR*)(pDiskPath.utf16());
#else
        tmpPath = (TCHAR*)(pDiskPath.toLocal8Bit());
#endif
        if(GetDiskFreeSpaceEx(tmpPath,
                              &uiFreeBytesAvailableToCaller,
                              &uiTotalNumberOfBytes,
                              &uiTotalNumberOfFreeBytes))
        {
            pTotal = uiTotalNumberOfBytes.QuadPart;
            pFree = uiFreeBytesAvailableToCaller.QuadPart;
            pUsage = pTotal - pFree;
            return true;
        }
        return false;
#else
        bool ok1 = false, ok2 = false, ok3 = false;
        QString tmpCmdString = QString("df | grep '%1' | awk '{print $3}'>DiskInfo").arg(pDiskPath);
        system(tmpCmdString.toLatin1().data());
        tmpCmdString = QString("df | grep '%1' | awk '{print $4}'>>DiskInfo").arg(pDiskPath);
        system(tmpCmdString.toLatin1().data());
        tmpCmdString = QString("df | grep '%1' | awk '{print $5}'>>DiskInfo").arg(pDiskPath);
        system(tmpCmdString.toLatin1().data());
        QFile tmpFile("DiskInfo");
        if(!tmpFile.exists())
            return false;
        char  tmpString[10];
        if(!tmpFile.open(QFile::ReadOnly))
        {
            tmpFile.remove();
            return false;
        }
        tmpFile.readLine(tmpString,10);
        QString Disk_Used = QString(tmpString);
        double tmpUsage = Disk_Used.toInt(&ok1, 10);
        if(ok1)
            pUsage = tmpUsage;
        tmpFile.readLine(tmpString,10);
        QString Disk_Free = QString(tmpString);
        double tmpFree = Disk_Free.toInt(&ok2, 10);
        if(ok2)
            pFree = tmpFree;
        if(ok1 && ok2)
            pTotal = pUsage +  pFree;
        tmpFile.readLine(tmpString,10);
        QString Percent = QString(tmpString);
        if(Percent.endsWith("%\n"))
            Percent = Percent.left(Percent.lastIndexOf("%"));
        double tmpPercent = Percent.toInt(&ok3, 10);
        if(ok3)
            pPercent = tmpPercent;
        tmpFile.remove();
        return (ok1 && ok2 && ok3);
#endif
    }
};

typedef enum {
    EVENT_INVALID = 0,
    EVENT_FLOOR,
    EVENT_ARROW,
    EVENT_FUNCTION,
    EVENT_AUDIO,
    EVENT_TIMER,         //>@定时事件
    EVENT_ALARM        //>@闹钟事件
} MAP_EVENT;

inline MAP_EVENT GetMapEventType(QString pKey)
{
    if(!pKey.compare("Floor", Qt::CaseInsensitive))
        return EVENT_FLOOR;
    else if(!pKey.compare("Arrow", Qt::CaseInsensitive))
        return EVENT_ARROW;
    else if(!pKey.compare("Function", Qt::CaseInsensitive))
        return EVENT_FUNCTION;
    else if(!pKey.compare("Audio", Qt::CaseInsensitive))
        return EVENT_AUDIO;
    else if(!pKey.compare("Timer", Qt::CaseInsensitive))
        return EVENT_TIMER;
    else if(!pKey.compare("Alarm", Qt::CaseInsensitive))
        return EVENT_ALARM;
    return EVENT_INVALID;
}

inline QString GetMapEventTypeString(MAP_EVENT pKey)
{
    if(pKey == EVENT_FLOOR)
        return "Floor";
    else if(pKey == EVENT_ARROW)
        return "Arrow";
    else if(pKey == EVENT_FUNCTION)
        return "Function";
    else if(pKey == EVENT_AUDIO)
        return "Audio";
    else if(pKey == EVENT_TIMER)
        return "Timer";
    else if(pKey == EVENT_ALARM)
        return "Alarm";
    return QString();
}

//>@ CLOCK

enum _CLOCKLABEL{
    ClockMon = 10,
    ClockTue = 11,
    ClockWed = 12,
    ClockThu = 13,
    ClockFri = 14,
    ClockSat = 15,
    ClockSun = 16,
    ClockColon = 17,
    ClockMinus = 18,
    ClockHourPointer=19,
    ClockMinutePointer=20,
    ClockSecondPointer=21,
    ClockDot=22
};

typedef enum{
    Element_HourPointer = 0,
    Element_MinutePointer = 1,
    Element_SecondPointer = 2,
    Element_Dot = 3,
    Element_Year0 = 4,
    Element_Year1 = 5,
    Element_Year2 = 6,
    Element_Year3 = 7,
    Element_Minus0 = 8,//>@-
    Element_Month0 = 9,
    Element_Month1 = 10,
    Element_Minus1 = 11,//>@-
    Element_Day0 = 12,
    Element_Day1 = 13,
    Element_Week = 14,
    Element_Hour0 = 15,
    Element_Hour1 = 16,
    Element_Colon0 = 17,//>@:
    Element_Minute0 = 18,
    Element_Minute1 = 19,
    Element_Colon1 = 20,//>@:
    Element_Second0 = 21,
    Element_Second1 = 22,
    Element_Date,
    Element_Time
}CLOCKE_ELEMENT;

typedef enum{
    ANALOG = 0,
    DIGITAL = 1
}CLOCK_TYPE;

typedef struct{
    QRectF       mRect;
    bool        mAnimateFlag;
    RC_POINTER  mRcPointer;
}CLOCK_ELEMENTINFO;

typedef enum{
    PTN_V_YMDHMS = 0,  //>@垂直分布  YY-MM-SS MM-SS-YY SS-MM-YY 三种时间格式
    PTN_V_DMYHMS,
    PTN_V_MDYHMS,
    PTN_V_YMDWHMS,
    PTN_V_DMYWHMS,
    PTN_V_MDYWHMS,
    PTN_H_YMDHMS,  //>@水平分布
    PTN_H_MDYHM,
    PTN_H_DMYHMS,
    PTN_H_MDYHMS,
    PTN_H_YMDWHMS,
    PTN_H_DMYWHMS,
    PTN_H_MDYWHMS,
    PTN_MOJI_MDWHM  //>@仿墨迹时间区域
}CLOCK_PATTERN;  //>@时钟样式

class CLOCK_INFO
{
public:
    CLOCK_INFO()
    {
        mYear = 0xff;
        mMonth = 0xff;
        mDay = 0xff;
        mHour = 0xff;
        mMinute = 0xff;
        mSecond = 0xff;
        mWeekDay = 0xff;
        mErrorFlag = true;
    }
    CLOCK_INFO(quint8 pYear, quint8 pMonth, quint8 pDay, quint8 pHour, quint8 pMinute, quint8 pSecond)
    {
        fromChar(pYear, pMonth, pDay, pHour, pMinute, pSecond);
    }
    CLOCK_INFO(quint32 &pTime)
    {
        fromInt(pTime);
    }
    CLOCK_INFO(int pTime)
    {
        fromInt((quint32 &)pTime);
    }
    CLOCK_INFO(QDateTime pTime)
    {
        int year = pTime.date().year();
        if(year>=2000)
            year-=2000;
        else if(year>=1000)
            year-=1000;
        fromChar(year, pTime.date().month(), pTime.date().day(), pTime.time().hour(), pTime.time().minute(), pTime.time().second());
    }
public:
    quint8  mYear;
    quint8  mMonth;
    quint8  mDay;
    quint8  mWeekDay;
    quint8  mHour;
    quint8  mMinute;
    quint8  mSecond;

    bool    mErrorFlag;
public:
    bool isValid()
    {
        return !mErrorFlag;
    }

    int dayOfWeek()
    {
        QDate tmpData(mYear, mMonth, mDay);
        return tmpData.dayOfWeek();
    }

    bool operator==(const CLOCK_INFO &s)
    {
        if(this->mYear != s.mYear)
            return false;
        if(this->mMonth != s.mMonth)
            return false;
        if(this->mDay != s.mDay)
            return false;
        if(this->mHour != s.mHour)
            return false;
        if(this->mMinute != s.mMinute)
            return false;
        if(this->mSecond != s.mSecond)
            return false;
        return true;
    }
    bool operator!=(const CLOCK_INFO &s)  { return !operator==(s); }

    bool toInt(quint32 &pTime)
    {
         if(isValid())
            pTime = (mYear << 26) + (mMonth << 22) + (mDay << 17) + (mHour << 12) + (mMinute << 6) + mSecond;
         return !mErrorFlag;
    }
    bool fromClock(CLOCK_INFO &s)
    {
        quint8 tmpYear = s.mYear;
        quint8 tmpMonth = s.mMonth;
        quint8 tmpDay = s.mDay;
        quint8 tmpHour = s.mHour;
        quint8 tmpMinute = s.mMinute;
        quint8 tmpSecond = s.mSecond;
        if(tmpYear<0 || tmpYear>63)
            goto exit;
        mYear = tmpYear;
        if(tmpMonth<1 || tmpMonth>12)
            goto exit;
        mMonth = tmpMonth;
        if(tmpDay<1 || tmpDay>31)
            goto exit;
        mDay = tmpDay;
        if(tmpHour<0 || tmpHour>23)
            goto exit;
        mHour = tmpHour;
        if(tmpMinute<0 || tmpMinute>59)
            goto exit;
        mMinute = tmpMinute;
        if(tmpSecond<0 || tmpSecond>59)
            goto exit;
        mSecond = tmpSecond;
        mWeekDay = dayOfWeek();
        mErrorFlag = false;
        return true;
    exit:
        CLOCK_INFO();
        return false;
    }
    bool fromInt(quint32 &s)
    {
        quint8 tmpYear = (s>>26) & 0x0000003f;
        quint8 tmpMonth = (s>>22) & 0x0000000f; //>4
        quint8 tmpDay = (s>>17) & 0x0000001f;   //>5
        quint8 tmpHour = (s>>12) & 0x0000001f;  //>5
        quint8 tmpMinute = (s>>6) & 0x0000003f; //>6
        quint8 tmpSecond = s & 0x0000003f;      //>6
        if(tmpYear<0 || tmpYear>63)
            goto exit;
        mYear = tmpYear;
        if(tmpMonth<1 || tmpMonth>12)
            goto exit;
        mMonth = tmpMonth;
        if(tmpDay<1 || tmpDay>31)
            goto exit;
        mDay = tmpDay;
        if(tmpHour<0 || tmpHour>23)
            goto exit;
        mHour = tmpHour;
        if(tmpMinute<0 || tmpMinute>59)
            goto exit;
        mMinute = tmpMinute;
        if(tmpSecond<0 || tmpSecond>59)
            goto exit;
        mSecond = tmpSecond;
        mWeekDay = dayOfWeek();
        mErrorFlag = false;
        return true;
    exit:
        CLOCK_INFO();
        return false;
    }
    bool fromChar(quint8 pYear, quint8 pMonth, quint8 pDay, quint8 pHour, quint8 pMinute, quint8 pSecond)
    {
        pYear = pYear & 0x3f;
        if(pYear<0 || pYear>63)
            goto exit;
        mYear = pYear;

        pMonth = pMonth & 0xf;
        if(pMonth<1 || pMonth>12)
            goto exit;
        mMonth = pMonth;

        pDay = pDay & 0x1f;
        if(pDay<1 || pDay>31)
            goto exit;
        mDay = pDay;

        pHour = pHour & 0x1f;
        if(pHour<0 || pHour>23)
            goto exit;
        mHour = pHour;

        pMinute = pMinute & 0x3f;
        if(pMinute<0 || pMinute>59)
            goto exit;
        mMinute = pMinute;

        pSecond = pSecond & 0x3f;
        if(pSecond<0 || pSecond>59)
            goto exit;
        mSecond = pSecond;

        mWeekDay = dayOfWeek();
        mErrorFlag = false;
        return true;
    exit:
        CLOCK_INFO();
        return false;
    }

    CLOCK_INFO & operator=(CLOCK_INFO &s)
    {
        fromClock(s);
        return *this;
    }
    CLOCK_INFO & operator=(quint32 &s)
    {
        fromInt(s);
        return *this;
    }
};

inline quint32 ParseTimeString(int pYear, int pMonth, int pDay, int pHour, int pMinute)
{
    quint32 tmpNum = 0;
    tmpNum += pYear<<20;
    tmpNum += pMonth<<16;
    tmpNum += pDay<<11;
    tmpNum += pHour<<6;
    tmpNum += pMinute;
    return tmpNum;
}

inline quint32 ParseTimeString(QString pYear, QString pMonth, QString pDay, QString pHour, QString pMinute)
{
    quint32 tmpNum = 0;
    bool ok = false;
    quint8 tmpYear = pYear.toInt(&ok, 10);
    if(ok)
        tmpNum += tmpYear<<20;
    else
        tmpNum += 0x3f<<20;
    quint8 tmpMonth = pMonth.toInt(&ok, 10); //>4
    if(ok)
        tmpNum += tmpMonth<<16;
    else
        tmpNum += 0x0f<<16;
    quint8 tmpDay = pDay.toInt(&ok, 10);   //>5
    if(ok)
        tmpNum += tmpDay<<11;
    else
        tmpNum += 0x1f<<11;
    quint8 tmpHour = pHour.toInt(&ok, 10);  //>5
    if(ok)
        tmpNum += tmpHour<<6;
    else
        tmpNum += 0x1f<<6;
    quint8 tmpMinute = pMinute.toInt(&ok, 10); //>6
    if(ok)
        tmpNum += tmpMinute;
    else
        tmpNum += 0x3f;
    return tmpNum;
}

//>@ FLOOR

#define FLOORSTART  1000
#define FLOORLEFTBTN  1001

//>@每个数字都是用ASCII码表示
class FLOOR_INFO
{
public:
    FLOOR_INFO()
    {
        mSingleBits = D_INVALID_NUM;
        mTenBits = D_INVALID_NUM;
        mHundredBits = D_INVALID_NUM;
        mReserve = D_INVALID_NUM;
    }
    FLOOR_INFO(char pHundred, char pTen, char pSingle)
    {
        mSingleBits = pSingle;
        mTenBits = pTen;
        mHundredBits = pHundred;
        mReserve = D_INVALID_NUM;
    }
    FLOOR_INFO(quint8 pHundred, quint8 pTen, quint8 pSingle)
    {
        mSingleBits = pSingle;
        mTenBits = pTen;
        mHundredBits = pHundred;
        mReserve = D_INVALID_NUM;
    }
    FLOOR_INFO(quint32 pFloor)
    {
        *this = pFloor;
    }
public:
    quint8  mSingleBits;  //>@保存为ASCII码，比如0表示为0x30
    quint8  mTenBits;
    quint8  mHundredBits;
    quint8  mReserve;
public:
    bool operator==(const FLOOR_INFO &s)
    {
        if(mHundredBits != s.mHundredBits)
            return false;
        if(mTenBits != s.mTenBits)
            return false;
        if(mSingleBits != s.mSingleBits)
            return false;
        return true;
    }
    bool operator!=(const FLOOR_INFO &s)  { return !operator==(s); }

    FLOOR_INFO & operator=(const FLOOR_INFO &s)
    {
        mHundredBits = s.mHundredBits;
        mTenBits = s.mTenBits;
        mSingleBits = s.mSingleBits;
        mReserve = s.mReserve;
        return *this;
    }
    FLOOR_INFO & operator=(const char* s)
    {
        if(strlen(s)==4)
        {
            mHundredBits = s[0];
            mTenBits = s[1];
            mSingleBits = s[2];
        }
        else
        {
            mSingleBits = D_INVALID_NUM;
            mTenBits = D_INVALID_NUM;
            mHundredBits = D_INVALID_NUM;
        }
        mReserve = D_INVALID_NUM;
        return *this;
    }
    FLOOR_INFO & operator=(const quint32 &s)
    {
        mHundredBits = (s >> 16) & 0x000000ff;
        mTenBits = (s >> 8) & 0x000000ff;
        mSingleBits = s & 0x000000ff;
        mReserve = D_INVALID_NUM;
        return *this;
    }
    FLOOR_INFO & operator=(const QString &s)
    {
        int count = s.count();
        if(count >= 6)
        {
            mHundredBits = s.mid(0, 2).toInt();
            mTenBits = s.mid(2, 2).toInt();
            mSingleBits = s.mid(4, 2).toInt();
        }
        else if(count >= 4)
        {
            mHundredBits = 0;
            mTenBits = s.mid(0, 2).toInt();
            mSingleBits = s.mid(2, 2).toInt();
        }
        else
        {
            mHundredBits = 0;
            mTenBits = 0;
            mSingleBits = s.mid(0, 2).toInt();
        }
        mReserve = D_INVALID_NUM;
        return *this;
    }
    bool isValid()
    {
        if((mSingleBits == D_INVALID_NUM) && (mTenBits == D_INVALID_NUM) && (mHundredBits == D_INVALID_NUM))
            return false;
        return true;
    }
    void Rise()
    {
        if(mSingleBits>='9')
        {
            mSingleBits = '0';
            if(mTenBits>='9')
            {
                mTenBits = '0';
                if(mHundredBits>='9')
                {
                    mHundredBits = '0';
                    mTenBits = '0';
                    mSingleBits = '0';
                }
                else
                    mHundredBits = mHundredBits + 1;
             }
             else
                mTenBits = mTenBits + 1;
        }
        else
        {
            mSingleBits = mSingleBits + 1;
        }
    }
    void Descend()
    {
        if(mSingleBits<='0')
        {
            mSingleBits = '9';
            if(mTenBits<='0')
            {
                mTenBits = '9';
                if(mHundredBits<='0')
                {
                    mHundredBits = '9';
                    mTenBits = '9';
                    mSingleBits = '9';
                }
                else
                    mHundredBits = mHundredBits - 1;
             }
             else
                mTenBits = mTenBits - 1;
        }
        else
        {
            mSingleBits = mSingleBits - 1;
        }
    }

    quint32 toInt()
    {
        return ((D_INVALID_NUM << 24) + (mHundredBits << 16) + (mTenBits << 8) + mSingleBits);
    }
    quint32 toIndex()  //>@转换为集成模式下的图片序号
    {
        return ((mHundredBits>=0x30)?(mHundredBits-0x30):0)*100 + ((mTenBits>=0x30)?(mTenBits-0x30):0)*10 + ((mSingleBits>=0x30)?(mSingleBits-0x30):0);
    }
    QString toMapFormat()
    {
        return QString::number(mHundredBits*10000 + mTenBits*100 + mSingleBits);
    }
    char* toString()    //>@转换为文本模式下的字符串
    {
        char *tmpText = (char *)malloc(4);
        tmpText[0] = mHundredBits;
        tmpText[1] = mTenBits;
        tmpText[2] = mSingleBits;
        tmpText[3] = '\0';
        return tmpText;
    }
};

inline quint32 ParseFloorString(QString pHundred, QString pTen, QString pUnit)
{
    quint32 tmpNum = D_INVALID_NUM << 24;
    bool ok = false;
    quint8 tmpHundred = pHundred.toInt(&ok, 10);
    if(ok)
    {
        tmpHundred += 0x30;
        tmpNum += tmpHundred<<16;
    }
    else
        tmpNum += D_INVALID_NUM<<16;
    quint8 tmpTen = pTen.toInt(&ok, 10);
    if(ok)
    {
        tmpTen += 0x30;
        tmpNum += tmpTen<<8;
    }
    else
        tmpNum += D_INVALID_NUM<<8;
    quint8 tmpUnit = pUnit.toInt(&ok, 10);
    if(ok)
    {
        tmpUnit += 0x30;
        tmpNum += tmpUnit;
    }
    else
        tmpNum += D_INVALID_NUM;
    return tmpNum;
}

inline quint32 ParseFloorString(QString pFloor)
{
    if(pFloor.isEmpty())
        return INVALIDRC;
    QChar pHundred, pTen, pUnit;
    if(pFloor.count() == 1)
    {
        pUnit = pFloor.at(0);
    }
    else if(pFloor.count() == 2)
    {
        pTen = pFloor.at(0);
        pUnit = pFloor.at(1);
    }
    else if(pFloor.count() == 3)
    {
        pHundred = pFloor.at(0);
        pTen = pFloor.at(1);
        pUnit = pFloor.at(2);
    }
    else
    {
        return EXTRARC;
    }

    quint32 tmpNum = D_INVALID_NUM << 24;
    quint8 tmpHundred = (quint8)pHundred.toLatin1();
    tmpHundred += 0x30;
    tmpNum += tmpHundred<<16;

    quint8 tmpTen = (quint8)pTen.toLatin1();
    tmpTen += 0x30;
    tmpNum += tmpTen<<8;

    quint8 tmpUnit = (quint8)pUnit.toLatin1();
    tmpUnit += 0x30;
    tmpNum += tmpUnit;

    return tmpNum;
}

inline quint32 ParseFloorString(QChar pHundred, QChar pTen, QChar pUnit)
{
    quint32 tmpNum = D_INVALID_NUM << 24;
    quint8 tmpHundred = (quint8)pHundred.toLatin1();
    tmpHundred += 0x30;
    tmpNum += tmpHundred<<16;

    quint8 tmpTen = (quint8)pTen.toLatin1();
    tmpTen += 0x30;
    tmpNum += tmpTen<<8;

    quint8 tmpUnit = (quint8)pUnit.toLatin1();
    tmpUnit += 0x30;
    tmpNum += tmpUnit;

    return tmpNum;
}

//>@ MESSAGE

typedef enum{
    LEVEL_INVALID = 100,
    LEVEL_INFO = 0,
    LEVEL_WARN = 1,
    LEVEL_ERROR = 2,
    LEVEL_SUCCESS = 3,
    LEVEL_QUESTION = 4
} MSG_LEVEL;

inline MSG_LEVEL GetMsgLevel(QString pLevel)
{
    if(!pLevel.compare("Info", Qt::CaseInsensitive))
        return LEVEL_INFO;
    else if(!pLevel.compare("Warn", Qt::CaseInsensitive))
        return LEVEL_WARN;
    else if(!pLevel.compare("Error", Qt::CaseInsensitive))
        return LEVEL_ERROR;
    return LEVEL_INVALID;
}

inline QString GetMsgLevel(MSG_LEVEL pLevel)
{
    if(pLevel == LEVEL_INFO)
        return QString("Info");
    else if(pLevel == LEVEL_WARN)
        return QString("Warn");
    else if(pLevel == LEVEL_ERROR)
        return QString("Error");
    return QString();
}

#define MSG_PERMANENT   10000000
#define MSG_TEMPORARY   10000001
#define MSG_HOLD        10000002

class MSG_INFO
{
public:
    MSG_INFO(MSG_LEVEL pLevel, QString pMsg, int pDelay)
    {
        mMsgLevel = pLevel;
        mMessage = pMsg;
        mDelay = pDelay;
        mCount = 1;
    }
    bool isValid()
    {
        if(mMessage.isEmpty() || mMsgLevel==LEVEL_INVALID || mDelay<=0)
            return false;
        return true;
    }
public:
    MSG_LEVEL mMsgLevel;
    QString   mMessage;
    int       mDelay;
    int       mCount;
};

//>@Animal

typedef enum{
    TypeFish=0,
    TypeInsect
}AnimalType;

typedef enum{
    StageBrith=0,
    StageYoung,
    StageMature,
    StageOld,
    StageDeath,
    StageUnknow=255
}AnimalStage;


//>@ SETBAR

typedef enum{
    ITEM_PREV = 0,
    ITEM_CUR = 1,
    ITEM_NEXT = 2,
    ITEM_HIDE = 3
} ITEM_LOCATION;

typedef enum{
    MOVE_NONE = 0,
    MOVE_LEFT = 1,
    MOVE_RIGHT = 2
} MOVE_DIRECTION;

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

class ThreadTick : public QThread
{
    Q_OBJECT
public:
    ThreadTick(quint32 pTickFreq = 1000, QObject *parent = 0);
    ~ThreadTick();

    void Wait(quint32 pTimeout);

    bool Start();
    bool Stop();

signals:
    void sTick();

public slots:
    void slot_Start();
    void slot_Stop();

protected:
    void run();

public:
    quint32         m_TickFreq;
    bool            m_Enable;
};

//>@FLOORBUTTON

#define TEXTRATIO   4.0
#define FLOORWIDTH  10.0

typedef enum{
    PIC_DARK=0,
    PIC_LIGHT=1,
    PIC_EFFECT=2,
    TEXT_DARK=3,
    TEXT_LIGHT=4
}BUTTON_RC;

typedef struct{
    qint8       mFadePointer;
    qint8       mEffectPointer;
    qint8       mTextPointer;
}BUTTON_POINTER;

typedef enum{
    PTN_FB_GRID = 0,    //>@表格样式
    PTN_FB_ELLIPSE,     //>@椭圆样式
    PTN_FB_GUIDE        //>@向导样式
} FB_PATTERN;

typedef enum{
    PTN_KB_GRID_H = 0,    //>@表格横排序
    PTN_KB_GRID_V,        //>@表格竖排序
    PTN_KB_QWERTY,
    PTN_KB_DISEN,
    PTN_KB_YUNGTAY
} KB_PATTERN;

typedef enum{
    PTN_SB_HOLLOW = 0,    //>@凹陷效果
    PTN_SB_MENU           //>@菜单效果风格
} SB_PATTERN;

struct PortSettings
{
    BaudRateType    BaudRate;
    DataBitsType    DataBits;
    ParityType      Parity;
    StopBitsType    StopBits;
    FlowType        FlowControl;
    long            Timeout_Millisec;
};

inline QString GetLocalIP()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    for(int i=0;i<list.count();i++)
    {
        QHostAddress address = list.at(i);
       if(address.protocol() == QAbstractSocket::IPv4Protocol) //我们使用IPv4地址
            return address.toString();
    }
    return 0;
}

inline QStringList GetLocalIPListFromNet(QString pNetName)
{
    QStringList tmpIPList;
    if(pNetName.isEmpty())
        return tmpIPList;
    QNetworkInterface tmpInterface = QNetworkInterface::interfaceFromName(pNetName);
    if(tmpInterface.isValid() == false)
        return tmpIPList;
    QList<QHostAddress> list = tmpInterface.allAddresses();
    for(int i=0;i<list.count();i++)
    {
        QHostAddress address = list.at(i);
       if(address.protocol() == QAbstractSocket::IPv4Protocol) //我们使用IPv4地址
       {
           tmpIPList.append(address.toString());
       }
    }
    return tmpIPList;
}

inline QStringList GetLocalIPList()  //获取ip地址
{
    QStringList tmpIPList;
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    for(int i=0;i<list.count();i++)
    {
        QHostAddress address = list.at(i);
       if(address.protocol() == QAbstractSocket::IPv4Protocol) //我们使用IPv4地址
       {
           tmpIPList.append(address.toString());
       }
    }
    return tmpIPList;
}

inline QString getUserName()  //获取用户名
{
    QStringList envVariables;
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
                 << "HOSTNAME.*" << "DOMAINNAME.*";
    QStringList environment = QProcess::systemEnvironment();
    for(int i=0;i<envVariables.count();i++)
    {
        QString string = envVariables.at(i);
        int index = environment.indexOf(QRegExp(string));
        if (index != -1)
        {
            QStringList stringList = environment.at(index).split('=');
            if (stringList.size() == 2)
            {
                return stringList.at(1);
            }
        }
    }
    return QString();
}

inline bool ConvertImageTo565(QImage &pImg)
{
    if(pImg.isNull())
        return false;
    int width=pImg.width();
    int height=pImg.height();
    int r,g,b;
    for (int i=0;i<height;i++)
    {
        for (int j=0;j<width;j++)
        {
            QPoint tmpPoint = QPoint(j, i);
            QRgb tmpRgb = pImg.pixel(tmpPoint);
            r = qRed(tmpRgb) >> 2;
            g = qGreen(tmpRgb) >> 2;
            b = qBlue(tmpRgb) >> 2;
            pImg.setPixel(tmpPoint, ((tmpRgb & 0xff000000) | (r<<16) | (g<<8) | (b)));
        }
    }
    return true;
}

inline bool RGB_BGR(QImage &pImg)
{
    if(pImg.isNull())
        return false;
    int width=pImg.width();
    int height=pImg.height();
    int r,g,b;
    for (int i=0;i<height;i++)
    {
        for (int j=0;j<width;j++)
        {
            QPoint tmpPoint = QPoint(j, i);
            QRgb tmpRgb = pImg.pixel(tmpPoint);
            r = qBlue(tmpRgb);
            g = qGreen(tmpRgb);
            b = qRed(tmpRgb);
            pImg.setPixel(tmpPoint, ((tmpRgb & 0xff000000) | (r<<16) | (g<<8) | (b)));
        }
    }
    return true;
}

//>@323248 -> 0x202030
inline quint32 DecnumToHexnum(quint32 pNum)
{
    quint32 tmpNum0 = (pNum%100000000/1000000);
    quint32 tmpNum1 = (pNum%1000000/10000);
    quint32 tmpNum2 = (pNum%10000/100);
    quint32 tmpNum3 = (pNum%100);
    return (tmpNum0<<24)+(tmpNum1<<16)+(tmpNum2<<8)+tmpNum3;
}

void ExecProcess(QString pCommand, QString &pStdOut, QString &pStdErr, int msecs = 3000);

UI_TYPE getUiType(QString pName);
QString getUiString(UI_TYPE pType);

qreal RegularAngle(qreal angle);
qreal CalDiffBeginAngle(qreal angle);

//>@从磅换算到像素
/*   Point实际是磅，也就是 1/72 inch
     DPI: Dot per inch，在显示器上也就是每英寸包含的像素。英尺的换算为 1 cm = 0.39 inch, 1 inch = 2.54 cm。
     DPI 可以通过如下计算得来：
            水平 dpi = 水平 resolution * 2.54 / 显示器宽度
            垂直 dpi = 垂直 resolution * 2.54 / 显示器高度
     pixel pitch（点距）: 像素点间距离。可以用25.4mm / DPI得到。
     字体在屏幕上的实际大小 = 字体像素大小 * 点距
*/
typedef enum{
    PRIOR_WIDTH = 0,
    PRIOR_HEIGHT
} PRIOR;
#define DPI 96 //>@Dot per inch
int CalcPixelFromPoint(int pPoint);
int CalcPointFromPixel(int pPixel);
int CalcFontSizeFromPixel(int pPixel);
int CalcFontSizeFromRect(QString pFontType, QSize pSize);
int CalcFontSizeFromRect(QString pFontType, QSize pSize, PRIOR pPrior);
int CalcFontSizeFromSize(QString pFontType, QSize pSize, PRIOR pPrior);
QSize CalcSizeFromFontSize(QString pFontType, int pFontSize);
QSize CalcSizeFromFontSize(QString pFontType, QChar pChar, int pFontSize);
QSize CalcSizeFromFontSize(QString pFontType, QString pText, int pFontSize);

QPointF CalcIntersectPos(QRectF pRect, QLineF pLine);

void WriteText(QPainter *pPainter, QString pText, QRect pRect, QFont pFont, QColor pColor);
void WriteText(QPainter *pPainter, QString pText, QRect pRect, QString pFontType=QString("Arial"), QColor pColor=Qt::black, PRIOR pPrior=PRIOR_WIDTH);
void WriteText(QPainter *pPainter, QString pText, QRect pRect, QString pFontType, QColor pColor, int pFontSize);

inline QPixmap ScalePixmap(QPixmap pPixmap, QSizeF pSize)
{
    QSize tmpSize = pSize.toSize();
    if(!(tmpSize.isEmpty()) && pPixmap.size() != tmpSize)
        return pPixmap.scaled(tmpSize);
    return pPixmap;
}

qreal normalizeAngle(qreal angle);

inline QBrush GetLinearGradientBrush(QPointF pStartPos, QColor pStartColor, QPointF pEndPos, QColor pEndColor)
{
    QLinearGradient linearGradient(pStartPos, pEndPos);
    linearGradient.setColorAt(0, pStartColor);
    linearGradient.setColorAt(1, pEndColor);//设置渐变的颜色和路径比例
    return QBrush(linearGradient);
}

QListWidgetItem *AddItemToListWidget(QListWidget *pList, QString pText);

bool isFileExist(QString FileName);
bool delFile(QString FileName);
QStringList SplitString(QString pString, QString pKey);

QString  BiteorMega(int peso);
bool is_file(QString fullFileName);

bool StringToInt(QString pString,int &tmp);

QString getFileIconName(QString pFileName);

FILEPATH_TYPE getRcFileType(QString pFileName);

FILE_TYPE getFileType(QString FileName);
FILE_TYPE getFileType(FILE_INFO *pFileInfo);
QString   getFileSuffix(FILE_TYPE pFileType);
QString   getNewFileName(FILE_TYPE pType);
RC_TYPE   getRcType(QString FileName);
QString   regularFile(QString pRcName, QString pSrcFile, QString pDstPath, QSize pSize);

int GetKeyType(QString pKey);

QString getParentDirectory(QString DirPath);
QString GetDirectoryName(QString DirPath);

QString getFileSuffix(QString pFileName);
QString getFileName(QString FilePath);
QString getFileDirectory(QString FilePath);

QRect GetRectFromString(QString pRectString);

QRectF GetRotateRect(QRectF pRect, ROTATE_ANGLE pAngle);
CONSTRAINT GetRotateConstraint(CONSTRAINT pConstraint, ROTATE_ANGLE pAngle);

qint64 GetPathSize(QString pDir);

inline bool CheckCapacity(QString path, quint64 pSize)
{
    qint64 tmpSize = GetPathSize(path);
    return (tmpSize == pSize);
}

QStringList findFiles(const QDir &dir,
                      const QString &fileName,
                      const QString &txt,
                      Qt::CaseSensitivity sensitive);
QStringList findFilesInFloder(QString DirName, QString fileName, bool subfolder);
QStringList ErgodicDirectory(QString d);
QStringList SearchFiles(QString pDirPath, QString pSuffix, bool pSubfolder=false);

QStringList getAllInFloders(bool pSubDir, QString pDir, bool pSize);
QStringList getAllFileInFloders(QString pDir);
QStringList getAllSubfolderInFloders(QString pDir);

QStringList getAllFilesAndSize(QString pDir);

bool CreatPath(QString pPath);
bool CopyFolder(QString pSrcDir, QString pDstDir, bool pSubfolder=true, bool pCopyFile=true, bool pIdel=false);
bool MoveFolder(QString pSrc, QString pDst);
bool DelFolder(QString pDir, bool pIdel=false);
bool ClearFolder(QString pDir);
bool CutFile(QString srcFile, QString dstFile);
bool CopyFile(QString srcFile, QString dstFile, QString pPrefix=QString());
bool DelFile(QString pPath);
bool CreatFile(QString pPath);

class FileOperate : public QThread
{
    Q_OBJECT
public:
    FileOperate(QObject *parent = 0);
    void Start(int pType);
    void run();
    void Wait(int pTimeout);
    void Stop();

    bool CopyFolder(QString pSrcDir, QString pDstDir, bool pSubfolder=true, bool pCopyFile=true, bool pIdel=false, quint32 pTimeout = 0x7FFFFFFF);
    bool MoveFolder(QString pSrc, QString pDst, quint32 pTimeout = 0x7FFFFFFF);
    bool DelFolder(QString pDir, bool pIdel=false, quint32 pTimeout = 0x7FFFFFFF);
    bool ClearFolder(QString pDir, quint32 pTimeout = 0x7FFFFFFF);
    bool CutFile(QString srcFile, QString dstFile, quint32 pTimeout = 0x7FFFFFFF);
    bool CopyFile(QString srcFile, QString dstFile, QString pPrefix=QString(), quint32 pTimeout = 0x7FFFFFFF);
    bool DelFile(QString pPath, quint32 pTimeout = 0x7FFFFFFF);

signals:
    void sProgress(QString pMsg, quint32 pPrg);

public:
    QString                    m_Src, m_Dst;
    bool                       m_SubFolder, m_CopyFile, m_Idel;
    QString                    m_Prefix;
    int                        m_OperateType;
    int                        m_Status;
    bool                       m_StopFlag;
};

//>@---------------MDP依赖函数-------------------

class MDPConfigParaInfo
{
public:
    enum Permission{RO,RW,EDIT};
public:
    MDPConfigParaInfo(const QDomElement &pElement);
    ~MDPConfigParaInfo();
    bool IsNull();

    typedef struct{
        QString                     Name;
        QList<MDPConfigParaInfo *>  ParaInfoList;
    } SubConfigParaInfo;

public:
    QString                     mName;
    Permission                  mPermission;
    UI_TYPE                mParaType;
    QString                     mParas;
    QString                     mValue;
    QList<SubConfigParaInfo*>   mSubInfos;
};

class MDPConfigInfo
{
public:
    MDPConfigInfo(const QDomElement &pElement);
    ~MDPConfigInfo();
    bool IsNull();

public:
    QString                     mName;
    bool                        mDrag;
    QString                     mRc;
    QRectF                      mRectF;
    int                         mZValue;
    QList<MDPConfigParaInfo *>  mParaInfoList;
};

bool ParseIP(QString pIP, QHostAddress &pIPAddr, quint16 &pPort, QString &pCode);

inline bool CheckIP(QString pIP)
{
    QHostAddress tmpIP;
    quint16 tmpPort;
    QString tmpCode;
    return ParseIP(pIP, tmpIP, tmpPort, tmpCode);
}


//>@---------------UI依赖函数-------------------
COM_TYPE getComponetType(QString ComponentName);
QString getComponetName(COM_TYPE ComponentType);
QString getComponetPath(COM_TYPE pType);
COM_TYPE getParentComType(QString ComPath);
COM_TYPE getParentComType(COM_TYPE pType);
QString getCurComName(QString ComPath);
QString getParentComPath(QString ComPath);
QString getComPath(QString ComName);
COM_TYPE getCurComType(QString ComPath);

#if(QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
void SetFuzzy(QGraphicsItem *pItem, qreal pBlurRadius);
void SetColor(QGraphicsItem *pItem, QColor pColor);
void SetShadow(QGraphicsItem *pItem, QColor pColor, qreal pBlurRadius, QPointF pPointF);
void SetOpacity(QGraphicsItem *pItem, qreal pOpacity);
void ClearFuzzy(QGraphicsItem *pItem);
void ClearColor(QGraphicsItem *pItem);
void ClearShadow(QGraphicsItem *pItem);
void ClearOpacity(QGraphicsItem *pItem);

void Effect_Fuzzy(QGraphicsItem *Item,qreal n);
void Effect_color(QGraphicsItem *Item,int r, int g, int b);
void Effect_Shadow(QGraphicsItem *Item,qreal dx, qreal dy);
void Effect_Transparent(QGraphicsItem *Item,qreal opacity);
#endif

//>@---------------转换函数-------------------
typedef enum{
    Audio_Start,
    Audio_PathErr,
    Audio_Suncess,
    Audio_Suncess_Cover,
    Audio_Suncess_Cancel,
    Audio_TimeOut,
    Audio_Failed
} AUDIO_CONVERT_STATE;

typedef enum{
    Video_Start,
    Video_PathErr,
    Video_Suncess,
    Video_Suncess_Cover,
    Video_Suncess_Cancel,
    Video_TimeOut,
    Video_Failed
} VIDEO_CONVERT_STATE;

typedef struct{
    QString                     m_FileName;
    QString                     m_FilePath;
    QString                     m_DstPath;
    quint32                     m_FileLenth;
}CONVERT_FILE_INFO;

typedef struct{
    quint16                     m_TopCut;
    quint16                     m_BottomCut;
    quint16                     m_LeftCut;
    quint16                     m_RightCut;
}CUT_SET;

QString GetStrFromKey(QString pString, QString pKey,
                      quint8 pStart, quint8 pLenth,
                      Qt::CaseSensitivity pCaps);
QString GetStrBehindKey(QString pString, QString pKey,
                        quint8 pStart, quint8 pLenth,
                        Qt::CaseSensitivity pCaps);
QString GetStrBetweenPreAndAfter(QString pString,
                                 QString pPre, QString pAfter,
                                 int pPreindent, int pAfterindent,
                                 Qt::CaseSensitivity pCaps);

bool ConvertImage(QString pSrcFileName, QRect pValidRect, QString pDstFileName, QSize pResolution);
bool ConvertImage(QString pSrcFileName,
                  QString pDstFileName,
                  QSize pResolution);
bool ConvertImage(QString pFileName, QSize pSize, PRIOR pPrior);
QImage GetScaledImage(QString pFileName, QSize pSize, PRIOR pPrior);
QSize GetScaledImageSize(QString pFileName, QSize pSize, PRIOR pPrior);

bool ConvertAudio(QString pSrcFileName, QString pDstFileName);

bool ConvertVideo(QString pSrcFileName, QString pDstFileName, QSize pResolution);
bool ConvertVideo(QString pSrcFileName, QRect pValidRect, QString pDstFileName, QSize pResolution);

//>@---------------函数-------------------
#ifdef WINDOWS
size_t _Scanf(pInputCharFun InputFun, const char* fmt,...);
size_t _Printf(pOutputStrFun OutputStrFun, const char* fmt,...);
size_t _Printf(pOutputCharFun OutputCharFun, const char* fmt,...);
#endif

//>@---------------日志函数-------------------
bool InitLogPath(QString pLogPath, QString pLogName, int pMaxLogFileNum);

//>@---------------公有类-------------------------

class GraphicsSceneBase : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsSceneBase(QObject *parent, QRect pRect);
public:
    QRectF            m_Rect;
};

class GraphicsViewBase : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsViewBase(QWidget *parent, QRect pRect);
    GraphicsViewBase(QWidget *parent);
public:
    GraphicsSceneBase *m_Scene;
};

//>@---------------解析类-------------------------

QString GetStrBehindKey
    (QString pString, QString pKey,
     quint8 pStart, quint8 pLenth,
     Qt::CaseSensitivity pCaps);

QString GetStrBetweenPreAndAfter
    (QString pString,
     QString pPre, QString pAfter,
     int pPreindent, int pAfterindent,
     Qt::CaseSensitivity pCaps);

/************************************************************************
//>@
************************************************************************/

typedef struct{
    RC_TYPE                     mType;
    QString                     mPath;
    QString                     mName;
    quint32                     mSize;
    QSize                       mResolution;
    quint32                     mLenth;
}Media_Info;
//>@
typedef struct{
    bool                        mEnable;
    QString                     mPara;
}GRABSCREEN_SET;
typedef struct{
    bool                        mEnable;
    QString                     mPara;
}WATERMARk_SET;
typedef struct{
    bool                        mEnable;
    QString                     mPara;
}TEXT_SET;

class MULTIMEDIA_CONVERT : public QObject
{
    Q_OBJECT
public:
    MULTIMEDIA_CONVERT(QObject *parent = 0);
    ~MULTIMEDIA_CONVERT();
    void KillProcess()
    {
        mMutex->lock();
        if(m_Process != 0)
        {
            disconnect(m_Process, SIGNAL(readyReadStandardOutput()), 0, 0);
            disconnect(m_Process, SIGNAL(finished(int, QProcess::ExitStatus)), 0, 0);
            m_Process->kill();
            static quint8 tmpState;
            while((tmpState = m_Process->state()))
            {
                QtSleep(20);
            }
            m_Process->waitForFinished(1000);
            delete m_Process;
            m_Process = 0;
        }
        mMutex->unlock();
    }
    bool StartProcess(QString pCmd, int pTimeout)
    {
        if(pCmd.isEmpty())
            return false;
        mTaskState = Start;
        KillProcess();
        m_Process = new QProcess(0);
        m_Process->setProcessChannelMode(QProcess::MergedChannels);
        connect(m_Process, SIGNAL(readyReadStandardOutput()),this, SLOT(slot_CatchStandardOutput()));
        connect(m_Process,
                SIGNAL(finished(int, QProcess::ExitStatus)),
                this,
                SLOT(slot_Finished(int, QProcess::ExitStatus)),
                Qt::DirectConnection);
        m_Process->start(pCmd);
        m_Process->waitForFinished(pTimeout);
        KillProcess();
        switch(mTaskState)
        {
            case Success:return true;
            default:return false;
        }
    }
    bool PreHandel(QString pSrc, QString pDst)
    {
        QFile srcFile(pSrc);
        if(!srcFile.exists())
        {
            return false;
        }
        QString dstDir = getFileDirectory(pDst);
        QDir dir(dstDir);
        if(!dir.exists())
        {
            if(!dir.mkpath(dstDir))
            {
                return false;
            }
        }
        QFile dstFile(pDst);
        if(dstFile.exists())
        {
            dstFile.setPermissions(QFile::WriteOther);
            if(!dstFile.remove())
            {
                return false;
            }
        }
        return true;
    }
    bool GetFileInfo(QString pSrc, int pTimeout);
    bool ConvertAudio(QString pSrc, QString pDst, int pTimeout);
    bool ConvertVideo(QString pSrc, QSize pResolution, int pFrame, QString pDst, int pTimeout);
    bool ConvertVideo(QString pSrc, QRect pRect, QSize pResolution, int pFrame, QString pDst, int pTimeout);
//    rm/flv/avi/rmvb
    Media_Info          m_FileInfo;
    TaskState           mTaskState;
    QMutex*             mMutex;
private:
    RC_TYPE GetSrcType(QString pStr)
    {
        if(pStr.contains("mp3",Qt::CaseInsensitive))
            return RC_MP3;
        if(pStr.contains("wav",Qt::CaseInsensitive))
            return RC_WAV;
        if(pStr.contains("mp4",Qt::CaseInsensitive))
            return RC_MP4;
        if(pStr.contains("avi",Qt::CaseInsensitive))
            return RC_AVI;
        if(pStr.contains("rmvb",Qt::CaseInsensitive))
            return RC_RMVB;
        if(pStr.contains("flv",Qt::CaseInsensitive))
            return RC_FLV;
        return RC_INVALID;
    }
    quint32 GetLenth(QString pLenth)
    {
        QStringList tmpStrList = SplitString(pLenth,":");
        quint8  tmpSecond = 0,
                tmpMinute = 0,
                tmpHour = 0;
        quint32 tmpLenth = 0;
        if(tmpStrList.count() != 3)
            return 0;
        bool tmpOk = false;
        tmpSecond = tmpStrList.at(2).toInt(&tmpOk,10);
        if(!tmpOk)
            return 0;
        tmpMinute = tmpStrList.at(1).toInt(&tmpOk,10);
        if(!tmpOk)
            return 0;
        tmpHour = tmpStrList.at(0).toInt(&tmpOk,10);
        if(!tmpOk)
            return 0;
        tmpLenth = tmpHour*60*60 + tmpMinute*60 +tmpSecond;
        return tmpLenth;
    }
    QSize GetResolution(QString pKey)
    {
        QStringList keyList = pKey.split(",", QString::SkipEmptyParts, Qt::CaseInsensitive);
        for(int i=0;i<keyList.count();i++)
        {
            QString subKey = keyList.at(i);
            QStringList subKeyList = subKey.split("x", QString::SkipEmptyParts, Qt::CaseInsensitive);
            if(subKeyList.count() == 2)
            {
                quint16 w = subKeyList.at(0).toInt();
                quint16 h = subKeyList.at(1).toInt();
                QSize size(w, h);
                return size;
            }
        }
        return QSize();
    }
    quint8 GetProgress(QString pKey, quint32 pLenth)
    {
        QStringList keyList = SplitString(pKey,".");
        if(keyList.isEmpty())
            return 0;
        bool ok = false;
        quint16 curPos = keyList.at(0).toInt(&ok,10);
        if(!ok)
            return 0;
        curPos = ((float)curPos/(float)pLenth)*100;
        curPos = (curPos > 100)? 100 : curPos;
        return curPos;
    }
    QString SizeToStr(QSize pSize)
    {
        int width = pSize.width();
        int height = pSize.height();
        width = width - width%10;
        height = height - height%10;
        QString size = QString("%1x%2").arg(width).arg(height);
        return size;
    }

    QProcess            *m_Process;
signals:
    void sProgress(quint8 pVaule);
private slots:
    void slot_CatchStandardOutput();
    void slot_Finished(int, QProcess::ExitStatus);
};

QPixmap GenerateSpectrum(QString pMediaName, QRectF pRect, QPixmap pBgPixmap=QPixmap());

class xmlParse : public QObject, public QDomDocument
{
    Q_OBJECT
public:
    xmlParse();
    xmlParse(QString fileName);
    ~xmlParse();

    static const QString XmlHead;
    static const QString XmlSuffix;

    QString errorStr;
    int errorLine;
    int errorColumn;

    bool m_Valid;

    bool m_IsChanged;
    bool m_ForceSave;

    void SetForcesave(bool pEnable) {m_ForceSave = pEnable;}

    QString m_FileDir;
    QString m_FileName;

    QDomElement itemElement;

    //>@根据路径查找
    bool GetItemElement(QString itemPath);
    //>@以元素itemDomElement为根路径进行查找
    bool GetItemElement(QDomElement itemDomElement, QString itemPath);

    //>@模糊查找
    bool SearchItemElement(QString itemName);

    //>@获取元素值 多态
    QString GetItemElementValue();
    QString GetItemElementValue(QString itemPath);

    //>@遍历属性
    QList<QDomAttr> GetItemElementAttrs(QDomElement itemDomElement);
    QList<QDomAttr> GetItemElementAttrs();
    QList<QDomAttr> GetItemElementAttrs(QString itemPath);

    //创建元素节点
    bool CreateItemElement(QString itemPath);
    bool CreateItemElement(QDomElement pParElement, QString pName, QString pValue);

    //>@保存某一个元素为XML文本
    bool SaveItemElement(QDomElement itemDomElement, QString& pContent);     //>@保存到pContent中
    bool SaveItemElement(QString &pContent);
    bool SaveItemElement(QDomElement itemDomElement, QIODevice *pDevice);   //>@保存到IODevice中
    bool SaveItemElement(QIODevice *pDevice);

    bool ModifyItemElement(QString itemPath, QString value);
    bool ModifyItemElement(QDomElement itemDomElement, QString value);
    bool SetText(QDomElement itemDomElement, QString value);

    bool ClearItemElement(QDomElement itemDomElement);
    bool DeleteItemElement(QDomElement itemDomElement);

    //获取当前元素所有的子节点
    QDomNodeList GetChildNodes()
    {
        return itemElement.childNodes();
    }

    bool write(QIODevice *device);
    bool read(QIODevice *device);
    bool Open();
    bool New();
    bool Save();
    bool SaveAs(QString fileName);
    bool Rename(QString fileName);
};

//>@注意key不能为“/”，否则会死机
class iniParse : public QSettings
{
    Q_OBJECT
public:
    iniParse(QString pfileName);
    void setProperty(QString key,QString value) { setValue(key, value); }
    void setProperty(QString key,int value)     { setValue(key, value); }

    bool getProperty(QString pkey, QString &pValue);
    bool getProperty(QString pkey, int &pValue);

    /*
    可以直接读取中文的QSTRING的，不过真正的意义在于把QSETTING的读取返回值放入 到QByteArray中才是关键。
    QTextCodec *codec=QTextCodec ::codecForName("GBK"); //设定字符集为GBK（大小写均可，还可用GB18030代替）
    QString outStr = codec->toUnicode(myArray); //这里就好解释了，通过译码器将字节数组中的值转化为UNICODE的QSTRING就O了
    */
};

#endif // GLOBALFUN_H
