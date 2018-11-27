#ifndef GPIOS_H
#define GPIOS_H
#include <QThread>
#define    CANOPENILE      "/dev/canopen"
#define    GPIONUM          24
#include <QString>

class Gpios : public QThread
{
    Q_OBJECT
public:
    explicit Gpios(QThread *parent = 0);
    ~Gpios();
    void initGpiosFd();
    void run();
    void setLedLightOffFlag(int index, int flag);
    void setRunThreadFlag(bool runflag);
    quint8 oneGetDialSwitchValue();
    static void mySignalFunc(int signum);
    int sysExecuteKoFile(const char* str);
    void currentProtection();
    void setElibLed(bool flag);

signals:
    void sigBtnPressed(quint8 btnindex);
    void sigSpecialBtnPressed(quint8 pNum);
    void sigDailChanged(quint8 pNum);
    void sigBtnCanceld(quint8 btnindex);
    void sigBtnReleased(quint8 btnIndex);

public slots:
    void onSigBtnPressedLightOn(quint8 btnindex);
    void onSigBtnPressedLightOff(quint8 btnindex);
    void onSigDailChanged(quint8 pNum);

private:
    void getDialSwitchValue();

private:
    volatile bool runThreadFlag;
    quint8 mDipValue;
    QString runPath;
};

#endif // GPIOS_H
