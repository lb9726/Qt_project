#ifndef PROTOCOLDEAL_H
#define PROTOCOLDEAL_H
#define SERIAL_DEVICE ("/dev/ttymxc1")
//#define SERIAL_DEVICE ("/dev/ttyUSB0")
const int OffsetHead = 2059; // 从文件的第2059的字节处拷贝5个字节，获取版本信息
#include <QObject>
#include <QThread>
#include <QDebug>
#include <iostream>
#include <fstream>
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QStringList>
#include <QTimer>
#include <QFile>
//#include "widget.h"
//#include "fileupdate.h"
#include "qdevicewatcher.h"
#include "qdevicewatcher_p.h"
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

/* 1.电梯基本显示数据 (TAG = 0x00)*/
typedef struct _bvt_id0_{
    unsigned char ID;                 /* ID标签 */
    s_ID0DAT1_T Data1;                /* 个位显示字符ASCII码表示 */
    s_ID0DAT2_T Data2;	              /* 十位显示字符ASCII码表示 */
    s_ID0DAT3_T Data3;	              /* 百位显示字符ASCII码表示 */
    s_ID0DAT4_T Data4;	              /* 千位显示字符ASCII码表示 */
    unsigned char ArrowStatus;	      /* 箭头状态 */
    unsigned char LiftSpecialStatus;  /* 电梯特殊状态 */
    unsigned char StationClockStatus; /* 到站钟状态 */
    unsigned char StationLightStatus; /* 到站灯状态 */
}s_BVTID0_T;

/*2.电梯扩展显示数据（TAG = 0x01）*/
typedef struct _bvt_id1_{
    unsigned char ID;                    /* ID标签 */
    unsigned char ShowLanguage;          /*显示语言*/
    unsigned char CtrCanFaultInfo;       /*控制柜故障信息*/
    unsigned char DoorMachineFaultInfo;  /*门机故障信息*/
    unsigned char LiftSpeedInt;          /*电梯速度整数部分*/
    unsigned char LiftSpeedFloat;        /*电梯速度小数部分*/
    unsigned char LiftHeightLowByte;     /*电梯高度低字节*/
    unsigned char LiftHeightHighByte;    /*电梯高度高字节*/
    unsigned char LiftRunMileage[4];     /*电梯运行里程数 - 字节低->高*/
    unsigned char LifeRunCounts[4];      /*电梯运行次数 - 字节高->低*/
}s_BVTID1_T;

/*3.按钮输入数据 (TAG = 0x02)*/
typedef struct _bvt_id2_ {
    unsigned char ID;                   /* ID标签 */
    unsigned char ButtonType;           /* 按钮类型 */
    unsigned char ButtonStatusLow;      /* 按钮状态低位 */
    unsigned char ButtonStatusHigh;     /* 按钮状态高位 */
}s_BVTID2_T;

/*4.按钮输出数据 (TAG = 0x03)*/
typedef struct _bvt_id3_ {
    unsigned char ID;                   /* ID标签 */
    unsigned char ButtonType;           /* 按钮类型 */
    unsigned char ButtonStatusLow;      /* 按钮状态低位 */
    unsigned char ButtonStatusHigh;     /* 按钮状态高位 */
}s_BVTID3_T;

/*5.MCU版本信息 (TAG = 0x04)*/
typedef struct _bvt_id4_ {
    unsigned char ID;                  /* ID标签 */
    unsigned char VersionInfoInt;      /* 版本整数部分（0~9） */
    unsigned char VersionInfoFloat;    /* 版本小数部分（00~99） */
    unsigned char year;                /* 年（0~99） */
    unsigned char month;               /* 月 */
    unsigned char day;                 /* 日 */
}s_BVTID4_T;

/*6.升级请求 (TAG = 0x05)*/
typedef struct _bvt_id5_ {
    unsigned char ID;                  /* ID标签 */
    unsigned char RequestStatus;       /* 请求状态 */
    /* 0--无需升级 1--开始升级 2--升级结束 3--请求进入升级状态 */
}s_BVTID5_T;

/*7.升级请求应答 (TAG = 0x06)*/
typedef struct _bvt_id6_ {
    unsigned char ID;                  /* ID标签 */
    unsigned char ResponseResult;      /* 应答结果 */
    /* 0--正常运行 1--请求成功 2--升级数据无误，可传输下一包升级数据 3--升级数据有误，需重发 */
}s_BVTID6_T;

/*8.升级数据 (TAG = 0x07)*/
typedef struct _bvt_id7_ {
    unsigned char ID;                  /* ID标签 */
    unsigned char UpdateData[64];      /* 数据(共计64字节) */
}s_BVTID7_T;

///*心跳ID帧数据*/
//typedef struct _bvt_id1_{
//    unsigned char Data1;
//    unsigned char Data2;
//}s_BVTID1_T;

/*各个数据帧组成的信息表*/
typedef struct _messagetable {
    s_BVTID0_T ID0_Message;     /* 1.电梯基本显示数据 (TAG = 0x00) */
    s_BVTID1_T ID1_Message;     /* 2.电梯扩展显示数据 (TAG = 0x01) */
    s_BVTID2_T ID2_Message;     /* 3.按钮输入数据 (TAG = 0x02) */
    s_BVTID3_T ID3_Message;     /* 4.按钮输出数据 (TAG = 0x03) */
    s_BVTID4_T ID4_Message;     /* 5.MCU版本信息 (TAG = 0x04) */
    s_BVTID5_T ID5_Message;     /* 6.升级请求 (TAG = 0x05) */
    s_BVTID6_T ID6_Message;     /* 7.升级请求应答 (TAG = 0x06) */
    s_BVTID7_T ID7_Message;     /* 8.升级数据 (TAG = 0x07) */
}messagetable;

//ID类型枚举
typedef enum _id_type_{
    ID00_BASE = 0x00,	/* 基本信息ID 包含楼层，箭头、功能信息*/
    ID01_ORDER,
//    ID01_HEAR,          /* 心跳ID */

    ID_UNKNOW,          /* 未知ID信息 */
}e_IDTYPE_T;

typedef struct _updateversion {
    unsigned char ver[2];   // 升级文件的版本信息
}UpdateVersion;

#pragma pack(pop)

class Widget;
class FileUpdate;
// 读取串口数据的线程
class ProducerFromBottom : public QObject /*QThread*/
{
    Q_OBJECT
public:
    ProducerFromBottom();
    ~ProducerFromBottom();
//    void run();
//    void StartThread(ProducerFromBottom *p);
    void SetSerialArgument();

public slots:
    void ReadyreadSlots();
};

// 向串口发送数据的线程
class WriteDataToBottom : public QObject /*QThread*/
{
    Q_OBJECT
public:
    WriteDataToBottom();
    ~WriteDataToBottom();
//    void run();
//    void StartThread(WriteDataToBottom *w);
    char GenerateDataVerifyForChar(char *str, unsigned long len);

public slots:
    void WriteDataSerial();
    void ConstructWriteData(char *wstr, char *src);
signals:
    void FillDataSignal(char *wstr, char *src);
    void WriteDataSignal();
};

class UpdateData: public QThread
{
    Q_OBJECT
public:
    UpdateData();
    ~UpdateData();
    void RequestUpdate(unsigned char req);
    void ReadUpdateFile(const char *filename);    // 读取升级文件并发送内容
    void AppendByte(char *buf, int len);
    void run();
    void RunNormal();
    void ReplyRun();
    void UpdateEnd(unsigned char req);

public slots:
    void Updating();
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
    unsigned long BstBvtGetFrameDatLen(unsigned char id);

    unsigned long BstBvtRecoverFrame(void *des, void *src, unsigned long srclen);      // 数据还原
    void CopyStringFromProtocol(unsigned char Id, void *str);
    void CopyStringFromUi(unsigned char Id, void *str);
    void PrintString(unsigned char *src, unsigned long length);
    bool StringCompare(unsigned char *temp, unsigned char *str, unsigned long len);
    bool AllocteMemory(void *p);
    unsigned char BstBvtVerify(unsigned char *data, unsigned long length); // CRC 数据校验
    unsigned long GetDataLength();
    void SetSerialArgument();
    void CloseSerial();
    void SetContinueFlag(unsigned char buf[]);
    void SetContinueFlag(int num);
    int GetContinueFlag();
    void CountString(unsigned char *des, char *src, int len);
    void RevVersion(unsigned char buffer[], unsigned char version[]);
    bool GetVersionFlag();
    void SetVersionFlag(bool flag);
    void NotifyCompare(unsigned char *buf);

public slots:
    void CompareVersion(unsigned char *Revversion, unsigned char *Readversion);   // 比较版本信息
    void GetUpdateVersion(const char *filename, UpdateVersion *Uver);  // 从升级文件中获取版本号用于下一步的比较
    void OnUpdateSlots();
    void AddUsbSlots();
    void WriteToSerialSlots(char *buf, unsigned long leng);

protected:
    Protocoldeal();
    unsigned char BstBvtRecvMonitor(void);
    void BstBvtSendMonitor(void);
    unsigned long BstBvtTransformFrame(void *src,unsigned long srclen,void *dst);
    void BstFifoMemCpy(unsigned char *pFrameBuf,void* dat, unsigned char DatLen);

signals:
    void AcceptDataFormBottom(unsigned char s);
    void AcceptDataFormTop();
    void SendPercent(int val);
    void UpdateFlagSignal();
    void StartCompareSignal(unsigned char *Revversion, unsigned char *Readversion);
    void ShowWhichScreen(int index);
    void HideWhichScreen(int index);
    void WriteToSerialSignal(char *buf, unsigned long leng);
private:
    ProducerFromBottom *ReadDataPthread;
    WriteDataToBottom *WriteDataPthread;
    QThread *RThread;
    QThread *WThread;
    UpdateData *upd;
    static Protocoldeal *instance;
    int ContinueFlag;
    int RunNormalFlag;
    bool VersionComFlag;
    Widget *wid;
    FileUpdate *fileup;
    QDeviceWatcher *UsbDetect;
};



#endif // PROTOCOLDEAL_H
