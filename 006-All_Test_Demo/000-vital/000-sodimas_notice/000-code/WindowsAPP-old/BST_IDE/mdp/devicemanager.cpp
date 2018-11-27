#include "devicemanager.h"
#include "logicthememanager.h"

DeviceManager::DeviceManager(QWidget *parent) :
    QWidget(parent
#ifdef LINUX
            ,Qt::FramelessWindowHint
#endif
            ), DevManager()
{
    _Initialize();
}

DeviceManager::DeviceManager(QRect pRect, QWidget *parent) :
    QWidget(parent
#ifdef LINUX
            ,Qt::FramelessWindowHint
#endif
            ), DevManager()
{
    _Initialize();

    setGeometry(pRect);

    //>@建立界面管理grid
    QGridLayout *tmpGridLayout = new QGridLayout;
    tmpGridLayout->setSpacing(0);
    tmpGridLayout->setMargin(0);
    setLayout(tmpGridLayout);
    //>@延时开启设备加载线程
    QTimer::singleShot(1, this, SLOT(slot_InitDevices()));
}

DeviceManager::~DeviceManager()
{
    CloseBootMdp();
}

void DeviceManager::_Initialize()
{
#if 1
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
#endif

#ifndef WINDOWS
    setShutDownSignal(SIGINT);   // shut down on ctrl-c
    setShutDownSignal(SIGTERM);   // shut down on killall
#endif

    installEventFilter(this);

    mMdpManager = 0;
    mBootLogo = new BootLogo;
#ifdef N329
    mBootLogo->m_TransparentPix = m_TranparentPix;
#endif
    mPaintEnable = false;

    //>@初始化参数
    m_PHDevList.clear();
    m_LGDevList.clear();

    m_DevSetBusy = false;
}

void DeviceManager::InitEnvirHash()
{
    m_EnvirHash.clear();
    QStringList environment = QProcess::systemEnvironment();
    QString str;
    foreach(str,environment)
    {
        QStringList tmpEnvirList = str.split('=');
        if(tmpEnvirList.count()==2)
        {
            m_EnvirHash.insert(tmpEnvirList.at(0), tmpEnvirList.at(1));
        }
    }
}

void DeviceManager::slot_Info(QString pMsg)
{
    if(!mBootLogo)
        return;
    if(mBootLogo && mBootLogo->ShowMessage(pMsg))
    {
        update();
        QtSleep(D_PROGRESSDELAYTIME);
    }
}

void DeviceManager::slot_Progress(quint32 pValue)
{
    if(!mBootLogo)
        return;
    if(mBootLogo && mBootLogo->SetProgress(pValue))
    {
        update();
        QtSleep(D_PROGRESSDELAYTIME);
    }
    if(mBootLogo->GetProgress() >= 100)
    {
        mPaintEnable = false;
        update();
    }
}

void DeviceManager::slot_Progress(QString pMsg, quint32 pValue)
{
    if(!mBootLogo)
        return;

    if(mBootLogo->SetProgress(pValue) && mBootLogo->ShowMessage(pMsg))
    {
        update();
        QtSleep(D_PROGRESSDELAYTIME);
    }
    int tmpValue = mBootLogo->GetProgress();
    if(tmpValue == 100 || tmpValue == 0)
    {
        mPaintEnable = false;
        update();
    }
}

bool DeviceManager::SetCmd(QString pCmd)
{
    xmlParse *tmpSetParse = new xmlParse;
    tmpSetParse->SetForcesave(false);
    if(tmpSetParse->setContent(pCmd))
    {
        QDomElement tmpElement = tmpSetParse->documentElement();
        if(!tmpElement.isNull())
        {
            QString tmpCommonType = tmpElement.tagName();
            if(!tmpCommonType.compare("Physical"))
            {
                QDomNodeList tmpPHDevList = tmpElement.childNodes();
                for(int i=0;i<tmpPHDevList.count();i++)
                {
                    QDomElement tmpPHElement = tmpPHDevList.at(i).toElement();
                    if(tmpPHElement.isNull())
                        continue;
                    PhysicalDevice* tmpPHDev = m_PHDevList.value(GetPHType(tmpPHElement.tagName()));
                    if(!tmpPHDev)
                        continue;
                    tmpPHDev->SetPara(tmpPHElement, tmpSetParse);
                }
            }
            else if(!tmpCommonType.compare("Logic"))
            {
                QDomNodeList tmpLGDevList = tmpElement.childNodes();
                for(int i=0;i<tmpLGDevList.count();i++)
                {
                    QDomElement tmpLGElement = tmpLGDevList.at(i).toElement();
                    if(tmpLGElement.isNull())
                        continue;
                    LogicDevice* tmpLGDev = m_LGDevList.value(GetLGType(tmpLGElement.tagName()));
                    if(!tmpLGDev)
                        continue;
                    tmpLGDev->SetPara(tmpLGElement, tmpSetParse);
                }
            }
            else if(!tmpCommonType.compare("Theme"))
            {
                LogicThemeManager* tmpThemeManager = VAR_CAST<LogicThemeManager*>(m_LGDevList.value(L_ThemeManager));
                if(!tmpThemeManager)
                    return false;
                QDomNodeList tmpComList = tmpElement.childNodes();
                for(int i=0;i<tmpComList.count();i++)
                {
                    QDomElement tmpComElement = tmpComList.at(i).toElement();
                    if(tmpComElement.isNull())
                        continue;
                    COM_TYPE tmpType = getComponetType(tmpComElement.tagName());
#ifdef SUPPORT_SCENE
                    GraphicsOperate* tmpCom = tmpThemeManager->m_UiContainer->m_ComDiagramGroup.value(tmpType);
#elif SUPPORT_WIDGET
                    WidgetBase* tmpCom = tmpThemeManager->m_UiContainer->m_ComDiagramGroup.value(tmpType);
#endif
                    if(!tmpCom)
                        continue;
                    tmpCom->SetPara(tmpComElement, tmpSetParse);
                }
            }
        }
    }
    tmpSetParse->deleteLater();
    return true;
}

bool DeviceManager::eventFilter(QObject *o, QEvent *e)
{
    if(o == this)
    {
        if(e->type() == QEvent::MouseButtonPress || e->type() == QEvent::KeyPress)  //>@唤醒液晶屏
        {
            IDE_TRACE_INT(e->type());
            if(e->type() == QEvent::KeyPress)
            {
                QKeyEvent *tmpEvent = static_cast<QKeyEvent*>(e);
                if(tmpEvent)
                {
                    IDE_TRACE_INT(tmpEvent->key());
#if 0
                    if(tmpEvent->key() == Qt::Key_Up || tmpEvent->key() == Qt::Key_F1)
                    {
                        emit sSetMenu();
                    }
#else
                    if(tmpEvent->key() == Qt::Key_F1)
                    {
                        PhysicalPlayer *tmpPlayer = VAR_CAST<PhysicalPlayer*>(m_PHDevList.value(P_Player));
                        if(tmpPlayer)
                        {
                            int tmpVol = 100;
                            bool flag = false;
                            for(int i=0;i<AOTRACK_MAXNUM;i++)
                            {
                                if(tmpPlayer->mPlayerList[i])
                                {
                                    tmpVol = tmpPlayer->mPlayerList[i]->m_Volume[0];
                                    if(tmpVol <= 90)
                                        tmpVol += 10;
                                    else
                                        tmpVol = 100;
                                    flag = true;
                                    break;
                                }
                            }
                            if(flag)
                                SetCmd(QString("<Physical><Player><LiftFlrVol>%1</LiftFlrVol><LiftArvVol>%1</LiftArvVol><BeepVol>%1</BeepVol><VideoVol>%1</VideoVol></Player></Physical>").arg(tmpVol));
                        }
                    }
                    else if(tmpEvent->key() == Qt::Key_F2)
                    {
                        PhysicalPlayer *tmpPlayer = VAR_CAST<PhysicalPlayer*>(m_PHDevList.value(P_Player));
                        if(tmpPlayer)
                        {
                            int tmpVol = 100;
                            bool flag = false;
                            for(int i=0;i<AOTRACK_MAXNUM;i++)
                            {
                                if(tmpPlayer->mPlayerList[i])
                                {
                                    tmpVol = tmpPlayer->mPlayerList[i]->m_Volume[0];
                                    if(tmpVol >= 10)
                                        tmpVol -= 10;
                                    else
                                        tmpVol = 0;
                                    flag = true;
                                    break;
                                }
                            }
                            if(flag)
                                SetCmd(QString("<Physical><Player><LiftFlrVol>%1</LiftFlrVol><LiftArvVol>%1</LiftArvVol><BeepVol>%1</BeepVol><VideoVol>%1</VideoVol></Player></Physical>").arg(tmpVol));
                        }
                    }
                    /*
                    else if(tmpEvent->key() == Qt::Key_F3)
                    {
                        PhysicalLCD *tmpLCD = VAR_CAST<PhysicalLCD*>(m_PHDevList.value(P_LCD));
                        if(tmpLCD)
                        {
                            int tmpLum = tmpLCD->mLuminance.mNum;
                            if(tmpLum < 95)
                                tmpLum += 5;
                            else
                                tmpLum = 100;
                            tmpLCD->SetLuminance(tmpLum);
                        }
                    }
                    else if(tmpEvent->key() == Qt::Key_F4)
                    {
                        PhysicalLCD *tmpLCD = VAR_CAST<PhysicalLCD*>(m_PHDevList.value(P_LCD));
                        if(tmpLCD)
                        {
                            int tmpLum = tmpLCD->mLuminance.mNum;
                            if(tmpLum > 5)
                                tmpLum -= 5;
                            else
                                tmpLum = 0;
                            tmpLCD->SetLuminance(tmpLum);
                        }
                    }
                    */
#endif
                }
            }
            PhysicalLCD *tmpLCD = VAR_CAST<PhysicalLCD*>(m_PHDevList.value(P_LCD));
            if(tmpLCD)
            {
                tmpLCD->slot_FeedLCD();
            }
        }
    }
    return QWidget::eventFilter(o,e);
}

void DeviceManager::paintEvent(QPaintEvent *e)
{
    if(!mBootLogo)
        return;
    if(!mPaintEnable)
    {
        if(!mBootLogo->m_BgPixmap.isNull())
        {
            QPainter p(this);
            p.setRenderHint(QPainter::SmoothPixmapTransform, true);
            p.setRenderHint(QPainter::Antialiasing);
            p.drawPixmap(rect(), mBootLogo->m_BgPixmap);
            if(!mBootLogo->mRencodePix.isNull() && !mBootLogo->mRencodeRect.isEmpty())
            {
                p.drawPixmap(mBootLogo->mRencodeRect, mBootLogo->mRencodePix);
            }
            p.end();
        }
    }
    else
    {
        QPainter p(this);
        p.setRenderHint(QPainter::SmoothPixmapTransform, true);
        p.setRenderHint(QPainter::Antialiasing);
        if(!mBootLogo->m_BgPixmap.isNull())
            p.drawPixmap(rect(), mBootLogo->m_BgPixmap);
        //>@绘制进度值
        int tmpValue = mBootLogo->GetProgress();
        QString tmpValueString = QString("%1").arg(tmpValue);
        p.save();
        QRect tmpPrgRect = QRect(mBootLogo->m_PrgRect.topLeft()+mBootLogo->m_PrgInfo.mPrgValueRect.topLeft(),
                                 mBootLogo->m_PrgInfo.mPrgValueRect.size());
        if(!mBootLogo->m_PrgPixmap.isNull())
            p.drawPixmap(mBootLogo->m_PrgRect, mBootLogo->m_PrgPixmap);
        QLinearGradient pgTopGradient(tmpPrgRect.topLeft(), tmpPrgRect.bottomLeft());
        pgTopGradient.setColorAt(0, QColor(254,145,29,255));
        pgTopGradient.setColorAt(0.5, QColor(238,99,0,255));
        pgTopGradient.setColorAt(1, QColor(254,145,29,255));//设置渐变的颜色和路径比例
        p.setPen(Qt::NoPen);
        p.setBrush(QBrush(pgTopGradient));
        int len = tmpPrgRect.width()*(100-tmpValue)/100.0;
        p.drawRoundedRect(tmpPrgRect.adjusted(0, 0, -len, 0), 15, 15);
        p.restore();
        p.save();
        p.setFont(mBootLogo->m_PrgInfo.mPrgTextFont);
        p.setPen(Qt::white);
        p.drawText(tmpPrgRect, Qt::AlignCenter, tmpValueString);
        p.restore();
        //>@绘制文字
        if(mBootLogo->m_ShowLogo)
        {
            if(!mBootLogo->m_LogoPixmap.isNull())
                p.drawPixmap(mBootLogo->m_LogoRect, mBootLogo->m_LogoPixmap);
        }
        else
        {
            if(!mBootLogo->m_MsgPixmap.isNull())
                p.drawPixmap(mBootLogo->m_MsgRect, mBootLogo->m_MsgPixmap);
            int count = mBootLogo->GetMsgCount();
            if(count)
            {
                p.save();
                p.setFont(mBootLogo->m_MsgInfo.mMsgTextFont);
                p.setPen(Qt::darkGreen);
                int tmpTextHeight = (qreal)mBootLogo->m_MsgInfo.mMsgTextRect.height()/(qreal)mBootLogo->m_MsgInfo.mMaxMsgRow;
                for(int i=0;i<count;i++)
                {
                    p.drawText(QRect(mBootLogo->m_MsgInfo.mMsgTextRect.left(),
                                     mBootLogo->m_MsgInfo.mMsgTextRect.top()+tmpTextHeight*i,
                                     mBootLogo->m_MsgInfo.mMsgTextRect.width(),
                                     tmpTextHeight),
                               mBootLogo->m_MsgInfo.mMsgQueue.at(i));
                }
                p.restore();
            }
        }
        if(!mBootLogo->m_VerInfo.mContent.isEmpty())
        {
            p.save();
            p.setFont(mBootLogo->m_VerInfo.mFont);
            p.setPen(mBootLogo->m_VerInfo.mColor);
            p.drawText(mBootLogo->m_VerInfo.mRect, Qt::AlignCenter, mBootLogo->m_VerInfo.mContent);
            p.restore();
        }
        if(!mBootLogo->mRencodePix.isNull() && !mBootLogo->mRencodeRect.isEmpty())
        {
            p.drawPixmap(mBootLogo->mRencodeRect, mBootLogo->mRencodePix);
        }
        p.end();
    }
    QWidget::paintEvent(e);
}

#ifndef WINDOWS
void handleShutDownSignal(int signalId)
{
#ifdef DISPLAY
    GlobalPara.m_DeviceManager->PrepareRelease();
#endif
    QApplication::exit(0);
}
void setShutDownSignal( int signalId )
{
    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handleShutDownSignal;
    if(sigaction(signalId, &sa, NULL) == -1)
    {
        exit(1);
    }
}
#endif

//>@初始化设备列表
void DeviceManager::slot_InitDevices()
{
#ifdef IMX6X
    ReMount(QString("/usr/bst"), 2);
#endif

    connect(this,SIGNAL(sProgress(QString,quint32)), this, SLOT(slot_Progress(QString,quint32)));
    mPaintEnable = false;
    QString tmpMdpFile = D_FSRCPATH + D_BOOTDIR + D_BOOTFILE;
    if(!OpenBootMdp(tmpMdpFile))
    {
        emit sProgress(tr("Open Boot File Failed, Will Use The Back File!"), 100);
        QString tmpDefaultMdpFile = D_FSRCPATH + D_BOOTDIR + D_BKBOOTFILE;
        if(!CopyFile(tmpDefaultMdpFile, tmpMdpFile))
            tmpMdpFile = tmpDefaultMdpFile;
        if(!OpenBootMdp(tmpMdpFile))
        {
            emit sProgress(tr("Open Default Boot File Failed, System Exit!"), 100);
            return;
        }
    }
    IDE_TRACE_STR(tmpMdpFile);

    mRencodePix = QPixmap(D_RENCODE);

    //>@初始化显示Splash界面
    mBootLogo->SetRencode(mRencodePix);
    mBootLogo->ShowLogoOrMsg(!mMdpManager->CanShowLog());
    mBootLogo->SetRect(rect());
    mBootLogo->SetPattern(mMdpManager->GetPattern());
    //>@获取版本号
    QString tmpVer = m_SYSVersion = GetSysVersion();
    m_APPVersion = GetAppVersion(qApp->applicationFilePath());
    if(!tmpVer.endsWith("\n"))
        tmpVer.append("\n");
    QString tmpVersion = QString("SYS:%1 APP:%2").arg(tmpVer).arg(m_APPVersion);
    mBootLogo->SetVersion(tmpVersion);
    mBootLogo->SetFile(D_FSRCPATH + D_BOOTDIR + QString("bg.png"));
    mPaintEnable = true;

    update();
    QtSleep(10);

    //>@初始化环境变量
    emit sProgress(QString("Start Load Environment Variable..."), 5);
    InitEnvirHash();

    //>@初始化中间件
    emit sProgress(QString("Start Load Middleware..."), 7);
    QList<MW_Type> tmpMWList = mMdpManager->m_MWElementGroup.keys();
    qSort(tmpMWList.begin(), tmpMWList.end());
    int tmpPgDiff = 15.0 / tmpMWList.count();
    for(int i=0;i<tmpMWList.count();i++)
    {
        MW_Type tmpType = tmpMWList.at(i);
        if(tmpType == P_INVALID)
            continue;
        QDomElement tmpElement = mMdpManager->m_MWElementGroup.value(tmpType);
        if(tmpElement.isNull())
            continue;
        emit sProgress(QString("Loading %1 ...").arg(tmpElement.tagName()), tmpPgDiff*i);
        AddMWDevice(tmpType, tmpElement);
        IDE_TRACE_INT(tmpType);
    }
    //>@初始化物理设备
    emit sProgress(QString("Start Load Physical Devices..."), 20);
    QList<PH_Type> tmpPHList = mMdpManager->m_PHElementGroup.keys();
    qSort(tmpPHList.begin(), tmpPHList.end());
    tmpPgDiff = 15.0 / tmpPHList.count();
    for(int i=0;i<tmpPHList.count();i++)
    {
        PH_Type tmpType = tmpPHList.at(i);
        if(tmpType == P_INVALID)
            continue;
        QDomElement tmpElement = mMdpManager->m_PHElementGroup.value(tmpType);
        if(tmpElement.isNull())
            continue;
        emit sProgress(QString("Loading %1 ...").arg(tmpElement.tagName()), 20+tmpPgDiff*i);
        AddPHDevice(tmpType, tmpElement);
        IDE_TRACE_INT(tmpType);
    }

    //>@初始化逻辑设备
    emit sProgress(QString("Start Load Logic Devices..."), 35);
    QList<LG_Type> tmpLGList = mMdpManager->m_LGElementGroup.keys();
    qSort(tmpLGList.begin(), tmpLGList.end());
    tmpPgDiff = 15.0 / tmpLGList.count();
    for(int i=0;i<tmpLGList.count();i++)
    {
        LG_Type tmpType = tmpLGList.at(i);
        if(tmpType == L_INVALID)
            continue;
        QDomElement tmpElement = mMdpManager->m_LGElementGroup.value(tmpType);
        if(tmpElement.isNull())
            continue;
        emit sProgress(QString("Loading %1 ...").arg(tmpElement.tagName()), 35+tmpPgDiff*i);
        AddLGDevice(tmpType, tmpElement);
        IDE_TRACE_INT(tmpType);
    }

    //>@开启线程循环。
    slot_StartMonitor();
    QtSleep(50);

    //>@生成二维码
#ifdef IMX6X
    if(isDevRW(QString("/usr/bst")))
    {
        RenCode tmpCode;
        QString tmpSSID = mDevCode;
        QString tmpIP;
        quint16 tmpPort = 0;
        PhysicalLANC *tmpLanc = VAR_CAST<PhysicalLANC*>(m_PHDevList.value(P_LANC));
        if(tmpLanc)
        {
            tmpIP = tmpLanc->mIP.toString();
            tmpPort = tmpLanc->mTCPSPort.mNum;
        }
        QString tmpName = mDevCode;
        QString tmpRenCode = QString("SSID:%1;IP:%2;Port:%3;Location:%4;Passwd:%5;").arg(tmpSSID).arg(tmpIP).arg(tmpPort).arg(tmpName).arg(mPasswd);
        QByteArray tmpBase64 = tmpRenCode.toLatin1().toBase64();
        tmpCode.Generate(tmpBase64, D_RENCODE);

        IDE_TRACE_STR(QString(tmpBase64));

        mRencodePix = QPixmap(D_RENCODE);
        mBootLogo->SetRencode(mRencodePix);
    }
#endif
}

void DeviceManager::slot_StartMonitor()
{
    //>@创建设备监控线程
    int tmpValue = mMdpManager->GetMonitor();
#ifdef N329
    if(tmpValue < 100)  //>@最小为100.
        tmpValue = 100;
#else
    if(tmpValue < 10)  //>@最小为10.
        tmpValue = 10;
#endif
    SetMonitor((quint32)tmpValue);
}

void DeviceManager::ReleaseDevices()
{
    QList<PH_Type> PHTypeList = m_PHDevList.keys();
    for(int i=0;i<PHTypeList.count();i++)
    {
        DelPHDevice(PHTypeList.at(i));
    }
    QList<LG_Type> LGTypeList = m_LGDevList.keys();
    for(int i=0;i<LGTypeList.count();i++)
    {
        DelLGDevice(LGTypeList.at(i));
    }
}

//>@删除设备之前需要执行。
void DeviceManager::PrepareRelease()
{
    if(m_DevMonitor)
        m_DevMonitor->ForceQuit();
    QtSleep(10);
    PhysicalDevice *tmpPHDev = 0;
    QList<PhysicalDevice*> tmpPHDevs = m_PHDevList.values();
    foreach(tmpPHDev, tmpPHDevs)
    {
        if(!tmpPHDev)
            continue;
        IDE_TRACE_STR(tmpPHDev->mDevName);
        tmpPHDev->PrepareRelease();
    }
}

void DeviceManager::slot_UiShowed()
{
    //>@初始化各个模块
    if(!m_PHDevList.isEmpty())
    {
        QList<PH_Type> tmpList = m_PHDevList.keys();
        qSort(tmpList.begin(), tmpList.end());
        for(int i=0;i<tmpList.count();i++)
        {
            PhysicalDevice *tmpPHDev = m_PHDevList.value(tmpList.at(i));
            if(tmpPHDev)
                tmpPHDev->slot_UiShowed();
        }
    }
    if(!m_LGDevList.isEmpty())
    {
        QHash<LG_Type, LogicDevice*>::const_iterator i = m_LGDevList.constBegin();
        while(i != m_LGDevList.constEnd())   //>@使用此方法会有一个bug，当只有一个元素时无法执行
        {
            LogicDevice *tmpLGDev = i.value();
            if(tmpLGDev)
                tmpLGDev->slot_UiShowed();
            ++i;
        }
    }
}

void DeviceManager::slot_Initialized()
{
    //>@初始化各个模块
    if(!m_PHDevList.isEmpty())
    {
        QList<PH_Type> tmpList = m_PHDevList.keys();
        qSort(tmpList.begin(), tmpList.end());
        for(int i=0;i<tmpList.count();i++)
        {
            PhysicalDevice *tmpPHDev = m_PHDevList.value(tmpList.at(i));
            if(tmpPHDev)
                tmpPHDev->slot_Initialize();
        }
    }
    if(!m_LGDevList.isEmpty())
    {
        QHash<LG_Type, LogicDevice*>::const_iterator i = m_LGDevList.constBegin();
        while(i != m_LGDevList.constEnd())   //>@使用此方法会有一个bug，当只有一个元素时无法执行
        {
            LogicDevice *tmpLGDev = i.value();
            if(tmpLGDev)
                tmpLGDev->slot_Initialize();
            ++i;
        }
    }

#ifdef DEBUG
    LogicThemeManager *tmpManager = VAR_CAST<LogicThemeManager *>(m_LGDevList.value(L_ThemeManager));
    if(tmpManager)
    {
        setGeometry(tmpManager->m_UiContainer->GetGeometory().toRect());
    }
#endif
}

QString DeviceManager::GetVersion(QString pName)
{
    LogicContentManager *tmpContentManager = VAR_CAST<LogicContentManager*>(m_LGDevList.value(L_ContentManager));
    if(tmpContentManager)
    {
        if(!pName.compare("Application", Qt::CaseInsensitive))
            return tmpContentManager->mApplication;
        else if(!pName.compare("System", Qt::CaseInsensitive))
            return tmpContentManager->mSystem;
    }
    return QString();
}

quint32 DeviceManager::Monitor(quint32 pCount)
{
#if 0
    QList<PhysicalDevice*> tmpPHDevList = m_PHDevList.values();
    PhysicalDevice *tmpPHDev = 0;
    foreach(tmpPHDev, tmpPHDevList)
    {
        if(tmpPHDev)
            tmpPHDev->Monitor(pCount);
    }
#else
    if(!m_PHDevList.isEmpty())
    {
        QHash<PH_Type, PhysicalDevice*>::const_iterator i = m_PHDevList.constBegin();
        while (i != m_PHDevList.constEnd())   //>@使用此方法会有一个bug，当只有一个元素时无法执行
        {
            PhysicalDevice *tmpPHDev = i.value();
            if(tmpPHDev)
                tmpPHDev->Monitor(pCount);
            ++i;
        }
    }
#endif
#if 0
    QList<LogicDevice*> tmpLGDevList = m_LGDevList.values();
    LogicDevice *tmpLGDev = 0;
    foreach(tmpLGDev, tmpLGDevList)
    {
        if(tmpLGDev)
            tmpLGDev->Monitor(pCount);
    }
#endif
    return 0;
}

void DeviceManager::SetDevicesEnable(bool pFlag, E_DEVMODE pDevMode)
{
    if((m_DevFlag == pFlag) && (m_DevMode == pDevMode))
        return;

//    if(m_DevSetBusy)
//    {
//        IDE_DEBUG("Set devices Busy...");
//        return;
//    }
//    m_DevSetBusy = true;

    m_DevFlag = pFlag;
    m_DevMode = pDevMode;
#if 0
    QList<PhysicalDevice*> tmpPHDevList = m_PHDevList.values();
    PhysicalDevice *tmpPHDev = 0;
    foreach(tmpPHDev, tmpPHDevList)
    {
        if(tmpPHDev)
            tmpPHDev->SetEnable(pFlag, pDevMode);
    }
    QList<LogicDevice*> tmpLGDevList = m_LGDevList.values();
    LogicDevice *tmpLGDev = 0;
    foreach(tmpLGDev, tmpLGDevList)
    {
        if(tmpLGDev)
            tmpLGDev->SetEnable(pFlag, pDevMode);
    }
#else
    if(!m_PHDevList.isEmpty())
    {
        QHash<PH_Type, PhysicalDevice*>::const_iterator i = m_PHDevList.constBegin();
        while (i != m_PHDevList.constEnd())   //>@使用此方法会有一个bug，当只有一个元素时无法执行
        {
            PhysicalDevice *tmpDev = i.value();
            if(tmpDev)
                tmpDev->SetEnable(pFlag, pDevMode);
            ++i;
            //QtSleep(1);
        }
    }
    if(!m_LGDevList.isEmpty())
    {
        QHash<LG_Type, LogicDevice*>::const_iterator i = m_LGDevList.constBegin();
        while (i != m_LGDevList.constEnd())   //>@使用此方法会有一个bug，当只有一个元素时无法执行
        {
            LogicDevice *tmpDev = i.value();
            if(tmpDev)
                tmpDev->SetEnable(pFlag, pDevMode);
            ++i;
            //QtSleep(1);
        }
    }
#endif

    //m_DevSetBusy = false;
}

bool DeviceManager::AddMWDevice(MW_Type pMWType, QDomElement pMWTypeElement, bool pCover)
{
    if(pMWType == M_INVALID || pMWTypeElement.isNull())
        return false;
    //>@查询此物理设备是否已经存在
    if(m_MWDevList.contains(pMWType))
    {
        if(pCover)
            DelMWDevice(pMWType);
        else
            return false;
    }
    MiddlewareDevice *tmpDev = new MiddlewareDevice(this);
    if(!tmpDev)
        return false;
    m_MWDevList.insert(pMWType, tmpDev);
    tmpDev->InitDevice(pMWType, pMWTypeElement);
}

bool DeviceManager::DelMWDevice(MW_Type pMWType)
{
    MiddlewareDevice *tmpDev = m_MWDevList.take(pMWType);
    if(tmpDev == 0)
        return true;
    tmpDev->deleteLater();
    return true;
}

bool DeviceManager::AddPHDevice(PH_Type pPHType, QDomElement pPHTypeElement, bool pCover)
{
    if(pPHType == P_INVALID || pPHTypeElement.isNull())
        return false;
    //>@查询此物理设备是否已经存在
    if(m_PHDevList.contains(pPHType))
    {
        if(pCover)
            DelPHDevice(pPHType);
        else
            return false;
    }
    PhysicalDevice *tmpPHDev = 0;
    switch(pPHType)
    {
        case P_CPU:
            tmpPHDev = new PhysicalCPU(this);
            break;
        case P_MEM:
            tmpPHDev = new PhysicalMEM(this);
            break;
        case P_DISK:
            tmpPHDev = new PhysicalDisk(this);
            break;
        case P_RTC:
            tmpPHDev = new PhysicalRTC(this);
            break;
        case P_MDISK:
            tmpPHDev = new PhysicalMobileDisk(this);
            break;
        case P_LCD:
            tmpPHDev = new PhysicalLCD(this);
            break;
        case P_Touch:
            tmpPHDev = new PhysicalTouch(this);
            break;
        case P_LED:
            tmpPHDev = new PhysicalLED(this);
            break;
        case P_WD:
            tmpPHDev = new PhysicalWD(this);
            break;
        case P_DIAL:
            tmpPHDev = new PhysicalDial(this);
            break;
        case P_LANC:
            tmpPHDev = new PhysicalLANC(this);
            break;
        case P_LANR:
            tmpPHDev = new PhysicalLANR(this);
            break;
        case P_Player:
            tmpPHDev = new PhysicalPlayer(this);
            break;
        case P_COM:
            tmpPHDev = new PhysicalCOM(this);
            break;
        case P_SAF:
            tmpPHDev = new PhysicalSAF(this);
            break;
        default:return false;
    }
    if(!tmpPHDev)
    {
        IDE_TRACE_INT(pPHType);
        return false;
    }
    m_PHDevList.insert(pPHType, tmpPHDev);
    tmpPHDev->InitDevice(pPHType, pPHTypeElement);
    return true;
}

bool DeviceManager::DelPHDevice(PH_Type pPHType)
{
    PhysicalDevice *tmpPHDev = m_PHDevList.take(pPHType);
    if(tmpPHDev == 0)
        return true;
    tmpPHDev->deleteLater();
    return true;
}

bool DeviceManager::AddLGDevice(LG_Type pLGType, QDomElement pLGTypeElement, bool pCover)
{
    if(pLGType == L_INVALID || pLGTypeElement.isNull())
        return false;
    //>@查询此物理设备是否已经存在
    if(m_LGDevList.contains(pLGType))
    {
        if(pCover)
            DelLGDevice(pLGType);
        else
            return false;
    }
    //>@
    LogicDevice *tmpLGDev = 0;
    switch(pLGType)
    {
        case L_Monitor:
            tmpLGDev = new LogicMonitor(this);
            break;
        case L_CardReader:
            tmpLGDev = new LogicCardReader(this);
            break;
        case L_NFCP2P:
            tmpLGDev = new LogicNFCP2P(this);
            break;
        case L_ThemeManager:
        {
            LogicThemeManager *tmpThemeManager = new LogicThemeManager(this);
            tmpLGDev = tmpThemeManager;
            break;
        }
        case L_ContentManager:
            tmpLGDev = new LogicContentManager(this);
            break;
        default:return false;
    }
    if(!tmpLGDev)
        return false;
    m_LGDevList.insert(pLGType, tmpLGDev);
    tmpLGDev->InitDevice(pLGType, pLGTypeElement);
    return true;
}

bool DeviceManager::DelLGDevice(LG_Type pLGType)
{
    LogicDevice* tmpLGDev = m_LGDevList.take(pLGType);
    if(tmpLGDev == 0)
        return true;
    //>@删除物理设备
    tmpLGDev->deleteLater();
    return true;
}

PhysicalDevice* DeviceManager::GetPHDev(PH_Type pPHType)
{
    return m_PHDevList.value(pPHType);
}

LogicDevice* DeviceManager::GetLGDev(LG_Type pLGType)
{
    return m_LGDevList.value(pLGType);
}

bool DeviceManager::OpenBootMdp(QString pPath)
{
    //>@获取CODE
    QString tmpPath = getFileDirectory(pPath);
    if(!tmpPath.endsWith("/"))
        tmpPath.append("/");

    CloseBootMdp();
    mMdpManager = new mdpManager(pPath);
    mMdpManager->SetForcesave(false);
    if(!mMdpManager->OpenMDP())
    {
        mMdpManager->m_IsChanged = false;
        delete mMdpManager;
        mMdpManager = 0;
        IDE_TRACE_STR(QString("OpenBootMdp-%1#Failed").arg(pPath));
        return false;
    }

    devManager *tmpManager = new devManager(tmpPath+"info.dev");
    if(tmpManager->OpenDEV())
        mDevCode = tmpManager->GetCode();
    else
        mDevCode = QString("No Code");
    tmpManager->m_IsChanged = false;
    delete tmpManager;

    mPasswd = mMdpManager->GetPassword();
    mLocation = mMdpManager->GetLocation();

    return true;
}
void DeviceManager::CloseBootMdp()
{
    if(mMdpManager)
    {
        delete mMdpManager;
        mMdpManager = 0;
    }
}
bool DeviceManager::SaveBootMdp()
{
    if(mMdpManager)
    {
        mMdpManager->m_IsChanged = true;
        if(!mMdpManager->SaveMDP())
        {
            IDE_TRACE_STR("SaveBootMDP#Failed");
            return false;
        }
        return true;
    }
    return false;
}
