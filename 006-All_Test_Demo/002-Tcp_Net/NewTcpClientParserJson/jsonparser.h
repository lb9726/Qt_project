#ifndef JSONPARSER_H
#define JSONPARSER_H

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

class TcpClient;

class JsonParser : public QObject
{
    Q_OBJECT
public:
    explicit JsonParser(QObject *parent = 0);

    bool jsonFormatIsRight(const QByteArray& byteArray);
    void parserJsonFormat(const QByteArray& byteArray);
    void parseBuffer(QByteArray& buffer);
    QByteArray& generateBuffer(QByteArray& buffer);
    char getCrcVerify(QByteArray msg, int length);
    bool judgeArrayIsEmpty(const QByteArray& buffer);
    QByteArray& convertJsonToByteArray(QJsonObject& msg);

    void login(QJsonValue &InfoMap);
    void getAllParameter();
    void setParameter(QJsonValue &InfoMap);
    void appUpdateFinished(QJsonValue &InfoMap);

    void heartBeatFrame(QJsonValue &InfoMap);
    void replyBeatFrameToServer(QJsonValue &InfoMap);

    void sendFile(QJsonValue &InfoMap);
    void updateProgress(QJsonValue &InfoMap);
    void rebootMachine(QJsonValue &InfoMap);
    void downLoadFile(QJsonValue &InfoMap);

    void replySendFile(bool flag);
    bool fileExist(QString fileName);
    void replyDownLoadFile(bool flag);

    int writeMsgToClient(QByteArray msg, int length);
    void setTcpClient(TcpClient * pClient);

signals:

    void sigDeviceConnected();
    void sigDeviceDisconnected();

    void sigLogin(QString ssid, QString passwd);
    void sigParameterSetUp(QString key, QVariant value);
    void sigGetAllParametere();
    void sigUpdateFinished(bool flag);
    void sigSendFile(bool flag, QString fileName);
    void sigUploadProgress(int percent);
    void sigDownLoad(bool flag, QString fileName);

public slots:
    void removeFile(QString path);

private:
    TcpClient* mMyTcpClient;
    QTimer mHeartBeatTimer;
    QByteArray mSendBufferFrame;
    QByteArray mRecvBufferFrame;
    QByteArray mJsonByte;
    bool  mHeartBeatFlag;
    int mUnableHeartBeatCounts;
    QJsonDocument mJsonDoc;
    int mPort;

};

#endif // JSONPARSER_H
