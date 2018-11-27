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
    bool floorCtrl(int ge,int shi,int bai);
    bool initUi(DIRECTION direction);
    bool initGeometry(DIRECTION direction, int layout);
    bool setFloorVal(QString m_gePath, QString m_shiPath, QString m_baiPath);
    bool setOffset(int geoffset,int shioffset,int baioffset);
signals:

public slots:

public:
    QString gePath;
    QString shiPath;
    QString baiPath;

    int geoffset;
    int shioffset;
    int baioffset;
};

#endif // FLOORUI_H
