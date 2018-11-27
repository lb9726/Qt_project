#ifndef TCPMODULE_H
#define TCPMODULE_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>

class MyTcpSocket;

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = 0, int port = 8010);
    ~MyTcpServer();
    int writeMsgToClient(QByteArray msg, int length);

signals:
    void myTcpServerRecvMsg(QByteArray msg, int length);
    void myTcpServerDeviceDisConnect();

public slots:
    void slotMyTcpServerDealWithMsg(QByteArray msg, int length);
    void slotMyTcpServerDisconnected(int descriptor);

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    bool mHasConnectFlag;
    MyTcpSocket* mMyTcpSocket;
};

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyTcpSocket(QObject *parent = 0);
    ~MyTcpSocket();
signals:
    void myTcpSocketRecvMsg(QByteArray msg, int length);
    void myTcpSocketDisconnected(int);

protected slots:
    void slotMyTcpSocketDataReceived();
    void slotMyTcpSocketDisconnected();
};

#endif // TCPMODULE_H
