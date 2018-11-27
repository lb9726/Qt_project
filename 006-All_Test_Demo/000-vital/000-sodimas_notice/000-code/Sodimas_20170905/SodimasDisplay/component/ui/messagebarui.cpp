#include "messagebarui.h"

MessageBarUi::MessageBarUi(QObject *parent) :
    UiBase(parent)
{

}

MessageBarUi::MessageBarUi(ThemeParser *pthemeManager, QDomElement &pElement)
{

}

bool MessageBarUi::initGeometry(DIRECTION direction, int layout){
    QVariant arg1 = getComRect(direction,layout);
    return invokeQmlMethod("setGeometry",arg1);
}

bool MessageBarUi::initUi(DIRECTION direction)
{
    Q_UNUSED(direction);
}

bool MessageBarUi::updateMsg(QString msg){
    QVariant arg = msg;
    return invokeQmlMethod("updateMsg",arg);
}
