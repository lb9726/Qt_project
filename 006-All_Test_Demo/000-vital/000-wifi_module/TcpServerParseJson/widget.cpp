#include "widget.h"
#include "ui_widget.h"
#include <QPixmap>
#include <QCoreApplication>

bool Widget::mAutoTest = false;
int Widget::mLiftFlrVol = 0;
int Widget::mLiftArvVol = 0;
int Widget::mAdMusicVol = 0;
int Widget::mLuminance = 0;
int Widget::mDarkLevel = 0;
int Widget::mStandBy = 0;
int Widget::mRotation = 0;
int Widget::mLayout = 0;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    mRunPath(QCoreApplication::applicationDirPath())
{
    ui->setupUi(this);
    QPixmap tmpmap(mRunPath + "/wifi.png");
    mWifiPicture = tmpmap;
    ui->label_usb->setText("hello");
    connect(&mWiFi, &WifiCommunication::sigMsgArrive, this, &Widget::sltMsgArrive);

    connect(&mWiFi, &WifiCommunication::sigLogin, this, &Widget::sltAppLogin);
    connect(&mWiFi, &WifiCommunication::sigDeviceConnected, this, &Widget::sltDeviceConnected);
    connect(&mWiFi, &WifiCommunication::sigDeviceDisconnected, this, &Widget::sltDeviceDisConnected);
    connect(&mWiFi, &WifiCommunication::sigGetAllParametere, this, &Widget::sltGetAllParameter);
    connect(&mWiFi, &WifiCommunication::sigUpdateFinished, this, &Widget::sltUpdateFinished);
    connect(&mWiFi, &WifiCommunication::sigParameterSetUp, this, &Widget::sltParameterSetUp);
}

Widget::~Widget()
{
    delete ui;
}

QByteArray Widget::replyLoginResult(bool res)
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
//    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"the format is " <<jsonFormatIsRight(loginRes);
    loginRes.insert(0, (char)ID_LOGIN);
    return loginRes;
}

QByteArray Widget::replyGetAllParameter()
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
//    qDebug()<<__PRETTY_FUNCTION__<<"the format is " <<jsonFormatIsRight(allPara);
    allPara.insert(0, (char)ID_SENDPARA);
    return allPara;
}

QByteArray Widget::sendHeartBeat()
{
    QJsonObject map;
    map.insert("type", "heartbeat");
    QJsonDocument document;
    document.setObject(map);
    QByteArray heartBeat = document.toJson(QJsonDocument::Compact);
    qDebug()<<__PRETTY_FUNCTION__<<"heartBeat = "<<heartBeat;
//    qDebug()<<__PRETTY_FUNCTION__<<"the format is " <<jsonFormatIsRight(heartBeat);
    heartBeat.insert(0, (char)ID_HEART);
    return heartBeat;
}

void Widget::sltMsgArrive(QByteArray msg, int length)
{
    ui->listWidget->addItem(msg.left(length));
}

void Widget::sltAppLogin(QString ssid, QString passwd)
{
    if(!QString::compare(ssid, "sodimas", Qt::CaseInsensitive))
    {
        if(!QString::compare("1234", passwd, Qt::CaseSensitive))
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is check ok";
            mWiFi.sltloginResult(true);
        }
        else
        {
            mWiFi.sltloginResult(false);
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is check false";
        }
    }
    else
    {
        qDebug()<<"wrong ssid";
    }
}

void Widget::sltDeviceConnected()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
    qDebug()<<__PRETTY_FUNCTION__<<"mRunPath = "<<mRunPath;

    ui->label_usb->setPixmap(mWifiPicture);
    ui->label_usb->setScaledContents(true);
    ui->label_usb->show();
}

void Widget::sltDeviceDisConnected()
{
    ui->label_usb->hide();
}

void Widget::sltGetAllParameter()
{
    QByteArray tmp = replyGetAllParameter();
    mWiFi.writeMsgToClient(tmp, tmp.length());
}

void Widget::sltUpdateFinished()
{
    qDebug()<<__PRETTY_FUNCTION__<<"update is finished";
}

void Widget::sltParameterSetUp(QString key, QVariant value)
{
    int va = 0;

    if(!QString::compare(key, "liftflrvol"))
    {
        va = value.toInt();
    }
    else if(!QString::compare(key, "liftarvvol"))
    {
        va = value.toInt();
    }
    else if(!QString::compare(key, "admusicvol"))
    {
        va = value.toInt();
    }
    else if(!QString::compare(key, "luminance"))
    {
        va = value.toInt();
    }
    else if(!QString::compare(key, "darklevel"))
    {
        va = value.toInt();
    }
    else if(!QString::compare(key, "standby"))
    {
        va = value.toInt();
    }
    else if(!QString::compare(key, "rotation"))
    {
        va = value.toInt();
    }
    else if(!QString::compare(key, "layout"))
    {
        va = value.toInt();
    }
    else if(!QString::compare(key, "autotest"))
    {
        bool pAutoTest = value.toBool();
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"pAutoTest = "<<pAutoTest;
    }
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"va = "<<va;
}

//void Widget::sltParameterSetUp()
//{
//    QByteArray tmp = replyGetAllParameter();
//    mWiFi.writeMsgToClient(tmp, tmp.length());
//}
