#ifndef COMPH_H
#define COMPH_H

#include <QObject>
#include "physicbase.h"

class ComPh : public PhysicBase
{
    Q_OBJECT
public:
    ComPh(GlobalSetting *globalSetting);

    bool initDevice(PH_Type pPHType, QDomElement pElement);
signals:

public slots:

public:
    bool m_Autotest;
    int m_TestInterval;
};

#endif // COMPH_H
