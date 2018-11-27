#ifndef WIFILOGO_H
#define WIFILOGO_H

#include "define.h"
#include "themeparser.h"
#include "uibase.h"

class WifiLogo : public UiBase
{
public:
    explicit WifiLogo(QObject *parent = 0);
    WifiLogo(ThemeParser *pthemeManager,QDomElement &pElement);
    bool initGeometry(DIRECTION direction, int layout);
    bool initUi(DIRECTION direction);
    bool setIcon(QString icon);
signals:

public slots:
};

#endif // WIFILOGO_H
