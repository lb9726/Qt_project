#ifndef ADCPROTECT_H
#define ADCPROTECT_H

#include <QThread>
#include <QString>

class AdcProtect : public QThread
{
    Q_OBJECT
public:
    explicit AdcProtect(QThread *parent = 0);
    void init();
    void initGpiosFd();
    int sysExecuteKoFile(const char* str);
    static void mySignalFunc(int signum);
    void currentProtection();
    void run();
signals:

public slots:
private:
    QString runPath;
};

#endif // ADCPROTECT_H
