#include "timeserver.h"
#include "timethread.h"
#include "dialog.h"
#include <QDebug>
TimeServer::TimeServer(QObject *parent)
    :QTcpServer(parent)
{
    dlg =(Dialog *)parent;
    qDebug()<< __PRETTY_FUNCTION__ << "new a object";
}

// qt5 之后的这个函数的参数不能写成int，需要改成 qintptr 才能调用成功
// 因为虚函数的调用，必须是，函数返回值，函数名，参数列表完全相同才能正确调用子类重写的函数
void TimeServer::incomingConnection(qintptr socketDescriptor)
{
    TimeThread *thread = new TimeThread(socketDescriptor, 0);

    connect(thread, SIGNAL(finished()), dlg, SLOT(slotShow()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()), Qt::DirectConnection);

    qDebug()<< __PRETTY_FUNCTION__ << "is start";
    thread->start();
}
