#ifndef SERIALPORT_H
#define SERIALPORT_H

typedef char                    UD_INT8;
typedef unsigned char           UD_UINT8;
typedef short       			UD_INT16;
typedef unsigned short      	UD_UINT16;
typedef int                     UD_INT32;
typedef unsigned int          	UD_UINT32;
typedef long long               UD_INT64;
typedef unsigned long long      UD_UINT64;
typedef UD_INT32                UD_BOOL;

#define D_RET_OK    0
#define D_RET_FAIL  1

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <errno.h>
#include <fcntl.h>
#include <assert.h>
#include <time.h>

#define _DEBUG_
//>@__PRETTY_FUNCTION__ 用于打印函数名
#ifdef _DEBUG_
#define IDE_DEBUG(x)         qDebug() << (QString("[%1,%2(),Line %3]-%4").arg(__FILE__).arg(__func__).arg(__LINE__).arg(QString(x)).toStdString().c_str())
#define IDE_TRACE()			 qDebug() << (QString("[%1,%2(),Line %3]-Trace").arg(__FILE__).arg(__func__).arg(__LINE__).toStdString().c_str())
#define IDE_TRACE_INT(x)	 qDebug() << (QString("[%1,%2(),Line %3]-%4=%5").arg(__FILE__).arg(__func__).arg(__LINE__).arg(#x).arg((int)x).toStdString().c_str())
#define IDE_TRACE_FLOAT(x)	 qDebug() << (QString("[%1,%2(),Line %3]-%4=%5").arg(__FILE__).arg(__func__).arg(__LINE__).arg(#x).arg((float)x).toStdString().c_str())
#define IDE_TRACE_STR(x)	 qDebug() << (QString("[%1,%2(),Line %3]-%4=%5").arg(__FILE__).arg(__func__).arg(__LINE__).arg(#x).arg(x).toStdString().c_str())
#define IDE_PRINTF(fmt, ...) qDebug() << ("[%s,%s(),Line %s]-"#fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define IDE_DEBUG(x)         //GlobalPara.m_Logger->info(QString("[%1,%2(),Line %3]-%4").arg(__FILE__).arg(__func__).arg(__LINE__).arg(QString(x)))
#define IDE_TRACE()			 //GlobalPara.m_Logger->info(QString("[%1,%2(),Line %3]-Trace").arg(__FILE__).arg(__func__).arg(__LINE__))
#define IDE_TRACE_INT(x)	 //GlobalPara.m_Logger->info(QString("[%1,%2(),Line %3]-%4=%5").arg(__FILE__).arg(__func__).arg(__LINE__).arg(#x).arg((int)x))
#define IDE_TRACE_STR(x)	 //GlobalPara.m_Logger->info(QString("[%1,%2(),Line %3]-%4=%5").arg(__FILE__).arg(__func__).arg(__LINE__).arg(#x).arg(x))
#define IDE_PRINTF(fmt, ...) //GlobalPara.m_Logger->info(QString("[%s,%s(),Line %s]-"#fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__))
#endif

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
    BAUD125000 = 125000,
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

typedef struct
{
    BaudRateType    BaudRate;
    DataBitsType    DataBits;
    ParityType      Parity;
    StopBitsType    StopBits;
    FlowType        FlowControl;
    long            Timeout_Millisec;
} PortSettings;

#ifdef __cplusplus
}
#endif

#include <QtCore>
#include <QtGui>
#include <QIODevice>
#include <QFile>
#include <QThread>
#include <QMutex>
#include <QReadWriteLock>

//>@NodeSerial
#if (defined(PHONE) || defined(UBUNTU) || defined(LINUX))
#ifndef IOS
#define _POSIX_VDISABLE	((unsigned char)'\377')
#endif
#include <stdio.h>
#include <termios.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#elif WINDOWS
#include <windows.h>
#endif

//>@if all warning messages are turned off, flag portability warnings to be turned off as well
#ifdef _TTY_NOWARN_
    #define _TTY_NOWARN_PORT_
#endif
//>@macros for thread support
#define LOCK_MUTEX() mutex->lock()
#define UNLOCK_MUTEX() mutex->unlock()
//>@macros for warning and debug messages
#ifdef _TTY_NOWARN_PORT_
    #define TTY_PORTABILITY_WARNING(s)
#else
    #define TTY_PORTABILITY_WARNING(s) qWarning(s)
#endif
//>@_TTY_NOWARN_PORT_
#ifdef _TTY_NOWARN_
    #define TTY_WARNING(s)
#else
    #define TTY_WARNING(s) qWarning(s)
#endif
//>@_TTY_NOWARN_

inline void QtComSleep(unsigned int msec, QEventLoop::ProcessEventsFlags flags = QEventLoop::AllEvents)
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
#endif
    }
}

//>@line status constants
#define LS_CTS  0x01
#define LS_DSR  0x02
#define LS_DCD  0x04
#define LS_RI   0x08
#define LS_RTS  0x10
#define LS_DTR  0x20
#define LS_ST   0x40
#define LS_SR   0x80

//>@error constants
#define E_NO_ERROR                   0
#define E_INVALID_FD                 1
#define E_NO_MEMORY                  2
#define E_CAUGHT_NON_BLOCKED_SIGNAL  3
#define E_PORT_TIMEOUT               4
#define E_INVALID_DEVICE             5
#define E_BREAK_CONDITION            6
#define E_FRAMING_ERROR              7
#define E_IO_ERROR                   8
#define E_BUFFER_OVERRUN             9
#define E_RECEIVE_OVERFLOW          10
#define E_RECEIVE_PARITY_ERROR      11
#define E_TRANSMIT_OVERFLOW         12
#define E_READ_FAILED               13
#define E_WRITE_FAILED              14

QString GetSerialPort(QString num);
QString GetSerialPortNum(QString portname);

class CommPort : public QIODevice
{
    Q_OBJECT
public:
    CommPort(QObject *parent = 0);
    virtual ~CommPort();

    virtual void Init(int pPort, int pBaud);

signals:
    void sReadyRead(void* pData, int pLen);
};

class BaseSerialport : public CommPort
{
    Q_OBJECT
    friend class SerialMonitorThread;
public:
    BaseSerialport(QObject *parent = 0);
    virtual ~BaseSerialport();

    void SetMonitorThread(QThread *pThread) {mMonitorThread = pThread;}
    virtual void construct();
    virtual void setPortName(const QString & name);
    virtual QString portName() const;
    virtual void setBaudRate(BaudRateType)=0;
    virtual BaudRateType baudRate() const;
    virtual void setDataBits(DataBitsType)=0;
    virtual DataBitsType dataBits() const;
    virtual void setParity(ParityType)=0;
    virtual ParityType parity() const;
    virtual void setStopBits(StopBitsType)=0;
    virtual StopBitsType stopBits() const;
    virtual void setFlowControl(FlowType)=0;
    virtual FlowType flowControl() const;
    virtual void setTimeout(long)=0;

    virtual bool open(OpenMode mode, bool pThreadEn);
    virtual void close();
    virtual bool openhw() = 0;
    virtual bool isSequential() const;
    virtual void closehw()=0;
    virtual void flush()=0;

    virtual qint64 size() const=0;
    virtual qint64 bytesAvailable()=0;
    virtual bool atEnd() const;

    virtual void ungetChar(char c)=0;
    virtual qint64 readLine(char * data, qint64 maxSize);

    QByteArray read(int pMaxNum, int pTimeout);
    int read(char* pBuffer, int pMaxNum, int pTimeout);

    virtual ulong lastError() const;
    virtual void translateError(ulong error)=0;

    virtual void setDtr(bool set=true)=0;
    virtual void setRts(bool set=true)=0;
    virtual ulong lineStatus()=0;

    void StartMonitor();
    void StopMonitor();
    void Monitor();

public:
    QMutex*             mutex;

    QString             port;//>@串口名
    PortSettings        Settings;
    ulong               lastErr;

    QThread*            mMonitorThread;

    virtual qint64 readData(char * data, qint64 maxSize)=0;
    virtual qint64 writeData(const char * data, qint64 maxSize)=0;
signals:
    void dsrChanged(bool status);
};

class SerialMonitorThread : public QThread
{
    Q_OBJECT
public:
    SerialMonitorThread(BaseSerialport *qesp)
        :QThread()
    {
        mQesp = qesp;
        mRun = true;
    }
    ~SerialMonitorThread()
    {
        Stop();
    }
    void Stop()
    {
        mRun = false;
        //mQesp->Stop();
    }

    BaseSerialport*       mQesp;
    bool                    mRun;
protected:
    virtual void run()
    {
        while (mRun)
        {
            mQesp->Monitor();
            usleep(100);
        }
        mRun = true;
    }
};

/************************************************************************
//>@NodeSerial
************************************************************************/
class SerialMonitorThread;
#if (defined(PHONE) || defined(UBUNTU) || defined(LINUX))
#ifndef IOS
class SerialPort : public BaseSerialport
{
    Q_OBJECT
public:
    SerialPort(QObject *parent = 0);
    SerialPort(QString pPortName, BaudRateType pBuad, QObject *parent = 0);
    virtual ~SerialPort();
    void Init(int pPort, int pBaud);
    void init();
    virtual void closehw();
    bool openhw();
    virtual void flush();
    virtual qint64 size() const;
    virtual void ungetChar(char c);
    virtual void setFlowControl(FlowType);
    virtual void setParity(ParityType);
    virtual void setDataBits(DataBitsType);
    virtual void setStopBits(StopBitsType);
    virtual void setBaudRate(BaudRateType);
    virtual void setDtr(bool set=true);
    virtual void setRts(bool set=true);
    virtual ulong lineStatus(void);
    virtual qint64 bytesAvailable();
    virtual void translateError(ulong);
    virtual void setTimeout(long);

    virtual qint64 bytesToWrite() const;
    virtual bool waitForReadyRead(int msecs);	///< @todo implement.

    virtual qint64 readData(char *data, qint64 maxSize);
    virtual qint64 writeData(const char *data, qint64 maxSize);

    bool isOpen()
    {
        if(!File)
            return false;
        return File->isOpen();
    }

    SerialPort& operator=(const SerialPort& s)
    {
        setOpenMode(s.openMode());
        port = s.port;
        Settings.BaudRate=s.Settings.BaudRate;
        Settings.DataBits=s.Settings.DataBits;
        Settings.Parity=s.Settings.Parity;
        Settings.StopBits=s.Settings.StopBits;
        Settings.FlowControl=s.Settings.FlowControl;
        lastErr=s.lastErr;

        File=s.File;
        memcpy(&Timeout, &(s.Timeout), sizeof(struct timeval));
        memcpy(&Copy_Timeout, &(s.Copy_Timeout), sizeof(struct timeval));
        memcpy(&CommConfig, &(s.CommConfig), sizeof(struct termios));
        return *this;
    }
private:
    void Stop()
    {
    }

public:
    QFile* File;
    struct termios CommConfig;
    struct timeval Timeout;
    struct timeval Copy_Timeout;

    QReadWriteLock*     bytesToWriteLock;
    qint64              _bytesToWrite;
};
#endif
#elif WINDOWS

class QReadWriteLock;

class SerialPort : public BaseSerialport
{
    Q_OBJECT
public:
    SerialPort(QString pPortName, BaudRateType pBuad, QObject *parent = 0);
    SerialPort(QObject *parent = 0);
    virtual ~SerialPort();

    void Init(int pPort, int pBaud);

    void init();
    virtual void closehw();
    bool openhw();
    virtual void flush();
    virtual qint64 size() const;
    virtual void ungetChar(char c);
    virtual void setFlowControl(FlowType);
    virtual void setParity(ParityType);
    virtual void setDataBits(DataBitsType);
    virtual void setStopBits(StopBitsType);
    virtual void setBaudRate(BaudRateType);
    virtual void setDtr(bool set=true);
    virtual void setRts(bool set=true);
    virtual ulong lineStatus(void);
    virtual qint64 bytesAvailable();
    virtual void translateError(ulong);
    virtual void setTimeout(long);

    bool isOpen() { return (Handle != INVALID_HANDLE_VALUE); }

    virtual qint64 bytesToWrite() const;
    virtual bool waitForReadyRead(int msecs);	///< @todo implement.

    SerialPort& operator=(const SerialPort& s)
    {
        setOpenMode(s.openMode());
        _bytesToWrite = s._bytesToWrite;
        bytesToWriteLock = new QReadWriteLock;
        memcpy(& overlap, & s.overlap, sizeof(OVERLAPPED));
        memcpy(& overlapWrite, & s.overlapWrite, sizeof(OVERLAPPED));
        lastErr=s.lastErr;
        port = s.port;
        Settings.FlowControl=s.Settings.FlowControl;
        Settings.Parity=s.Settings.Parity;
        Settings.DataBits=s.Settings.DataBits;
        Settings.StopBits=s.Settings.StopBits;
        Settings.BaudRate=s.Settings.BaudRate;
        Handle=s.Handle;
        memcpy(&CommConfig, &s.CommConfig, sizeof(COMMCONFIG));
        memcpy(&CommTimeouts, &s.CommTimeouts, sizeof(COMMTIMEOUTS));
        return *this;
    }
private:
    void Stop()
    {
        if(Handle != INVALID_HANDLE_VALUE)
            SetCommMask(Handle, 0);
    }
protected:
    virtual qint64 readData(char *data, qint64 maxSize);
    virtual qint64 writeData(const char *data, qint64 maxSize);

public:
    HANDLE              Handle;
    OVERLAPPED          overlap;
    OVERLAPPED          overlapWrite;
    COMMCONFIG          CommConfig;
    COMMTIMEOUTS        CommTimeouts;

    QReadWriteLock*     bytesToWriteLock;
    qint64              _bytesToWrite;
};
#endif

#endif // NODESERIAL_H






