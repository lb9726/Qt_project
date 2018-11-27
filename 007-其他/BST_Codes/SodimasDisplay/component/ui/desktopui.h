#ifndef DESKTOPUI_H
#define DESKTOPUI_H
#include "define.h"
#include "themeparser.h"
#include "uibase.h"

class DesktopUi : public UiBase
{
    Q_OBJECT
public:
    explicit DesktopUi(QObject *parent = 0);
    DesktopUi(ThemeParser *pthemeManager,QDomElement &pElement);
    bool initUi(DIRECTION direction);
    bool initGeometry(DIRECTION direction, int layout);
signals:

public slots:

};

#endif // DESKTOPUI_H
