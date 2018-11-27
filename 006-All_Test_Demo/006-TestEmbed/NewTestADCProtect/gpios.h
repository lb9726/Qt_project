#ifndef GPIOS_H
#define GPIOS_H

#include <QThread>
#include <QTimer>

#define    CANOPENILE      "/dev/canopen"
#define    GPIONUM          24

class Gpios : public QThread
{
    Q_OBJECT
public:
    explicit Gpios(QThread *parent = 0);
    static void mySignalFunc(int signum);
    void initGpiosFd();
    void allLightoff();
    virtual void run();
    int sysExecuteKoFile(const char *str);
    int getFloorNum(float num);
    void judgeIsProtect(int index, int num);
    void getDialSwitchValue();
    void setPressed();
    void setLight(unsigned char c, unsigned char pos);
private:
    void readAdcValue();

signals:
    void sigBtnPressed(quint8 btnindex);
    void sigSpecialBtnPressed(quint8 pNum);
    void sigDailChanged(quint8 pNum);
    void sigBtnCanceld(quint8 btnindex);
    void sigBtnReleased(quint8 btnIndex);
    void sigNeedProctect();

public slots:
    void sltTimeOut();
private:
    volatile bool runThreadFlag;
    quint8 mDipValue;
    QString runPath;
    QTimer mProtectTimer;
    int mFreq;
};

#endif // GPIOS_H
