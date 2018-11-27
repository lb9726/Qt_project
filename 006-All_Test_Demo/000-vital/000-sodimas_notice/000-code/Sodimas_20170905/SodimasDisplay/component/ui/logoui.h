#ifndef LOGOUI_H
#define LOGOUI_H
#include "define.h"
#include "themeparser.h"
#include "uibase.h"


class LogoUi : public UiBase
{
    Q_OBJECT
public:
    explicit LogoUi(QObject *parent = 0);
    LogoUi(ThemeParser *pthemeManager,QDomElement &pElement);
signals:

public slots:

};

#endif // LOGOUI_H
