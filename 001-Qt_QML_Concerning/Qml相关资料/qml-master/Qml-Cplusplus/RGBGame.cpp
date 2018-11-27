#include <QTimerEvent>
#include <QDateTime>
#include "RGBGame.h"

RGBGame::RGBGame(QObject *parent)
    :QObject(parent)
    , m_algorithm(RandomRGB)
    , m_currentColor(Qt::black)
    , m_nColorTimer(0)
{
    qsrand(QDateTime::currentDateTime().toTime_t());
}

RGBGame::~RGBGame()
{

}

QColor RGBGame::color() const
{
    return m_currentColor;
}

void RGBGame::setColor(const QColor &color)
{
    m_currentColor = color;
    emit colorChanged(m_currentColor);
}

QColor RGBGame::timeColor() const
{
    QTime time = QTime::currentTime();
    int r = time.hour();
    int g = time.minute()*2;
    int b = time.second()*4;
    return QColor::fromRgb(r, g, b);
}

RGBGame::GenerateAlgorithm RGBGame::algorithm() const
{
    return m_algorithm;
}

void RGBGame::setAlgorithm(GenerateAlgorithm algorithm)
{
    m_algorithm = algorithm;
}

void RGBGame::start()
{
    if (0 == m_nColorTimer)
    {
        m_nColorTimer = startTimer(1000);
    }
}

void RGBGame::stop()
{
    if (0 < m_nColorTimer)
    {
        killTimer(m_nColorTimer);
        m_nColorTimer = 0;
    }
}

void RGBGame::timerEvent(QTimerEvent *e)
{
    if (m_nColorTimer == e->timerId())
    {
        switch (m_algorithm)
        {
        case RandomRGB:
            m_currentColor.setRgb(qrand()%255, qrand()%255,qrand()%255);
            break;
        case RandomRed:
            m_currentColor.setRed(qrand()%255);
            break;
        case RandomGreen:
            m_currentColor.setGreen(qrand()%255);
            break;
        case RandomBlue:
            m_currentColor.setBlue(qrand()%255);
            break;
        case LinearIncrease:
        {
            int r = m_currentColor.red() + 10;
            int g = m_currentColor.green() + 10;
            int b = m_currentColor.blue() + 10;
            m_currentColor.setRgb(r%255,g%255,b%255);
        }
        break;
        default:
            break;
        }
        emit colorChanged(m_currentColor);
        emit currentTime(
                    QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    }
    else
    {
        QObject::timerEvent(e);
    }
}

/*
RGBGame 类中的槽 start()、stop(),信号 colorChanged()、currentTime()都可以在
QML 中直接调用或者与 QML 中的信号、函数连接,例子参照 main.qml。
槽必须声明为 public。
QML 引 擎 会 为 每 一 个 信 号 自 动 创 建 一 个 可 以 在 QML 中 使 用 的 信 号 处 理 器
on<Signal>,Signal 首字母大写,信号中的所有参数在信号处理器中都是可用的。值得注意
的是,如果信号的参数类型未注册到 Qt元对象系统 ,不被 QML 引擎支持,但不会出错,只
是这个参数不能通过信号处理器来访问。
*/






