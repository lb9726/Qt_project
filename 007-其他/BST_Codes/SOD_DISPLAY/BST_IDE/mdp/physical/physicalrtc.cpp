#include "physicalrtc.h"
#include "deviceinc.h"

PhysicalRTC::PhysicalRTC(QObject *parent):
    PhysicalDevice(parent)
{
    mMonitorWait.SetTimeout(60*1000 - 100);  //>@默认一分钟更新一次
}

PhysicalRTC::~PhysicalRTC()
{
}

quint32 PhysicalRTC::InitPara()
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
        if(!tmpName.compare("SecondEnable"))
        {
            QString tmpValue = tmpElement.text();
            mSecondEnable = (tmpValue.compare("false")==0)?(false):(true);
            tmpHBoxLayout = CreateBool(tmpName, tmpValue);
            if(mSecondEnable)
                mMonitorWait.SetTimeout(900);
            else
                mMonitorWait.SetTimeout(60*1000 - 100);
        }
        else
        {
            continue;
        }
        validcount++;
        m_SettingGroup->m_VLayout->addWidget(CreateLine());
        m_SettingGroup->m_VLayout->addLayout(tmpHBoxLayout);
    }
    return validcount;
}

void PhysicalRTC::UpdatePara(QObject* pObject)
{
    QString tmpObjectName = pObject->objectName();
    QDomElement tmpElement = m_ParaElement.firstChildElement(tmpObjectName);
    if(tmpElement.isNull())
        return;
    QString tmpName = tmpElement.tagName();
    if(!tmpName.compare("SecondEnable"))
    {
        Switch *tmpComponent = VAR_CAST<Switch *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyBool(tmpComponent, tmpElement, mSecondEnable, tmpComponent->isChecked()))
            {
            }
            else
            {
                if(mSecondEnable)
                    mMonitorWait.SetTimeout(900);
                else
                    mMonitorWait.SetTimeout(60*1000 - 100);
            }
        }
    }
}

void PhysicalRTC::slot_Initialize()
{
#ifdef DISPLAY
    LogicThemeManager *tmpThemeManager = VAR_CAST<LogicThemeManager *>(m_DevManager->GetLGDev(L_ThemeManager));
    if(tmpThemeManager && tmpThemeManager->m_EventMap)
    {
        connect(this, SIGNAL(sUpdateTime(QDateTime)), tmpThemeManager->m_EventMap, SLOT(slot_AlarmEvent(QDateTime)), Qt::QueuedConnection);
    }
#endif
}

QDateTime PhysicalRTC::GetTime()
{
    mCurDateTime = QDateTime::currentDateTime();
    return mCurDateTime;
}

bool PhysicalRTC::SyncTime(QDateTime pDateTime)
{
    if(pDateTime.isNull())
        return false;
    mCurDateTime = pDateTime;
#ifdef WINDOWS
    IDE_DEBUG(mCurDateTime.toString("yyyy-MM-dd h:m"));
    SYSTEMTIME *tmpTime = new SYSTEMTIME;
    tmpTime->wYear = mCurDateTime.date().year();
    tmpTime->wMonth = mCurDateTime.date().month();
    tmpTime->wDay = mCurDateTime.date().day();
    tmpTime->wHour = mCurDateTime.time().hour();
    tmpTime->wMinute = mCurDateTime.time().minute();
    tmpTime->wSecond = mCurDateTime.time().second();
    ::SetSystemTime(tmpTime);
    delete tmpTime;
#else
    QString tmpCmd = QString("date -s \"%1\"").arg(mCurDateTime.toString("yyyy-MM-dd h:m")).toLatin1().data();
    system(tmpCmd.toLatin1().data());
    system("hwclock -w");
    //IDE_TRACE_STR(tmpCmd);
#endif
    UpdateClockUI();
    return true;
}

bool PhysicalRTC::SetValue(QString pValue)
{
    QDateTime tmpDateTime = QDateTime::fromString(pValue, "yyyy-MM-dd h:m");
    return SyncTime(tmpDateTime);
}

void PhysicalRTC::UpdateClockUI()
{
    mCurDateTime = QDateTime::currentDateTime();
    //>@更新时间
    if(!mSecondEnable)      //>@如果在秒钟不使能的情况下，将秒钟清零
        mCurDateTime.time().setHMS(mCurDateTime.time().hour(), mCurDateTime.time().minute(), 0);
    emit sUpdateTime(mCurDateTime);
    //IDE_DEBUG(mCurDateTime.toString("yyyy-MM-dd h:m"));
}

quint32 PhysicalRTC::Monitor(quint32 pCount)
{
    if(!mEnable)
    {
        mMonitorWait.update(pCount, m_DevManager->m_DevMonitorFreq);
        return 0;
    }
    if(!mMonitorWait.isValid(pCount, m_DevManager->m_DevMonitorFreq))
        return 2;
    UpdateClockUI();
    return 1;
}


