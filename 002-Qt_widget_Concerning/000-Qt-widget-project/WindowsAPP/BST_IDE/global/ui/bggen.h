#ifndef BASEBGGEN_H
#define BASEBGGEN_H

#include "global.h"

class BaseBgGen : public QObject
{
    Q_OBJECT
public:
    BaseBgGen(QObject *parent = 0);
    void SetFocus(bool pFocus);
    void SetTitle(QString pTitle);
    void SetFullScreen(bool pEnable);
    void Resize(QSize pSize);
    void Refresh();
    void SetPattern(int pPattern) {m_Pattern = pPattern;}
public:
    QBrush       m_BgBrush, m_FgBrush,
                 m_BGlowBrush,                      //>@底部光晕
                 m_TGlassBrush, m_TGlassBrush2,     //>@顶部玻璃磨砂效果
                 m_LightBrush;      //>@选中高亮效果
    QColor       m_BgPenColor, m_FgPenColor;
    QRect        m_BgRect, m_FgRect,
                 m_BGlowRect,      //>@底部光晕
                 m_TGlassRect,     //>@顶部玻璃磨砂效果
                 m_LightRect;      //>@选中高亮效果
    QString      m_Content;
    QFont        m_ContentFont;
    QColor       m_ContentColor;

    QImage       m_Image;
    bool         m_Focus;
    bool         m_FullScreen;

    int          m_Pattern;
};

#endif // BASEBGGEN_H
