#include "wificommunication.h"
#include <QDebug>
//#include <QString>
//#include <QCoreApplication>

//#include <QJsonDocument>
//#include <QJsonArray>
//#include <QJsonValue>
//#include <QJsonParseError>
//#include <QVariantMap>


#define BVT_STX (0x80)        /* 帧起始字符 */
#define BVT_ETX (0x81)        /* 帧结束字符 */
#define BVT_ESC (0x1B)        /* 转换字符 */

#define BVT_STX_AF (0xE7)     /* 帧起始转换后增加字符 */
#define BVT_ETX_AF (0xE8)     /* 帧结束转换后增加字符 */
#define BVT_ESC_AF (0x00)     /* 转换字符转换后增加字符 */

//#define ID_HEART            (0x01)        /* heartBeat */
//#define ID_LOGIN            (0x02)        /* login */
//#define ID_GETPARA          (0x03)        /* getallparameter */
//#define ID_SENDPARA         (0x04)        /* sendallparameter */
//#define ID_UPDATEFINISH     (0x05)        /* appupdateFinished */
//#define ID_SENDFILE         (0x06)        /* sendFile */
//#define ID_REBOOT           (0x07)        /* reboot */

WifiCommunication::WifiCommunication(int port, QObject *parent) : QObject(parent)
  , mPort(port)
{
    mMyTcpServer = new MyTcpServer(parent, port);
    qDebug()<<"mPort = "<<mPort;
    if (mMyTcpServer)
    {
        connect(mMyTcpServer, &MyTcpServer::myTcpServerRecvMsg, this, &WifiCommunication::parserJsonFormat);
        connect(mMyTcpServer, &MyTcpServer::myTcpServerRecvMsg, this, &WifiCommunication::sigMsgArrive);
        connect(mMyTcpServer, &MyTcpServer::myTcpServerDeviceDisConnect, this, &WifiCommunication::sigDeviceDisconnected);
    }
}

//WifiCommunication::WifiCommunication(int port)
//{

//}

bool WifiCommunication::jsonFormatIsRight(const QByteArray &byteArray)
{
    bool flag = false;
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(byteArray, &jsonError); // 转化为JSON文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) // 解析未发生错误
    {
        flag = true;
    }
    else
    {
        flag = false;
    }
    qDebug()<<__PRETTY_FUNCTION__<<"flag = "<<flag;
    return flag;
}

//https://www.cnblogs.com/senior-engineer/p/5608985.html
void WifiCommunication::parserJsonFormat(const QByteArray &byteArray)
{
    if ((byteArray.isNull() || byteArray.isEmpty()))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"mJsonString is empty!";
        return ;
    }
    if (!jsonFormatIsRight(byteArray))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"the byteArray format is wrong";
        return ;
    }
    QJsonDocument qdoc = QJsonDocument::fromJson(byteArray);
    QJsonObject cmdqobj = qdoc.object();
    QJsonValue value0 = cmdqobj.value(QString("type"));
    QString type = value0.toString();
    qWarning() << type<<"\n";
    if (!QString::compare(type, "login", Qt::CaseInsensitive))
    {
        if (cmdqobj.contains("info"))
        {            
            login(cmdqobj);
        }
        else
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"has no info";
        }
    }
    else if(!QString::compare(type, "heartbeat", Qt::CaseInsensitive))
    {
        if (cmdqobj.contains("info"))
        {

        }
        else
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"has no info";
        }
    }
    else if(!QString::compare(type, "getallparameter", Qt::CaseInsensitive))
    {
        if (cmdqobj.contains("info"))
        {
            getAllParameter();
        }
        else
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"has no info";
        }
    }
    else if(!QString::compare(type, "setparameter", Qt::CaseInsensitive))
    {
        if (cmdqobj.contains("info"))
        {
            setParameter(cmdqobj);
        }
        else
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"has no info";
        }
    }
    else if(!QString::compare(type, "appupdateFinished", Qt::CaseInsensitive))
    {
        if (cmdqobj.contains("info"))
        {
            appUpdateFinished();
        }
        else
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"has no info";
        }
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"cmd is error";
    }
}

// 解析读取到的QByteArray的数据，需要对帧头和帧尾进行处理
void WifiCommunication::parseBuffer(QByteArray &buffer)
{
    mRecvBufferFrame.clear();
    if (judgeArrayIsEmpty(buffer))
    {
        return;
    }  
    char ch = buffer.at(0);
    if ((quint8)ch != BVT_STX) // 帧头
    {
        return;
    }
    ch = buffer.at(buffer.length() - 1);
    if ((quint8)ch != BVT_ETX) // 帧尾
    {
        return;
    }
    int i = buffer.length() - 2; // 去掉帧头，帧尾
    int j = 1;
    while (j < i)
    {
        ch = buffer.at(j);
        if ((quint8)ch == BVT_ESC) // 如果为BVT_ESC则需要判断紧接着的一个字符值
        {
            char chNext = buffer.at(j+1);
            if ((quint8)chNext == BVT_STX_AF)
            {
                mRecvBufferFrame.append(BVT_STX);
                j = j + 1;
            }
            else if ((quint8)chNext == BVT_ETX_AF)
            {
                mRecvBufferFrame.append(BVT_ETX);
                j = j + 1;
            }
            else if ((quint8)chNext == BVT_ESC_AF)
            {
                mRecvBufferFrame.append(BVT_ESC);
                j = j + 1;
            }
            else // 以上都不是的话，此帧error
            {
                mRecvBufferFrame.clear();
                qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"BVT_ESC the after char is error";
                return ;
            }
        }
        else
        {
            mRecvBufferFrame.append(ch);
            j = j + 1;
        }
    }
    int len = mRecvBufferFrame.length();
    char recvCrc = mRecvBufferFrame.at(len - 1);
    // 文件传送前后crc值校验相同
    if (recvCrc == getCrcVerify(mRecvBufferFrame, len - 1))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"mRecvBufferFrame ="<< mRecvBufferFrame;
        mRecvBufferFrame.remove(len - 1, 1); // 去掉最后一个crc校验位
        return;
    }
    else
    {
        mRecvBufferFrame.clear();
        return ;
    }
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"mRecvBufferFrame ="<< mRecvBufferFrame;
    parserJsonFormat(mRecvBufferFrame);
}

// 生成包含帧头帧尾和crc校验位的一帧数据
// QByteArray &buffer 为ID + json格式字符串转化后字符串
// char pID 为数据帧的ID标志
QByteArray& WifiCommunication::generateBuffer(QByteArray &buffer)
{
    mSendBufferFrame.clear();
    int j = 0;
    if (!judgeArrayIsEmpty(buffer))
    {
        return mSendBufferFrame;
    }
    if (j < buffer.length()) // 满足条件先加入帧头
    {
        mSendBufferFrame.append(BVT_STX);
    }

    // 校验位是本数据中不包含帧头和帧尾 但包含ID 的时候计算出来的
    char crc = getCrcVerify(buffer, buffer.length());
    QByteArray addCrcBuffer = buffer.append(crc);
    int i = addCrcBuffer.length();
    char tmp;
    while (j < i)
    {
        tmp = addCrcBuffer.at(j);
        if ((quint8)tmp == BVT_STX)
        {
            mSendBufferFrame.append((char)BVT_ESC);
            mSendBufferFrame.append((char)BVT_STX_AF);
        }
        else if ((quint8)tmp == BVT_ETX)
        {
            mSendBufferFrame.append((char)BVT_ESC);
            mSendBufferFrame.append((char)BVT_ETX_AF);
        }
        else if ((quint8)tmp == BVT_ESC)
        {
            mSendBufferFrame.append((char)BVT_ESC);
            mSendBufferFrame.append((char)BVT_ESC_AF);
        }
        else
        {
            mSendBufferFrame.append(tmp);
        }
        ++j;
    }

    mSendBufferFrame.append((char)BVT_ETX); // 加入帧尾
    qDebug()<<__PRETTY_FUNCTION__<<" mSendBufferFrame = "<<mSendBufferFrame;
    return mSendBufferFrame;
}

char WifiCommunication::getCrcVerify(QByteArray msg, int length)
{
    char crc = 0x00;
    for (int i = 0; i < length; ++i)
    {
        crc ^= msg.at(i);
    }
    return crc & 0x7f;
}

// 判断QByteArray的数据是否为空
bool WifiCommunication::judgeArrayIsEmpty(const QByteArray &buffer)
{
    bool flag = true;
    if (buffer.isNull() || buffer.isEmpty())
    {
        flag = true;
    }
    else
    {
        flag = false;
    }
    qDebug()<<__PRETTY_FUNCTION__<<"flag = "<< flag;
    return flag;
}

QByteArray& WifiCommunication::sendJsonFrame(QJsonObject& msg)
{
    mJson.clear();
    QJsonDocument document;
    document.setObject(msg);
    mJson = document.toJson(QJsonDocument::Compact);
    qDebug()<<__PRETTY_FUNCTION__<<"allPara = "<<mJson;
    qDebug()<<__PRETTY_FUNCTION__<<"the format is " <<jsonFormatIsRight(mJson);
    return mJson;
}

void WifiCommunication::sltloginResult(bool flag)
{
    QJsonObject loginObj;
    QJsonObject info;
    loginObj.insert("type", "login");
    if(flag)
    {
        info.insert("result", true);
    }
    else
    {
        info.insert("result", false);
    }
    loginObj.insert("info", QJsonValue(info));
    QByteArray tmp =  sendJsonFrame(loginObj);
//    tmp.insert(0, (char)ID_LOGIN);
    writeMsgToClient(tmp, tmp.length());
    if (flag)
    {
        emit  sigDeviceConnected();
    }
}

int WifiCommunication::writeMsgToClient(QByteArray msg, int length)
{
    int len = -1;
    if (mMyTcpServer)
    {
        len = mMyTcpServer->writeMsgToClient(msg, length);
    }
    qDebug()<<__PRETTY_FUNCTION__<<"len = "<<len;
    return len;
}

void WifiCommunication::login(QJsonObject &map)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<map;
    QJsonValue infoVal = map["info"];
//    QJsonValue infoVal = cmdqobj.value(QString("info"));
    QJsonObject obj = infoVal.toObject();

    QJsonValue ssidval = obj["ssid"];
    QString ssid = ssidval.toString();
    QJsonValue passwordval = obj["password"];
    QString password = passwordval.toString();

    qDebug()<<__PRETTY_FUNCTION__<<"ssid = "<<ssid<<"passwd = "<<password;
    if(obj.contains(QString("ssid")) && obj.contains(QString("password")))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"ssid = "<<ssid<<"passwd = "<<password;
        emit sigLogin(ssid, password);
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"ssid passwd is lost";
    }
}

void WifiCommunication::getAllParameter()
{
    emit sigGetAllParametere();
}

void WifiCommunication::setParameter(QJsonObject &map)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"map = "<<map;
    QJsonValue infoVal = map["info"];
//    QJsonValue infoVal = cmdqobj.value(QString("info"));
    QJsonObject obj = infoVal.toObject();
    QVariantMap tmpMap =  obj.toVariantMap();
    QMapIterator<QString, QVariant> it(tmpMap);
    while (it.hasNext())
    {
        it.next();
        emit sigParameterSetUp(it.key(), it.value());
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"it.key() = "<<it.key()<<"it.value() = "<<it.value();
    }
}

void WifiCommunication::appUpdateFinished()
{
    emit sigUpdateFinished();
}

void WifiCommunication::heatBeatFrame()
{

}
