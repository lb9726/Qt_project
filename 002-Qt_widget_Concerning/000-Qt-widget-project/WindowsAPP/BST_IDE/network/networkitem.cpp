#include "networkitem.h"


NetworkItem::NetworkItem(QString pGUID, QString pSSID, QObject *parent) :
    QObject(parent)
{
    mSSID = pSSID;
    mSecureEn = false;
    mGuid = pGUID;
}

NetworkItem::NetworkItem(const QNetworkConfiguration &pConfig, QObject *parent) :
    QObject(parent)
{
    mConfiguration = pConfig;
    mSession = new QNetworkSession(mConfiguration, this);

    connect(mSession, SIGNAL(newConfigurationActivated()), this, SLOT(newConfigurationActivated()));
    connect(mSession, SIGNAL(stateChanged(QNetworkSession::State)), this, SLOT(stateChanged(QNetworkSession::State)));

    mSSID = mConfiguration.name();
    mSecureEn = false;
    mGuid = GetGUID();
}

NetworkItem::~NetworkItem()
{
    Disconnect();
    //mSession->deleteLater();
}

/*
    使用Qt也可以获取GUID,但此方法只能查看已经建立连接的有线或者无线网卡的GUID。
#ifndef QT_NO_NETWORKINTERFACE
    const QNetworkInterface tmpInterface = mSession->interface();
    if(!tmpInterface.isValid())
        return false;
    mGuid = tmpInterface.name();  //>@如果是linux，则返回eth0之类的。
#endif
*/
QString NetworkItem::GetGUID()
{
    QString tmpGuid;
#ifdef SURRPORT_WINSDK
    QString tmpStdOut, tmpStdErr;
    QString tmpExe = GlobalPara.m_SoftwareDir + QString("ETC/Wlsample.exe");
    QString tmpCommand = QString("\"%1\" ei").arg(tmpExe);
    ExecProcess(tmpCommand, tmpStdOut, tmpStdErr);
    int index = 0;
    int start = 0;
    QStringList tmpGuidList;
    while((index = tmpStdOut.indexOf("GUID: ", start)) >= 0)
    {
        tmpGuidList.append(tmpStdOut.mid(index+6, 36));
        start = index+6+36;
    }
    if(!tmpGuidList.isEmpty())
        tmpGuid = tmpGuidList.first();
#endif
    return tmpGuid;
}

bool NetworkItem::isConnected()
{
    if(mGuid.isEmpty())
        mGuid = GetGUID();
    if(mGuid.isEmpty())
        return false;
#ifdef SURRPORT_WINSDK
    QString tmpExe = GlobalPara.m_SoftwareDir + QString("ETC/Wlsample.exe");
    QString tmpSubCommand = QString("\"%1\" qi %2").arg(tmpExe).arg(mGuid);
    QString tmpStdOut, tmpStdErr;
    ExecProcess(tmpSubCommand, tmpStdOut, tmpStdErr);
    int index1 = tmpStdOut.indexOf("Interface state:");
    if(index1 < 0)
        return false;
    int index2 = tmpStdOut.indexOf("\".", index1+18);
    if(index2 < 0)
        return false;
    QString tmpState = tmpStdOut.mid(index1+18, index2 - index1 - 18);
    if(tmpState.isEmpty())
        return false;
    if(!tmpState.compare("connected", Qt::CaseInsensitive))
        return true;
#endif
    return false;
}

bool NetworkItem::Connect(int pTimeout)
{
    if(isConnected())
    {
#if 1
        if(!Disconnect(2000))
        {
            IDE_DEBUG("Can't disconnect ssid!");
            return false;
        }
#else
        return true;
#endif
    }

    if(mGuid.isEmpty())
        mGuid = GetGUID();
    if(mGuid.isEmpty())
        return false;

#ifdef SURRPORT_WINSDK
    QString tmpExe = GlobalPara.m_SoftwareDir + QString("ETC/Wlsample.exe");
    QString tmpSubCommand = QString("\"%1\" disc %2 %3 i %4")
                            .arg(tmpExe)
                            .arg(mGuid)
                            .arg(mSSID)/*SSID*/
                            .arg((mSecureEn?QString("s"):QString("u")));
    IDE_TRACE_STR(tmpSubCommand);
    QString tmpStdOut, tmpStdErr;
    ExecProcess(tmpSubCommand, tmpStdOut, tmpStdErr);
#endif
    while(pTimeout -= 100)
    {
        QtSleep(100);
        if(isConnected())
            return true;
    }
    return false;
}

bool NetworkItem::Disconnect(int pTimeout)
{
    if(!isConnected())
        return true;

    if(mGuid.isEmpty())
        mGuid = GetGUID();
    if(mGuid.isEmpty())
        return false;

#ifdef SURRPORT_WINSDK
    QString tmpExe = GlobalPara.m_SoftwareDir + QString("ETC/Wlsample.exe");
    QString tmpSubCommand = QString("\"%1\" dc %2").arg(tmpExe).arg(mGuid);
    QString tmpStdOut, tmpStdErr;
    ExecProcess(tmpSubCommand, tmpStdOut, tmpStdErr);
#endif
    while(pTimeout -= 100)
    {
        QtSleep(100);
        if(!isConnected())
            return true;
    }
    return false;
}

void NetworkItem::stateChanged(QNetworkSession::State state)
{
    mTooltip.clear();

    if (mConfiguration.name().isEmpty())
        mTooltip += tr("<b>HIDDEN NETWORK</b><br>");
    else
        mTooltip += tr("<b>%1</b><br>").arg(mConfiguration.name());

#ifndef QT_NO_NETWORKINTERFACE
    const QNetworkInterface tmpInterface = mSession->interface();
    if (tmpInterface.isValid())
    {
        IDE_TRACE_STR(tmpInterface.humanReadableName());
        mTooltip += tr("<br>Interface: %1").arg(tmpInterface.humanReadableName());
    }
    mTooltip += tr("<br>Id: %1").arg(mConfiguration.identifier());
#endif

    const QString bearerTypeName = mConfiguration.bearerTypeName();
    if (!bearerTypeName.isEmpty())
        mTooltip += tr("<br>Bearer: %1").arg(bearerTypeName);

    QString s = tr("<br>State: %1 (%2)");
    switch (state)
    {
    case QNetworkSession::Invalid:
        s = s.arg(tr("Invalid"));
        break;
    case QNetworkSession::NotAvailable:
        s = s.arg(tr("Not Available"));
        break;
    case QNetworkSession::Connecting:
        s = s.arg(tr("Connecting"));
        break;
    case QNetworkSession::Connected:
        s = s.arg(tr("Connected"));
        emit sConnected();
        break;
    case QNetworkSession::Closing:
        s = s.arg(tr("Closing"));
        break;
    case QNetworkSession::Disconnected:
        s = s.arg(tr("Disconnected"));
        emit sDisconnected();
        break;
    case QNetworkSession::Roaming:
        s = s.arg(tr("Roaming"));
        break;
    default:
        s = s.arg(tr("Unknown"));
    }

    if (mSession->isOpen())
        s = s.arg(tr("Open"));
    else
        s = s.arg(tr("Closed"));

    mTooltip += s;

    mTooltip += tr("<br><br>Active time: %1 seconds").arg(mSession->activeTime());
    mTooltip += tr("<br>Received data: %1 bytes").arg(mSession->bytesReceived());
    mTooltip += tr("<br>Sent data: %1 bytes").arg(mSession->bytesWritten());
}

void NetworkItem::newConfigurationActivated()
{
    QNetworkConfiguration::BearerType bearerType = mConfiguration.bearerType();
    switch (bearerType)
    {
    case QNetworkConfiguration::BearerWLAN:
        break;
    case QNetworkConfiguration::BearerEthernet:
        break;
    case QNetworkConfiguration::Bearer2G:
        break;
    case QNetworkConfiguration::BearerBluetooth:
        break;
    case QNetworkConfiguration::BearerCDMA2000:
    case QNetworkConfiguration::BearerWCDMA:
    case QNetworkConfiguration::BearerHSPA:
        break;
    default:
        break;
    }

    stateChanged(mSession->state());
}
