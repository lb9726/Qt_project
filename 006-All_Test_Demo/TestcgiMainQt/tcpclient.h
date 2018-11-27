#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QHostAddress>
#include <QTcpSocket>
#include <QByteArray>

class TcpClient : public QObject
{
    Q_OBJECT

public:
    explicit TcpClient(QObject *parent = 0);
    ~TcpClient();
    int writeMsgToServer(QByteArray msg, int length);
    void connectToServer();
    QByteArray& generateBuffer(QByteArray& buffer);
    void setPort(int port);
    void setIpString(const QString &ipString);

signals:

public slots:

private:
    bool judgeArrayIsEmpty(const QByteArray& buffer);
    char getCrcVerify(QByteArray msg, int length);

private:
    int mPort;
    QString mIpString;
    QHostAddress mServerIP;
    QTcpSocket mTcpSocket;
    QByteArray mSendBufferFrame;
};

#endif // TCPCLIENT_H
