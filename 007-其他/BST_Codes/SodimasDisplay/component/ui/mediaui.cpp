#include "mediaui.h"

MediaUi::MediaUi(QObject *parent) :
    UiBase(parent)
{
}

MediaUi::MediaUi(ThemeParser *pthemeManager, QDomElement &pElement)
{

}

bool MediaUi::initGeometry(DIRECTION direction, int layout){
    //QVariant arg1 = getComRect(direction,layout);
    //return invokeQmlMethod("setGeometry",arg1);
    Q_UNUSED(direction);
    Q_UNUSED(layout);
}

bool MediaUi::initUi(DIRECTION direction)
{
    Q_UNUSED(direction);
}
