#ifndef PHYSICALDISK_H
#define PHYSICALDISK_H

#include "physicalmovable.h"
#include "physicalprotocol.h"

bool ReMountFat(QString pPath, int pType);

class PhysicalStorage : public PhysicalHost
{
    Q_OBJECT
public:
    PhysicalStorage(QString pAddr, QString pBufferPath=QString(), QObject *parent=0);
    ~PhysicalStorage();

    bool Scan();        //>@扫描缓冲路径下的工程

    bool isSync();
    bool Sync();
    void Async();

    void OpenLocation(QString pLocation);

    bool DeleteTheme(QString pThemeName);
    bool SetRunningTheme(QString pThemeName);
    QString GetRunningTheme();

    bool Download(QString pThemeName, QString pThemePath=QString());
    bool Upload(QString pThemeName);

    bool DownloadPreference(QString pMdpName);
    bool UploadPreference(QString pMdpName);
};


class PhysicalDisk : public PhysicalDevice
{
    Q_OBJECT
public:
    PhysicalDisk(QObject *parent = 0);
    ~PhysicalDisk();

    quint32 InitPara();
    QString GetInformation() { return QString("%1>%2").arg(mDevName).arg(mDevPath); }

    quint32 Monitor(quint32 pCount);

public slots:
    void UpdatePara(QObject* pObject); 

public:
    QString         mModel;
    NUM_INFO        mThreshold;

    double          mUsageRate;
    double          mDiskUsed;
    double          mDiskFree;
    double          mDiskTotal;
};

class PhysicalLCD;

QString GetMountAddr(QString pDriver, QString pStorageBufPath);
QString MountStorage(QString pDriver, QString pStorageBufPath);
bool UnMountStorage(QString pMountAddr);
class PhysicalMobileDisk : public PhysicalMovable
{
    Q_OBJECT
public:
    PhysicalMobileDisk(QObject *parent = 0);
    ~PhysicalMobileDisk();

    bool SetPlugged();
    bool SetUnplugged();
    void ScanDisks();

    bool CopyFolder(QString pSrcDir, QString pDstDir);

    bool GetDiskList(QString pDirPath, CONTENT_TYPE pContentType);
    bool GetComList(QString pDirPath, CONTENT_TYPE pContentType, QStringList pSuffix);

    bool UpgradeComList(QString pDstPath, bool pBackup);
    bool UpgradeDiskList(QString pDstPath, bool pBackup);

    CONTENT_TYPE CanUpgrade();
    bool UpgradeTo(QString pDstPasth);

    bool InitSubPHPara(QString pName, QDomElement pElement, QHBoxLayout*& pHBoxLayout);
    void UpdateSubPHPara(QString pName, QObject *pObject, QDomElement pElement);

public slots:
    void slot_Initialize();
    void slot_DeviceAdded(QString dev);
    void slot_DeviceRemoved(QString dev);

public:
    QStringList              mMobileDisks;
    bool                     mExecutable;  //>@是否作为执行盘，否则作为升级盘
    QString                  mMountBufPath;

    CONTENT_TYPE             mContentType;

    QPointer<PhysicalLCD>    mLCD;
};

#endif // NODEMEM_H
