#include "desktopui.h"

DesktopUi::DesktopUi(QObject *parent) :
    UiBase(parent)
{
}

DesktopUi::DesktopUi(ThemeParser *pthemeManager, QDomElement &pElement)
{

}
bool DesktopUi::initGeometry(DIRECTION direction, int layout){
    ///已经在QML中写死了
}

bool DesktopUi::initUi(DIRECTION direction)
{    
    if(direction == Horizontal){
        QString res = getResourcePath(0);
        //IDE_TRACE_STR(res);
        if(!res.isEmpty())
            setQmlProperty("bgImage",res);
    }else{
        QString res = getResourcePath(1);
        //IDE_TRACE_STR(res);
        if(!res.isEmpty())
            setQmlProperty("bgImage",res);
    }
}
