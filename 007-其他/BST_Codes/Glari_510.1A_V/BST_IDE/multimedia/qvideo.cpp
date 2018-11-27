#include "qvideo.h"

qvideo::qvideo(QObject *parent) :
    QObject(parent)
{
    m_Player = 0;
    m_HLayout = 0;
    m_Stop = false;

    m_Player = new Phonon::VideoPlayer(Phonon::VideoCategory, 0);
    connect(m_Player, SIGNAL(finished()), this, SLOT(slot_Finished()));
    m_Player->setWindowFlags(Qt::FramelessWindowHint);

    m_HLayout = new QHBoxLayout(0);
    m_HLayout->setMargin(0);
    m_HLayout->setSpacing(0);
    m_HLayout->addWidget(m_Player);
}

qvideo::~qvideo()
{
    if(m_Player != 0)
    {
        disconnect(m_Player, SIGNAL(finished()), this, SLOT(slot_Finished()));
        m_Player->stop();
        delete m_Player;
        m_Player = 0;
    }
    if(m_HLayout != 0)
    {
        delete m_HLayout;
        m_HLayout = 0;
    }
    m_Stop = false;
}

void qvideo::slot_SetVideoRect(QWidget *pWidget)
{
    pWidget->setLayout(m_HLayout);
}

void qvideo::slot_Play(QString pSrc, QString pPara, bool pRepeat, bool pPrior)
{
    if(pPath.isEmpty())
        return;
    if(m_Player != 0)
    {
        m_Player->mediaObject()->setCurrentSource(pSrc);
        m_Player->play();
        m_Stop = false;
    }
    IDE_TRACE_INT(QThread::currentThreadId());
}

void qvideo::slot_SetVolume(float pVolume)
{
    if(m_Player != 0)
    {
        if(!m_Stop)
            m_Player->setVolume(pVolume);
    }
}

void qvideo::slot_Seek(qint64 pMs)
{
    if(m_Player != 0)
    {
        if(!m_Stop)
        {
            if(m_Player->mediaObject()->isSeekable())
                m_Player->seek(pMs);
        }
    }
}

void qvideo::slot_Pause()
{
    if(m_Player != 0)
    {
        if(!m_Stop)
            m_Player->pause();
    }
}

void qvideo::slot_Resume()
{
    if(m_Player != 0)
    {
        if(!m_Stop)
            m_Player->play();
    }
}

void qvideo::slot_Stop()
{
    if(m_Player != 0)
    {
        m_Player->stop();
        m_Stop = true;
    }
}

void qvideo::slot_Finished()
{
    emit sFinished();
}
