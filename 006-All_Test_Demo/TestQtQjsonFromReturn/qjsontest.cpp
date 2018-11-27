#include "qjsontest.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QDebug>

QjsonTest::QjsonTest(QObject *parent) : QObject(parent)
{

}

QByteArray QjsonTest::generateJsonData()
{
    QByteArray tmpString;
    QJsonObject loginObj;
    QJsonObject info;
    loginObj.insert("type", "login");
    if (1)
    {
        info.insert("result", true);
    }
    else
    {
        info.insert("result", false);
    }
    loginObj.insert("info", QJsonValue(info));
    QJsonDocument document;
    document.setObject(loginObj);
    tmpString = document.toJson(QJsonDocument::Compact);
    qDebug()<<__PRETTY_FUNCTION__<<__LINE__<<tmpString;
    return tmpString;
}
