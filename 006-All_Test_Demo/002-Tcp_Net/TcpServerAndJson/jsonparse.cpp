#include "jsonparse.h"
#include <QJsonArray>
//#include <QFile>
#include <QJsonValue>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QCoreApplication>
#include <QJsonParseError>
#include <QVariantMap>

#define BVT_STX 0x80        /* 帧起始字符 */
#define BVT_ETX 0x81        /* 帧结束字符 */
#define BVT_ESC 0x1B        /* 转换字符 */

#define BVT_STX_AF 0xE7     /* 帧起始转换后增加字符 */
#define BVT_ETX_AF 0xE8     /* 帧结束转换后增加字符 */
#define BVT_ESC_AF 0x00     /* 转换字符转换后增加字符 */

bool JsonParse::mAutoTest = false;
int JsonParse::mLiftFlrVol = 0;
int JsonParse::mLiftArvVol = 0;
int JsonParse::mAdMusicVol = 0;
int JsonParse::mLuminance = 0;
int JsonParse::mDarkLevel = 0;
int JsonParse::mStandBy = 0;
int JsonParse::mRotation = 0;
int JsonParse::mLayout = 0;

JsonParse::JsonParse(QObject *parent) : QObject(parent)
  , mRunPath(QCoreApplication::applicationDirPath())
{
}

bool JsonParse::jsonFormatIsRight(const QByteArray& byteArray)
{
    bool flag = false;
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(byteArray, &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))   // 解析未发生错误
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

//void JsonParse::createJsonStr()
//{

//}

//https://www.cnblogs.com/senior-engineer/p/5608985.html
WIFICMDS JsonParse::parserJsonFormat(const QByteArray &byteArray)
{
    if ((byteArray.isNull() || byteArray.isEmpty()))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"mJsonString is empty!";
        return ERROR;
    }
    if (!jsonFormatIsRight(byteArray))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"the byteArray format is wrong";
        return ERROR;
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
            return LOGIN;
        }
        else
        {
            return ERROR;
        }
    }
    else if(!QString::compare(type, "heartbeat", Qt::CaseInsensitive))
    {
        if (cmdqobj.contains("info"))
        {
            return HEARTBEAT;
        }
        else
        {
            return ERROR;
        }
    }
    else if(!QString::compare(type, "getallparameter", Qt::CaseInsensitive))
    {
        if (cmdqobj.contains("info"))
        {
            return GETALLPARA;
        }
        else
        {
            return ERROR;
        }
    }
    else if(!QString::compare(type, "setparameter", Qt::CaseInsensitive))
    {
        if (cmdqobj.contains("info"))
        {
            return SETPARA;
        }
        else
        {
            return ERROR;
        }
    }
    else if(!QString::compare(type, "appupdateFinished", Qt::CaseInsensitive))
    {
        if (cmdqobj.contains("info"))
        {
            return UPDATE;
        }
        else
        {
            return ERROR;
        }
    }
    else
    {
        return ERROR;
    }
}

// 解析读取到的QByteArray的数据，需要对帧头和帧尾进行处理
quint32 JsonParse::ParseBuffer(QByteArray &buffer)
{
    if (!judgeArrayIsEmpty(buffer))
    {
        return false;
    }
    return true;
}

// 判断QByteArray的数据是否为空
bool JsonParse::judgeArrayIsEmpty(const QByteArray &buffer)
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

QByteArray JsonParse::replyLoginResult(bool res)
{
    QJsonObject map;
    QJsonObject info;

    info.insert("result", res);
    map.insert("info", QJsonValue(info));
    map.insert("type", "login");
    qDebug()<<__PRETTY_FUNCTION__<<"map = "<<map;

    QByteArray loginRes;
    QJsonDocument document;
    document.setObject(map);
    loginRes = document.toJson(QJsonDocument::Compact);
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"loginRes = "<<loginRes;
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"the format is " <<jsonFormatIsRight(loginRes);
    return loginRes;
}

QByteArray JsonParse::replyGetAllParameter()
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
    qDebug()<<__PRETTY_FUNCTION__<<"the format is " <<jsonFormatIsRight(allPara);
    return allPara;
}

QByteArray JsonParse::sendHeartBeat()
{
    QJsonObject map;
    map.insert("type", "heartbeat");
    QJsonDocument document;
    document.setObject(map);
    QByteArray heartBeat = document.toJson(QJsonDocument::Compact);
    qDebug()<<__PRETTY_FUNCTION__<<"heartBeat = "<<heartBeat;
    qDebug()<<__PRETTY_FUNCTION__<<"the format is " <<jsonFormatIsRight(heartBeat);
    return heartBeat;
}

QByteArray JsonParse::sendJsonFrame(QJsonObject msg)
{
    QJsonDocument document;
    document.setObject(msg);
    QByteArray allPara = document.toJson(QJsonDocument::Compact);
    qDebug()<<__PRETTY_FUNCTION__<<"allPara = "<<allPara;
    qDebug()<<__PRETTY_FUNCTION__<<"the format is " <<jsonFormatIsRight(allPara);
    return allPara;
}

void JsonParse::setLayout(int playout)
{
    mLayout = playout;
}

void JsonParse::setLiftFlrVol(int pVol)
{
    mLiftFlrVol = pVol;
}

void JsonParse::setLiftArvVol(int pVol)
{
    mLiftArvVol = pVol;
}

void JsonParse::setAdMusicVol(int pVol)
{
    mAdMusicVol = pVol;
}

void JsonParse::setLuminance(int pLuminance)
{
    mLuminance = pLuminance;
}

void JsonParse::setDarkLevel(int pdarklevel)
{
    mDarkLevel = pdarklevel;
}

void JsonParse::setStandBy(int pstandy)
{
    mStandBy = pstandy;
}

void JsonParse::setRotation(int protation)
{
    mRotation = protation;
}
