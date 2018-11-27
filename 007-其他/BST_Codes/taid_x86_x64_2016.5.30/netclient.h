#ifndef NETCLIENT_H
#define NETCLIENT_H

#include <QHostAddress>
#include <QObject>
#include <QTcpSocket>
/*
用于实现
网络连接状态
服务器登录
获取设备列表
文件传输
升级资源包发布
*/
class NetClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected WRITE setConnected NOTIFY connectedChanged)
public:
    explicit NetClient(QObject *parent = 0);

    void init();
    bool fileTransmit();

    bool connected()const;
    void setConnected(bool status);

    bool writeData(QString msg);
signals:
    void connectedChanged(bool status);

public slots:
    void slt_deviceList();

    void slt_connected();
    void slt_disconnected();
    void dataReceived();
private:
    bool m_isConnected;//网络连接状态
    bool m_hasLogin;
    QTcpSocket *m_tcpSocket;
    QHostAddress m_HostAddress;
};

#endif // NETCLIENT_H
