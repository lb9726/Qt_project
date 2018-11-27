#ifndef TOOLUDP_H
#define TOOLUDP_H

#include "global.h"
#include <QUdpSocket>
#include <QHostInfo>

//>@如果是BST_IDE与BST_DISPLAY都在本机上进行调试，则不能屏蔽下面的宏定义，否则本机无法搜索本机上的DISPLAY
#define NOUDPLOCALFLITER  //>@# UDP本地滤波，滤除反射信号

class ToolUdp : public QIODevice
{
    Q_OBJECT
public:
    ToolUdp(QObject *parent = 0);
    ~ToolUdp();

    bool OpenUdp(quint16 pPort);
    void Close();
    bool isOpen();
    bool IsLocalIP(QHostAddress pIP);
    void UpdateBroadcastAddresses();

    qint64 readData(char *data, qint64 maxlen) {return -1;}
    qint64 writeData(const char *data, qint64 len) {return -1;}

    qint64 write(QByteArray data, QHostAddress host);
    qint64 write(const QByteArray &data);   //>@广播发送
    qint64 reply(const QByteArray &data);
    qint64 echo();
    QByteArray readAll();

public:
    quint16             mPort;
    QUdpSocket*         mUdpSocket;
    QList<QHostAddress> mBroadcastIpAddrs;

    QByteArray          m_RevByteArray;
    QHostAddress        m_SenderHost; //>@当前数据的发送方的IP
    quint16             m_SenderPort; //>@当前数据的发送方的PORT
};

#endif // NODEUDP_H
