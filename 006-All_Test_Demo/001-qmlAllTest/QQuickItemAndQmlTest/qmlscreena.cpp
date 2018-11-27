#include "qmlscreena.h"
#include <QDebug>
#include <QTimerEvent>
#include <QDateTime>

QmlScreenA::QmlScreenA()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
    qDebug()<<"this = "<<this;
    connect(&mtimer, &QTimer::timeout, this, &QmlScreenA::changeColor);
    connect(this, &QmlScreenA::colorChanged, this, &QmlScreenA::slotColorChanged);
    mtimer.start(5000);
}

QmlScreenA::~QmlScreenA()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
}

QColor QmlScreenA::color() const
{
    return m_currentColor;
}

void QmlScreenA::setColor(const QColor &color)
{
    if (m_currentColor != color)
    {
        m_currentColor = color;
        emit colorChanged(m_currentColor);
    }
}

QColor QmlScreenA::timeColor() const
{
    QTime time = QTime::currentTime();
    int r = time.hour();
    int g = time.minute()*2;
    int b = time.second()*4;
    return QColor::fromRgb(r, g, b);
}

QmlScreenA::GenerateAlgorithm QmlScreenA::algorithm() const
{
    return m_algorithm;
}

void QmlScreenA::setAlgorithm(QmlScreenA::GenerateAlgorithm algorithm)
{
    m_algorithm = algorithm;
}

void QmlScreenA::changeColor()
{
    static int flag = 0;
    if (3 == flag)
    {
        flag = 0;
    }
    if (0 == flag)
    {
        setColor(QColor(200, 0, 0));
    }
    else if (1 == flag)
    {
//        setColor(QColor(250, 20, 0));
        setColor(QColor(200, 0, 0));
    }
    else if (2 == flag)
    {
//        setColor(QColor(220,50, 30));
        setColor(QColor(200, 0, 0));
    }
    ++flag;
}

void QmlScreenA::slotColorChanged()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"color changed";
}
