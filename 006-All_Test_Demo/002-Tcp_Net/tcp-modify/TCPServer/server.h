#ifndef SERVER_H
#define SERVER_H

//该函数主要作用是实现TCP协议的服务器
#include <QTcpServer>
#include <QObject>
#include "tcpclientsocket.h"
#include <QTextCodec>

class Server : public QTcpServer
{
    Q_OBJECT
public:
 //   explicit Server(QObject *parent = 0);
    Server(QObject *parent = 0,int port = 0);
    QList<TCPClientSocket *> tcpClientSocketList;//建立一个TCPClientSocket列表，用于存储客户端连接的TCPClientSocket
    QTextCodec *codec;

signals:
    void updataServer(QString,int);


public slots:
    void updataClients(QString,int);
    void slotDisconnected(int);

protected:
    void incomingConnection(qintptr socketDescriptor);
};

#endif // SERVER_H
