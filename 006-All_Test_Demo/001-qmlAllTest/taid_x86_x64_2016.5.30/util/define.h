#ifndef DEFINE_H
#define DEFINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <errno.h>
#include <fcntl.h>
#include <assert.h>
#include <time.h>

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
#include <RegStr.h>   //>@注册表常用宏定义
#ifdef SURRPORT_NETMANAGER  //>@网络流量定义
#include <iphlpapi.h>
#include <ipexport.h>
#include <iprtrmib.h>
#include <iptypes.h>
#endif
#endif

#if (defined(UBUNTU) || defined(LINUX))
//#include <sys/stat.h>
//#include <sys/mman.h>
//#include <sys/soundcard.h>
//#include <sys/ioctl.h>
//#include <sys/fcntl.h>
//#include <sys/types.h>
//#include <signal.h>
//#include <unistd.h>
//#include <sys/reboot.h>
#endif

#include <QtCore>
#include <QtGui>

#ifdef N329
#define D_QT_MAX_46   (1)
#else
#define D_QT_MAX_46   (0)
#endif
#define D_QT_MAX_47   (QT_VERSION < QT_VERSION_CHECK(4, 7, 0))
#define D_QT_MAX_50   (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))

#define D_QT_MIN_46   (QT_VERSION > QT_VERSION_CHECK(4, 5, 9))
#define D_QT_MIN_50   (QT_VERSION > QT_VERSION_CHECK(4, 9, 9))

#if D_QT_MIN_50
#include <QtWidgets>
#endif

#include <QDomDocument>
//#include <QSvgRenderer>
#include <QtNetwork>
#include <QString>

#define QTASCII 0            //>@ 是否使用QT定义的ASCII码表

#if  D_QT_MAX_47
Q_DECLARE_METATYPE(QVariant)
#endif

#if D_QT_MAX_47
#define VAR_CAST   static_cast
#else
#define VAR_CAST   qobject_cast
#endif

#define VAR_NAME(varname)   QString(#varname)   //转换变量名为字符串

#define PI 3.14159265358979323846264338327950288419717

#define D_Error             QString("Error")
#define D_Warning           QString("Warning")
#define D_Success           QString("Success")
#define D_Failed            QString("Failed")

//------------------------------------------------------------------------------

typedef char                    UD_INT8;
typedef unsigned char           UD_UINT8;
typedef short       			UD_INT16;
typedef unsigned short      	UD_UINT16;
typedef int                     UD_INT32;
typedef unsigned int          	UD_UINT32;
typedef long long               UD_INT64;
typedef unsigned long long      UD_UINT64;
typedef UD_UINT8                UD_BOOL;

//------------------------------------------------------------------------------
#ifdef I_GUI
#define  D_PAINTER_INIT(object)         QPainter p(object);
#else
#define  D_PAINTER_INIT(object)         QStyleOption opt;   \
    opt.init(object); \
    QPainter p(object); \
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, object);
#endif

#define D_FUNC_TYPE(addr)             (UD_UINT8)((UD_UINT16)(addr) >> 8)
#define D_FUNC_SUBADDR(addr)          (UD_UINT8)((UD_UINT16)(addr) & 0x00ff)

#define D_FUNC_ADDR(type,subaddr)     (UD_UINT16)((UD_UINT16)((UD_UINT16)(type) << 8) + (UD_UINT16)(subaddr))

#define D_FUNC_LENGTH(lenH,lenL)      (UD_UINT16)((UD_UINT16)((UD_UINT16)(lenH) << 8) + (UD_UINT16)(lenL))
#define D_FUNC_LENH(len)              (UD_UINT8)((UD_UINT16)(len) >> 8)
#define D_FUNC_LENL(len)              (UD_UINT8)((UD_UINT16)(len) & 0x00ff)

typedef enum {
    E_REG_DEV  = 0,
    E_REG_CPU,
    E_REG_MEM,
    E_REG_DISK,
    E_REG_DIAL,
    E_REG_PLAYER,
    E_REG_LCD,
    E_REG_LED,
    E_REG_RTC,
    E_REG_WDG,
    E_REG_NET,
    E_REG_PROTOCOL
}E_REG_TYPE;

//-----------------------------------------------------------------------------------------
//                                类型      进度和消息  错误类型
typedef void  ( *pShowInfo )   (  quint16, QVariant, QVariant );

typedef void (*CallBackFunc)();

//-----------------------------------------------------------------------------------------

inline bool operator==(QPixmap b1, QPixmap b2) { return b1.data_ptr() == b2.data_ptr(); }

//-----------------------------------------------------------------------------------------

#define D_SYSTEMDIR         QString("SYSTEM/")      //>@存放物理、逻辑以及中间件
#define D_THEMEDIR          QString("THEME/")       //>@存放所有主题
#define D_EXEDIR            QString("EXE/")         //>@存放所有应用程序
#define D_LOGDIR            QString("LOG/")         //>@存放Log文件
#define D_TMPDIR            QString("TEMP/")        //>@存放临时文件
#define D_BOOTDIR           QString("BOOT/")        //>@存放启动文件（U盘设备文件、MDP、LOGCONF等）

#ifdef WINDOWS
#define D_FSRCPATH              QCoreApplication::applicationDirPath() + QString("/RuntimeRC/FSRC/")    //>@FLASH中的出厂资源路径
#define D_BST_DISPLAY           QCoreApplication::applicationDirPath() + QString("/BST_DISPLAY.exe")    //>@
#define D_BST_START             QCoreApplication::applicationDirPath() + QString("/BST_START.exe")     //>@FLASH中的出厂资源路径
#define D_MENCODER              D_FSRCPATH + QString("TOOL/Windows/mencoder.exe")
#define D_FFMPEG_PATH           D_FSRCPATH + QString("TOOL/Windows/ffmpeg.exe")
#define D_FFSERVER_PATH         D_FSRCPATH + QString("TOOL/Windows/ffserver.exe")
#define D_MPLAYER_PATH          D_FSRCPATH + QString("TOOL/Windows/mplayer.exe")
#define D_GSTREAMER_PATH        D_FSRCPATH + QString("TOOL/Windows/gst-launch.exe")
#endif
#ifdef UBUNTU
#define D_FSRCPATH              QCoreApplication::applicationDirPath() + QString("/RuntimeRC/FSRC/")    //>@FLASH中的出厂资源路径
#define D_BST_DISPLAY           QCoreApplication::applicationDirPath() + QString("/BST_DISPLAY")        //>@
#define D_BST_START             QCoreApplication::applicationDirPath() + QString("/BST_START")         //>@FLASH中的出厂资源路径
#define D_MENCODER              D_FSRCPATH + QString("TOOL/Ubuntu/mencoder")
#define D_FFMPEG_PATH           D_FSRCPATH + QString("TOOL/Ubuntu/ffmpeg")
#define D_FFSERVER_PATH         D_FSRCPATH + QString("TOOL/Ubuntu/ffserver")
#define D_MPLAYER_PATH          D_FSRCPATH + QString("TOOL/Ubuntu/mplayer")
#define D_GSTREAMER_PATH        D_FSRCPATH + QString("TOOL/Ubuntu/gst-launch")
#endif
#ifdef LINUX
#define D_FSRCPATH              QCoreApplication::applicationDirPath() + QString("/../")         //>@FLASH中的出厂资源路径
#define D_BST_DISPLAY           D_FSRCPATH + D_EXEDIR + QString("BST_DISPLAY")    //>@
#define D_BST_START             D_FSRCPATH + D_EXEDIR + QString("BST_START")
#define D_MENCODER              D_FSRCPATH + D_SYSTEMDIR + D_MIDDLEWAREDIR + QString("MPlayer/mencoder")
#define D_FFMPEG_PATH           D_FSRCPATH + D_SYSTEMDIR + D_MIDDLEWAREDIR + QString("ffmpeg/ffmpeg")
#define D_FFSERVER_PATH         D_FSRCPATH + D_SYSTEMDIR + D_MIDDLEWAREDIR + QString("ffmpeg/ffserver")
#define D_MPLAYER_PATH          D_FSRCPATH + D_SYSTEMDIR + D_MIDDLEWAREDIR + QString("MPlayer/mplayer")
#define D_GSTREAMER_PATH        D_FSRCPATH + D_SYSTEMDIR + D_MIDDLEWAREDIR + QString("Gstreamer/gst-launch")
#endif
#ifdef ANDROID
#define D_FSRCPATH              QString("/mnt/sdcard/data/bst/")         //>@FLASH中的出厂资源路径
#define D_BST_DISPLAY           D_FSRCPATH + D_EXEDIR + QString("BST_DISPLAY")    //>@
#define D_BST_START             D_FSRCPATH + D_EXEDIR + QString("BST_START")
#define D_MENCODER              D_FSRCPATH + D_SYSTEMDIR + D_MIDDLEWAREDIR + QString("MPlayer/mencoder")
#define D_FFMPEG_PATH           D_FSRCPATH + D_SYSTEMDIR + D_MIDDLEWAREDIR + QString("ffmpeg/ffmpeg")
#define D_FFSERVER_PATH         D_FSRCPATH + D_SYSTEMDIR + D_MIDDLEWAREDIR + QString("ffmpeg/ffserver")
#define D_MPLAYER_PATH          D_FSRCPATH + D_SYSTEMDIR + D_MIDDLEWAREDIR + QString("MPlayer/mplayer")
#define D_GSTREAMER_PATH        D_FSRCPATH + D_SYSTEMDIR + D_MIDDLEWAREDIR + QString("Gstreamer/gst-launch")
#endif
#ifdef IOS
#define D_FSRCPATH              QString("../Document/")         //>@FLASH中的出厂资源路径
#define D_BST_DISPLAY           D_FSRCPATH + D_EXEDIR + QString("BST_DISPLAY")    //>@
#define D_BST_START             D_FSRCPATH + D_EXEDIR + QString("BST_START")
#define D_MENCODER              D_FSRCPATH + D_SYSTEMDIR + D_MIDDLEWAREDIR + QString("MPlayer/mencoder")
#define D_FFMPEG_PATH           D_FSRCPATH + D_SYSTEMDIR + D_MIDDLEWAREDIR + QString("ffmpeg/ffmpeg")
#define D_FFSERVER_PATH         D_FSRCPATH + D_SYSTEMDIR + D_MIDDLEWAREDIR + QString("ffmpeg/ffserver")
#define D_MPLAYER_PATH          D_FSRCPATH + D_SYSTEMDIR + D_MIDDLEWAREDIR + QString("MPlayer/mplayer")
#define D_GSTREAMER_PATH        D_FSRCPATH + D_SYSTEMDIR + D_MIDDLEWAREDIR + QString("Gstreamer/gst-launch")
#endif
#ifdef BBOS
#define D_FSRCPATH              QString("/usr/bst/")         //>@FLASH中的出厂资源路径
#define D_BST_DISPLAY           D_FSRCPATH + D_EXEDIR + QString("BST_DISPLAY")    //>@
#define D_BST_START             D_FSRCPATH + D_EXEDIR + QString("BST_START")
#define D_MENCODER              D_FSRCPATH + D_SYSTEMDIR + D_MIDDLEWAREDIR + QString("MPlayer/mencoder")
#define D_FFMPEG_PATH           D_FSRCPATH + D_SYSTEMDIR + D_MIDDLEWAREDIR + QString("ffmpeg/ffmpeg")
#define D_FFSERVER_PATH         D_FSRCPATH + D_SYSTEMDIR + D_MIDDLEWAREDIR + QString("ffmpeg/ffserver")
#define D_MPLAYER_PATH          D_FSRCPATH + D_SYSTEMDIR + D_MIDDLEWAREDIR + QString("MPlayer/mplayer")
#define D_GSTREAMER_PATH        D_FSRCPATH + D_SYSTEMDIR + D_MIDDLEWAREDIR + QString("Gstreamer/gst-launch")
#endif

//>@__PRETTY_FUNCTION__ 用于打印函数名
#define D_CODE_LOCATE        (QString("[%1,%2(),Line %3]").arg(__FILE__).arg(__FUNCTION__).arg(__LINE__))

#define _DEBUG_

#ifdef _DEBUG_
#define IDE_DEBUG(x)         qDebug() << (QString("%1-%2").arg(D_CODE_LOCATE).arg(QString(x)).toStdString().c_str())
#define IDE_TRACE()			 qDebug() << (QString("%1-Trace").arg(D_CODE_LOCATE).toStdString().c_str())
#define IDE_TRACE_INT(x)	 qDebug() << (QString("%1-%2=%3").arg(D_CODE_LOCATE).arg(#x).arg((int)x).toStdString().c_str())
#define IDE_TRACE_STR(x)	 qDebug() << (QString("%1-%2=%3").arg(D_CODE_LOCATE).arg(#x).arg(x).toStdString().c_str())
#define IDE_PRINTF(fmt, ...) qDebug() << ("[%s,%s(),Line %s]-"#fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define IDE_DEBUG(x)
#define IDE_TRACE()
#define IDE_TRACE_INT(x)
#define IDE_TRACE_STR(x)
#define IDE_PRINTF(fmt, ...)
#endif

#define GBKToUnicode(x)    QTextCodec::codecForName("GBK")->toUnicode(x)
#define GBKToUTF8(x)       QTextCodec::codecForName("UTF-8")->fromUnicode(GBKToUnicode(x)).data()

#ifdef UNICODE
#define ToMfcStr(x)         (WCHAR*)(QString(x)).utf16()
#else
#define ToMfcStr(x)         (QString(x)).toLocal8Bit()
#endif

#define CLEAR(x)            memset(&(x), 0, sizeof(x))

//-----------------------------------------------------------------------------------------

#define D_CHG_MODE          0x00000001
#define D_CHG_STATE         0x00000002
#define D_CHG_MODE_STATE    0x00000003

#define D_MAXTIMEOUT        0x7fffffff

typedef enum{
    WS_UNINIT = 0,
    WS_LAUNCH = 0x00000001,     //启动过程
    WS_LOADED = 0x00000002,    //设备加载完
    WS_INITED = 0x00000004,     //--设备与UI均初始化好，但还没有进入正常模式，比如CANOPEN的preoperation模式

    WS_IDLE = 0x00000008,       //--正常工作/空闲模式
    WS_RUNNING = 0x00000010,    //--忙模式
    WS_PAUSED = 0x00000020,     //--暂停模式
    WS_STOPPED = 0x00000040,    //--停止模式

    WS_MSGPRG = 0x00000080,     //消息进度模式
    WS_UPGRADE = 0x00000100,    //升级模式
    WS_SETUP = 0x00000200,      //设置模式
    WS_SRNSAVE = 0x00000400,    //屏保模式
    WS_MSGWIN = 0x00000800      //消息窗模式
}E_WORK_STATE;

Q_DECLARE_METATYPE(E_WORK_STATE)
inline uint qHash(E_WORK_STATE pType) { return (uint)pType; }

typedef enum{
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
}E_RC_INDEX;

Q_DECLARE_METATYPE(E_RC_INDEX)
inline uint qHash(E_RC_INDEX pType) { return (uint)pType; }

typedef enum {
    RC_UNKNOW = 0,
    RC_BMP,
    RC_JPG,
    RC_PNG,
    RC_GIF,
    RC_SVG,
    //
    RC_MP3,
    RC_WAV,
    //
    RC_MP4,
    RC_AVI,
    RC_RMVB,
    RC_FLV,
    //
    RC_FONT,
    RC_QML,
    RC_3DS,
    RC_QRC,
    RC_QUI,
    //
    RC_HTML,
    RC_XML,
    RC_INI,
    RC_LOG,
    //
    RC_MDP,
    RC_UI,
    RC_MAP,
    RC_PRO
}RC_TYPE;

inline RC_TYPE GetRcType(QString FileName)
{
    if (FileName.isEmpty())
        return RC_UNKNOW;
    if(FileName.endsWith(".bmp",Qt::CaseInsensitive))
        return RC_BMP;
    else if(FileName.endsWith(".jpg",Qt::CaseInsensitive))
        return RC_JPG;
    else if(FileName.endsWith(".png",Qt::CaseInsensitive))
        return RC_PNG;
    else if(FileName.endsWith(".gif",Qt::CaseInsensitive))
        return RC_GIF;
    else if(FileName.endsWith(".svg",Qt::CaseInsensitive))
        return RC_SVG;
    else if(FileName.endsWith(".qml",Qt::CaseInsensitive))
        return RC_QML;
    else if(FileName.endsWith(".ui",Qt::CaseInsensitive))
        return RC_QUI;
    else if(FileName.endsWith(".qrc",Qt::CaseInsensitive))
        return RC_QRC;
    else if(FileName.endsWith(".avi",Qt::CaseInsensitive))
        return RC_AVI;
    else if(FileName.endsWith(".wav",Qt::CaseInsensitive))
        return RC_WAV;
    else if(FileName.endsWith(".mp3",Qt::CaseInsensitive))
        return RC_MP3;
    else if(FileName.endsWith(".mp4",Qt::CaseInsensitive))
        return RC_MP4;
    else if(FileName.endsWith(".flv",Qt::CaseInsensitive))
        return RC_FLV;
    else if(FileName.endsWith(".rmvb",Qt::CaseInsensitive))
        return RC_RMVB;
    else if(FileName.endsWith(".font",Qt::CaseInsensitive))
        return RC_FONT;
    else if(FileName.endsWith(".3ds",Qt::CaseInsensitive))
        return RC_3DS;
    else if(FileName.endsWith(".html",Qt::CaseInsensitive))
        return RC_HTML;
    else if(FileName.endsWith(".xml",Qt::CaseInsensitive))
        return RC_XML;
    else if(FileName.endsWith(".ini",Qt::CaseInsensitive))
        return RC_INI;
    else if(FileName.endsWith(".log",Qt::CaseInsensitive))
        return RC_LOG;
    else if(FileName.endsWith(".qui",Qt::CaseInsensitive))
        return RC_QUI;
    else if(FileName.endsWith(".mdp",Qt::CaseInsensitive))
        return RC_MDP;
    else if(FileName.endsWith(".ui",Qt::CaseInsensitive))
        return RC_UI;
    else if(FileName.endsWith(".map",Qt::CaseInsensitive))
        return RC_MAP;
    else if(FileName.endsWith(".pro",Qt::CaseInsensitive))
        return RC_PRO;
    return RC_UNKNOW;
}

typedef enum{
    E_NUM_0=0,
    E_NUM_1,
    E_NUM_2,
    E_NUM_3,
    E_NUM_4,
    E_NUM_5,
    E_NUM_6,
    E_NUM_7,
    E_NUM_8,
    E_NUM_9,
    E_NUM_A,
    E_NUM_B,
    E_NUM_C,
    E_NUM_D,
    E_NUM_E,
    E_NUM_F,
    E_NUM_G,
    E_NUM_H,
    E_NUM_I,
    E_NUM_J,
    E_NUM_K,
    E_NUM_L,
    E_NUM_M,
    E_NUM_N,
    E_NUM_O,
    E_NUM_P,
    E_NUM_Q,
    E_NUM_R,
    E_NUM_S,
    E_NUM_T,
    E_NUM_U,
    E_NUM_V,
    E_NUM_W,
    E_NUM_X,
    E_NUM_Y,
    E_NUM_Z
}E_NUM_INDEX;

typedef enum{
    E_CHAR_0='0',
    E_CHAR_1,
    E_CHAR_2,
    E_CHAR_3,
    E_CHAR_4,
    E_CHAR_5,
    E_CHAR_6,
    E_CHAR_7,
    E_CHAR_8,
    E_CHAR_9,
    E_CHAR_A='A',
    E_CHAR_B,
    E_CHAR_C,
    E_CHAR_D,
    E_CHAR_E,
    E_CHAR_F,
    E_CHAR_G,
    E_CHAR_H,
    E_CHAR_I,
    E_CHAR_J,
    E_CHAR_K,
    E_CHAR_L,
    E_CHAR_M,
    E_CHAR_N,
    E_CHAR_O,
    E_CHAR_P,
    E_CHAR_Q,
    E_CHAR_R,
    E_CHAR_S,
    E_CHAR_T,
    E_CHAR_U,
    E_CHAR_V,
    E_CHAR_W,
    E_CHAR_X,
    E_CHAR_Y,
    E_CHAR_Z
}E_CHAR_INDEX;

typedef enum{
    AO_LIFTFLR=0,
    AO_LIFTARV=1,
    AO_BEEP=2,
    AO_VIDEO=3,
    //>@
    AO_ADMUSIC=4,
    AO_APSRC,      //>@音频对讲采集及发送
    AO_APDST,      //>@音频对讲接收并显示
    AO_VPSRC,      //>@视频对讲采集及发送
    AO_VPDST,      //>@视频对讲接收并显示
    AO_NONE
} AO_TYPE;

typedef enum{
    AO_IDEL=0,
    AO_STARTING,
    AO_PLAYING,
    AO_PAUSE,
    AO_ABORT,
    AO_STOP,
    //
    AO_RCERROR,
    AO_DRVERROR,
    AO_FMTERROR,
    AO_MEMERROR,
    AO_FINISHED,
    AO_UNKNOWERROR
} AO_MODE;

typedef enum{
    DS_OFF = 0x00000001,     //>@离线状态
    DS_ON = 0x00000002,      //>@在线
    DS_ERR = 0x00000004,     //>@错误状态
    DS_LOCK = 0x00000008,    //>@锁定状态
    DS_NACK = 0x00000010,    //>@设备有应答
    DS_ACK = 0x00000020     //>@设备有应答
}E_DEV_STATE;

#define D_STATECODE         QString("SC")

//>@获取设备状态，返回是否Plug
inline E_DEV_STATE GetDevStatus(QDomElement pDevElement)
{
    E_DEV_STATE tmpState = DS_OFF;
    if(pDevElement.isNull())
        return tmpState;
    QString tmpDevStatus = pDevElement.attribute(D_STATECODE);
    if(tmpDevStatus.compare("OFF", Qt::CaseInsensitive) == 0)
        tmpState = DS_OFF;
    else if(tmpDevStatus.compare("ON", Qt::CaseInsensitive) == 0)
        tmpState = DS_ON;
    else if(tmpDevStatus.compare("LOCK", Qt::CaseInsensitive) == 0)
        tmpState = DS_LOCK;
    else if(tmpDevStatus.compare("ERR", Qt::CaseInsensitive) == 0)
        tmpState = DS_ERR;
    else if(tmpDevStatus.compare("ACK", Qt::CaseInsensitive) == 0)
        tmpState = DS_ACK;
    else if(tmpDevStatus.compare("NACK", Qt::CaseInsensitive) == 0)
        tmpState = DS_NACK;
    else if(tmpDevStatus.compare("LOCK", Qt::CaseInsensitive) == 0)
        tmpState = DS_LOCK;
    return tmpState;
}

inline E_DEV_STATE GetDevStatus(QDomElement pDevElement, QString &pDevPath)
{
    if(pDevElement.isNull())
        pDevPath.clear();
    else
        pDevPath = pDevElement.text();
    return GetDevStatus(pDevElement);
}

//>@组件类型
typedef enum{
    UI_INVALID=0,
    UI_FRAME,
    UI_LEFTFRAME,
    UI_TOPFRAME,
    UI_RIGHTFRAME,
    UI_BOTTOMFRAME,
    UI_NOTE,
    //>@-------------桌面
    UI_DESKTOP,
    UI_AREA,
    //>@
    UI_SETBAR,
    UI_MESSAGEBAR,
    UI_PROGRESSBAR,
    UI_SRNSAVEFORM,
    UI_ANIMAL,
    //>@Function Info Reginon
    UI_FLOOR,
    UI_DTNELE,
    UI_FLRSIGN,
    UI_ARROW,
    UI_FUNCTION,
    //>@MultiMedia Reginon
    UI_MEDIA,
    UI_IMAGE,  //>@照片墙
    UI_PICTURE,
    UI_VIDEO,
    UI_WEBVIEW,
    UI_PHONE,
    //>@Text Bulletin Reginon
    UI_TEXT,
    UI_TITLE,
    //>@Integrated Info Reginon
    UI_LOGO,
    UI_WEATHER,
    UI_CLOCK,
    UI_DATE,
    UI_TIME,
    UI_NAMEPLATE,
    UI_RENCODE,
    //>@Button Operate Reginon
    UI_FLRBTN,
    UI_FLRLCD,
    UI_KEYBOARDBTN,    //>@键盘
    UI_CALLBTN,        //>@外呼按钮
    UI_DOORBTN,        //>@门机按钮
    UI_EMERGENCYBTN,   //>@警铃按钮
    UI_TALKBACKBTN,    //>@对讲按钮
    UI_SETBTN,
    UI_PHONEBTN,
    UI_DISABILITYBTN,
    //>@检修
    UI_STATUS,
    //>@-------------以下为组件
    UI_COMPONENT,
    UI_LINE,
    UI_LABEL,
    UI_STRING,
    UI_CHECK,
    UI_BOOL,
    UI_ENUM,
    UI_NUM,
    UI_BUTTON,
    UI_LINK,
    UI_ENETADDR,
    UI_TIMEEDIT
} UI_TYPE;

Q_DECLARE_METATYPE(UI_TYPE)
inline uint qHash(UI_TYPE pType) { return (uint)pType; }

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
    P_Elevator,
    P_DIAL,  //>@必须在最后
    //>@具体接口
    P_WIFI,
    P_CAN,
    P_485,
    P_SPI,
    P_COM,
    P_SAF,
    P_UDP,
    P_TCP,
    P_TEST
} PH_Type;

Q_DECLARE_METATYPE(PH_Type)
inline uint qHash(PH_Type pType) { return (uint)pType; }

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
    else if(pPara.compare("Elevator", Qt::CaseInsensitive) == 0)
        PHType = P_Elevator;
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

Q_DECLARE_METATYPE(MW_Type)
inline uint qHash(MW_Type pType) { return (uint)pType; }

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
} E_DEV_TYPE;

inline E_DEV_TYPE GetDevType(QString pDevName)
{
    if(pDevName.contains("Physical"))
        return DEV_PHYSICAL;
    else if(pDevName.contains("Logic"))
        return DEV_LOGIC;
    else if(pDevName.contains("Middleware"))
        return DEV_MIDDLEWARE;
    return DEV_NONE;
}

inline QString GetDevTypeString(E_DEV_TYPE pDevType)
{
    if(pDevType == DEV_PHYSICAL)
        return QString("Physical");
    else if(pDevType == DEV_LOGIC)
        return QString("Logic");
    else if(pDevType == DEV_MIDDLEWARE)
        return QString("Middleware");
    return QString();
}

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

#define MSG_PERMANENT   10000000
#define MSG_TEMPORARY   10000001
#define MSG_HOLD        10000002

typedef enum{
    ML_INFO = 0,
    ML_WARN,
    ML_ERROR,
    ML_SUCCESS,
    ML_QUESTION,
    ML_INVALID = 100
} E_MSG_LEVEL;

inline E_MSG_LEVEL GetMsgLevel(QString pLevel)
{
    if(!pLevel.compare("Info", Qt::CaseInsensitive))
        return ML_INFO;
    else if(!pLevel.compare("Warn", Qt::CaseInsensitive))
        return ML_WARN;
    else if(!pLevel.compare("Error", Qt::CaseInsensitive))
        return ML_ERROR;
    return ML_INVALID;
}

inline QString GetMsgLevel(E_MSG_LEVEL pLevel)
{
    if(pLevel == ML_INFO)
        return QString("Info");
    else if(pLevel == ML_WARN)
        return QString("Warn");
    else if(pLevel == ML_ERROR)
        return QString("Error");
    return QString();
}

class DualValue
{
public:
    DualValue()
    {
        mX = mY = 0;
    }
    DualValue(QString pText, QString pSign = QString("x"))
    {
        fromString(pText, pSign);
    }
    DualValue(quint32  pX, quint32 pY)
    {
        mX = pX;
        mY = pY;
    }
    QString toString(QString pSign = QString("x"))
    {
        return QString("%1%2%3").arg(mX).arg(pSign).arg(mY);
    }
    void fromString(QString pText, QString pSign = QString("x"))
    {
        QStringList tmpAddrList = pText.split(pSign);
        if(tmpAddrList.count() != 2)
        {
            mX = mY = 0;
        }
        else
        {
            mX = tmpAddrList.at(0).toInt();
            mY = tmpAddrList.at(1).toInt();
        }
    }
    QSize toSize()
    {
        return QSize(mX, mY);
    }
    QSizeF toSizeF()
    {
        return QSizeF(mX, mY);
    }
    bool isValid()
    {
        if(mX <= 0 || mY <= 0)
            return false;
        return true;
    }
    bool operator==(const DualValue &s)
    {
        if(mX != s.mX || mY != s.mY)
            return false;
        return true;
    }
    bool operator!=(const DualValue &s)  { return !operator==(s); }
    DualValue & operator=(const DualValue &s)
    {
        mX = s.mX;
        mY = s.mY;
        return *this;
    }
    DualValue & operator=(const QString &s)
    {
        fromString(s);
        return *this;
    }
public:
    quint32  mX, mY;
};

typedef DualValue   UD_SIZE;

class QuadValue
{
public:
    QuadValue()
    {
        for(int i=0;i<4;i++)
            mAddr[i] = 0;
    }
    QuadValue(QString pText, QString pSign = QString("."))
    {
        fromString(pText, pSign);
    }
    QuadValue(quint32 pValue0, quint32 pValue1, quint32 pValue2, quint32 pValue3)
    {
        mAddr[0] = pValue0;
        mAddr[1] = pValue1;
        mAddr[2] = pValue2;
        mAddr[3] = pValue3;
    }
    QuadValue(quint8 *pAddr)
    {
        for(int i=0;i<4;i++)
            mAddr[i] = pAddr[i];
    }
    QString toString(QString pSign = QString("."))
    {
        return QString("%1%5%2%5%3%5%4").arg(mAddr[0]).arg(mAddr[1]).arg(mAddr[2]).arg(mAddr[3]).arg(pSign);
    }
    void fromString(QString pText, QString pSign = QString("."))
    {
        QStringList tmpAddrList = pText.split(pSign);
        if(tmpAddrList.count() != 4)
        {
            for(int i=0;i<4;i++)
                mAddr[i] = 0;
        }
        else
        {
            for(int i=0;i<4;i++)
                mAddr[i] = tmpAddrList.at(i).toInt();
        }
    }
    bool isValid()
    {
        for(int i=0;i<4;i++)
        {
            if(mAddr[i] != 0)
                return true;
        }
        return false;
    }
    bool operator==(const QuadValue &s)
    {
        for(int i=0;i<4;i++)
        {
            if(this->mAddr[i] != s.mAddr[i])
                return false;
        }
        return true;
    }
    bool operator!=(const QuadValue &s)  { return !operator==(s); }
    QuadValue & operator=(const QuadValue &s)
    {
        for(int i=0;i<4;i++)
        {
            this->mAddr[i] = s.mAddr[i];
        }
        return *this;
    }
    QuadValue & operator=(const QString &s)
    {
        fromString(s);
        return *this;
    }
public:
    quint32  mAddr[4];
};

typedef QuadValue   UD_IPADDR;

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

typedef enum{
    Failed = 0,
    Success,
    Start,
    Timeout,
    Stopped,
    Unstart
}TaskState;

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

inline ParityType GetParityType(QString pType)
{
    if(!pType.compare("ODD", Qt::CaseInsensitive))
        return PAR_ODD;
    else if(!pType.compare("EVEN", Qt::CaseInsensitive))
        return PAR_EVEN;
    return PAR_NONE;
}

inline QString GetParityTypeString(ParityType pType)
{
    if(pType == PAR_ODD)
        return QString("ODD");
    else if(pType == PAR_EVEN)
        return QString("EVEN");
    return QString();
}

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

typedef struct
{
    BaudRateType    BaudRate;
    DataBitsType    DataBits;
    ParityType      Parity;
    StopBitsType    StopBits;
    FlowType        FlowControl;
    long            Timeout_Millisec;
} PortSettings;

//>@重启
inline bool Reboot(bool pSync = true)
{
#if (defined(UBUNTU) || defined(LINUX))
    if(pSync)
    {
        sync(); // 同步磁盘数据,将缓存数据回写到硬盘,以防数据丢失
        QtSleep(100);
    }
    return reboot(RB_AUTOBOOT);
#else
    Q_UNUSED(pSync);
    qApp->quit();
    QProcess::startDetached(qApp->applicationFilePath(), qApp->arguments());
    return true;
#endif
}

//>@退出
inline void Exit(int pRetCode, bool pSync = true)
{
#if (defined(UBUNTU) || defined(LINUX))
    if(pSync)
        sync(); // 同步磁盘数据,将缓存数据回写到硬盘,以防数据丢失
#else
    Q_UNUSED(pSync);
#endif
    QCoreApplication::exit(pRetCode);
}

#ifdef IOS
#include "ocniclass.h"
#endif


#define LineLen 8       //线长
#define LinePulse 10    //线宽
#define RectPulse 5     //内框和外框的间隔

#define D_SCANIPFREQ    2000

#define D_DaemonHBTime        20000

#define D_WAITTIME      5000

#define D_TIPTEXT       QString("Text")

#define D_CMDSUBSPLIT  QString("#")

/*
    DISPLAY目录结构
    FSRC
    |---BOOT
    |---EXE
    |---LOG
    |---TEMP
    |---TEST
    |---THEME
    |---SYSTEM
        |---Physical
        |---Middleware
        |---Logic
*/

#define D_ENVIR_DISPLAY     QString("QWS_DISPLAY")
#define D_ENVIR_SIZE        QString("QWS_SIZE")

#define D_BURNIMG_PATH      QString("BurnImg")
#define D_ROOTFS_PATH       QString("RootFS")
#define D_RUNTIME_PATH      QString("RuntimeRC/")

#define D_SYSTEMDIR         QString("SYSTEM/")      //>@存放物理、逻辑以及中间件
#define D_THEMEDIR          QString("THEME/")       //>@存放所有主题
#define D_EXEDIR            QString("EXE/")         //>@存放所有应用程序
#define D_LOGDIR            QString("LOG/")         //>@存放Log文件
#define D_TMPDIR            QString("TEMP/")        //>@存放临时文件
#define D_BOOTDIR           QString("BOOT/")        //>@存放启动文件（U盘设备文件、MDP、LOGCONF等）
#define OUTPUTDIR           QString("OUTPUT")

#define D_SYSBIN_DIR        QString("SysBin/")

#define D_UI_DIR           QString("COM/")

#define D_PHYSICALDIR       QString("Physical/")
#define D_LOGICDIR          QString("Logic/")
#define D_MIDDLEWAREDIR     QString("Middleware/")

#define RESOURCESDIR        QString("Resources")

#define D_TMPSUFFIX         QString(".tmp")

//>@默认主题
#ifdef SODIMAS
#define D_DEFAULTTHEMEDIR   QString("BACKUP/")
#else
#define D_DEFAULTTHEMEDIR   QString("BST/")      //>@存放默认主题
#endif

#define D_TEMP_PATH         D_TMPDIR

//>@升级目录结构
#define D_DISK_CMD         QString("BST.cmd")
#define D_DISK_FS          QString("DISK/")
#define D_DISK_COM         QString("COM/")
#define D_DISK_RC_LOGO     QString("COM/LOGO/")
#define D_DISK_RC_VIDEO    QString("COM/VIDEO/")
#define D_DISK_RC_IMAGE    QString("COM/IMAGE/")
#define D_DISK_RC_TEXT     QString("COM/TEXT/")
#define D_DISK_BACKUP      QString("BACKUP/")

//>@SETTING目录结构
#define D_DEV_DIR               QString("DEV/")
#define D_DEV_OFFLINE_DIR       QString("OFFLINE/")
#define D_DEV_ONLINE_DIR        QString("ONLINE/")
#define D_DEV_FACTORY_DIR       QString("FACTORY/")

#define D_ETC_DIR               QString("ETC/")
#define D_ETC_BOOT_FILE         QString("boot.ini")

#define D_UPGRADE_DIR           QString("UPGRADE/")
#define D_TEST_DIR              QString("TEST/")

#define D_LOGO_DIR              QString("LOGO/")

//>@关键文件
#define D_BOOTPARABIN       QString("bootpara.bin")
#define D_ENVIRONMENT       QString("Scene.env")
#define D_SERVERNAME        QString("Server.sev")
#define D_MDPNAME           QString("Device.mdp")
#ifdef SUPPORT_PLUGIN
#ifdef WINDOWS
#define D_UINAME    QString("BST_PLUGIN.dll")
#else
#define D_UINAME    QString("libBST_PLUGIN.so")
#endif
#else
#define D_UINAME        QString("Theme.ui")
#endif
#define D_MAPNAME           QString("Event.map")
#define D_STCNAME           QString("Software.stc")
#define D_STONAME           QString("Chart.sto")

#define D_INTERFACEBIN      QString("Interface.bin")

#define D_BOOTFILE          D_MDPNAME
#define D_BKBOOTFILE        QString("BOOT.default")
#define D_SCHEDULEMAP       D_MAPNAME
#define D_TIPSXML           QString("TIPS.xml")
#define D_LOGCONF           QString("LOG.conf")
#define D_LOGBKCONF         QString("LOG.default")
#define D_LOGFILE           QString("BST_%1.log")

#define D_LISTITEM_NAME  10000
#define D_LISTITEM_PATH  10001

//>@---------------测试函数-------------------
//>@TestBench
#define FUNCTIONFLAG            "function"
#define BRACKETFLAG             "("
#define RESULTSUFFIX            "_result"

#endif

