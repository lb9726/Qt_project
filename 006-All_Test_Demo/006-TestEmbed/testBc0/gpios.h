#ifndef GPIOS_H
#define GPIOS_H

#define IO_PATH(str)  "/sys/class/gpio/gpio"#str"/value"
#define LED_ON  '1'
#define LED_OFF '0'
#define   BTNNUM        12
#define   DAILSWITCH    16
#include <QThread>
#include <QTimer>

class Gpios : public QThread
{
    Q_OBJECT
public:
    explicit Gpios(QThread *parent = 0);
    ~Gpios();
    void setButtonStatus(unsigned char index, unsigned char sta);
    unsigned char lightOnCheckInputStatus(unsigned char index);
    unsigned char normalInputStatus(unsigned char index);
    void startCheckBtnStatus();
    void run();
    void setLedLightFlag(int index, int flag);
//    void setLightOffFlag(bool offflag);
    void setRunThreadFlag(bool runflag);
    quint8 getDialSwitchValue();
signals:
    void sigBtnPressed(quint8 btnindex);
    void sigSpecialBtnPressed(quint8 pNum);
    void sigDailChanged(quint8 pNum);
    void sigBtnCanceld(quint8 btnindex);
public slots:
    void onSigBtnPressed(quint8 btnindex);
    void onSigDailChanged(quint8 pNum);
private:
    volatile bool runThreadFlag;
    volatile int ledLightFlag[BTNNUM];
    quint8 mDipValue;
    int CtrlLightNum[BTNNUM];
    int ButtonPressFlag[BTNNUM];
    int ButtonLightOnPressFlag[BTNNUM];
    int count[4];
    int sendPressSignalFlag[BTNNUM];
    int sendCancelSignalFlag[BTNNUM];
};

#endif // GPIOS_H
