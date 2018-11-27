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
signals:

public slots:
};

#endif // PROTECTADC_H
