#include "qtcan.h"

quint32 CanPort::mDevOpenCount = 0;

CanPort::CanPort(QObject *parent) : CommPort(parent)
{
    mExtThread = false;
    mMonitorThread = 0;
    mMonitorCount = 0;
}

CanPort::~CanPort()
{
}

void CanPort::StartMonitor()
{
    if(!mMonitorThread)
        mMonitorThread = new CanMonitorThread(this);
    if(!mMonitorThread->isRunning())
        mMonitorThread->start(QThread::LowPriority);
}

void CanPort::StopMonitor()
{
    if(mMonitorThread != 0)
    {
        mMonitorThread->Stop();
        mMonitorThread->wait(1000);
        mMonitorThread->deleteLater();
        mMonitorThread = 0;
    }
}

//void CanPort::Monitor()
//{
//    mMonitorCount++;
//    if(isOpen())
//    {
//        mCanBufLen = VCI_Receive(VCI_USBCAN1, 0, mPortNum, mCanObjBuf, 2000, 0);
//        mCanBufIndex = 0;
//        if(mCanBufLen > 0)
//        {
//            VCI_CAN_OBJ *tmpFrame = 0;
//            while(tmpFrame = readNext())
//            {
//                UD_CAN_FRAME tmpCanFrame;
//                tmpCanFrame.ID.Extend = tmpFrame->ExternFlag;
//                tmpCanFrame.ID.ID = tmpFrame->ID;
//                tmpCanFrame.Length = tmpFrame->DataLen;
//                memcpy(tmpCanFrame.Data, tmpFrame->Data, tmpFrame->DataLen);
//                emit sReadyRead((void*)(&tmpCanFrame), 1);
//            }
//        }
//        else if(mCanBufLen < 0) //USB掉线
//        {
//            mDevOpenCount = 0;
//            QIODevice::close();  //设置未打开的标志
//        }
//    }
//    else
//    {
//        if(mMonitorCount%10 == 0) //每10x30ms执行一次
//        {
//            open(ReadWrite);
//        }
//    }
//}
void CanPort::Monitor()
{
    mMonitorCount++;
    if(isOpen())
    {
        mCanBufLen = VCI_Receive(VCI_USBCAN1, 0, mPortNum, mCanObjBuf, 2000, 0);
        if(mCanBufLen > 0)
        {
            emit readyRead();
            mCanBufIndex = 0;
        }
        else if(mCanBufLen < 0) //USB掉线
        {
            mCanBufIndex = 0;
            mDevOpenCount = 0;
            QIODevice::close();  //设置未打开的标志
        }
    }
    else
    {
        if(mMonitorCount%10 == 0) //每10x30ms执行一次
        {
            open(ReadWrite);
        }
    }
}


void CanPort::setPortNum(int pNum)
{
    mPortNum = pNum;
}

int CanPort::portNum()
{
    return mPortNum;
}

void CanPort::setBaudRate(quint32 baudRate)
{
    mBaudRate = baudRate;
}

quint32 CanPort::baudRate(void)
{
    return mBaudRate;
}

void CanPort::Init(int pPort, int pBaud)
{
    setBaudRate(pBaud);
    setPortNum(pPort);
    if(!isOpen())
        open(QIODevice::ReadWrite);
}

bool CanPort::open(OpenMode mode)
{
    if(QIODevice::isOpen())
    {
        return true;
    }

    if(mDevOpenCount == 0)
    {
        DWORD Reserved = 0;
        //打开设备
        if(VCI_OpenDevice(VCI_USBCAN1,0,Reserved)!=1)
            return false;
    }
    mDevOpenCount++;

    VCI_INIT_CONFIG InitInfo[1];
    if(mBaudRate == 5000)
    {
        InitInfo->Timing0=0xBF;
        InitInfo->Timing1=0xFF;
    }
    else if(mBaudRate == 10000)
    {
        InitInfo->Timing0=0x31;
        InitInfo->Timing1=0x1C;
    }
    else if(mBaudRate == 20000)
    {
        InitInfo->Timing0=0x18;
        InitInfo->Timing1=0x1C;
    }
    else if(mBaudRate == 25000)
    {
        InitInfo->Timing0=0x0F;
        InitInfo->Timing1=0x2F;
    }
    else if(mBaudRate == 40000)
    {
        InitInfo->Timing0=0x87;
        InitInfo->Timing1=0xFF;
    }
    else if(mBaudRate == 50000)
    {
        InitInfo->Timing0=0x09;
        InitInfo->Timing1=0x1C;
    }
    else if(mBaudRate == 80000)
    {
        InitInfo->Timing0=0x83;
        InitInfo->Timing1=0xFF;
    }
    else if(mBaudRate == 100000)
    {
        InitInfo->Timing0=0x04;
        InitInfo->Timing1=0x1C;
    }
    else if(mBaudRate == 125000)
    {
        InitInfo->Timing0=0x03;
        InitInfo->Timing1=0x1C;
    }
    else if(mBaudRate == 200000)
    {
        InitInfo->Timing0=0x81;
        InitInfo->Timing1=0xFA;
    }
    else if(mBaudRate == 250000)
    {
        InitInfo->Timing0=0x01;
        InitInfo->Timing1=0x1C;
    }
    else if(mBaudRate == 400000)
    {
        InitInfo->Timing0=0x80;
        InitInfo->Timing1=0xFA;
    }
    else if(mBaudRate == 500000)
    {
        InitInfo->Timing0=0x00;
        InitInfo->Timing1=0x1C;
    }
    else if(mBaudRate == 666000)
    {
        InitInfo->Timing0=0x80;
        InitInfo->Timing1=0xB6;
    }
    else if(mBaudRate == 800000)
    {
        InitInfo->Timing0=0x00;
        InitInfo->Timing1=0x16;
    }
    else if(mBaudRate == 1000000)
    {
        InitInfo->Timing0=0x00;
        InitInfo->Timing1=0x14;
    }
    InitInfo->Filter=0;
    InitInfo->AccCode=0x80000008;
    InitInfo->AccMask=0xFFFFFFFF;
    InitInfo->Mode=0;  //0为正常模式，2位自测模式
    //初始化通道0
    if(VCI_InitCAN(VCI_USBCAN1, 0, mPortNum, InitInfo) != 1)
        return false;

    QtComSleep(100);

    //使能通道0
    if(VCI_StartCAN(VCI_USBCAN1, 0, mPortNum) != 1)	//can-0
        return false;

    mCanBufLen = 0;
    mCanBufIndex = 0;

    StartMonitor();
    return QIODevice::open(mode);
}

void CanPort::close()
{
    StopMonitor();

    if(mDevOpenCount != 0)
        mDevOpenCount--;
    if(mDevOpenCount == 0)
    {
        VCI_CloseDevice(VCI_USBCAN1,0);
    }
    QIODevice::close();
}

void CanPort::flush()
{
    if (isOpen())
    {
        mCanBufIndex = mCanBufLen= 0;
    }
}

bool CanPort::atEnd()
{
    if (isOpen())
    {
        return (bool)(mCanBufLen == mCanBufIndex);
    }
    return true;
}

qint64 CanPort::size()
{
    if (isOpen())
    {
        return (mCanBufLen-mCanBufIndex);
    }
    mCanBufIndex = mCanBufLen = 0;
    return 0;
}

qint64 CanPort::bytesAvailable()
{
    return size();
}

//VCI_CAN_OBJ *CanPort::readNext()
//{
//    if(mCanBufLen > 0 && !atEnd())
//    {
//        return &(mCanObjBuf[mCanBufIndex++]);
//    }
//    return (VCI_CAN_OBJ*)0;
//}

VCI_CAN_OBJ *CanPort::readNext()
{
    if(mCanBufLen > 0 && !atEnd())
    {
        if(mCanBufIndex < mCanBufLen)
            return &(mCanObjBuf[mCanBufIndex++]);
    }
    return (VCI_CAN_OBJ*)0;
}

qint64 CanPort::write(VCI_CAN_OBJ *pFrame)
{
    ULONG flag = VCI_Transmit(VCI_USBCAN1, 0, mPortNum, pFrame, 1);//CAN message send
    if(flag == -1)  //USB掉线
    {
        mDevOpenCount = 0;
        QIODevice::close();  //设置未打开的标志
        return -1;
    }
    else if(flag == 0)
    {
        return -1;
    }
    return (qint64)flag;
}

qint64 CanPort::write(UD_CAN_FRAME *pData)
{
    VCI_CAN_OBJ tmpFrame;
    tmpFrame.ExternFlag = pData->ID.Extend;
    tmpFrame.ID = pData->ID.ID;
    tmpFrame.DataLen = pData->Length;
    memcpy(tmpFrame.Data, pData->Data, pData->Length);
    return write((VCI_CAN_OBJ *)(&tmpFrame));
}

qint64 CanPort::readData(char *data, qint64 maxSize)
{
    DWORD retVal = 0;
    return (qint64)retVal;
}

qint64 CanPort::writeData(const char *data, qint64 maxSize)
{
    DWORD retVal = 0;
    return (qint64)retVal;
}

