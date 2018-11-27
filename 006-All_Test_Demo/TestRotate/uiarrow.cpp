#include "uiarrow.h"
#include <QPainter>
#include <QApplication>
#include "mainwindow.h"

UiArrow::UiArrow(QWidget* parent, Qt::WindowFlags f)
    :QWidget(parent, f)
{
    timeline = 0;
    mFrame = 0;
    mScrollLevel = 12;

    timeline = new QTimeLine;
    timeline->setEasingCurve(QEasingCurve::Linear);
    timeline->setLoopCount(0);
    timeline->setUpdateInterval(1);
    connect(timeline, SIGNAL(frameChanged(int)), this, SLOT(slot_frameChanged(int)));
    connect(&m_timers, SIGNAL(timeout()), this, SLOT(setScrollLevels()));
    m_timers.start(6000);
}

void UiArrow::Init(QDomElement pElement)
{
    QRect tmpRect = gUiLoader->m_themeParser->getComRect(gUiLoader->m_themeLayout, pElement, gUiLoader->m_themeDirection);
    this->setGeometry(tmpRect);

    IDE_TRACE_INT(tmpRect.width());
    IDE_TRACE_INT(tmpRect.height());

    QString tmpThemePath = gUiLoader->m_themeParser->m_FileDir;

    QDomElement tmpRcElement = pElement.firstChildElement("resource");
    QDomElement tmpRc1Element =tmpRcElement.firstChildElement("rc1");
    if(!tmpRc1Element.isNull())
    {
        mUpArrPix = QPixmap(tmpThemePath + tmpRc1Element.text());
    }
    QDomElement tmpRc2Element =tmpRcElement.firstChildElement("rc2");
    if(!tmpRc2Element.isNull())
    {
        mDownArrPix = QPixmap(tmpThemePath + tmpRc2Element.text());
    }
    QDomElement tmpRc3Element =tmpRcElement.firstChildElement("rc3");
    if(!tmpRc3Element.isNull())
    {
        mUpDownArrPix = QPixmap(tmpThemePath + tmpRc3Element.text());
    }
    update();
}

void UiArrow::slot_frameChanged(int pFrame)
{
    if(mFrame != pFrame)
    {
        mFrame = pFrame;
        update();
    }
}

void UiArrow::setScrollLevels()
{
    mScrollLevel -= 2;
    if (mScrollLevel <= 2)
    {
        mScrollLevel = 12;
    }
    timeline->stop();
    timeline->setDuration(6000/mScrollLevel);
    timeline->setFrameRange(0, mPixmap.height());
    timeline->start();
    IDE_TRACE_INT(mScrollLevel);
}

void UiArrow::setArrowType(ArrowType pType, int pScrollSpeed)
{
    Q_UNUSED(pScrollSpeed);
    if(mType != pType)
    {
        mType = pType;

        if(mType == NoArrow)
        {
            mPixmap = QPixmap();
        }
        else if(mType == UpArrow || mType == UpScrollArrow )
        {
            mPixmap = mUpArrPix;
        }
        else if(mType == DownArrow || mType == DownScrollArrow)
        {
            mPixmap = mDownArrPix;
        }
        else if(mType == UpDownArrow)
        {
            mPixmap = mUpDownArrPix;
        }
        StartWork(mScrollLevel);
    }
}

void UiArrow::StartWork(int pScrollLevel)
{
    if(pScrollLevel != 0)
    {
        timeline->setDuration(6000/pScrollLevel);
        timeline->setFrameRange(0, mPixmap.height());
        if (QTimeLine::Running != timeline->state())
        {
            IDE_TRACE_STR("QTimeLine is Running");
            timeline->start();
        }
        else
        {
            IDE_TRACE_STR("QTimeLine is not Running");
            timeline->stop();
            timeline->start();
        }
    }
}

void UiArrow::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter p(this);

    if(mType == NoArrow)
    {
        p.drawPixmap(rect(), mPixmap);
    }
    else if(mType == UpArrow)
    {
        p.drawPixmap(rect(), mPixmap);
    }
    else if(mType == DownArrow)
    {
        p.drawPixmap(rect(), mPixmap);
    }
    else if(mType == UpScrollArrow)
    {
        p.drawPixmap(0, -mFrame, mPixmap);
        p.drawPixmap(0, height()-mFrame, mPixmap);
    }
    else if(mType == DownScrollArrow)
    {
        p.drawPixmap(0, mFrame, mPixmap);
        p.drawPixmap(0, -height()+mFrame, mPixmap);
    }
    else if(mType == UpDownArrow)
    {
        p.drawPixmap(rect(), mPixmap);
    }
}
