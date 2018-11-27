#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "global.h"

#include <QIODevice>
#include <QFile>
#include <QThread>
#include <QMutex>
//>@NodeSerial
#ifdef WINDOWS
#include <windows.h>
#include <QThread>
#include <QReadWriteLock>
#else
#include <stdio.h>
#include <termios.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <fcntl.h>      /*??????*/
#include <linux/serial.h>
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

#define CBR_125000	125000

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

class TransPort : public QIODevice
{
    Q_OBJECT
public:
    TransPort(QObject *parent = 0);

    virtual void Stop() = 0;
    virtual void Monitor() = 0;
};

//>@structure to contain port settings
/************************************************************************
//>@BaseSerialport
************************************************************************/
class BaseSerialport : public TransPort
{
    Q_OBJECT
public:
    BaseSerialport(QObject *parent = 0);
    virtual ~BaseSerialport();

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

    virtual bool open(OpenMode mode)=0;
    virtual bool isSequential() const;
    virtual void close()=0;
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
public:
    QMutex*             mutex;

    QString             port;//>@´®¿ÚÃû
    PortSettings        Settings;
    ulong               lastErr;

    virtual qint64 readData(char * data, qint64 maxSize)=0;
    virtual qint64 writeData(const char * data, qint64 maxSize)=0;
signals:
    void dsrChanged(bool status);
};
/************************************************************************
//>@NodeSerial
************************************************************************/
class SerialMonitorThread;
#ifdef WINDOWS
class QReadWriteLock;

class SerialPort : public BaseSerialport
{
    Q_OBJECT
    friend class SerialMonitorThread;
public:
    SerialPort(QString pPortName, BaudRateType pBuad, QObject *parent = 0);
    SerialPort(QObject *parent = 0);
    virtual ~SerialPort();
    void init();
    virtual void close();
    virtual bool open(OpenMode mode);
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
    void StartMonitor();
    void ReleaseMonitor();
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
    void Stop()
    {
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
    SerialMonitorThread*  m_MonitorThread;
};
#else
class SerialPort : public BaseSerialport
{
    Q_OBJECT
    friend class SerialMonitorThread;
public:
    SerialPort(QObject *parent = 0);
    SerialPort(QString pPortName, BaudRateType pBuad, QObject *parent = 0);
    virtual ~SerialPort();
    void init();
    virtual void close();
    virtual bool open(OpenMode mode);
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
    SerialMonitorThread*      m_MonitorThread;
};
#endif

#ifdef LINUX
#include <linux/types.h>
#include <linux/ioctl.h>
#endif

#define REAL_BOARD

struct spi_parameter{
    unsigned int active_level:1;
    unsigned int lsb:1, tx_neg:1, rx_neg:1, divider:16;
    unsigned int sleep:4;
};

struct spi_data{
    unsigned int write_data;
    unsigned int read_data;
    unsigned int bit_len;
};

#define SPI_MAJOR		231

#define SPI_IOC_MAGIC			'u'
#define SPI_IOC_MAXNR			 3

#ifdef LINUX
#define SPI_IOC_GETPARAMETER	_IOR(SPI_IOC_MAGIC, 0, struct usi_parameter *)
#define SPI_IOC_SETPARAMETER	_IOW(SPI_IOC_MAGIC, 1, struct usi_parameter *)
#define SPI_IOC_SELECTSLAVE	    _IOW(SPI_IOC_MAGIC, 2, int)
#define SPI_IOC_TRANSIT			_IOW(SPI_IOC_MAGIC, 3, struct usi_data *)

#define SpiSelectCS0()				ioctl(spi_fd, SPI_IOC_SELECTSLAVE, 0)
#define SpiDeselectCS0()			ioctl(spi_fd, SPI_IOC_SELECTSLAVE, 1)

#define SpiDoTransit()				ioctl(spi_fd, SPI_IOC_TRANSIT, &gdata)
#else
#define SPI_IOC_GETPARAMETER
#define SPI_IOC_SETPARAMETER
#define SPI_IOC_SELECTSLAVE
#define SPI_IOC_TRANSIT

#define SpiSelectCS0()
#define SpiDeselectCS0()

#define SpiDoTransit()
#endif

#define SpiSetBitLen(x)				gdata.bit_len = x
#define SpiSetData(x)				gdata.write_data = x
#define SpiGetData()				gdata.read_data

extern unsigned char ReadBackBuffer[2][22];

class SpiPort : public TransPort
{
    Q_OBJECT
    friend class SerialMonitorThread;
public:
    SpiPort(QObject *parent = 0);
    ~SpiPort();

    virtual void close();
    virtual bool open(OpenMode mode);

    QByteArray readAll();
    virtual qint64 bytesAvailable();
    virtual void flush();

    qint64 readData(char * data, qint64 maxSize);
    qint64 writeData(const char * data, qint64 maxSize);

    int WriteFrame(char pBuffer[], unsigned int pLen);
    int ReadFrame(unsigned char pBuffer[], unsigned int pLen);

    void StartMonitor();
    void ReleaseMonitor();
    void StopMonitor();
    void Monitor();
    void Stop();

public:
    QFile                    *m_File;
    SerialMonitorThread*      m_MonitorThread;

    int                       spi_fd;
    struct spi_data           gdata;
    struct spi_parameter      para,para1;
    unsigned char             ReadBackBuffer[2][22];
};

#ifdef SPI
typedef SpiPort               E_TRANS_PORT;
#else
typedef SerialPort            E_TRANS_PORT;
#endif

class SerialMonitorThread : public QThread
{
    Q_OBJECT
public:
    SerialMonitorThread(TransPort *qesp)
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
        mQesp->Stop();
    }

    TransPort*       mQesp;
    bool                    mRun;
protected:
    virtual void run()
    {
        while (mRun)
        {
            mQesp->Monitor();
            msleep(10);
        }
        mRun = true;
    }
};

#endif // NODESERIAL_H






