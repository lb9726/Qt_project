#ifndef GIFPLAYER_H
#define GIFPLAYER_H

#include <QLabel>
#include <QImageReader>
#include <QMovie>
#include <QWidget>

class GifPlayer : public QLabel
{
    Q_OBJECT
public:
    GifPlayer(QWidget *parent = 0);
    GifPlayer(QString pGifFile, bool pRepeat, QWidget *parent = 0);
    ~GifPlayer();
    void Play(QString pGifFile, bool pRepeat);
    void Play();
    void Stop();

public slots:
    void slot_Finished();
    void slot_Error(QImageReader::ImageReaderError pErr);

public:
    QMovie  *m_GifMovie;
    bool     m_Repeat;
};

#endif // GIFPLAYER_H
