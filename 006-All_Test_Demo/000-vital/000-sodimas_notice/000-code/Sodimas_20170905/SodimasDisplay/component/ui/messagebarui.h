#ifndef MESSAGEBARUI_H
#define MESSAGEBARUI_H
#include "define.h"
#include "themeparser.h"
#include "uibase.h"


class MessageBarUi : public UiBase
{
    Q_OBJECT
public:
    explicit MessageBarUi(QObject *parent = 0);
    MessageBarUi(ThemeParser *pthemeManager,QDomElement &pElement);
    bool initGeometry(DIRECTION direction, int layout);
    bool initUi(DIRECTION direction);
    bool updateMsg(QString msg);
signals:

public slots:

};

#endif // MESSAGEBARUI_H
