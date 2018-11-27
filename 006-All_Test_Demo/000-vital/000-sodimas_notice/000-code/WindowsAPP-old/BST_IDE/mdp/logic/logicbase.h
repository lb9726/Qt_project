#ifndef LOGICBASE_H
#define LOGICBASE_H

#include "devfun.h"
#include "physicalbase.h"

class LogicDevice : public IODeviceBase
{
    Q_OBJECT
public:
    LogicDevice(QObject *parent = 0);
    ~LogicDevice();

    bool InitDevice(LG_Type pLGType, QDomElement pElement);
    QString GetSystemPath()  {return D_FSRCPATH+D_SYSTEMDIR+D_LOGICDIR+mDevName+QString("/");}
    bool AppendPHDevice(PhysicalDevice *pPHDev);
    virtual QStringList AppendPHDevices(QStringList pPHDevs);
    QStringList PHDevicesAdded(QStringList pList, CheckList *pComponent);
    void PHDeviceDeleted(PhysicalDevice *pPHDev, bool pSync=true);
    virtual void DeletePHDevices();
    void SetPHDeviceEditble(bool flag);

    QHBoxLayout* InitDevicePara(QDomElement pElement);
    void UpdateDevicePara(QObject *pSender, QDomElement pElement);

    void ResetPHDevs();

public slots:
    virtual void ParseDataStream(){ IDE_TRACE(); }
    virtual void DeviceStateChanged();

signals:
    void sPHDeviceChanged();

public:
    QHash<PH_Type, PhysicalDevice*> mPHDevices;       //>@协议类型节点表
    LG_Type                         mDevType;
};

#endif // BASEPARSER_H
