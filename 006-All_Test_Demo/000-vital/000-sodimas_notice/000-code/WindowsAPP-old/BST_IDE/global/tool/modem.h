#ifndef MODEM_H
#define MODEM_H

#include "global.h"
#include "serialport.h"

#define SOH  0x01 //SOH: 包头
#define EOT  0x04 //EOT: 结束文件传输
#define ACK  0x06 //ACK: 正常响应，如：数据包正确接收
#define NACK  0x15 //NACK: 非正常响应
#define CAN  0x18 //CAN: 取消文件传输
#define XMC  0x43 //C: ASCII字符C

class Modem : public QObject
{
    Q_OBJECT
public:
    Modem(QIODevice *pDev, QObject *parent = 0);
    ~Modem();

    virtual void Init(QIODevice *pDev);
    void Release();

    void Stop();
    QByteArray read(int pMaxNum, int pTimeout);

    bool SendIAP(int pTimeout);
    bool Send(QByteArray pArray, int pTimeout);
    bool SendFile(QString pFile, int pTimeout);
    bool SendEnd(int pTimeout);

    bool GetIAP(int pTimeout);
    QByteArray Get(bool &pEnd, int pTimeout);
    bool GetFile(QString pFile, int pTimeout);

signals:
    void sProgress(int pValue);
    void sFinished();

public:
    QPointer<QIODevice>       m_Device;

    char                     *m_ModemFlag;
    char                     *m_ModemAck;
    char                     *m_FrameHead;
    char                     *m_FrameEnd;
    char                     *m_FrameBuffer;
    char                     *m_FrameAck;
    int                       m_FrameSize;  //>@帧大小

    bool                      m_StopFlag;
    FRAME_STATE               m_FrameState;
};

#endif // MODEM_H
