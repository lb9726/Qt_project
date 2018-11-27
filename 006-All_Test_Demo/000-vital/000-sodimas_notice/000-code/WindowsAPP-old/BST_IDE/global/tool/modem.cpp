#include "modem.h"

Modem::Modem(QIODevice *pDev, QObject *parent) :
    QObject(parent)
{
    m_FrameState = State_Wait;
    m_StopFlag = false;

    m_ModemFlag = 0;
    m_FrameHead = 0;
    m_FrameEnd = 0;
    m_ModemAck = 0;
    m_FrameAck = 0;
    m_FrameBuffer = 0;
    m_FrameSize = 1;
    Init(pDev);
}

Modem::~Modem()
{
    Release();
}

void Modem::Init(QIODevice *pDev)
{
    Release();
    m_Device = pDev;
    m_FrameState = State_Wait;

    m_ModemFlag = (char*)malloc(4);
    m_FrameHead = (char*)malloc(3);
    m_FrameEnd = (char*)malloc(2);
    m_FrameBuffer = (char*)malloc(1024);
    m_ModemAck = (char*)malloc(1);
    m_FrameAck = (char*)malloc(1);

    m_FrameSize = 1024;
    strcpy(m_ModemFlag, "IAP");
    m_FrameHead[0] = 0x02;
    m_FrameHead[1] = 0x00;
    m_FrameHead[2] = 0x00;
    m_FrameEnd[0] = 0x00;
    m_FrameEnd[1] = 0x00;
    memset(m_FrameBuffer, 0, 1024);
    m_ModemAck[0] = 0x43;
    m_FrameAck[0] = 0x06;
}

void Modem::Release()
{
    if(m_ModemFlag)
        free(m_ModemFlag);
    if(m_FrameHead)
        free(m_FrameHead);
    if(m_FrameEnd)
        free(m_FrameEnd);
    if(m_ModemAck)
        free(m_ModemAck);
    if(m_FrameAck)
        free(m_FrameAck);
    if(m_FrameBuffer)
        free(m_FrameBuffer);
}

int ReadWait(QIODevice *pDev, char* pBuffer, int pMaxNum, int pTimeout)
{
    volatile int tmpCount = 0;
    int tmpTimeout = pTimeout/10;
    while((tmpCount++ < tmpTimeout) && pDev->bytesAvailable() < pMaxNum)
        QtSleep(10);
    return pDev->read(pBuffer, (qint64)pMaxNum);
}

QByteArray ReadWait(QIODevice *pDev, int pMaxNum, int pTimeout)
{
    volatile int tmpCount = 0;
    int tmpTimeout = pTimeout/10;
    while((tmpCount++ < tmpTimeout) && pDev->bytesAvailable() < pMaxNum)
        QtSleep(10);
    return pDev->read((qint64)pMaxNum);
}

bool Modem::SendIAP(int pTimeout)
{
    if(m_Device.isNull())
        return false;
    QIODevice *tmpDev = m_Device.data();
    if(!tmpDev)
        return false;
    if(m_FrameState != State_Wait)
        return true;
    m_StopFlag = false;
    volatile int tmpCount = 0;
    int tmpTimeout = pTimeout/100;
    char tmpRet = 0;
    while(!m_StopFlag && (tmpCount++ < tmpTimeout) && (tmpRet != m_ModemAck[0]))
    {
        tmpDev->write(m_ModemFlag, 3);
        ReadWait(tmpDev, &tmpRet, 1, 100);
    }
    if(tmpRet == m_ModemAck[0])
    {
        m_FrameState = State_Data;
        return true;
    }
    return false;
}

void Modem::Stop()
{
    m_StopFlag = true;
    m_FrameState = State_Wait;
}

bool Modem::Send(QByteArray pArray, int pTimeout)
{
    if(m_Device.isNull())
        return false;
    QIODevice *tmpDev = m_Device.data();
    if(!tmpDev)
        return false;
    if(m_FrameState != State_Data)
        return false;
    if(pArray.isEmpty())
        return false;
    tmpDev->write(m_FrameHead, 3);
    tmpDev->write(pArray);
    tmpDev->write(m_FrameEnd, 2);

    volatile int tmpCount = 0;
    int tmpTimeout = pTimeout/100;
    char tmpRet = 0;
    while(!m_StopFlag && (tmpCount++ < tmpTimeout) && (tmpRet != m_FrameAck[0]))
        ReadWait(tmpDev, &tmpRet, 1, 100);
    if(tmpRet == m_FrameAck[0])
        return true;
    return false;
}

bool Modem::SendEnd(int pTimeout)
{
    if(m_Device.isNull())
        return false;
    QIODevice *tmpDev = m_Device.data();
    if(!tmpDev)
        return false;
    if(m_FrameState != State_Data)
        return true;
    m_StopFlag = false;
    char tmpEOT = EOT;
    tmpDev->write(&tmpEOT, 1);
    volatile int tmpCount = 0;
    int tmpTimeout = pTimeout/100;
    char tmpRet = 0;
    while(!m_StopFlag && (tmpCount++ < tmpTimeout) && (tmpRet != ACK))
        ReadWait(tmpDev, &tmpRet, 1, 100);
    if(tmpRet == ACK)
    {
        m_FrameState = State_Wait;
        return true;
    }
    return false;
}

bool Modem::SendFile(QString pFile, int pTimeout)
{
    QFile tmpFile(pFile);
    if(!tmpFile.open(QFile::ReadOnly))
        return false;

    bool tmpSendFlag = false;
    if(!SendIAP(10000))
    {
        tmpFile.close();
        return false;
    }
    int tmpFrmCount = tmpFile.size() / m_FrameSize;
    int tmpTimeDiff = pTimeout / tmpFrmCount;
    int tmpFrmNum = 0;
    QByteArray tmpArray;
    do{
        tmpArray = tmpFile.read(m_FrameSize);
        if(tmpArray.isEmpty())
            break;
        if(tmpArray.count() < m_FrameSize)
        {
            for(int i=tmpArray.count();i<m_FrameSize;i++)
                tmpArray.append(0x1a);
        }
        tmpFrmNum ++;
        IDE_TRACE_INT(tmpFrmNum);
        emit sProgress(tmpFrmNum*100/tmpFrmCount);
    }while(tmpSendFlag = Send(tmpArray, tmpTimeDiff));
    tmpSendFlag = SendEnd(1000);
    tmpFile.close();
    if(tmpSendFlag)
        return true;
    return false;
}

bool Modem::GetIAP(int pTimeout)
{
    if(m_Device.isNull())
        return false;
    QIODevice *tmpDev = m_Device.data();
    if(!tmpDev)
        return false;
    if(m_FrameState != State_Wait)
        return true;
    m_StopFlag = false;
    volatile int tmpCount = 0;
    int tmpTimeout = pTimeout/100;
    char *tmpRet = (char*)malloc(4);
    while(!m_StopFlag && (tmpCount++ < tmpTimeout))
    {
        ReadWait(tmpDev, tmpRet, 3, 100);
        if(!memcmp(tmpRet, m_ModemFlag, 3))
        {
            tmpDev->write(m_ModemAck, 1);  //>@·¢ËÍÂÖÑ¯×Ö·ûC
            m_FrameState = State_Data;
            return true;
        }
    }
    return false;
}

QByteArray Modem::Get(bool &pEnd, int pTimeout)
{
    if(m_Device.isNull())
        return QByteArray();
    QIODevice *tmpDev = m_Device.data();
    if(!tmpDev)
        return QByteArray();
    if(m_FrameState != State_Data)
        return QByteArray();
    int tmpMaxNum = 1024+3+2;
    QByteArray tmpArray = ReadWait(tmpDev, tmpMaxNum, pTimeout);
    if(tmpArray.count() < 5)
        return QByteArray();
    quint8 tmpHead0 = (quint8)tmpArray.at(0);
    quint8 tmpHead1 = (quint8)tmpArray.at(1);
    quint8 tmpHead2 = (quint8)tmpArray.at(2);
    if(tmpHead0 != m_FrameHead[0] ||
       tmpHead1 != m_FrameHead[1] ||
       tmpHead2 != m_FrameHead[2])
        return QByteArray();
    quint8 tmpEnd0 = (quint8)tmpArray.at(tmpArray.count()-2);
    quint8 tmpEnd1 = (quint8)tmpArray.at(tmpArray.count()-1);
    if(tmpEnd0 != m_FrameEnd[0] ||
       tmpEnd1 != m_FrameEnd[1])
        return QByteArray();
    pEnd = (tmpMaxNum == tmpArray.count())?false:true;
    return tmpArray.mid(3,tmpArray.count()-5);
}

bool Modem::GetFile(QString pFile, int pTimeout)
{
    QFile tmpFile(pFile);
    if(!tmpFile.open(QFile::WriteOnly))
        return false;

    if(!GetIAP(10000))
    {
        tmpFile.close();
        return false;
    }
    bool tmpEnd = false;
    QByteArray tmpArray;
    int tmpFrmNum = 0;
    while(!tmpEnd)
    {
        tmpArray = Get(tmpEnd, pTimeout);
        tmpFrmNum ++;
        emit sProgress(tmpFrmNum);
        tmpFile.write(tmpArray);
    }
    tmpFile.close();
    return true;
}

//SerialPort *tmpPort = new SerialPort("COM8", BAUD115200);
//tmpPort->open(QIODevice::ReadWrite);
//Modem *tmpModem = new Modem(tmpPort);
//tmpModem->SendFile(QString("d:/test.bin"), 5000000);
//IDE_TRACE();
