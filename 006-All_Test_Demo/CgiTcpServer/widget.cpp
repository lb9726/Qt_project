#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

using std::nothrow;
//#define TESTLINK(a, b, c) #a###b###c
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    mWiFi = nullptr;
    ui->setupUi(this);
    init();
//    qDebug()<<TESTLINK('1', '3', 3);
}

Widget::~Widget()
{
    delete ui;
    if (mWiFi)
    {
        delete mWiFi;
        mWiFi = nullptr;
    }
}

void Widget::init()
{
    mWiFi = new(nothrow) WifiCommunication();
    connect(mWiFi, &WifiCommunication::sigLogin, this, &Widget::sltAppLogin);
    connect(mWiFi, &WifiCommunication::sigGetAllParametere, this, &Widget::sltGetAllParameter);
    connect(mWiFi, &WifiCommunication::sigParameterSetUp, this, &Widget::sltParameterSetUp);
    connect(mWiFi, &WifiCommunication::sigRebootMachine, this, &Widget::sltRebootMachine);

    connect(mWiFi, &WifiCommunication::sigShowUploadFileMessage, this, &Widget::sltShowUploadFileMessage);
    connect(mWiFi, &WifiCommunication::sigWebUpload, this, &Widget::sltWebUpload);
}

void Widget::sltGetAllParameter()
{
    QJsonObject map;
    QJsonObject info;

    info.insert("autotest", true);
    info.insert("liftflrvol", 28);
    info.insert("liftarvvol", 28);
    info.insert("admusicvol", 3);
    info.insert("luminance", 4);
    info.insert("darklevel", 5);
    info.insert("standby", 300000);
    info.insert("rotation", 90);
    info.insert("layout", 1);

    map.insert("info", QJsonValue(info));
    map.insert("type", "sendallparameter");
//    qDebug()<<__PRETTY_FUNCTION__<<"map = "<<map;
    QByteArray allPara;
    if (mWiFi)
    {
        allPara = mWiFi->convertJsonToByteArray(map);
        allPara = mWiFi->generateBuffer(allPara);
        mWiFi->writeMsgToClient(allPara, allPara.length());
    }
    qDebug()<<__PRETTY_FUNCTION__<<"allPara = "<<allPara;
}

void Widget::sltParameterSetUp(QString key, QVariant value)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"key "<<key<<"value = "<<value;
}

void Widget::sltRebootMachine(bool flag)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"will reboot"<<flag;
}

void Widget::sltAppLogin(QString ssid, QString passwd)
{
    if (!QString::compare(ssid, "sodimas", Qt::CaseInsensitive))
    {
        if (!QString::compare(QString("1234"), passwd, Qt::CaseSensitive))
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is check ok";
            if (mWiFi)
            {
                mWiFi->sltloginResult(true);
//                emit sigLoginMessage(true);
            }
        }
        else
        {
            if (mWiFi)
            {
                mWiFi->sltloginResult(false);
//                emit sigLoginMessage(false);
            }
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is check false";
        }
    }
    else
    {
        if (mWiFi)
        {
            mWiFi->sltloginResult(false);
//                emit sigLoginMessage(false);
        }
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"wrong ssid";
    }
}

void Widget::sltWebUpload(QString fileName)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"fileName = "<<fileName;
}

void Widget::sltShowUploadFileMessage(bool pFlag)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"pFlag = "<<pFlag;
}
