#ifndef PROTECTADC_H
#define PROTECTADC_H

#include <QThread>

class ProtectADC : public QThread
{
    Q_OBJECT
public:
    explicit ProtectADC(QThread *parent = 0);
    void readAdcValue();
    virtual void run();
    void setPressed();
    void setLight(unsigned char c, unsigned char pos);
    void initGpiosFd();
    int sysExecuteKoFile(const char *str);
    static void mySignalFunc(int signum);
signals:

public slots:
};

#endif // PROTECTADC_H
