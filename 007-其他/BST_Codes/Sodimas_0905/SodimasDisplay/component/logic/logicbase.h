#ifndef LOGICBASE_H
#define LOGICBASE_H

#include <QObject>
#include "globalfun.h"

class LogicBase : public QObject
{
    Q_OBJECT
public:
    explicit LogicBase(QObject *parent = 0);

    virtual bool initDevice(LG_Type pPHType, QDomElement pElement);
signals:

public slots:

};

#endif // LOGICBASE_H
