#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "global.h"
#include "networkitem.h"

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = 0);
    
    QList<NetworkItem*> GetNetConfs(QNetworkConfiguration::StateFlags pNetState);
    QList<NetworkItem*> GetNetConfs(QNetworkConfiguration::BearerType pNetType);

    NetworkItem* GetNetConf(QNetworkConfiguration::BearerType pNetType);

signals:
    
private Q_SLOTS:
    void configurationAdded(const QNetworkConfiguration &config);
    void configurationRemoved(const QNetworkConfiguration &config);
    void configurationChanged(const QNetworkConfiguration &config);
    void updateConfigurations();
    
public:
    QTimer                                                      mMonitorTimer;
    QNetworkConfigurationManager                                mManager;
    QHash<QString, NetworkItem*>                                mConfigurations;
    QMultiMap<QNetworkConfiguration::StateFlags, QString>       mConfigStates;
    QMultiMap<QNetworkConfiguration::BearerType, QString>       mConfigTypes;
};

QString GetWinWifiSSID();

#endif // NETWORKMANAGER_H
