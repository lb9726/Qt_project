#include "physicalwd.h"
#include "deviceinc.h"
#ifdef LINUX
#include <linux/watchdog.h>
#include <sys/ioctl.h>
#endif

PhysicalWD::PhysicalWD(QObject *parent) :
    PhysicalDevice(parent)
{
    mMonitorWait.SetTimeout(3000);

    m_WDTimeout.setnum(0);
    m_WDFreq = 0;

    m_WDFile = new QFile;
    m_WDFile->setFileName(QString("/dev/watchdog"));
#if 0
    if(m_WDFile->open(QIODevice::WriteOnly))
    {
        int tmpSecond = 10;
        ioctl(m_WDFile->handle(), WDIOC_SETTIMEOUT, &tmpSecond);
        ioctl(m_WDFile->handle(), WDIOC_GETTIMEOUT, &tmpSecond);
        IDE_DEBUG(QString("Initialize watchdog timeout [%1s]").arg(tmpSecond));
    }
#endif
}

PhysicalWD::~PhysicalWD()
{
    if(m_WDFile)
    {
        m_WDFile->close();
        m_WDFile->deleteLater();
    }
}

quint32 PhysicalWD::Monitor(quint32 pCount)
{
    if(!mMonitorWait.isValid(pCount, m_DevManager->m_DevMonitorFreq))
        return 2;
    //>@执行看门狗
#ifdef N329
    if(!m_WDFile->isOpen())
    {
        if(m_WDFile->open(QIODevice::ReadWrite|QIODevice::Unbuffered))
        {
            int tmpSecond = 5;
            IDE_DEBUG(QString("Open watchdog success"));
            ioctl(m_WDFile->handle(), WDIOC_SETTIMEOUT, &tmpSecond);
        }
        else
        {
            IDE_TRACE();
            return 0;
        }
    }
    if(m_WDFile->handle())
    {
        //IDE_DEBUG("Feed dog Success!");
        ioctl(m_WDFile->handle(), WDIOC_KEEPALIVE);
    }
#elif IMX6X
    if(!m_WDFile->isOpen())
    {
        int tmpSecond = m_WDTimeout.mNum / 1000 * 2;  //>@自动设置喂狗的时间为超时时间的一半
        IDE_TRACE_INT(m_WDTimeout.mNum);
        if((tmpSecond > 0) && m_WDFile->open(QIODevice::WriteOnly))
        {
            ioctl(m_WDFile->handle(), WDIOC_SETTIMEOUT, &tmpSecond);
            ioctl(m_WDFile->handle(), WDIOC_GETTIMEOUT, &m_WDFreq);
            IDE_DEBUG(QString("Open watchdog success, timeout [%1s], feed freq [%2ms]").arg(m_WDFreq).arg(m_WDTimeout.mNum));
        }
        else
        {
            IDE_DEBUG(QString("Open watchdog failed"));
            return 0;
        }
    }
    if(m_WDFile->handle())
    {
#if 1
        ioctl(m_WDFile->handle(), WDIOC_KEEPALIVE, 0);
        IDE_DEBUG("Feed dog Success!");
#else
        qint64 ret = m_WDFile->write("\0", 1);
        if(ret != 1)
            IDE_DEBUG("Feed dog Failed!");
        else
            IDE_DEBUG("Feed dog Success!");
#endif
    }
#endif
    return 1;
}

quint32 PhysicalWD::InitPara()
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
        if(!tmpName.compare("Freq"))
        {
            QStringList tmpValueList = tmpElement.attribute("List").split(';');
            if(tmpValueList.count()==2)
            {
                m_WDTimeout.setnum(tmpElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
                mMonitorWait.SetTimeout(m_WDTimeout.mNum);
            }
            tmpHBoxLayout = CreateNum(tmpName, m_WDTimeout.mNum, m_WDTimeout.mMinNum, m_WDTimeout.mMaxNum);
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

void PhysicalWD::UpdatePara(QObject* pObject)
{
    QString tmpObjectName = pObject->objectName();
    QDomElement tmpElement = m_ParaElement.firstChildElement(tmpObjectName);
    if(tmpElement.isNull())
        return;
    QString tmpName = tmpElement.tagName();
    if(!tmpName.compare("Timeout"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, tmpElement, m_WDTimeout.mNum, tmpComponent->value(), m_WDTimeout.mMinNum, m_WDTimeout.mMaxNum))
            {
            }
        }
    }
}



