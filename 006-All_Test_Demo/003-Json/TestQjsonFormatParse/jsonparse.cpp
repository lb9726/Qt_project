#include "jsonparse.h"
#include <QJsonArray>
#include <QFile>
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

void JsonParse::readJson()
{
    QString val;
    QFile file;
    file.setFileName(mRunPath + "/test.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"open file failed";
        return;
    }

    val = file.readAll();
    file.close();
    qWarning() << val;
    qDebug()<<"val.length() = "<<val.length();
    mJsonString = val.toUtf8();
    if (jsonFormatIsRight(mJsonString))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"the format is right";
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"the format is wrong";
        return;
    }
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject sett2 = d.object();

    QJsonValue value0 = sett2.value(QString("appDesc"));
    qWarning() << value0;

    QJsonValue value = sett2.value(QString("appName"));
    qWarning() << value;
    QJsonObject item = value.toObject();
    qWarning() << tr("QJsonObject of description: ") << item;

    /* incase of string value get value and convert into string*/
    qWarning() << tr("QJsonObject[appName] of description: ") << item["description"];
    QJsonValue subobj = item["description"];
    qWarning() << subobj.toString();

    /* incase of array get array and convert into string*/
    qWarning() << tr("QJsonObject[appName] of value: ") << item["imp"];
    QJsonArray test = item["imp"].toArray();
    qWarning() << test[1].toString();
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

void JsonParse::createJsonStr()
{

}

void JsonParse::parserJsonFormat()
{
    if ((mJsonString.isNull() || mJsonString.isEmpty()))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"mJsonString is empty!";
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(mJsonString);
    QJsonObject qjo = doc.object();
    QJsonValue value0 = qjo.value(QString("type"));
    QString str = value0.toString();
    qWarning() << str<<"\n";

    QJsonValue value1 = qjo.value(QString("info"));
    qWarning() << value1 <<"\n";

    QJsonObject item = value1.toObject();
    qWarning() << tr("QJsonObject of description: ") << item;

    /* incase of string value get value and convert into string*/
    qWarning() << tr("QJsonObject[appName] of autotest: ") << item["autotest"];
    QJsonValue subobj = item["autotest"];
    qWarning() << subobj.toString();
    parserJsonFormat(mJsonString);
}

void JsonParse::parserJsonFormat(const QByteArray &byteArray)
{
    if ((byteArray.isNull() || byteArray.isEmpty()))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"mJsonString is empty!";
        return;
    }
    if (!jsonFormatIsRight(byteArray))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"the byteArray format is wrong";
        return;
    }
    QJsonDocument qdoc = QJsonDocument::fromJson(byteArray);
    QJsonObject cmdqobj = qdoc.object();
    QJsonValue value0 = cmdqobj.value(QString("type"));
    QString type = value0.toString();
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<< type<<"\n";
    if (!QString::compare(type, "login", Qt::CaseInsensitive))
    {
        qWarning() << type <<" login\n";
    }
    else if(!QString::compare(type, "heartbeat", Qt::CaseInsensitive))
    {
//        heatBeatFrame();
    }
    else if(!QString::compare(type, "getallparameter", Qt::CaseInsensitive))
    {
//        QVariantMap map = cmdObj["info"].toMap();
//        getallparameter(map);
    }
    else if(!QString::compare(type, "setparameter", Qt::CaseInsensitive))
    {
//        if(cmdObj.contains("info"))
//        {
//            QVariantMap para = cmdObj["info"].toMap();
//            setparameter(para);
//        }
        if (cmdqobj.contains("info"))
        {
            QJsonValue valueStr = cmdqobj.value(QString("info"));
            QVariantMap map = cmdqobj.toVariantMap();
        }
    }
    else if(!QString::compare(type, "appupdateFinished", Qt::CaseInsensitive))
    {
//        if(cmdObj.contains("info"))
//        {
//            QVariantMap para = cmdObj["info"].toMap();
//            appupdateFinished(para);
//        }
    }
    else
    {

    }
    sendJsonFrame();
}

void JsonParse::sendJsonFrame()
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
    QFile tmpfile(mRunPath + "/libo.txt");
    if (tmpfile.open(QIODevice::WriteOnly))
    {
        tmpfile.write(allPara);
    }
    else
    {
        qDebug()<<"file is open error!";
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

void JsonParse::readHeartJson()
{
//    QByteArray tmpArray("\{\"type\"\:\"heartbeat\"\,\"info\"\:\"1\"\}");
    QByteArray tmpArray("\{\"type\"\:\"heartbeat\"\}");
    mJsonString = tmpArray;
    QJsonDocument qjdoc = QJsonDocument::fromJson(mJsonString);

    QJsonObject qjobj = qjdoc.object();
    qDebug()<<"qjobj = "<<qjobj;

    QJsonValue jsonType = qjobj.value(QString("type"));
    QString tpyeString = jsonType.toString();
    qDebug()<<"tpyeString = "<<tpyeString;

    QJsonValue jsonInfo = qjobj.value(QString("info"));
    qWarning() << jsonInfo;
    if (jsonInfo.isUndefined())
    {
        qDebug()<<"jsonInfo is isUndefined";
    }
    QString infoString = jsonInfo.toString();
    qDebug()<<"infoString = "<<infoString;
    qDebug()<<"infoString.toint() = "<<infoString.toInt();
    if (!QString::compare("1", infoString, Qt::CaseInsensitive))
    {
        qDebug()<<"is 1";
    }
//    if (QString("1") == value0.toString())

    else
    {
        qDebug()<<"is not 1";
    }
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

void JsonParse::setLayout(int playout)
{
    mLayout = playout;
}
