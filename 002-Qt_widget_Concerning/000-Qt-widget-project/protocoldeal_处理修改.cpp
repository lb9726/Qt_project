#include "protocoldeal.h"
#include <QThread>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
using namespace std;

#define BVT_ESC 0x1B	/* 转换字符 */
#define BVT_STX 0x80	/* 帧起始字符 */
#define BVT_ETX 0x81	/* 帧结束字符 */
#define BVT_STX_AF 0xE7	/* 帧起始转换后增加字符 */
#define BVT_ETX_AF 0xE8	/* 帧结束转换后增加字符 */
#define BVT_ESC_AF 0x00	/* 转换字符转换后增加字符 */

#define BVT_MAX_FRAME_LENTH 44	/* 最大帧字节长度 */
#define BVT_SEND_INTERVAL 50	/* 发送数据间隔,单位ms */
#define BVT_RECV_CHANNEL_SIZE 1	/* 接收通道数据类型大小 */
#define BVT_PTL_FRAMEID_POS 1	/* 协议帧ID地址 */
#define BVT_PTL_DATASTART_POS 2	/* 协议帧数据地址 */
#define MAX_LENGTH 4096         /*缓冲区大小*/
static unsigned char gBvtRecStatus ;
static unsigned char gBvtRecCnt ;
static unsigned char gBvtFrameBuf[BVT_MAX_FRAME_LENTH] ;
static char WriteDataBuf[BVT_MAX_FRAME_LENTH] ;
//static char curstr[BVT_MAX_FRAME_LENTH] ;

static unsigned char totalBuf[MAX_LENGTH]; // 串口读到的数据存储的数据缓冲区
static unsigned char tempBuf[1024];        // 去掉帧头和帧尾，并且解析好的数据的缓冲区
static unsigned long StringSize;
static unsigned long position;
messagetable mestable;
Protocoldeal* Protocoldeal::instance = NULL;
//QTimer *timer;
QSerialPort *my_serialport = NULL;

Protocoldeal::Protocoldeal()
{
    SetSerialArgument();     //配置串口参数，连接信号和槽
    ReadDataPthread = new ProducerFromBottom;
    ReadDataPthread->StartThread(ReadDataPthread);

    WriteDataPthread = new WriteDataToBottom;
    WriteDataPthread->StartThread(WriteDataPthread);

    connect(my_serialport, SIGNAL(readyRead()), ReadDataPthread, SLOT(ReadyreadSlots()), Qt::QueuedConnection);
    position = 0;
    cout << __PRETTY_FUNCTION__<<"启动协议的的构造函数"<<endl;
}

Protocoldeal::~Protocoldeal()
{
    ReadDataPthread->requestInterruption();
    ReadDataPthread->quit();
    ReadDataPthread->wait();
    delete ReadDataPthread;

    WriteDataPthread->requestInterruption();
    WriteDataPthread->quit();
    WriteDataPthread->wait();
    delete WriteDataPthread;

    CloseSerial();
    delete my_serialport;
    cout << __PRETTY_FUNCTION__<<"启动协议的的析构函数"<<endl;
}

// 单例模式
Protocoldeal* Protocoldeal::GetInstance()
{
    if (NULL == instance)
    {
        instance = new Protocoldeal();
    }
    return instance;
}

void Protocoldeal::BstBvtPtlInit()
{

}

/*!
 * BstBvtSetFrameData
 * 设置帧数据
 * @param[in]   id：帧ID	dat：帧数据地址
 * @param[out]  None
 * @retval  	None
 */
void Protocoldeal::BstBvtSetFrameData(e_IDTYPE_T id, void *dat)
{
    unsigned char TranBuf[BVT_MAX_FRAME_LENTH] = {0};
    unsigned char FrameBuf[BVT_MAX_FRAME_LENTH] = {0};
    unsigned char *pFrameBuf = FrameBuf;
    unsigned char *pStarVer;
    unsigned char DatLen = 0;
    unsigned long TranLen = 0;/*转换后帧长度*/

    if(id >= ID_UNKNOW)/*未知ID*/
        return;
//    DatLen = BstBvtGetFrameDatLen(id);
    *pFrameBuf++ = BVT_STX;	/*加入帧头*/
    pStarVer = pFrameBuf;
    *pFrameBuf++ = id;	/*加入ID*/
    BstFifoMemCpy(pFrameBuf,dat, DatLen);/*加入数据*/
    pFrameBuf += DatLen;
    *pFrameBuf++ = BstBvtVerify(pStarVer, DatLen+1);/*加入校验值*/
    *pFrameBuf++ = BVT_ETX;/*加入帧尾*/
//    TranLen = BstBvtTransformFrame(FrameBuf,pFrameBuf-FrameBuf,TranBuf);/*帧转换*/
//    SendFifoChannelPush(BST_BVTSEND_USE,TranBuf,TranLen);/*压入发送FIFO*/
}

//CRC 数据校验
/*!
 * BstBvtVerify
 * 协议校验算法
 * @param[in]   data：待校验的数据指针	length：待校验的数据长度
 * @param[out]  None
 * @retval  	None
 */
unsigned char Protocoldeal::BstBvtVerify(unsigned char *data, unsigned long length)
{
    unsigned char result = 0;
    unsigned int i;

    for (i = 0; i < length; i++)
    {
        result = result ^ data[i];
    }
    return result & 0x7f;
}

/*!
 * BstBvtRecoverFrame
 * 协议去除字符转换，恢复数据
 * @param[in]   src：原数据地址 len：原数据长度
 * @param[out]  None
 * @retval  	恢复后的帧长度
 */
unsigned long Protocoldeal::BstBvtRecoverFrame(void *des, void *src, unsigned long srclen)
{
    unsigned long Len = srclen - 2; /*去掉帧头和帧尾*/
    unsigned long TranLen = 0;      /*数据域的数据长度*/
    unsigned long Cnt = 0;
    unsigned char *lDst = (unsigned char*)des;
    unsigned char *lSrc = (unsigned char*)src;
    qDebug()<< "srclen = "<< srclen;
    if (0 == srclen)
    {
        qDebug()<< "buf is empty ";
        return TranLen;
    }
    lSrc++; /*取出跳过帧头数据*/
    while(Cnt < Len)
    {qDebug()<< "enter while = ";
        if(BVT_ESC == *lSrc)                   //如果是转换字符
        {
            if(*(lSrc + 1) == BVT_STX_AF)      //转换后的帧头
                *lDst++ = BVT_STX;
            else if(*(lSrc + 1) == BVT_ETX_AF) //转换后的 帧尾
                *lDst++ = BVT_ETX;
            else if(*(lSrc + 1) == BVT_ESC_AF) //转换后的 转换符
                *lDst++ = BVT_ESC;
            lSrc += 2;
            Cnt += 2;
        }else{
            *lDst++ = *lSrc++;
            Cnt++;
        }
        TranLen++;
    }qDebug()<< "TranLen = "<< TranLen;
    //*lDst = *lSrc; /*取出帧尾数据*/
    return TranLen;
}

//内存拷贝
void Protocoldeal::BstFifoMemCpy(unsigned char *pFrameBuf,void* dat, unsigned char DatLen)
{
    memcpy(pFrameBuf, dat, DatLen);
}

/*!
 * BstBvtGetFrameDatLen
 * 获取帧数据长度
 * @param[in]   id：帧ID
 * @param[out]  None
 * @retval  	帧ID对应的数据长度
 */
unsigned char Protocoldeal::BstBvtGetFrameDatLen(unsigned char id)
{
    unsigned char len = 0;
    switch(id)
    {
        case 0x00: len = sizeof(s_BVTID0_T);break;
        case 0x01: len = sizeof(s_BVTID1_T);break;
        default:
            break;
    }
    return len;
}

//比较unsigned char 类型的字符串是否相同
bool Protocoldeal::StringCompare(unsigned char *temp, unsigned char *str, unsigned long len)
{
    unsigned long i = 0;
    for (i = 0; i < len; i++)
    {
        if(temp[i] != str[i])
        {
            cout << "the different index i = " << i << endl;
            return true;
        }
    }
    cout << "the string is same as before i = " << i << endl;
    return false;
}

//
bool Protocoldeal::AllocteMemory(void *p)
{
    if (NULL == p)
    {
        printf("allcote memory is failed");
        return true;
    }
    return false;
}

// 检测发送的新的数据帧的内容是否有变更
bool Protocoldeal::JudgeChange(unsigned char ID, unsigned char str[])
{
    unsigned long len;
    unsigned char *temp;
    bool AllocteFlag = false;
    len = BstBvtGetFrameDatLen(ID);      // 需要拷贝的ID的数据长度
    temp = (unsigned char *)malloc(len); // 分配内存
    if (!AllocteMemory(temp))
    {
        memset(temp, 0, len);
        switch(ID)
        {
        case 0x00:
                memcpy(temp, &mestable.ID0_Message, len);// 拷贝信息表中的数据到temp中，用来和新收到的数据进行比较
                AllocteFlag = true;
            break;
        case 0x01:
            AllocteFlag = true;
            break;
        case 0x02:
            AllocteFlag = true;
            break;
        default:
            AllocteFlag = true;
            break;
        }
        if (AllocteFlag)
        {
            cout << "AllocteFlag = " << AllocteFlag << endl;
            PrintString(temp, len);
            AllocteFlag = false;
            if (StringCompare(temp, str, len))
            {
                free(temp);
                temp = NULL;
                return true;  // true 表示变更了
            }
            else
            {
                free(temp);
                temp = NULL;
                return false;
            }
        }
    }
}

/*应用层调用从协议层拷贝数据到应用层的接口*/
void Protocoldeal::CopyStringFromProtocol(unsigned char Id, void *str)
{
    if (0x00 == Id)
    {
        memcpy(str, tempBuf, StringSize);
        cout << "Id == " << "0x00 in if()"<< endl;
    }
    else if (0x01 == Id)
    {
        memcpy(str, tempBuf, StringSize);
        cout << "Id == " << "0x01 in else if()"<< endl;
    }
    cout << "StringSize = 拷贝string的长度"<< StringSize << endl;
}

void Protocoldeal::PrintString(unsigned char *src, unsigned long length)
{
    cout << __PRETTY_FUNCTION__<<endl;
    unsigned long i = 0;
    for (i = 0; i < length; i++)
    {
        printf("%X ", src[i]);
    }
    printf("\n");
}

//配置串口参数，连接信号和槽
void Protocoldeal::SetSerialArgument()
{
    if (NULL == my_serialport)
    {
        my_serialport = new QSerialPort;
    }
    my_serialport->setPortName(SERIAL_DEVICE);
    qDebug() << "Name : " << my_serialport->portName();
    bool OpenSerialFlag = false;
    if (my_serialport->open(QIODevice::ReadWrite))  //使用读写的方式打开串口
    {
        cout << __PRETTY_FUNCTION__<< "enter funtion"<<endl;
        //设置波特率
        my_serialport->setBaudRate(QSerialPort::Baud9600);
        //设置数据位
        my_serialport->setDataBits(QSerialPort::Data8);
        //设置校验位
        my_serialport->setParity(QSerialPort::NoParity);
        //设置流控制
        my_serialport->setFlowControl(QSerialPort::NoFlowControl);
        //设置停止位
        my_serialport->setStopBits(QSerialPort::OneStop);
        my_serialport->clearError();
        OpenSerialFlag = true;
    }
    if (!OpenSerialFlag)
        qDebug()<<__PRETTY_FUNCTION__<< "OpenSerialFlag = "<< OpenSerialFlag << "打开串口失败!";
    else
        qDebug()<<__PRETTY_FUNCTION__<< "OpenSerialFlag = "<< OpenSerialFlag << "打开串口成功!";
}

// 当线程结束时，将打开的串口关闭掉
void Protocoldeal::CloseSerial()
{
    if (NULL != my_serialport)
    {
        qDebug() <<__PRETTY_FUNCTION__ <<"close readserialport success!";
        my_serialport->close(); // 关闭串口
    }
}

ProducerFromBottom::ProducerFromBottom()
{
    cout << __PRETTY_FUNCTION__<<"启动读串口的线程的构造函数"<<endl;
}

ProducerFromBottom::~ProducerFromBottom()
{
//    CloseSerial();
    cout << __PRETTY_FUNCTION__<<"启动读串口的线程的析构函数"<<endl;
}

// 读串口数据函数

void ProducerFromBottom::ReadyreadSlots()
{
    cout << __PRETTY_FUNCTION__ << "读数据的槽函数"<<endl;
    static bool Isstart = false;
    char str;
    unsigned long i = 0;
    unsigned long j = 0;
    unsigned long count = 0;
    while(1)
    {
        int flag = my_serialport->read(&str, 1); // 每次读取一个字节到str中存储
        if (0 == flag)
        {
            qDebug()<< "读不到数据退出";
            break;
        }

        count ++;
        printf("%X\n", str);
        if (BVT_STX == str)
        {
            Isstart = true;
            i = 0;
            totalBuf[i++] = (unsigned char)str;
        }
        else if(Isstart)      // 如果遇到帧头，则将遇到帧尾之前的所有数据保存下来
        {
            totalBuf[i++] = (unsigned char)str;

            if (BVT_ETX == str)
            {
                Isstart = false;
                j = i;
                i = 0;
                qDebug()<< "遇到帧尾退出";
                break;
            }
        }
    }

    printf("count = %d\n", count);
    Protocoldeal *Protocol = Protocoldeal::GetInstance();
    StringSize = Protocol->BstBvtRecoverFrame(tempBuf, totalBuf, j);
    printf("接收到的数据为 totalBuf = ");
    Protocol->PrintString(totalBuf, j);
    printf("解析后的数据为 tempBuf = ");
    Protocol->PrintString(tempBuf, StringSize);
    if ( tempBuf[j-1] == Protocol->BstBvtVerify(tempBuf, j)) // 数据校验
    {
        printf("tempBuf[0] = %X \n", tempBuf[0]);
        cout <<" verify is true "<< endl;
        if (Protocol->JudgeChange(tempBuf[0], tempBuf))
        {
            printf("emit message\n");
            emit Protocol->AcceptDataFormBottom(tempBuf[0]);
        }
    }

//    memcpy(&mestable.ID0_Message, tempBuf, sizeof(mestable));
//    printf("ID0_Message.ID = %X \n", mestable.ID0_Message.ID);
//    printf("Data1 = %X \n", mestable.ID0_Message.Data1);
//    printf("Data2 = %X \n", mestable.ID0_Message.Data2);
//    printf("Data3 = %X \n", mestable.ID0_Message.Data3);
//    printf("Data4 = %X \n", mestable.ID0_Message.Data4);
//    printf("ArrowStatus = %X \n", mestable.ID0_Message.ArrowStatus);
//    printf("LiftSpecialStatus = %X \n", mestable.ID0_Message.LiftSpecialStatus);
//    printf("StationClockStatus = %X \n", mestable.ID0_Message.StationClockStatus);
//    printf("StationLightStatus = %X \n", mestable.ID0_Message.StationLightStatus);
}
//void ProducerFromBottom::CharToUnsignedChar(char *str, unsigned long len)
//{
//    if (len <= 0)
//    {
//        qDebug()<< "len < 0 donothing!";
//        return;
//    }
//    unsigned long i = 0;
//    for (i = 0; i < len; i++)
//    {
//        totalBuf[i] = (unsigned char)str[i];
//    }
//    qDebug()<< __PRETTY_FUNCTION__ << "len = "<< len;
//}

//int ProducerFromBottom::CheckWholeString(unsigned char *str, unsigned long len)
//{
//    unsigned long i = 0;
//    bool HaveHead = false;
//    bool HaveTail = false;
//    qDebug()<< __PRETTY_FUNCTION__ << "len = "<< len;
//    for (i = 0; i < len; i++)
//    {
//        if (BVT_STX == str[i])         // 找到帧头
//            HaveHead = true;
//        else if (BVT_ETX == str[i])    // 找到帧尾
//            HaveTail = true;
//    }
//    if (HaveHead && HaveTail)          // 完整数据帧
//    {
//        position = 0;
//        qDebug()<< __PRETTY_FUNCTION__ << "HaveHead= "<<" HaveTail= "<<" position = "
//                <<HaveHead << HaveTail <<position;
//        return 1;
//    }
//    else if (HaveHead &&(!HaveTail))   // 只有帧头，未找到帧尾
//    {
//        position = position + len;
//        qDebug()<< __PRETTY_FUNCTION__ << "HaveHead= "<<" HaveTail= "<<" position = "
//                <<HaveHead << HaveTail <<position;
//        return 2;
//    }
//    else if (!HaveHead && (!HaveTail)) // 帧头帧尾都没有
//    {
//        position = position + len;
//        qDebug()<< __PRETTY_FUNCTION__ << "HaveHead= "<<" HaveTail= "<<" position = "
//                <<HaveHead << HaveTail <<position;
//        return 3;
//    }
//    else if (!HaveHead && HaveTail)    // 只有帧尾
//    {
//        position = position + len;
//        qDebug()<< __PRETTY_FUNCTION__ << "HaveHead= "<<" HaveTail= "<<" position = "
//                <<HaveHead << HaveTail <<position;
//        return 4;
//    }
//}

// 读串口数据函数
//void ProducerFromBottom::ReadyreadSlots()
//{
//    cout << __PRETTY_FUNCTION__ << "读数据的槽函数"<<endl;
//    QByteArray arr = my_serialport->readAll();
//    char *str = arr.data();
//    int len = arr.length();
//    qDebug()<< "arr.length = " << len;
//    CharToUnsignedChar(str, len);
//    int flag = CheckWholeString(totalBuf, len);
//    Protocoldeal *Protocol = Protocoldeal::GetInstance();
//    if (1 == flag)   // 一包完整的数据帧时
//    {
//        StringSize = Protocol->BstBvtRecoverFrame(tempBuf, totalBuf, j);
//        printf("接收到的数据为 totalBuf = ");
//        Protocol->PrintString(totalBuf, j);
//        printf("解析后的数据为 tempBuf = ");
//        Protocol->PrintString(tempBuf, StringSize);
//        if ( tempBuf[j-1] == Protocol->BstBvtVerify(tempBuf, j)) // 数据校验
//        {
//            printf("tempBuf[0] = %X \n", tempBuf[0]);
//            cout <<" verify is true "<< endl;
//            if (Protocol->JudgeChange(tempBuf[0], tempBuf))
//            {
//                printf("emit message\n");
//                emit Protocol->AcceptDataFormBottom(tempBuf[0]);
//            }
//        }
//    }
//    else if (2 == flag) // 只有帧头，未找到帧尾
//    {

//    }
//    else if (3 == flag) // 帧头帧尾都没有
//    {

//    }
//    else if (4 == flag) // 只有帧尾
//    {

//    }
//}

unsigned long Protocoldeal::GetDataLength()
{
    return StringSize;
}

void ProducerFromBottom::run()
{
    cout << __PRETTY_FUNCTION__ << "配置串口"<<endl;
//    SetSerialArgument();  // 配置串口，连接信号，传输数据
}

// 开启线程
void ProducerFromBottom::StartThread(ProducerFromBottom *p)
{
    cout << __PRETTY_FUNCTION__<<endl;
    if (NULL != p)
    {
        if (!(p->isRunning()))  // 当线程不在运行时，启动线程
        {
            qDebug() << "启动读取串口的线程";
            p->start();
        }
    }
}

WriteDataToBottom::WriteDataToBottom()
{
    cout << __PRETTY_FUNCTION__<<"启动写线程的构造函数"<<endl;
//    ConsumerFromBottom_pointer = totalBuf;
}

WriteDataToBottom::~WriteDataToBottom()
{
    mtimer->stop();
    delete mtimer;
    cout <<__PRETTY_FUNCTION__<<"启动写线程的析构函数"<< endl;
}

void WriteDataToBottom::run()
{
    qDebug() <<__PRETTY_FUNCTION__ <<"Will setArgument";
    QTimer *mytimer = new QTimer;
    mtimer = mytimer;
    connect(mytimer, SIGNAL(timeout()), this, SLOT(WriteDataSerial()), Qt::QueuedConnection);
    mytimer->start(1500);
    WriteDataBuf[0] = 0x80;
    WriteDataBuf[1] = 0x00;
    WriteDataBuf[2] = 0x01;
    WriteDataBuf[3] = 0x00;
    WriteDataBuf[4] = 0x00;
    WriteDataBuf[5] = 0x33;
    WriteDataBuf[6] = 0x33;
    WriteDataBuf[7] = 0x81;
    this->exec();
}

void WriteDataToBottom::WriteDataSerial()
{
    if (NULL != my_serialport)
    {
        qDebug() << __PRETTY_FUNCTION__ <<"write data";
        qint64 len = my_serialport->write(WriteDataBuf, 8);
        qDebug() << "write length = " << len;
    }
}

// 启动线程
void WriteDataToBottom::StartThread(WriteDataToBottom *w)
{
    cout << __PRETTY_FUNCTION__ <<endl;
    // 开启线程
    if (NULL != w)
    {
        if (!(w->isRunning())) // 当线程不在运行时，启动线程
        {
            cout <<" 启动写数据串口的线程 "<< endl;
            w->start();
        }
    }
}
