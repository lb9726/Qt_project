#ifndef TOOLTCP_H
#define TOOLTCP_H

#include "global.h"
#include <QNetworkInterface>
#include <QTcpServer>
#include <QTcpSocket>

typedef QTcpSocket* TcpSocketPtr;
Q_DECLARE_METATYPE(TcpSocketPtr)
inline uint qHash(TcpSocketPtr pPtr) {  return (uint)pPtr; }

#if 1
class ToolTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    ToolTcpServer(quint16 pPort = D_NMPORT, QObject *parent = 0);
    ~ToolTcpServer();

    bool Open(QHostAddress pIP, quint16 pPort);
    void Close();

    void incomingConnection(int handle);

signals:
    void sNewSocket(QTcpSocket *pSocket);
};
#else
class ToolTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    ToolTcpServer(quint16 pPort = D_NMPORT, QObject *parent = 0);
    ~ToolTcpServer();

    bool Open(QHostAddress pIP, quint16 pPort);
    void Close();
    qint64 Write(uint pType, QByteArray data);

    bool Abort(TcpSocketPtr pSocket);
    bool AbortAll();

    void incomingConnection(int handle);

    uint GetType(QTcpSocket *pSocket);
    bool SetType(QTcpSocket *pSocket, uint pType);
    bool Remove(QTcpSocket *pSocket, bool pDel=true);

    bool isNull();
    bool isConnected(QTcpSocket* pSocket);

signals:
    void readyRead(QTcpSocket *pSocket);
    void sDisconnected(QHostAddress pAddr, quint16 pPort);
    void sNewSocket(QTcpSocket *pSocket);

public slots:
    void slot_Disconnected();
    void slot_AbortAll();

public:
    QList<TcpSocketPtr>            m_UndefClients; //>@没有功能定义的连接
    QMultiHash<uint, TcpSocketPtr> m_Clients;      //>@客户机句柄 uint表示具体应用，一种应用允许多个客户机
};
#endif

class ToolTcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    ToolTcpClient(uint pType = 0, QObject *parent = 0);
    ~ToolTcpClient();

    bool Open(QHostAddress pIP, quint16 pPort, quint32 pTimeout);
    void Open(QHostAddress pIP, quint16 pPort);
    void Close();
    bool isConnected();

    int GetConnectedTime();

public slots:
    void slot_Connected();
    void slot_Disconnected();

signals:
    void sConnState(bool pState);

public:
    QDateTime       mConnectTime; //>@用于记录已建立连接的时间
    uint            mType;        //>@标示此客户机的用途，帧格式(4个字节):uint
};

#endif // NODETCP_H

