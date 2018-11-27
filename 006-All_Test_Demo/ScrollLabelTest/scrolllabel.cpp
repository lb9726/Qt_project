#include "scrolllabel.h"
#include <QDebug>
#include <QString>

ScrollLabel::ScrollLabel(QLabel *parent) : QLabel(parent)
{
    connect(&mTimer, &QTimer::timeout, this, &ScrollLabel::scrollCaption);
}

void ScrollLabel::setScrollText(QString str)
{
    mScrollText = str;
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"mScrollText = "<<mScrollText;
}

void ScrollLabel::setInterval(int pInterval)
{
    mInterval = pInterval;
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"mInterval = "<<mInterval;
    if (mTimer.isActive())
    {
        mTimer.stop();
        mTimer.setInterval(mInterval);
        mTimer.start();
    }
    else
    {
        mTimer.setInterval(mInterval);
        mTimer.start();
    }
}

void ScrollLabel::scrollCaption()
{
    static int nPos = 0;
    // 当截取的位置比字符串长时，从头开始
    if (nPos > mScrollText.length())
    {
        nPos = 0;
    }
    this->setText(mScrollText.mid(nPos));
//    mTmpText = mScrollText.append(" ") + mScrollText.left(nPos);
//    this->setText(mTmpText);
    ++nPos;
}
