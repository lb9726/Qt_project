#include "logiccontentmanager.h"
#include "physicaldisk.h"
#ifdef IDE
    #include "tabpage.h"
    #include "mdppage.h"
#endif
#include "deviceinc.h"

LogicContentManager::LogicContentManager(QObject *parent)
    :LogicDevice(parent)
{
    mUpgradeDev = 0;
#ifdef SUPPORT_MUTEX
    mUpgradeMutex = new QMutex(QMutex::Recursive);
#else
    mUpgradeMutex = false;
#endif
}

LogicContentManager::~LogicContentManager()
{
#ifdef SUPPORT_MUTEX
    if(mUpgradeMutex)
        delete mUpgradeMutex;
#else
    mUpgradeMutex = false;
#endif
}

quint32 LogicContentManager::InitPara()
{
    QDomNodeList tmpParaNodeList = m_ParaElement.childNodes();
    if(tmpParaNodeList.count() < 1)
        return 0;
    quint32 validcount = 0;
    QHBoxLayout* tmpHBoxLayout;
    for(int i=0;i<tmpParaNodeList.count();i++)
    {
        QDomElement tmpElement = tmpParaNodeList.at(i).toElement();
        if(tmpElement.isNull())
            continue;
        QString tmpName = tmpElement.tagName();
        if(!tmpName.compare("Backstage"))
        {
            mBackStage = (tmpElement.text().compare("false")==0)?(false):(true);
            tmpHBoxLayout = CreateBool(tmpName, tmpElement.text());
        }
        else if(!tmpName.compare("UserLogo"))
        {
            mUserLogo = tmpElement.text();
            tmpHBoxLayout = CreateTextEdit(tmpName, mUserLogo);
        }
        else if(!tmpName.compare("Device"))
        {
            tmpHBoxLayout = InitDevicePara(tmpElement);
        }
        else
            continue;
        if(!tmpHBoxLayout)
            continue;
        validcount++;
        m_SettingGroup->m_VLayout->addWidget(CreateLine());
        m_SettingGroup->m_VLayout->addLayout(tmpHBoxLayout);
    }
    validcount++;
    return validcount;
}

void LogicContentManager::UpdatePara(QObject* pObject)
{
    QString tmpObjectName = pObject->objectName();
    QDomElement tmpElement = m_ParaElement.firstChildElement(tmpObjectName);
    if(tmpElement.isNull())
        return;
    QString tmpName = tmpElement.tagName();
    if(!tmpName.compare("Backstage"))
    {
        Switch *tmpComponent = VAR_CAST<Switch *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyBool(tmpComponent, tmpElement, mBackStage, tmpComponent->isChecked()))
            {
            }
        }
    }
    else if(!tmpName.compare("UserLogo"))
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            ModifyTextEdit(tmpComponent, tmpElement, mUserLogo, tmpComponent->text());
        }
    }
    else if(!tmpName.compare("Device"))
    {
        UpdateDevicePara(pObject, tmpElement);
    }
}

void LogicContentManager::slot_Initialize()
{
#ifdef DISPLAY
    mSystem = m_DevManager->m_SYSVersion;
    mApplication = m_DevManager->m_APPVersion;
#endif

#ifdef IDE
    mDisk = VAR_CAST<PhysicalDisk*>(m_DevManager->m_PHDevList.value(P_DISK));
#elif DISPLAY
    mDisk = VAR_CAST<PhysicalDisk*>(m_DevManager->m_PHDevList.value(P_DISK));
#endif

    //>@查看是否为触控设备，如果是，则进行Manuual升级
#ifdef DISPLAY
    mLCD = VAR_CAST<PhysicalLCD*>(m_DevManager->m_PHDevList.value(P_LCD));
    if(!mLCD.isNull() && mLCD.data()->mTouchEn)
    {
#ifdef SUPPORT_TOUCH
        LogicThemeManager *tmpThemeManager = 0;
#ifdef IDE
        tmpThemeManager = VAR_CAST<LogicThemeManager*>(m_DevManager->m_LGDevList.value(L_ThemeManager));
#elif DISPLAY
        tmpThemeManager = VAR_CAST<LogicThemeManager*>(m_DevManager->m_LGDevList.value(L_ThemeManager));
#endif
#ifdef SUPPORT_SCENE
        UiScene *tmpUiContainer = tmpThemeManager->m_UiContainer;
#elif SUPPORT_WIDGET
        UiWidget *tmpUiContainer = tmpThemeManager->m_UiContainer;
#endif
        connect(this, SIGNAL(sSetMenu(bool)), tmpUiContainer, SLOT(slot_SetMenu(bool)));
#endif
    }
#endif
}

void LogicContentManager::DeviceStateChanged()
{
#ifdef SUPPORT_MUTEX
    if(!mUpgradeMutex->tryLock())
#else
    if(mUpgradeMutex)
#endif
    {
        IDE_DEBUG(QString("Can't Upgrade Content for mutex locked!"));
        return;
    }
#ifndef SUPPORT_MUTEX
    mUpgradeMutex = true;
#endif

    mUpgradeDev = VAR_CAST<PhysicalUpgrade*>(sender());
    if(mUpgradeDev && mUpgradeDev->mDevState == S_ON && mPHDevices.contains(mUpgradeDev->mDevType))
    {
        //>@计算末地址
        if(!mDisk.isNull())
        {
#ifdef SUPPORT_WIDGET
            if(!mLCD.isNull() && mLCD.data()->mTouchEn)
            {
                emit sSetMenu(true);
            }
            else
#endif
            {
                bool flag = false;
                flag = mUpgradeDev->UpgradeTo(mDisk.data()->mDevPath);
                if(!flag)
                {
                    emit sInfo(QString("Upgrade failed"), 4000);
#ifdef DISPLAY
                    m_DevManager->PlayBeep(D_Error);
#endif
                }
                else
                {
                    emit sInfo(QString("Upgrade success"), 4000);
#ifdef DISPLAY
                    m_DevManager->PlayBeep(D_Error);
#endif
                }
            }
        }
    }

#ifdef SUPPORT_MUTEX
    mUpgradeMutex->unlock();
#else
    mUpgradeMutex = false;
#endif
}


