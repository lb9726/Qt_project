#include "wificommunication.h"
#include <QDebug>
#include <QFile>
#include "common/define.h"

const QString putClientPath = QString("/usr/bst/usrfs/Ftp.zip");
const QString putServerPath = QString("/home/ftp/pub/Ftp.zip");
const QString getServerSrcPath = QString("/home/ftp/pub/Ftp.zip");
const QString getClientDestPath = QString("/usr/bst/Ftp2.zip");

#define BVT_STX (0x80)        /* 帧起始字符 */
#define BVT_ETX (0x81)        /* 帧结束字符 */
#define BVT_ESC (0x1B)        /* 转换字符 */

#define BVT_STX_AF (0xE7)     /* 帧起始转换后增加字符 */
#define BVT_ETX_AF (0xE8)     /* 帧结束转换后增加字符 */
#define BVT_ESC_AF (0x00)     /* 转换字符转换后增加字符 */

WifiCommunication::WifiCommunication(int port, QObject *parent)
    : QObject(parent)
    , mPort(port)
{
    mUnableHeartBeatCounts = 0;
    mMyTcpServer = new MyTcpServer(parent, port);
    if (mMyTcpServer)
    {
        connect(mMyTcpServer, &MyTcpServer::myTcpServerRecvMsg, this, &WifiCommunication::parseBuffer);
        connect(mMyTcpServer, &MyTcpServer::myTcpServerDeviceDisConnect, this, &WifiCommunication::onDeviceDisconnected);
    }
    connect(&mHeartBeatTimer, &QTimer::timeout, this, &WifiCommunication::onHeartBeatTimeOut);
}

bool WifiCommunication::jsonFormatIsRight(const QByteArray &byteArray)
{
    bool flag = false;
    QJsonParseError jsonError;
    mJsonDoc = QJsonDocument::fromJson(byteArray, &jsonError); // 转化为JSON文档
    if (!mJsonDoc.isNull() && (jsonError.error == QJsonParseError::NoError)) // 解析未发生错误
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
    if (!jsonFormatIsRight(byteArray)) // 执行后取得 mJsonDoc
    {
        qDebug()<<__PRETTY_FUNCTION__<<"the byteArray format is wrong";
        return ;
    }

    QJsonObject cmdqobj = mJsonDoc.object();
    QJsonValue jsonType = cmdqobj.value(QString("type"));

    QString type = jsonType.toString();

    QJsonValue jsonInfo = cmdqobj.value(QString("info"));
    if (jsonInfo.isUndefined())
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"has no info";
        return;
    }

    if (!QString::compare(type, "login", Qt::CaseInsensitive))
    {
        login(jsonInfo);
    }
    else if(!QString::compare(type, "heartbeat", Qt::CaseInsensitive))
    {
        heartBeatFrame(jsonInfo);
    }
    else if(!QString::compare(type, "getallparameter", Qt::CaseInsensitive))
    {
        getAllParameter();
    }
    else if(!QString::compare(type, "setparameter", Qt::CaseInsensitive))
    {
        setParameter(jsonInfo);
    }
    else if(!QString::compare(type, "appupdateFinished", Qt::CaseInsensitive))
    {
        appUpdateFinished(jsonInfo);
    }
    else if (!QString::compare(type, "sendFile", Qt::CaseInsensitive))
    {
        sendFile(jsonInfo);
    }
    else if (!QString::compare(type, "updateProgress", Qt::CaseInsensitive))
    {
        updateProgress(jsonInfo);
    }
    else if (!QString::compare(type, "reboot", Qt::CaseInsensitive))
    {
        rebootMachine(jsonInfo);
    }
    else if (!QString::compare(type, "download", Qt::CaseInsensitive))
    {
        downLoadFile(jsonInfo);
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"cmd is invalid";
    }
}

// 解析读取到的QByteArray的数据，需要对帧头和帧尾进行处理
void WifiCommunication::parseBuffer(QByteArray buffer)
{
    mRecvBufferFrame.clear();
    if (judgeArrayIsEmpty(buffer))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"buffer is empty";
        return;
    }
    int tmpBufLen = buffer.length();
    int index = 0;
    bool startFlag = false;
    bool hasTail = false;
    char ch;
    while (index < tmpBufLen)
    {
        ch = buffer.at(index);
        if (startFlag)
        {
            if (BVT_ETX == (quint8)ch) // 如果是帧尾
            {
                hasTail = true;
                break;
            }
            else if (BVT_STX == (quint8)ch)
            {
                qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is more one FrameHead"<<"\n";
                break;
            }
            else // 如果不是帧尾和帧头
            {
                if ((quint8)ch == BVT_ESC) // 如果为BVT_ESC则需要判断紧接着的一个字符值
                {
                    char chNext = buffer.at(index+1);
                    if ((quint8)chNext == BVT_STX_AF)
                    {
                        mRecvBufferFrame.append(BVT_STX);
                        ++index;
                    }
                    else if ((quint8)chNext == BVT_ETX_AF)
                    {
                        mRecvBufferFrame.append(BVT_ETX);
                        ++index;
                    }
                    else if ((quint8)chNext == BVT_ESC_AF)
                    {
                        mRecvBufferFrame.append(BVT_ESC);
                        ++index;
                    }
                    else // 以上都不是的话，此帧error
                    {
                        mRecvBufferFrame.clear();
                        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"BVT_ESC the after char is error"<<"\n";
                        return ;
                    }
                }
                else
                {
                    mRecvBufferFrame.append(ch);
                    ++index;
                }
            }
        }
        else if (BVT_STX != (quint8)ch) // 如果不是帧头，往后找
        {
            ++index;
        }
        else if (BVT_STX == (quint8)ch) // 如果是帧头，设置开始标志为true
        {
            startFlag = true;
            ++index;
        }
    }

    int len = mRecvBufferFrame.length();
    if (0 == len)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"mRecvBufferFrame is empty\n";
        return;
    }
    if (!hasTail)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"has no tail\n";
        return;
    }
    char recvCrc = mRecvBufferFrame.at(len - 1);
    // 文件传送前后crc值校验相同
    if (recvCrc == getCrcVerify(mRecvBufferFrame, len - 1))
    {
//        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"mRecvBufferFrame ="<< mRecvBufferFrame<<"\n";
        mRecvBufferFrame.remove(len - 1, 1); // 去掉最后一个crc校验位
    }
    else
    {
        mRecvBufferFrame.clear();
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"Crc is error"<<"\n";
        return ;
    }
//    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"mRecvBufferFrame ="<< mRecvBufferFrame<<"\n";
    parserJsonFormat(mRecvBufferFrame);
}

// 生成包含帧头帧尾和crc校验位的一帧数据
// QByteArray &buffer 为ID + json格式字符串转化后字符串
// char pID 为数据帧的ID标志
QByteArray& WifiCommunication::generateBuffer(QByteArray &buffer)
{
    mSendBufferFrame.clear();
    int j = 0;
    if (judgeArrayIsEmpty(buffer))
    {
        return mSendBufferFrame;
    }
    if (j < buffer.length()) // 满足条件先加入帧头
    {
        qDebug()<<"add first FrameHead";
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
//    qDebug()<<__PRETTY_FUNCTION__<<" mSendBufferFrame = "<<mSendBufferFrame;
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

QByteArray& WifiCommunication::convertJsonToByteArray(QJsonObject& msg)
{
    mJsonByte.clear();
    QJsonDocument document;
    document.setObject(msg);
    mJsonByte = document.toJson(QJsonDocument::Compact);
//    qDebug()<<__PRETTY_FUNCTION__<<"mJsonByte = "<<mJsonByte;
//    qDebug()<<__PRETTY_FUNCTION__<<"the format is " <<jsonFormatIsRight(mJsonByte);
    return mJsonByte;
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

//    tmp.insert(0, (char)ID_LOGIN);
    QByteArray tmp =  convertJsonToByteArray(loginObj);
    tmp = generateBuffer(tmp);
    writeMsgToClient(tmp, tmp.length());
    if (flag)
    {
        emit  sigDeviceConnected();
//        QtSleep(1000);
//        sendHeartBeat();
        mHeartBeatTimer.start(5000);
    }
}

int WifiCommunication::writeMsgToClient(QByteArray msg, int length)
{
    int len = -1;
    if (mMyTcpServer)
    {
        len = mMyTcpServer->writeMsgToClient(msg, length);
    }
//    qDebug()<<__PRETTY_FUNCTION__<<"len = "<<len;
    return len;
}

void WifiCommunication::login(QJsonValue &InfoMap)
{
//    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<InfoMap;
    if (QJsonValue::Object == InfoMap.type())
    {
        QJsonObject obj = InfoMap.toObject();

        QJsonValue ssidval = obj["ssid"];
        QString ssid = ssidval.toString();
        QJsonValue passwordval = obj["password"];
        QString password = passwordval.toString();

        qDebug()<<__PRETTY_FUNCTION__<<"ssid = "<<ssid<<"passwd = "<<password;
        if(obj.contains(QString("ssid")) && obj.contains(QString("password")))
        {
//            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"ssid = "<<ssid<<"passwd = "<<password;
            emit sigLogin(ssid, password);
        }
        else
        {
            qDebug()<<__PRETTY_FUNCTION__<<"ssid passwd is lost";
        }
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is not QJsonValue::Object";
    }
}

void WifiCommunication::getAllParameter()
{
    emit sigGetAllParametere();
}

void WifiCommunication::setParameter(QJsonValue &InfoMap)
{
//    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"InfoMap = "<<InfoMap;

    QJsonObject obj = InfoMap.toObject();
    QVariantMap tmpMap =  obj.toVariantMap();
    QMapIterator<QString, QVariant> it(tmpMap);
    while (it.hasNext())
    {
        it.next();
        emit sigParameterSetUp(it.key(), it.value());
//        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"it.key() = "<<it.key()<<"it.value() = "<<it.value();
    }
}

void WifiCommunication::appUpdateFinished(QJsonValue &InfoMap)
{
//    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"InfoMap = "<<InfoMap;
    if (QJsonValue::Bool == InfoMap.type())
    {
        bool tmpFlag = InfoMap.toBool();
//        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"tmpFlag = "<<tmpFlag;
        emit sigUpdateFinished(tmpFlag);
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is not Bool";
    }
}

void WifiCommunication::heartBeatFrame(QJsonValue &InfoMap)
{
//    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"InfoMap = "<<InfoMap;
    if (QJsonValue::Double == InfoMap.type())
    {
        double heartNum = InfoMap.toDouble();
//        qDebug()<<"heartNum = "<<heartNum;
        if (1 == heartNum)
        {
            qDebug()<<"is 1 online";
            mHeartBeatFlag = true;
        }
        else
        {
            mHeartBeatFlag = false;
        }
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is not string";
    }
}

void WifiCommunication::sendFile(QJsonValue &InfoMap)
{
//    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"InfoMap = "<<InfoMap;
    if (QJsonValue::Object == InfoMap.type())
    {
        QJsonObject sendFileObj = InfoMap.toObject();

        QJsonValue sendFileFlag = sendFileObj["flag"];
        bool tmpFlag = false;
        if (QJsonValue::Bool == sendFileFlag.type())
        {
            tmpFlag = sendFileFlag.toBool();
        }
        else
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is not Bool";
        }
        QString fileNameString;
        QJsonValue tmpfileName = sendFileObj["filename"];
        if (QJsonValue::String == tmpfileName.type())
        {
            fileNameString = tmpfileName.toString();
        }
        else
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is not string";
        }
        if (tmpFlag)
        {
            removeFile("/home/ftp/pub/" + fileNameString);
            QtSleep(100);
            replySendFile(tmpFlag);
        }
        emit sigSendFile(tmpFlag, fileNameString);
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<tmpFlag<<fileNameString;
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is not QJsonValue::Object";
    }
//    qDebug()<<"\n";
}

void WifiCommunication::updateProgress(QJsonValue &InfoMap)
{
//    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"InfoMap = "<<InfoMap;
    if (QJsonValue::Double == InfoMap.type())
    {
        double pro = InfoMap.toDouble();
//        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"pro = "<<pro;
        emit sigUploadProgress((int)pro);
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is not double";
    }
}

void WifiCommunication::rebootMachine(QJsonValue &InfoMap)
{
//    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"InfoMap = "<<InfoMap;
    if (QJsonValue::Bool == InfoMap.type())
    {
        bool flag = InfoMap.toBool();
        emit sigRebootMachine(flag);
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"flag = "<<flag;
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is not bool";
    }
}

void WifiCommunication::downLoadFile(QJsonValue &InfoMap)
{
//    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"InfoMap = "<<InfoMap;
    if (QJsonValue::Object == InfoMap.type())
    {
        QJsonObject downLoadFileObj = InfoMap.toObject();

        QJsonValue downLoadFileFlag = downLoadFileObj["flag"];
        bool tmpFlag = false;
        if (QJsonValue::Bool == downLoadFileFlag.type())
        {
            tmpFlag = downLoadFileFlag.toBool();
        }
        else
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is not Bool";
        }
//        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"tmpFlag = "<<tmpFlag;

        QString fileNameString;
        QJsonValue tmpfileName = downLoadFileObj["filename"];
        if (QJsonValue::String == tmpfileName.type())
        {
            fileNameString = tmpfileName.toString();
        }
        else
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is not string";
        }
        if (tmpFlag)
        {
//            bool flag = fileExist("/home/ftp/pub/" + fileNameString);
//            QtSleep(20);
//            replyDownLoadFile(flag);
            emit sigDownLoad(tmpFlag, fileNameString);
        }
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<tmpFlag<<fileNameString;
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is not QJsonValue::Object";
    }
}

void WifiCommunication::replySendFile(bool flag)
{
    QJsonObject typeObj;

    typeObj.insert("type", "sendfile");
    typeObj.insert("info", flag);

    QByteArray sendFileByte = convertJsonToByteArray(typeObj);
//    qDebug()<<__PRETTY_FUNCTION__<<"sendFileByte = "<<sendFileByte<<"flag = "<<flag;
    sendFileByte = generateBuffer(sendFileByte);
    writeMsgToClient(sendFileByte, sendFileByte.length());
}

void WifiCommunication::replyDownLoadFile(bool flag)
{
    QJsonObject typeObj;

    typeObj.insert("type", "download");
    typeObj.insert("info", flag);

    QByteArray downLoadByte = convertJsonToByteArray(typeObj);
//    qDebug()<<__PRETTY_FUNCTION__<<"downLoadByte = "<<downLoadByte<<"flag = "<<flag;
    downLoadByte = generateBuffer(downLoadByte);
    writeMsgToClient(downLoadByte, downLoadByte.length());
}

bool WifiCommunication::fileExist(QString fileName)
{
    system("chmod 777 -R /home/ftp/pub/");
    QtSleep(10);
    QFile tmpFile(fileName);
    if (tmpFile.exists())
    {
        qDebug()<<__PRETTY_FUNCTION__<<"file is exists";
        return true;
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"file is not exists";
        return false;
    }
}

//void WifiCommunication::sizeSetting(QJsonValue &InfoMap)
//{
//    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"InfoMap = "<<InfoMap;
//    if (QJsonValue::Double == InfoMap.type())
//    {
//        double num = InfoMap.toDouble();
//        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"num = "<<num;
//        emit sigSizeChange((int)num);
//    }
//    else
//    {
//        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is not Double";
//    }
//}

void WifiCommunication::sendHeartBeat()
{
    QJsonObject map;
    map.insert("type", "heartbeat");
    map.insert("info", "isonline");
    QByteArray heartBeat = convertJsonToByteArray(map);
//    qDebug()<<__PRETTY_FUNCTION__<<"heartBeat = "<<heartBeat;
    heartBeat = generateBuffer(heartBeat);
    writeMsgToClient(heartBeat, heartBeat.length());
    //    heartBeat.insert(0, (char)ID_HEART);
}

int WifiCommunication::startWifi()
{
    int flag = system("source /etc/init.d/S40network start");
    return flag;
}

int WifiCommunication::stopWifi()
{
    int flag = system("source /etc/init.d/S40network stop");
    return flag;
}

void WifiCommunication::onHeartBeatTimeOut()
{
    if (!mHeartBeatFlag)
    {
        ++mUnableHeartBeatCounts;
        if (mUnableHeartBeatCounts >= 10) // 达到10次没有回复
        {
            if (mMyTcpServer)
            {
                mMyTcpServer->slotMyTcpServerCloseSocket();
                mUnableHeartBeatCounts = 0;
            }
            mHeartBeatTimer.stop();
        }
    }
    else
    {
        mHeartBeatFlag = false;
        mUnableHeartBeatCounts = 0;
    }
    sendHeartBeat();
}

void WifiCommunication::onDeviceDisconnected()
{
    mHeartBeatTimer.stop();
    emit sigDeviceDisconnected();
}

void WifiCommunication::removeFile(QString path)
{
    system("chmod 777 -R /home/ftp/pub/");
    QFile tmpFile(path);
    bool flag = tmpFile.exists();
    qDebug()<<__PRETTY_FUNCTION__<<"flag = "<<flag;
    if (flag)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"is exists";
        tmpFile.remove();
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"is not exists";
    }
}
