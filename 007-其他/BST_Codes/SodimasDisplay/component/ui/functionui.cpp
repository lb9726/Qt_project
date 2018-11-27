#include "functionui.h"

FunctionUi::FunctionUi(QObject *parent) :
    UiBase(parent)
{
}

FunctionUi::FunctionUi(ThemeParser *pthemeManager, QDomElement &pElement)
{

}

bool FunctionUi::initGeometry(DIRECTION direction, int layout){
    QVariant arg1 = getComRect(direction,layout);
    return invokeQmlMethod("setGeometry", arg1);
}

bool FunctionUi::initUi(DIRECTION direction)
{
    Q_UNUSED(direction);
}

bool FunctionUi::setFunction(QString logoPath){
    QVariant arg1 = logoPath;
    return invokeQmlMethod("setFunction", arg1);
}


