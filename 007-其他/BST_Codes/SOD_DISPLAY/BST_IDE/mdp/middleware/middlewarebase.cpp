#include "middlewarebase.h"

MiddlewareDevice::MiddlewareDevice(QObject *parent) :
    IODeviceBase(parent)
{
}

MiddlewareDevice::~MiddlewareDevice()
{
}

bool MiddlewareDevice::InitDevice(MW_Type pType, QDomElement pElement)
{
    if(pElement.isNull())
        return false;
    mDevName = pElement.tagName();
    if(!pElement.hasChildNodes())
        return false;
    m_ParaElement = pElement;
    //>@创建配置页面
    if(m_SettingGroup)
        m_SettingGroup->deleteLater();
    m_SettingGroup = new Groupbox(mDevName);
    if(!m_SettingGroup)
        return false;
    //>@初始化参数项
    quint32 count = InitPara();
    if(!count)
    {
        m_SettingGroup->deleteLater();
        m_SettingGroup = 0;
        return false;
    }
    m_SettingGroup->setFixedHeight(80+count*35);
    return true;
}

bool MiddlewareDevice::DownloadLibrary()
{
    return true;
}

bool MiddlewareDevice::GenerateScript()
{
    return true;
}

