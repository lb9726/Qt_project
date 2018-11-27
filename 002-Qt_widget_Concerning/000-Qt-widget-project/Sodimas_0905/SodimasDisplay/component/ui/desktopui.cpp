#include "desktopui.h"

DesktopUi::DesktopUi(QObject *parent) :
    UiBase(parent)
{
}

DesktopUi::DesktopUi(ThemeParser *pthemeManager, QDomElement &pElement)
{

}
bool DesktopUi::initGeometry(DIRECTION direction, int layout){
    ///Size and position 已经在QML中写死了
    initUi(direction);
}

bool DesktopUi::initUi(DIRECTION direction)
{    
    if(direction == Horizontal){
        QString res = getResQmlPath(0);
        //IDE_TRACE_STR(res);
        if(!res.isEmpty())
            setQmlProperty("bgImage",res);
    }else{
        QString res = getResQmlPath(1);
        //IDE_TRACE_STR(res);
        if(!res.isEmpty())
            setQmlProperty("bgImage",res);
    }
}
