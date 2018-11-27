#ifndef THEMELG_H
#define THEMELG_H

#include <QObject>
#include "logicbase.h"
#include "themeparser.h"

class ThemeLg : public LogicBase
{
    Q_OBJECT
public:
    ThemeLg(ThemeParser *themeParser);
    bool initDevice(LG_Type pPHType, QDomElement pElement);
signals:

public slots:

public:
    int m_Layout;
    QString m_ThemeName;

};

#endif // THEMELG_H
