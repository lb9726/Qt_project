#include "bggen.h"

BaseBgGen::BaseBgGen(QObject *parent) :
    QObject(parent)
{
    m_Focus = m_FullScreen = false;

    m_Pattern = 0;
}

//>@框的高度不可以拖动，宽度可以随意拖动
void BaseBgGen::Resize(QSize pSize)
{
    m_BgRect = QRect(QPoint(0,0), pSize);
    if(m_Pattern == 0)
    {
        QLinearGradient tmpBgGradient(m_BgRect.topLeft(), m_BgRect.bottomLeft());
        tmpBgGradient.setColorAt(0, QColor(54,183,251,180));
        tmpBgGradient.setColorAt(0.4, QColor(17,116,192,180));
        tmpBgGradient.setColorAt(0.5, QColor(5, 50, 171, 240));
        tmpBgGradient.setColorAt(0.6, QColor(17,116,192, 180));
        tmpBgGradient.setColorAt(1, QColor(54,183,251,180));//设置渐变的颜色和路径比例
        m_BgBrush = QBrush(tmpBgGradient);
        m_BgPenColor = QColor(54,183,251,180);
    }
    else if(m_Pattern == 1)
    {
        m_BgBrush = QBrush(Qt::black);
        m_BgPenColor = Qt::black;
    }

    m_FgRect = m_BgRect.adjusted(4,4,-4,-4);
    if(m_Pattern == 0)
    {
        m_FgBrush = QBrush(QColor(1, 12, 24, 255));
        m_FgPenColor = QColor(13,37,68,255);
    }
    else if(m_Pattern == 1)
    {
        m_FgBrush = QBrush(Qt::black);
        m_FgPenColor = Qt::black;
    }

    int x = m_FgRect.x();
    int y = m_FgRect.y();
    int w = m_FgRect.width();
    int h = m_FgRect.height();
    int tmpW = w*6/5.0;
    m_BGlowRect = QRect(x-w/10.0, y+h-w/5.0, tmpW, tmpW);
    QRadialGradient tmpBGlowGradient(m_BGlowRect.x()+m_BGlowRect.width()/2,
                                   m_BGlowRect.y()+m_BGlowRect.height()/2,
                                   m_BGlowRect.width()/2);
    tmpBGlowGradient.setColorAt(0,QColor(240, 238, 250, 80));
    tmpBGlowGradient.setColorAt(1.0,QColor(245,245,255,0));//设置渐变的颜色和路径比例
    m_BGlowBrush = QBrush(tmpBGlowGradient);

    m_TGlassRect = QRect(x, y+25, w, 35);
    QLinearGradient tmpTGlassGradient(m_TGlassRect.topLeft(), m_TGlassRect.bottomLeft());
    tmpTGlassGradient.setColorAt(0, QColor(79, 120, 165, 200));
    tmpTGlassGradient.setColorAt(0.45, QColor(55, 92, 136, 200));
    tmpTGlassGradient.setColorAt(0.46, QColor(20, 62, 117, 200));
    tmpTGlassGradient.setColorAt(1, QColor(13,91,173,200));//设置渐变的颜色和路径比例
    m_TGlassBrush = QBrush(tmpTGlassGradient);

    QLinearGradient tmpTGlassGradient2(m_TGlassRect.topLeft(), m_TGlassRect.topRight());
    tmpTGlassGradient2.setColorAt(0, QColor(1, 12, 24, 255));
    tmpTGlassGradient2.setColorAt(0.4, QColor(1, 12, 24, 40));
    tmpTGlassGradient2.setColorAt(0.5, QColor(1, 12, 24, 0));
    tmpTGlassGradient2.setColorAt(0.6, QColor(1, 12, 24, 40));
    tmpTGlassGradient2.setColorAt(1, QColor(1, 12, 24, 255));//设置渐变的颜色和路径比例
    m_TGlassBrush2 = QBrush(tmpTGlassGradient2);

    m_LightRect = QRect(x, y+260, w, 80);
    m_LightBrush = QBrush(QColor(255,199,0,120));

    m_ContentFont = QFont("Arial", 15, QFont::DemiBold);
    m_ContentColor = QColor(221, 222, 223, 255);

    Refresh();
}

void BaseBgGen::SetFullScreen(bool pEnable)
{
    m_FullScreen = pEnable;
}

void BaseBgGen::SetFocus(bool pFocus)
{
    if(pFocus)
        m_FgBrush = QBrush(QColor(20, 40, 60, 255));
    else
        m_FgBrush = QBrush(QColor(1, 12, 24, 255));
    Refresh();
}

void BaseBgGen::SetTitle(QString pTitle)
{
    m_Content = pTitle;
}

void BaseBgGen::Refresh()
{
    if(m_Image.isNull())
    {
#ifdef QT_V453
        m_Image = QImage(D_FSRCPATH + D_BOOTDIR + QString("transparent.png")).scaled(m_BgRect.size());
#else
        m_Image = QImage(m_BgRect.size(), QImage::Format_ARGB32_Premultiplied);
#endif
    }
#ifndef QT_V453
    if(m_FullScreen)
        m_Image.fill(m_BgPenColor); //>@创建透明图层
    else
        m_Image.fill(Qt::transparent);
#endif
    QPainter p(&m_Image);
    p.setRenderHint(QPainter::Antialiasing);

    p.setBrush(m_BgBrush);
    p.setPen(m_BgPenColor);
    if(m_FullScreen)
        p.drawRect(m_BgRect);
    else
        p.drawRoundedRect(m_BgRect, 15, 15, Qt::AbsoluteSize);

    p.setBrush(m_FgBrush);
    p.setPen(m_FgPenColor);
    p.drawRoundedRect(m_FgRect, 13, 13, Qt::AbsoluteSize);

    if(!m_Content.isEmpty())
    {
        //>@绘制玻璃效果
        p.setPen(Qt::NoPen);
        p.setBrush(m_TGlassBrush);
        p.drawRect(m_TGlassRect);
        p.setBrush(m_TGlassBrush2);
        p.drawRect(m_TGlassRect);

        //>@绘制文字
        p.setFont(m_ContentFont);
        p.setBrush(m_ContentColor);
        p.setPen(m_ContentColor);
        p.drawText(m_TGlassRect, Qt::AlignHCenter|Qt::AlignVCenter, m_Content);
    }

    //>@绘制底部光晕
    p.setBrush(m_BGlowBrush);
    p.setPen(Qt::NoPen);
    p.drawEllipse(m_BGlowRect);//在相应的坐标画出来

    p.end();
}
