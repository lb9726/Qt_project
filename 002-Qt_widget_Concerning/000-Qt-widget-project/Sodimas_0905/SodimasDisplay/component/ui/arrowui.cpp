#include "arrowui.h"
#include <QDebug>
ArrowUi::ArrowUi(QObject *parent) :
    UiBase(parent)
{

}

ArrowUi::ArrowUi(ThemeParser *pthemeManager, QDomElement &pElement):
    UiBase(pthemeManager,pElement)
{

}

bool ArrowUi::initGeometry(DIRECTION direction, int layout){
    QVariant arg1 = getComRect(direction,layout);
    //qDebug()<<"ArrowUi "<<arg1;
    return invokeQmlMethod("setGeometry",arg1);
}

bool ArrowUi::initUi(DIRECTION direction)
{
    Q_UNUSED(direction);
    QString res = getResQmlPath(1);
    ///IDE_TRACE_STR(res);
    if(!res.isEmpty())
        setQmlProperty("upArrow",res);

    res = getResQmlPath(2);
    //IDE_TRACE_STR(res);
    if(!res.isEmpty())
        setQmlProperty("downArrow",res);

    res = getResQmlPath(3);
    //IDE_TRACE_STR(res);
    if(!res.isEmpty())
        setQmlProperty("doubleArrow",res);
}

bool ArrowUi::arrowCtrl(int direction,bool isScroll){
    QVariant arg1 = direction;
    QVariant arg2 = isScroll;
    return invokeQmlMethod("arrowCtrl",arg1,arg2);
}

void ArrowUi::setArrowSpeed(int interval){
    if(interval>=1&&interval<=4){
        QVariant inter = 60000/interval;
        invokeQmlMethod("arrowSpeed",inter);
    }else{
        IDE_TRACE_INT(interval);
    }
}

