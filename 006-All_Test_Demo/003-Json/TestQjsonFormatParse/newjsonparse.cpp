#include "newjsonparse.h"
#include "jsonparse.h"
#include <QJsonArray>
#include <QFile>
#include <QJsonValue>
#include <QDebug>
#include <QJsonObject>
#include <QString>
#include <QCoreApplication>
#include <QJsonParseError>
#include <QVariantMap>

#define BVT_STX (0x80)        /* 帧起始字符 */
#define BVT_ETX (0x81)        /* 帧结束字符 */
#define BVT_ESC (0x1B)        /* 转换字符 */

#define BVT_STX_AF (0xE7)     /* 帧起始转换后增加字符 */
#define BVT_ETX_AF (0xE8)     /* 帧结束转换后增加字符 */
#define BVT_ESC_AF (0x00)     /* 转换字符转换后增加字符 */

bool NewJsonParse::mAutoTest = false;
int NewJsonParse::mLiftFlrVol = 0;
int NewJsonParse::mLiftArvVol = 0;
int NewJsonParse::mAdMusicVol = 0;
int NewJsonParse::mLuminance = 0;
int NewJsonParse::mDarkLevel = 0;
int NewJsonParse::mStandBy = 0;
int NewJsonParse::mRotation = 0;
int NewJsonParse::mLayout = 0;

NewJsonParse::NewJsonParse(QObject *parent):QObject(parent)
  , mRunPath(QCoreApplication::applicationDirPath())
{
//    readFile();
//    parserJson();
//    qDebug();
//    parserJson1();
//    readFileLines(mRunPath + "/libo.txt");
//    writeFileWithFrameHeadAndFrameTail(mRunPath + "/Frame.txt");
//    readFileLines(mRunPath + "/Frame.txt");
//    readFrameHeadAndFrameTailFile(mRunPath + "/Frame.txt");

    sendHeartFrame();
}

QByteArray& NewJsonParse::readFile()
{
    mJsonString.clear();
    QFile file;
    file.setFileName(mRunPath + "/libo.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"open file failed";
        return mJsonString;
    }

    mJsonString = file.readAll();
    file.close();
    qWarning() << mJsonString;
    qDebug()<<"mJsonString.length() = "<<mJsonString.length();
    return mJsonString;
}

void NewJsonParse::readFileLines(QString fileName)
{
    QFile file;
    file.setFileName(fileName); // mRunPath + "/libo.txt"
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"open file failed";
        return;
    }
    while (!file.atEnd())
    {
        QByteArray byteArray = file.readLine();
        qDebug()<<__PRETTY_FUNCTION__<<"byteArray = "<<byteArray<<"\n";
        parseBuffer(byteArray);
    }
}

void NewJsonParse::readFrameHeadAndFrameTailFile(QString fileName)
{
    QFile file;
    file.setFileName(fileName); // mRunPath + "/libo.txt"
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"open file failed";
        return /*QByteArray()*/;
    }
    while (!file.atEnd())
    {
        QByteArray byteArray = file.readLine();
        qDebug()<<__PRETTY_FUNCTION__<<"byteArray = "<<byteArray<<"\n";
        parseBuffer(byteArray);
    }
    file.close();
}

void NewJsonParse::sendJsonFrame()
{
    QJsonObject map;
    QJsonObject info;
    info.insert("autotest", mAutoTest);
    info.insert("liftflrvol", mLiftFlrVol);
    info.insert("liftarvvol", mLiftArvVol);
    info.insert("admusicvol", mAdMusicVol);
    info.insert("luminance", mLuminance);
    info.insert("darklevel", mDarkLevel);
    info.insert("standby", mStandBy);
    info.insert("rotation", mRotation);
    info.insert("layout", mLayout);

    map.insert("info", QJsonValue(info));
    map.insert("type", "sendallparameter");
    qDebug()<<__PRETTY_FUNCTION__<<"map = "<<map;
    QByteArray allPara;

    QJsonDocument document;
    document.setObject(map);
    allPara = document.toJson(QJsonDocument::Compact);

    qDebug()<<__PRETTY_FUNCTION__<<"allPara = "<<allPara;

    QFile tmpfile(mRunPath + "/libo.txt");
    if (tmpfile.open(QIODevice::WriteOnly))
    {
        tmpfile.write(allPara);
    }
    else
    {
        qDebug()<<"file is open error!";
    }
    tmpfile.close();
}

bool NewJsonParse::jsonFormatIsRight(const QByteArray &byteArray)
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

void NewJsonParse::parserJson()
{
    QByteArray tmpString = mJsonString;
    QJsonDocument doc = QJsonDocument::fromJson(tmpString);

    QJsonObject qjo = doc.object();
    qDebug()<<__PRETTY_FUNCTION__<<"qjo = "<<qjo;

    QJsonValue jsonInfo = qjo.value(QString("info"));
    qDebug()<<__PRETTY_FUNCTION__<<"jsonInfo = "<<jsonInfo;
    QString str = jsonInfo.toString();

    qDebug()<<"type is "<<jsonInfo.type();
    qDebug()<<__PRETTY_FUNCTION__<<"str = "<<str;

    QJsonObject obj = jsonInfo.toObject();
    qDebug()<<__PRETTY_FUNCTION__<<"lines = " <<__LINE__<<"obj = "<<obj;
    QVariantMap tmpMap =  obj.toVariantMap();
    QMapIterator<QString, QVariant> it(tmpMap);
    while (it.hasNext())
    {
        it.next();
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"it.key() = "<<it.key()<<"it.value() = "<<it.value();
    }
}

void NewJsonParse::parserJson1()
{
//    QByteArray tmpString("\{\"type\"\:\"getallparameter\"\,\"info\"\:\"get\"\}");
    QByteArray tmpString("\{\"type\"\:\"getallparameter\"\,\"info\"\:45}");
    QJsonDocument doc = QJsonDocument::fromJson(tmpString);
    QJsonObject qjo = doc.object();
    qDebug()<<__PRETTY_FUNCTION__<<"qjo = "<<qjo;

    QJsonValue jsonInfo = qjo.value(QString("info"));
    qDebug()<<__PRETTY_FUNCTION__<<"jsonInfo = "<<jsonInfo;
    if (QJsonValue::String == jsonInfo.type())
    {
        qDebug()<<__PRETTY_FUNCTION__<<"jsonInfo.toString() = "<<jsonInfo.toString();
    }
    else if (QJsonValue::Double == jsonInfo.type())
    {
        qDebug()<<__PRETTY_FUNCTION__<<"jsonInfo.toDouble() = "<<jsonInfo.toDouble();
    }
//    qDebug()<<__PRETTY_FUNCTION__<<"str = "<<str;

}

void NewJsonParse::parserWholeJson(const QByteArray &byteArray)
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
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"cmd is error";
    }
}

QByteArray &NewJsonParse::convertJsonToByteArray(QJsonObject &msg)
{
    mJsonByte.clear();
    QJsonDocument document;
    document.setObject(msg);
    mJsonByte = document.toJson(QJsonDocument::Compact);
    qDebug()<<__PRETTY_FUNCTION__<<"mJsonByte = "<<mJsonByte;
    qDebug()<<__PRETTY_FUNCTION__<<"the format is " <<jsonFormatIsRight(mJsonByte);
    return mJsonByte;
}

QByteArray &NewJsonParse::generateBuffer(QByteArray &buffer)
{
    mSendBufferFrame.clear();
    int j = 0;
    if (judgeArrayIsEmpty(buffer))
    {
        return mSendBufferFrame;
    }
    qDebug()<<__PRETTY_FUNCTION__<<"buffer.length() = "<<buffer.length();
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
    qDebug()<<__PRETTY_FUNCTION__<<" mSendBufferFrame = "<<mSendBufferFrame;
    return mSendBufferFrame;
}

void NewJsonParse::parseBuffer(QByteArray buffer)
{
    mRecvBufferFrame.clear();
    if (judgeArrayIsEmpty(buffer))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"buffer is empty";
        return;
    }
    char ch = buffer.at(0);
    if ((quint8)ch != BVT_STX) // 帧头
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"first is not BVT_STX";
        return;
    }
    ch = buffer.at(buffer.length() - 1);
    if ((quint8)ch != BVT_ETX) // 帧尾
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"ended is not BVT_ETX";
        return;
    }
    int i = buffer.length() - 1; // 去掉帧头，帧尾
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
                qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"BVT_ESC the after char is error"<<"\n";
                return ;
            }
        }
        else
        {
            mRecvBufferFrame.append(ch);
            j = j + 1;
        }
    }
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"mRecvBufferFrame ="<< mRecvBufferFrame<<"\n";
    int len = mRecvBufferFrame.length();
    char recvCrc = mRecvBufferFrame.at(len - 1);
    // 文件传送前后crc值校验相同
    if (recvCrc == getCrcVerify(mRecvBufferFrame, len - 1))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"mRecvBufferFrame ="<< mRecvBufferFrame<<"\n";
        mRecvBufferFrame.remove(len - 1, 1); // 去掉最后一个crc校验位
    }
    else
    {
        mRecvBufferFrame.clear();
        return ;
    }
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"mRecvBufferFrame ="<< mRecvBufferFrame<<"\n";
    parserJsonFormat(mRecvBufferFrame);
}

void NewJsonParse::parserJsonFormat(const QByteArray &byteArray)
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
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"cmd is error";
    }
}

bool NewJsonParse::judgeArrayIsEmpty(const QByteArray &buffer)
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
    qDebug()<<__PRETTY_FUNCTION__<<"is not empty flag = "<< flag;
    return flag;
}

char NewJsonParse::getCrcVerify(QByteArray msg, int length)
{
    char crc = 0x00;
    for (int i = 0; i < length; ++i)
    {
        crc ^= msg.at(i);
    }
    return crc & 0x7f;
}

void NewJsonParse::writeFileWithFrameHeadAndFrameTail(QString fileName)
{
    QFile file;
    file.setFileName(fileName); // mRunPath + "/Frame.txt"
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"open file failed";
        return;
    }
    sltGetAllParameter();
    generateBuffer(mJsonByte);
    file.write(mSendBufferFrame);
    file.close();
}

void NewJsonParse::login(QJsonValue &InfoMap)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<InfoMap;
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
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"ssid = "<<ssid<<"passwd = "<<password;
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
    qDebug()<<"\n";
}

void NewJsonParse::getAllParameter()
{
    qDebug()<<"\n";
}

void NewJsonParse::setParameter(QJsonValue &InfoMap)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"InfoMap = "<<InfoMap;

    QJsonObject obj = InfoMap.toObject();
    QVariantMap tmpMap =  obj.toVariantMap();
    QMapIterator<QString, QVariant> it(tmpMap);
    while (it.hasNext())
    {
        it.next();
        emit sigParameterSetUp(it.key(), it.value());
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"it.key() = "<<it.key()<<"it.value() = "<<it.value();
    }
    qDebug()<<"\n";
}

void NewJsonParse::appUpdateFinished(QJsonValue &InfoMap)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"InfoMap = "<<InfoMap;
    if (QJsonValue::Bool == InfoMap.type())
    {
        bool tmpFlag = InfoMap.toBool();
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"tmpFlag = "<<tmpFlag;
        emit sigUpdateFinished(tmpFlag);
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is not Bool";
    }
    qDebug()<<"\n";
}

void NewJsonParse::heartBeatFrame(QJsonValue &InfoMap)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"InfoMap = "<<InfoMap;
    if (QJsonValue::Double == InfoMap.type())
    {
        double heartNum = InfoMap.toDouble();
        qDebug()<<"heartNum = "<<heartNum;
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
    qDebug()<<"\n";
}

void NewJsonParse::sendFile(QJsonValue &InfoMap)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"InfoMap = "<<InfoMap;
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
        emit sigSendFile(tmpFlag, fileNameString);
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<tmpFlag<<fileNameString;
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is not QJsonValue::Object";
    }
    qDebug()<<"\n";
}

void NewJsonParse::updateProgress(QJsonValue &InfoMap)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"InfoMap = "<<InfoMap;
    if (QJsonValue::Double == InfoMap.type())
    {
        double pro = InfoMap.toDouble();
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"pro = "<<pro;
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is not double";
    }
}

void NewJsonParse::rebootMachine(QJsonValue &InfoMap)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"InfoMap = "<<InfoMap;
    if (QJsonValue::Bool == InfoMap.type())
    {
        bool flag = InfoMap.toBool();
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"flag = "<<flag;
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is not bool";
    }
}

void NewJsonParse::sltGetAllParameter()
{
    QJsonObject map;
    QJsonObject info;

    info.insert("autotest", mAutoTest);
    info.insert("liftflrvol", mLiftFlrVol);
    info.insert("liftarvvol", mLiftArvVol);
    info.insert("admusicvol", mAdMusicVol);
    info.insert("luminance", mLuminance);
    info.insert("darklevel", mDarkLevel);
    info.insert("standby", mStandBy);
    info.insert("rotation", mRotation);
    info.insert("layout", mLayout);

    map.insert("info", QJsonValue(info));
    map.insert("type", "sendallparameter");
    qDebug()<<__PRETTY_FUNCTION__<<"map = "<<map;
    QByteArray allPara;
    allPara = convertJsonToByteArray(map);
//    allPara = mWiFi.convertJsonToByteArray(map);
//    mWiFi.writeMsgToClient(allPara, allPara.length());
    qDebug()<<__PRETTY_FUNCTION__<<"allPara = "<<allPara;
}

void NewJsonParse::sendHeartFrame()
{
    QJsonObject map;
    map.insert("type", "heartbeat");
    map.insert("info", "isonline");
    QByteArray heartBeat = convertJsonToByteArray(map);
    qDebug()<<__PRETTY_FUNCTION__<<"heartBeat = "<<heartBeat;
//    heartBeat = generateBuffer(heartBeat);
//    writeMsgToClient(heartBeat, heartBeat.length());
}
