#ifndef USBLOGO_H
#define USBLOGO_H

#include "define.h"
#include "themeparser.h"
#include "uibase.h"

class UsbLogo : public UiBase
{
public:
    explicit UsbLogo(QObject *parent = 0);
    UsbLogo(ThemeParser *pthemeManager,QDomElement &pElement);
    bool setIcon(QString icon);
    bool initUi(DIRECTION direction);
    bool initGeometry(DIRECTION direction, int layout);
signals:

public slots:
};


#endif // USBLOGO_H
