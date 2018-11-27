#include "getcomputerinfos.h"
#include <QHostInfo>
#include <QList>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QDebug>

GetComputerInfos::GetComputerInfos(QObject *parent) : QObject(parent)
{

}

// 获取本机IP can't work
QString GetComputerInfos::getIp()
{
    QString ipAddr;
#if 0
    QList<QHostAddress> AddressList = QNetworkInterface::allAddresses();
    foreach(QHostAddress address, AddressList){
        if(address.protocol() == QAbstractSocket::IPv4Protocol &&
                address != QHostAddress::Null &&
                address != QHostAddress::LocalHost){
            if (address.toString().contains("127.0.")){
                continue;
            }
            ipAddr = address.toString();
            break;
        }
    }
#else
    QString localHost = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localHost);
    //    info.addresses();//QHostInfo的address函数获取本机ip地址
    //如果存在多条ip地址ipv4和ipv6：
    foreach(QHostAddress address,info.addresses())
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
        {//只取ipv4协议的地址
            qDebug()<<address.toString();
            ipAddr = address.toString();
        }
    }
#endif
    qDebug()<<"ipAddr  = "<<ipAddr;
    return ipAddr;
}

// work ok
QString GetComputerInfos::getIp2()
{
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost
            && ipAddressesList.at(i).toIPv4Address())
        {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
    {
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }
    qDebug()<<"ipString = "<<ipAddress;
    return ipAddress;
}

// work ok can acquire IP address
QString GetComputerInfos::getMachineIp()
{
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost
            && ipAddressesList.at(i).toIPv4Address())
        {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
    {
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }
    qDebug()<<"ipAddress = "<<ipAddress;
    return ipAddress;
}

