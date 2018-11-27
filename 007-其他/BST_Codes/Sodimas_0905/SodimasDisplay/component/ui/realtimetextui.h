#ifndef REALTIMETEXTUI_H
#define REALTIMETEXTUI_H
#include "define.h"
#include "themeparser.h"
#include "uibase.h"


class RealtimeTextUi : public UiBase
{
    Q_OBJECT
public:
    explicit RealtimeTextUi(QObject *parent = 0);
    RealtimeTextUi(ThemeParser *pthemeManager,QDomElement &pElement);
    bool initGeometry(DIRECTION direction, int layout);
    bool initUi(DIRECTION direction);
    bool updateText(QString msgLine1, QString msgLine2);
    bool appendText(QString msgLine1, QString msgLine2);
signals:

public slots:

};

#endif // REALTIMETEXTUI_H
