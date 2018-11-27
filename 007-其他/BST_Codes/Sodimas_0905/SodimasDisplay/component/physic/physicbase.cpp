#include "physicbase.h"

PhysicBase::PhysicBase(GlobalSetting *globalSetting) :
    QObject(globalSetting)
{
    m_initialized = false;
    m_enabled = false;
    mDevMode = DM_NORMAL;
    m_globalSetting = globalSetting;
}

PhysicBase::PhysicBase()
{
    m_initialized = false;
    m_enabled = false;
    mDevMode = DM_NORMAL;
    m_globalSetting = NULL;
}

void PhysicBase::setGlobalSetting(GlobalSetting *globalSetting){
    m_globalSetting = globalSetting;
}

PhysicBase::~PhysicBase()
{

}

bool PhysicBase::initDevice(PH_Type pPHType, QDomElement pElement)
{
    m_NodeElement = pElement;
    return true;
}

void PhysicBase::slot_Initialize()
{

    m_initialized = true;
    disconnect(this, SIGNAL(sInfo(QString,int)), 0, 0);
    if(m_globalSetting)
    {
        mThemeManager = (ThemeLg*)(m_globalSetting->m_LGDevList.value(L_ThemeManager));
//        if(mThemeManager && mThemeManager->m_UiContainer)
//        {
//            disconnect(this, SIGNAL(sOperateTheme(AREA_OPERATE, QVariant)), 0, 0);
//            disconnect(this, SIGNAL(sInfo(QString,int,E_DEVMODE)), 0, 0);
//            disconnect(this, SIGNAL(sWarn(QString,int,E_DEVMODE)), 0, 0);
//            disconnect(this, SIGNAL(sError(QString,int,E_DEVMODE)), 0, 0);
//            disconnect(this, SIGNAL(sProgress(quint32)), 0, 0);
//            disconnect(this, SIGNAL(sProgress(quint32,quint32)), 0, 0);
//            connect(this, SIGNAL(sOperateTheme(AREA_OPERATE, QVariant)), mThemeManager, SLOT(slot_OperateTheme(AREA_OPERATE, QVariant)), Qt::QueuedConnection);
//            connect(this, SIGNAL(sInfo(QString,int,E_DEVMODE)), mThemeManager->m_UiContainer, SIGNAL(sInfo(QString,int,E_DEVMODE)), Qt::QueuedConnection);
//            connect(this, SIGNAL(sWarn(QString,int,E_DEVMODE)), mThemeManager->m_UiContainer, SIGNAL(sWarn(QString,int,E_DEVMODE)), Qt::QueuedConnection);
//            connect(this, SIGNAL(sError(QString,int,E_DEVMODE)), mThemeManager->m_UiContainer, SIGNAL(sError(QString,int,E_DEVMODE)), Qt::QueuedConnection);
//            connect(this, SIGNAL(sProgress(quint32)), mThemeManager->m_UiContainer, SIGNAL(sProgress(quint32)), Qt::QueuedConnection);
//            connect(this, SIGNAL(sProgress(quint32,quint32)), mThemeManager->m_UiContainer, SIGNAL(sProgress(quint32,quint32)), Qt::QueuedConnection);
//        }
    }

    if(m_globalSetting)
        m_LCD = (LcdPh*)(m_globalSetting->m_PHDevList.value(P_LCD));
    if(m_LCD)
    {
        //disconnect(this, SIGNAL(sFeedLCD()), 0, 0);
        //connect(this, SIGNAL(sFeedLCD()), m_LCD.data(), SLOT(slot_FeedLCD()), Qt::QueuedConnection);
    }

}

//pDevMode表示设置设备禁能的优先级，优先级低的无法执行成功，如果mDevMode = DM_NORMAL则表示任何优先级都可以操作
bool PhysicBase::SetEnable(bool pFlag, DEV_MODE pDevMode, bool pForce)
{
    if(!pForce)
    {
        if(pDevMode < mDevMode)   //离开禁能模式时需要判断是否是否可以离开，只有优先级高的DEV才可以使之离开禁能模式
        {
            if(pFlag == m_enabled)
                return false;
            IDE_DEBUG(QString("new devmode %1[%2] is smaller than %3[%4] ").arg(pDevMode).arg(pFlag).arg(mDevMode).arg(m_enabled));
            return false;
        }
        if(pFlag == m_enabled)
        {
            bool diff = (pDevMode == mDevMode);
            if(pFlag)
                mDevMode = DM_NORMAL;
            else
                mDevMode = pDevMode;
            if(diff)
                return false;
            return true;
        }
    }
    if(pFlag)
    {
        mDevMode = DM_NORMAL;
        m_enabled = true;
    }
    else
    {
        mDevMode = pDevMode;
        m_enabled = false;
    }
    return true;
}

bool PhysicBase::setElementText(QDomElement pElement, QString pValue)
{
    if(pElement.isNull())
        return false;
    if(pElement.text().compare(pValue))
    {
        if(m_globalSetting && m_globalSetting->m_mdpParser)
        {
            return m_globalSetting->m_mdpParser->setText(pElement, pValue);
        }
    }
    return true;
}

bool PhysicBase::setPara(QString pParaName, QString pParaValue, bool pSyncUI)
{
    //获取控件类型
    QDomElement tmpOldElement = m_NodeElement.firstChildElement(pParaName);
    return setPara(tmpOldElement, pParaValue, pSyncUI);
}

bool PhysicBase::setPara(QDomElement pElement, QString pParaValue, bool pSyncUI)
{
    if(pElement.isNull())
        return false;
    if(!pSyncUI)
    {
        if(m_globalSetting && m_globalSetting->m_mdpParser)
            return m_globalSetting->m_mdpParser->setText(pElement, pParaValue);
        return false;
    }

    return true;
}

bool PhysicBase::setPara(QDomElement &pElement, XmlParse *pParse)
{
    if(pElement.isNull() || pParse==0)
        return false;
    //设置参数部分
    QDomNodeList tmpParaList = pElement.childNodes();
    for(int i=0;i<tmpParaList.count();i++)
    {
        QDomElement tmpParaElement = tmpParaList.at(i).toElement();
        if(!tmpParaElement.isNull())
        {
            QString tmpParaName = tmpParaElement.tagName();
            QDomElement tmpOldElement = m_NodeElement.firstChildElement(tmpParaName);
            if(tmpOldElement.isNull())
                continue;
            //设置参数的属性部分
            QList<QDomAttr> tmpList = pParse->getItemElementAttrs(tmpParaElement);
            for(int j=0;j<tmpList.count();j++)
            {
                QDomAttr tmpDomAttr = tmpList.at(j);
                if(tmpDomAttr.isNull())
                    continue;
                tmpOldElement.setAttribute(tmpDomAttr.name(), tmpDomAttr.value());
            }
            //设置此参数的值
            setPara(tmpOldElement, tmpParaElement.text());
        }
    }
    //执行命令部分
    return SetValue(pElement.text());
}

QString PhysicBase::getPara(QString pParaName)
{
    QDomElement tmpOldElement = m_NodeElement.firstChildElement(pParaName);
    return getPara(tmpOldElement);
}

QString PhysicBase::getPara(QDomElement pElement)
{
    if(pElement.isNull())
        return QString();
    return pElement.text();
}

bool PhysicBase::getPara(QDomElement &pElement, XmlParse *pParse)
{
    if(pElement.isNull() || pParse==0)
        return false;
    //获取参数部分
    QDomNodeList tmpParaList = pElement.childNodes();
    for(int i=0;i<tmpParaList.count();i++)
    {
        QDomElement tmpParaElement = tmpParaList.at(i).toElement();
        if(!tmpParaElement.isNull())
        {
            QString tmpParaName = tmpParaElement.tagName();
            QDomElement tmpOldElement = m_NodeElement.firstChildElement(tmpParaName);
            if(tmpOldElement.isNull())
                continue;
            //获取参数的属性部分
            QList<QDomAttr> tmpList = pParse->getItemElementAttrs(tmpParaElement);
            for(int j=0;j<tmpList.count();j++)
            {
                QDomAttr tmpDomAttr = tmpList.at(j);
                if(tmpDomAttr.isNull())
                    continue;
                tmpDomAttr.setValue(tmpOldElement.attribute(tmpDomAttr.name()));
            }
            //获取此参数的值
            pParse->setText(tmpParaElement, getPara(tmpOldElement));
        }
    }
    pParse->setText(pElement, GetValue(pElement.text()));
    return true;
}

bool PhysicBase::saveMdp()
{
    if(m_globalSetting && m_globalSetting->m_mdpParser && m_globalSetting->m_mdpParser->m_IsChanged)
        return m_globalSetting->m_mdpParser->saveMdp();
    return false;
}

//修改XML以及判断此值是否有效
bool PhysicBase::modifyBool(QDomElement &pElement, bool &pDstValue, bool pNewValue)
{
    if(!pElement.isNull())
    {
        bool tmpOldValue = (pElement.text().compare("true", Qt::CaseInsensitive)) ? false : true;
        if(tmpOldValue != pNewValue)
        {
            if(!setElementText(pElement, pNewValue?("true"):("false")))
            {
                return false;
            }
            pDstValue = pNewValue;
        }
        return true;
    }
    return false;
}

bool PhysicBase::modifyEnum(QDomElement &pElement, QString &pDstValue, QString pNewValue)
{
    if(!pElement.isNull())
    {
        QString tmpOldValue = pElement.text();
        if(tmpOldValue.compare(pNewValue))
        {
            if(!setElementText(pElement, pNewValue))
            {
                return false;
            }
            pDstValue = pNewValue;
        }
        return true;
    }
    return false;
}

bool PhysicBase::modifyNum(QDomElement &pElement, int &pDstValue, int pNewValue, int pMinValue, int pMaxValue)
{
    if(!pElement.isNull())
    {
        int tmpOldValue = pElement.text().toInt();
        if(tmpOldValue != pNewValue)
        {

            if(pNewValue < pMinValue || pNewValue > pMaxValue)  //判断值是否有效
            {
                return false;
            }
            if(!setElementText(pElement,QString("%1").arg(pNewValue)))
            {
                return false;
            }
            pDstValue = pNewValue;
        }
        return true;
    }
    return false;
}

bool PhysicBase::modifyText(QDomElement &pElement, QString &pDstValue, QString pNewValue)
{
    if(!pElement.isNull())
    {
        QString tmpOldValue = pElement.text();
        if(tmpOldValue.compare(pNewValue))
        {
            if(!setElementText(pElement, pNewValue))
            {
                return false;
            }
            pDstValue = pNewValue;
        }
        return true;
    }
    return false;
}
