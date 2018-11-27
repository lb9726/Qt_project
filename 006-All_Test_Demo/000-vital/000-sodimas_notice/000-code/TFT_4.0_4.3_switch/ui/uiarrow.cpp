#include "uiarrow.h"
#include <QPainter>
#include <QApplication>
#include "mainwindow.h"

UiArrow::UiArrow(QWidget* parent, Qt::WindowFlags f)
    :QWidget(parent,f)
{
    timeline = 0;
    mFrame = 0;
    mScrollLevel = 5;

    timeline = new QTimeLine;
    timeline->setEasingCurve(QEasingCurve::Linear);
    timeline->setLoopCount(0);
    connect(timeline,SIGNAL(frameChanged(int)),this,SLOT(slot_frameChanged(int)));
}

void UiArrow::Init(QDomElement pElement)
{
    QRect tmpRect = gUiLoader->m_themeParser->getComRect(gUiLoader->m_themeLayout, pElement, gUiLoader->m_themeDirection);
    this->setGeometry(tmpRect);
    IDE_TRACE_STR(pElement.text());
    QString tmpThemePath = gUiLoader->m_themeParser->m_FileDir;

    QDomElement run =pElement.firstChildElement("run");
    IDE_TRACE_STR(run.text());
    QDomElement run2 =pElement.firstChildElement("Run");
    IDE_TRACE_STR(run2.text());
    QDomElement fuck =pElement.firstChildElement("fuck");
    IDE_TRACE_STR(fuck.text());
    QDomElement tmpRcElement = pElement.firstChildElement("resource");
    QDomElement tmpRc1Element =tmpRcElement.firstChildElement("rc1");
    IDE_TRACE_STR(tmpRcElement.text());
    IDE_TRACE_STR(tmpRc1Element.text());

    if(!tmpRc1Element.isNull())
    {
        mUpArrPix = QPixmap(tmpThemePath + tmpRc1Element.text());
    }
    QDomElement tmpRc2Element =tmpRcElement.firstChildElement("rc2");
    IDE_TRACE_STR(tmpRc2Element.text());
    if(!tmpRc2Element.isNull())
    {
        mDownArrPix = QPixmap(tmpThemePath + tmpRc2Element.text());
    }
    QDomElement tmpRc3Element =tmpRcElement.firstChildElement("rc3");
    IDE_TRACE_STR(tmpRc3Element.text());
    if(!tmpRc3Element.isNull())
    {
        mUpDownArrPix = QPixmap(tmpThemePath + tmpRc3Element.text());
    }
    getArrowSpeedFormTheme(fuck.text());
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

void UiArrow::setArrowType(ArrowType pType,int pScrollSpeed)
{
    if(mType != pType)
    {
        mType = pType;
       // mScrollLevel = pScrollSpeed;

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
        timeline->setCurrentTime(6000/pScrollLevel);
        timeline->setFrameRange(0,mPixmap.height());
        timeline->start();
    }
}

void UiArrow::paintEvent(QPaintEvent *e)
{
    QPainter p(this);

    if(mType == NoArrow)
    {
        p.drawPixmap(rect(),mPixmap);
    }
    else if(mType == UpArrow)
    {
        p.drawPixmap(rect(),mPixmap);
    }
    else if(mType == DownArrow)
    {
        p.drawPixmap(rect(),mPixmap);
    }
    else if(mType == UpScrollArrow)
    {
        p.drawPixmap(0,-mFrame,mPixmap);
        p.drawPixmap(0,height()-mFrame,mPixmap);
    }
    else if(mType == DownScrollArrow)
    {
        p.drawPixmap(0,mFrame,mPixmap);
        p.drawPixmap(0,-height()+mFrame,mPixmap);
    }
    else if(mType == UpDownArrow)
    {
        p.drawPixmap(rect(),mPixmap);
    }
}

void UiArrow::getArrowSpeedFormTheme(QString text)
{
    IDE_TRACE_STR(text);
    if (text.isEmpty())
    {
        mScrollLevel = 3;
    }
    else
    {
        QStringList tmp = text.split(";");
        for (int i = 0; i < tmp.length(); ++i)
        {
            QString tmp1 = tmp.at(i);
            if (tmp1.contains("Interval", Qt::CaseInsensitive))
            {
                QStringList tmplist = tmp1.split(":");
                if (tmplist.isEmpty())
                {
                    mScrollLevel = 3;
                }
                else if (tmplist.length() >= 2)
                {
                    QString t2 = tmplist.at(1);
                    int speed = t2.toInt();
                    if (speed < 0 )
                    {
                        mScrollLevel = 4;
                    }
                    else if (speed > mDuration)
                    {
                        mScrollLevel = 1;
                    }
                    else
                    {
                        mScrollLevel = mDuration / speed;
                        IDE_TRACE_INT(mScrollLevel);
                    }
                    IDE_TRACE_STR(QString("%1 %2 %3").arg(t2).arg(speed).arg(mScrollLevel));
                }
                return;
            }
            else
            {
                mScrollLevel = 3;
            }
        }
    }
    IDE_TRACE_INT(mDuration / mScrollLevel);
    IDE_TRACE_INT(mScrollLevel);
}
