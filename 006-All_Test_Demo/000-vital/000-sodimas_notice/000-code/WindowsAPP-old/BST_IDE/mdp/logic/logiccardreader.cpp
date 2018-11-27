#include "logiccardreader.h"

LogicCardReader::LogicCardReader(QObject *parent)
    :LogicDevice(parent)
{
}

LogicCardReader::~LogicCardReader()
{
}

quint32 LogicCardReader::InitPara()
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
        if(!tmpName.compare("CardMode"))
        {
            QStringList tmpDevList = tmpElement.text().split(';');
            tmpHBoxLayout = CreateCheck(tmpName, tmpElement.attribute("List").split(';'), tmpDevList);
        }
        else if(!tmpName.compare("KeyAlarm"))
        {
            tmpHBoxLayout = CreateTextEdit(tmpName, tmpElement.text());
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

void LogicCardReader::UpdatePara(QObject* pObject)
{
    QString tmpObjectName = pObject->objectName();
    QDomElement tmpElement = m_ParaElement.firstChildElement(tmpObjectName);
    if(tmpElement.isNull())
        return;
    QString tmpName = tmpElement.tagName();
    if(!tmpName.compare("CardMode"))
    {
        CheckList *tmpComponent = VAR_CAST<CheckList *>(pObject);
        if(tmpComponent)
        {
            QStringList tmpList = tmpComponent->GetSelectList();
            if(!ModifyCheck(tmpComponent, tmpElement, mCardModes, tmpList))
            {
            }
        }
    }
    else if(!tmpName.compare("KeyAlarm"))
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyTextEdit(tmpComponent, tmpElement, mKeyAlarm, tmpComponent->text()))
            {
            }
        }
    }
    else if(!tmpName.compare("Device"))
    {
        UpdateDevicePara(pObject, tmpElement);
    }
}
