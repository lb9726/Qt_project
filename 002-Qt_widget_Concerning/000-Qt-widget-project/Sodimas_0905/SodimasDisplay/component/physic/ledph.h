#ifndef LEDPH_H
#define LEDPH_H

#include <QObject>
#include "physicbase.h"
#include "global/define.h"

typedef enum{
    LED_SLOW = 0,
    LED_FAST,
    LED_HIGH
} LED_SPEED;

class LedPh : public PhysicBase
{
    Q_OBJECT
public:
    LedPh(GlobalSetting *globalSetting);

    bool initDevice(PH_Type pPHType, QDomElement pElement);
    LED_SPEED getSpeedFromSting(QString speed);
signals:

public slots:

public:
    LED_SPEED m_Normal;
    LED_SPEED m_Busy;
    LED_SPEED m_Error;

};

#endif // LEDPH_H
