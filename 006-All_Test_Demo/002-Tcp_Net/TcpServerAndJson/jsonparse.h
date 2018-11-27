#ifndef JSONPARSE_H
#define JSONPARSE_H

#include <QObject>
#include <QByteArray>

typedef enum
{
    ERROR = -1,
    HEARTBEAT,
    LOGIN,
    GETALLPARA,
    SETPARA,
    UPDATE
}WIFICMDS;

class JsonParse : public QObject
{
    Q_OBJECT
public:
    explicit JsonParse(QObject *parent = 0);
//    void readJson();
    bool jsonFormatIsRight(const QByteArray& byteArray);
//    void createJsonStr();
//    void parserJsonFormat();
    WIFICMDS parserJsonFormat(const QByteArray& byteArray);
//    void sendJsonFrame();
    quint32 ParseBuffer(QByteArray& buffer);
    bool judgeArrayIsEmpty(const QByteArray& buffer);

    static void setLiftFlrVol(int pVol);
    static void setLiftArvVol(int pVol);
    static void setAdMusicVol(int pVol);
    static void setLuminance(int pLuminance);
    static void setDarkLevel(int pdarklevel);
    static void setStandBy(int pstandy);
    static void setRotation(int protation);
    static void setLayout(int playout);

    QByteArray replyLoginResult(bool res);
    QByteArray replyGetAllParameter();
    QByteArray sendHeartBeat();
    QByteArray sendJsonFrame(QJsonObject msg);

signals:

public slots:

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
};

#endif // JSONPARSE_H
