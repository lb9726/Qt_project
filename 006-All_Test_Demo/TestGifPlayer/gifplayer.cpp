#include "gifplayer.h"
#include "define.h"
#include <QDebug>

GifPlayer::GifPlayer(QWidget *parent) :
    QLabel(parent)
{
    setStyleSheet("background-color: transparent;");
    setScaledContents(true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setAttribute(Qt::WA_TranslucentBackground);

    m_GifMovie = 0;

    m_Repeat = false;
    m_GifMovie = new QMovie;
    connect(m_GifMovie, SIGNAL(finished()), this, SLOT(slot_Finished()));
    connect(m_GifMovie, SIGNAL(error(QImageReader::ImageReaderError)), this, SLOT(slot_Error(QImageReader::ImageReaderError)));
    setMovie(m_GifMovie);
}

GifPlayer::GifPlayer(QString pGifFile, bool pRepeat, QWidget *parent) :
    QLabel(parent)
{
    setStyleSheet("background-color: transparent;");
    setScaledContents(true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setAttribute(Qt::WA_TranslucentBackground);

    m_Repeat = false;
    m_GifMovie = new QMovie;
    connect(m_GifMovie, SIGNAL(finished()), this, SLOT(slot_Finished()));
    connect(m_GifMovie, SIGNAL(error(QImageReader::ImageReaderError)), this, SLOT(slot_Error(QImageReader::ImageReaderError)));
    setMovie(m_GifMovie);

    Play(pGifFile, pRepeat);
}

GifPlayer::~GifPlayer()
{
    if (m_GifMovie)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"is call";
        delete m_GifMovie;
    }
}

void GifPlayer::Play(QString pGifFile, bool pRepeat)
{
    if(!QFile::exists(pGifFile))
    {
        return;
    }
    if(m_GifMovie->state() != QMovie::NotRunning)
    {
        m_GifMovie->stop();
    }
    m_Repeat = pRepeat;
    m_GifMovie->setFileName(pGifFile);
    m_GifMovie->start();
    Play();
}

void GifPlayer::Play()
{
    if(m_GifMovie)
    {
        show();
        m_GifMovie->start();
    }
}

void GifPlayer::Stop()
{
    if(m_GifMovie)
    {
        hide();
        m_GifMovie->stop();
    }
}

void GifPlayer::slot_Finished()
{
    if(m_Repeat)
    {
        m_GifMovie->start();
    }
    else
    {
        Stop();
    }
    IDE_TRACE();
}

void GifPlayer::slot_Error(QImageReader::ImageReaderError pErr)
{
    Q_UNUSED(pErr);
    Stop();
    IDE_TRACE_INT(pErr);
}

