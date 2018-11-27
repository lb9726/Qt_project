#include "floorui.h"
#include <QDebug>

FloorUi::FloorUi(QObject *parent) :
    UiBase(parent)
{
}

FloorUi::FloorUi(ThemeParser *pthemeManager, QDomElement &pElement)
{

}

bool FloorUi::initGeometry(DIRECTION direction, int layout){
    QVariant arg1 = getComRect(direction,layout);
    return invokeQmlMethod("setGeometry",arg1);
}

bool FloorUi::initUi(DIRECTION direction)
{
    Q_UNUSED(direction);
}

bool FloorUi::setFloorVal(QString gePath,QString shiPath,QString baiPath){
    QVariant arg1 = gePath;
    QVariant arg2 = shiPath;
    QVariant arg3 = baiPath;

    return invokeQmlMethod("setFloor",arg1,arg2,arg3);
}
