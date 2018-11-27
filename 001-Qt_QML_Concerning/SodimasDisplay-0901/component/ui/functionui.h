#ifndef FUNCTIONUI_H
#define FUNCTIONUI_H
#include "define.h"
#include "themeparser.h"
#include "uibase.h"


class FunctionUi : public UiBase
{
    Q_OBJECT
public:
    explicit FunctionUi(QObject *parent = 0);
    FunctionUi(ThemeParser *pthemeManager,QDomElement &pElement);

    bool initGeometry(DIRECTION direction, int layout);
    bool initUi(DIRECTION direction);
    bool setFunction(int index);
    bool setFunction(QString logoPath);
    QString tipText(int index);
signals:

public slots:
public:
    QString functionPath;
};

#endif // FUNCTIONUI_H
