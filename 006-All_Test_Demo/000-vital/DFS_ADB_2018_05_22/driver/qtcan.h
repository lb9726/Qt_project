#ifndef QTCAN_H
#define QTCAN_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "qtcanlib/ControlCAN.h"

#ifdef __cplusplus
}
#endif

#include "qtuart.h"

class CanPort;

/*! 初始化CAN接口时需要传入信息 */
typedef struct {
    UD_UINT32 ID;			/*! 帧ID */
    UD_BOOL   Extend;       /*! 是否扩展帧 */
} UD_CAN_ID;

/*! CAN帧数据结构 */
typedef struct{
    UD_CAN_ID  ID;
    UD_UINT32  Length;		/*! 数据长度 */
    UD_UINT8   Data[8];		/*! 数据 */
    UD_BOOL    Flag;        /*! 忙标志*/
}  UD_CAN_FRAME;

class CanMonitorThread;

class CanPort : public CommPort
{
    Q_OBJECT
    friend class CanMonitorThread;
public:
    CanPort(QObject *parent = 0);
    virtual ~CanPort();

    virtual void setPortNum(int pNum);
    virtual int portNum();

    virtual void setBaudRate(quint32 pBaud);
    virtual quint32 baudRate();

    virtual bool open(OpenMode mode);
    virtual void close();

    void Init(int pPort, int pBaud);

    VCI_CAN_OBJ *readNext();
    qint64 write(VCI_CAN_OBJ *pFrame);
    qint64 write(UD_CAN_FRAME *pData);

    virtual void flush();

    virtual qint64 size();
    virtual qint64 bytesAvailable();
    virtual bool atEnd();

    virtual qint64 readData(char * data, qint64 maxSize);
    virtual qint64 writeData(const char * data, qint64 maxSize);

    void SetMonitorThread(CanMonitorThread* pThread) {mMonitorThread=pThread; mExtThread=true;}
    void StartMonitor();
    void StopMonitor();
    void Monitor();

public:
    static quint32      mDevOpenCount;

    CanMonitorThread*   mMonitorThread;
    bool                mExtThread;
    quint32             mMonitorCount;

    int                 mPortNum;
    quint32             mBaudRate;

    VCI_CAN_OBJ         mCanObjBuf[2500];
    int                 mCanBufLen;  //当前接收的有效帧个数
    int                 mCanBufIndex;
};

class CanMonitorThread : public QThread
{
    Q_OBJECT
public:
    CanMonitorThread(CanPort *qesp)
        :QThread()
    {
        mQesp = qesp;
        mRun = true;
    }
    ~CanMonitorThread()
    {
        Stop();
    }
    void Stop()
    {
        mRun = false;
    }
    virtual void run()
    {
        while (mRun)
        {
            mQesp->Monitor();
            usleep(30000);
        }
        mRun = true;
    }
public:
    CanPort                *mQesp;
    bool                    mRun;
};

#endif // QTCAN_H


