#ifndef LCDPH_H
#define LCDPH_H

#include <QObject>
#include "physicbase.h"
#include "global/define.h"

class LcdPh : public PhysicBase
{
    Q_OBJECT
public:
    LcdPh(GlobalSetting *globalSetting);
    bool initDevice(PH_Type pPHType, QDomElement pElement);
signals:

public slots:
    void  setbacklight(int level);
    void  standbyFilter(bool withArrow);
public:
    int m_Rotation;
    int m_Luminance;
    int m_Standby;
    int m_Standbyluminance;
    int m_Screensave;
    int m_Screensavehold;
};

#endif // LCDPH_H
