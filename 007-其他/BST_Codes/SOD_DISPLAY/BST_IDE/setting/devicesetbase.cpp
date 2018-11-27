#include "devicesetbase.h"

DeviceBase::DeviceBase(QWidget *parent) :
    QWidget(parent), DevManager()
{
    m_LoginState = false;

    m_Connected = false;

    mOperateState = true;
    mNetworkList = 0;
    mCurNetDev = 0;
    mNetworkManager = new NetworkManager(this);
    mMdpManager = 0;
    mUiManager = 0;
    mBusyFlagIcon = 0;
    mOffline = true; //>@默认离线状态
    mHostAsync = false;  //>@默认不是主动断开的连接，如果是主动断开的连接，则不提示网络断线

    mUploadState = false;
    mManualExit = false;

    m_MediaPlayer = new MEDIA_PLAYER;
    m_MediaPlayer->SetEnable(true);

    mBusyTimeout.setSingleShot(true);
    connect(&mBusyTimeout, SIGNAL(timeout()), this, SLOT(slot_CloseBusy()));

    connect(this, SIGNAL(sConnect()), this, SLOT(slot_Connect()), Qt::QueuedConnection);
    connect(this, SIGNAL(sDisconnect()), this, SLOT(slot_Disconnect()), Qt::QueuedConnection);

    SetMonitor(500);

    QTimer::singleShot(100, this, SLOT(slot_Initialize()));
}

DeviceBase::~DeviceBase()
{
    //>@先注销逻辑设备，再注销物理设备
    QList<LogicDevice*> tmpLgList = m_LGDevList.values();
    LogicDevice *tmpLGDev = 0;
    foreach(tmpLGDev, tmpLgList)
        tmpLGDev->deleteLater();
    QList<PhysicalDevice*> tmpPhList = m_PHDevList.values();
    PhysicalDevice *tmpPHDev = 0;
    foreach(tmpPHDev, tmpPhList)
        tmpPHDev->deleteLater();
    m_PHDevList.clear();
    m_LGDevList.clear();

    if(mBusyFlagIcon)
        mBusyFlagIcon->deleteLater();
}

bool DeviceBase::isChanged()
{
    if(mMdpManager)
    {
        if(mMdpManager->m_IsChanged)
            return true;
    }
    if(mUiManager)
    {
        if(mUiManager->m_IsChanged)
            return true;
    }
    return false;
}

void DeviceBase::slot_Initialize()
{
    Disconnected(false);
}

void DeviceBase::slot_PHConnState(bool pConn)
{
    if(pConn)
    {
        Connected();
    }
    else
    {
        Disconnected();
    }
}

void DeviceBase::AddNetworkToList(NetworkItem *pNetItem)
{
    if(!pNetItem)
        return;
    QString tmpSSID = pNetItem->mSSID;
#ifndef SUPPORT_LOCALTEST
    if(tmpSSID.count() != 18 && tmpSSID.compare(D_DEFAULTSSID, Qt::CaseInsensitive))
        return;
#endif
    QListWidgetItem *tmpItem = AddItemToListWidget(mNetworkList, tmpSSID);
    if(tmpItem)
    {
        tmpItem->setData(D_NETSSID, (quint32)pNetItem);
        tmpItem->setData(D_IPADDR, D_PREFER_IP);
        tmpItem->setIcon(QIcon(":/Icon/rc/Icon/off.png"));
    }
    IDE_TRACE_STR(tmpSSID);
}

bool DeviceBase::Connected()
{
    if(m_Connected)
        return true;
    m_Connected = true;

    mHostAsync = false;

    emit sConnect();
    return true;
}

bool DeviceBase::isConnected()
{
    if(!m_TcpSocket.isNull())
        return m_TcpSocket->isConnected();
    return false;
}

void DeviceBase::slot_Connect()
{
    slot_InitFromNetwork();
    //>@oldconfg
    if(m_LoginState)
        slot_GotoStep(2);
}

void DeviceBase::slot_Disconnect()
{
    Disconnected();
}

bool DeviceBase::Disconnected(bool pMsg)
{
    if(!m_Connected)
        return true;
    m_Connected = false;

    CloseBootMdp();
    CloseThemeUI();

    if(mCurNetDev)
    {
        mCurNetDev->setIcon(QIcon(":/Icon/rc/Icon/off.png"));
#ifndef SUPPORT_LOCALTEST
        NetworkItem *tmpNetItem = VAR_CAST<NetworkItem*>((QObject*)(mCurNetDev->data(D_NETSSID).toUInt()));
        if(tmpNetItem)
            tmpNetItem->Disconnect(3000);
#endif
        mCurNetDev = 0;
    }

    if(pMsg && !mHostAsync)
    {
#ifdef PHONE
        QMessageBox tmpMsgBox(QMessageBox::Warning,
                              tr("Warn"),
                              tr("Current network is broken,\n"
                                 "Please check it!"),
                              QMessageBox::Ok);
#else
        MsgBox tmpMsgBox(tr("Current network is broken,\n"
                                "Please check it!"));
#endif
        tmpMsgBox.exec();
    }
    ShowBusy(false);
    slot_GotoStep(12);

    return true;
}

void DeviceBase::ShowBusy(bool pFlag, quint64 pTimeout)
{
    if(!mBusyFlagIcon)
    {
        mBusyFlagIcon = new gifplayer(this);
    }

    mBusyTimeout.stop();
    if(pFlag)
    {
        mNetTransFlag = false;
        setDisabled(true);
#if 1
        mBusyFlagIcon->setFixedSize(100,100);
        mBusyFlagIcon->move(QPoint(size().width()/2 - mBusyFlagIcon->size().width()/2.0, size().height()/2 - mBusyFlagIcon->size().height()/2.0));
#else
#ifdef PHONE
        QDesktopWidget *desktop = qApp->desktop();
        int screen_number = desktop->primaryScreen();
        QRect rect = desktop->screenGeometry(screen_number);
        QPoint tmpPoint = rect.topLeft() + QPoint(size().width()/2 - mBusyFlagIcon->size().width()/2.0, size().height()/2 - mBusyFlagIcon->size().height()/2.0);
#else
        QPoint tmpPoint = mapToGlobal(QPoint(0,0)) + QPoint(size().width()/2 - mBusyFlagIcon->size().width()/2.0, size().height()/2 - mBusyFlagIcon->size().height()/2.0);
#endif
        mBusyFlagIcon->setFixedSize(100, 100);
        mBusyFlagIcon->move(tmpPoint);
#endif
        if(pTimeout > 0)
        {
            mBusyFlagIcon->Play(":/Icon/rc/Icon/busy.gif", true);
            mBusyTimeout.setInterval(pTimeout);
            mBusyTimeout.start();
        }
        QtSleep(100);
    }
    else
    {
        QtSleep(100);
        slot_CloseBusy();
    }
}

void DeviceBase::slot_CloseBusy()
{
    mNetTransFlag = true;
    setEnabled(true);
    mBusyFlagIcon->Stop();
}

bool DeviceBase::OpenBootMdp(QString pPath)
{
    CloseBootMdp();
    mMdpManager = new mdpManager(pPath);
    if(!mMdpManager->OpenMDP())
    {
        mMdpManager->m_IsChanged = false;
        delete mMdpManager;
        mMdpManager = 0;
        return false;
    }
    mMdpManager->m_IsChanged = false;
    return true;
}

void DeviceBase::CloseBootMdp()
{
    if(mMdpManager)
    {
        delete mMdpManager;
        mMdpManager = 0;
    }
}

bool DeviceBase::SaveBootMdp()
{
    if(mMdpManager)
    {
        mMdpManager->m_IsChanged = true;
        if(!mMdpManager->SaveMDP())
        {
            return false;
        }
        return true;
    }
    return false;
}

bool DeviceBase::OpenThemeUI(QString pPath)
{
    CloseThemeUI();
    mUiManager = new uiManager(pPath);
    if(!mUiManager->OpenUI())
    {
        mUiManager->m_IsChanged = false;
        delete mUiManager;
        mUiManager = 0;
        return false;
    }
    mUiManager->m_IsChanged = false;
    return true;
}

void DeviceBase::CloseThemeUI()
{
    if(mUiManager)
    {
        delete mUiManager;
        mUiManager = 0;
    }
}

bool DeviceBase::SaveThemeUI()
{
    if(mUiManager)
    {
        mUiManager->m_IsChanged = true;
        if(!mUiManager->SaveUI())
        {
            return false;
        }
        return true;
    }
    return false;
}

bool DeviceBase::ProcessCommand(int pType, QString pCmd, bool pWait)
{
    if(pCmd.isEmpty())
        return false;
    if(!(mMdpManager))
        return false;

    if(!m_CmdString.isEmpty() && !m_CmdString.compare(pCmd, Qt::CaseInsensitive))
        return false;

    m_CmdString = pCmd;

    //ShowBusy(true, 0xffffffff);  //>@如果加上会出现slider重复执行两次的bug。
    if(!isOfflineLogin())
    {
        mNetTransFlag = false;
        qint64 tmpTimeout = 0;
        if(pWait)
        {
            ShowBusy(true, 0xffffffff);
            tmpTimeout = 5000;
        }
        if(!m_TcpSocket->Set(pCmd, tmpTimeout))
        {
#ifdef PHONE
            QMessageBox tmpMsgBox(QMessageBox::Warning,
                                  tr("Warn"),
                                  tr("set %1 error!").arg(pCmd),
                                  QMessageBox::Ok);
#else
            MsgBox tmpMsgBox(tr("set %1 error!").arg(pCmd));
#endif
            tmpMsgBox.exec();
            if(pWait)
                ShowBusy(false);
            return false;
        }
        if(pWait)
            ShowBusy(false);
    }
    //>@同步本地操作
    xmlParse *tmpSetParse = new xmlParse;
    if(tmpSetParse->setContent(pCmd))
    {
        QDomElement tmpElement = tmpSetParse->documentElement();
        if(!tmpElement.isNull())
        {
            QString tmpCommonType = tmpElement.tagName();
            if(!tmpCommonType.compare("Physical", Qt::CaseInsensitive))
            {
                QDomNodeList tmpDevList = tmpElement.childNodes();
                for(int i=0;i<tmpDevList.count();i++)
                {
                    QDomElement tmpDevElement = tmpDevList.at(i).toElement();
                    if(tmpDevElement.isNull())
                        continue;
                    PH_Type tmpType = GetPHType(tmpDevElement.tagName());
                    QDomElement tmpLocalDevElement = mMdpManager->m_PHElementGroup.value(tmpType);
                    QString tmpPath = QString("/CfgPara/Physical/%1").arg(tmpDevElement.tagName());
                    if(tmpLocalDevElement.isNull())
                        if(!mMdpManager->CreateItemElement(tmpPath))
                            continue;
                    QDomNodeList tmpParaList = tmpDevElement.childNodes();
                    for(int i=0;i<tmpParaList.count();i++)
                    {
                        QDomElement tmpParaElement = tmpParaList.at(i).toElement();
                        if(tmpParaElement.isNull())
                            continue;
                        QString tmpParaName = tmpParaElement.tagName();
                        QString tmpParaPath = QString("%1/%2").arg(tmpPath).arg(tmpParaName);
                        QDomElement tmpLocalParaElement = tmpLocalDevElement.firstChildElement(tmpParaName);
                        if(tmpLocalParaElement.isNull())
                            if(!mMdpManager->CreateItemElement(tmpParaPath))
                                continue;
                        mMdpManager->ModifyItemElement(tmpLocalParaElement, tmpParaElement.text());
                    }
                }
                //mMdpManager->SaveMDP();
            }
            else if(!tmpCommonType.compare("Logic", Qt::CaseInsensitive))
            {
                QDomNodeList tmpDevList = tmpElement.childNodes();
                for(int i=0;i<tmpDevList.count();i++)
                {
                    QDomElement tmpDevElement = tmpDevList.at(i).toElement();
                    if(tmpDevElement.isNull())
                        continue;
                    LG_Type tmpType = GetLGType(tmpDevElement.tagName());
                    QDomElement tmpLocalDevElement = mMdpManager->m_LGElementGroup.value(tmpType);
                    QString tmpPath = QString("/CfgPara/Logic/%1").arg(tmpDevElement.tagName());
                    if(tmpLocalDevElement.isNull())
                        if(!mMdpManager->CreateItemElement(tmpPath))
                            continue;
                    QDomNodeList tmpParaList = tmpDevElement.childNodes();
                    for(int i=0;i<tmpParaList.count();i++)
                    {
                        QDomElement tmpParaElement = tmpParaList.at(i).toElement();
                        if(tmpParaElement.isNull())
                            continue;
                        QString tmpParaName = tmpParaElement.tagName();
                        QString tmpParaPath = QString("%1/%2").arg(tmpPath).arg(tmpParaName);
                        QDomElement tmpLocalParaElement = tmpLocalDevElement.firstChildElement(tmpParaName);
                        if(tmpLocalParaElement.isNull())
                            if(!mMdpManager->CreateItemElement(tmpParaPath))
                                continue;
                        mMdpManager->ModifyItemElement(tmpLocalParaElement, tmpParaElement.text());
                    }
                }
                //mMdpManager->SaveMDP();
            }
            else if(!tmpCommonType.compare("Theme", Qt::CaseInsensitive))
            {
                QDomNodeList tmpComList = tmpElement.childNodes();
                for(int i=0;i<tmpComList.count();i++)
                {
                    QDomElement tmpComElement = tmpComList.at(i).toElement();
                    if(tmpComElement.isNull())
                        continue;
                    COM_TYPE tmpType = getComponetType(tmpComElement.tagName());
                    QDomElement tmpLocalComElement = mUiManager->m_ComElementGroup.value(tmpType);
                    if(tmpLocalComElement.isNull())
                        continue;
                    QDomNodeList tmpList = tmpComElement.childNodes();
                    for(int i=0;i<tmpList.count();i++)
                    {
                        QDomElement tmpElement = tmpList.at(i).toElement();
                        if(tmpElement.isNull())
                            continue;
                        QString tmpName = tmpElement.tagName();
                        if(tmpName.compare("StateMachine", Qt::CaseInsensitive) == 0)
                        {
                            QDomElement tmpLocalSMElement = tmpLocalComElement.firstChildElement("StateMachine");
                            if(tmpLocalSMElement.isNull())
                                continue;
                            QDomNodeList tmpStateList = tmpElement.childNodes();
                            for(int j=0;j<tmpStateList.count();j++)
                            {
                                QDomElement tmpNewStateElement = tmpStateList.at(j).toElement();
                                if(tmpNewStateElement.isNull())
                                    continue;
                                QString tmpStateName = tmpNewStateElement.tagName();
                                QDomElement tmpOldStateElement = tmpLocalSMElement.firstChildElement(tmpStateName);
                                if(tmpOldStateElement.isNull())
                                    continue;
                                QString tmpEffectType = tmpNewStateElement.attribute(D_EFFECTTYPE);
                                if(mUiManager->ModifyEffectType(tmpOldStateElement, tmpEffectType))
                                {
                                    QString tmpEffectPara = tmpNewStateElement.attribute(D_EFFECTPARA);
                                    mUiManager->ModifyEffectPara(tmpOldStateElement, tmpEffectPara);
                                }
                            }
                        }
                    }
                }
                //mUiManager->SaveUI();
            }
        }
    }
    tmpSetParse->deleteLater();
    //ShowBusy(false);
    return true;
}

DeviceSetBase::DeviceSetBase(QWidget *parent, PhysicalLANR* pLanR) :
    DeviceBase(parent)
{
    if(pLanR)
    {
        m_FromOutside = true;
        m_LanRouter = pLanR;
    }
    else
    {
        m_LanRouter = new PhysicalLANR(this);
        m_LanRouter->slot_Initialize();
    }
}

DeviceSetBase::~DeviceSetBase()
{
    Async();
    if(!m_FromOutside)
    {
        if(!m_LanRouter.isNull())
            m_LanRouter->deleteLater();
    }
}

quint32 DeviceSetBase::Monitor(quint32 pCount)
{
    if(!m_TcpSocket.isNull())
    {
        m_TcpSocket->Monitor(pCount);
    }
    if(!m_FromOutside && !m_LanRouter.isNull())
    {
        m_LanRouter->Monitor(pCount);
    }
}

QStringList DeviceSetBase::ScanNetwork()
{
    if(m_LanRouter.isNull())
        return QStringList();
    //>@扫描节点，对于已经存在的不处理，如果之前存在现在不存在的设置为offline
    return m_LanRouter->Scan(QString(), 2000);
}

bool DeviceSetBase::isSync(quint32 pTimeout)
{
    if(!m_TcpSocket.isNull())
    {
        if(pTimeout == 0)
            return isConnected();
        int i = 0;
        while(((i+=10) < pTimeout) && !isConnected())
            QtSleep(10);
        return isConnected();
    }
    return false;
}

void DeviceSetBase::Sync()
{
    if(mNetworkList)
    {
        QListWidgetItem *tmpDevItem = mNetworkList->currentItem();
        if(tmpDevItem)
        {
            QHostAddress tmpIP = QHostAddress(tmpDevItem->data(D_IPADDR).toString());
            quint16 tmpPort = D_NMPORT;
            Sync(tmpIP, tmpPort);
        }
    }
}

void DeviceSetBase::Sync(QHostAddress pIP, quint16 pPort)
{
    if(m_LanRouter.isNull())
        return;
    if(!m_TcpSocket.isNull())
    {
        disconnect(m_TcpSocket.data(), SIGNAL(sConnState(bool)), 0, 0);
        m_TcpSocket->Async();
    }
    m_TcpSocket = m_LanRouter->Connect(pIP, pPort);
    if(m_TcpSocket.isNull())
        return;
    connect(m_TcpSocket.data(), SIGNAL(sConnState(bool)), this, SLOT(slot_PHConnState(bool)));
}

void DeviceSetBase::Async()
{
    if(!m_TcpSocket.isNull())
    {
        disconnect(m_TcpSocket.data(), SIGNAL(sConnState(bool)), 0, 0);
        m_TcpSocket->Async();
    }
}

void DeviceSetBase::slot_Disconnect()
{
    Async();
    Disconnected();
}

DeviceSetApp::DeviceSetApp(QWidget *parent) :
    DeviceSetBase(parent)
{
#ifdef QT5
    connect(qApp, SIGNAL(applicationStateChanged(Qt::ApplicationState)), this, SLOT(onAppStateChanged(Qt::ApplicationState)));
#endif
}

DeviceSetApp::~DeviceSetApp()
{

}

QString DeviceSetApp::ResizeQSS(QString pQSS)
{
#ifdef QT5
    qreal tmpVDiff = (qreal)(m_SizeLog[1].height()) / (qreal)(m_SizeLog[0].height());

    //>@计算比例：pixelSize = DPI * pointSize/72
    QScreen *screen = qApp->primaryScreen();
    qreal tmpDpi = screen->logicalDotsPerInch();
#ifndef D_USEWIDGET
    qreal tmpPtDiff = ((qreal)(m_SizeLog[1].height()*D_PC_DPI) / (qreal)(m_SizeLog[0].height()*tmpDpi));
#else
    qreal tmpPtDiff = ((qreal)(m_SizeLog[1].width()*D_PC_DPI) / (qreal)(m_SizeLog[0].width()*tmpDpi));
#endif
    qreal tmpPxDiff = tmpVDiff;

    int index = 0;
    while(1)
    {
        int index1 = pQSS.indexOf("pt", index);
        if(index1 < 0)
            break;
        int index2 = pQSS.lastIndexOf(" ", index1);
        if(index2 < 0)
            continue;
        int tmpindex = index2 + 1;
        QString tmpSizeStr = pQSS.mid(tmpindex, index1-tmpindex);
        int tmpSize = tmpSizeStr.toInt();
        tmpSize*=tmpPtDiff;
        pQSS.replace(tmpindex, index1-tmpindex, QString::number(tmpSize));

        index = index1 + 2;
    }
    index = 0;
    while(1)
    {
        int index1 = pQSS.indexOf("px", index);
        if(index1 < 0)
            break;
        int index2 = pQSS.lastIndexOf(" ", index1);
        if(index2 < 0)
            continue;
        int tmpindex = index2 + 1;
        QString tmpSizeStr = pQSS.mid(tmpindex, index1-tmpindex);
        int tmpSize = tmpSizeStr.toInt();
        tmpSize*=tmpPxDiff;
        pQSS.replace(tmpindex, index1-tmpindex, QString::number(tmpSize));

        index = index1 + 2;
    }
#endif
    return pQSS;
}

void DeviceSetApp::resizeEvent(QResizeEvent *e)
{
#ifdef QT5
    static int count = 0;
    QSize tmpSize = e->size();
    if(tmpSize.isNull())
        return;
    if(count == 0)   //>@在ui设计器中设置的大小，也即原始ui的大小
    {
        m_SizeLog[0] = tmpSize;
        setMinimumSize(0, 0);
        setMaximumSize(10000, 10000);

#ifndef ANDROID
        InitUI();
#endif

        count++;
    }
    else if(count == 1)   //>@在手机中显示的大小，只有在此处才重新设置界面布局
    {
        if(!(tmpSize.width() >= m_SizeLog[0].width() && tmpSize.height() >= m_SizeLog[0].height()))
            return;
#ifdef PHONE
        if(m_SizeLog[0] != tmpSize)
#endif
        {
            m_SizeLog[1] = tmpSize;

            setFixedSize(tmpSize);

            qreal tmpVDiff = (qreal)(m_SizeLog[1].height()) / (qreal)(m_SizeLog[0].height());
            int tmpH = tmpVDiff * m_SizeLog[0].width();
            if(tmpH <= m_SizeLog[1].width())
                tmpH = m_SizeLog[1].width();
            qreal tmpHDiff = (qreal)(tmpH) / (qreal)(m_SizeLog[0].width());

            //>@计算比例：pixelSize = DPI * pointSize/72
            QScreen *screen = qApp->primaryScreen();
            qreal tmpDpi = screen->logicalDotsPerInch();
#ifndef D_USEWIDGET
            qreal tmpPtDiff = tmpVDiff*D_PC_DPI/tmpDpi;
            qreal tmpPxDiff = tmpVDiff;
#else
            qreal tmpPtDiff = tmpHDiff*D_PC_DPI/tmpDpi;
            qreal tmpPxDiff = tmpHDiff;
#endif

            //>@修改控件fix大小
            QList<QWidget*>tmpList = findChildren<QWidget*>();
            QWidget *tmpWid = 0;
            foreach(tmpWid, tmpList)
            {
                tmpWid->setFocusPolicy(Qt::NoFocus); //>@同时设置所有widget都没有焦点
                QListWidget *tmpListWidget = VAR_CAST<QListWidget*>(tmpWid);
                if(tmpListWidget)
                {
                    tmpListWidget->setGridSize(QSize(tmpListWidget->gridSize().width()*tmpHDiff, tmpListWidget->gridSize().height()*tmpVDiff));
                }
                if(tmpWid->minimumWidth() && (tmpWid->minimumWidth() == tmpWid->maximumWidth()))
                    tmpWid->setFixedWidth(tmpWid->minimumWidth() * tmpHDiff);
                if(tmpWid->minimumHeight() && (tmpWid->minimumHeight() == tmpWid->maximumHeight()))
                    tmpWid->setFixedHeight(tmpWid->minimumHeight() * tmpVDiff);
            }

            QList<QLayout*>tmpLayoutList = findChildren<QLayout*>();
            QLayout *tmpLayout = 0;
            foreach(tmpLayout, tmpLayoutList)
            {
                if(tmpLayout)
                {
                    int space = tmpLayout->spacing();
                    if(space > 0)
                        tmpLayout->setSpacing(space*tmpHDiff);
                    int left, top, right, bottom;
                    tmpLayout->getContentsMargins(&left, &top, &right, &bottom);
                    if(left > 0)
                        left *= tmpHDiff;
                    if(top > 0)
                        top *= tmpVDiff;
                    if(right > 0)
                        right *= tmpHDiff;
                    if(bottom > 0)
                        bottom *= tmpVDiff;
                    tmpLayout->setContentsMargins(left, top, right, bottom);
                }
            }

            //>@修改Stylesheet中的字体
            QString tmpStyleSheet = styleSheet();
            int index = 0;
            while(1)
            {
                int index1 = tmpStyleSheet.indexOf("pt", index);
                if(index1 < 0)
                    break;
                int index2 = tmpStyleSheet.lastIndexOf(" ", index1);
                if(index2 < 0)
                    continue;
                int tmpindex = index2 + 1;
                QString tmpSizeStr = tmpStyleSheet.mid(tmpindex, index1-tmpindex);
                int tmpSize = tmpSizeStr.toInt();
                tmpSize*=tmpPtDiff;
                tmpStyleSheet.replace(tmpindex, index1-tmpindex, QString::number(tmpSize));

                index = index1 + 2;
            }
            index = 0;
            while(1)
            {
                int index1 = tmpStyleSheet.indexOf("px", index);
                if(index1 < 0)
                    break;
                int index2 = tmpStyleSheet.lastIndexOf(" ", index1);
                if(index2 < 0)
                    continue;
                int tmpindex = index2 + 1;
                QString tmpSizeStr = tmpStyleSheet.mid(tmpindex, index1-tmpindex);
                int tmpSize = tmpSizeStr.toInt();
                tmpSize*=tmpPxDiff;
                tmpStyleSheet.replace(tmpindex, index1-tmpindex, QString::number(tmpSize));

                index = index1 + 2;
            }
            setStyleSheet(tmpStyleSheet);
            //>@
            count++;
        }

        InitUI();
    }
#endif
    QWidget::resizeEvent(e);
}

void DeviceSetApp::keyPressEvent(QKeyEvent * event)
{
    if(event->key() == Qt::Key_Back) //press return
    {
        slot_BackStep();
        event->accept();  //>@表示当前窗口需要此信号，如果为ingore则表示不需要，此信号会继续给此窗口的父窗口使用。
        return;
    }
    QWidget::keyPressEvent(event);
}

QEvent::Type SimpleCustomEvent::m_evType = (QEvent::Type)QEvent::None;

SimpleCustomEvent::SimpleCustomEvent(int arg1, const QString &arg2)
    : QEvent(eventType()), m_arg1(arg1), m_arg2(arg2)
{}

SimpleCustomEvent::~SimpleCustomEvent()
{}

QEvent::Type SimpleCustomEvent::eventType()
{
    if(m_evType == QEvent::None)
    {
        m_evType = (QEvent::Type)registerEventType();
    }
    return m_evType;
}

