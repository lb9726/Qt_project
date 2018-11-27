#include "devfun.h"

#include "deviceinc.h"


#include "physicallcd.h"


void WaitInfo::SetTimeout(quint32 pValue)
{
    mFreq = pValue;
}

bool WaitInfo::isValid(quint32 pCurCount, quint32 pFreq)
{
    if(pFreq <= 0)
        return false;
    if(mNext != pCurCount)
        return false;
    update(pCurCount, pFreq);
    return true;
}

void WaitInfo::update(quint32 pCurCount, quint32 pFreq)
{
    if(pFreq)
        mNext = (pCurCount + mFreq / pFreq) % D_MAXTIMEOUT;
}

IODeviceBase::IODeviceBase(QObject *parent) :
    QIODevice(parent)
{
    mInitialized = false;

    mEnable = false;
    mDevMode = DM_NORMAL;

    m_SettingGroup = 0;
    SetDeviceManager(parent);
}

IODeviceBase::~IODeviceBase()
{
    if(m_SettingGroup)
    {
        m_SettingGroup->deleteLater();
        m_SettingGroup = 0;
    }
}

bool IODeviceBase::SetDeviceManager(QObject *pManager)
{
    m_DevManager = VAR_CAST<DeviceManager *>(pManager);

    if(m_DevManager)
    {
        connect(this, SIGNAL(sSetDevices(bool,E_DEVMODE)), m_DevManager, SLOT(SetDevicesEnable(bool,E_DEVMODE)), Qt::QueuedConnection);
        connect(this, SIGNAL(sProgress(QString,quint32)), m_DevManager, SIGNAL(sProgress(QString,quint32)), Qt::QueuedConnection);
        connect(this, SIGNAL(sProgress(QString,quint32,quint32)), m_DevManager, SIGNAL(sProgress(QString,quint32,quint32)), Qt::QueuedConnection);
        connect(this, SIGNAL(sBeep(QString)), m_DevManager, SIGNAL(sBeep(QString)), Qt::QueuedConnection);
        connect(this, SIGNAL(sIcon(QString)), m_DevManager, SIGNAL(sIcon(QString)), Qt::QueuedConnection);

        connect(this, SIGNAL(sShowIcon(QString)), m_DevManager, SIGNAL(sShowIcon(QString)), Qt::QueuedConnection);
        connect(this, SIGNAL(sHideIcon(QString)), m_DevManager, SIGNAL(sHideIcon(QString)), Qt::QueuedConnection);
    }

}

bool IODeviceBase::SetElementText(QDomElement pElement, QString pValue)
{
    if(pElement.isNull())
        return false;
    if(pElement.text().compare(pValue))
    {
        if(m_DevManager && m_DevManager->mMdpManager)
        {
            return m_DevManager->mMdpManager->SetText(pElement, pValue);
        }
    }
    return true;
}

quint32 IODeviceBase::InitPara()
{
    QDomNodeList tmpParaNodeList = m_ParaElement.childNodes();
    if(tmpParaNodeList.isEmpty())
        return 0;
    quint32 validcount = 0;
    QHBoxLayout* tmpHBoxLayout = 0;
    for(int i=0;i<tmpParaNodeList.count();i++)
    {
        QDomElement tmpElement = tmpParaNodeList.at(i).toElement();
        if(tmpElement.isNull())
            continue;
        QString tmpName = tmpElement.tagName();
        UI_TYPE tmpType = getUiType(tmpElement.attribute("Para"));
        QStringList tmpList = tmpElement.attribute("List").split(';');
        QString tmpValue = tmpElement.text();
        if(tmpType == UI_CHECK)
        {
            tmpHBoxLayout = CreateCheck(tmpName, tmpList, tmpValue.split(';'));
        }
        else if(tmpType == UI_ENUM)
        {
            tmpHBoxLayout = CreateEnum(tmpName, tmpList, tmpValue);
        }
        else if(tmpType == UI_NUM)
        {
            if(tmpList.count() != 2)
                continue;
            tmpHBoxLayout = CreateNum(tmpName, tmpValue.toInt(), tmpList.at(0).toInt(), tmpList.at(1).toInt());
        }
        else if(tmpType == UI_BOOL)
        {
            tmpHBoxLayout = CreateBool(tmpName, tmpValue);
        }
        else if(tmpType == UI_ENETADDR)
        {
            tmpHBoxLayout = CreateENetAddrEdit(tmpName, tmpValue);
        }
        else if(tmpType == UI_TIMEEDIT)
        {
            tmpHBoxLayout = CreateDateTimeEdit(tmpName, QDateTime::fromString(tmpValue));
        }
        else if(tmpType == UI_BUTTON)
        {
            tmpHBoxLayout = CreateButton(tmpName, tmpList);
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

void IODeviceBase::UpdatePara(QObject* pObject)
{
    QString tmpObjectName = pObject->objectName();
    QDomElement tmpElement = m_ParaElement.firstChildElement(tmpObjectName);
    if(tmpElement.isNull())
        return;
    QString tmpName = tmpElement.tagName();
    UI_TYPE tmpType = getUiType(tmpElement.attribute("Para"));
    if(tmpType == UI_CHECK)
    {
        CheckList *tmpComponent = VAR_CAST<CheckList *>(pObject);
        if(tmpComponent)
        {
            QStringList tmpList;
            if(!ModifyCheck(tmpComponent, tmpElement, tmpList, tmpComponent->GetSelectList()))
            {
                emit sError(QString(""));
            }
        }
    }
    else if(tmpType == UI_ENUM)
    {
        ComboBox *tmpComponent = VAR_CAST<ComboBox *>(pObject);
        if(tmpComponent)
        {
            QString tmpValue;
            if(!ModifyEnum(tmpComponent, tmpElement, tmpValue, tmpComponent->currentText()))
            {
                emit sError(QString(""));
            }
        }
    }
    else if(tmpType == UI_NUM)
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            int tmpValue;
            if(!ModifyNum(tmpComponent, tmpElement, tmpValue, tmpComponent->value()))
            {
                emit sError(QString(""));
            }
        }
    }
    else if(tmpType == UI_BOOL)
    {
        Switch *tmpComponent = VAR_CAST<Switch *>(pObject);
        if(tmpComponent)
        {
            bool tmpValue;
            if(!ModifyBool(tmpComponent, tmpElement, tmpValue, tmpComponent->isChecked()))
            {
                emit sError(QString(""));
            }
        }
    }
    else if(tmpType == UI_ENETADDR)
    {
        ENetAddrEdit *tmpComponent = VAR_CAST<ENetAddrEdit *>(pObject);
        if(tmpComponent)
        {
            ENET_ADDR tmpValue;
            if(!ModifyENetAddrEdit(tmpComponent, tmpElement, tmpValue, tmpComponent->GetIP()))
            {
                emit sError(QString(""));
            }
        }
    }
    else if(tmpType == UI_TIMEEDIT)
    {
        DateTimeEdit *tmpComponent = VAR_CAST<DateTimeEdit *>(pObject);
        if(tmpComponent)
        {
            QDateTime tmpValue;
            if(!ModifyDateTimeEdit(tmpComponent, tmpElement, tmpValue, tmpComponent->GetDateTime()))
            {
                emit sError(QString(""));
            }
        }
    }
    else if(tmpType == UI_BUTTON)
    {
        ButtonList *tmpComponent = VAR_CAST<ButtonList *>(pObject);
        if(tmpComponent)
        {
        }
    }
}

bool IODeviceBase::SetPara(QString pParaName, QString pParaValue, bool pSyncUI)
{
    //>@获取控件类型
    QDomElement tmpOldElement = m_ParaElement.firstChildElement(pParaName);
    return SetPara(tmpOldElement, pParaValue, pSyncUI);
}

bool IODeviceBase::SetPara(QDomElement pElement, QString pParaValue, bool pSyncUI)
{
    if(pElement.isNull())
        return false;
    if(!pSyncUI)
    {
        if(m_DevManager && m_DevManager->mMdpManager)
            return m_DevManager->mMdpManager->SetText(pElement, pParaValue);
        return false;
    }
    QString pParaName = pElement.tagName();
    UI_TYPE tmpType = getUiType(pElement.attribute("Para"));
    switch(tmpType)
    {
        case UI_ENUM:
        {
            ComboBox *tmpComponent = m_SettingGroup->findChild<ComboBox *>(pParaName);
            if(tmpComponent)
            {
                tmpComponent->setCurrentText(pParaValue);
            }
            break;
        }
        case UI_BOOL:
        {
            Switch *tmpComponent = m_SettingGroup->findChild<Switch *>(pParaName);
            if(tmpComponent)
            {
                tmpComponent->setChecked((pParaValue.compare("TRUE", Qt::CaseInsensitive))?false:true);
            }
            break;
        }
        case UI_STRING:
        {
            TextEdit *tmpComponent = m_SettingGroup->findChild<TextEdit *>(pParaName);
            if(tmpComponent)
            {
                tmpComponent->setText(pParaValue);
            }
            break;
        }
        case UI_ENETADDR:
        {
            ENetAddrEdit *tmpComponent = m_SettingGroup->findChild<ENetAddrEdit *>(pParaName);
            if(tmpComponent)
            {
                tmpComponent->SetIP(pParaValue);
            }
            break;
        }
        case UI_TIMEEDIT:
        {
            DateTimeEdit *tmpComponent = m_SettingGroup->findChild<DateTimeEdit *>(pParaName);
            if(tmpComponent)
            {
                tmpComponent->SetDateTime(pParaValue);
            }
            break;
        }
        case UI_CHECK:
        {
            CheckList *tmpComponent = m_SettingGroup->findChild<CheckList *>(pParaName);
            if(tmpComponent)
            {
                tmpComponent->InitCheckList(pParaValue.split(';'));
            }
            break;
        }
        case UI_NUM:
        {
            SpinEdit *tmpComponent = m_SettingGroup->findChild<SpinEdit *>(pParaName);
            if(tmpComponent)
            {
                tmpComponent->setValue(pParaValue.toInt());
            }
            break;
        }
        default:return false;
    }
    return true;
}

bool IODeviceBase::SetPara(QDomElement &pElement, xmlParse *pParse)
{
    if(pElement.isNull() || pParse==0)
        return false;
    //>@设置参数部分
    QDomNodeList tmpParaList = pElement.childNodes();
    for(int i=0;i<tmpParaList.count();i++)
    {
        QDomElement tmpParaElement = tmpParaList.at(i).toElement();
        if(!tmpParaElement.isNull())
        {
            QString tmpParaName = tmpParaElement.tagName();
            QDomElement tmpOldElement = m_ParaElement.firstChildElement(tmpParaName);
            if(tmpOldElement.isNull())
                continue;
            //>@设置参数的属性部分
            QList<QDomAttr> tmpList = pParse->GetItemElementAttrs(tmpParaElement);
            for(int j=0;j<tmpList.count();j++)
            {
                QDomAttr tmpDomAttr = tmpList.at(j);
                if(tmpDomAttr.isNull())
                    continue;
                tmpOldElement.setAttribute(tmpDomAttr.name(), tmpDomAttr.value());
            }
            //>@设置此参数的值
            SetPara(tmpOldElement, tmpParaElement.text());
        }
    }
    //>@执行命令部分
    return SetValue(pElement.text());
}

QString IODeviceBase::GetPara(QString pParaName)
{
    QDomElement tmpOldElement = m_ParaElement.firstChildElement(pParaName);
    return GetPara(tmpOldElement);
}

QString IODeviceBase::GetPara(QDomElement pElement)
{
    if(pElement.isNull())
        return QString();
    return pElement.text();
}

bool IODeviceBase::GetPara(QDomElement &pElement, xmlParse *pParse)
{
    if(pElement.isNull() || pParse==0)
        return false;
    //>@获取参数部分
    QDomNodeList tmpParaList = pElement.childNodes();
    for(int i=0;i<tmpParaList.count();i++)
    {
        QDomElement tmpParaElement = tmpParaList.at(i).toElement();
        if(!tmpParaElement.isNull())
        {
            QString tmpParaName = tmpParaElement.tagName();
            QDomElement tmpOldElement = m_ParaElement.firstChildElement(tmpParaName);
            if(tmpOldElement.isNull())
                continue;
            //>@获取参数的属性部分
            QList<QDomAttr> tmpList = pParse->GetItemElementAttrs(tmpParaElement);
            for(int j=0;j<tmpList.count();j++)
            {
                QDomAttr tmpDomAttr = tmpList.at(j);
                if(tmpDomAttr.isNull())
                    continue;
                tmpDomAttr.setValue(tmpOldElement.attribute(tmpDomAttr.name()));
            }
            //>@获取此参数的值
            pParse->SetText(tmpParaElement, GetPara(tmpOldElement));
        }
    }
    pParse->SetText(pElement, GetValue(pElement.text()));
    return true;
}

bool IODeviceBase::SaveMdp()
{
    if(m_DevManager && m_DevManager->mMdpManager && m_DevManager->mMdpManager->m_IsChanged)
        return m_DevManager->mMdpManager->SaveMDP();
    return false;
}

//>@pDevMode表示设置设备禁能的优先级，优先级低的无法执行成功，如果mDevMode = DM_NORMAL则表示任何优先级都可以操作
bool IODeviceBase::SetEnable(bool pFlag, E_DEVMODE pDevMode, bool pForce)
{
    if(!pForce)
    {
        if(pDevMode < mDevMode)   //>@离开禁能模式时需要判断是否是否可以离开，只有优先级高的DEV才可以使之离开禁能模式
        {
            if(pFlag == mEnable)
                return false;
            IDE_DEBUG(QString("new devmode %1[%2] is smaller than %3[%4] ").arg(pDevMode).arg(pFlag).arg(mDevMode).arg(mEnable));
            return false;
        }
        if(pFlag == mEnable)
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
        mEnable = true;
    }
    else
    {
        mDevMode = pDevMode;
        mEnable = false;
    }
    return true;
}

quint32 IODeviceBase::Monitor(quint32 pCount)
{
    return 0;
}


GraphicsScene *IODeviceBase::GetThemeScene()
{
    LogicThemeManager *tmpThemeLGDev = VAR_CAST<LogicThemeManager*>(m_DevManager->m_LGDevList.value(L_ThemeManager));
    if(!tmpThemeLGDev)
        return (GraphicsScene*)0;
    return tmpThemeLGDev->m_UiContainer;

}


void IODeviceBase::slot_Initialize()
{
    mInitialized = true;
    disconnect(this, SIGNAL(sInfo(QString,int)), 0, 0);
    if(m_DevManager)
    {

        mThemeManager = VAR_CAST<LogicThemeManager*>(m_DevManager->m_LGDevList.value(L_ThemeManager));
        if(mThemeManager && mThemeManager->m_UiContainer)
        {
            disconnect(this, SIGNAL(sOperateTheme(AREA_OPERATE, QVariant)), 0, 0);
            disconnect(this, SIGNAL(sInfo(QString,int,E_DEVMODE)), 0, 0);
            disconnect(this, SIGNAL(sWarn(QString,int,E_DEVMODE)), 0, 0);
            disconnect(this, SIGNAL(sError(QString,int,E_DEVMODE)), 0, 0);
            disconnect(this, SIGNAL(sProgress(quint32)), 0, 0);
            disconnect(this, SIGNAL(sProgress(quint32,quint32)), 0, 0);

            connect(this, SIGNAL(sOperateTheme(AREA_OPERATE, QVariant)), mThemeManager, SLOT(slot_OperateTheme(AREA_OPERATE, QVariant)), Qt::QueuedConnection);
            connect(this, SIGNAL(sInfo(QString,int,E_DEVMODE)), mThemeManager->m_UiContainer, SIGNAL(sInfo(QString,int,E_DEVMODE)), Qt::QueuedConnection);
            connect(this, SIGNAL(sWarn(QString,int,E_DEVMODE)), mThemeManager->m_UiContainer, SIGNAL(sWarn(QString,int,E_DEVMODE)), Qt::QueuedConnection);
            connect(this, SIGNAL(sError(QString,int,E_DEVMODE)), mThemeManager->m_UiContainer, SIGNAL(sError(QString,int,E_DEVMODE)), Qt::QueuedConnection);
            connect(this, SIGNAL(sProgress(quint32)), mThemeManager->m_UiContainer, SIGNAL(sProgress(quint32)), Qt::QueuedConnection);
            connect(this, SIGNAL(sProgress(quint32,quint32)), mThemeManager->m_UiContainer, SIGNAL(sProgress(quint32,quint32)), Qt::QueuedConnection);
        }
    }

    if(m_DevManager)
        m_LCD = VAR_CAST<PhysicalLCD*>(m_DevManager->m_PHDevList.value(P_LCD));
    if(!m_LCD.isNull())
    {
        disconnect(this, SIGNAL(sFeedLCD()), 0, 0);
        connect(this, SIGNAL(sFeedLCD()), m_LCD.data(), SLOT(slot_FeedLCD()), Qt::QueuedConnection);
    }

}

QFrame* IODeviceBase::CreateLine()
{
    QFrame *line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    return line;
}

QHBoxLayout* IODeviceBase::CreateButton(QString pName, QStringList pBtnListTag)
{
    QHBoxLayout *tmpHBoxLayout = new QHBoxLayout;
    Label *tmpLabel = new Label(pName);
    ButtonList *tmpButtonList = new ButtonList(pBtnListTag);
    tmpButtonList->setObjectName(pName);
    connect(tmpButtonList, SIGNAL(sChanged(QObject*)), this, SLOT(UpdatePara(QObject*)));

    tmpHBoxLayout->addWidget(tmpLabel);
    tmpHBoxLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum));
    tmpHBoxLayout->addLayout(tmpButtonList);
    return tmpHBoxLayout;
}


QHBoxLayout* IODeviceBase::CreateTextEdit(QString pName, QString pValue)
{
    QHBoxLayout *tmpHBoxLayout = new QHBoxLayout;
    Label *tmpLabel = new Label(pName);
    TextEdit *tmpTextEdit = new TextEdit(pValue);
    tmpTextEdit->setObjectName(pName);
    connect(tmpTextEdit, SIGNAL(sChanged(QObject*)), this, SLOT(UpdatePara(QObject*)));

    tmpHBoxLayout->addWidget(tmpLabel);
    tmpHBoxLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum));
    tmpHBoxLayout->addWidget(tmpTextEdit);
    return tmpHBoxLayout;
}

QHBoxLayout* IODeviceBase::CreateEnum(QString pName, QStringList pParaList, QString pSelectPara)
{
    QHBoxLayout *tmpHBoxLayout = new QHBoxLayout;
    Label *tmpLabel = new Label(pName);
    ComboBox *tmpComboBox = new ComboBox(pParaList, pSelectPara);
    tmpComboBox->setObjectName(pName);
    connect(tmpComboBox, SIGNAL(sChanged(QObject*)), this, SLOT(UpdatePara(QObject*)));

    tmpHBoxLayout->addWidget(tmpLabel);
    tmpHBoxLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum));
    tmpHBoxLayout->addWidget(tmpComboBox);
    return tmpHBoxLayout;
}
QHBoxLayout* IODeviceBase::CreateBool(QString pName, QString pPara)
{
    QHBoxLayout *tmpHBoxLayout = new QHBoxLayout;
    Label *tmpLabel = new Label(pName);
    Switch *tmpSwitch = new Switch(pPara, "TRUE", "FALSE");
    tmpSwitch->setObjectName(pName);
    connect(tmpSwitch, SIGNAL(sChanged(QObject*)), this, SLOT(UpdatePara(QObject*)));

    tmpHBoxLayout->addWidget(tmpLabel);
    tmpHBoxLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum));
    tmpHBoxLayout->addWidget(tmpSwitch);
    return tmpHBoxLayout;
}

QHBoxLayout* IODeviceBase::CreateENetAddrEdit(QString pName, QString pIP)
{
    QHBoxLayout *tmpHBoxLayout = new QHBoxLayout;
    Label *tmpLabel = new Label(pName);
    ENetAddrEdit *tmpNetAddrEdit = new ENetAddrEdit(pIP);
    tmpNetAddrEdit->setObjectName(pName);
    connect(tmpNetAddrEdit, SIGNAL(sChanged(QObject*)), this, SLOT(UpdatePara(QObject*)));

    tmpHBoxLayout->addWidget(tmpLabel);
    tmpHBoxLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum));
    tmpHBoxLayout->addWidget(tmpNetAddrEdit);
    return tmpHBoxLayout;
}
QHBoxLayout* IODeviceBase::CreateDateTimeEdit(QString pName, QDateTime pDateTime)
{
    QHBoxLayout *tmpHBoxLayout = new QHBoxLayout;
    Label *tmpLabel = new Label(pName);
    DateTimeEdit *tmpDateTimeEdit = new DateTimeEdit(pDateTime);
    tmpDateTimeEdit->setObjectName(pName);
    connect(tmpDateTimeEdit, SIGNAL(sChanged(QObject*)), this, SLOT(UpdatePara(QObject*)));

    tmpHBoxLayout->addWidget(tmpLabel);
    tmpHBoxLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum));
    tmpHBoxLayout->addWidget(tmpDateTimeEdit);
    return tmpHBoxLayout;
}

QHBoxLayout* IODeviceBase::CreateCheck(QString pName, QStringList pParaList, QStringList pSelectParaList)
{
    QHBoxLayout *tmpHBoxLayout = new QHBoxLayout;
    Label *tmpLabel = new Label(pName);
    CheckList *tmpCheckList = new CheckList(pParaList, pSelectParaList);
    tmpCheckList->setObjectName(pName);
    connect(tmpCheckList, SIGNAL(sChanged(QObject*)), this, SLOT(UpdatePara(QObject*)));
    //mParaItems.insert(pName, tmpCheckList);
    //>@
    tmpHBoxLayout->addWidget(tmpLabel);
    tmpHBoxLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum));
    tmpHBoxLayout->addLayout(tmpCheckList);
    return tmpHBoxLayout;
}

QHBoxLayout* IODeviceBase::CreateNum(QString pName, int pValue, int pMinValue, int pMaxValue)
{
    QHBoxLayout *tmpHBoxLayout = new QHBoxLayout;
    Label *tmpLabel = new Label(pName);
    SpinEdit *tmpSpinEdit = new SpinEdit(pValue, pMinValue, pMaxValue);
    tmpSpinEdit->setObjectName(pName);
    connect(tmpSpinEdit, SIGNAL(sChanged(QObject*)), this, SLOT(UpdatePara(QObject*)));
    //mParaItems.insert(pName, tmpSpinEdit);
    //>@
    tmpHBoxLayout->addWidget(tmpLabel);
    tmpHBoxLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum));
    tmpHBoxLayout->addWidget(tmpSpinEdit);
    return tmpHBoxLayout;
}

bool IODeviceBase::ModifyButton(ButtonList *pComponent, QDomElement &pElement, QStringList &pDstValue, QStringList pNewValue)
{
    if(pComponent && !pElement.isNull())
    {
        return true;
    }
    return false;
}

bool IODeviceBase::ModifyTextEdit(TextEdit *pComponent, QDomElement &pElement, QString &pDstValue, QString pNewValue)
{
    if(pComponent && !pElement.isNull())
    {
        QString tmpOldValue = pElement.text();
        if(tmpOldValue.compare(pNewValue))
        {
            if(!SetElementText(pElement, pNewValue))
            {
                pComponent->setText(tmpOldValue);
                return false;
            }
            pDstValue = pNewValue;
        }
        return true;
    }
    return false;
}

bool IODeviceBase::ModifyEnum(ComboBox *pComponent, QDomElement &pElement, QString &pDstValue, QString pNewValue)
{
    if(pComponent && !pElement.isNull())
    {
        QString tmpOldValue = pElement.text();
        if(tmpOldValue.compare(pNewValue))
        {
            if(!SetElementText(pElement, pNewValue))
            {
                pComponent->setCurrentText(tmpOldValue);
                return false;
            }
            pDstValue = pNewValue;
        }
        return true;
    }
    return false;
}

bool IODeviceBase::ModifyBool(Switch *pComponent, QDomElement &pElement, bool &pDstValue, bool pNewValue)
{
    if(pComponent && !pElement.isNull())
    {
        bool tmpOldValue = (pElement.text().compare("TRUE", Qt::CaseInsensitive))?false:true;
        if(tmpOldValue != pNewValue)
        {
            if(!SetElementText(pElement, pNewValue?("TRUE"):("FALSE")))
            {
                pComponent->setChecked(tmpOldValue);
                return false;
            }
            pDstValue = pNewValue;
        }
        return true;
    }
    return false;
}

bool IODeviceBase::ModifyENetAddrEdit(ENetAddrEdit *pComponent, QDomElement &pElement, ENET_ADDR &pDstValue, ENET_ADDR pNewValue)
{
    if(pComponent && !pElement.isNull())
    {
        QString tmpOldValue = pElement.text();
        QString tmpNewValue = pNewValue.toQString();
        if(tmpOldValue.compare(tmpNewValue))
        {
            if(!SetElementText(pElement, tmpNewValue))
            {
                pComponent->SetIP(tmpOldValue);
                return false;
            }
            pDstValue = pNewValue;
        }
        return true;
    }
    return false;
}

bool IODeviceBase::ModifyDateTimeEdit(DateTimeEdit *pComponent, QDomElement &pElement, QDateTime &pDstValue, QDateTime pNewValue)
{
    if(pComponent && !pElement.isNull())
    {
        QString tmpNewValue = pElement.text();
        QString tmpOldValue = pNewValue.toString(Qt::LocalDate);
        if(tmpNewValue.compare(tmpOldValue))
        {
            if(!SetElementText(pElement, pNewValue.toString(Qt::LocalDate)))
            {
                pComponent->SetDateTime(QDateTime::fromString(tmpOldValue, Qt::LocalDate));
                return false;
            }
            pDstValue = pNewValue;
        }
        return true;
    }
    return false;
}



bool IODeviceBase::ModifyCheck(CheckList *pComponent, QDomElement &pElement, QStringList &pDstValue, QStringList pNewValue)
{
    if(pComponent && !pElement.isNull())
    {
        QString tmpOldValue = pElement.text();
        QString tmpNewValue = pComponent->GetSelectText(pNewValue);
        if(tmpOldValue.compare(tmpNewValue))
        {
            if(!SetElementText(pElement, tmpNewValue))
            {
                pComponent->InitCheckList(tmpOldValue.split(';'));
                return false;
            }
            pDstValue = pNewValue;
        }
        return true;
    }
    return false;
}

//>@修改XML以及判断此值是否有效
bool IODeviceBase::ModifyNum(SpinEdit *pComponent, QDomElement &pElement, int &pDstValue, int pNewValue, int pMinValue, int pMaxValue)
{
    if(pComponent && !pElement.isNull())
    {
        int tmpOldValue = pElement.text().toInt();
        if(tmpOldValue != pNewValue)
        {
            if(pMinValue == -1)
                pMinValue = pComponent->m_MinValue;
            if(pMaxValue == -1)
                pMaxValue = pComponent->m_MaxValue;
            if(pNewValue < pMinValue || pNewValue > pMaxValue)  //>@判断值是否有效
            {
                pComponent->setValue(tmpOldValue);
                return false;
            }
            if(!SetElementText(pElement,QString("%1").arg(pNewValue)))
            {
                pComponent->setValue(tmpOldValue);
                return false;
            }
            pDstValue = pNewValue;
        }
        return true;
    }
    return false;
}

DevManager::DevManager() :
    BstUiManager()
{
    m_DevMonitor = 0;
    m_DevMonitorFreq = 100;
#ifdef QT_V453
    m_TranparentPix = QPixmap(D_FSRCPATH + D_BOOTDIR + QString("transparent.png"));
#endif
}

DevManager::~DevManager()
{
    SetMonitor(0);  //>@
}

bool DevManager::SetMonitor(quint32 pFreq)
{
    if(pFreq == 0)  //>@停止
    {
        bool ret = true;
        if(m_DevMonitor)
        {
            m_DevMonitor->ForceQuit();
            ret = m_DevMonitor->wait(1000);
            if(!ret)
                m_DevMonitor->terminate();  //>@使用run的thread使用quit没有用处，只有terminate有用处。
            m_DevMonitor->deleteLater();
            m_DevMonitor = 0;
        }
        return ret;
    }
    if(!m_DevMonitor)
        m_DevMonitor = new DeviceMonitor(this);
    if(pFreq > 10)
    {
        m_DevMonitorFreq = pFreq;
    }
    if(!m_DevMonitor->isRunning())
        m_DevMonitor->start();
    return true;
}

DeviceMonitor::DeviceMonitor(DevManager *qesp) :
    QThread()
{
    m_DevManager = qesp;
    m_Quit = false;
    m_Count = 0;
}

DeviceMonitor::~DeviceMonitor()
{
    m_Quit = true;
}
void DeviceMonitor::ForceQuit()
{
    m_Quit = true;
}

void DeviceMonitor::run()
{
    while(!m_Quit)
    {
        if(m_DevManager)
        {
            m_Count++;
            if(m_Count == D_MAXTIMEOUT)
                m_Count = 0;

            m_DevManager->Monitor(m_Count);

            Wait(m_DevManager->m_DevMonitorFreq);
        }
        else
        {
            Wait(50);
        }
    }
}

void DeviceMonitor::Wait(int pTimeout)
{
    quint32 i=0;
    while(!m_Quit && ((i+=10) < pTimeout))
        msleep(10);
}

