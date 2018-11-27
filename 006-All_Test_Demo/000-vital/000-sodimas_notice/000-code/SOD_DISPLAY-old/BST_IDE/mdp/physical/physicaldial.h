#ifndef PHYSICALDIAL_H
#define PHYSICALDIAL_H

#include "physicalbase.h"
#include "definedial.h"
#include "physicalelevator.h"

typedef enum{
    DIAL_NONE = 0,
    DIAL_ROTATION,
    DIAL_ROTATIONEN,
    DIAL_STANDBY,
    DIAL_AUDIO,
    DIAL_AUTOTEST,
    DIAL_ROTSEL1,
    DIAL_ROTSEL0,
    DIAL_AUDVOL1,
    DIAL_AUDVOL0
} DIAL_TYPE;

Q_DECLARE_METATYPE(DIAL_TYPE)
inline uint qHash(DIAL_TYPE pType) { return (uint)pType; }

inline DIAL_TYPE GetDialType(QString pName)
{
    DIAL_TYPE tmpType = DIAL_NONE;
    if(pName.isEmpty())
        return tmpType;
    if(!pName.compare("Rotation", Qt::CaseInsensitive))
        tmpType = DIAL_ROTATION;
    else if(!pName.compare("RotationEn", Qt::CaseInsensitive))
        tmpType = DIAL_ROTATIONEN;
    else if(!pName.compare("Standby", Qt::CaseInsensitive))
        tmpType = DIAL_STANDBY;
    else if(!pName.compare("Audio", Qt::CaseInsensitive))
        tmpType = DIAL_AUDIO;
    else if(!pName.compare("AutoTest", Qt::CaseInsensitive))
        tmpType = DIAL_AUTOTEST;

    else if(!pName.compare("RotSel1", Qt::CaseInsensitive))
        tmpType = DIAL_ROTSEL1;
    else if(!pName.compare("RotSel0", Qt::CaseInsensitive))
        tmpType = DIAL_ROTSEL0;
    else if(!pName.compare("AudVol1", Qt::CaseInsensitive))
        tmpType = DIAL_AUDVOL1;
    else if(!pName.compare("AudVol0", Qt::CaseInsensitive))
        tmpType = DIAL_AUDVOL0;
    return tmpType;
}

class DIAL_INFO
{
public:
    DIAL_INFO()
    {
        mType = DIAL_NONE;
        mValue = -1;
    }
    void setName(QString pName)
    {
        mName = pName;
        mType = GetDialType(pName);
    }
public:
    DIAL_TYPE  mType;    //>@表示此拨码的用途
    QString    mName;    //>@表示此拨码的名字
    char       mValue;   //>@表示此拨码的值
};

/*
    薄码开关只有在启动的第一时刻检测一下，其他时候都不进行检测
    启动时刻检测的目的在于同步薄码开关于当前软件配置。

    生产时，需要将所有拨码开关拨为OFF状态，同时设置MDP文件中的旧VAL为true，以此实现第一次启动dial有效。
*/
class PhysicalDial : public PhysicalDevice
{
    Q_OBJECT
public:
    PhysicalDial(QObject *parent = 0);
    ~PhysicalDial();

    quint32 InitPara();
    void ProcessDial();

    /*
        1:仅读出dail值并显示在messagebar上
        2：读出并执行
    */
    quint32 ExecDial(int pMode = 2);

    quint32 Monitor(quint32 pCount);

public slots:
    void UpdatePara(QObject* pObject);
    void slot_Initialize();

public:
#ifdef N329
    QFile                  *m_File;
#endif
    QHash<int, DIAL_INFO*>  m_DialNums;   //>@以拨码序号为索引
    QPointer<PhysicalElevator>      mElevator;
};

#endif // PHYSICALDIAL_H

