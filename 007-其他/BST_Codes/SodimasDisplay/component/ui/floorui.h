#ifndef FLOORUI_H
#define FLOORUI_H
#include "define.h"
#include "themeparser.h"
#include "uibase.h"


class FloorUi : public UiBase
{
    Q_OBJECT
public:
    explicit FloorUi(QObject *parent = 0);
    FloorUi(ThemeParser *pthemeManager,QDomElement &pElement);
    bool arrowCtrl(int direction,bool isScroll);
    bool initUi(DIRECTION direction);
    bool initGeometry(DIRECTION direction, int layout);
    bool setFloorVal(QString gePath, QString shiPath, QString baiPath);
signals:

public slots:

};

#endif // FLOORUI_H
