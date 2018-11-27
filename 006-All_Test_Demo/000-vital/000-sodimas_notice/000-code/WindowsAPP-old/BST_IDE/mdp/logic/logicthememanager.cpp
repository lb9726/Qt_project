#include "logicthememanager.h"
#include "deviceinc.h"
#include "physicalnet.h"
#include "physicalplayer.h"

LogicThemeManager::LogicThemeManager(QObject *parent)
    :LogicDevice(parent)
{
#ifdef DISPLAY
    m_UiContainer = 0;
    m_EventMap = new EventMap(this);
#endif
    mThemeLayout[0] = mThemeLayout[1] = 0;

#ifdef N329
    m_TransparentPix = m_DevManager->m_TranparentPix;
#endif
}

LogicThemeManager::~LogicThemeManager()
{
#ifdef DISPLAY
    if(m_EventMap)
        m_EventMap->deleteLater();
#endif
    ReleaseTheme();
}

//>@查找某种类型的文件
QStringList LogicThemeManager::SearchThemes(QString pDirPath)
{
    if(pDirPath.endsWith('/')==false)
        pDirPath.append('/');
    QDir dir = QDir(pDirPath);
    QStringList findList;
    QStringList folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (int i = 0; i < folders.size(); ++i)
    {
        QString tmpThemeName = folders[i];
        if(!tmpThemeName.endsWith('/'))
            tmpThemeName.append('/');
        if(QFile::exists(pDirPath + tmpThemeName + D_UINAME))
            findList.append(folders[i]);
    }
    return findList;
}

bool LogicThemeManager::SetAutoTest(bool pFlag)
{
#ifdef DISPLAY
    //>@查看是否为大屏文本模式
    if(pFlag)
    {
        mAutoTestEnable = true;
        m_UiContainer->OperateUI(OPERATE_SHOW);
#ifdef SUPPORT_SCENE
        GraphicsText *tmpText = VAR_CAST<GraphicsText *>(m_UiContainer->m_ComDiagramGroup.value(COM_TEXT));
        if(tmpText)
        {
            if(tmpText->m_ScreenMode == 1)
            {
                tmpText->SetTextShowMode(0, true);
            }
        }
#endif
        m_UiContainer->update();
    }
    else
    {
        mAutoTestEnable = false;
        QList<COM_TYPE> tmpList;
        tmpList.clear();
        tmpList << COM_LOGO;
#ifdef SUPPORT_SCENE
        GraphicsText *tmpText = VAR_CAST<GraphicsText *>(m_UiContainer->m_ComDiagramGroup.value(COM_TEXT));
        if(tmpText)
        {
            if(tmpText->GetTextShowMode() == 1)
            {
                tmpText->SetTextShowMode(1, false);
            }
        }
#endif
        m_UiContainer->OperateUI(OPERATE_HIDE, tmpList);
        m_UiContainer->update();
    }
#endif
}

bool LogicThemeManager::SetEnable(bool pFlag, E_DEVMODE pDevMode, bool pForce)
{
#ifdef DISPLAY
    if(m_UiContainer && IODeviceBase::SetEnable(pFlag, pDevMode, pForce))
    {
        if(pFlag)
        {
            QList<COM_TYPE> tmpList;
#ifdef SUPPORT_SCENE
            GraphicsText *tmpText = VAR_CAST<GraphicsText *>(m_UiContainer->m_ComDiagramGroup.value(COM_TEXT));
            if(tmpText)
            {
                if(tmpText->GetTextShowMode() == 1)
                {
                    tmpList << COM_FLOOR << COM_FUNCTION << COM_ARROW;
                }
            }
#endif
            m_UiContainer->OperateUI(OPERATE_SHOW, tmpList);
            m_UiContainer->update();

            //>@当从网络连接模式退出时，需要重新初始化可能被修改的参数
            if(pDevMode == DM_NET)
            {
                if(mThemeLayoutBk != mThemeLayout[0])
                    SetPara(QString("Layout"), QString::number(mThemeLayoutBk), true);
            }
        }
        else
        {
            QList<COM_TYPE> tmpList;
            tmpList.clear();
            tmpList << COM_LOGO;
            if(pDevMode == DM_BIGSRN)
            {
                tmpList << COM_TEXT;
            }
            else if(pDevMode == DM_STANDBY)
            {
                tmpList << COM_TEXT << COM_FLOOR << COM_FUNCTION << COM_ARROW;
            }
            m_UiContainer->OperateUI(OPERATE_HIDE, tmpList);
            m_UiContainer->update();

            //>@当进入网络连接模式时，需要保存可能被修改的参数
            if(pDevMode == DM_NET)
            {
                mThemeLayoutBk = mThemeLayout[0];
            }
        }
        return true;
    }
#endif
    return false;
}

QString LogicThemeManager::GetInformation()
{
#ifdef DISPLAY
    if(m_UiContainer)
    {
        //>@遍历所有主题
        QString tmpThemeNameString;
        tmpThemeNameString.append(mActiveTheme);
        for (int i=0; i<mThemeNameList.size(); ++i)
        {
            QString tmpThemeName = mThemeNameList.at(i);
            if(!mActiveTheme.compare(tmpThemeName))
                continue;
            tmpThemeNameString.append(',');
            tmpThemeNameString.append(tmpThemeName);
        }
        return QString("%1>%2,%3").arg(mDevName).arg(mThemesPath).arg(tmpThemeNameString);
    }
#endif
    return QString("%1").arg(mDevName);
}

quint32 LogicThemeManager::InitPara()
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
        if(!tmpName.compare("Location"))
        {
            QString tmpValue = tmpElement.text();
            mLocation = tmpValue;
            tmpHBoxLayout = CreateTextEdit(tmpName, tmpValue);
        }
        else if(!tmpName.compare("Layout"))
        {
            QString tmpLanguage = tmpElement.text();
            QStringList tmpLanguageList = tmpElement.attribute("List").split(';', QString::SkipEmptyParts);
            mThemeLayout[0] = tmpLanguageList.indexOf(tmpLanguage);
            mThemeLayoutBk = mThemeLayout[0];
            tmpHBoxLayout = CreateEnum(tmpName, tmpLanguageList, tmpLanguage);
        }
        else if(!tmpName.compare("ActiveTheme"))
        {
            mActiveTheme = tmpElement.text();
            mThemeNameList = tmpElement.attribute("List").split(';', QString::SkipEmptyParts);
            tmpHBoxLayout = CreateEnum(tmpName, mThemeNameList, mActiveTheme);
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

    QString tmpThemeListDir;
#ifdef DISPLAY
    PhysicalDisk *tmpDev = VAR_CAST<PhysicalDisk *>(m_DevManager->m_PHDevList.value(P_DISK));
    if(tmpDev)
        tmpThemeListDir = tmpDev->mDevPath;
    if(tmpThemeListDir.isEmpty())
        tmpThemeListDir = D_FSRCPATH;
#elif SETTING
    tmpThemeListDir = m_DevManager->mBufferPath;
#endif
    mThemesPath = tmpThemeListDir + D_THEMEDIR;

#ifdef DISPLAY
    //>@启动界面
#if 0
    QTimer::singleShot(100, this, SLOT(InitTheme()));
#else
    InitTheme();
#endif
#endif

//    connect(&mTestTimer, SIGNAL(timeout()), this, SLOT(slot_TestTimerTimeout()));
//    mTestTimer.setInterval(8000);
//    mTestTimer.start();

    return validcount;
}

void LogicThemeManager::slot_TestTimerTimeout()
{
    static int i = 0;
    if(i >= mThemeNameList.count())
        i = 0;
    mActiveTheme = mThemeNameList.at(i);
#ifdef DISPLAY
    m_DevManager->SetDevicesEnable(false, DM_INIT);
    QString tmpThemePath;
    if(!mActiveTheme.isEmpty())
        tmpThemePath = mThemesPath + mActiveTheme + QString("/") + D_UINAME;
    else
        tmpThemePath = D_FSRCPATH + D_THEMEDIR + D_DEFAULTTHEMEDIR + D_UINAME;
    if(m_UiContainer->LoadUI(tmpThemePath))
        m_DevManager->SetDevicesEnable(true, DM_INIT);
#endif
    i++;
}

bool LogicThemeManager::hasTheme()
{
    bool ret = false;
#ifdef DISPLAY
    ret = m_UiContainer && m_UiContainer->m_UiManager;
#endif
    return ret;
}

//>@查看主题路径，将当前磁盘中有效的主题同步到MDP文件中。
void LogicThemeManager::UpdateThemeList()
{
    QDomElement tmpElement = m_ParaElement.firstChildElement("ActiveTheme");
#ifdef DISPLAY
    QString tmpThemeListDir;
    if(tmpThemeListDir.isEmpty())
    {
        if(!tmpElement.isNull())
        {
            tmpElement.removeAttribute("List");
            m_UiContainer->m_UiManager->ModifyItemElement(tmpElement, QString());
        }
        mThemeNameList.clear();
        mActiveTheme.clear();
        return;
    }
#endif
    mThemeNameList = SearchThemes(mThemesPath);
    if(!mThemeNameList.isEmpty() && !mThemeNameList.contains(mActiveTheme))
        mActiveTheme = mThemeNameList.first();
#ifdef DISPLAY
    if(!tmpElement.isNull())
    {
        tmpElement.setAttribute(QString("List"), mThemeNameList.join(";"));
        m_UiContainer->m_UiManager->ModifyItemElement(tmpElement, mActiveTheme);
    }
#endif
    //>@同步更改UI
    ComboBox *tmpComponent = m_SettingGroup->findChild<ComboBox *>("ActiveTheme");
    if(tmpComponent)
        tmpComponent->InitEnumList(mThemeNameList, mActiveTheme);
}

void LogicThemeManager::UpdatePara(QObject* pObject)
{
    QString tmpObjectName = pObject->objectName();
    QDomElement tmpElement = m_ParaElement.firstChildElement(tmpObjectName);
    if(tmpElement.isNull())
        return;
    QString tmpName = tmpElement.tagName();
    if(!tmpName.compare("Location"))
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            QString tmpLocation;
            if(!ModifyTextEdit(tmpComponent, tmpElement, tmpLocation, tmpComponent->text()))
            {
            }
            else
            {
                mLocation = tmpLocation;
            }
        }
    }
    else if(!tmpName.compare("Layout"))
    {
        ComboBox *tmpComponent = VAR_CAST<ComboBox *>(pObject);
        if(tmpComponent)
        {
            QString tmpString;
            if(!ModifyEnum(tmpComponent, tmpElement, tmpString, tmpComponent->currentText()))
            {
            }
            else
            {
                mThemeLayout[0] = tmpComponent->currentIndex();
#ifdef DISPLAY
                m_UiContainer->ModifyLayout(mThemeLayout[0]);
                //1  箭头在前 4.3寸
                //2  箭头在后 4.3寸
                //3  箭头在前 4.0寸
                //4  箭头在后 4.0寸
#endif
            }
        }
    }
    else if(!tmpName.compare("ActiveTheme"))
    {
        ComboBox *tmpComponent = VAR_CAST<ComboBox *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyEnum(tmpComponent, tmpElement, mActiveTheme, tmpComponent->currentText()))
            {
            }
            else
            {
#ifdef DISPLAY
                m_DevManager->SetDevicesEnable(false, DM_INIT);
                QString tmpThemePath;
                if(!mActiveTheme.isEmpty())
                    tmpThemePath = mThemesPath + mActiveTheme + QString("/") + D_UINAME;
                else
                    tmpThemePath = D_FSRCPATH + D_THEMEDIR + D_DEFAULTTHEMEDIR + D_UINAME;
                if(m_UiContainer->LoadUI(tmpThemePath))
                    m_DevManager->SetDevicesEnable(true, DM_INIT);
#endif
            }
        }
    }
    else if(!tmpName.compare("Device"))
    {
        UpdateDevicePara(pObject, tmpElement);
    }
}

void LogicThemeManager::DeviceStateChanged()
{
#ifdef DISPLAY
    PhysicalMobileDisk *tmpPHDev = VAR_CAST<PhysicalMobileDisk*>(sender());
    if(!tmpPHDev)
        return;
    //>@如果存储设备插上，并且当前没有正在运行的主题，则重启程序
    if(tmpPHDev->mExecutable && tmpPHDev->mDevState == S_ON)
    {
    }
    //>@如果存储设备拔下，并且当前正在运行的主题在此设备中，则重启程序
    else if(tmpPHDev->mDevState == S_OFF)
    {
    }
#endif
}

void LogicThemeManager::slot_OperateTheme(AREA_OPERATE pOperate, QVariant pPara)
{
    switch(pOperate)
    {
    case OPERATE_INIT:
        InitTheme();
        break;
    case OPERATE_RELEASE:
        ReleaseTheme();
        break;
    case OPERATE_CLEAR:
        IDE_TRACE();
        ClearTheme();
        break;
    case OPERATE_SHOW:
        ShowTheme();
        break;
    case OPERATE_HIDE:
        HideTheme();
        break;
    case OPERATE_TURN:
        RotateTheme(pPara.toInt());
        break;
    default:
        break;
    }
}

void LogicThemeManager::slot_Initialize()
{
    IODeviceBase::slot_Initialize();
#ifdef DISPLAY
    if(m_UiContainer)
    {
#ifdef SUPPORT_SCENE
        GraphicsLogo *tmpLogo = VAR_CAST<GraphicsLogo*>(m_UiContainer->m_ComDiagramGroup.value(COM_LOGO));
#elif SUPPORT_WIDGET
        WidgetStatus *tmpLogo = VAR_CAST<WidgetStatus*>(m_UiContainer->m_ComDiagramGroup.value(COM_STATUS));
#endif
        if(tmpLogo)
        {
#ifdef IDE
            connect(m_DevManager, SIGNAL(sIcon(QString)), tmpLogo, SLOT(slot_ShowIcon(QString)));
#elif DISPLAY
            connect(m_DevManager, SIGNAL(sIcon(QString)), tmpLogo, SLOT(slot_ShowIcon(QString)));
            connect(m_DevManager, SIGNAL(sShowIcon(QString)), tmpLogo, SLOT(slot_ShowIcon(QString)));
            connect(m_DevManager, SIGNAL(sHideIcon(QString)), tmpLogo, SLOT(slot_HideIcon(QString)));
#endif
        }

#ifdef SUPPORT_SCENE
        m_Media = VAR_CAST<GraphicsMedia*>(m_UiContainer->m_ComDiagramGroup.value(COM_MEDIA));
#elif SUPPORT_WIDGET
        m_Media = VAR_CAST<WidgetMedia*>(m_UiContainer->m_ComDiagramGroup.value(COM_MEDIA));
#endif
    }

    //>@如果有视频，则需要将视频关闭
    if(mPlayer.isNull())
    {
#ifdef IDE
        mPlayer = VAR_CAST<PhysicalPlayer*>(m_DevManager->m_PHDevList.value(P_Player));
#elif DISPLAY
        mPlayer = VAR_CAST<PhysicalPlayer*>(m_DevManager->m_PHDevList.value(P_Player));
#elif SETTING
        mPlayer = VAR_CAST<PhysicalPlayer*>(m_DevManager->m_PHDevList.value(P_Player));
#endif
    }

    if(m_UiContainer && !m_UiContainer->m_ComDiagramGroup.isEmpty())
    {
#ifdef SUPPORT_SCENE
//        QHash<COM_TYPE, GraphicsComponent*>::const_iterator i = m_UiContainer->m_ComDiagramGroup.constBegin();
//        while (i != m_UiContainer->m_ComDiagramGroup.constEnd())   //>@使用此方法会有一个bug，当只有一个元素时无法执行
//        {
//            GraphicsComponent *tmpComDev = i.value();
//            if(tmpComDev)
//                tmpComDev->slot_Initialize();
//            ++i;
//        }
#elif SUPPORT_WIDGET
        QHash<COM_TYPE, WidgetBase*>::const_iterator i = m_UiContainer->m_ComDiagramGroup.constBegin();
        while (i != m_UiContainer->m_ComDiagramGroup.constEnd())   //>@使用此方法会有一个bug，当只有一个元素时无法执行
        {
            WidgetBase *tmpComDev = i.value();
            if(tmpComDev)
                tmpComDev->slot_Initialize();
            ++i;
        }
#endif
    }
#endif
}

bool LogicThemeManager::SetValue(QString pValue)
{
    if(pValue.isEmpty())
        return true;
    QStringList tmpList = pValue.split(';');
    for(int i=0;i<tmpList.count();i++)
    {
        QStringList tmpCmdList = tmpList.at(i).split(':');
        if(tmpCmdList.count()<2)
            continue;
        QString tmpName = tmpCmdList.at(0);
        QString tmpValue = tmpCmdList.at(1);
        if(!tmpName.compare("del", Qt::CaseInsensitive))
        {
            QDomElement tmpElement = m_ParaElement.firstChildElement("ActiveTheme");
            QStringList tmpThemeList = tmpElement.attribute("List").split(';');
            if(tmpThemeList.isEmpty())
                return false;
            int index = tmpThemeList.indexOf(tmpValue);
            if(index < 0)
                return false;
            if(!tmpValue.compare(mActiveTheme, Qt::CaseInsensitive))  //>@如果删除当前主题，需要将另一主题设置为活动主题
            {
                if(tmpThemeList.count() > 1)
                {
                    index++;
                    if(index >= tmpThemeList.count())
                        index = 0;
                    mActiveTheme = tmpThemeList.at(index);
#ifdef DISPLAY
                    QString tmpThemePath;
                    if(!mActiveTheme.isEmpty())
                        tmpThemePath = mThemesPath + mActiveTheme + QString("/") + D_UINAME;
                    else
                        tmpThemePath = D_FSRCPATH + D_THEMEDIR + D_DEFAULTTHEMEDIR + D_UINAME;
                    m_UiContainer->LoadUI(tmpThemePath);
                    //>@显示Theme
                    ShowTheme();
#endif
                }
            }
            tmpThemeList.removeAt(index);
            DelFolder(mThemesPath + tmpValue, true);
        }
    }
    return true;
}

void LogicThemeManager::slot_ReportOperate(AREA_OPERATE pOperate, QVariant pPara)
{
    switch(pOperate)
    {
        case OPERATE_PROGRESS:
        {
            QStringList tmpList = pPara.toString().split(':');
            if(tmpList.count() == 2)
                emit sProgress(tmpList.at(0), tmpList.at(1).toInt());
            break;
        }
        default:
            break;
    }
}

#ifdef DISPLAY
#ifdef SUPPORT_SCENE
void LogicThemeManager::InitTheme(UiView* pView, uiManager *pManager)
{
    m_UiContainer = new UiScene(pView, this);
    m_UiContainer->InitLayout(mThemeLayout[0]);
    m_UiContainer->Initialize(pManager);
}
#endif
#endif

void LogicThemeManager::InitTheme()
{
    ReleaseTheme();
#ifdef DISPLAY
#ifdef SUPPORT_SCENE
    m_UiContainer = new UiScene(0, this);
#elif SUPPORT_WIDGET
    m_UiContainer = new UiWidget(this);
#elif SUPPORT_PLUGIN
    m_UiContainer = new UiPlugin(0, this);
#endif
    m_UiContainer->InitLayout(mThemeLayout[0]);
#endif
    QString tmpThemePath;
    if(!mActiveTheme.isEmpty())
        tmpThemePath = mThemesPath + mActiveTheme + QString("/") + D_UINAME;
    else
        tmpThemePath = D_FSRCPATH + D_THEMEDIR + D_DEFAULTTHEMEDIR + D_UINAME;
    IDE_TRACE_STR(tmpThemePath);
#ifdef DISPLAY
    m_UiContainer->Initialize(tmpThemePath);
    IDE_TRACE();
#endif
}

void LogicThemeManager::ReleaseTheme()
{
#ifdef DISPLAY
    if(!m_UiContainer)
        return;
    m_UiContainer->deleteLater();
    m_UiContainer = 0;
#endif
}

void LogicThemeManager::ClearTheme()
{
#ifdef DISPLAY
    if(m_UiContainer)
        m_UiContainer->OperateUI(OPERATE_CLEAR);
#endif
}

void LogicThemeManager::HideTheme()
{
#ifdef DISPLAY
    if(!m_UiContainer || m_UiContainer->m_MainForm.isNull())
        return;

    //>@断开所有的驱动信号
    m_EventMap->SetEnable(false);

    //>@如果有音视频，则需要关闭音视频
    if(!m_Media.isNull())
    {
        m_Media.data()->Abort(AO_VIDEO);
    }

#if (defined(SUPPORT_SCENE) || defined(SUPPORT_WIDGET))
    #ifdef DISPLAY
        m_UiContainer->m_MainForm.data()->hide();
    #endif
#elif SUPPORT_PLUGIN
    if(!(m_UiPlugin && m_UiPlugin->m_PluginInterface))
        return;
    MainForm *tmpMainWidget = m_UiPlugin->m_PluginInterface->Widget();
    if(tmpMainWidget)
    {
    #ifdef DISPLAY
        tmpMainWidget->hide();
    #endif
    }
#endif
#endif
}

void LogicThemeManager::ShowTheme()
{
#ifdef DISPLAY
    if(!m_UiContainer)
        return;

    m_DevManager->slot_Initialized(); //>@注意，一定要先发送初试命令，再显示，否则Widget方案中的按钮滑动会超出范围。

#if (defined(SUPPORT_SCENE) || defined(SUPPORT_WIDGET))
    disconnect(m_DevManager, SIGNAL(sProgress(QString, quint32)), 0, 0);
#ifdef DISPLAY
#if 1
    QGridLayout *tmpLayout = VAR_CAST<QGridLayout*>(m_DevManager->layout());
    if(tmpLayout && !m_UiContainer->m_MainForm.isNull())
    {
        IDE_TRACE();
        m_UiContainer->m_MainForm.data()->show();
        QLayoutItem *tmpItem = tmpLayout->itemAtPosition(mLocation.mRow, mLocation.mColumn);
        if(!(tmpItem && tmpItem->widget() == (QWidget*)(m_UiContainer->m_MainForm.data())))
        {
            tmpLayout->addWidget(m_UiContainer->m_MainForm.data(), mLocation.mRow, mLocation.mColumn);
        }
    }
#else
    m_UiContainer->m_MainForm.data()->setGeometry(m_DevManager->rect());
    m_UiContainer->m_MainForm.data()->show();
#endif
    connect(m_DevManager,SIGNAL(sProgress(QString,quint32)), m_UiContainer, SLOT(slot_Progress(QString,quint32)));
    connect(m_DevManager,SIGNAL(sProgress(QString,quint32,quint32)), m_UiContainer, SLOT(slot_Progress(QString,quint32,quint32)));

    //>@驱动信号
    m_EventMap->SetEnable(true);

    m_DevManager->slot_UiShowed();

#endif
#elif SUPPORT_PLUGIN
    if(!(m_UiPlugin && m_UiPlugin->m_PluginInterface))
        return;
    MainForm *tmpMainWidget = m_UiPlugin->m_PluginInterface->Widget();
    if(tmpMainWidget)
    {
    #ifdef DISPLAY
        QGridLayout *tmpLayout = VAR_CAST<QGridLayout*>(m_DevManager->layout());
        if(tmpLayout)
            tmpLayout->addWidget(tmpMainWidget, mLocation.mRow, mLocation.mColumn);
    #endif
    }
#endif
#endif
}

void LogicThemeManager::RotateTheme(int pNewRotate)
{
#ifdef DISPLAY
    if(!m_UiContainer)
        return;
    m_UiContainer->Rotate(pNewRotate);
#endif
}

