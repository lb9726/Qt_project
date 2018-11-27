#include "graphicsprogress.h"

GraphicsProgress::GraphicsProgress(QGraphicsItem *parent) :
    GraphicsOperate(parent)
{
    m_ExtraProgress = 0;
    connect(&m_DelayTimer, SIGNAL(timeout()), this, SLOT(slot_CloseProgress()));
    m_DelayTimer.setSingleShot(true);
}

GraphicsProgress::~GraphicsProgress()
{
    if(m_ExtraProgress)
    {
        m_ExtraProgress->deleteLater();
        m_ExtraProgress = 0;
    }
}

bool GraphicsProgress::Start()
{
    GraphicsOperate::Start();

    //>@获取Progress信息
    int tmpPenWid = 6;
    QRect tmpPrgRect = rect().toRect();
#ifdef N329
    QImage tmpPrgImage = QImage(D_FSRCPATH + D_BOOTDIR + QString("transparent.png")).scaled(size().toSize());
    if(tmpPrgImage.isNull())
        return false;
#else
    QImage tmpPrgImage(size().width(), size().height(), QImage::Format_ARGB32_Premultiplied);
    if(tmpPrgImage.isNull())
        return false;
    tmpPrgImage.fill(Qt::transparent); //>@创建透明图层
#endif
    QPainter tmpPrgPainter(&tmpPrgImage);
#ifdef PIXQUALITY
    tmpPrgPainter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    tmpPrgPainter.setRenderHint(QPainter::Antialiasing);
#endif
    //>@绘制进度条外框
    int round = tmpPrgRect.height()/2;
    tmpPrgPainter.save();
    tmpPrgPainter.setBrush(QBrush(QColor(55,70,79)));
    tmpPrgPainter.drawRoundedRect(tmpPrgRect, round, round);
    tmpPrgPainter.restore();
    //>@绘制进度条底图及顶图
    tmpPrgPainter.save();
    QLinearGradient pgBaseGradient(QPointF(0,tmpPenWid/2), QPointF(0,tmpPrgRect.width()-tmpPenWid));
    pgBaseGradient.setColorAt(0, QColor(241,241,241,255));
    pgBaseGradient.setColorAt(0.5, QColor(202,202,202,255));
    pgBaseGradient.setColorAt(1, QColor(241,241,241,255));//设置渐变的颜色和路径比例
    tmpPrgPainter.setPen(Qt::NoPen);
    tmpPrgPainter.setBrush(QBrush(pgBaseGradient));
    m_TextRect = tmpPrgRect.adjusted(tmpPenWid, tmpPenWid, -tmpPenWid, -tmpPenWid);
    tmpPrgPainter.drawRoundedRect(m_TextRect, round, round);
    tmpPrgPainter.restore();
    int fontheight = tmpPrgRect.height()*2.0/3.0;
    int fontsize = CalcFontSizeFromSize(QString("Arial"), QSize(100,fontheight), PRIOR_HEIGHT);
    m_Font = QFont("Arial", fontsize, QFont::DemiBold, false);
    m_Color = Qt::white;
    tmpPrgPainter.end();
    m_BgPix = QPixmap::fromImage(tmpPrgImage);

    //>@默认隐藏Message窗口
#ifndef IDE
    hide();
#endif
    return true;
}

bool GraphicsProgress::PaintEffect(QPainter *p)
{
    DrawPixmap(p, m_BgPix, rect());
    p->save();
    //>@绘制进度
    int tmpValue = m_ProgressValue;
    QString tmpValueString = QString("%1").arg(tmpValue);
    QLinearGradient pgTopGradient(m_TextRect.topLeft(), m_TextRect.bottomLeft());
    pgTopGradient.setColorAt(0, QColor(254,145,29,255));
    pgTopGradient.setColorAt(0.5, QColor(238,99,0,255));
    pgTopGradient.setColorAt(1, QColor(254,145,29,255));//设置渐变的颜色和路径比例
    p->setPen(Qt::NoPen);
    p->setBrush(QBrush(pgTopGradient));
    int len = m_TextRect.width()*(100-tmpValue)/100.0;
    int round = m_TextRect.height()/2;
    p->drawRoundedRect(m_TextRect.adjusted(0, 0, -len, 0), round, round);
    p->restore();
    p->save();
    p->setFont(m_Font);
    p->setPen(Qt::white);
    p->drawText(m_TextRect, Qt::AlignCenter, tmpValueString);
    p->restore();
    return true;
}

void GraphicsProgress::updateEffect(AREA_OPERATE pOperate, QVariant pPara)
{
    GraphicsOperate::updateEffect(pOperate, pPara);
    switch(pOperate)
    {
        case OPERATE_KEYPRESS:
        {
            //>@分解坐标，如果点击上半区域，则向上滚动
            QPoint tmpPoint = pPara.toPoint();
            if(tmpPoint.y() < size().height()/2)
            {
                UpdateValue(40);
            }
            else if(tmpPoint.y() > size().height()/2)
            {
                UpdateValue(70);
            }
            break;
        }
        default:
            break;
    }
}

void GraphicsProgress::updateEffect(quint32 pValue, AREA_OPERATE pOperate)
{
    if(pValue < 0)
        pValue = 0;
    if(pValue > 100)
        pValue = 100;
    m_ProgressValue = pValue;
    //>@执行STEP0中的内容
    if(isVisible()==false)
    {
        setZValue(D_PRGCTRL_ZVALUE);
        show();
    }
    m_DelayTimer.start(5000);
    update();
}

void GraphicsProgress::slot_CloseProgress()
{
    hide();
}

void GraphicsProgress::UpdateExtraValue(quint32 pValue)
{
    if(!m_ExtraProgress)
    {
        m_ExtraProgress = new GraphicsProgress(parentItem());
        m_ExtraProgress->m_UiContainer = m_UiContainer;
        m_ExtraProgress->InitComponent(m_ComElement, false);
        m_ExtraProgress->moveBy(0, geometry().height());
    }
    m_ExtraProgress->UpdateValue(pValue);
}

void GraphicsProgress::UpdateValue(quint32 pValue)
{
    updateEffect(pValue, OPERATE_RUN);  //>@永远不能调用UpdateExtraValue，否则会出现递归死循环
}

void GraphicsProgress::slot_Progress(quint32 pPrg)
{
    UpdateValue(pPrg);
}

void GraphicsProgress::slot_Progress(quint32 pPrg, quint32 pExtraPrg)
{
    UpdateValue(pPrg);
    UpdateExtraValue(pExtraPrg);
}

DesignProgress::DesignProgress(QGraphicsItem *parent) :
    GraphicsProgress(parent)
{
    QAction *tmpDefault = m_ActionGroup->addAction(tr("0"));
    m_ActionGroup->addAction(tr("30"));
    m_ActionGroup->addAction(tr("50"));
    m_ActionGroup->addAction(tr("70"));
    m_ActionGroup->addAction(tr("90"));
    m_ActionGroup->addAction(tr("100"));
    tmpDefault->setChecked(true);
}

QSize DesignProgress::GetRcSize(QString pRcName)
{
    int tmpKey = GetKey(pRcName);
    if(tmpKey == BGRC)
        return size().toSize();
    else if(tmpKey == 0)
        return size().toSize();
    return QSize();
}

QList<QAction*> DesignProgress::GetActionList()
{
    QList<QAction*> tmpList;
    if(m_ActionGroup)
    {
        tmpList = m_ActionGroup->actions();
        //>@根据当前已有的资源使能响应功能Action
        for(int i=0;i<tmpList.count();i++)
        {
            QAction *tmpAction = tmpList.at(i);
            if(tmpAction == 0)
                continue;
            tmpAction->setEnabled(true);
        }
    }
    return tmpList;
}

void DesignProgress::ExecAction(QAction *pAction)
{
    UpdateValue(pAction->text().toInt());
}

