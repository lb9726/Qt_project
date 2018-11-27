#ifndef APPCLIENT_H
#define APPCLIENT_H


#include <QObject>
#include <QTimer>
#include <QTcpServer>
#include <QMap>
#include <QTcpSocket>

#include "qjson/parser.h"
#include "qjson/serializer.h"
#include "qjson/qobjecthelper.h"

using namespace QJson;

class AppClient : public QObject
{
    Q_OBJECT
public:
    explicit AppClient(QObject *parent = 0);

    void initTcpServer(int port);
    quint32 ParseBuffer(QByteArray &buffer);
    quint32 ParseFrame();

    quint32 login(QVariantMap &map);
    quint32 getallparameter(QVariantMap &map);
    quint32 setparameter(QVariantMap &map);
    quint32 appupdateFinished(QVariantMap &map);

    quint32 heatBeatFrame();
    void sendHB();


    void sendFrame(QByteArray &ba);
    void sendRawMessage(QByteArray &data);
signals:
    void sigAppConnected();
    void sigAppDisconnected();

    void sigLogin(QString ssid,QString passwd);
    void sigParameterSetting(QString name,QVariant value);
    void sigSyncParametere();
    void sigUpdateFinished();
public slots:
    void sltloginResult(bool flag);
    void sendJson(QVariantMap &jsonMap);
    void startWork();
private slots:
    void hbTimerOut();
    void newConnectSlot();
    void readyMessage();
    void clientDisconnected();
public:
    bool isConnected;
    QTcpServer *m_pTcpServer;
    QTcpSocket *m_pTcpSocket;

    quint16  m_tcpPort;
    QByteArray  m_FrameBuffer;

    QTimer  *m_HBTimeoutTimer;
    quint16  m_HBCount;
    Parser  JsonParser;
    Serializer JsonSerializer;
};

#endif // APPCLIENT_H
