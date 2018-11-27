#include "physicaltouch.h"
#ifdef SUPPORT_SCENE
#include "uiscene.h"
#elif SUPPORT_WIDGET
#include "uiwidget.h"
#endif
#include "deviceinc.h"

PhysicalTouch::PhysicalTouch(QObject *parent) :
    PhysicalMovable(parent)
{
}

PhysicalTouch::~PhysicalTouch()
{
}

void PhysicalTouch::slot_Initialize()
{
    if(isDevExist(QString("/dev/input/event1")))
    {
#ifdef SUPPORT_WIDGET
        emit sShowIcon("touch");
#elif SUPPORT_SCENE
        m_DevManager->ShowIcon("touch");
#endif
    }
    else
    {
#ifdef SUPPORT_WIDGET
        emit sHideIcon("touch");
#elif SUPPORT_SCENE
        m_DevManager->ShowIcon("");
#endif
    }
}

void PhysicalTouch::slot_DeviceAdded(QString dev)
{
    QStringList tmpList = dev.split('>');
    if(tmpList.count()!=2)
        return;
    QString tmpType = tmpList.at(0);
    if(tmpType.compare("touch", Qt::CaseInsensitive) == 0)  //>@touch
    {
        //>@重启应用程序，恢复touch，此处不需要点亮Touch标签
        qApp->quit();
        QProcess::startDetached(qApp->applicationFilePath(), QStringList());
    }
}

void PhysicalTouch::slot_DeviceRemoved(QString dev)
{
    QStringList tmpList = dev.split('>');
    if(tmpList.count()!=2)
        return;
    QString tmpType = tmpList.at(0);
    if(tmpType.compare("touch", Qt::CaseInsensitive) == 0)  //>@touch
    {
#ifdef DISPLAY
#ifdef SUPPORT_WIDGET
        emit sHideIcon("touch");
#elif SUPPORT_SCENE
        m_DevManager->ShowIcon("");
#endif
#endif
    }
}

quint32 PhysicalTouch::InitPara()
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

void PhysicalTouch::UpdatePara(QObject* pObject)
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
        }
    }
}
