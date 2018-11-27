#ifndef WIDGETTEXT_H
#define WIDGETTEXT_H

#include "widgetbase.h"
#include "effectbase.h"

class WidgetText : public WidgetBase
{
    Q_OBJECT
public:
    WidgetText(QWidget *parent=0);
    ~WidgetText();

    void ShowText(int pIndex, QString pText, QColor pColor = Qt::black, QFont pFont = QFont("Arial", 12, QFont::DemiBold, false));

public slots:
    void slot_ExecOperate(AREA_OPERATE pOperate, QVariant pPara);

public:
    QColor    m_Color;
};

#endif // WIDGETTEXT_H
