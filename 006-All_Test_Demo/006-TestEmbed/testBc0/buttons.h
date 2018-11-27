#ifndef BUTTONS_H
#define BUTTONS_H

#define IO_PATH(str)  "/sys/class/gpio/gpio"#str"/value"
#define LED_ON  '1'
#define LED_OFF '0'
#define   BTNNUM        12
#define   DAILSWITCH    16
#include <QThread>
#include <QTimer>

class ButtonS : public QThread
{
    Q_OBJECT
public:
    explicit ButtonS(QThread *parent = 0);
    ~ButtonS();
    void setButtonStatus(unsigned char index, unsigned char sta);
    unsigned char lightOnCheckInputStatus(unsigned char index);
    unsigned char normalInputStatus(unsigned char index);
    void startCheckBtnStatus();
    void run();
    void setLedFlag(int flag);
//    void setLightOffFlag(bool offflag);
    void setRunThreadFlag(bool runflag);
    quint8 getDialSwitchValue();
signals:
    void sigBtnPressed(quint8 btnindex);
    void sigSpecialBtnPressed(quint8 pNum);
    void sigDailChanged(quint8 pNum);
public slots:
    void onSigBtnPressed(quint8 btnindex);
    void onSigDailChanged(quint8 pNum);
private:
    volatile bool runThreadFlag;
    quint8 mDipValue;
};

class SpecialBtns : public QThread
{
    Q_OBJECT
public:
    SpecialBtns(QThread *parent = 0);
    void run();
    void setSpecialLedState(unsigned char index, unsigned char sta);
    void setLightOnFlag(bool onflag);
    void setLightOffFlag(bool offflag);
    void setRunThreadFlag(bool runflag);
    void startSetSpecailBtnStatus();
    unsigned char specialBtnCheckInputStatus(unsigned char index);

signals:
    void sigSpecialBtnPressed(quint8 pNum);

public slots:
    void onsigSpecialBtnPressed(quint8 pNum);
    void onTimeOut();
private:
    volatile bool runThreadFlag;
//    QTimer fiveSecond;
};

class ExtensionBtns: public QThread
{
    Q_OBJECT
public:
    ExtensionBtns(QThread *parent = 0);
    void setLightOnFlag(bool onflag);
    void setLightOffFlag(bool offflag);
    void setRunThreadFlag(bool runflag);
    void run();

private:
    volatile bool runThreadFlag;
};

#endif // BUTTONS_H
