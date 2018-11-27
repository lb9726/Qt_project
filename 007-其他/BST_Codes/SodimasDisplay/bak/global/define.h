#ifndef DEFINE_H
#define DEFINE_H

#define _DEBUG_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <errno.h>
#include <fcntl.h>
#include <assert.h>
#include <time.h>

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QDomDocument>
#include <QTextStream>
#include <QTextCodec>
#include <QFileInfo>
#include <QUrl>

#define QTASCII 0            //>@ 是否使用QT定义的ASCII码表

#if(QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
#define VAR_CAST   qobject_cast
#else
#define VAR_CAST   static_cast
#endif

#define GBKToUnicode(x)                         QTextCodec::codecForName("GBK")->toUnicode(x)
#define GBKToUTF8(x)                            QTextCodec::codecForName("UTF-8")->fromUnicode(GBKToUnicode(x)).data()

#ifdef UNICODE
#define ToMfcStr(x)  (WCHAR*)(QString(x)).utf16()
#else
#define ToMfcStr(x)  (QString(x)).toLocal8Bit()
#endif

#define AUDIO_STARTUP       QString(":/SystemAudio/rc/SystemAudio/open.wav")   //>@开机音乐
#define AUDIO_PLUGIN        QString(":/SystemAudio/rc/SystemAudio/plugin.wav") //>@可移动设备插上
#define AUDIO_PLUGOUT       QString(":/SystemAudio/rc/SystemAudio/plugout.wav")//>@可移动设备拔下
#define AUDIO_ERROR         QString(":/SystemAudio/rc/SystemAudio/error.wav")  //>@错误音

#define CLEAR(x)            memset(&(x), 0, sizeof(x))

#define D_NMPORT        60001   //>@网络管理端口号
#define D_CMDPORT       60002   //>@网络命令行端口号
#define D_SQLPORT       60003
#define D_WEBPORT       60010   //>@WEB端口号

#define D_VPVPORT       60006   //>@视频对讲视频端口
#define D_VPAPORT       60007   //>@视频对讲音频端口

#define D_SCANIPFREQ    2000

#define D_DaemonHBTime        20000

#define D_WAITTIME      5000

#define D_PARAMETER     QString("Parameter")
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

#define D_COM_DIR           QString("COM/")

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

#define D_GrabScreenPath    (D_FSRCPATH + D_TEMP_PATH)
#define D_GrabScreenName    QString("GrabScreen.JPG")

#define D_PCSCA_PATH        QString("pcsca")
#define D_DHCPCONF          QString("udhcpd.conf")
#define D_FFSERVERCONF      QString("ffserver.conf")
#define D_WIFICONF          QString("/etc/Wireless/RT2870AP/RT2870AP.dat")
#define D_DNSCONF           QString("/etc/resolv.conf")

/***************************************************/
#ifdef SODIMAS
#define D_COMFLOOR_PREFIX          QString("RC")
#define D_COMARROW_PREFIX          QString("RC")
#define D_COMFUNCTION_PREFIX       QString("RC")
#define D_COMAUDIO_PREFIX          QString("RC")
#define D_COMDESKTOP_PREFIX        QString("RC")
#endif

/**
//>@BeepSound
*/
#define D_Error             QString("Error")
#define D_Warning           QString("Warning")
#define D_Success           QString("Success")
#define D_Failed            QString("Failed")

#define D_StartUI           QString("StartUi")
#define D_PauseUI           QString("PauseUi")
#define D_ResumeUI          QString("ResumeUi")
#define D_RebootUI          QString("RebootUi")

#define D_KeyPress          QString("KeyPress")
#define D_NFDConn           QString("NFDConn")
#define D_NFDDisconn        QString("NFDDisconn")
#define D_UdiskPush         QString("UdiskPush")
#define D_UdiskPull         QString("UdiskPull")
#define D_NFCBrushCard      QString("NFCBrushCard")

#define D_UpGradeFailed     QString("UpGradeFailed")
#define D_UpGradeSucess     QString("UpGradeSucess")

#define D_NETERROR          QString("no such device")
#define D_GATEWAYERROR      QString("Network is unreachable")

//#define D_NFCDevPluge       QString("UdiskPluge")
//#define D_NFCDevPull        QString("UdiskPull")
//#define D_CamPluge          QString("CamPluge")
//#define D_CamkPull          QString("CamPull")
//#define D_MousePluge        QString("MousePluge")
//#define D_MousePull         QString("MousePull")
//#define D_KeyboardPluge     QString("KeyboardPluge")
//#define D_KeyboardPull      QString("KeyboardPull")
//#define D_MousePluge        QString("MousePluge")
//#define D_MousePull         QString("MousePull")
//#define D_GprsPluge         QString("GprsPluge")
//#define D_GprsPull          QString("GprsPull")
//#define D_WifiPluge         QString("WifiPluge")
//#define D_WifiPull          QString("WifiPull")

#define PI 3.14159265358979323846264338327950288419717

#define MAX_TMPMCHARLEN			1024
#define MAX_TMPWCHARLEN			1024
#define MID_TMPMCHARLEN			256
#define MID_TMPWCHARLEN			256
#define MIN_TMPMCHARLEN			32
#define MIN_TMPWCHARLEN			32

#define ret_OK					 2
#define ret_ERROR 				-2

#define ret_INVALID				 255
#define ret_NULL				 0

#define ret_SUCCESS 			 1
#define ret_FAIL				-1

#define ret_FULL				 3
#define ret_EMPTY				-3

#define ret_BUSY				 4
#define ret_IDLE				-4

#define LineLen 8     //线长
#define LinePulse 10  //线宽
#define RectPulse 5  //内框和外框的间隔

//输入输出函数定义
#define MAX_SCANF_COUNT 10
typedef char			( *pInputCharFun )  ( void );
typedef unsigned char	( *pInputByteFun )  ( void );
typedef size_t			( *pInputStrFun )   (  char *, size_t  );
typedef size_t			( *pInputMbyteFun )	(  unsigned char *, size_t  );
typedef bool			( *pOutputCharFun ) ( char );
typedef bool			( *pOutputByteFun ) ( unsigned char );
typedef size_t			( *pOutputStrFun )  ( char *, size_t );
typedef size_t			( *pOutputMbyteFun )( unsigned char *, size_t );

//>@直接在应用程序处创建
inline bool WriteLog(QString pLog)
{
    if(pLog.isEmpty())
        return false;
    QString tmpLogFile = D_FSRCPATH + QString("/history.log");
    QFile tmpFile(tmpLogFile);
    tmpFile.setPermissions(QFile::WriteOther);
    QIODevice::OpenMode tmpMode;
    //>@判断是否超出最大大小(10MByte)
    if(tmpFile.size() > 10000000)
        tmpMode = QIODevice::WriteOnly;
    else
        tmpMode = QIODevice::Append;
    if(!tmpFile.open(tmpMode))
    {
        qDebug() << "open log error!";
        return false;
    }
    if(!pLog.endsWith("\n"))
        pLog.append("\n");
    tmpFile.write(pLog.toLatin1());
    tmpFile.close();
#ifdef LINUX
    system("sync");
#endif
    return true;
}

//>@__PRETTY_FUNCTION__ 用于打印函数名
#ifdef _DEBUG_
//#define IDE_DEBUG(x)         qDebug() << (QString("[%1,%2(),Line %3]-%4").arg(__FILE__).arg(__func__).arg(__LINE__).arg(QString(x)).toStdString().c_str())
//#define IDE_TRACE()			 qDebug() << (QString("[%1,%2(),Line %3]-Trace").arg(__FILE__).arg(__func__).arg(__LINE__).toStdString().c_str())
//#define IDE_TRACE_INT(x)	 qDebug() << (QString("[%1,%2(),Line %3]-%4=%5").arg(__FILE__).arg(__func__).arg(__LINE__).arg(#x).arg((int)x).toStdString().c_str())
//#define IDE_TRACE_STR(x)	 qDebug() << (QString("[%1,%2(),Line %3]-%4=%5").arg(__FILE__).arg(__func__).arg(__LINE__).arg(#x).arg(x).toStdString().c_str())
//#define IDE_PRINTF(fmt, ...) qDebug() << ("[%s,%s(),Line %s]-"#fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)


#define IDE_DEBUG(x)
#define IDE_TRACE()
#define IDE_TRACE_INT(x)
#define IDE_TRACE_STR(x)
#define IDE_PRINTF(fmt, ...)


#define L_IDE_DEBUG(x)         qDebug() << (QString("[%1,%2(),Line %3]-%4").arg(__FILE__).arg(__func__).arg(__LINE__).arg(QString(x)).toStdString().c_str())
#define L_IDE_TRACE()			 qDebug() << (QString("[%1,%2(),Line %3]-Trace").arg(__FILE__).arg(__func__).arg(__LINE__).toStdString().c_str())
#define L_IDE_TRACE_INT(x)	 qDebug() << (QString("[%1,%2(),Line %3]-%4=%5").arg(__FILE__).arg(__func__).arg(__LINE__).arg(#x).arg((int)x).toStdString().c_str())
#define L_IDE_TRACE_FLOAT(x)	 qDebug() << (QString("[%1,%2(),Line %3]-%4=%5").arg(__FILE__).arg(__func__).arg(__LINE__).arg(#x).arg((float)x).toStdString().c_str())
#define L_IDE_TRACE_STR(x)	 qDebug() << (QString("[%1,%2(),Line %3]-%4=%5").arg(__FILE__).arg(__func__).arg(__LINE__).arg(#x).arg(x).toStdString().c_str())
#define L_IDE_PRINTF(fmt, ...) qDebug() << ("[%s,%s(),Line %s]-"#fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)


#else
#define IDE_DEBUG(x)         qDebug() << (QString("[%1,%2(),Line %3]-%4").arg(__FILE__).arg(__func__).arg(__LINE__).arg(QString(x)))
#define IDE_TRACE()			 qDebug() << (QString("[%1,%2(),Line %3]-Trace").arg(__FILE__).arg(__func__).arg(__LINE__))
#define IDE_TRACE_INT(x)	 qDebug() << (QString("[%1,%2(),Line %3]-%4=%5").arg(__FILE__).arg(__func__).arg(__LINE__).arg(#x).arg((int)x))
#define IDE_TRACE_STR(x)	 qDebug() << (QString("[%1,%2(),Line %3]-%4=%5").arg(__FILE__).arg(__func__).arg(__LINE__).arg(#x).arg(x))
#define IDE_PRINTF(fmt, ...) qDebug() << (QString("[%s,%s(),Line %s]-"#fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__))
#endif

#define IDE_LOGGER(x)        IDE_DEBUG(x);WriteLog(QString("[%1,%2,%3(),Line %4]-%5").arg(qApp->applicationName()).arg(__FILE__).arg(__func__).arg(__LINE__).arg(QString(x)))

#endif // DEFINE_H


