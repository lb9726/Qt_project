#ifndef PROTOCOLDEAL_H
#define PROTOCOLDEAL_H
//#define SERIAL_DEVICE ("/dev/ttymxc1")
#define SERIAL_DEVICE ("/dev/ttyUSB0")
#include <QObject>
#include <QThread>
#include <QDebug>
#include <iostream>
#include <fstream>
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QStringList>
#pragma pack(push, 1) //按照1字节对齐

//*ID = 0的帧内容*/
/* 楼层值个位的数据结构 */
typedef struct _id0_dat1_{
    unsigned char Unit: 7;	/* 个位显示字符ASCII码表示 */
    unsigned char Reserved: 1;
}s_ID0DAT1_T;
/* 楼层值十位的数据结构 */
typedef struct _id0_dat2_{
    unsigned char Tend: 7;	/* 十位显示字符ASCII码表示 */
    unsigned char Reserved: 1;
}s_ID0DAT2_T;
/* 楼层值百位的数据结构 */
typedef struct _id0_dat3_{
    unsigned char Hund: 7;	/* 百位显示字符ASCII码表示 */
    unsigned char Reserved: 1;
}s_ID0DAT3_T;
/* 楼层值千位的数据结构 */
typedef struct _id0_dat4_ {
    unsigned char Thou: 7;   /* 千位显示字符ASCII码表示 */
    unsigned char Reserved: 1;
}s_ID0DAT4_T;

/*箭头状态（e_ARROWSTA_T类型枚举）*/
enum {
    ArrowHide = 0,  /*0-不显示*/
    ArrowUp,        /*1-上箭头*/
    ArrowDn,        /*2-下箭头*/
    ArrowUpRoll,    /*3-上箭头滚动*/
    ArrowDnRoll,    /*4-下箭头滚动*/
    ArrowUpFlash,   /*5-上箭头闪烁*/
    ArrowDnFlash,   /*6-下箭头闪烁*/
};

/* 1.电梯信息帧 (ID = 0x00) */
typedef struct _bvt_id0_{
    unsigned char ID;                 /* ID */
    s_ID0DAT1_T Data1;                /* 个位显示字符ASCII码表示 */
    s_ID0DAT2_T Data2;	              /* 十位显示字符ASCII码表示 */
    s_ID0DAT3_T Data3;	              /* 百位显示字符ASCII码表示 */
    s_ID0DAT4_T Data4;	              /* 千位显示字符ASCII码表示 */
    unsigned char ArrowStatus;	      /* 箭头状态 */
    unsigned char LiftSpecialStatus;  /* 电梯特殊状态 */
    unsigned char StationClockStatus; /* 到站钟状态 */
    unsigned char StationLightStatus; /* 到站灯状态 */
}s_BVTID0_T;

/*2.控制命令帧（ID = 0x01）*/
typedef struct _bvt_id1_{
    unsigned char byte1;
    unsigned char byte2;
    unsigned char byte3;
    unsigned char byte4;
    unsigned char byte5;
    unsigned char byte6;
    unsigned char byte7;
}s_BVTID1_T;

///*心跳ID帧数据*/
//typedef struct _bvt_id1_{
//    unsigned char Data1;
//    unsigned char Data2;
//}s_BVTID1_T;

/*各个数据帧组成的信息表*/
typedef struct _messagetable {
    s_BVTID0_T ID0_Message;     //电梯信息帧 (ID = 0x00)
}messagetable;

//ID类型枚举
typedef enum _id_type_{
    ID00_BASE = 0x00,	/* 基本信息ID 包含楼层，箭头、功能信息*/
    ID01_ORDER,
//    ID01_HEAR,          /* 心跳ID */

    ID_UNKNOW,          /* 未知ID信息 */
}e_IDTYPE_T;

#pragma pack(pop)

// 读取串口数据的线程
class ProducerFromBottom : public QThread
{
    Q_OBJECT
public:
    ProducerFromBottom();
    ~ProducerFromBottom();
    void run();
    void SetSerialArgument();
    void CopySerialDataToBuf(QByteArray arr);
    void StartThread(ProducerFromBottom *p);
//public:
//    QSerialPort *my_serialport;
public slots:
    void ReadyreadSlots();
    void CloseSerial();
private:
//    QSerialPort *my_serialport;
    unsigned char *ProducerFromBottom_pointer;
    int ProCounts;
};

// 向串口发送数据的线程
class WriteDataToBottom : public QThread
{
    Q_OBJECT
public:
    WriteDataToBottom();
    ~WriteDataToBottom();
    void run();
    void SetSerialArgument();
    unsigned char *GetPointPosition();
    int GetConCounts();
    void SetConCounts(int counts);
    void StartThread(WriteDataToBottom *w);
public slots:
    void CloseSerial();
    void WriteDataSerial();
private:
    QSerialPort *Write_serialport;
    unsigned char *ConsumerFromBottom_pointer;
    int ConCounts;
};

// 协议处理的类
class Protocoldeal: public QObject
{
    Q_OBJECT

public:
    ~Protocoldeal();
    static Protocoldeal *GetInstance();
    void BstBvtPtlInit(void);
    void BstBvtSetFrameData(e_IDTYPE_T id,void *dat);
    void BstBvtCopyFrameData(e_IDTYPE_T id,void *dat);
    e_IDTYPE_T BstBvtPtlMonitor(void);
    bool JudgeChange(unsigned char ID, unsigned char str[]);
    unsigned char BstBvtGetFrameDatLen(unsigned char id);

    unsigned long BstBvtRecoverFrame(void *des, void *src, unsigned long srclen);      // 数据还原
    void CopyStringFromProtocol(unsigned char Id, void *str);
    void PrintString(unsigned char *src, unsigned long length);
    bool StringCompare(unsigned char *temp, unsigned char *str, unsigned long len);
    bool AllocteMemory(void *p);
    unsigned char BstBvtVerify(unsigned char *data, unsigned long length); // CRC 数据校验
    unsigned long GetDataLength();
//    ProducerFromBottom * GetReadThreadPointer();
//    WriteDataToBottom * GetWriteThreadPointer();
protected:
    Protocoldeal();
    unsigned char BstBvtRecvMonitor(void);
    void BstBvtSendMonitor(void);
    unsigned long BstBvtTransformFrame(void *src,unsigned long srclen,void *dst);
    void BstFifoMemCpy(unsigned char *pFrameBuf,void* dat, unsigned char DatLen);

signals:
    void AcceptDataFormBottom(unsigned char s);
    void AcceptDataFormTop();

private:
    ProducerFromBottom *ReadDataPthread;
    WriteDataToBottom *WriteDataPthread;
    static Protocoldeal *instance;
};



#endif // PROTOCOLDEAL_H
