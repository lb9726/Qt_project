#ifndef FULLTEXTUI_H
#define FULLTEXTUI_H

#include "define.h"
#include "themeparser.h"
#include "uibase.h"


class FullTextUi : public UiBase
{
    Q_OBJECT
public:
    explicit FullTextUi(QObject *parent = 0);
    FullTextUi(ThemeParser *pthemeManager,QDomElement &pElement);
    bool initGeometry(DIRECTION direction, int layout);
    bool initUi(DIRECTION direction);
    bool updateText(QString msgLine1, QString, QString msgLine3, QString msgLine4);
signals:

public slots:

};

#endif // FULLTEXTUI_H
