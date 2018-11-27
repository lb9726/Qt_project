#ifndef NETWORKITEM_H
#define NETWORKITEM_H

#include "global.h"
#include <QNetworkConfiguration>
#include <QNetworkSession>
#include <QNetworkInterface>

inline QNetworkInterface GetNetcard(QString pNetCardName)
{
    QNetworkInterface tmpWLAN = QNetworkInterface::interfaceFromName(pNetCardName);
    if(tmpWLAN.isValid() && tmpWLAN.flags().testFlag(QNetworkInterface::IsUp))
        IDE_DEBUG("Connection found");
    else
        IDE_DEBUG("No connection found on retry, retrying again");
    return tmpWLAN;
}

class NetworkItem : public QObject
{
    Q_OBJECT
public:
    NetworkItem(QString pGUID, QString pSSID, QObject *parent = 0);
    NetworkItem(const QNetworkConfiguration &pConfig, QObject *parent = 0);
    ~NetworkItem();

    QNetworkSession::State GetState() { return mSession->state();}
    QString GetTooltip() {return mTooltip;}
    bool isWLAN()  {return (mConfiguration.bearerType() == QNetworkConfiguration::BearerWLAN); }

private Q_SLOTS:
    void stateChanged(QNetworkSession::State state);
    void newConfigurationActivated();

public:
    QNetworkConfiguration mConfiguration;
    QNetworkSession      *mSession;
    QString               mTooltip;

    void SetSSID(QString pSSID) {mSSID = pSSID;}

public:
    bool isConnected();
    bool Connect(int pTimeout = 3000);  //>@开启此网络，如果是无线网，则连接此SSID
    bool Disconnect(int pTimeout = 1000);
    static QString GetGUID();

    void SetSecure(bool pEnable) {mSecureEn = pEnable;}
    bool isSecure() {return mSecureEn;}

signals:
    void sConnected();
    void sDisconnected();

public:
    QString               mSSID;
    bool                  mSecureEn;
    QString               mGuid;
};

#endif // NETWORKITEM_H

