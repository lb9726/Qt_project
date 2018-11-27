#include "pluginwidget.h"

#ifdef LINUX
#include <linux/watchdog.h>
#include <sys/ioctl.h>
#endif

QString GetSerialPort(QString num)
{
    QString tmpPortName;
#ifdef WINDOWS
    tmpPortName = QString("COM%1").arg(num);
#elif UBUNTU
    //tmpPortName = QString("/dev/ttyUSB%1").arg(num);
    tmpPortName = QString("/dev/ttyS%1").arg(num);
#elif IMX6X
    tmpPortName = QString("/dev/ttymxc%1").arg(num);
#else
    tmpPortName = QString("/dev/ttyS%1").arg(num);
#endif
    IDE_DEBUG(tmpPortName);
    return tmpPortName;
}

ThreadTick::ThreadTick(QObject *parent) :
    QThread(parent)
{
    m_MainForm = VAR_CAST<PluginWidget*>(parent);
    m_WDFile = new QFile;
    m_WDFile->setFileName(QString("/dev/watchdog"));

    m_LedFile = new QFile;
    m_LedFile->setFileName(QString("/dev/led"));
    m_LEDState = 0;
}

ThreadTick::~ThreadTick()
{
    if(m_WDFile)
    {
        m_WDFile->close();
        m_WDFile->deleteLater();
    }
}

void ThreadTick::FeedDog()
{
#ifdef LINUX
#ifdef N329
    if(!m_WDFile->isOpen())
    {
        if(m_WDFile->open(QIODevice::ReadWrite|QIODevice::Unbuffered))
        {
            int tmpSecond = 5;
          //  IDE_DEBUG(QString("Open watchdog success"));
            ioctl(m_WDFile->handle(), WDIOC_SETTIMEOUT, &tmpSecond);
        }
        else
        {
            IDE_TRACE();
            return;
        }
    }
    if(!m_LedFile->isOpen())
    {
        m_LedFile->open(QIODevice::ReadWrite|QIODevice::Unbuffered);
    }
    if(m_WDFile->handle())
    {
        //IDE_DEBUG("Feed dog Success!");
        ioctl(m_WDFile->handle(), WDIOC_KEEPALIVE);
        if(m_LedFile->isOpen())
        {
            if(m_LEDState)
                m_LEDState = 0;
            else
                m_LEDState = 1;
            QByteArray tmpArray;
            tmpArray.append(m_LEDState);
            m_LedFile->write(tmpArray);
        }
    }
#else
    if(!m_WDFile->isOpen())
    {
        if(m_WDFile->open(QIODevice::WriteOnly))
        {
#ifdef IMX53
            int tmpSecond = 6;
            ioctl(m_WDFile->handle(), 0xc0045706, &tmpSecond);
#elif  IMX6X
            int tmpSecond = 3000;  //>@自动设置喂狗的时间为超时时间的一半
            ioctl(m_WDFile->handle(), WDIOC_SETTIMEOUT, &tmpSecond);
#endif
        }

    }
    if(m_WDFile->isOpen() && m_WDFile->handle())
    {
        ioctl(m_WDFile->handle(), WDIOC_KEEPALIVE, 0);
        IDE_DEBUG("Feed dog Success!");
    }
#endif
#endif
}

void ThreadTick::run()
{
    while(1)
    {
#ifdef LINUX
        FeedDog();
        system("sync");
        system("echo 3 > /proc/sys/vm/drop_caches");
#endif
        if(!m_MainForm.isNull())
        {
            m_MainForm.data()->slot_Tick();
        }
        sleep(1);
    }
}

QString PluginWidget::ResizeQSS(QString pQSS)
{
#ifdef QT5
    qreal tmpVDiff = (qreal)(m_SizeLog[1].height()) / (qreal)(m_SizeLog[0].height());

    //>@计算比例：pixelSize = DPI * pointSize/72
    QScreen *screen = qApp->primaryScreen();
    qreal tmpDpi = screen->logicalDotsPerInch();
    qreal tmpPtDiff = ((qreal)(m_SizeLog[1].width()*90) / (qreal)(m_SizeLog[0].width()*tmpDpi));
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

void PluginWidget::resizeEvent(QResizeEvent *e)
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

        count++;
    }
    else if(count == 1)   //>@在手机中显示的大小，只有在此处才重新设置界面布局
    {
#ifdef PHONE
        if(m_SizeLog[0] != tmpSize)
#endif
        {
            m_SizeLog[1] = tmpSize;

            qreal tmpVDiff = (qreal)(m_SizeLog[1].height()) / (qreal)(m_SizeLog[0].height());
            qreal tmpHDiff = (qreal)(m_SizeLog[1].width()) / (qreal)(m_SizeLog[0].width());

            //>@计算比例：pixelSize = DPI * pointSize/72
            QScreen *screen = qApp->primaryScreen();
            qreal tmpDpi = screen->logicalDotsPerInch();
            qreal tmpPtDiff = ((qreal)(m_SizeLog[1].width()*90) / (qreal)(m_SizeLog[0].width()*tmpDpi));
            qreal tmpPxDiff = tmpVDiff;

            //>@修改控件fix大小
            QList<QWidget*>tmpList = findChildren<QWidget*>();
            QWidget *tmpWid = 0;
            foreach(tmpWid, tmpList)
            {
                tmpWid->setFocusPolicy(Qt::NoFocus); //>@同时设置所有widget都没有焦点
                QListWidget *tmpListWidget = VAR_CAST<QListWidget*>(tmpWid);
                if(tmpListWidget)
                {
                    tmpListWidget->setGridSize(QSize(tmpListWidget->gridSize().width()*tmpVDiff, tmpListWidget->gridSize().height()*tmpVDiff));
                }
                if(tmpWid->minimumWidth() && (tmpWid->minimumWidth() == tmpWid->maximumWidth()))
                    tmpWid->setFixedWidth(tmpWid->minimumWidth() * tmpVDiff);
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
                        top *= tmpHDiff;
                    if(right > 0)
                        right *= tmpHDiff;
                    if(bottom > 0)
                        bottom *= tmpHDiff;
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

PluginWidget::PluginWidget(QWidget*parent) :
    QMainWindow(parent)
{
    installEventFilter(this);

    mRegistedFlrCount = 0;

    mSetMenuWidget = 0;
    mSetMenu = 0;
    m_DelayCount = 0;
    m_DelayLabel = 0;

    m_Luminance = 100;

    m_ProtocolType = D_ELE_TYPE_CAN;
    //CAN
    m_CanBuad = 50;
    m_CanID = 1;
    //485
    m_485Buad = 9600;
    m_485ID = 1;

    mMdpParser = 0;
    mUiParser = 0;
    m_ThemeManager = 0;

    mDebugLabel = 0;
    m_TickThread = 0;
    m_SerialPort = 0;

    m_FilterFrame = false;

    mBusy = false;
    m_FrameState = State_Wait;
    m_FrameFlag = 0xffffffff;

    m_AutoTest = false;
    m_MinFlr = m_MaxFlr = -1;

    m_Ticks = 1;
    mKeyEchoLCD = 0;

    m_LcdFile = new QFile;
    m_LcdFile->setFileName(D_FB_DEV);
    m_LcdFile->open(QIODevice::ReadWrite|QIODevice::Unbuffered);

    mVideo = 0;
    mAudio = new ThreadAudio;
	m_UDiskBusy = false;

    for(int i=0;i<7;i++)
        m_BtnBk[i] = 0x00;

    mTcpServer = 0;
    mServerTcp = 0;
    mEternetTcp = 0;

    mTcpServer = new ToolTcpServer;
    connect(mTcpServer, SIGNAL(sNewSocket(QTcpSocket*)), this, SLOT(slot_NewSocket(QTcpSocket*)));
    mTcpServer->Open(QHostAddress::Any, D_NMPORT);

    mEternetTcp = new ToolTcpClient;
    connect(mEternetTcp, SIGNAL(sConnState(bool)), this, SLOT(slot_TcpConnState(bool)));
    connect(mEternetTcp, SIGNAL(readyRead()), this, SLOT(slot_TcpReadyRead()));
    connect(this, SIGNAL(sTcpConnect(QHostAddress, quint16)), this, SLOT(slot_TcpConnect(QHostAddress, quint16)), Qt::QueuedConnection);

    connect(this, SIGNAL(sDateTime(AREA_OPERATE,QVariant)), this, SLOT(slot_DateTime(AREA_OPERATE,QVariant)), Qt::QueuedConnection);

    QTimer::singleShot(100, this, SLOT(slot_Initialize()));
}

PluginWidget::~PluginWidget()
{
    if(mMdpParser)
        mMdpParser->deleteLater();
    if(mUiParser)
        mUiParser->deleteLater();
    if(m_LcdFile)
        m_LcdFile->deleteLater();
    if(mAudio)
        mAudio->deleteLater();
    if(mTcpServer)
        mTcpServer->deleteLater();
    if(mEternetTcp)
        mEternetTcp->deleteLater();
    if(m_TickThread)
        m_TickThread->deleteLater();
    if(m_SerialPort)
        m_SerialPort->deleteLater();
    if(mSetMenuWidget)
        mSetMenuWidget->deleteLater();
}

void PluginWidget::slot_Initialize()
{
    SetLuminance(100);
}

bool PluginWidget::eventFilter(QObject *o, QEvent *e)
{
    if(e)
    {
        if(e->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *event = static_cast<QMouseEvent*>(e);
            if(event)
            {
                WaitForSetMenu(event->pos(), 8000);
            }
        }
        else if(e->type() == QEvent::MouseButtonRelease)
        {
            StopWait();
        }
    }
    return QMainWindow::eventFilter(o, e);
}

void PluginWidget::WaitForSetMenu(QPoint pPos, int pTimeout)
{
    if(m_DelayTimer.isActive())
        return;
    if(mSetMenu && mSetMenu->isVisible())
    {
        StopWait();
        return;
    }
    m_DelayCount = pTimeout/1000;
    m_DelayTimer.start();

    if(!m_DelayLabel)
    {
        m_DelayLabel = new QLabel(this);
        m_DelayLabel->setStyleSheet("border: 2px solid rgba(239,173,36, 250); border-radius: 25px; font: bold large \"Arial\"; font-size:90pt; color: rgba(255,255,255,180);");
        m_DelayLabel->setAlignment(Qt::AlignCenter);
    }
    int w = 100;
    int h = 160;
    m_DelayLabel->setGeometry(QRect(pPos.x() - w/2, pPos.y()-h, w, h));
}

void PluginWidget::StopWait()
{
    if(m_DelayLabel)
    {
        m_DelayLabel->clear();
        m_DelayLabel->hide();
    }
    m_DelayTimer.stop();
}

void PluginWidget::slot_DelayTimeout()
{
    m_DelayCount--;
    if(m_DelayCount <= 0)
    {
        StopWait();
        ShowSetMenu();
    }
    else
    {
        if(m_DelayCount < 6)
        {
            m_DelayLabel->setText(QString::number(m_DelayCount));
            m_DelayLabel->show();
        }
        m_DelayTimer.start();
    }
}

void PluginWidget::InitSetMenu()
{
    mSetMenuWidget = new QWidget(this);
    mSetMenuWidget->setFixedSize(size());
    mSetMenuWidget->setStyleSheet("QWidget { background-color: rgba(100,100,100,150); border:none; }");

    mSetMenu = new StackedMenu(mSetMenuWidget);
    connect(mSetMenu, SIGNAL(sHide()), this, SLOT(slot_SetMenuHide()));
    QRect tmpRect = rect();
    int tmpWidth = mSetMenu->width();
    int tmpHeight = mSetMenu->height();
    QRect tmpSetMenuRect = QRect(tmpRect.left()+(tmpRect.width()-tmpWidth)/2,
                              tmpRect.top()+(tmpRect.height()-tmpHeight)/2 + 300,
                              tmpWidth,
                              tmpHeight);

    mSetMenu->SetGeometry(tmpSetMenuRect);
    mSetMenuWidget->hide();

    m_DelayCount = 0;
    m_DelayLabel = 0;
    m_DelayTimer.setSingleShot(true);
    m_DelayTimer.setInterval(1000);
    connect(&m_DelayTimer, SIGNAL(timeout()), this, SLOT(slot_DelayTimeout()));
}

void PluginWidget::slot_SetMenuHide()
{
    HideSetMenu();
}

void PluginWidget::ShowSetMenu()
{
    //同步信息
    mSetMenu->SyncDateTime(QDateTime::currentDateTime());
    mSetMenu->SyncLuminance(m_Luminance);
    mSetMenu->SyncVolume(m_AudioVol, m_VideoVol);
    if(m_ProtocolType == D_ELE_TYPE_CAN)
        mSetMenu->SyncProtocol(m_ProtocolType, m_CanBuad, m_CanID);
    else
        mSetMenu->SyncProtocol(m_ProtocolType, m_485Buad, m_485ID);

    mSetMenuWidget->raise();
    mSetMenuWidget->show();
}

void PluginWidget::HideSetMenu()
{
    mSetMenuWidget->hide();
}

void PluginWidget::SyncTime(QDateTime pDateTime)
{
    QString tmpCmd = QString("date -s \"%1\"").arg(pDateTime.toString("yyyy-MM-dd h:m"));
#ifndef WINDOWS
    system(tmpCmd.toLatin1().data());
    system("hwclock -w");
#endif
    emit sDateTime(OPERATE_CHANGE, pDateTime);
}

void PluginWidget::SyncLuminance(int pLuminance)
{
    if(pLuminance != m_Luminance)
    {
        m_Luminance = pLuminance;
        SetLuminance(m_Luminance);
        if(mMdpParser)
        {
            QDomElement pElement = mMdpParser->m_PHElementGroup.value(P_LCD);
            if(!pElement.isNull())
            {
                QDomElement tmpElement = pElement.firstChildElement("Luminance");
                if(!tmpElement.isNull())
                {
                    mMdpParser->ModifyItemElement(tmpElement, QString::number(pLuminance));
                }
            }
        }
    }
}

void PluginWidget::SyncVolume(int pAudio, int pVideo)
{
    if(mAudio && mAudio->m_Volume != pAudio)
    {
        mAudio->SetVolume(pAudio);
        IDE_TRACE();
        if(mMdpParser)
        {
            QDomElement pElement = mMdpParser->m_PHElementGroup.value(P_Player);
            if(!pElement.isNull())
            {
                QDomElement tmpElement = pElement.firstChildElement("Audio");
                if(!tmpElement.isNull())
                {
                    mMdpParser->ModifyItemElement(tmpElement, QString::number(pAudio));
                }
            }
        }
    }
    if(mVideo && mVideo->m_Volume != pVideo)
    {
        mVideo->SetVolume(pVideo);
        IDE_TRACE();
        if(mMdpParser)
        {
            QDomElement pElement = mMdpParser->m_PHElementGroup.value(P_Player);
            if(!pElement.isNull())
            {
                QDomElement tmpElement = pElement.firstChildElement("Video");
                if(!tmpElement.isNull())
                {
                    mMdpParser->ModifyItemElement(tmpElement, QString::number(pVideo));
                }
            }
        }
    }
}

void PluginWidget::SyncProtocol(int pType, int pBuad, int pID)
{
    m_ProtocolType = pType;
    if(m_ProtocolType == D_ELE_TYPE_CAN)
    {
        m_CanBuad = pBuad;
        m_CanID = pID;
    }
    else
    {
        m_485Buad = pBuad;
        m_485ID = pID;
    }
    if(mMdpParser)
    {
        QDomElement pElement = mMdpParser->m_PHElementGroup.value(P_COM);
        if(!pElement.isNull())
        {
            QDomElement tmpElement = pElement.firstChildElement("Type");
            if(!tmpElement.isNull())
            {
                mMdpParser->ModifyItemElement(tmpElement, QString::number(m_ProtocolType));
                tmpElement = pElement.firstChildElement("Buad");
                if(!tmpElement.isNull())
                {
                    if(m_ProtocolType == D_ELE_TYPE_CAN)
                        mMdpParser->ModifyItemElement(tmpElement, QString::number(m_CanBuad));
                    else
                        mMdpParser->ModifyItemElement(tmpElement, QString::number(m_485Buad));
                }
                tmpElement = pElement.firstChildElement("ID");
                if(!tmpElement.isNull())
                {
                    if(m_ProtocolType == D_ELE_TYPE_CAN)
                        mMdpParser->ModifyItemElement(tmpElement, QString::number(m_CanID));
                    else
                        mMdpParser->ModifyItemElement(tmpElement, QString::number(m_485ID));
                }
            }
        }
    }
}

void PluginWidget::InitMdpPara(QDomElement pElement)
{
    if(pElement.isNull())
        return;
    QString tmpName = pElement.tagName();
    PH_Type tmpType = GetPHType(tmpName);
    switch(tmpType)
    {
        case P_RTC:
        {
            QDomElement tmpElement = pElement.firstChildElement("DateTime");
            if(!tmpElement.isNull())
            {
                QString tmpPara = tmpElement.attribute("update");
                if(!tmpPara.compare("true", Qt::CaseInsensitive))
                {
                    QString tmpCmd = QString("date -s \"%1\"").arg(tmpElement.text()).toLatin1().data();
#ifdef LINUX
                    system(tmpCmd.toLatin1().data());
                    system("hwclock -w");
#endif
                    tmpElement.setAttribute("update", "false");
                }
            }
            break;
        }
        case P_LAN:
        {
            QDomElement tmpElement = pElement.firstChildElement("WIFI");
            if(!tmpElement.isNull())
            {
                bool tmpAPMode = (bool)(!(tmpElement.attribute("Mode").compare("AP", Qt::CaseInsensitive)));
                if(tmpAPMode)
                {
                    bool tmpDHCP = (bool)(!(tmpElement.attribute("DHCP").compare("true", Qt::CaseInsensitive)));
                    m_WifiIP = QHostAddress(tmpElement.attribute("IP"));
                    QString tmpCMD;
                    if(tmpDHCP)
                        tmpCMD = QString("ifconfig ra0 %1 up | route add default dev ra0 | udhcpd -f %2SYSTEM/Physical/LAN/wifi_udhcpd.conf &").arg(m_WifiIP.toString()).arg(D_FSRCPATH);
                    else
                        tmpCMD = QString("ifconfig ra0 %1 up | route add default dev ra0 &").arg(m_WifiIP.toString());
#ifndef WINDOWS
                    system(tmpCMD.toLatin1().data());
#endif
                }
            }
            tmpElement = pElement.firstChildElement("Eternet");
            if(!tmpElement.isNull())
            {
                bool tmpDHCP = (bool)(!(tmpElement.attribute("DHCP").compare("true", Qt::CaseInsensitive)));
                m_EternteIP = QHostAddress(tmpElement.attribute("IP"));
                QString tmpCMD;
                if(tmpDHCP)
                    tmpCMD = QString("ifconfig eth0 %1 up | route add default dev eth0 | udhcpd -f %2SYSTEM/Physical/LAN/eternet_udhcpd.conf &").arg(m_EternteIP.toString()).arg(D_FSRCPATH);
                else
                    tmpCMD = QString("ifconfig eth0 %1 up | route add default dev eth0 &").arg(m_WifiIP.toString());
#ifndef WINDOWS
                system(tmpCMD.toLatin1().data());
#endif
            }
            tmpElement = pElement.firstChildElement("Server");
            if(!tmpElement.isNull())
            {
                m_ServerIP = QHostAddress(tmpElement.attribute("IP"));
                m_ServerPort = tmpElement.attribute("Port").toInt();
            }
            break;
        }
        case P_LCD:
        {
            QDomElement tmpElement = pElement.firstChildElement("Luminance");
            if(!tmpElement.isNull())
            {
                m_Luminance = tmpElement.text().toInt();
                SetLuminance(m_Luminance);
            }
            break;
        }
        case P_Player:
        {
            QDomElement tmpElement = pElement.firstChildElement("Audio");
            if(!tmpElement.isNull())
            {
                m_AudioVol = tmpElement.text().toInt();
                if(mAudio && mAudio->m_Volume != m_AudioVol)
                {
                    mAudio->SetVolume(m_AudioVol);
                }
            }
            tmpElement = pElement.firstChildElement("Video");
            if(!tmpElement.isNull())
            {
                m_VideoVol = tmpElement.text().toInt();
                if(mVideo && mVideo->m_Volume != m_VideoVol)
                {
                    mVideo->SetVolume(m_VideoVol);
                }
            }
            break;
        }
        case P_COM:
        {
            QDomElement tmpElement = pElement.firstChildElement("Type");
            if(!tmpElement.isNull())
            {
                m_ProtocolType = tmpElement.text().toInt();
                if(m_ProtocolType == D_ELE_TYPE_CAN)
                {

                    tmpElement = pElement.firstChildElement("Buad");
                    if(!tmpElement.isNull())
                    {
                        m_CanBuad = tmpElement.text().toInt();
                    }
                    tmpElement = pElement.firstChildElement("ID");
                    if(!tmpElement.isNull())
                    {
                        m_CanID = tmpElement.text().toInt();
                    }
                }
                else if(m_ProtocolType == D_ELE_TYPE_485)
                {
                    tmpElement = pElement.firstChildElement("Buad");
                    if(!tmpElement.isNull())
                    {
                        m_485Buad = tmpElement.text().toInt();
                    }
                    tmpElement = pElement.firstChildElement("ID");
                    if(!tmpElement.isNull())
                    {
                        m_485ID = tmpElement.text().toInt();
                    }
                }
            }
            break;
        }
        default:
            break;
    }
}

void PluginWidget::InitUiPara()
{

}

void PluginWidget::SetRcPath(QString pRcPath)
{
    m_RcPath = pRcPath;
    if(!pRcPath.isEmpty())
        InitParameter(pRcPath + QString("../../Device.mdp"), pRcPath + QString("Theme.ui"));
}

bool PluginWidget::InitParameter(QString pMdpFile, QString pUiFile)
{
    mMdpParser = new MdpParser(pMdpFile);
    mUiParser = new UiParser(pUiFile);
    return true;
}

bool PluginWidget::SetLuminance(int pValue)
{
    if(pValue < 0)
        pValue = 0;
    else if(pValue > 100)
        pValue = 100;
#ifdef N329
    int tmpValue = 0;
    //>@10?190???0~100???1.8
    tmpValue = 190-(pValue*18/10);
    IDE_DEBUG(QString("Bright light is %1").arg(tmpValue));
    ioctl(m_LcdFile->handle(), IOCTL_LCD_BRIGHTNESS, &tmpValue);
#endif
    return true;
}

void PluginWidget::TcpConnect(QHostAddress pIP, quint16 pPort)
{
    mEternetTcp->Open(pIP, pPort);
}

void PluginWidget::slot_NewSocket(QTcpSocket* pSocket)
{
    if(mServerTcp)
    {
        disconnect(mServerTcp, SIGNAL(readyRead()), 0, 0);
    }
    mServerTcp = VAR_CAST<ToolTcpClient*>(pSocket);
    if(mServerTcp)
    {
        connect(mServerTcp, SIGNAL(readyRead()), this, SLOT(slot_ServerReadyRead()));
    }
}

void PluginWidget::slot_TcpConnState(bool pFlag)
{

}

void PluginWidget::slot_TcpConnect(QHostAddress pIP, quint16 pPort)
{
    TcpConnect(pIP, pPort);
}

void PluginWidget::DelBtnFocus()
{
    QList<QPushButton*>tmpList = findChildren<QPushButton*>();
    QPushButton *tmpWid = 0;
    foreach(tmpWid, tmpList)
    {
        tmpWid->setFocusPolicy(Qt::NoFocus); //>@同时设置所有widget都没有焦点
    }
}

void PluginWidget::SetReportCallback(LogicThemeManager *pThemeManager, EmitReport pFunc)
{
    m_ThemeManager = pThemeManager;
    m_EmitReport = pFunc;
}

void PluginWidget::UpdateProgress(QString pMsg, int pValue)
{
    ReportOperate(OPERATE_PROGRESS, QVariant(QString("%1:%2").arg(pMsg).arg(pValue)));
    QtSleep(100);
}

void PluginWidget::ReportOperate(AREA_OPERATE pOperate, QVariant pPara)
{
    if(m_ThemeManager)
    {
        m_EmitReport(m_ThemeManager, pOperate, pPara);   //回调在此发生！传数据到到Qt主程序中，并把释放信号的类实例指针回传
    }
}

void PluginWidget::ExecOperate(AREA_OPERATE pOperate, QVariant pPara)
{

}

void PluginWidget::slot_ReportOperate(AREA_OPERATE pOperate, QVariant pPara)
{
    ReportOperate(pOperate, pPara);
}

void PluginWidget::slot_ExecOperate(AREA_OPERATE pOperate, QVariant pPara)
{
    ExecOperate(pOperate, pPara);
}


bool PluginWidget::UDiskUpgrade(QString pDev)
{
	QString tmpDevPath = MountStorage(pDev, "/usr/Storage/");
    if(tmpDevPath.isEmpty())
    {
        IDE_TRACE_STR(pDev);
        return false;
    }
    QString tmpExeFile = tmpDevPath + QString("BST.cmd");
    QFileInfo tmpFileInfo(tmpExeFile);
    if(tmpFileInfo.exists())
    {
        ExecCmd(tmpExeFile);
    }
    else
    {
        QString tmpSrcDisk = tmpDevPath+QString("DISK/");
        QString pDstPath = QString("/usr/bst/");
        QStringList tmpFileList = findFilesInFloder(tmpSrcDisk, QString(), true);
        if(!tmpFileList.isEmpty())
        {
            QProgressBar *tmpPrgBar = findChild<QProgressBar*>(QString("progress"));
            if(tmpPrgBar)
                tmpPrgBar->show();

            int tmpSrcDiskLength = tmpSrcDisk.count();
            qreal diff = 100.0 / tmpFileList.count();
            QString tmpSrcPath, tmpRelPath, tmpDstPath;
            for(int i=0;i<tmpFileList.count();i++)
            {
                tmpSrcPath = tmpFileList.at(i);
                IDE_TRACE_STR(tmpSrcPath);

                int index = tmpSrcPath.indexOf(tmpSrcDisk);
                if(index < 0)
                    continue;
                tmpRelPath = tmpSrcPath.mid(index+tmpSrcDiskLength);
                if(tmpRelPath.isEmpty())
                    continue;
                tmpDstPath = pDstPath + tmpRelPath;
                IDE_TRACE_STR(tmpDstPath);

                CopyFile(tmpSrcPath, tmpDstPath);

                if(tmpPrgBar)
                    tmpPrgBar->setValue(diff*i);
                QtSleep(1);
            }

            if(tmpPrgBar)
                tmpPrgBar->hide();
        }
    }
    return true;
}

void PluginWidget::SetState(int pType, bool pState)
{
    switch(pType)
    {
        case P_COM:
        {
            QCheckBox *tmpState = findChild<QCheckBox*>(QString("com_state"));
            if(tmpState)
                tmpState->setChecked(pState);
            break;
        }
        case P_MDISK:
        {
            QCheckBox *tmpState = findChild<QCheckBox*>(QString("mdisk_state"));
            if(tmpState)
                tmpState->setChecked(pState);
            break;
        }
        case P_Touch:
        {
            QCheckBox *tmpState = findChild<QCheckBox*>(QString("touch_state"));
            if(tmpState)
                tmpState->setChecked(pState);
            break;
        }
        case P_LAN:
        {
            QCheckBox *tmpState = findChild<QCheckBox*>(QString("net_state"));
            if(tmpState)
                tmpState->setChecked(pState);
            break;
        }
        default:
            break;
    }
}

void PluginWidget::Init(bool pAutoTest, int pMinFlr, int pMaxFlr)
{
    bool tmpFlag = false;

    m_AutoTest = pAutoTest;
    m_MinFlr = pMinFlr;
    m_MaxFlr = pMaxFlr;

    WidgetMedia *tmpMedia = findChild<WidgetMedia*>();
    if(tmpMedia)
    {
        mVideo = tmpMedia->m_MediaPlayer;
    }

    InitFlrBtn();
    InitKeyBtn();

    QProgressBar *tmpPrgBar = findChild<QProgressBar*>(QString("progress"));
    if(tmpPrgBar)
        tmpPrgBar->hide();

    //>@通信状态
    if(m_SerialPort)
    {
        connect(m_SerialPort, SIGNAL(readyRead()), this, SLOT(slot_ProcessData()), Qt::QueuedConnection);
        tmpFlag = m_SerialPort->open(QIODevice::ReadWrite);
    }
    SetState(P_COM, tmpFlag);

    //>@u盘状态
#ifdef IMX6X
    tmpFlag = false;
    QStringList tmpDevList;
    tmpDevList << "/dev/sda1" << "/dev/sda2" << "/dev/sdb1" << "/dev/sdb2" << "/dev/sdc1" << "/dev/sdc2";
    QString tmpDev;
    foreach(tmpDev, tmpDevList)
    {
        tmpFlag = UDiskUpgrade(tmpDev);
        if(tmpFlag)
            break;
    }
    SetState(P_MDISK, tmpFlag);
#endif
    //>@Touch状态
    tmpFlag = isDevExist(QString("/dev/input/event1"));
    SetState(P_Touch, tmpFlag);

    mDeviceWatcher = new QDeviceWatcher;
    mWatcherThread = new QThread;
    mDeviceWatcher->moveToThread(mWatcherThread);
    mDeviceWatcher->appendEventReceiver(this);
    mWatcherThread->start();
    mDeviceWatcher->start();

    connect(mDeviceWatcher, SIGNAL(deviceAdded(QString)), this, SLOT(slot_DeviceAdded(QString)), Qt::AutoConnection);
    connect(mDeviceWatcher, SIGNAL(deviceRemoved(QString)), this, SLOT(slot_DeviceRemoved(QString)), Qt::AutoConnection);

    DelBtnFocus();

    m_TickThread = new ThreadTick(this);
    m_TickThread->start(QThread::LowestPriority);

    InitSetMenu();

    if(mMdpParser && mMdpParser->OpenMDP())
    {
        QList<PH_Type> tmpList = mMdpParser->m_PHElementGroup.keys();
        qSort(tmpList.begin(), tmpList.end());
        for(int i=0;i<tmpList.count();i++)
        {
            QDomElement tmpElement = mMdpParser->m_PHElementGroup.value(tmpList.at(i));
            InitMdpPara(tmpElement);
        }
    }
    if(mUiParser && mUiParser->OpenUI())
    {
        InitUiPara();
    }

    if(m_AutoTest)
    {
        emit sArrow(OPERATE_CHANGE, ArrowUproll);
    }
    emit sDateTime(OPERATE_CHANGE, QDateTime::currentDateTime());
}

void PluginWidget::slot_UpdateUDisk()
{
    if(m_UDiskBusy)
        return;
    m_UDiskBusy = true;

    bool tmpAutotest = m_AutoTest;
    m_AutoTest = false;
#ifdef IMX53
    QString tmpDevPath = "/mnt/usb/";
#else
    QString tmpDevPath = MountStorage(m_MDDev, "/usr/Storage/");
    if(tmpDevPath.isEmpty())
    {
        IDE_TRACE_STR(m_MDDev);
        return;
    }
#endif
    QString tmpExeFile = tmpDevPath + QString("BST.cmd");
    QFileInfo tmpFileInfo(tmpExeFile);
    if(tmpFileInfo.exists())
    {
        ExecCmd(tmpExeFile);
    }
    else
    {
        QString tmpSrcDisk = tmpDevPath+QString("DISK/");
        QString pDstPath = QString("/usr/bst/");
        QStringList tmpFileList = findFilesInFloder(tmpSrcDisk, QString(), true);
        if(!tmpFileList.isEmpty())
        {
            QProgressBar *tmpPrgBar = findChild<QProgressBar*>(QString("progress"));
            if(tmpPrgBar)
            {
                tmpPrgBar->setRange(0, tmpFileList.count());
                tmpPrgBar->show();
                tmpPrgBar->setValue(1);
            }

            int tmpSrcDiskLength = tmpSrcDisk.count();
            QString tmpSrcPath, tmpRelPath, tmpDstPath;
            for(int i=0;i<tmpFileList.count();i++)
            {
                if(m_TickThread)
                    m_TickThread->FeedDog();
                tmpSrcPath = tmpFileList.at(i);
                //IDE_TRACE_STR(tmpSrcPath);

                int index = tmpSrcPath.indexOf(tmpSrcDisk);
                if(index < 0)
                    continue;

                //IDE_TRACE();

                tmpRelPath = tmpSrcPath.mid(index+tmpSrcDiskLength);
                if(tmpRelPath.isEmpty())
                    continue;

                tmpDstPath = pDstPath + tmpRelPath;
                //IDE_TRACE_STR(tmpDstPath);

                CopyFile(tmpSrcPath, tmpDstPath);

                if(tmpPrgBar)
                {
                    if(!tmpPrgBar->isVisible())
                        tmpPrgBar->show();
                    tmpPrgBar->setValue(i);
                }
                QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            }

            if(tmpPrgBar)
                tmpPrgBar->hide();
        }
    }
    SetState(P_MDISK, true);
    m_AutoTest = tmpAutotest;
    IDE_DEBUG("UDisk update succes!");

    m_UDiskBusy = false;
}

void PluginWidget::slot_DeviceAdded(QString dev)
{
    QStringList tmpList = dev.split('>');
    if(tmpList.count()!=2)
        return;
    QString tmpType = tmpList.at(0);
    if(tmpType.compare("storage", Qt::CaseInsensitive) == 0)  //>@U盘
    {
        m_MDDev = tmpList.at(0);
        QTimer::singleShot(2000, this, SLOT(slot_UpdateUDisk()));
    }
    else if(tmpType.compare("touch", Qt::CaseInsensitive) == 0)  //>@touch
    {
        //>@重启应用程序，恢复touch，此处不需要点亮Touch标签
        qApp->quit();
        QProcess::startDetached(qApp->applicationFilePath(), QStringList());
    }
}

void PluginWidget::slot_DeviceRemoved(QString dev)
{
    QStringList tmpList = dev.split('>');
    if(tmpList.count() != 2)
        return;
    QString tmpType = tmpList.at(0);
    if(tmpType.compare("storage", Qt::CaseInsensitive) == 0)  //>@U盘
    {
        Reboot();
    }
    else if(tmpType.compare("usbdev", Qt::CaseInsensitive) == 0)  //>@从机U盘
    {
        QString tmpExeFile = QString("/usr/bst/BST.cmd");
        QFileInfo tmpFileInfo(tmpExeFile);
        if(tmpFileInfo.exists())
        {
            ExecCmd(tmpExeFile);
        }
        //>@拔出丛机USB线时都要重启
        Reboot();
    }
}

void PluginWidget::ProcessFrame(int pType, QByteArray pArray)
{
    int tmpCount = pArray.count();
    if(mDebugLabel)
        mDebugLabel->setText(QString("Recv[%1]").arg(tmpCount));
    volatile int i=0;
    volatile quint8 tmpValue;
    while(i<tmpCount)
    {
        if(mDebugLabel)
            mDebugLabel->setText(QString("1--i[%1]").arg(i));
        switch(m_FrameState)
        {
            case State_Wait:
            {
                mCurFrame[pType].clear();
                for(;i<tmpCount;i++)
                {
                    if(mDebugLabel)
                        mDebugLabel->setText(QString("3--i[%1]").arg(i));
                    tmpValue = (quint8)(pArray.at(i));
                    if(0x80 == tmpValue)
                        m_FrameFlag = 0;
                    else if(0xc0 == tmpValue)
                        m_FrameFlag = 1;
                    else
                    {
                        i++;
                        break;
                    }
                    m_FrameState = State_Continus;
                    break;
                }
                break;
            }
            case State_Continus:
            {
                for(;i<tmpCount;i++)
                {
                    tmpValue = (quint8)(pArray.at(i));
                    volatile quint8 tmpHeader = tmpValue;
                    mCurFrame[pType].append(tmpValue);
                    if(((m_FrameFlag == 0) && (tmpHeader == 0x81)) || ((m_FrameFlag == 1) && (tmpHeader == 0xc1)))
                    {
                        int tmpCrcIndex = mCurFrame[pType].count()-2;
                        quint8 tmpRealCrc = (quint8)mCurFrame[pType].at(tmpCrcIndex);
                        quint8 tmpCrc = 0;
                        //校验
#ifdef SPI
                        for(int m=1;m<tmpCrcIndex;m++)
                        {
                            quint8 tmpData = (quint8)mCurFrame[pType].at(m);
                            tmpCrc += tmpData;
                        }
                        tmpCrc = tmpCrc & 0x7f;
#else
                        tmpCrc = tmpRealCrc;
#endif
                        if(tmpCrc == tmpRealCrc)
                        {
                            quint8 tmpID = (quint8)mCurFrame[pType].at(1);
                            if(mDebugLabel)
                                mDebugLabel->setText(QString("ID[%1]").arg(tmpID));
                            switch(tmpID)
                            {
                                case D_ID_ELE:  //>@楼层帧
                                {
                                    if(mCurFrame[pType].count() == 11)
                                        ParseFloor();
                                    break;
                                }
                                case D_ID_ELE1:  //>@楼层帧
                                {
                                    if(mCurFrame[pType].count() == 11)
                                        ParseFloor();
                                    break;
                                }
                                case D_ID_BTN:  //>@按钮点灯信号帧
                                {
                                    if(mCurFrame[pType].count() == 11)
                                        ParseButton();
                                    break;
                                }
                                case D_ID_REG:  //>@按钮点灯信号帧
                                {
                                    ParseReg(pType);
                                    break;
                                }
                                default:
                                    break;
                            }
                        }
                        else
                        {
                            //IDE_TRACE_INT(tmpRealCrc);
                            //IDE_TRACE_INT(tmpCrc);
                        }
                        mCurFrame[pType].clear();
                        m_FrameState = State_Wait;
                        i++;
                        break;
                    }
                }
                break;
            }
            default:
                m_FrameState = State_Wait;
                i++;
                break;
        }
    }
    pArray.clear();
}

void PluginWidget::slot_ServerReadyRead()
{
    if(mServerTcp && mServerTcp->bytesAvailable())
    {
        QByteArray tmpBufferData = mServerTcp->readAll();
        ProcessFrame(D_BUF_TYPE_WIFI, tmpBufferData);
    }
}

void PluginWidget::slot_TcpReadyRead()
{
    if(mEternetTcp && mEternetTcp->bytesAvailable())
    {
        QByteArray tmpBufferData = mEternetTcp->readAll();
        ProcessFrame(D_BUF_TYPE_SERVER, tmpBufferData);
    }
}

void PluginWidget::slot_ProcessData()
{
    if(mBusy)
        return;
    mBusy = true;
    if(m_SerialPort && m_SerialPort->bytesAvailable())
    {
        QByteArray tmpBufferData = m_SerialPort->readAll();
        m_SerialPort->flush();
        ProcessFrame(D_BUF_TYPE_COM, tmpBufferData);
    }
    mBusy = false;
}

void PluginWidget::InitFlrBtn()
{
    connect(&mBtnPressSigMap, SIGNAL(mapped(int)), this, SLOT(slot_BtnPress(int)));
    connect(&mBtnReleaseSigMap, SIGNAL(mapped(int)), this, SLOT(slot_BtnRelease(int)));

    QList<int> tmpBtnList = mFlrBtnList.keys();
    for(int i=0;i<tmpBtnList.count();i++)
    {
        int tmpKey = tmpBtnList.at(i);
        WidgetButton* tmpBtn = mFlrBtnList.value(tmpKey);
        if(tmpBtn)
        {
            mBtnPressSigMap.setMapping(tmpBtn, tmpKey);
            connect(tmpBtn, SIGNAL(sPressed()), &mBtnPressSigMap, SLOT(map()));

            mBtnReleaseSigMap.setMapping(tmpBtn, tmpKey);
            connect(tmpBtn, SIGNAL(sReleased()), &mBtnReleaseSigMap, SLOT(map()));

            if(m_AutoTest)
                tmpBtn->m_BtnType = 1;

            connect(this, SIGNAL(sButtonRelease(int)), this, SLOT(slot_ButtonRelease(int)), Qt::QueuedConnection);
        }
    }
}


void PluginWidget::InitKeyBtn()
{
    connect(&mKeyBtnPressSigMap, SIGNAL(mapped(int)), this, SLOT(slot_KeyBtnPress(int)));
    connect(&mKeyBtnReleaseSigMap, SIGNAL(mapped(int)), this, SLOT(slot_KeyBtnRelease(int)));

    QList<int> tmpBtnList = mKeyBtnList.keys();
    for(int i=0;i<tmpBtnList.count();i++)
    {
        int tmpKey = tmpBtnList.at(i);
        WidgetButton* tmpBtn = mKeyBtnList.value(tmpKey);
        if(tmpBtn)
        {
            mKeyBtnPressSigMap.setMapping(tmpBtn, tmpKey);
            connect(tmpBtn, SIGNAL(sPressed()), &mKeyBtnPressSigMap, SLOT(map()));

            mKeyBtnReleaseSigMap.setMapping(tmpBtn, tmpKey);
            connect(tmpBtn, SIGNAL(sReleased()), &mKeyBtnReleaseSigMap, SLOT(map()));
        }
    }
}

void PluginWidget::slot_KeyBtnPress(int id)
{
    if(mKeyEchoLCD)
    {
        QString tmpString = mKeyEchoLCD->text();
        if(tmpString.compare("ERR") == 0)
            mKeyEchoLCD->clear();

        if(id >= 48 && id <= 57)
        {
            QString tmpString = mKeyEchoLCD->text();
            if(!tmpString.isEmpty())
            {
                char tmpHighLetter = tmpString.at(0).toAscii();
                if(tmpHighLetter >= 65 && tmpHighLetter <= 90)
                {
                    if(tmpString.count() >= 4)
                        return;
                }
                else
                {
                    if(tmpString.count() >= 3)
                        return;
                }
            }
            tmpString.append((char)(id));
            mKeyEchoLCD->setText(tmpString);
        }
        else if(id >= 65 && id <= 90)
        {
            QString tmpString = mKeyEchoLCD->text();
            if(!tmpString.isEmpty())
            {
                char tmpHighLetter = tmpString.at(0).toAscii();
                if(tmpHighLetter >= 65 && tmpHighLetter <= 90)
                {
                    tmpString = tmpString.mid(1);
                }
            }
            tmpString.prepend((char)(id));
            mKeyEchoLCD->setText(tmpString);
        }
        else if(id == 8) //>@DEL
        {
            QString tmpString = mKeyEchoLCD->text();
            mKeyEchoLCD->setText(tmpString.mid(0, tmpString.count()-1));
        }
        else if(id == 13)
        {
            QString tmpString = mKeyEchoLCD->text();
            slot_KeyBtnConfirm(tmpString);
        }
    }
}

quint8 GetCrc(QByteArray pFrame)
{
    quint8 tmpCrc = 0;
    for(int i=0;i<pFrame.count();i++)
        tmpCrc += pFrame.at(i);
    return (tmpCrc&0x7f);
}

void PluginWidget::slot_KeyBtnConfirm(QString pNum)
{
    if(pNum.isEmpty())
        return;
    if(m_SerialPort && m_SerialPort->isOpen())
    {
        bool ok = false;
        int tmpNum = pNum.toInt(&ok);
        if(!ok)
        {
            mKeyEchoLCD->setText("ERR");
            return;
        }
        if(tmpNum < m_MinFlr || tmpNum > m_MaxFlr)
        {
            mKeyEchoLCD->setText("ERR");
            return;
        }
        mKeyEchoLCD->clear();

        tmpNum -= 1;

        QByteArray tmpArray;
        tmpArray.append((char)0xc0);
        tmpArray.append((char)0x02);
        int k,z,i;
        k=tmpNum/7;
        z=tmpNum%7;
        quint8 Zero=0x00;
        for(i=0;i<7;i++)
        {
            if(k!=i)
            {
                tmpArray.append(Zero);
            }
            else
            {
                tmpArray.append((quint8)(1<<z));
            }
        }
        tmpArray.append((char)GetCrc(tmpArray.mid(1)));
        tmpArray.append((char)0xc1);
        m_SerialPort->write(tmpArray);

        QtSleep(100);

        tmpArray.clear();
        tmpArray.append((char)0xc0);
        tmpArray.append((char)0x02);
        tmpArray.append((char)0x00);
        tmpArray.append((char)0x00);
        tmpArray.append((char)0x00);
        tmpArray.append((char)0x00);
        tmpArray.append((char)0x00);
        tmpArray.append((char)0x00);
        tmpArray.append((char)0x00);
        tmpArray.append((char)GetCrc(tmpArray.mid(1)));
        tmpArray.append((char)0xc1);
        m_SerialPort->write(tmpArray);
    }
}

void PluginWidget::slot_KeyBtnRelease(int id)
{

}

void PluginWidget::slot_ButtonRelease(int id)
{
    if(!mFlrBtnList.isEmpty())
    {
        WidgetButton* tmpBtn = mFlrBtnList.value(id);
        if(tmpBtn)
        {
            tmpBtn->slot_ExecOperate(OPERATE_KEYRELEASE, QVariant());
        }
    }
}

void PluginWidget::ParseFloor(int pType)
{
    QString tmpDebug = "Recv[ ";
    for(int i=0;i<11;i++)
    {
        tmpDebug.append(QString("%1 ").arg(mCurFrame[pType][i]&0x000000ff));
    }
    tmpDebug.append("]\r\n");
    IDE_DEBUG(tmpDebug);
}

void PluginWidget::slot_RegisteFlr(int pFlrNum, int pIndex)
{
    if(!mRegistedFlrs.isEmpty())
    {
        if(pIndex < mRegistedFlrs.count()-1)
        {
            QLabel *tmpLabel = mRegistedFlrs.at(pIndex);
            if(tmpLabel)
                tmpLabel->setText(QString("%1").arg(pFlrNum));
        }
    }
}

QByteArray PluginWidget::ReadReg(int pAddr)
{

}

bool PluginWidget::WriteReg(int pAddr, QByteArray pValue)
{

}

void PluginWidget::ParseReg(int pType)
{
    if(m_FrameFlag == 0) //>@主机发送来的数据
    {
        quint8 tmpValue = mCurFrame[pType].at(2) & 0x01;
        if(tmpValue == 0) //Read
        {
            QByteArray tmpRegData = mCurFrame[pType].mid(3, mCurFrame[pType].count()-5);
            QList<QByteArray> tmpRegList = tmpRegData.split(';');
            QByteArray tmpRetData;
            foreach(QByteArray tmpReg, tmpRegList)
            {
                if(tmpReg.isEmpty())
                    continue;
                bool ok = false;
                int tmpAddr = tmpReg.toInt(&ok, 10);
                if(!ok)
                    continue;
                QByteArray tmpRegValue = ReadReg(tmpAddr);
                if(tmpRegValue.isEmpty())
                    continue;
                tmpRetData.append(tmpReg+':'+tmpRegValue+';');
            }

            QByteArray tmpArray;
            tmpArray.append((char)0xc0);
            tmpArray.append((char)D_ID_REG);
            tmpArray.append((char)0x00);
            tmpArray.append(tmpRetData);
            tmpArray.append((char)GetCrc(tmpArray.mid(1)));
            tmpArray.append((char)0xc1);
            if(pType == D_BUF_TYPE_COM)
            {
                m_SerialPort->write(tmpArray);
#ifdef SPI
                m_SerialPort->write(tmpArray);
                m_SerialPort->write(tmpArray);
                m_SerialPort->write(tmpArray);
                m_SerialPort->write(tmpArray);
#endif
            }
            else if(pType == D_BUF_TYPE_WIFI)
            {
                mServerTcp->write(tmpArray);
            }
            else if(pType == D_BUF_TYPE_SERVER)
            {
                mEternetTcp->write(tmpArray);
            }
        }
        else               //Write
        {
            QByteArray tmpRegData = mCurFrame[pType].mid(3, mCurFrame[pType].count()-5);
            QList<QByteArray> tmpRegList = tmpRegData.split(';');
            foreach(QByteArray tmpReg, tmpRegList)
            {
                if(tmpReg.isEmpty())
                    continue;
                QList<QByteArray> tmpRegValueList = tmpReg.split(':');
                if(tmpRegValueList.count() != 2)
                    continue;
                bool ok = false;
                int tmpAddr = tmpRegValueList.at(0).toInt(&ok, 10);
                if(!ok)
                    continue;
                WriteReg(tmpAddr, tmpRegValueList.at(1));
            }
        }
    }
}

void PluginWidget::ParseButton(int pType)
{
    if(m_FrameFlag == 0)
    {
        int tmpIndex = 0;
        if(!mRegistedFlrs.isEmpty())
        {
            QLabel *tmpLabel;
            foreach(tmpLabel, mRegistedFlrs)
            {
                tmpLabel->clear();
            }
        }

        for(int i=2;i<=8;i++)
        {
            quint8 tmpValue = mCurFrame[pType].at(i) & 0x7f;
            if(m_FilterFrame)
            {
                if(m_BtnBk[i-2] == tmpValue)
                    continue;
            }
            for(int j=0;j<7;j++)
            {
                int tmpNum = (i-2)*7+j+1;
                if(tmpNum <= 42)
                {
                    if(m_MinFlr >= 0 && m_MaxFlr >= 0)
                    {
                        if(tmpNum < m_MinFlr || tmpNum > m_MaxFlr)
                        {
                            continue;
                        }
                    }
                }
                if(tmpValue & (1<<j))
                {
                    slot_BtnLight(tmpNum);
                    slot_RegisteFlr(tmpNum, tmpIndex);
                    tmpIndex++;
                }
                else
                {
                    slot_BtnCancel(tmpNum);
                }
            }
        }
        mRegistedFlrCount = tmpIndex;
    }
}

void PluginWidget::slot_BtnLight(int id)
{
    WidgetButton *tmpButton = mFlrBtnList.value(id);
    if(tmpButton)
    {
        IDE_TRACE_INT(id);
        tmpButton->slot_ExecOperate(OPERATE_KEYLIGHT, QVariant());
    }
}

void PluginWidget::slot_BtnCancel(int id)
{
    WidgetButton *tmpButton = mFlrBtnList.value(id);
    if(tmpButton)
    {
        IDE_TRACE_INT(id);
        tmpButton->slot_ExecOperate(OPERATE_KEYCANCEL, QVariant());
    }
}

void PluginWidget::slot_BtnPress(int id)
{
    if(m_AutoTest)
    {
        WidgetButton* tmpBtn = mFlrBtnList.value(id);
        if(tmpBtn)
        {
            tmpBtn->slot_ExecOperate(OPERATE_KEYPRESS, QVariant());
        }
    }
    else
    {
        int tmpNum = id - 1;
        if(tmpNum >= 0)
        {
            if(m_SerialPort && m_SerialPort->isOpen())
            {
                QByteArray tmpArray;
                tmpArray.append((char)0xc0);
                tmpArray.append((char)0x02);
                int k,z,i;
                k=tmpNum/7;
                z=tmpNum%7;
                quint8 Zero=0x00;
                for(i=0;i<7;i++)
                {
                    if(k!=i)
                    {
                        tmpArray.append(Zero);
                    }
                    else
                    {
                        tmpArray.append((quint8)(1<<z));
                    }
                }
                tmpArray.append((char)GetCrc(tmpArray.mid(1)));
                tmpArray.append((char)0xc1);
                m_SerialPort->write(tmpArray);
#ifdef SPI
                m_SerialPort->write(tmpArray);
                m_SerialPort->write(tmpArray);
                m_SerialPort->write(tmpArray);
                m_SerialPort->write(tmpArray);
#endif
            }
        }
    }
}

void PluginWidget::slot_BtnRelease(int id)
{
    if(m_AutoTest)
    {
        if(id >= 43)
        {
            WidgetButton* tmpBtn = mFlrBtnList.value(id);
            if(tmpBtn)
            {
                tmpBtn->slot_ExecOperate(OPERATE_KEYRELEASE, QVariant());
            }
        }
    }
    else
    {
        if(m_SerialPort && m_SerialPort->isOpen())
        {
            QByteArray tmpArray;
            tmpArray.append((char)0xc0);
            tmpArray.append((char)0x02);
            tmpArray.append((char)0x00);
            tmpArray.append((char)0x00);
            tmpArray.append((char)0x00);
            tmpArray.append((char)0x00);
            tmpArray.append((char)0x00);
            tmpArray.append((char)0x00);
            tmpArray.append((char)0x00);
            tmpArray.append((char)GetCrc(tmpArray.mid(1)));
            tmpArray.append((char)0xc1);
            m_SerialPort->write(tmpArray);
#ifdef SPI
            m_SerialPort->write(tmpArray);
            m_SerialPort->write(tmpArray);
            m_SerialPort->write(tmpArray);
            m_SerialPort->write(tmpArray);
#endif
        }
    }
}

void PluginWidget::slot_DateTime(AREA_OPERATE pOperate, QVariant pPara)
{

}

void PluginWidget::slot_Tick()
{
    if(m_AutoTest)
    {
        static int floor = 0;
        static bool direction = true;
        if(m_Ticks%2 == 0)
        {
            if(direction)
            {
                if(floor==m_MaxFlr)
                {
                    floor = m_MaxFlr-1;
                    emit sArrow(OPERATE_CHANGE, ArrowDownroll);
                    direction=false;
                }
                else
                    floor++;
                FLOOR_INFO tmpFloor((quint8)(floor/100 + 48), (quint8)(floor%100/10 + 48), (quint8)(floor%10+48));
                emit sFloor(OPERATE_CHANGE, tmpFloor.toInt());
                emit sButtonRelease(tmpFloor.toIndex());
                FloorChanged(&tmpFloor);

                mAudio->Play(m_RcPath + QString("Media/M%1.wav").arg(tmpFloor.toIndex()));
            }
            else
            {
                if(floor==m_MinFlr)
                {
                    floor = m_MinFlr+1;
                    emit sArrow(OPERATE_CHANGE, ArrowUproll);
                    direction=true;
                }
                else
                    floor--;
                FLOOR_INFO tmpFloor((quint8)(floor/100 + 48), (quint8)(floor%100/10 + 48), (quint8)(floor%10+48));
                emit sFloor(OPERATE_CHANGE, tmpFloor.toInt());
                emit sButtonRelease(tmpFloor.toIndex());
                FloorChanged(&tmpFloor);

                mAudio->Play(m_RcPath + QString("Media/M%1.wav").arg(tmpFloor.toIndex()));
            }
        }
    }

    if(mEternetTcp && (!mEternetTcp->isConnected()) && !m_ServerIP.isNull() && m_ServerPort >= 60000)
        emit sTcpConnect(m_ServerIP, m_ServerPort);

    m_Ticks++;
}

//Q_EXPORT_PLUGIN2(pluginwidget, PluginWidget)
