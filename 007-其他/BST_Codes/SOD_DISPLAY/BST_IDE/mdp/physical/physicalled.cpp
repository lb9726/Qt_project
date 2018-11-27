#include "physicalled.h"
#include "deviceinc.h"

PhysicalLED::PhysicalLED(QObject *parent) :
    PhysicalDevice(parent)
{
    m_LEDState = 0;
    m_File = new QFile;
    m_File->setFileName(QString("/dev/led"));
    m_File->open(QIODevice::ReadWrite|QIODevice::Unbuffered);
}

PhysicalLED::~PhysicalLED()
{
    if(m_File)
    {
        m_File->close();
        m_File->deleteLater();
    }
}

//>@将当前LED设置为某种状态下的闪烁方式
void PhysicalLED::SetBlink(QString pSpeed)
{
    m_BlinkFreq = 2000;
    if(!m_File->isOpen())
        return;
    if(!pSpeed.compare("Slow", Qt::CaseInsensitive))
        m_BlinkFreq = 1000;
    else if(!pSpeed.compare("Fast", Qt::CaseInsensitive))
        m_BlinkFreq = 500;
    else if(!pSpeed.compare("High", Qt::CaseInsensitive))
        m_BlinkFreq = 100;
    mMonitorWait.SetTimeout(m_BlinkFreq);
}

quint32 PhysicalLED::Monitor(quint32 pCount)
{
    if(!mMonitorWait.isValid(pCount, m_DevManager->m_DevMonitorFreq))
        return 2;
    if(!m_File->isOpen())
    {
        if(!m_File->open(QIODevice::ReadWrite|QIODevice::Unbuffered))
            return 0;
    }
    if(m_LEDState)
        m_LEDState = 0;
    else
        m_LEDState = 1;
    QByteArray tmpArray;
    tmpArray.append(m_LEDState);
    m_File->write(tmpArray);
    tmpArray.clear();
    return 1;
}

quint32 PhysicalLED::InitPara()
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
        if(!tmpName.compare("Normal"))
        {
            m_NormalSpeed = tmpElement.text();
            tmpHBoxLayout = CreateEnum(tmpName, tmpElement.attribute("List").split(';'), m_NormalSpeed);
        }
        else if(!tmpName.compare("Busy"))
        {
            m_BusySpeed = tmpElement.text();
            tmpHBoxLayout = CreateEnum(tmpName, tmpElement.attribute("List").split(';'), m_BusySpeed);
        }
        else if(!tmpName.compare("Eroor"))
        {
            m_ErrorSpeed = tmpElement.text();
            tmpHBoxLayout = CreateEnum(tmpName, tmpElement.attribute("List").split(';'), m_ErrorSpeed);
        }
        else
        {
            continue;
        }
        validcount++;
        m_SettingGroup->m_VLayout->addWidget(CreateLine());
        m_SettingGroup->m_VLayout->addLayout(tmpHBoxLayout);
    }
    //>@
    SetBlink(m_NormalSpeed);
    return validcount;
}

void PhysicalLED::UpdatePara(QObject* pObject)
{
    QString tmpObjectName = pObject->objectName();
    QDomElement tmpElement = m_ParaElement.firstChildElement(tmpObjectName);
    if(tmpElement.isNull())
        return;
    QString tmpName = tmpElement.tagName();
    if(!tmpName.compare("Normal"))
    {
        ComboBox *tmpComponent = VAR_CAST<ComboBox *>(pObject);
        if(tmpComponent)
        {
            ModifyEnum(tmpComponent, tmpElement, m_NormalSpeed, tmpComponent->currentText());
        }
    }
    else if(!tmpName.compare("Busy"))
    {
        ComboBox *tmpComponent = VAR_CAST<ComboBox *>(pObject);
        if(tmpComponent)
        {
            ModifyEnum(tmpComponent, tmpElement, m_BusySpeed, tmpComponent->currentText());
        }
    }
    else if(!tmpName.compare("Eroor"))
    {
        ComboBox *tmpComponent = VAR_CAST<ComboBox *>(pObject);
        if(tmpComponent)
        {
            ModifyEnum(tmpComponent, tmpElement, m_ErrorSpeed, tmpComponent->currentText());
        }
    }
}



