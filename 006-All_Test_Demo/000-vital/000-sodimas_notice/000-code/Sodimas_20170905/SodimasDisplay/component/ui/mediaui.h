#ifndef MEDIAUI_H
#define MEDIAUI_H
#include "define.h"
#include "themeparser.h"
#include "uibase.h"


class MediaUi : public UiBase
{
    Q_OBJECT
public:
    explicit MediaUi(QObject *parent = 0);
    MediaUi(ThemeParser *pthemeManager,QDomElement &pElement);
    bool initGeometry(DIRECTION direction, int layout);
    bool initUi(DIRECTION direction);
signals:

public slots:

};

#endif // MEDIAUI_H
