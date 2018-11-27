#ifndef TCPMODULE_H
#define TCPMODULE_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include "jsonparse.h"

class MyTcpSocket;

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = 0, int port = 0);
    ~MyTcpServer();
    void writeMsgToClient(QByteArray msg, int length);

signals:
    void myTcpServerSendMsgToScreen(QByteArray msg, int length);

public slots:
    void slotMyTcpServerDealWithMsg(QByteArray msg, int length);
    void slotMyTcpServerDisconnected(int);

protected:
    void incomingConnection(qintptr socketDescriptor);
private:
    bool mHasConnectFlag;
    MyTcpSocket* mMyTcpSocket;
    JsonParse mJsonParse;
};

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyTcpSocket(QObject *parent = 0);

signals:
    void myTcpSocketRecvMsg(QByteArray msg, int length);
    void myTcpSocketDisconnected(int);

protected slots:
    void slotMyTcpSocketDataReceived();
    void slotMyTcpSocketDisconnected();
};

#endif // TCPMODULE_H
