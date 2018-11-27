#include "logicdlna.h"


LogicDLNA::LogicDLNA(QObject *parent)
    :LogicDevice(parent)
{
}

LogicDLNA::~LogicDLNA()
{
}

quint32 LogicDLNA::InitPara()
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
        if(!tmpName.compare("Name"))
        {
            tmpHBoxLayout = CreateTextEdit(tmpName, tmpElement.text());
        }
        else if(!tmpName.compare("Port"))
        {
            QStringList tmpValueList = tmpElement.attribute("List").split(';');
            if(tmpValueList.count()!=2)
            {
                mPort = 0;
                mMinPort = 0;
                mMaxPort = 0;
            }
            else
            {
                mMinPort = tmpValueList.at(0).toInt();
                mMaxPort = tmpValueList.at(1).toInt();
                if(mPort < mMinPort || mPort > mMaxPort)  //>@判断值是否有效
                    mPort = 0;
            }
            tmpHBoxLayout = CreateNum(tmpName, mPort, mMinPort, mMaxPort);
        }
        else if(!tmpName.compare("MediaCatalog"))
        {
            tmpHBoxLayout = CreateTextEdit(tmpName, tmpElement.text());
        }
        else if(!tmpName.compare("DBCatalog"))
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

void LogicDLNA::UpdatePara(QObject* pObject)
{
    QString tmpObjectName = pObject->objectName();
    QDomElement tmpElement = m_ParaElement.firstChildElement(tmpObjectName);
    if(tmpElement.isNull())
        return;
    QString tmpName = tmpElement.tagName();
    if(!tmpName.compare("Name"))
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyTextEdit(tmpComponent, tmpElement, mName, tmpComponent->text()))
            {
            }
        }
    }
    else if(!tmpName.compare("Port"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, tmpElement, mPort, tmpComponent->value(), mMinPort, mMaxPort))
            {
            }
        }
    }
    else if(!tmpName.compare("MediaCatalog"))
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyTextEdit(tmpComponent, tmpElement, mMediaCatalog, tmpComponent->text()))
            {
            }
        }
    }
    else if(!tmpName.compare("DBCatalog"))
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyTextEdit(tmpComponent, tmpElement, mDBCatalog, tmpComponent->text()))
            {
            }
        }
    }
    else if(!tmpName.compare("Device"))
    {
        UpdateDevicePara(pObject, tmpElement);
    }
}
