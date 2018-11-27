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
#define MAX_LENGTH 2048         /*缓冲区大小*/
const char *path = "/home/root/BVD241.bin";
//const char *mount = "mount /dev/sda1 /media";
const char *usbpath = "/media/update/BVD241.bin_3";
static unsigned char VersionInfo[2] ;  // 保存单片机发送的版本信息
//static unsigned char UsbVersion[2];    //
static char WriteDataBuf[1024] ;
static char WriteSrc[1000];
static unsigned char totalBuf[MAX_LENGTH]; // 串口读到的数据存储的数据缓冲区
static unsigned char tempBuf[1024];        // 去掉帧头和帧尾，并且解析好的数据的缓冲区
static unsigned long StringSize;
static unsigned long position;
static unsigned long UiWriteSize;
messagetable mestable;
UpdateVersion upv;
Protocoldeal* Protocoldeal::instance = NULL;
QSerialPort *my_serialport = NULL;

Protocoldeal::Protocoldeal():
    ContinueFlag(10),
    RunNormalFlag(0),
    VersionComFlag(false),
    UsbInsertFlag(false)
{
    SetSerialArgument();     //配置串口参数，连接信号和槽
    connect(this, SIGNAL(WriteToSerialSignal(char*,ulong)), this, SLOT(WriteToSerialSlots(char*,ulong)));

    RThread = new QThread(this);
    ReadDataPthread = new ProducerFromBottom;
    ReadDataPthread->moveToThread(RThread);
    connect(my_serialport, SIGNAL(readyRead()), ReadDataPthread, SLOT(ReadyreadSlots()), Qt::QueuedConnection);
    RThread->start();

    WThread = new QThread(this);
    WriteDataPthread = new WriteDataToBottom;
    WriteDataPthread->moveToThread(WThread);
    connect(WriteDataPthread, SIGNAL(FillDataSignal(char*,char*)), WriteDataPthread, SLOT(ConstructWriteData(char*,char*)),Qt::QueuedConnection);
    connect(WriteDataPthread, SIGNAL(WriteDataSignal()), WriteDataPthread, SLOT(WriteDataSerial()), Qt::QueuedConnection);
    WThread->start();

    UsbDetect = new QDeviceWatcher;
    UsbDetect->appendEventReceiver(this);
    upd = new UpdateData;
    qDebug()<<"主线程ID为："<<QThread::currentThreadId();
    connect(UsbDetect, SIGNAL(deviceAdded(QString)), this, SLOT(AddUsbSlots(QString)), Qt::QueuedConnection);
//    connect(my_serialport, SIGNAL(readyRead()), ReadDataPthread, SLOT(ReadyreadSlots()), Qt::QueuedConnection);
    connect(this, SIGNAL(StartCompareSignal(unsigned char*,unsigned char*)), this, SLOT(CompareVersion(unsigned char*,unsigned char*)), Qt::QueuedConnection);
    connect(this, SIGNAL(UpdateFlagSignal()), this, SLOT(OnUpdateSlots()), Qt::QueuedConnection);
    position = 0;
    GetUpdateVersion(path, &upv);  // 从文件中获取版本信息
    UsbDetect->start();
    cout << __PRETTY_FUNCTION__<<"调用协议的的构造函数"<<endl;
}

Protocoldeal::~Protocoldeal()
{
//    ReadDataPthread->requestInterruption();
//    ReadDataPthread->quit();
//    ReadDataPthread->wait();

    RThread->requestInterruption();
    RThread->quit();
    RThread->wait();
    delete ReadDataPthread;
    delete RThread;
//    WriteDataPthread->requestInterruption();
//    WriteDataPthread->quit();
//    WriteDataPthread->wait();

    WThread->requestInterruption();
    WThread->quit();
    WThread->wait();
    delete WriteDataPthread;
    delete WThread;

    delete UsbDetect;
    CloseSerial();
    delete my_serialport;
    delete upd;
    delete instance;
    cout << __PRETTY_FUNCTION__<<"调用协议的的析构函数"<<endl;
}

// 单例模式
Protocoldeal* Protocoldeal::GetInstance()
{
    if (NULL == instance)
    {
        instance = new Protocoldeal();
        qDebug("%s just enter first", __PRETTY_FUNCTION__);
    }
    qDebug("%s enter when call on", __PRETTY_FUNCTION__);
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
    unsigned char FrameBuf[BVT_MAX_FRAME_LENTH] = {0};
    unsigned char *pFrameBuf = FrameBuf;
    unsigned char *pStarVer;
    unsigned char DatLen = 0;

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
    qDebug()<< __PRETTY_FUNCTION__<<"length = "<< length;
    unsigned char result = 0;
    unsigned int i;
    if (length <= 0)
    {
        qDebug()<< "校验失败,长度有误!";
        return result;
    }
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
    if (2 >= srclen)
    {
        qDebug()<< "buf is empty ";
        return TranLen;
    }
    lSrc++; /*取出跳过帧头数据*/
    while(Cnt < Len)
    {
        qDebug()<< "enter while";
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
    qDebug()<< "TranLen = "<< TranLen;
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
unsigned long Protocoldeal::BstBvtGetFrameDatLen(unsigned char id)
{
    qDebug()<< __PRETTY_FUNCTION__;
    unsigned long len = 0;
    switch(id)
    {
    case 0x00: len = sizeof(s_BVTID0_T); break;
    case 0x01: len = sizeof(s_BVTID1_T); break;
    case 0x02: len = sizeof(s_BVTID2_T); break;
    case 0x03: len = sizeof(s_BVTID3_T); break;
    case 0x04: len = sizeof(s_BVTID4_T); break;
    case 0x05: len = sizeof(s_BVTID5_T); break;
    case 0x06: len = sizeof(s_BVTID6_T); break;
//    case 0x07: len = sizeof(s_BVTID7_T); break;
    case ID07_UPDATEDATA: len = sizeof(s_BVTID7_T);break;
    default: qDebug()<<"Id error"; break;
    }
    qDebug("id = %x, len = %ld",id, len);
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

// 分配内存是否成功
bool Protocoldeal::AllocteMemory(void *p)
{
    if (NULL == p)
    {
        qDebug("allcote memory is failed");
        return true;
    }
    return false;
}

// 检测发送的新的数据帧的内容是否有变更
bool Protocoldeal::JudgeChange(unsigned char ID, unsigned char str[])
{
    qDebug()<<__PRETTY_FUNCTION__;
    unsigned long len;
//    unsigned char *temp;
    bool AllocteFlag = false;
    len = BstBvtGetFrameDatLen(ID);      // 需要拷贝的ID的数据长度
//    temp = (unsigned char *)malloc(len); // 分配内存
    static unsigned char temp[200];
    if (!AllocteMemory(temp))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"allocate success!"<< "ID = " << ID;
        memset(temp, 0, len);
        switch(ID)
        {
        case 0x00:
            memcpy(temp, &mestable.ID0_Message, len);// 拷贝信息表中的数据到temp中，用来和新收到的数据进行比较
            AllocteFlag = true;
            break;
        case 0x01:
            memcpy(temp, &mestable.ID1_Message, len);
            AllocteFlag = true;
            break;
        case 0x02:
            AllocteFlag = true;
            break;
        case 0x03:
            AllocteFlag = true;
            break;
        case 0x04:
            AllocteFlag = true;
            break;
        case 0x05:
            AllocteFlag = true;
            break;
        case 0x06:
            AllocteFlag = true;
            break;
        case ID07_UPDATEDATA:
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
                qDebug()<< "before free";
//                free(temp);
                qDebug()<< "after free";
//                temp = NULL;
                qDebug()<< "set NULL";
                return true;  // true 表示变更了
            }
            else
            {
                qDebug()<< "can not changes";
                qDebug()<< "before free";
//                free(temp);
                qDebug()<< "after free";
//                temp = NULL;
                qDebug()<< "set NULL";
                return false;
            }
        }
    }
    qDebug()<<__PRETTY_FUNCTION__<<"allocate failure!";
    return false;
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
    memset(tempBuf, 0, sizeof(tempBuf));
    cout << "StringSize = 拷贝string的长度"<< StringSize << endl;
}

// 从UI层拷贝数据到协议层,根据Id指定拷贝的数据长度
void Protocoldeal::CopyStringFromUi(unsigned char Id, void *str)
{
    cout << __PRETTY_FUNCTION__<<" ";
    printf("Id = %x\n",Id);
    unsigned long strlength = BstBvtGetFrameDatLen(Id);
    memset(WriteSrc, 0, sizeof(WriteSrc));
    if (0x07 == Id) // 读文件时需要追加Id
    {
        WriteSrc[0] = Id;
        memcpy(WriteSrc+1, str, strlength - 1);
        qDebug("读文件时需要追加Id");
    }
    else
    {
        memcpy(WriteSrc, str, strlength);
    }
    qDebug("emit FillDataSignal(WriteDataBuf, WriteSrc)");
    emit WriteDataPthread->FillDataSignal(WriteDataBuf, WriteSrc);
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
    qDebug()<<__PRETTY_FUNCTION__<<"当前线程为："<<QThread::currentThread();
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

unsigned long Protocoldeal::GetDataLength()
{
    return StringSize;
}

void Protocoldeal::SetContinueFlag(unsigned char buf[])
{
    ContinueFlag = 10;
    qDebug()<< __PRETTY_FUNCTION__;
    if (0x06 == buf[0] && 0x00 == buf[1])     // 正常运行
    {
        ContinueFlag = -1;
    }
    else if (0x06 == buf[0] && 0x01 == buf[1])// 请求成功
    {
        ContinueFlag = 1;
    }
    else if (0x06 == buf[0] && 0x02 == buf[1])// 收到升级数据包为真
    {
        ContinueFlag = 2;
    }
    else if (0x06 == buf[0] && 0x03 == buf[1])// 收到数据包有误
    {
        ContinueFlag = 3;
    }
    else                                      // 没有收到回应
    {
        ContinueFlag = 0;
    }
    qDebug()<<"ContinueFlag = "<< ContinueFlag;
}

void Protocoldeal::SetContinueFlag(int num)
{
    qDebug()<< __PRETTY_FUNCTION__<< "before flag = "<< ContinueFlag;
    ContinueFlag = num;
    qDebug()<< __PRETTY_FUNCTION__<< "after flag = "<< ContinueFlag;
}

int Protocoldeal::GetContinueFlag()
{
    qDebug()<< __PRETTY_FUNCTION__<<"ContinueFlag = "<< ContinueFlag;
    return ContinueFlag;
}

// 接收并保存版本信息
void Protocoldeal::RevVersion(unsigned char buffer[], unsigned char version[])
{
    qDebug()<< __PRETTY_FUNCTION__;
    if (0x04 == buffer[0])
    {
        qDebug("接收到版本号信息，buffer[0]= %x", buffer[0]);
        memcpy(version, buffer+1, 2);
        qDebug("接收到版本号信息，version[0]= %x,version[1]= %x", version[0], version[1]);
        qDebug("emit StartCompareSignal(version, upv.ver)");
        emit StartCompareSignal(version, upv.ver); // 通知开始进行版本比较
    }
}

// 从指定的文件中指定的位置获取升级的版本信息
void Protocoldeal::GetUpdateVersion(const char *filename, UpdateVersion *Uver)
{
    qDebug()<< __PRETTY_FUNCTION__;
    QFile file(filename);
    char buf[5];
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug("打开文件%s失败!", filename);
        return;
    }
    file.seek(OffsetHead);
    memset(Uver->ver, 0, sizeof(Uver->ver)); // 使用前先清为0
    file.read(buf, 5);
    qDebug("%x %x %x %x %x", buf[0], buf[1], buf[2], buf[3], buf[4]);
    CountString(Uver->ver, buf, 5);
    qDebug("verinfo[0] = %x,verinfo[1] = %x", Uver->ver[0], Uver->ver[1]);
    file.close();
//    emit UpdateFlagSignal();
}

// 将五个字节的版本信息，转化为2个字节的，方便与接收到的2个字节的版本信息比较
void Protocoldeal::CountString(unsigned char *des, char *src, int len)
{
    qDebug()<< __PRETTY_FUNCTION__;
    int i = 0;
    unsigned char interger = 0;
    unsigned char decimal = 0;

    while ('.' != src[i])
    {
        if (src[i] > '0' && src[i] < '9')
            interger = interger * 10 + src[i] - '0';
        i++;
    }
    i++;
    int j ;
    for (j = i; j < len; j ++)
    {
        if (src[i] > '0' && src[i] < '9')
            decimal = decimal * 10 + src[i] - '0';
    }
    des[0] = interger;  // 整数部分
    des[1] = decimal;   // 小数部分
    qDebug("des[0] = %d, des[1] = %d", des[0], des[1]);
}

// 版本在协议中为2个字节的
void Protocoldeal::CompareVersion(unsigned char *Revversion, unsigned char *Readversion)
{
    qDebug()<< __PRETTY_FUNCTION__;
    unsigned long i;
    for (i = 0; i < 2; i++)
    {
        if (Revversion[i] != Readversion[i])
        {
            if (Revversion[i] < Readversion[i]) // 修改代码，做测试准备
            {
                qDebug()<< "获取到的版本比当前版本新，需升级，返回true";
                VersionComFlag = true;    // 需要升级，返回true
                return;
            }
            else
            {
                qDebug()<< "获取到的版本比当前版本旧，无需升级，返回false";
                VersionComFlag = false;   // 获取到的版本比当前版本旧，不用升级，返回false
                return;
            }
        }
    }
    qDebug()<< "版本号相同，无需升级，返回false";
    VersionComFlag = false;
    return;    // 版本号相同，无需升级，返回false
}

bool Protocoldeal::GetVersionFlag()
{
    qDebug()<< __PRETTY_FUNCTION__<< "versionflag = "<< VersionComFlag;
    return VersionComFlag;
}

void Protocoldeal::SetVersionFlag(bool flag)
{
    VersionComFlag = flag;
    qDebug()<< __PRETTY_FUNCTION__<< "versionflag = "<< VersionComFlag;
}

void Protocoldeal::OnUpdateSlots()
{
    qDebug()<< __PRETTY_FUNCTION__;
    upd->start();
//    if (GetVersionFlag())
//    {
//        qDebug("show Fileupdate screen");
//        emit ShowWhichScreen(0);
//        SetVersionFlag(false);
//    }
//    else
//    {
//        qDebug("show liftscren");
////        emit ShowWhichScreen(1);
//    }
}

void Protocoldeal::AddUsbSlots(QString dev)
{
    qDebug()<< __PRETTY_FUNCTION__;
    QString str;
    str = "mount " + dev + " /media";
    qDebug()<<"str = "<< str;
    QByteArray ba = str.toLatin1();
    char *mount = ba.data();
    qDebug("mount = %s", mount);
    system(mount);      // 挂载ｕ盘到／media目录下
    QThread::sleep(4);
    GetUpdateVersion(usbpath, &upv);   //从usb中获取版本
    CompareVersion(VersionInfo, upv.ver); //对版本进行比较
    OnUpdateSlots();
}

void Protocoldeal::NotifyCompare(unsigned char *buf)
{
    qDebug()<< __PRETTY_FUNCTION__;
    if (0x04 == buf[0])
    {
        emit UpdateFlagSignal();
    }
}

void Protocoldeal::SetUsbInsertFlag(bool flag)
{
    qDebug()<< __PRETTY_FUNCTION__<<"before UsbInsertFlag = "<< UsbInsertFlag;
    UsbInsertFlag = flag;
    qDebug()<< __PRETTY_FUNCTION__<<"after UsbInsertFlag = "<< UsbInsertFlag;
}

bool Protocoldeal::GetUsbInsertFlag()
{
    qDebug()<< __PRETTY_FUNCTION__<<"UsbInsertFlag = "<< UsbInsertFlag;
    return UsbInsertFlag;
}

void Protocoldeal::WriteToSerialSlots(char *buf, unsigned long leng)
{
    if (NULL != my_serialport)
    {
        qDebug()<<"write data";
        qDebug()<<__PRETTY_FUNCTION__<<"当前线程为："<<QThread::currentThread();
        qint64 len = my_serialport->write(buf, leng);
        qDebug() << "write length = " << len;
    }
}

ProducerFromBottom::ProducerFromBottom()
{
    cout << __PRETTY_FUNCTION__<<"启动读串口的线程的构造函数"<<endl;
}

ProducerFromBottom::~ProducerFromBottom()
{
    cout << __PRETTY_FUNCTION__<<"启动读串口的线程的析构函数"<<endl;
}

// 读串口数据函数

void ProducerFromBottom::ReadyreadSlots()
{
    cout << __PRETTY_FUNCTION__ << "读数据的槽函数"<<endl;
    qDebug()<<"读线程ID为："<<QThread::currentThreadId();
    qDebug()<<__PRETTY_FUNCTION__<<"当前线程为："<<QThread::currentThread();
    static bool Isstart = false;
    char str;
    unsigned char s;
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
        s = (unsigned char)str;
        count ++;
        printf("%X ", s);
        if (BVT_STX == s)
        {
            Isstart = true;
            i = 0;
            totalBuf[i++] = s /*(unsigned char)str*/;
        }
        else if(Isstart)      // 如果遇到帧头，则将遇到帧尾之前的所有数据保存下来
        {
            totalBuf[i++] = s /*(unsigned char)str*/;

            if (BVT_ETX == s)
            {
                Isstart = false;
                j = i;
                i = 0;
                qDebug()<< "遇到帧尾退出";
                break;
            }
        }
    }

    printf("count = %ld\n", count);
    Protocoldeal *Protocol = Protocoldeal::GetInstance();
    StringSize = Protocol->BstBvtRecoverFrame(tempBuf, totalBuf, j);
    Protocol->SetContinueFlag(tempBuf);
    Protocol->RevVersion(tempBuf, VersionInfo); // 从单片机中获取保存版本信息
    printf("接收到的数据为 totalBuf = ");
    Protocol->PrintString(totalBuf, j);
    printf("解析后的数据为 tempBuf = ");
    Protocol->PrintString(tempBuf, StringSize);
    Protocol->NotifyCompare(tempBuf);
    memcpy(&mestable.ID0_Message, tempBuf, sizeof(mestable));
    if ( tempBuf[j-1] == Protocol->BstBvtVerify(tempBuf, j)) // 数据校验
    {
        printf("tempBuf[0] = %X \n", tempBuf[0]);
        cout <<" verify is true "<< endl;
        if (Protocol->JudgeChange(tempBuf[0], tempBuf))
        {
            printf("emit message\n");
            emit Protocol->AcceptDataFormBottom(tempBuf[0]);
        }
        else
        {
            memset(tempBuf, 0, StringSize);
            qDebug("接收的数据没有改变，清空数据");
        }
    }
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

//void ProducerFromBottom::run()
//{
//    cout << __PRETTY_FUNCTION__ << "配置串口"<<endl;
//    connect(my_serialport, SIGNAL(readyRead()), this, SLOT(ReadyreadSlots()), Qt::QueuedConnection);
//    this->exec();
//}

// 开启线程
//void ProducerFromBottom::StartThread(ProducerFromBottom *p)
//{
//    cout << __PRETTY_FUNCTION__<<endl;
//    if (NULL != p)
//    {
//        if (!(p->isRunning()))  // 当线程不在运行时，启动线程
//        {
//            qDebug() << "启动读取串口的线程";
//            p->start();
//        }
//    }
//}

WriteDataToBottom::WriteDataToBottom()
{
    cout << __PRETTY_FUNCTION__<<"调用写线程的构造函数"<<endl;
}

WriteDataToBottom::~WriteDataToBottom()
{
    cout <<__PRETTY_FUNCTION__<<"调用写线程的析构函数"<< endl;
}

//void WriteDataToBottom::run()
//{
//    qDebug() <<__PRETTY_FUNCTION__ <<"Will setArgument";
//    connect(this, SIGNAL(FillDataSignal(char*,char*)), this, SLOT(ConstructWriteData(char*,char*)),
//            Qt::QueuedConnection);
//    connect(this, SIGNAL(WriteDataSignal()), this, SLOT(WriteDataSerial()), Qt::QueuedConnection);
//    this->exec();
//}

// 组成一帧完整的包含帧头帧尾和校验值的数据帧
void WriteDataToBottom::ConstructWriteData(char *wstr, char *src)
{
    qDebug() << __PRETTY_FUNCTION__<< "写线程ID为："<<QThread::currentThreadId();
    qDebug()<<__PRETTY_FUNCTION__<<"当前线程为："<<QThread::currentThread();
    unsigned long i = 0;
    unsigned long counts = 0;
    memset(WriteDataBuf, 0, sizeof(WriteDataBuf));  // 先清空上一次的数据
    unsigned long len = Protocoldeal::GetInstance()->BstBvtGetFrameDatLen((unsigned char)src[0]);
    src[len] = GenerateDataVerifyForChar(src, len); // 先在用户拷贝的数据后面加入校验位
    wstr[counts++] = BVT_STX;  // 在需要写的数据开头加上帧头
    for (i = 0; i < len + 1; i++) //遍历用户数据进行数据帧转换，加上帧头帧尾以及对数据中出现的帧头帧尾和转义字符替换
    {
        if (BVT_STX == (unsigned char)src[i])
        {
            wstr[counts++] = BVT_ESC;
            wstr[counts++] = BVT_STX_AF;
        }
        else if (BVT_ETX == (unsigned char)src[i])
        {
            wstr[counts++] = BVT_ESC;
            wstr[counts++] = BVT_ETX_AF;
        }
        else if (BVT_ESC == (unsigned char)src[i])
        {
            wstr[counts++] = BVT_ESC;
            wstr[counts++] = BVT_ESC_AF;
        }
        else
        {
            wstr[counts++] = src[i];
        }
    }
    wstr[counts++] = BVT_ETX;
    UiWriteSize = counts;
    printf("即将要write的数据为 ");
    for(i = 0;i < counts; i++)
        printf("%X ", wstr[i]);
    printf("\n");
    qDebug()<<" 转换后的数据长度为 UiWriteSize =" << UiWriteSize;
    qDebug("emit WriteDataSignal()");
    emit WriteDataSignal();
}

// 向串口写数据
void WriteDataToBottom::WriteDataSerial()
{
    qDebug() << __PRETTY_FUNCTION__<< "写线程ID为："<<QThread::currentThreadId();
//    if (NULL != my_serialport)
//    {
//        qDebug()<<"write data";
//        qDebug()<<__PRETTY_FUNCTION__<<"当前线程为："<<QThread::currentThread();
//        qint64 len = my_serialport->write(WriteDataBuf, UiWriteSize);
//        qDebug() << "write length = " << len;
//    }
    /*因为my_serialport是在主线程中定义的，直接在子线程中使用会出现问题
    QObject: Cannot create children for a parent that is in a different thread.*/
    Protocoldeal::GetInstance()->WriteToSerialSignal(WriteDataBuf, UiWriteSize);
}

// 启动线程
//void WriteDataToBottom::StartThread(WriteDataToBottom *w)
//{
//    cout << __PRETTY_FUNCTION__ <<endl;
//    // 开启线程
//    if (NULL != w)
//    {
//        if (!(w->isRunning())) // 当线程不在运行时，启动线程
//        {
//            cout <<" 启动写数据串口的线程 "<< endl;
//            w->start();
//        }
//    }
//}

// 生成校验值
char WriteDataToBottom::GenerateDataVerifyForChar(char *str, unsigned long len)
{
    qDebug()<< __PRETTY_FUNCTION__<<"len = "<< len;
    char result = 0;
    unsigned long i;
    if (len <= 0)
    {
        qDebug()<< "校验失败,长度有误!";
        return result;
    }

    for (i = 0; i < len; i++)
    {
        result = result ^ str[i];
    }
    qDebug(" %s, result = %x\n", __PRETTY_FUNCTION__, result);
    return result & 0x7f;
}

// 系统升级
UpdateData::UpdateData()
{
    qDebug()<< __PRETTY_FUNCTION__;
}

UpdateData::~UpdateData()
{
    qDebug()<< __PRETTY_FUNCTION__;
}

// 单片机是否正常运行
void UpdateData::RunNormal()
{
//    unsigned char req;
    ReplyRun();
    // emit 运行正常的通知，进行迁移画面
}

// 回复正常运行
void UpdateData::ReplyRun()
{
    qDebug()<< __PRETTY_FUNCTION__;qDebug()<<"Update线程ID为："<<currentThreadId();
    unsigned char endstr[2];
    bool first = true;
    endstr[0] = 0x05;
    endstr[1] = 0x00;
    Protocoldeal *pro = Protocoldeal::GetInstance();
    int ret;
    while (1)
    {
        if (first)
        {
            pro->CopyStringFromUi(endstr[0], endstr);
            first = false;
            qDebug("只进入一次");
        }
        while(!(ret = pro->GetContinueFlag())) // 未收到校验正确的结果时
            msleep(50);  // 返回值为0时，一直在此处阻塞,一直检测返回值，直到不为0时，继续往下执行
        pro->SetContinueFlag(0);  // 重新将标志位赋值为0

        if (-1 == ret)   // 回复正常运行
        {
          //  emit  显示屏回到正常显示画面的部件
            break;
        }
        else if ( 3 == ret) // 数据校验错误，需要重发
        {
            pro->CopyStringFromUi(endstr[0], endstr);
        }
    }
    qDebug("升级结束请求成功!");
    qDebug("emit Protocoldeal::GetInstance()->ShowWhichScreen(1)");
    emit Protocoldeal::GetInstance()->ShowWhichScreen(1);// 显示屏回到正常显示画面的部件
}

void UpdateData::run()
{
    qDebug()<< __PRETTY_FUNCTION__;qDebug()<<"Update线程ID为："<<currentThreadId();
    Protocoldeal *pro = Protocoldeal::GetInstance();
    if (NULL != pro)
    {
        if (pro->GetVersionFlag())
        {
            pro->SetVersionFlag(false);
            qDebug("即将升级!");
            Updating();
        }
        else
        {
            qDebug("不必升级!");
            RunNormal();
        }
    }
//    if (!pro->GetVersionFlag())
//            RunNormal();
}

// 将文件大小转换成四个字节的字符串
void UpdateData::ConvertFileSize(const char *filename, unsigned char *str, int Cpcount)
{
    qDebug()<< __PRETTY_FUNCTION__;
    QFile file(filename);
    int length = file.size();
    qDebug("filelength = %d", length);
    IntToUnsignedChar(length, str, Cpcount);
}

void UpdateData::IntToUnsignedChar(int length, unsigned char *str, int Cpcount)
{
    qDebug()<< __PRETTY_FUNCTION__;
    unsigned char temp[10];
    int ar[16];
    int i = 0;
    int j = 0;
    int index = 0;
    int addcount = 0;
    int num = 0;
    bool flag = true;
    SplitNumber(length, ar, i); // 将int类型的值拆分到数组中
    qDebug("i = %d", i);
    for (j = 0; j < i; j++)
    {
        if (i < 8 && flag)
        {
            temp[index++] = 0x00;
            qDebug("i = %d 小于8,最高位置为0x00", i);
            flag = false; // 保证i < 8的判断只进行一次操作
        }
        addcount++;
        num = num*10 + ar[j];
        if (0 == addcount % 2)
        {
            temp[index++] = num + 0x00; // 赋值操作
            num = 0;
        }
    }
    memcpy(str, temp, Cpcount);
    return;
}

// 将int类型的值拆分到数组中
void UpdateData::SplitNumber(int length, int array[], int &i)
{
    qDebug()<< __PRETTY_FUNCTION__;
    while(length)
    {
        array[i] = length % 10;
        length = length / 10;
        qDebug("array[%d] = %d", i, array[i]);
        i++;
    }
    int j = 0;
    int k = i - 1; // 指向最后一个值的下标
    int temp;
    for (j = 0; j < i/2; j++, k--) // 将数组倒置一下
    {
        temp = array[j];
        array[j] = array[k];
        array[k] = temp;
    }
    for(j = 0; j < i; j++)
    {
        qDebug("ar[%d] = %d", j, array[j]);
    }
}

void UpdateData::WholeFileVerify(const char*filename, unsigned char *str, int Cpcount)
{
    qDebug()<< __PRETTY_FUNCTION__;
    QFile file(filename);
    char ch;
    unsigned char prech = 0x00;
    unsigned char curch;
    unsigned char result[10];
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug("打开文件%s失败!", filename);
        return ;
    }
    while(!file.atEnd())
    {
        file.read(&ch, 1);
        curch = (unsigned char)ch;
        prech = curch ^ prech;
    }
    curch = prech & 0x7f;
    result[0] = 0x00;
    result[1] = curch;
    memcpy(str, result, Cpcount);
}

// 请求进入升级模式 或 请求开始升级成功
void UpdateData::RequestUpdate(unsigned char req)
{
    qDebug()<< __PRETTY_FUNCTION__;
    unsigned char endstr[8];
    bool first = true;
//    endstr[0] = 0x05;
    endstr[0] = ID05_UPDATEREQ;
    endstr[1] = req;
    if (0x01 == req)
    {
        ConvertFileSize(path, &endstr[2], 4);
        WholeFileVerify(path, &endstr[6], 2);
    }
    else
    {
        int i;
        for(i = 2; i < 8; i++)
        {
            endstr[i] = 0x00;
        }
    }

    Protocoldeal *pro = Protocoldeal::GetInstance();
    int ret;
    while (1)
    {
        if (first)
        {
            pro->CopyStringFromUi(endstr[0], endstr);
            first = false;
            qDebug("只进入一次");
        }
        while(!(ret = pro->GetContinueFlag())) // 未收到校验正确的结果时
            msleep(50);  // 返回值为0时，一直在此处阻塞,一直检测返回值，直到不为0时，继续往下执行
        pro->SetContinueFlag(0);  // 重新将标志位赋值为0

        if (1 == ret && 0x01 == req)   // 请求成功
        {
          //  emit  显示屏回到正常显示画面的部件
            qDebug("请求进入升级状态,请求成功");
            break;
        }
        else if (1 == ret && 0x03 == req) // 开始升级请求成功
        {
            qDebug("开始升级请求成功");
            break;
        }
        else if ( 3 == ret) // 数据校验错误，需要重发
        {
            pro->CopyStringFromUi(endstr[0], endstr);
        }
    }
    qDebug("请求进入升级模式,请求成功!");
//    emit 退出升级模式的画面;
}

// 当读取的字符长度小于64时，对buf用0xff填充至64个字节
void UpdateData::AppendByte(char *buf, int len)
{
    qDebug()<< __PRETTY_FUNCTION__;
    int i;
    for (i = len; i < 64; i++)
    {
        buf[i] = 0xff;
        qDebug("填充0xff至字符串中");
    }
}

// 每次从文件中读64个字节的写入串口中
void UpdateData::ReadUpdateFile(const char *filename)
{
    qDebug()<< __PRETTY_FUNCTION__;
    static bool firstRead = true;
    QFile file(filename);
    int filelength = file.size();  // 文件大小
    static int readcount = 0;
    int percent = 0;
    char buffer[64];
    int readlen = 0;
    memset(buffer, 0, sizeof(buffer));
    int ret;
    qDebug("文件大小 = %d", filelength);
    Protocoldeal *pro = Protocoldeal::GetInstance();
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug("打开文件%s失败!", filename);
        return ;
    }
    while(1)
    {
        if (firstRead)
        {
            readlen = file.read(buffer, 64);
            if (readlen < 64)
            {
                AppendByte(buffer, readlen);// 字符长度小于64时，对buf用0xff填充至64个字节
            }
            pro->CopyStringFromUi(0x07, buffer);
            firstRead = false;
            readcount++;
            qDebug("只在第一次进入");
        }
        while( !(ret = pro->GetContinueFlag())) // 未收到校验正确的结果时
            msleep(50);  // 返回值为0时，一直在此处阻塞,一直检测返回值，直到不为0时，继续往下执行
        pro->SetContinueFlag(0);  // 重新将标志位赋值为0

        if ( 3 == ret) // 数据校验失败,重发
        {
            qDebug("校验错误重发!");
            pro->CopyStringFromUi(0x07, buffer); // 重新发送buffer的内容
        }
        else if ( 2 == ret && (!file.atEnd())) // 未到达文件末尾时
        {
            readlen = file.read(buffer, 64);
            if (readlen < 64)
            {
                AppendByte(buffer, readlen);// 字符长度小于64时，对buf用0xff填充至64个字节
            }
            pro->CopyStringFromUi(0x07, buffer);
            percent = readcount*6400 / filelength;
            qDebug("percent大小 = %d, readcount = %d", percent, readcount);
            readcount++;
        }
        else if (2 == ret && (file.atEnd())) // 校验结果为真，并且已经达到文件末尾时跳出循环
        {
            percent = 100;
            readcount = 0;
            qDebug("emit pro->SendPercent(percent)");
            emit pro->SendPercent(percent); // emit 百分比percent
            break;
        }
        qDebug("emit pro->SendPercent(percent)");
        emit pro->SendPercent(percent); // emit 百分比percent
    } 
    file.close();
}

// 升级结束 req
void UpdateData::UpdateEnd(unsigned char req)
{
    qDebug()<< __PRETTY_FUNCTION__;
    unsigned char endstr[2];
    bool first = true;
    endstr[0] = 0x05;
    endstr[1] = req;
    Protocoldeal *pro = Protocoldeal::GetInstance();
    int ret;
    while (1)
    {
        if (first)
        {
            pro->CopyStringFromUi(endstr[0], endstr);
            first = false;
            qDebug("只进入一次");
        }
        while(!(ret = pro->GetContinueFlag())) // 未收到校验正确的结果时
            msleep(50);  // 返回值为0时，一直在此处阻塞,一直检测返回值，直到不为0时，继续往下执行
        pro->SetContinueFlag(0);  // 重新将标志位赋值为0

        if (1 == ret)   // 请求成功,显示屏退出升级模式
        {
            break;
        }
        else if ( 3 == ret) // 数据校验错误，需要重发
        {
            pro->CopyStringFromUi(endstr[0], endstr);
        }
    }
    qDebug("升级结束请求成功!");
}

// 正在升级中
void UpdateData::Updating()
{
    qDebug()<< __PRETTY_FUNCTION__;
    Protocoldeal *pro = Protocoldeal::GetInstance();
    RequestUpdate(0x03);  // 显示屏发送请求进入升级 TAG=0x05  Byte1 = 0x03
    RequestUpdate(0x01);  // 显示屏发送开始升级请求 TAG=0x05  Byte1 = 0x01
//    readcount = 0;
    if (NULL != pro && pro->GetUsbInsertFlag())
    {
        qDebug("U盘插入升级，切换界面");
        emit pro->HideWhichScreen(1);
        emit pro->ShowWhichScreen(0);
        pro->SetUsbInsertFlag(false);
        ReadUpdateFile(usbpath); // 显示屏发送升级数据
    }
    else
    {
        qDebug("emit pro->ShowWhichScreen(0)");
        emit pro->ShowWhichScreen(0);
        ReadUpdateFile(path); // 显示屏发送升级数据
    }
    UpdateEnd(0x02);  // 显示屏发送升级结束 TAG = 0x05  Byte1 = 0x02
    //    emit 退出升级模式的画面;
    qDebug("emit pro->HideWhichScreen(0)");
    qDebug("emit pro->ShowWhichScreen(1)");
    emit pro->HideWhichScreen(0);
    emit pro->ShowWhichScreen(1);
    qDebug("退出升级模式的画面,进入正常显示电梯部件的界面");
}

