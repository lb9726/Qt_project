#include "fulltextui.h"

FullTextUi::FullTextUi(QObject *parent) :
    UiBase(parent)
{
}

FullTextUi::FullTextUi(ThemeParser *pthemeManager, QDomElement &pElement)
{

}



bool FullTextUi::initGeometry(DIRECTION direction, int layout){
    QVariant arg1 = getComRect(direction,layout);
    return invokeQmlMethod("setGeometry",arg1);
}

bool FullTextUi::initUi(DIRECTION direction)
{
    Q_UNUSED(direction);
}

bool FullTextUi::updateText(QString msgLine1,QString msgLine2,QString msgLine3,QString msgLine4){
    QVariant arg = msgLine1;
    QVariant arg2 = msgLine2;
    QVariant arg3 = msgLine3;
    QVariant arg4 = msgLine4;
    return invokeQmlMethod("updateText",arg,arg2,arg3,arg4);
}
