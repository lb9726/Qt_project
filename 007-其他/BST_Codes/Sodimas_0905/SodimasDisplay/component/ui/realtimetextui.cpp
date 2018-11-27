#include "realtimetextui.h"

RealtimeTextUi::RealtimeTextUi(QObject *parent) :
    UiBase(parent)
{
}

RealtimeTextUi::RealtimeTextUi(ThemeParser *pthemeManager, QDomElement &pElement)
{

}

bool RealtimeTextUi::initGeometry(DIRECTION direction, int layout){
    QVariant arg1 = getComRect(direction,layout);
    return invokeQmlMethod("setGeometry",arg1);
}

bool RealtimeTextUi::initUi(DIRECTION direction)
{
    Q_UNUSED(direction);
}

bool RealtimeTextUi::updateText(QString msgLine1,QString msgLine2){
    QVariant arg = msgLine1;
    QVariant arg2 = msgLine2;
    return invokeQmlMethod("updateText",arg,arg2);
}

bool RealtimeTextUi::appendText(QString m1,QString m2){
//    QVariant arg = msgLine1;
//    QVariant arg2 = msgLine2;
    return invokeQmlMethod("appendText",m1,m2);
}


