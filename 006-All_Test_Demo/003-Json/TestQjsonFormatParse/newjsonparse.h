#ifndef NEWJSONPARSE_H
#define NEWJSONPARSE_H

#include <QObject>
#include <QByteArray>
#include <QJsonDocument>
#include <QVariant>

class NewJsonParse : public QObject
{
    Q_OBJECT
public:
    explicit NewJsonParse(QObject *parent = 0);
    QByteArray& readFile();
    void readFileLines(QString fileName);
    void readFrameHeadAndFrameTailFile(QString fileName);
    void sendJsonFrame();
    bool jsonFormatIsRight(const QByteArray& byteArray);
    void parserJson();
    void parserJson1();
    void parserWholeJson(const QByteArray &byteArray);
    QByteArray& convertJsonToByteArray(QJsonObject& msg);
    QByteArray& generateBuffer(QByteArray& buffer);
    void parseBuffer(QByteArray buffer);
    void parserJsonFormat(const QByteArray& byteArray);
    bool judgeArrayIsEmpty(const QByteArray& buffer);
    char getCrcVerify(QByteArray msg, int length);
    void writeFileWithFrameHeadAndFrameTail(QString fileName);
    void sendHeartFrame();

signals:
    void sigDeviceConnected();
    void sigDeviceDisconnected();

    void sigLogin(QString ssid, QString passwd);
    void sigParameterSetUp(QString key, QVariant value);
    void sigGetAllParametere();
    void sigUpdateFinished(bool flag);
    void sigSendFile(bool flag, QString fileName);
    void sigUploadProgress(int percent);

public slots:
    void login(QJsonValue &InfoMap);
    void getAllParameter();
    void setParameter(QJsonValue &InfoMap);
    void appUpdateFinished(QJsonValue &InfoMap);
    void heartBeatFrame(QJsonValue &InfoMap);
    void sendFile(QJsonValue &InfoMap);
    void updateProgress(QJsonValue &InfoMap);
    void rebootMachine(QJsonValue &InfoMap);


    void sltGetAllParameter();
private:
    QString mRunPath;
    QByteArray mJsonString;
    static bool mAutoTest;
    static int mLiftFlrVol;  //  设置  message
    static int mLiftArvVol;  //  设置  gong
    static int mAdMusicVol;  //  设置  music
    static int mLuminance;   //  设置  normal
    static int mDarkLevel;   //  standby为显示器进入standby模式后的背光亮度
    static int mStandBy;     //  设置  delay before standby
    static int mRotation;    //  0;90;180;270
    static int mLayout;      //  0;1;2;3
    QJsonDocument mJsonDoc;
    bool mHeartBeatFlag;
    QByteArray mSendBufferFrame;
    QByteArray mRecvBufferFrame;
    QByteArray mJsonByte;
};

#endif // NEWJSONPARSE_H
