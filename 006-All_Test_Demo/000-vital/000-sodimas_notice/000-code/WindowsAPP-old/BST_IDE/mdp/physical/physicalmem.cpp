#include "physicalmem.h"
#include "deviceinc.h"

PhysicalMEM::PhysicalMEM(QObject *parent):
    PhysicalDevice(parent)
{
#ifdef IMX6X
    mMonitorWait.SetTimeout(4000);
#else
    mMonitorWait.SetTimeout(60000);
#endif
}

PhysicalMEM::~PhysicalMEM()
{
}

quint32 PhysicalMEM::Monitor(quint32 pCount)
{
    if(!mMonitorWait.isValid(pCount, m_DevManager->m_DevMonitorFreq))
        return 2;
    if(mAutoRelease && mThreshold.mNum)
    {
#ifdef IMX6X
        system("sync");
        system("echo 3 > /proc/sys/vm/drop_caches");
        IDE_DEBUG("Mem released!");
#endif
#ifdef N329
        double tmpMemoryUsed, tmpMemoryFree, tmpMemoryTotal;
        if(!SYSTEM_CMD::GetMemSpace(tmpMemoryTotal, tmpMemoryUsed, tmpMemoryFree))
            return 0;
        mUsageRate = tmpMemoryUsed * 100 / tmpMemoryTotal;
        if(mUsageRate > mThreshold.mNum)
        {
#ifdef DISPLAY
            //>@通过释放THEME中缓存的图片释放MEM
            LogicThemeManager *tmpThemeManager = VAR_CAST<LogicThemeManager *>(m_DevManager->GetLGDev(L_ThemeManager));
            if(!tmpThemeManager)
                return 0;
            tmpThemeManager->ReleaseImageBuffer();
#endif
        }
#endif
    }
    return 1;
}

quint32 PhysicalMEM::InitPara()
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
        if(!tmpName.compare("Model"))
        {
            mMemModel = tmpElement.text();
            tmpHBoxLayout = CreateTextEdit(tmpName, mMemModel);
        }
        else if(!tmpName.compare("AutoRelease"))
        {
            mAutoRelease = (tmpElement.text().compare("false")==0)?(false):(true);
            tmpHBoxLayout = CreateBool(tmpName, tmpElement.text());
        }
        else if(!tmpName.compare("Threshold"))
        {
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

void PhysicalMEM::UpdatePara(QObject* pObject)
{
    QString tmpObjectName = pObject->objectName();
    QDomElement tmpElement = m_ParaElement.firstChildElement(tmpObjectName);
    if(tmpElement.isNull())
        return;
    QString tmpName = tmpElement.tagName();
    if(!tmpName.compare("Model"))
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyTextEdit(tmpComponent, tmpElement, mMemModel, tmpComponent->text()))
            {
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
