#ifndef WIFICOMMUNICATION_H
#define WIFICOMMUNICATION_H

#include <QObject>
#include <QByteArray>
#include <QTimer>
#include <QString>
#include <QCoreApplication>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonObject>
#include <QVariantMap>

#include "tcpmodule.h"

typedef enum
{
    ERROR = -1,
    HEARTBEAT,
    LOGIN,
    GETALLPARA,
    SETPARA,
    UPDATE
}WIFICMDS;

typedef enum ID_FRAME
{
    ID_HEART        =     0x01,        /* heartBeat */
    ID_LOGIN        =     0x02,       /* login */
    ID_GETPARA      =     0x03,    /* getallparameter */
    ID_SENDPARA     =     0x04,    /* sendallparameter */
    ID_UPDATEFINISH =     0x05,/* appupdateFinished */
    ID_SENDFILE     =     0x06,  /* sendFile */
    ID_REBOOT       =     0x07,   /* reboot */
}ID_FRAME;

class WifiCommunication : public QObject
{
    Q_OBJECT
public:
    explicit WifiCommunication(QObject *parent = 0);
    /******************JSON start***************************/
    bool jsonFormatIsRight(const QByteArray& byteArray);
    void parserJsonFormat(const QByteArray& byteArray);
    void parseBuffer(QByteArray& buffer);
    QByteArray& generateBuffer(QByteArray& buffer);
    char getCrcVerify(QByteArray msg, int length);
    bool judgeArrayIsEmpty(const QByteArray& buffer);

    QByteArray sendJsonFrame(QJsonObject& msg);
    void sltloginResult(bool flag);
    int writeMsgToClient(QByteArray msg, int length);

    /******************JSON ended**************************/

    /******************TcpSocket start***************************/
    /*
     * 当MyTcpSocket数据到达时，MyTcpSocket通知MyTcpServer服务器对数据进行处理，
     * 例如检查json格式是否正确等.组合生成需要回复的QJson字符串，调用MyTcpSocket write数据给Client
    */
    /******************TcpSocket ended**************************/

    /******************Json format dealwith start***************************/
private:
    void login(QJsonObject &map);
    void getAllParameter();
    void setParameter(QJsonObject &map);
    void appUpdateFinished();
    void heatBeatFrame();
    /******************Json format dealwith end***************************/
signals:
    void sigMsgArrive(QByteArray msg, int length);

    void sigDeviceConnected();
    void sigDeviceDisconnected();

    void sigLogin(QString ssid, QString passwd);
    void sigParameterSetUp(QString key, QVariant value);
    void sigGetAllParametere();
    void sigUpdateFinished();

public slots:

private:
    MyTcpServer* mMyTcpServer;
    QTimer mHeartBeatTimer;
    QByteArray mSendBufferFrame;
    QByteArray mRecvBufferFrame;
};

#endif // WIFICOMMUNICATION_H
