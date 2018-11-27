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
    void setLedLightFlag(int index, int flag);
    void setRunThreadFlag(bool runflag);
    quint8 getDialSwitchValue();
    static void mySignalFunc(int signum);
    int sysExecuteKoFile(const char* str);
    void currentProtection();

signals:
    void sigBtnPressed(quint8 btnindex);
    void sigSpecialBtnPressed(quint8 pNum);
    void sigDailChanged(quint8 pNum);
    void sigBtnCanceld(quint8 btnindex);

public slots:
    void onSigBtnPressedLightOn(quint8 btnindex);
    void onSigBtnPressedLightOff(quint8 btnindex);
    void onSigDailChanged(quint8 pNum);

private:
    volatile bool runThreadFlag;
    quint8 mDipValue;
    QString runPath;
    volatile int ledLightFlag[12];
};

#endif // GPIOS_H
