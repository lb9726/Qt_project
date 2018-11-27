#include "protocoldeal.h"
#include <QThread>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
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
static char prestr[BVT_MAX_FRAME_LENTH] ;
static char curstr[BVT_MAX_FRAME_LENTH] ;

static unsigned char totalBuf[MAX_LENGTH]; // 串口读到的数据存储的数据缓冲区
Protocoldeal* Protocoldeal::instance = NULL;
Protocoldeal::Protocoldeal()
{
    GetDataPthread = new ProducerFromBottom;
    GetDataPthread->StartThread(GetDataPthread);
}

Protocoldeal::~Protocoldeal()
{
    GetDataPthread->requestInterruption();
    GetDataPthread->quit();
    GetDataPthread->wait();
    delete GetDataPthread;
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

//e_IDTYPE_T Protocoldeal::BstBvtPtlMonitor(void)
//{

//}

//void Protocoldeal::BstBvtCopyFrameData(e_IDTYPE_T id, void *dat)
//{

//}

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
    DatLen = BstBvtGetFrameDatLen(id);
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
unsigned long Protocoldeal::BstBvtRecoverFrame(void *src, unsigned long srclen)
{
    unsigned long Len = srclen - 2;/*去掉帧头和帧尾*/
    unsigned long TranLen = 2;/*恢复后长度,帧头和帧尾*/
    unsigned long Cnt = 0;
    unsigned char *lDst = (unsigned char*)src;
    unsigned char *lSrc = (unsigned char*)src;

    *lDst++ = *lSrc++;/*取出帧头数据*/
    while(Cnt < Len)
    {
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
    }
    *lDst = *lSrc;/*取出帧尾数据*/
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
unsigned char Protocoldeal::BstBvtGetFrameDatLen(e_IDTYPE_T id)
{
    unsigned char len = 0;
    switch(id)
    {
        case ID00_BASE: len = sizeof(s_BVTID0_T);break;
        case ID01_HEAR: len = sizeof(s_BVTID1_T);break;
        default:
            break;
    }
    return len;
}
// return a file fd
int Protocoldeal::RetFileLength(char filename[])
{
    long size;
    ifstream in(filename, ios::in|ios::binary|ios::ate);   // need include namespace std;  ios::ate 放到文件末尾
    size = in.tellg();  // 取得文件长度
    in.seekg(0, ios::beg);
    in.close();
    return size;
}

void Protocoldeal::RedFile()
{/*
    while (1)
    {
        int size = RetFileLength(FILE_DEVICE);
        if (size <= 0)
        {
            cout << FILE_DEVICE <<" is empty! "<< endl;
            cout << __PRETTY_FUNCTION__<<endl;
            continue;
        }
        cout <<" size = "<< size << endl;
        ifstream in(FILE_DEVICE, ios::in|ios::binary);// need input namespace std;
        char *buffer;
        buffer = new char[size];
        memset(buffer, 0, size);                      //全部清0
        in.read(buffer, size - 1);                    // read the length of size to buffer
        memcpy(gBvtFrameBuf, buffer, size - 1);       // 返回值之中包含了文件结束符，所以需要减掉一
        memcpy(curstr, buffer, sizeof(buffer));
        cout << "buffer = " << buffer << endl;
        cout << "gBvtFrameBuf = " << gBvtFrameBuf << endl;
        BstBvtRecoverFrame(gBvtFrameBuf, size - 1);   // 还原数据，去掉帧头帧尾或者数据中出现的帧头帧尾的处理
        cout << __PRETTY_FUNCTION__<<endl;
        memset(gBvtFrameBuf, 0, sizeof(gBvtFrameBuf));
        if (JudgeChange(prestr, curstr))
        {
            QString s = ChartoQString(buffer);
            cout << "setting sth\n";
            emit AcceptDataFormBottom(s);
            cout << "the string changes"<< endl;
        }
        memcpy(prestr, curstr, sizeof(curstr));
        delete []buffer;
        in.close();
    }*/
}

//void Protocoldeal::run()
//{
//    cout << __PRETTY_FUNCTION__<<endl;
//    SetSerialArgument();
//}

bool Protocoldeal::JudgeChange(char str[], char str2[])
{
    if (0 == strcmp(str, str2))
    {
        return false;
    }
    return true;
}

QString Protocoldeal::ChartoQString(unsigned char *str)
{
    QString qtext;
    qtext.clear();qtext = " ";
//    qtext = QString("%1").arg(str);
//    qtext = qtext.append(str);
//    qtext += str;
    qDebug() << "qtext = "<< qtext;
    return qtext;  // 漏写，出现段错误
}

QSerialPortInfo Protocoldeal::FindSerial()
{
    QSerialPortInfo com_info;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        if( info.serialNumber() == "/dev/ttymxc1" )
        {
            com_info = info;
            break;
        }
    }
    qDebug() << "Name : " << com_info.portName();
    qDebug() << "Description : " << com_info.description();
    qDebug() << "serialNumber: " << com_info.serialNumber();
    return com_info;
}
/*
void Protocoldeal::SetSerialArgument()
{
    my_serialport = new QSerialPort;
    my_serialport->setPortName("/dev/ttymxc1");
    qDebug() << "Name : " << my_serialport->portName();
//    connect(my_serialport, SIGNAL(readyRead()), this, SLOT(ReadyreadSlots()));
    if (my_serialport->open(QIODevice::ReadOnly))
    {
        cout << "enter funtion"<<endl;
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
        my_serialport->clear();
        cout << "before connect"<<endl;
        connect(my_serialport, SIGNAL(readyRead()), this, SLOT(ReadyreadSlots()));
        this->exec();  // 需要在子线程中调用线程的exec的函数，使得进入消息队列
        cout << "after connect"<<endl;
    }
    cout << "end funtion"<<endl;
}
*/
/*
void Protocoldeal::ReadyreadSlots()
{
//    QSerialPort my_serialport;
    QByteArray arr = my_serialport->readAll();
    qDebug()<< "arr = " << arr;
//    QString s;
//    s.clear();
//    s.prepend(arr);
    QString s = arr.toHex();
    qDebug()<< "s = " << s <<endl;
    cout << "setting sth\n";
    emit AcceptDataFormBottom(s);
    cout << "the string changes"<< endl;
}
*/
ProducerFromBottom::ProducerFromBottom()
{
    cout << __PRETTY_FUNCTION__<<endl;
    ProducerFromBottom_pointer = totalBuf;
    ProCounts = 0;
}

ProducerFromBottom::~ProducerFromBottom()
{

}

//配置串口参数，连接信号和槽
void ProducerFromBottom::SetSerialArgument()
{
    my_serialport = new QSerialPort;
    my_serialport->setPortName("/dev/ttymxc1");
    qDebug() << "Name : " << my_serialport->portName();
//    connect(my_serialport, SIGNAL(readyRead()), this, SLOT(ReadyreadSlots()));
    if (my_serialport->open(QIODevice::ReadOnly))  //使用只读的方式打开串口
    {
        cout << "enter funtion"<<endl;
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
        my_serialport->clear();
        cout << "before connect"<<endl;
        connect(my_serialport, SIGNAL(readyRead()), this, SLOT(ReadyreadSlots()), Qt::DirectConnection);
        //this->exec();  // 需要在子线程中调用线程的exec的函数，使得进入消息队列
        // 在子线程中使用this->exec()的话，将会导致，主线程的connect的槽函数收不到信号
        cout << "after connect"<<endl;
    }
//    Protocoldeal *Protocol = Protocoldeal::GetInstance();
////    Protocol->BstBvtRecoverFrame(totalBuf, j);
//    emit Protocol->AcceptDataFormBottom("send");
//    cout << "end funtion"<<endl;
}

//void ProducerFromBottom::ReadyreadSlots()
//{
//    QByteArray arr = my_serialport->readAll();
//    qDebug()<< "arr = " << arr;

//    QString s = arr.toHex();
//    qDebug()<< "s = " << s <<endl;
//    CopySerialDataToBuf(arr);
//    cout << "setting sth\n";
//    cout << "the string changes"<< endl;
//}

void ProducerFromBottom::ReadyreadSlots()
{
    static bool Isstart = false;
    char str;
    unsigned long i = 0;
    unsigned long j = 0;
    while(1)
    {
        my_serialport->read(&str, 1); // 每次读取一个字节到str中存储
        printf("%X\n", str);
        if (BVT_STX == str)
        {
            Isstart = true;
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
                break;
            }
        }
    }
    Protocoldeal *Protocol = Protocoldeal::GetInstance();
    Protocol->BstBvtRecoverFrame(totalBuf, j);
    emit Protocol->AcceptDataFormBottom("send");
    cout << "send message"<< endl;
    for(i = 0; i < j; i++)
    {
        //cout<< totalBuf[i]<< " ";
        printf("%X ", totalBuf[i]);
    }
    printf("\n");printf("funck !!!\n");
    qDebug() << "totalBuf"<< totalBuf <<endl;
    cout << "i = "<< i <<" "<<"j = "<< j << endl;
    cout << "setting sth\n";
    cout << "the string changes"<< endl;
}

//将串口数据拷贝到缓冲区
void ProducerFromBottom::CopySerialDataToBuf(QByteArray arr)
{
    static int position = 0;      //相对于数组首地址的偏移
    ConsumerFromBottom CFormBottom;
    int len = arr.length();
    // 当本次从串口获取的数据长度加上当前位置小于最大长度时，继续拷贝，否则从头拷贝
    if (len + position < MAX_LENGTH)
    {
        // 完整写数据的次数超过完整读数据的次数一次时 0~4095计作完整的一次
        if (1 == ProCounts - CFormBottom.GetConCounts())
        {
            // 完整写数据的次数超过完整读数据的次数一次时,
            // 此时写数据的指针位置必须是比读指针的位置小的时候才能写入数据，否则会覆盖掉未读取的数据
            if (totalBuf + position < CFormBottom.GetPointPosition())
            {
                memcpy(totalBuf + position, arr, len); // 从上一次结束的位置的下一个位置进行拷贝
                position = position + len;             // 拷贝结束，更新position的值
            }
        }
        // 完整写数据的次数等于完整读数据的次数时
        else if (0 == ProCounts - CFormBottom.GetConCounts())
        {
            // 写数据的次数等于读数据的次数时
            // 此时写数据的指针位置大于读数据的指针位置时，才可以继续写入数据
            if (totalBuf + position > CFormBottom.GetPointPosition())
            {
                memcpy(totalBuf + position, arr, len); // 从上一次结束的位置的下一个位置进行拷贝
                position = position + len;             // 拷贝结束，更新position的值
            }
        }
        else
        {
            // 超过1次之后不能再往缓冲区写入数据
        }
    }
    else
    {
        position = 0;
        ProCounts ++;  // 0～4095计数一次计算完整的读写次数
        memcpy(totalBuf + position, arr, len);
        position = position + len;
    }
    ProducerFromBottom_pointer += arr.length() - 1;
    qDebug()<<"ProducerFromBottom_pointer = "<< *ProducerFromBottom_pointer;
    qDebug()<<"ProducerFromBottom_pointer = "<< *ProducerFromBottom_pointer;
}

void ProducerFromBottom::run()
{
    cout << __PRETTY_FUNCTION__<<endl;
    SetSerialArgument();
}

// 开启线程
void ProducerFromBottom::StartThread(ProducerFromBottom *p)
{
    cout << __PRETTY_FUNCTION__<<endl;
    if (!(p->isRunning()))  // 当线程不在运行时，启动线程
    {
        p->start();
    }
}

ConsumerFromBottom::ConsumerFromBottom()
{
    cout << __PRETTY_FUNCTION__<<endl;
    ConsumerFromBottom_pointer = totalBuf;
}

ConsumerFromBottom::~ConsumerFromBottom()
{

}

unsigned char* ConsumerFromBottom::GetPointPosition()
{
    return ConsumerFromBottom_pointer;
}

int ConsumerFromBottom::GetConCounts()
{
    qDebug()<< __PRETTY_FUNCTION__ << "ConCounts = "<< ConCounts;
    return ConCounts;
}

void ConsumerFromBottom::SetConCounts(int counts)
{
    qDebug() << __PRETTY_FUNCTION__ <<" before ConCounts = "<< ConCounts;
    ConCounts = counts;
    qDebug() << __PRETTY_FUNCTION__ <<" after ConCounts = "<< ConCounts;
}



