#ifndef PROGRESSBARUI_H
#define PROGRESSBARUI_H
#include "define.h"
#include "themeparser.h"
#include "uibase.h"


class ProgressBarUi : public UiBase
{
    Q_OBJECT
public:
    explicit ProgressBarUi(QObject *parent = 0);
    ProgressBarUi(ThemeParser *pthemeManager,QDomElement &pElement);
    bool initGeometry(DIRECTION direction, int layout);
    bool initUi(DIRECTION direction);
    bool updateValue(int progress);
signals:

public slots:

};

#endif // PROGRESSBARUI_H
