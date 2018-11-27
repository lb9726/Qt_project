#ifndef PROTOCOLDEAL_H
#define PROTOCOLDEAL_H
#define FILE_DEVICE ("/dev/ttymxc1")
//#define FILE_DEVICE ("test")
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
/* 数据2的数据结构 */
typedef struct _id0_dat2_{
    unsigned char Hund: 7;	/* 百位显示字符ASCII码表示 */
    unsigned char Reserved: 1;
}s_ID0DAT2_T;
/* 数据3的数据结构 */
typedef struct _id0_dat3_{
    unsigned char Tend: 7;	/* 十位显示字符ASCII码表示 */
    unsigned char Reserved: 1;
}s_ID0DAT3_T;
/* 数据4的数据结构 */
typedef struct _id0_dat4_{
    unsigned char Unit: 7;	/* 个位显示字符ASCII码表示 */
    unsigned char Reserved: 1;
}s_ID0DAT4_T;
/* 数据5的数据结构 */
typedef struct _id0_dat5_{
    unsigned char ArrowUp: 1;	/*上箭头*/
    unsigned char ArrowDn: 1;	/*下箭头*/
    unsigned char Runing: 1;	/*运行*/
    unsigned char Arrived: 1;	/*到站使能*/
    unsigned char VoidEn: 1;	/*语音报站使能*/
    unsigned char PowerDown: 1;	/*节能使能*/
    unsigned char Reserved: 1;	/*预留*/
    unsigned char Reserved1: 1;	/*预留*/
}s_ID0DAT5_T;

typedef struct _bvt_id0_{
    s_ID0DAT2_T Data2;	/* 数据2 */
    s_ID0DAT3_T Data3;	/* 数据3 */
    s_ID0DAT4_T Data4;	/* 数据4 */
    s_ID0DAT5_T Data5;	/* 数据5 */
    unsigned char Func[2];/*数据6/7/8*/
    unsigned char Language1;
    unsigned char Language2;
    unsigned char Year;
    unsigned char Month;
    unsigned char Day;
    unsigned char Week;
    unsigned char Hour;
    unsigned char Min;
    unsigned char Second;
}s_BVTID0_T;

/*心跳ID帧数据*/
typedef struct _bvt_id1_{
    unsigned char Data1;
    unsigned char Data2;
}s_BVTID1_T;


//ID类型枚举
typedef enum _id_type_{
    ID00_BASE = 0,	/* 基本信息ID 包含楼层，箭头、功能信息*/
    ID01_HEAR,		/* 心跳ID */

    ID_UNKNOW,		/* 未知ID信息 */
}e_IDTYPE_T;

#pragma pack(pop)

class Protocoldeal : public QThread
{
    Q_OBJECT

public:
    Protocoldeal();
    ~Protocoldeal();
    void run();
    void BstBvtPtlInit(void);
    void BstBvtSetFrameData(e_IDTYPE_T id,void *dat);
    void BstBvtCopyFrameData(e_IDTYPE_T id,void *dat);
    e_IDTYPE_T BstBvtPtlMonitor(void);
    int RetFileLength(char filename[]);
    void RedFile();
    bool JudgeChange(char str[], char str2[]);
    QString ChartoQString(char str[]);
    QSerialPortInfo FindSerial();
    void SetSerialArgument();
    bool JudgeCompleteData(QString s);
    void QStringToChar(QString s);

protected:
    unsigned char BstBvtRecvMonitor(void);
    void BstBvtSendMonitor(void);
    unsigned long BstBvtTransformFrame(void *src,unsigned long srclen,void *dst);
    unsigned char BstBvtVerify(unsigned char *data, unsigned long length); // CRC 数据校验
    unsigned long BstBvtRecoverFrame(void *src,unsigned long srclen);      // 数据还原
    void BstFifoMemCpy(unsigned char *pFrameBuf,void* dat, unsigned char DatLen);
    unsigned char BstBvtGetFrameDatLen(e_IDTYPE_T id);

signals:
    void AcceptDataFormBottom(QString s);
    void AcceptDataFormTop();
public slots:
    void ReadyreadSlots();
private:
    QSerialPort *my_serialport;
};

#endif // PROTOCOLDEAL_H
