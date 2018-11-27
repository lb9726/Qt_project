#include "networkmanager.h"

NetworkManager::NetworkManager(QObject *parent) :
    QObject(parent)
{
    connect(&mManager, SIGNAL(configurationAdded(QNetworkConfiguration)),
            this, SLOT(configurationAdded(QNetworkConfiguration)));
    connect(&mManager, SIGNAL(configurationRemoved(QNetworkConfiguration)),
            this, SLOT(configurationRemoved(QNetworkConfiguration)));
    connect(&mManager, SIGNAL(configurationChanged(QNetworkConfiguration)),
            this, SLOT(configurationChanged(QNetworkConfiguration)));

    connect(&mMonitorTimer, SIGNAL(timeout()), this, SLOT(updateConfigurations()));
    mMonitorTimer.setInterval(2000);
    //mMonitorTimer.start();
#ifndef SURRPORT_WINSDK
    updateConfigurations();
#endif
}

QList<NetworkItem*> NetworkManager::GetNetConfs(QNetworkConfiguration::StateFlags pNetState)
{
    updateConfigurations();
    QStringList tmpNetIdentifierList = mConfigStates.values(pNetState);
    QString tmpNetIdentifier;
    QList<NetworkItem*> tmpList;
    foreach(tmpNetIdentifier, tmpNetIdentifierList)
    {
        IDE_TRACE_STR(tmpNetIdentifier);
        tmpList.append(mConfigurations.value(tmpNetIdentifier));
    }
    return tmpList;
}

NetworkItem* NetworkManager::GetNetConf(QNetworkConfiguration::BearerType pNetType)
{
   NetworkItem* tmpNetworkItem = 0;
#ifdef SURRPORT_WINSDK
    if(pNetType == QNetworkConfiguration::BearerWLAN)
    {
        QString tmpGUID = NetworkItem::GetGUID();
        if(tmpGUID.isEmpty())
            return tmpNetworkItem;
        QString tmpExe = GlobalPara.m_SoftwareDir + QString("ETC/Wlsample.exe");
        QString tmpGvlCommand = QString("\"%1\" gvl %2")
                                .arg(tmpExe)
                                .arg(tmpGUID);
        IDE_TRACE_STR(tmpGvlCommand);
        QString tmpStdOut, tmpStdErr;
        ExecProcess(tmpGvlCommand, tmpStdOut, tmpStdErr);
        int index = tmpStdOut.indexOf("SSID: ");;
        while(index >= 0)
        {
            index += 6;
#ifdef WINDOWS
            int index2 = tmpStdOut.indexOf("\r\n", index);
#else
            int index2 = tmpStdOut.indexOf("\n", index);
#endif
            if(index2 < 0)
                break;

            QString tmpSSID = tmpStdOut.mid(index, index2-index);
            IDE_TRACE_STR(tmpSSID);
            tmpNetworkItem = new NetworkItem(tmpGUID, tmpSSID);
            if(tmpNetworkItem)
                return tmpNetworkItem;

            index = tmpStdOut.indexOf("SSID: ", index);
        }
    }
#endif
    return tmpNetworkItem;
}

QList<NetworkItem*> NetworkManager::GetNetConfs(QNetworkConfiguration::BearerType pNetType)
{
    QList<NetworkItem*> tmpList;
#ifdef SURRPORT_WINSDK
    if(pNetType == QNetworkConfiguration::BearerWLAN)
    {
        QString tmpGUID = NetworkItem::GetGUID();
        if(tmpGUID.isEmpty())
            return tmpList;
        QString tmpExe = GlobalPara.m_SoftwareDir + QString("ETC/Wlsample.exe");
        QString tmpGvlCommand = QString("\"%1\" gvl %2")
                                .arg(tmpExe)
                                .arg(tmpGUID);
        IDE_TRACE_STR(tmpGvlCommand);
        QString tmpStdOut, tmpStdErr;
        ExecProcess(tmpGvlCommand, tmpStdOut, tmpStdErr);
        int index = tmpStdOut.indexOf("SSID: ");;
        while(index >= 0)
        {
            index += 6;
#ifdef WINDOWS
            int index2 = tmpStdOut.indexOf("\r\n", index);
#else
            int index2 = tmpStdOut.indexOf("\n", index);
#endif
            if(index2 < 0)
                break;
            QString tmpSSID = tmpStdOut.mid(index, index2-index);
            IDE_TRACE_STR(tmpSSID);
            NetworkItem *tmpItem = new NetworkItem(tmpGUID, tmpSSID);
            tmpList.append(tmpItem);
            index = tmpStdOut.indexOf("SSID: ", index);
        }
    }
#else
    updateConfigurations();
    IDE_TRACE_INT(pNetType);
    QStringList tmpNetIdentifierList = mConfigTypes.values(pNetType);
    IDE_TRACE_INT(tmpNetIdentifierList.count());
    QString tmpNetIdentifier;
    foreach(tmpNetIdentifier, tmpNetIdentifierList)
    {
        IDE_TRACE_STR(tmpNetIdentifier);
        tmpList.append(mConfigurations.value(tmpNetIdentifier));
    }
#endif
    return tmpList;
}

void NetworkManager::configurationAdded(const QNetworkConfiguration &config)
{
    const QNetworkConfiguration::StateFlags state = config.state();
    const QNetworkConfiguration::BearerType type = config.bearerType();

    IDE_TRACE_INT(state);
    IDE_TRACE_INT(type);
    IDE_TRACE_STR(config.identifier());

    mConfigStates.insert(state, config.identifier());
    mConfigTypes.insert(type, config.identifier());
    NetworkItem *item = new NetworkItem(config);
    mConfigurations.insert(config.identifier(), item);
}

void NetworkManager::configurationRemoved(const QNetworkConfiguration &config)
{
    foreach (const QNetworkConfiguration::StateFlags &state, mConfigStates.uniqueKeys())
        mConfigStates.remove(state, config.identifier());

    foreach (const QNetworkConfiguration::BearerType &type, mConfigTypes.uniqueKeys())
        mConfigTypes.remove(type, config.identifier());

    mConfigurations.remove(config.identifier());
}

void NetworkManager::configurationChanged(const QNetworkConfiguration &config)
{
    foreach (const QNetworkConfiguration::StateFlags &state, mConfigStates.uniqueKeys())
        mConfigStates.remove(state, config.identifier());

    mConfigStates.insert(config.state(), config.identifier());

    foreach (const QNetworkConfiguration::BearerType &type, mConfigTypes.uniqueKeys())
        mConfigTypes.remove(type, config.identifier());

    mConfigTypes.insert(config.bearerType(), config.identifier());
}

void NetworkManager::updateConfigurations()
{
    QList<QNetworkConfiguration> allConfigurations = mManager.allConfigurations();

    while (!allConfigurations.isEmpty())
    {
        QtSleep(1);
        configurationAdded(allConfigurations.takeFirst());
    }
}

QString GetWinWifiSSID()
{
    NetworkManager tmpManager;
    NetworkItem* tmpItem = tmpManager.GetNetConf(QNetworkConfiguration::BearerWLAN);
    if(tmpItem)
    {
        return tmpItem->mSSID;
    }
    return QString();
}
