#include "qtuart.h"

/************************************************************************
//>@BaseSerialport
************************************************************************/
/*!
\fn BaseSerialport::BaseSerialport()
Default constructor.
*/

QString GetSerialPort(QString num)
{
    QString tmpPortName;
#ifdef WINDOWS
    tmpPortName = QString("COM%1").arg(num);
#elif UBUNTU
    tmpPortName = QString("/dev/ttyUSB%1").arg(num);
#elif IMX6X
    tmpPortName = QString("/dev/ttymxc%1").arg(num);
#else
    tmpPortName = QString("/dev/ttyAMA%1").arg(num);
#endif
    return tmpPortName;
}

QString GetSerialPortNum(QString portname)
{
    QString tmpPortNum;
#ifdef WINDOWS
    tmpPortNum = portname.remove(QString("COM"));
#elif UBUNTU
    tmpPortNum = portname.remove(QString("/dev/ttyUSB"));
#else
    tmpPortNum = portname.remove(QString("/dev/ttyAMA"));
#endif
    return tmpPortNum;
}


CommPort::CommPort(QObject *parent)
    :  QIODevice(parent)
{
}

CommPort::~CommPort()
{
}

void CommPort::Init(int pPort, int pBaud)
{
    Q_UNUSED(pPort);
    Q_UNUSED(pBaud);
}

BaseSerialport::BaseSerialport(QObject *parent)
    :  CommPort(parent)
{
    mMonitorThread = 0;
    mutex = 0;
    construct();
}

BaseSerialport::~BaseSerialport()
{
    if(mutex)
    {
        delete mutex;
        mutex = 0;
    }
}

void BaseSerialport::StartMonitor()
{
    if(!mMonitorThread)
    {
        mMonitorThread = new SerialMonitorThread(this);
    }
    if(!mMonitorThread->isRunning())
    {
        mMonitorThread->start(QThread::LowPriority);
    }
}

void BaseSerialport::StopMonitor()
{
    if(mMonitorThread != 0)
    {
        mMonitorThread->quit();
        mMonitorThread->deleteLater();
        mMonitorThread = 0;
    }
}

void BaseSerialport::Monitor()
{
    LOCK_MUTEX();
    if(isOpen())
    {
        if(bytesAvailable()>0)
        {
            emit readyRead();
//            QByteArray tmpArray = readAll();
//            const char *tmpBuf = tmpArray.constData();
//            emit sReadyRead((void*)tmpBuf, tmpArray.count());
        }
    }
    UNLOCK_MUTEX();
}

void BaseSerialport::construct()
{
    mutex = new QMutex( QMutex::Recursive );
    setOpenMode(QIODevice::NotOpen);
}

void BaseSerialport::setPortName(const QString & name)
{
    port = name;
}

QString BaseSerialport::portName() const
{
    return port;
}

BaudRateType BaseSerialport::baudRate(void) const
{
    return Settings.BaudRate;
}

DataBitsType BaseSerialport::dataBits() const
{
    return Settings.DataBits;
}

ParityType BaseSerialport::parity() const
{
    return Settings.Parity;
}

StopBitsType BaseSerialport::stopBits() const
{
    return Settings.StopBits;
}

FlowType BaseSerialport::flowControl() const
{
    return Settings.FlowControl;
}

bool BaseSerialport::isSequential() const
{
    return true;
}

bool BaseSerialport::atEnd() const
{
    if (size()) {
        return true;
    }
    return false;
}

bool BaseSerialport::open(OpenMode mode, bool pThreadEn)
{
    close();
    if(pThreadEn)
        StartMonitor();
    if(openhw())
    {
        QIODevice::open(mode);
    }
    return isOpen();;
}

void BaseSerialport::close()
{
    QIODevice::close();
    closehw();
}

qint64 BaseSerialport::readLine(char * data, qint64 maxSize)
{
    qint64 numBytes = bytesAvailable();
    char* pData = data;
    if (maxSize < 2)	//maxSize must be larger than 1
        return -1;
    /*read a byte at a time for MIN(bytesAvail, maxSize - 1) iterations, or until a newline*/
    while (pData<(data+numBytes) && --maxSize)
    {
        readData(pData, 1);
        if (*pData++ == '\n')
        {
            break;
        }
    }
    *pData='\0';
    /*return size of data read*/
    return (pData-data);
}

QByteArray BaseSerialport::read(int pMaxNum, int pTimeout)
{
    volatile int tmpCount = 0;
    int tmpTimeout = pTimeout/10;
    while((tmpCount++ < tmpTimeout) && bytesAvailable() < pMaxNum)
        QtComSleep(10);
    return QIODevice::read((qint64)pMaxNum);
}

int BaseSerialport::read(char* pBuffer, int pMaxNum, int pTimeout)
{
    volatile int tmpCount = 0;
    int tmpTimeout = pTimeout/10;
    while((tmpCount++ < tmpTimeout) && bytesAvailable() < pMaxNum)
        QtComSleep(10);
    return QIODevice::read(pBuffer, (qint64)pMaxNum);
}

/*!
\fn ulong BaseSerialport::lastError() const
Returns the code for the last error encountered by the port, or E_NO_ERROR if the last port
operation was successful.  Possible error codes are:

\verbatim
Error                           Explanation
---------------------------     -------------------------------------------------------------
E_NO_ERROR                      No Error has occured
E_INVALID_FD                    Invalid file descriptor (port was not opened correctly)
E_NO_MEMORY                     Unable to allocate memory tables (POSIX)
E_CAUGHT_NON_BLOCKED_SIGNAL     Caught a non-blocked signal (POSIX)
E_PORT_TIMEOUT                  Operation timed out (POSIX)
E_INVALID_DEVICE                The file opened by the port is not a character device (POSIX)
E_BREAK_CONDITION               The port detected a break condition
E_FRAMING_ERROR                 The port detected a framing error
                                (usually caused by incorrect baud rate settings)
E_IO_ERROR                      There was an I/O error while communicating with the port
E_BUFFER_OVERRUN                Character buffer overrun
E_RECEIVE_OVERFLOW              Receive buffer overflow
E_RECEIVE_PARITY_ERROR          The port detected a parity error in the received data
E_TRANSMIT_OVERFLOW             Transmit buffer overflow
E_READ_FAILED                   General read operation failure
E_WRITE_FAILED                  General write operation failure
\endverbatim
*/
ulong BaseSerialport::lastError() const
{
    return lastErr;
}
/************************************************************************
//>@SerialPort
************************************************************************/
/*!
\class QextSerialPort
\author Stefan Sander
\author Michal Policht

A cross-platform serial port class.
This class encapsulates a serial port on both POSIX and Windows systems.  The user will be
notified of errors and possible portability conflicts at run-time by default - this behavior can
be turned off by defining _TTY_NOWARN_ (to turn off all warnings) or _TTY_NOWARN_PORT_ (to turn
off portability warnings) in the project.

You may choose from polling or event driven API. For details check setQueryMode() documentation.

\note
On Windows NT/2000/XP this class uses Win32 serial port functions by default.  The user may
select POSIX behavior under NT, 2000, or XP ONLY by defining _TTY_ in the project. I can
make no guarantees as to the quality of POSIX support under NT/2000 however.

*/

#if (defined(PHONE) || defined(UBUNTU) || defined(LINUX))
#ifndef IOS
SerialPort::SerialPort(QString pPortName, BaudRateType pBuad, QObject *parent):
    BaseSerialport(parent)
{
    File = 0;

    setPortName(pPortName);
    Settings.BaudRate = pBuad;
    Settings.DataBits = DATA_8;
    Settings.StopBits = STOP_1;
    Settings.Parity = PAR_NONE;
    Settings.FlowControl = FLOW_OFF;
    Settings.Timeout_Millisec = 1;
    init();
}

SerialPort::SerialPort(QObject *parent):
    BaseSerialport(parent)
{
    File = 0;

    Settings.BaudRate = BAUD115200;
    Settings.DataBits = DATA_8;
    Settings.StopBits = STOP_1;
    Settings.Parity = PAR_NONE;
    Settings.FlowControl = FLOW_OFF;
    Settings.Timeout_Millisec = 1;
    init();
}

SerialPort::~SerialPort()
{
    if(bytesToWriteLock)
        delete bytesToWriteLock;
    if (isOpen())
        close();
    if(File)
        File->close();
    delete File;
}
//edit kattle
void SerialPort::Init(int pPort, int pBaud)
{
    QString tmpPort = GetSerialPort(QString::number(pPort));
    BaudRateType tmpBaud = (BaudRateType)pBaud;
    setPortName(tmpPort);
    setBaudRate(tmpBaud);
    if(!isOpen())
        open(QIODevice::ReadWrite, true);
}

void SerialPort::init()
{
    _bytesToWrite = 0;
    bytesToWriteLock = new QReadWriteLock;
    File=new QFile();
}

bool SerialPort::openhw()
{
    close();
    /*open the port*/
    LOCK_MUTEX();
    File->setFileName(port);
    if (File->open(QIODevice::ReadWrite|QIODevice::Unbuffered))
    {
        /*configure port settings*/
        tcgetattr(File->handle(), &CommConfig);

        /*set up other port settings*/
        CommConfig.c_cflag|=CREAD|CLOCAL;
        CommConfig.c_lflag&=(~(ICANON|ECHO|ECHOE|ECHOK|ECHONL|ISIG));
        CommConfig.c_iflag&=(~(INPCK|IGNPAR|PARMRK|ISTRIP|ICRNL|IXANY));
        CommConfig.c_oflag&=(~OPOST);
        CommConfig.c_cc[VMIN]=0;
        CommConfig.c_cc[VINTR] = _POSIX_VDISABLE;
        CommConfig.c_cc[VQUIT] = _POSIX_VDISABLE;
        CommConfig.c_cc[VSTART] = _POSIX_VDISABLE;
        CommConfig.c_cc[VSTOP] = _POSIX_VDISABLE;
        CommConfig.c_cc[VSUSP] = _POSIX_VDISABLE;
        setBaudRate(Settings.BaudRate);
        setDataBits(Settings.DataBits);
        setParity(Settings.Parity);
        setStopBits(Settings.StopBits);
        setFlowControl(Settings.FlowControl);
        setTimeout(Settings.Timeout_Millisec);
        tcsetattr(File->handle(), TCSAFLUSH, &CommConfig);
        UNLOCK_MUTEX();
        return true;
    }
    UNLOCK_MUTEX();
    return false;
}

void SerialPort::closehw()
{
    LOCK_MUTEX();
    if(File)
        File->close();
    UNLOCK_MUTEX();
}

void SerialPort::flush()
{
    LOCK_MUTEX();
    if (isOpen()) {
        File->flush();
    }
    UNLOCK_MUTEX();
}

/*!
\fn qint64 QextSerialPort::size() const
This function will return the number of bytes waiting in the receive queue of the serial port.
It is included primarily to provide a complete QIODevice interface, and will not record errors
in the lastErr member (because it is const).  This function is also not thread-safe - in
multithreading situations, use QextSerialPort::bytesAvailable() instead.
*/
qint64 SerialPort::size() const
{
    int numBytes;
    if (ioctl(File->handle(), FIONREAD, &numBytes)<0) {
        numBytes=0;
    }
    return (qint64)numBytes;
}

/*!
\fn qint64 QextSerialPort::bytesAvailable()
Returns the number of bytes waiting in the port's receive queue.  This function will return 0 if
the port is not currently open, or -1 on error.  Error information can be retrieved by calling
QextSerialPort::getLastError().
*/
qint64 SerialPort::bytesAvailable()
{
    LOCK_MUTEX();
    if (isOpen()) {
        int bytesQueued;
        fd_set fileSet;
        FD_ZERO(&fileSet);
        FD_SET(File->handle(), &fileSet);

        /*on Linux systems the Timeout structure will be altered by the select() call.
          Make sure we use the right timeout values*/
        //memcpy(&Timeout, &Copy_Timeout, sizeof(struct timeval));
        Timeout = Copy_Timeout;
        int n=select(File->handle()+1, &fileSet, NULL, &fileSet, &Timeout);
        if (!n) {
            lastErr=E_PORT_TIMEOUT;
            UNLOCK_MUTEX();
            return -1;
        }
        if (n==-1 || ioctl(File->handle(), FIONREAD, &bytesQueued)==-1) {
            translateError(errno);
            UNLOCK_MUTEX();
            return -1;
        }
        lastErr=E_NO_ERROR;
        UNLOCK_MUTEX();
        return bytesQueued + QIODevice::bytesAvailable();
    }
    UNLOCK_MUTEX();
    return 0;
}

/*!
\fn void QextSerialPort::translateError(ulong error)
Translates a system-specific error code to a QextSerialPort error code.  Used internally.
*/
void SerialPort::translateError(ulong error)
{
    switch (error) {
        case EBADF:
        case ENOTTY:
            lastErr=E_INVALID_FD;
            break;

        case EINTR:
            lastErr=E_CAUGHT_NON_BLOCKED_SIGNAL;
            break;

        case ENOMEM:
            lastErr=E_NO_MEMORY;
            break;
    }
}

/*!
\fn qint64 QextSerialPort::readData(char *data, qint64 maxSize)
Reads a block of data from the serial port.  This function will read at most maxlen bytes from
the serial port and place them in the buffer pointed to by data.  Return value is the number of
bytes actually read, or -1 on error.

\warning before calling this function ensure that serial port associated with this class
is currently open (use isOpen() function to check if port is open).
*/
qint64 SerialPort::readData(char *data, qint64 maxSize)
{
    LOCK_MUTEX();
    int retVal=0;
    retVal=File->read(data, maxSize);
    if (retVal==-1)
        lastErr=E_READ_FAILED;
    UNLOCK_MUTEX();

    return retVal;
}

/*!
\fn qint64 QextSerialPort::writeData(const char *data, qint64 maxSize)
Writes a block of data to the serial port.  This function will write len bytes
from the buffer pointed to by data to the serial port.  Return value is the number
of bytes actually written, or -1 on error.

\warning before calling this function ensure that serial port associated with this class
is currently open (use isOpen() function to check if port is open).
*/
qint64 SerialPort::writeData(const char *data, qint64 maxSize)
{
    LOCK_MUTEX();
    int retVal=0;
    retVal=File->write(data, maxSize);
    if (retVal==-1)
       lastErr=E_WRITE_FAILED;
    UNLOCK_MUTEX();

    return retVal;
}

/*!
\fn void QextSerialPort::ungetChar(char c)
This function is included to implement the full QIODevice interface, and currently has no
purpose within this class.  This function is meaningless on an unbuffered device and currently
only prints a warning message to that effect.
*/
void SerialPort::ungetChar(char c)
{
    Q_UNUSED(c);
    /*meaningless on unbuffered sequential device - return error and print a warning*/
    TTY_WARNING("QextSerialPort: ungetChar() called on an unbuffered sequential device - operation is meaningless");
}

/*!
\fn void QextSerialPort::setFlowControl(FlowType flow)
Sets the flow control used by the port.  Possible values of flow are:
\verbatim
    FLOW_OFF            No flow control
    FLOW_HARDWARE       Hardware (RTS/CTS) flow control
    FLOW_XONXOFF        Software (XON/XOFF) flow control
\endverbatim
*/
void SerialPort::setFlowControl(FlowType flow)
{
    LOCK_MUTEX();
    if (Settings.FlowControl!=flow) {
        Settings.FlowControl=flow;
    }
    if (isOpen()) {
        switch(flow) {

            /*no flow control*/
            case FLOW_OFF:
                CommConfig.c_cflag&=(~CRTSCTS);
                CommConfig.c_iflag&=(~(IXON|IXOFF|IXANY));
                tcsetattr(File->handle(), TCSAFLUSH, &CommConfig);
                break;

            /*software (XON/XOFF) flow control*/
            case FLOW_XONXOFF:
                CommConfig.c_cflag&=(~CRTSCTS);
                CommConfig.c_iflag|=(IXON|IXOFF|IXANY);
                tcsetattr(File->handle(), TCSAFLUSH, &CommConfig);
                break;

            case FLOW_HARDWARE:
                CommConfig.c_cflag|=CRTSCTS;
                CommConfig.c_iflag&=(~(IXON|IXOFF|IXANY));
                tcsetattr(File->handle(), TCSAFLUSH, &CommConfig);
                break;
        }
    }
    UNLOCK_MUTEX();
}

/*!
\fn void QextSerialPort::setParity(ParityType parity)
Sets the parity associated with the serial port.  The possible values of parity are:
\verbatim
    PAR_SPACE       Space Parity
    PAR_MARK        Mark Parity
    PAR_NONE        No Parity
    PAR_EVEN        Even Parity
    PAR_ODD         Odd Parity
\endverbatim
*/
void SerialPort::setParity(ParityType parity) {
    LOCK_MUTEX();
    if (Settings.Parity!=parity) {
        if (parity==PAR_MARK || (parity==PAR_SPACE && Settings.DataBits==DATA_8)) {
        }
        else {
            Settings.Parity=parity;
        }
    }
    if (isOpen()) {
        switch (parity) {

            /*space parity*/
            case PAR_SPACE:
                if (Settings.DataBits==DATA_8) {
                    TTY_PORTABILITY_WARNING("QextSerialPort:  Space parity is only supported in POSIX with 7 or fewer data bits");
                }
                else {

                    /*space parity not directly supported - add an extra data bit to simulate it*/
                    CommConfig.c_cflag&=~(PARENB|CSIZE);
                    switch(Settings.DataBits) {
                        case DATA_5:
                            Settings.DataBits=DATA_6;
                            CommConfig.c_cflag|=CS6;
                            break;

                        case DATA_6:
                            Settings.DataBits=DATA_7;
                            CommConfig.c_cflag|=CS7;
                            break;

                        case DATA_7:
                            Settings.DataBits=DATA_8;
                            CommConfig.c_cflag|=CS8;
                            break;

                        case DATA_8:
                            break;
                    }
                    tcsetattr(File->handle(), TCSAFLUSH, &CommConfig);
                }
                break;

            /*mark parity - WINDOWS ONLY*/
            case PAR_MARK:
                TTY_WARNING("QextSerialPort: Mark parity is not supported by POSIX.");
                break;

            /*no parity*/
            case PAR_NONE:
                CommConfig.c_cflag&=(~PARENB);
                tcsetattr(File->handle(), TCSAFLUSH, &CommConfig);
                break;

            /*even parity*/
            case PAR_EVEN:
                CommConfig.c_cflag&=(~PARODD);
                CommConfig.c_cflag|=PARENB;
                tcsetattr(File->handle(), TCSAFLUSH, &CommConfig);
                break;

            /*odd parity*/
            case PAR_ODD:
                CommConfig.c_cflag|=(PARENB|PARODD);
                tcsetattr(File->handle(), TCSAFLUSH, &CommConfig);
                break;
        }
    }
    UNLOCK_MUTEX();
}

/*!
\fn void QextSerialPort::setDataBits(DataBitsType dataBits)
Sets the number of data bits used by the serial port.  Possible values of dataBits are:
\verbatim
    DATA_5      5 data bits
    DATA_6      6 data bits
    DATA_7      7 data bits
    DATA_8      8 data bits
\endverbatim

\note
This function is subject to the following restrictions:
\par
    5 data bits cannot be used with 2 stop bits.
\par
    1.5 stop bits can only be used with 5 data bits.
\par
    8 data bits cannot be used with space parity on POSIX systems.

*/
void SerialPort::setDataBits(DataBitsType dataBits) {
    LOCK_MUTEX();
    if (Settings.DataBits!=dataBits) {
        if ((Settings.StopBits==STOP_2 && dataBits==DATA_5) ||
            (Settings.StopBits==STOP_1_5 && dataBits!=DATA_5) ||
            (Settings.Parity==PAR_SPACE && dataBits==DATA_8)) {
        }
        else {
            Settings.DataBits=dataBits;
        }
    }
    if (isOpen()) {
        switch(dataBits) {

            /*5 data bits*/
            case DATA_5:
                if (Settings.StopBits==STOP_2) {
                    TTY_WARNING("QextSerialPort: 5 Data bits cannot be used with 2 stop bits.");
                }
                else {
                    Settings.DataBits=dataBits;
                    CommConfig.c_cflag&=(~CSIZE);
                    CommConfig.c_cflag|=CS5;
                    tcsetattr(File->handle(), TCSAFLUSH, &CommConfig);
                }
                break;

            /*6 data bits*/
            case DATA_6:
                if (Settings.StopBits==STOP_1_5) {
                    TTY_WARNING("QextSerialPort: 6 Data bits cannot be used with 1.5 stop bits.");
                }
                else {
                    Settings.DataBits=dataBits;
                    CommConfig.c_cflag&=(~CSIZE);
                    CommConfig.c_cflag|=CS6;
                    tcsetattr(File->handle(), TCSAFLUSH, &CommConfig);
                }
                break;

            /*7 data bits*/
            case DATA_7:
                if (Settings.StopBits==STOP_1_5) {
                    TTY_WARNING("QextSerialPort: 7 Data bits cannot be used with 1.5 stop bits.");
                }
                else {
                    Settings.DataBits=dataBits;
                    CommConfig.c_cflag&=(~CSIZE);
                    CommConfig.c_cflag|=CS7;
                    tcsetattr(File->handle(), TCSAFLUSH, &CommConfig);
                }
                break;

            /*8 data bits*/
            case DATA_8:
                if (Settings.StopBits==STOP_1_5) {
                    TTY_WARNING("QextSerialPort: 8 Data bits cannot be used with 1.5 stop bits.");
                }
                else {
                    Settings.DataBits=dataBits;
                    CommConfig.c_cflag&=(~CSIZE);
                    CommConfig.c_cflag|=CS8;
                    tcsetattr(File->handle(), TCSAFLUSH, &CommConfig);
                }
                break;
        }
    }
    UNLOCK_MUTEX();
}

/*!
\fn void QextSerialPort::setStopBits(StopBitsType stopBits)
Sets the number of stop bits used by the serial port.  Possible values of stopBits are:
\verbatim
    STOP_1      1 stop bit
    STOP_1_5    1.5 stop bits
    STOP_2      2 stop bits
\endverbatim

\note
This function is subject to the following restrictions:
\par
    2 stop bits cannot be used with 5 data bits.
\par
    1.5 stop bits cannot be used with 6 or more data bits.
\par
    POSIX does not support 1.5 stop bits.
*/
void SerialPort::setStopBits(StopBitsType stopBits) {
    LOCK_MUTEX();
    if (Settings.StopBits!=stopBits) {
        if ((Settings.DataBits==DATA_5 && stopBits==STOP_2) || stopBits==STOP_1_5) {}
        else {
            Settings.StopBits=stopBits;
        }
    }
    if (isOpen()) {
        switch (stopBits) {

            /*one stop bit*/
            case STOP_1:
                Settings.StopBits=stopBits;
                CommConfig.c_cflag&=(~CSTOPB);
                tcsetattr(File->handle(), TCSAFLUSH, &CommConfig);
                break;

            /*1.5 stop bits*/
            case STOP_1_5:
                TTY_WARNING("QextSerialPort: 1.5 stop bit operation is not supported by POSIX.");
                break;

            /*two stop bits*/
            case STOP_2:
                if (Settings.DataBits==DATA_5) {
                    TTY_WARNING("QextSerialPort: 2 stop bits cannot be used with 5 data bits");
                }
                else {
                    Settings.StopBits=stopBits;
                    CommConfig.c_cflag|=CSTOPB;
                    tcsetattr(File->handle(), TCSAFLUSH, &CommConfig);
                }
                break;
        }
    }
    UNLOCK_MUTEX();
}

/*!
\fn void QextSerialPort::setBaudRate(BaudRateType baudRate)
Sets the baud rate of the serial port.  Note that not all rates are applicable on
all platforms.  The following table shows translations of the various baud rate
constants on Windows(including NT/2000) and POSIX platforms.  Speeds marked with an *
are speeds that are usable on both Windows and POSIX.
\verbatim

  RATE          Windows Speed   POSIX Speed
  -----------   -------------   -----------
   BAUD50                 110          50
   BAUD75                 110          75
  *BAUD110                110         110
   BAUD134                110         134.5
   BAUD150                110         150
   BAUD200                110         200
  *BAUD300                300         300
  *BAUD600                600         600
  *BAUD1200              1200        1200
   BAUD1800              1200        1800
  *BAUD2400              2400        2400
  *BAUD4800              4800        4800
  *BAUD9600              9600        9600
   BAUD14400            14400        9600
  *BAUD19200            19200       19200
  *BAUD38400            38400       38400
   BAUD56000            56000       38400
  *BAUD57600            57600       57600
   BAUD76800            57600       76800
  *BAUD115200          115200      115200
   BAUD128000          128000      115200
   BAUD256000          256000      115200
\endverbatim
*/
void SerialPort::setBaudRate(BaudRateType baudRate) {
    LOCK_MUTEX();
    if (Settings.BaudRate!=baudRate) {
        switch (baudRate) {
            case BAUD14400:
                Settings.BaudRate=BAUD9600;
                break;

            case BAUD56000:
                Settings.BaudRate=BAUD38400;
                break;

            case BAUD76800:

#ifndef B76800
                Settings.BaudRate=BAUD57600;
#else
                Settings.BaudRate=baudRate;
#endif
                break;

            case BAUD128000:
            case BAUD256000:
                Settings.BaudRate=BAUD115200;
                break;

            default:
                Settings.BaudRate=baudRate;
                break;
        }
    }
    if (isOpen()) {
        switch (baudRate) {

            /*50 baud*/
            case BAUD50:
                TTY_PORTABILITY_WARNING("QextSerialPort Portability Warning: Windows does not support 50 baud operation.");
#ifdef CBAUD
                CommConfig.c_cflag&=(~CBAUD);
                CommConfig.c_cflag|=B50;
#else
                cfsetispeed(&CommConfig, B50);
                cfsetospeed(&CommConfig, B50);
#endif
                break;

            /*75 baud*/
            case BAUD75:
                TTY_PORTABILITY_WARNING("QextSerialPort Portability Warning: Windows does not support 75 baud operation.");
#ifdef CBAUD
                CommConfig.c_cflag&=(~CBAUD);
                CommConfig.c_cflag|=B75;
#else
                cfsetispeed(&CommConfig, B75);
                cfsetospeed(&CommConfig, B75);
#endif
                break;

            /*110 baud*/
            case BAUD110:
#ifdef CBAUD
                CommConfig.c_cflag&=(~CBAUD);
                CommConfig.c_cflag|=B110;
#else
                cfsetispeed(&CommConfig, B110);
                cfsetospeed(&CommConfig, B110);
#endif
                break;

            /*134.5 baud*/
            case BAUD134:
                TTY_PORTABILITY_WARNING("QextSerialPort Portability Warning: Windows does not support 134.5 baud operation.");
#ifdef CBAUD
                CommConfig.c_cflag&=(~CBAUD);
                CommConfig.c_cflag|=B134;
#else
                cfsetispeed(&CommConfig, B134);
                cfsetospeed(&CommConfig, B134);
#endif
                break;

            /*150 baud*/
            case BAUD150:
                TTY_PORTABILITY_WARNING("QextSerialPort Portability Warning: Windows does not support 150 baud operation.");
#ifdef CBAUD
                CommConfig.c_cflag&=(~CBAUD);
                CommConfig.c_cflag|=B150;
#else
                cfsetispeed(&CommConfig, B150);
                cfsetospeed(&CommConfig, B150);
#endif
                break;

            /*200 baud*/
            case BAUD200:
                TTY_PORTABILITY_WARNING("QextSerialPort Portability Warning: Windows does not support 200 baud operation.");
#ifdef CBAUD
                CommConfig.c_cflag&=(~CBAUD);
                CommConfig.c_cflag|=B200;
#else
                cfsetispeed(&CommConfig, B200);
                cfsetospeed(&CommConfig, B200);
#endif
                break;

            /*300 baud*/
            case BAUD300:
#ifdef CBAUD
                CommConfig.c_cflag&=(~CBAUD);
                CommConfig.c_cflag|=B300;
#else
                cfsetispeed(&CommConfig, B300);
                cfsetospeed(&CommConfig, B300);
#endif
                break;

            /*600 baud*/
            case BAUD600:
#ifdef CBAUD
                CommConfig.c_cflag&=(~CBAUD);
                CommConfig.c_cflag|=B600;
#else
                cfsetispeed(&CommConfig, B600);
                cfsetospeed(&CommConfig, B600);
#endif
                break;

            /*1200 baud*/
            case BAUD1200:
#ifdef CBAUD
                CommConfig.c_cflag&=(~CBAUD);
                CommConfig.c_cflag|=B1200;
#else
                cfsetispeed(&CommConfig, B1200);
                cfsetospeed(&CommConfig, B1200);
#endif
                break;

            /*1800 baud*/
            case BAUD1800:
                TTY_PORTABILITY_WARNING("QextSerialPort Portability Warning: Windows and IRIX do not support 1800 baud operation.");
#ifdef CBAUD
                CommConfig.c_cflag&=(~CBAUD);
                CommConfig.c_cflag|=B1800;
#else
                cfsetispeed(&CommConfig, B1800);
                cfsetospeed(&CommConfig, B1800);
#endif
                break;

            /*2400 baud*/
            case BAUD2400:
#ifdef CBAUD
                CommConfig.c_cflag&=(~CBAUD);
                CommConfig.c_cflag|=B2400;
#else
                cfsetispeed(&CommConfig, B2400);
                cfsetospeed(&CommConfig, B2400);
#endif
                break;

            /*4800 baud*/
            case BAUD4800:
#ifdef CBAUD
                CommConfig.c_cflag&=(~CBAUD);
                CommConfig.c_cflag|=B4800;
#else
                cfsetispeed(&CommConfig, B4800);
                cfsetospeed(&CommConfig, B4800);
#endif
                break;

            /*9600 baud*/
            case BAUD9600:
#ifdef CBAUD
                CommConfig.c_cflag&=(~CBAUD);
                CommConfig.c_cflag|=B9600;
#else
                cfsetispeed(&CommConfig, B9600);
                cfsetospeed(&CommConfig, B9600);
#endif
                break;

            /*14400 baud*/
            case BAUD14400:
                TTY_WARNING("QextSerialPort: POSIX does not support 14400 baud operation.  Switching to 9600 baud.");
#ifdef CBAUD
                CommConfig.c_cflag&=(~CBAUD);
                CommConfig.c_cflag|=B9600;
#else
                cfsetispeed(&CommConfig, B9600);
                cfsetospeed(&CommConfig, B9600);
#endif
                break;

            /*19200 baud*/
            case BAUD19200:
#ifdef CBAUD
                CommConfig.c_cflag&=(~CBAUD);
                CommConfig.c_cflag|=B19200;
#else
                cfsetispeed(&CommConfig, B19200);
                cfsetospeed(&CommConfig, B19200);
#endif
                break;

            /*38400 baud*/
            case BAUD38400:
#ifdef CBAUD
                CommConfig.c_cflag&=(~CBAUD);
                CommConfig.c_cflag|=B38400;
#else
                cfsetispeed(&CommConfig, B38400);
                cfsetospeed(&CommConfig, B38400);
#endif
                break;

            /*56000 baud*/
            case BAUD56000:
                TTY_WARNING("QextSerialPort: POSIX does not support 56000 baud operation.  Switching to 38400 baud.");
#ifdef CBAUD
                CommConfig.c_cflag&=(~CBAUD);
                CommConfig.c_cflag|=B38400;
#else
                cfsetispeed(&CommConfig, B38400);
                cfsetospeed(&CommConfig, B38400);
#endif
                break;

            /*57600 baud*/
            case BAUD57600:
#ifdef CBAUD
                CommConfig.c_cflag&=(~CBAUD);
                CommConfig.c_cflag|=B57600;
#else
                cfsetispeed(&CommConfig, B57600);
                cfsetospeed(&CommConfig, B57600);
#endif
                break;

            /*76800 baud*/
            case BAUD76800:
                TTY_PORTABILITY_WARNING("QextSerialPort Portability Warning: Windows and some POSIX systems do not support 76800 baud operation.");
#ifdef CBAUD
                CommConfig.c_cflag&=(~CBAUD);

#ifdef B76800
                CommConfig.c_cflag|=B76800;
#else
                TTY_WARNING("QextSerialPort: QextSerialPort was compiled without 76800 baud support.  Switching to 57600 baud.");
                CommConfig.c_cflag|=B57600;
#endif //B76800
#else  //CBAUD
#ifdef B76800
                cfsetispeed(&CommConfig, B76800);
                cfsetospeed(&CommConfig, B76800);
#else
                TTY_WARNING("QextSerialPort: QextSerialPort was compiled without 76800 baud support.  Switching to 57600 baud.");
                cfsetispeed(&CommConfig, B57600);
                cfsetospeed(&CommConfig, B57600);
#endif //B76800
#endif //CBAUD
                break;

            /*115200 baud*/
            case BAUD115200:
#ifdef CBAUD
                CommConfig.c_cflag&=(~CBAUD);
                CommConfig.c_cflag|=B115200;
#else
                cfsetispeed(&CommConfig, B115200);
                cfsetospeed(&CommConfig, B115200);
#endif
                break;

            /*128000 baud*/
            case BAUD128000:
                TTY_WARNING("QextSerialPort: POSIX does not support 128000 baud operation.  Switching to 115200 baud.");
#ifdef CBAUD
                CommConfig.c_cflag&=(~CBAUD);
                CommConfig.c_cflag|=B115200;
#else
                cfsetispeed(&CommConfig, B115200);
                cfsetospeed(&CommConfig, B115200);
#endif
                break;

            /*256000 baud*/
            case BAUD256000:
                TTY_WARNING("QextSerialPort: POSIX does not support 256000 baud operation.  Switching to 115200 baud.");
#ifdef CBAUD
                CommConfig.c_cflag&=(~CBAUD);
                CommConfig.c_cflag|=B115200;
#else
                cfsetispeed(&CommConfig, B115200);
                cfsetospeed(&CommConfig, B115200);
#endif
                break;
        default:
            break;
        }
        tcsetattr(File->handle(), TCSAFLUSH, &CommConfig);
    }
    UNLOCK_MUTEX();
}

/*!
\fn void QextSerialPort::setDtr(bool set)
Sets DTR line to the requested state (high by default).  This function will have no effect if
the port associated with the class is not currently open.
*/
void SerialPort::setDtr(bool set) {
    LOCK_MUTEX();
    if (isOpen()) {
        int status;
        ioctl(File->handle(), TIOCMGET, &status);
        if (set) {
            status|=TIOCM_DTR;
        }
        else {
            status&=~TIOCM_DTR;
        }
        ioctl(File->handle(), TIOCMSET, &status);
    }
    UNLOCK_MUTEX();
}

/*!
\fn void QextSerialPort::setRts(bool set)
Sets RTS line to the requested state (high by default).  This function will have no effect if
the port associated with the class is not currently open.
*/
void SerialPort::setRts(bool set) {
    LOCK_MUTEX();
    if (isOpen()) {
        int status;
        ioctl(File->handle(), TIOCMGET, &status);
        if (set) {
            status|=TIOCM_RTS;
        }
        else {
            status&=~TIOCM_RTS;
        }
        ioctl(File->handle(), TIOCMSET, &status);
    }
    UNLOCK_MUTEX();
}

/*!
\fn ulong QextSerialPort::lineStatus(void)
returns the line status as stored by the port function.  This function will retrieve the states
of the following lines: DCD, CTS, DSR, and RI.  On POSIX systems, the following additional lines
can be monitored: DTR, RTS, Secondary TXD, and Secondary RXD.  The value returned is an unsigned
long with specific bits indicating which lines are high.  The following constants should be used
to examine the states of individual lines:

\verbatim
Mask        Line
------      ----
LS_CTS      CTS
LS_DSR      DSR
LS_DCD      DCD
LS_RI       RI
\endverbatim

This function will return 0 if the port associated with the class is not currently open.
*/
ulong SerialPort::lineStatus(void) {
    unsigned long Status=0, Temp=0;
    LOCK_MUTEX();
    if (isOpen()) {
        ioctl(File->handle(), TIOCMGET, &Temp);
        if (Temp&TIOCM_CTS) {
            Status|=LS_CTS;
        }
        if (Temp&TIOCM_DSR) {
            Status|=LS_DSR;
        }
        if (Temp&TIOCM_RI) {
            Status|=LS_RI;
        }
        if (Temp&TIOCM_CD) {
            Status|=LS_DCD;
        }
        if (Temp&TIOCM_DTR) {
            Status|=LS_DTR;
        }
        if (Temp&TIOCM_RTS) {
            Status|=LS_RTS;
        }
        if (Temp&TIOCM_ST) {
            Status|=LS_ST;
        }
        if (Temp&TIOCM_SR) {
            Status|=LS_SR;
        }
    }
    UNLOCK_MUTEX();
    return Status;
}

bool SerialPort::waitForReadyRead(int msecs)
{
    Q_UNUSED(msecs);
    //@todo implement
    return false;
}

qint64 SerialPort::bytesToWrite() const
{
    return _bytesToWrite;
}

/*!
\fn void QextSerialPort::setTimeout(ulong millisec);
Sets the read and write timeouts for the port to millisec milliseconds.
Setting 0 for both sec and millisec indicates that timeouts are not used for read nor
write operations. Setting -1 indicates that read and write should return immediately.

\note this function does nothing in event driven mode.
*/
void SerialPort::setTimeout(long millisec)
{
    LOCK_MUTEX();
    Settings.Timeout_Millisec = millisec;
    Copy_Timeout.tv_sec = millisec / 1000;
    Copy_Timeout.tv_usec = millisec % 1000;
    if (isOpen()) {
        tcgetattr(File->handle(), &CommConfig);
        CommConfig.c_cc[VTIME] = millisec/100;
        tcsetattr(File->handle(), TCSAFLUSH, &CommConfig);
    }
    UNLOCK_MUTEX();
}
#endif
#elif WINDOWS

SerialPort::SerialPort(QString pPortName, BaudRateType pBuad, QObject *parent):
    BaseSerialport(parent)
{
    mMonitorThread = 0;
    bytesToWriteLock = 0;
    Handle=INVALID_HANDLE_VALUE;

    setPortName(pPortName);
    Settings.BaudRate = pBuad;
    Settings.DataBits = DATA_8;
    Settings.StopBits = STOP_1;
    Settings.Parity = PAR_NONE;
    Settings.FlowControl = FLOW_OFF;
    Settings.Timeout_Millisec = 1;
    init();
}

SerialPort::SerialPort(QObject *parent):
    BaseSerialport(parent)
{
    mMonitorThread = 0;
    bytesToWriteLock = 0;
    Handle=INVALID_HANDLE_VALUE;

    Settings.BaudRate = BAUD115200;
    Settings.DataBits = DATA_8;
    Settings.StopBits = STOP_1;
    Settings.Parity = PAR_NONE;
    Settings.FlowControl = FLOW_OFF;
    Settings.Timeout_Millisec = 1;
    init();
}

SerialPort::~SerialPort()
{
    if (isOpen())
        close();
    CloseHandle(overlap.hEvent);
    if(bytesToWriteLock)
    {
        delete bytesToWriteLock;
        bytesToWriteLock = 0;
    }
}

void SerialPort::Init(int pPort, int pBaud)
{
    QString tmpPort = GetSerialPort(QString::number(pPort));
    BaudRateType tmpBaud = (BaudRateType)pBaud;
    setPortName(tmpPort);
    setBaudRate(tmpBaud);
    if(!isOpen())
        open(QIODevice::ReadWrite, true);
}

void SerialPort::init()
{
    _bytesToWrite = 0;
    overlap.Internal = 0;
    overlap.InternalHigh = 0;
    overlap.Offset = 0;
    overlap.OffsetHigh = 0;
    overlap.hEvent = CreateEvent(NULL, true, false, NULL);
    bytesToWriteLock = new QReadWriteLock;
    Handle=INVALID_HANDLE_VALUE;
    mMonitorThread = new SerialMonitorThread(this);
}

bool SerialPort::openhw()
{
    unsigned long confSize = sizeof(COMMCONFIG);
    CommConfig.dwSize = confSize;
    DWORD dwFlagsAndAttributes = 0;
    closehw();
    LOCK_MUTEX();
    /*open the port*/
    Handle=CreateFileA
            (port.toLatin1(),
             GENERIC_READ|GENERIC_WRITE,
             FILE_SHARE_READ|FILE_SHARE_WRITE,
             NULL,
             OPEN_EXISTING,
             dwFlagsAndAttributes,
             NULL);
    if (Handle!=INVALID_HANDLE_VALUE)
    {
        /*configure port settings*/
        GetCommConfig(Handle, &CommConfig, &confSize);
        GetCommState(Handle, &(CommConfig.dcb));
        /*set up parameters*/
        CommConfig.dcb.fBinary=TRUE;
        CommConfig.dcb.fInX=FALSE;
        CommConfig.dcb.fOutX=FALSE;
        CommConfig.dcb.fAbortOnError=FALSE;
        CommConfig.dcb.fNull=FALSE;
        setBaudRate(Settings.BaudRate);
        setDataBits(Settings.DataBits);
        setStopBits(Settings.StopBits);
        setParity(Settings.Parity);
        setFlowControl(Settings.FlowControl);
        setTimeout(Settings.Timeout_Millisec);
        SetCommConfig(Handle, &CommConfig, sizeof(COMMCONFIG));
        UNLOCK_MUTEX();
        return true;
    }
    UNLOCK_MUTEX();
    return false;
}

void SerialPort::closehw()
{
    LOCK_MUTEX();
    if (isOpen())
    {
        flush();
        if (CloseHandle(Handle))
            Handle = INVALID_HANDLE_VALUE;
        _bytesToWrite = 0;
    }
    UNLOCK_MUTEX();
}

void SerialPort::flush()
{
    LOCK_MUTEX();
    if (isOpen())
    {
        FlushFileBuffers(Handle);
    }
    UNLOCK_MUTEX();
}

/*!
\fn qint64 QextSerialPort::size() const
This function will return the number of bytes waiting in the receive queue of the serial port.
It is included primarily to provide a complete QIODevice interface, and will not record errors
in the lastErr member (because it is const).  This function is also not thread-safe - in
multithreading situations, use QextSerialPort::bytesAvailable() instead.
*/
qint64 SerialPort::size() const
{
    int availBytes;
    COMSTAT ComStat;
    DWORD ErrorMask=0;
    ClearCommError(Handle, &ErrorMask, &ComStat);
    availBytes = ComStat.cbInQue;
    return (qint64)availBytes;
}

/*!
\fn qint64 QextSerialPort::bytesAvailable()
Returns the number of bytes waiting in the port's receive queue.  This function will return 0 if
the port is not currently open, or -1 on error.  Error information can be retrieved by calling
QextSerialPort::getLastError().
*/
qint64 SerialPort::bytesAvailable()
{
    LOCK_MUTEX();
    if (isOpen()) {
        DWORD Errors;
        COMSTAT Status;
        bool success=ClearCommError(Handle, &Errors, &Status);
        translateError(Errors);
        if (success) {
            lastErr=E_NO_ERROR;
            UNLOCK_MUTEX();
            return Status.cbInQue + QIODevice::bytesAvailable();
        }
        UNLOCK_MUTEX();
        return (unsigned int)-1;
    }
    UNLOCK_MUTEX();
    return 0;
}

/*!
\fn void QextSerialPort::translateError(ulong error)
Translates a system-specific error code to a QextSerialPort error code.  Used internally.
*/
void SerialPort::translateError(ulong error)
{
    if (error&CE_BREAK)
        lastErr=E_BREAK_CONDITION;
    else if (error&CE_FRAME)
        lastErr=E_FRAMING_ERROR;
    else if (error&CE_IOE)
        lastErr=E_IO_ERROR;
    else if (error&CE_MODE)
        lastErr=E_INVALID_FD;
    else if (error&CE_OVERRUN)
        lastErr=E_BUFFER_OVERRUN;
    else if (error&CE_RXPARITY)
        lastErr=E_RECEIVE_PARITY_ERROR;
    else if (error&CE_RXOVER)
        lastErr=E_RECEIVE_OVERFLOW;
    else if (error&CE_TXFULL)
        lastErr=E_TRANSMIT_OVERFLOW;
}

/*!
\fn qint64 QextSerialPort::readData(char *data, qint64 maxSize)
Reads a block of data from the serial port.  This function will read at most maxlen bytes from
the serial port and place them in the buffer pointed to by data.  Return value is the number of
bytes actually read, or -1 on error.

\warning before calling this function ensure that serial port associated with this class
is currently open (use isOpen() function to check if port is open).
*/
qint64 SerialPort::readData(char *data, qint64 maxSize)
{
    DWORD retVal = 0;
    LOCK_MUTEX();
    if (!ReadFile(Handle, (void*)data, (DWORD)maxSize, & retVal, NULL))
    {
        lastErr = E_READ_FAILED;
        retVal = (DWORD)-1;
    }
    UNLOCK_MUTEX();
    return (qint64)retVal;
}

/*!
\fn qint64 QextSerialPort::writeData(const char *data, qint64 maxSize)
Writes a block of data to the serial port.  This function will write len bytes
from the buffer pointed to by data to the serial port.  Return value is the number
of bytes actually written, or -1 on error.

\warning before calling this function ensure that serial port associated with this class
is currently open (use isOpen() function to check if port is open).
*/
qint64 SerialPort::writeData(const char *data, qint64 maxSize)
{
    DWORD retVal = 0;
    LOCK_MUTEX();
    if (!WriteFile(Handle, (void*)data, (DWORD)maxSize, & retVal, NULL))
    {
        lastErr = E_WRITE_FAILED;
        retVal = (DWORD)-1;
    }
    UNLOCK_MUTEX();
    return (qint64)retVal;
}

/*!
\fn void QextSerialPort::ungetChar(char c)
This function is included to implement the full QIODevice interface, and currently has no
purpose within this class.  This function is meaningless on an unbuffered device and currently
only prints a warning message to that effect.
*/
void SerialPort::ungetChar(char c)
{
    /*meaningless on unbuffered sequential device - return error and print a warning*/
    TTY_WARNING("QextSerialPort: ungetChar() called on an unbuffered sequential device - operation is meaningless");
}

/*!
\fn void QextSerialPort::setFlowControl(FlowType flow)
Sets the flow control used by the port.  Possible values of flow are:
\verbatim
    FLOW_OFF            No flow control
    FLOW_HARDWARE       Hardware (RTS/CTS) flow control
    FLOW_XONXOFF        Software (XON/XOFF) flow control
\endverbatim
*/
void SerialPort::setFlowControl(FlowType flow)
{
    LOCK_MUTEX();
    if (Settings.FlowControl!=flow)
    {
        Settings.FlowControl=flow;
    }
    if (isOpen())
    {
        switch(flow)
        {
            /*no flow control*/
            case FLOW_OFF:
                CommConfig.dcb.fOutxCtsFlow=FALSE;
                CommConfig.dcb.fRtsControl=RTS_CONTROL_DISABLE;
                CommConfig.dcb.fInX=FALSE;
                CommConfig.dcb.fOutX=FALSE;
                SetCommConfig(Handle, &CommConfig, sizeof(COMMCONFIG));
                break;

            /*software (XON/XOFF) flow control*/
            case FLOW_XONXOFF:
                CommConfig.dcb.fOutxCtsFlow=FALSE;
                CommConfig.dcb.fRtsControl=RTS_CONTROL_DISABLE;
                CommConfig.dcb.fInX=TRUE;
                CommConfig.dcb.fOutX=TRUE;
                SetCommConfig(Handle, &CommConfig, sizeof(COMMCONFIG));
                break;

            case FLOW_HARDWARE:
                CommConfig.dcb.fOutxCtsFlow=TRUE;
                CommConfig.dcb.fRtsControl=RTS_CONTROL_HANDSHAKE;
                CommConfig.dcb.fInX=FALSE;
                CommConfig.dcb.fOutX=FALSE;
                SetCommConfig(Handle, &CommConfig, sizeof(COMMCONFIG));
                break;
        }
    }
    UNLOCK_MUTEX();
}

/*!
\fn void QextSerialPort::setParity(ParityType parity)
Sets the parity associated with the serial port.  The possible values of parity are:
\verbatim
    PAR_SPACE       Space Parity
    PAR_MARK        Mark Parity
    PAR_NONE        No Parity
    PAR_EVEN        Even Parity
    PAR_ODD         Odd Parity
\endverbatim
*/
void SerialPort::setParity(ParityType parity) {
    LOCK_MUTEX();
    if (Settings.Parity!=parity) {
        Settings.Parity=parity;
    }
    if (isOpen()) {
        CommConfig.dcb.Parity=(unsigned char)parity;
        switch (parity) {

            /*space parity*/
            case PAR_SPACE:
                if (Settings.DataBits==DATA_8) {
                    TTY_PORTABILITY_WARNING("QextSerialPort Portability Warning: Space parity with 8 data bits is not supported by POSIX systems.");
                }
                CommConfig.dcb.fParity=TRUE;
                break;

            /*mark parity - WINDOWS ONLY*/
            case PAR_MARK:
                TTY_PORTABILITY_WARNING("QextSerialPort Portability Warning:  Mark parity is not supported by POSIX systems");
                CommConfig.dcb.fParity=TRUE;
                break;

            /*no parity*/
            case PAR_NONE:
                CommConfig.dcb.fParity=FALSE;
                break;

            /*even parity*/
            case PAR_EVEN:
                CommConfig.dcb.fParity=TRUE;
                break;

            /*odd parity*/
            case PAR_ODD:
                CommConfig.dcb.fParity=TRUE;
                break;
        }
        SetCommConfig(Handle, &CommConfig, sizeof(COMMCONFIG));
    }
    UNLOCK_MUTEX();
}

/*!
\fn void QextSerialPort::setDataBits(DataBitsType dataBits)
Sets the number of data bits used by the serial port.  Possible values of dataBits are:
\verbatim
    DATA_5      5 data bits
    DATA_6      6 data bits
    DATA_7      7 data bits
    DATA_8      8 data bits
\endverbatim

\note
This function is subject to the following restrictions:
\par
    5 data bits cannot be used with 2 stop bits.
\par
    1.5 stop bits can only be used with 5 data bits.
\par
    8 data bits cannot be used with space parity on POSIX systems.

*/
void SerialPort::setDataBits(DataBitsType dataBits) {
    LOCK_MUTEX();
    if (Settings.DataBits!=dataBits) {
        if ((Settings.StopBits==STOP_2 && dataBits==DATA_5) ||
            (Settings.StopBits==STOP_1_5 && dataBits!=DATA_5)) {
        }
        else {
            Settings.DataBits=dataBits;
        }
    }
    if (isOpen()) {
        switch(dataBits) {

            /*5 data bits*/
            case DATA_5:
                if (Settings.StopBits==STOP_2) {
                    TTY_WARNING("QextSerialPort: 5 Data bits cannot be used with 2 stop bits.");
                }
                else {
                    CommConfig.dcb.ByteSize=5;
                    SetCommConfig(Handle, &CommConfig, sizeof(COMMCONFIG));
                }
                break;

            /*6 data bits*/
            case DATA_6:
                if (Settings.StopBits==STOP_1_5) {
                    TTY_WARNING("QextSerialPort: 6 Data bits cannot be used with 1.5 stop bits.");
                }
                else {
                    CommConfig.dcb.ByteSize=6;
                    SetCommConfig(Handle, &CommConfig, sizeof(COMMCONFIG));
                }
                break;

            /*7 data bits*/
            case DATA_7:
                if (Settings.StopBits==STOP_1_5) {
                    TTY_WARNING("QextSerialPort: 7 Data bits cannot be used with 1.5 stop bits.");
                }
                else {
                    CommConfig.dcb.ByteSize=7;
                    SetCommConfig(Handle, &CommConfig, sizeof(COMMCONFIG));
                }
                break;

            /*8 data bits*/
            case DATA_8:
                if (Settings.StopBits==STOP_1_5) {
                    TTY_WARNING("QextSerialPort: 8 Data bits cannot be used with 1.5 stop bits.");
                }
                else {
                    CommConfig.dcb.ByteSize=8;
                    SetCommConfig(Handle, &CommConfig, sizeof(COMMCONFIG));
                }
                break;
        }
    }
    UNLOCK_MUTEX();
}

/*!
\fn void QextSerialPort::setStopBits(StopBitsType stopBits)
Sets the number of stop bits used by the serial port.  Possible values of stopBits are:
\verbatim
    STOP_1      1 stop bit
    STOP_1_5    1.5 stop bits
    STOP_2      2 stop bits
\endverbatim

\note
This function is subject to the following restrictions:
\par
    2 stop bits cannot be used with 5 data bits.
\par
    1.5 stop bits cannot be used with 6 or more data bits.
\par
    POSIX does not support 1.5 stop bits.
*/
void SerialPort::setStopBits(StopBitsType stopBits) {
    LOCK_MUTEX();
    if (Settings.StopBits!=stopBits) {
        if ((Settings.DataBits==DATA_5 && stopBits==STOP_2) ||
            (stopBits==STOP_1_5 && Settings.DataBits!=DATA_5)) {
        }
        else {
            Settings.StopBits=stopBits;
        }
    }
    if (isOpen()) {
        switch (stopBits) {

            /*one stop bit*/
            case STOP_1:
                CommConfig.dcb.StopBits=ONESTOPBIT;
                SetCommConfig(Handle, &CommConfig, sizeof(COMMCONFIG));
                break;

            /*1.5 stop bits*/
            case STOP_1_5:
                TTY_PORTABILITY_WARNING("QextSerialPort Portability Warning: 1.5 stop bit operation is not supported by POSIX.");
                if (Settings.DataBits!=DATA_5) {
                    TTY_WARNING("QextSerialPort: 1.5 stop bits can only be used with 5 data bits");
                }
                else {
                    CommConfig.dcb.StopBits=ONE5STOPBITS;
                    SetCommConfig(Handle, &CommConfig, sizeof(COMMCONFIG));
                }
                break;

            /*two stop bits*/
            case STOP_2:
                if (Settings.DataBits==DATA_5) {
                    TTY_WARNING("QextSerialPort: 2 stop bits cannot be used with 5 data bits");
                }
                else {
                    CommConfig.dcb.StopBits=TWOSTOPBITS;
                    SetCommConfig(Handle, &CommConfig, sizeof(COMMCONFIG));
                }
                break;
        }
    }
    UNLOCK_MUTEX();
}

/*!
\fn void QextSerialPort::setBaudRate(BaudRateType baudRate)
Sets the baud rate of the serial port.  Note that not all rates are applicable on
all platforms.  The following table shows translations of the various baud rate
constants on Windows(including NT/2000) and POSIX platforms.  Speeds marked with an *
are speeds that are usable on both Windows and POSIX.
\verbatim

  RATE          Windows Speed   POSIX Speed
  -----------   -------------   -----------
   BAUD50                 110          50
   BAUD75                 110          75
  *BAUD110                110         110
   BAUD134                110         134.5
   BAUD150                110         150
   BAUD200                110         200
  *BAUD300                300         300
  *BAUD600                600         600
  *BAUD1200              1200        1200
   BAUD1800              1200        1800
  *BAUD2400              2400        2400
  *BAUD4800              4800        4800
  *BAUD9600              9600        9600
   BAUD14400            14400        9600
  *BAUD19200            19200       19200
  *BAUD38400            38400       38400
   BAUD56000            56000       38400
  *BAUD57600            57600       57600
   BAUD76800            57600       76800
  *BAUD115200          115200      115200
   BAUD128000          128000      115200
   BAUD256000          256000      115200
\endverbatim
*/
void SerialPort::setBaudRate(BaudRateType baudRate) {
    LOCK_MUTEX();
    if (Settings.BaudRate!=baudRate) {
        switch (baudRate) {
            case BAUD50:
            case BAUD75:
            case BAUD134:
            case BAUD150:
            case BAUD200:
                Settings.BaudRate=BAUD110;
                break;

            case BAUD1800:
                Settings.BaudRate=BAUD1200;
                break;

            case BAUD76800:
                Settings.BaudRate=BAUD57600;
                break;

            default:
                Settings.BaudRate=baudRate;
                break;
        }
    }
    if (isOpen()) {
        switch (baudRate) {

            /*50 baud*/
            case BAUD50:
                TTY_WARNING("QextSerialPort: Windows does not support 50 baud operation.  Switching to 110 baud.");
                CommConfig.dcb.BaudRate=CBR_110;
                break;

            /*75 baud*/
            case BAUD75:
                TTY_WARNING("QextSerialPort: Windows does not support 75 baud operation.  Switching to 110 baud.");
                CommConfig.dcb.BaudRate=CBR_110;
                break;

            /*110 baud*/
            case BAUD110:
                CommConfig.dcb.BaudRate=CBR_110;
                break;

            /*134.5 baud*/
            case BAUD134:
                TTY_WARNING("QextSerialPort: Windows does not support 134.5 baud operation.  Switching to 110 baud.");
                CommConfig.dcb.BaudRate=CBR_110;
                break;

            /*150 baud*/
            case BAUD150:
                TTY_WARNING("QextSerialPort: Windows does not support 150 baud operation.  Switching to 110 baud.");
                CommConfig.dcb.BaudRate=CBR_110;
                break;

            /*200 baud*/
            case BAUD200:
                TTY_WARNING("QextSerialPort: Windows does not support 200 baud operation.  Switching to 110 baud.");
                CommConfig.dcb.BaudRate=CBR_110;
                break;

            /*300 baud*/
            case BAUD300:
                CommConfig.dcb.BaudRate=CBR_300;
                break;

            /*600 baud*/
            case BAUD600:
                CommConfig.dcb.BaudRate=CBR_600;
                break;

            /*1200 baud*/
            case BAUD1200:
                CommConfig.dcb.BaudRate=CBR_1200;
                break;

            /*1800 baud*/
            case BAUD1800:
                TTY_WARNING("QextSerialPort: Windows does not support 1800 baud operation.  Switching to 1200 baud.");
                CommConfig.dcb.BaudRate=CBR_1200;
                break;

            /*2400 baud*/
            case BAUD2400:
                CommConfig.dcb.BaudRate=CBR_2400;
                break;

            /*4800 baud*/
            case BAUD4800:
                CommConfig.dcb.BaudRate=CBR_4800;
                break;

            /*9600 baud*/
            case BAUD9600:
                CommConfig.dcb.BaudRate=CBR_9600;
                break;

            /*14400 baud*/
            case BAUD14400:
                TTY_PORTABILITY_WARNING("QextSerialPort Portability Warning: POSIX does not support 14400 baud operation.");
                CommConfig.dcb.BaudRate=CBR_14400;
                break;

            /*19200 baud*/
            case BAUD19200:
                CommConfig.dcb.BaudRate=CBR_19200;
                break;

            /*38400 baud*/
            case BAUD38400:
                CommConfig.dcb.BaudRate=CBR_38400;
                break;

            /*56000 baud*/
            case BAUD56000:
                TTY_PORTABILITY_WARNING("QextSerialPort Portability Warning: POSIX does not support 56000 baud operation.");
                CommConfig.dcb.BaudRate=CBR_56000;
                break;

            /*57600 baud*/
            case BAUD57600:
                CommConfig.dcb.BaudRate=CBR_57600;
                break;

            /*76800 baud*/
            case BAUD76800:
                TTY_WARNING("QextSerialPort: Windows does not support 76800 baud operation.  Switching to 57600 baud.");
                CommConfig.dcb.BaudRate=CBR_57600;
                break;

            /*115200 baud*/
            case BAUD115200:
                CommConfig.dcb.BaudRate=CBR_115200;
                break;

            /*125000 baud*/
            case BAUD125000:
                TTY_PORTABILITY_WARNING("QextSerialPort Portability Warning: POSIX does not support 125000 baud operation.");
                CommConfig.dcb.BaudRate=125000;
                break;

            /*128000 baud*/
            case BAUD128000:
                TTY_PORTABILITY_WARNING("QextSerialPort Portability Warning: POSIX does not support 128000 baud operation.");
                CommConfig.dcb.BaudRate=CBR_128000;
                break;

            /*256000 baud*/
            case BAUD256000:
                TTY_PORTABILITY_WARNING("QextSerialPort Portability Warning: POSIX does not support 256000 baud operation.");
                CommConfig.dcb.BaudRate=CBR_256000;
                break;
        }
        SetCommConfig(Handle, &CommConfig, sizeof(COMMCONFIG));
    }
    UNLOCK_MUTEX();
}

/*!
\fn void QextSerialPort::setDtr(bool set)
Sets DTR line to the requested state (high by default).  This function will have no effect if
the port associated with the class is not currently open.
*/
void SerialPort::setDtr(bool set) {
    LOCK_MUTEX();
    if (isOpen()) {
        if (set) {
            EscapeCommFunction(Handle, SETDTR);
        }
        else {
            EscapeCommFunction(Handle, CLRDTR);
        }
    }
    UNLOCK_MUTEX();
}

/*!
\fn void QextSerialPort::setRts(bool set)
Sets RTS line to the requested state (high by default).  This function will have no effect if
the port associated with the class is not currently open.
*/
void SerialPort::setRts(bool set) {
    LOCK_MUTEX();
    if (isOpen()) {
        if (set) {
            EscapeCommFunction(Handle, SETRTS);
        }
        else {
            EscapeCommFunction(Handle, CLRRTS);
        }
    }
    UNLOCK_MUTEX();
}

/*!
\fn ulong QextSerialPort::lineStatus(void)
returns the line status as stored by the port function.  This function will retrieve the states
of the following lines: DCD, CTS, DSR, and RI.  On POSIX systems, the following additional lines
can be monitored: DTR, RTS, Secondary TXD, and Secondary RXD.  The value returned is an unsigned
long with specific bits indicating which lines are high.  The following constants should be used
to examine the states of individual lines:

\verbatim
Mask        Line
------      ----
LS_CTS      CTS
LS_DSR      DSR
LS_DCD      DCD
LS_RI       RI
\endverbatim

This function will return 0 if the port associated with the class is not currently open.
*/
ulong SerialPort::lineStatus(void) {
    unsigned long Status=0, Temp=0;
    LOCK_MUTEX();
    if (isOpen()) {
        GetCommModemStatus(Handle, &Temp);
        if (Temp&MS_CTS_ON) {
            Status|=LS_CTS;
        }
        if (Temp&MS_DSR_ON) {
            Status|=LS_DSR;
        }
        if (Temp&MS_RING_ON) {
            Status|=LS_RI;
        }
        if (Temp&MS_RLSD_ON) {
            Status|=LS_DCD;
        }
    }
    UNLOCK_MUTEX();
    return Status;
}

bool SerialPort::waitForReadyRead(int msecs)
{
    //@todo implement
    return false;
}

qint64 SerialPort::bytesToWrite() const
{
    return _bytesToWrite;
}

/*!
\fn void QextSerialPort::setTimeout(ulong millisec);
Sets the read and write timeouts for the port to millisec milliseconds.
Setting 0 for both sec and millisec indicates that timeouts are not used for read nor
write operations. Setting -1 indicates that read and write should return immediately.

\note this function does nothing in event driven mode.
*/
void SerialPort::setTimeout(long millisec)
{
    LOCK_MUTEX();
    Settings.Timeout_Millisec = millisec;
    if (millisec == -1)
    {
        CommTimeouts.ReadIntervalTimeout = MAXDWORD;
        CommTimeouts.ReadTotalTimeoutConstant = 0;
    }
    else
    {
        CommTimeouts.ReadIntervalTimeout = millisec;
        CommTimeouts.ReadTotalTimeoutConstant = millisec;
    }
    CommTimeouts.ReadTotalTimeoutMultiplier = 0;
    CommTimeouts.WriteTotalTimeoutMultiplier = millisec;
    CommTimeouts.WriteTotalTimeoutConstant = 0;
    SetCommTimeouts(Handle, &CommTimeouts);
    UNLOCK_MUTEX();
}
#endif
