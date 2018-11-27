#include "timethread.h"
#include <QDateTime>
#include <QByteArray>
#include <QDataStream>
#include <QDebug>
TimeThread::TimeThread(int socketDescriptor,QObject *parent)
    :QThread(parent), socketDescriptor(socketDescriptor)
{
    qDebug()<< __PRETTY_FUNCTION__ << "new a TimeThread";
}

void TimeThread::run()
{
    QTcpSocket tcpSocket;       // 创建一个QTcpSocket类
    if(!tcpSocket.setSocketDescriptor(socketDescriptor))
    {
        emit error(tcpSocket.error());
        return;
    }
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_3);

    uint time2u = QDateTime::currentDateTime().toTime_t();
    out<<time2u;
    qDebug() << "time2u = " << time2u;
    tcpSocket.write(block);           // 将获得的当前时间传回客户端
    tcpSocket.disconnectFromHost();   // 断开连接
    tcpSocket.waitForDisconnected();  // 等待返回
}
