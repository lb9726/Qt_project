#include "usblogo.h"

UsbLogo::UsbLogo(QObject *parent) :
    UiBase(parent)
{

}

UsbLogo::UsbLogo(ThemeParser *pthemeManager, QDomElement &pElement):
    UiBase(pthemeManager,pElement)
{

}

bool UsbLogo::initGeometry(DIRECTION direction, int layout){
    Q_UNUSED(direction);
    Q_UNUSED(layout);
    return true;
}

bool UsbLogo::initUi(DIRECTION direction)
{
    Q_UNUSED(direction);
    QString usbLogo = bundleResourceDir + D_BUNDLEDIR+"/"+D_USBLOGO;

    if(QFile::exists(usbLogo)){
        setQmlProperty("source",D_QMLRESSHCEL + usbLogo);
    }else{
        IDE_TRACE_STR(usbLogo);
      return false;
    }
    return true;
}

bool UsbLogo::setIcon(QString icon){
    QVariant arg = icon;
    return invokeQmlMethod("setIcon",arg);
}

