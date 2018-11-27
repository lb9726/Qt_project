#include "physicalcpu.h"
#include "deviceinc.h"
#include "logicthememanager.h"

#ifdef LINUX
#define D_PWRMGR_DEV	QString("/sys/devices/platform/w55fa95-clk/clock")
#endif

PhysicalCPU::PhysicalCPU(QObject *parent) :
    PhysicalDevice(parent)
{
    mIsIdelState = false;

    mMonitorWait.mFreq = 6000;

#ifdef WINDOWS

#elif LINUX
    m_File = new QFile;
    m_File->setFileName(D_PWRMGR_DEV);
    if(m_File->open(QIODevice::ReadWrite|QIODevice::Unbuffered))
    {
        IDE_DEBUG("Open pwmmanager success!");
    }
#elif PHONE

#endif
}

PhysicalCPU::~PhysicalCPU()
{
#ifdef WINDOWS

#elif LINUX
    if(m_File)
        m_File->close();
#endif
}

QString GetUsbScanPath(QString pCPUModel)
{
    QString tmpScanPath;
    if(pCPUModel.compare("N329xx", Qt::CaseInsensitive)==0)
        tmpScanPath = QString("/sys/devices/platform/omap/musb-ti81xx/musb-hdrc.1/");
    else if(pCPUModel.compare("AM335x", Qt::CaseInsensitive)==0)
        tmpScanPath = QString("/sys/devices/platform/omap/musb-ti81xx/musb-hdrc.1/");
    else if(pCPUModel.compare("DM3730", Qt::CaseInsensitive)==0)
        tmpScanPath = QString("/sys/devices/platform/omap/musb-ti81xx/musb-hdrc.1/");
    else if(pCPUModel.compare("IMX6x", Qt::CaseInsensitive)==0)
        tmpScanPath = QString("/sys/devices/platform/omap/musb-ti81xx/musb-hdrc.1/");
    return tmpScanPath;
}

bool PhysicalCPU::GotoIdel()
{
#ifdef LINUX
#ifdef N329
    if (m_File->handle() < 0)
    {
        IDE_DEBUG(QString("Goto idel error!"));
        return false;
    }
    IDE_DEBUG(QString("Goto idel success!"));
    m_File->write("pd");
#endif
#endif
    mIsIdelState = true;
    return true;
}

bool PhysicalCPU::LeaveIdel()
{
#ifdef LINUX
#ifdef N329
    if (m_File->handle() < 0)
    {
        IDE_DEBUG(QString("Leave idel error!"));
        return false;
    }
    IDE_DEBUG(QString("Leave idel success!"));
    m_File->write("id");
#endif
#endif
    mIsIdelState = false;
    return true;
}

quint32 PhysicalCPU::Monitor(quint32 pCount)
{
    if(!mEnable)
        return 0;
    if(!mMonitorWait.isValid(pCount, m_DevManager->m_DevMonitorFreq))
        return 2;
    if(mAutoRelease && mThreshold.mNum)
    {
        double tmpTotal, tmpUsage;
        if(!SYSTEM_CMD::GetCpuLoad(tmpTotal, tmpUsage))
            return 0;
        mUsageRate = tmpUsage*100/tmpTotal;
#ifdef DISPLAY
        //>@通过关闭音视频播放模块以及关闭THEME特效释放CPU
        if(mUsageRate < mThreshold.mNum)
        {
            PhysicalPlayer *tmpPlayer = VAR_CAST<PhysicalPlayer *>(m_DevManager->GetPHDev(P_Player));
            if(tmpPlayer)
                tmpPlayer->Abort();
        }
#endif
    }
    return 1;
}

quint32 PhysicalCPU::InitPara()
{
    QDomNodeList tmpParaNodeList = m_ParaElement.childNodes();
    if(tmpParaNodeList.count() < 1)
        return 0;
    quint32 validcount = 0;
    QHBoxLayout* tmpHBoxLayout = 0;
    for(int i=0;i<tmpParaNodeList.count();i++)
    {
        QDomElement tmpElement = tmpParaNodeList.at(i).toElement();
        if(tmpElement.isNull())
            continue;
        QString tmpName = tmpElement.tagName();
        if(!tmpName.compare("Model"))
        {
            mCpuModel = tmpElement.text();
            mUsbScanPath = GetUsbScanPath(mCpuModel);
            tmpHBoxLayout = CreateEnum(tmpName, tmpElement.attribute("List").split(';'), mCpuModel);
        }
        else if(!tmpName.compare("AutoRelease"))
        {
            mAutoRelease = (tmpElement.text().compare("false")==0)?(false):(true);
            tmpHBoxLayout = CreateBool(tmpName, tmpElement.text());
        }
        else if(!tmpName.compare("Threshold"))
        {
            mThreshold.mNum = tmpElement.text().toInt();
            QStringList tmpValueList = tmpElement.attribute("List").split(';');
            if(tmpValueList.count()==2)
            {
                mThreshold.setnum(tmpElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
            }
            tmpHBoxLayout = CreateNum(tmpName, mThreshold.mNum, mThreshold.mMinNum, mThreshold.mMaxNum);
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

void PhysicalCPU::UpdatePara(QObject* pObject)
{
    QString tmpObjectName = pObject->objectName();
    QDomElement tmpElement = m_ParaElement.firstChildElement(tmpObjectName);
    if(tmpElement.isNull())
        return;
    QString tmpName = tmpElement.tagName();
    if(!tmpName.compare("Model"))
    {
        ComboBox *tmpComponent = VAR_CAST<ComboBox *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyEnum(tmpComponent, tmpElement, mCpuModel, tmpComponent->currentText()))
            {
            }
            else
            {
                mUsbScanPath = GetUsbScanPath(mCpuModel);
            }
        }
    }
    else if(!tmpName.compare("AutoRelease"))
    {
        Switch *tmpComponent = VAR_CAST<Switch *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyBool(tmpComponent, tmpElement, mAutoRelease, tmpComponent->isChecked()))
            {
            }
        }
    }
    else if(!tmpName.compare("Threshold"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, tmpElement, mThreshold.mNum, tmpComponent->value(), mThreshold.mMinNum, mThreshold.mMaxNum))
            {
            }
        }
    }
}

