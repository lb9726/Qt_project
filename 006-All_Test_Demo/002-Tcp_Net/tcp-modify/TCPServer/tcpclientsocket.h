#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include <QTcpSocket>

//该函数主要作用是建立TCP套接字 ,用于跟Client通信

class TCPClientSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TCPClientSocket(QObject *parent = 0);

signals:
    void updataClients(QString,int);
    void disconnected(int);

public slots:
    void dataReceived();
    void slotDisconnected();

};

#endif // TCPCLIENTSOCKET_H
