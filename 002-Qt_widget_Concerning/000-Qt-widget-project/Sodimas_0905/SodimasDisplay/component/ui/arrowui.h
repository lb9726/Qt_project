#ifndef UIARROW_H
#define UIARROW_H

#include "define.h"
#include "themeparser.h"
#include "uibase.h"


class ArrowUi : public UiBase
{
    Q_OBJECT
public:
    explicit ArrowUi(QObject *parent = 0);
    ArrowUi(ThemeParser *pthemeManager,QDomElement &pElement);
    bool arrowCtrl(int direction,bool isScroll);
    void setArrowSpeed(int interval);
    bool initUi(DIRECTION direction);
    bool initGeometry(DIRECTION direction, int layout);
signals:

public slots:

};

#endif // UIARROW_H
