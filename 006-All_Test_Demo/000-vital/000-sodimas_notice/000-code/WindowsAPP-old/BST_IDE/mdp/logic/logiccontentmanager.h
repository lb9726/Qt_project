#ifndef LogicContentManager_H
#define LogicContentManager_H

#include "logicbase.h"
#include <QMultiHash>
#ifndef PHONE
#include "modem.h"
#endif
#include "physicaldisk.h"

class GraphicsScene;

/*
  用于管理各类升级设备，并对设备进行升级
    目录：
       BST.cmd
       DISK
         BOOT
         THEME
         SYSTEM
         EXE
       COM
         Dekstop
         Logo
         Video
         Image
         Text
*/

class LogicContentManager : public LogicDevice
{
    Q_OBJECT
public:
    LogicContentManager(QObject *parent = 0);
    ~LogicContentManager();

public:
    quint32 InitPara(); //>@返回处理的参数个数

signals:
    void sSetMenu(bool pUpgradeMode);

public slots:
    void UpdatePara(QObject* pObject);
    void DeviceStateChanged();
    void slot_Initialize();

public:
    QPointer<PhysicalUpgrade> mUpgradeDev;
    QPointer<PhysicalLCD>     mLCD;
    QPointer<PhysicalDisk>    mDisk;

    bool                      mBackStage;
    QString                   mSystem;
    QString                   mUserLogo;
    QString                   mApplication;

#ifdef SUPPORT_MUTEX
    QMutex                    *mUpgradeMutex;
#else
    bool                       mUpgradeMutex;
#endif
};

#endif // PARSERUPGRADE_H
