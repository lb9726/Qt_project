#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "global.h"

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

//>@structure to contain port settings
/************************************************************************
//>@BaseSerialport
************************************************************************/
class BaseSerialport : public QIODevice
{
    Q_OBJECT
    friend class SerialMonitorThread;
public:
    BaseSerialport(QObject *parent = 0);
    virtual ~BaseSerialport();

    void SetMonitorThread(QThread *pThread) {m_MonitorThread = pThread;}
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
    void Monitor()
    {
        LOCK_MUTEX();
        if(isOpen())
        {
            if(bytesAvailable()>0)
                emit readyRead();
        }
        UNLOCK_MUTEX();
    }
public:
    QMutex*             mutex;

    QString             port;//>@´®¿ÚÃû
    PortSettings        Settings;
    ulong               lastErr;

    QThread*            m_MonitorThread;

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






