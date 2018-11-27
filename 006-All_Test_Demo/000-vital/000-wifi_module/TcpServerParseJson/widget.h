#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
//#include <QByteArray>
#include "wificommunication.h"
#include <QPixmap>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QByteArray replyLoginResult(bool res);
    QByteArray replyGetAllParameter();
    QByteArray sendHeartBeat();
    QByteArray sendJsonFrame(QJsonObject& msg);

private:
    Ui::Widget *ui;
    QString mRunPath;
    static bool mAutoTest;
    static int mLiftFlrVol;  //  设置  message
    static int mLiftArvVol;  //  设置  gong
    static int mAdMusicVol;  //  设置  music
    static int mLuminance;   //  设置  normal
    static int mDarkLevel;   //  standby为显示器进入standby模式后的背光亮度
    static int mStandBy;     //  设置  delay before standby
    static int mRotation;    //  0;90;180;270
    static int mLayout;      //  0;1;2;3
    WifiCommunication mWiFi;
    QPixmap mWifiPicture;

public slots:
    void sltMsgArrive(QByteArray msg, int length);

    void sltAppLogin(QString ssid, QString passwd);
    void sltDeviceConnected();
    void sltDeviceDisConnected();
    void sltGetAllParameter();
    void sltUpdateFinished();
    void sltParameterSetUp(QString key, QVariant value);
};

#endif // WIDGET_H
