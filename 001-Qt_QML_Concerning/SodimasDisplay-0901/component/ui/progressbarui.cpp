#include "progressbarui.h"

ProgressBarUi::ProgressBarUi(QObject *parent) :
    UiBase(parent)
{
}

ProgressBarUi::ProgressBarUi(ThemeParser *pthemeManager, QDomElement &pElement)
{

}

bool ProgressBarUi::initGeometry(DIRECTION direction, int layout){
    QVariant arg1 = getComRect(direction,layout);
    return invokeQmlMethod("setGeometry",arg1);
}

bool ProgressBarUi::initUi(DIRECTION direction)
{
    Q_UNUSED(direction);
}

bool ProgressBarUi::updateValue(int progress){
    QVariant arg = progress;
    return invokeQmlMethod("updateProgress",arg);
}

