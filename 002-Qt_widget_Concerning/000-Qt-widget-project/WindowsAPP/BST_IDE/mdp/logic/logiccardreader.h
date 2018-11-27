#ifndef LOGICCARDREADER_H
#define LOGICCARDREADER_H

#include "logicbase.h"

class LogicCardReader : public LogicDevice
{
    Q_OBJECT
public:
    LogicCardReader(QObject *parent = 0);
    ~LogicCardReader();

public:
    quint32 InitPara();

public slots:
    void UpdatePara(QObject* pObject);
    void ParseDataStream(){}
    void DeviceStateChanged() {}

public:
    QStringList         mCardModes;
    QString             mKeyAlarm;
};

#endif // LOGICCARDREADER_H
