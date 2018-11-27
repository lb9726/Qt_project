#include "wifilogo.h"


WifiLogo::WifiLogo(QObject *parent) :
    UiBase(parent)
{

}

WifiLogo::WifiLogo(ThemeParser *pthemeManager, QDomElement &pElement):
    UiBase(pthemeManager,pElement)
{

}

bool WifiLogo::initGeometry(DIRECTION direction, int layout){
    Q_UNUSED(direction);
    Q_UNUSED(layout);
    return true;
}

bool WifiLogo::initUi(DIRECTION direction)
{
    Q_UNUSED(direction);
    QString usbLogo = bundleResourceDir + D_BUNDLEDIR+"/"+D_WIFILOGO;

    if(QFile::exists(usbLogo)){
        setQmlProperty("source",D_QMLRESSHCEL + usbLogo);
    }else{
        IDE_TRACE_STR(usbLogo);
      return false;
    }
    return true;
}

bool WifiLogo::setIcon(QString icon){
    QVariant arg = icon;
    return invokeQmlMethod("setIcon",arg);
}
