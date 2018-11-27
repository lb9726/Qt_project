#include "logicmonitor.h"

LogicMonitor::LogicMonitor(QObject *parent)
    :LogicDevice(parent)
{
}

LogicMonitor::~LogicMonitor()
{
}

quint32 LogicMonitor::InitPara()
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
        if(!tmpName.compare("Size"))
        {
            tmpHBoxLayout = CreateTextEdit(tmpName, tmpElement.text());
        }
        else if(!tmpName.compare("Audio"))
        {
            tmpHBoxLayout = CreateBool(tmpName, tmpElement.text());
        }
        else if(!tmpName.compare("TDM"))
        {
            int tmpValue, tmpMinValue, tmpMaxValue;
            QStringList tmpValueList = tmpElement.attribute("List").split(';');
            if(tmpValueList.count()!=2)
            {
                tmpValue = 0;
                tmpMinValue = 0;
                tmpMaxValue = 0;
            }
            else
            {
                tmpMinValue = tmpValueList.at(0).toInt();
                tmpMaxValue = tmpValueList.at(1).toInt();
                if(tmpValue < tmpMinValue || tmpValue > tmpMaxValue)  //>@判断值是否有效
                    tmpValue = 0;
            }
            tmpHBoxLayout = CreateNum(tmpName, tmpValue, tmpMinValue, tmpMaxValue);
        }
        else if(!tmpName.compare("MotionAlarm"))
        {
            tmpHBoxLayout = CreateBool(tmpName, tmpElement.text());
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

bool LogicMonitor::SetValue(QString pValue)
{
    if(pValue.isEmpty())
        return true;
    if(!pValue.compare("start"))
    {
#ifdef DISPLAY
        //>@启动mjpeg-streamer
        system("start.sh");
#endif
    }
    else if(!pValue.compare("stop"))
    {
        //>@关闭mjpeg-streamer
        system("killall ffmpeg mplayer");
    }
    return true;
}

void LogicMonitor::UpdatePara(QObject* pObject)
{
    QString tmpObjectName = pObject->objectName();
    QDomElement tmpElement = m_ParaElement.firstChildElement(tmpObjectName);
    if(tmpElement.isNull())
        return;
    QString tmpName = tmpElement.tagName();
    if(!tmpName.compare("Size"))
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            QString tmpSize;
            if(!ModifyTextEdit(tmpComponent, tmpElement, tmpSize, tmpComponent->text()))
            {
            }
            else
            {
                mSize = RESOLUTION(tmpSize);
            }
        }
    }
    else if(!tmpName.compare("Audio"))
    {
        Switch *tmpComponent = VAR_CAST<Switch *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyBool(tmpComponent, tmpElement, mAudio, tmpComponent->isChecked()))
            {
            }
        }
    }
    else if(!tmpName.compare("TDM"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, tmpElement, mTDM, tmpComponent->value(), mMinTDM, mMaxTDM))
            {
            }
        }
    }
    else if(!tmpName.compare("MotionAlarm"))
    {
        Switch *tmpComponent = VAR_CAST<Switch *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyBool(tmpComponent, tmpElement, mMotionAlarm, tmpComponent->isChecked()))
            {
            }
        }
    }
    else if(!tmpName.compare("Device"))
    {
        UpdateDevicePara(pObject, tmpElement);
    }
}

