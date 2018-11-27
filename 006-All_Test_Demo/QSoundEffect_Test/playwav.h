#ifndef PLAYWAV_H
#define PLAYWAV_H

#include <QObject>
#include <QSoundEffect>

class PlayWav : public QObject
{
    Q_OBJECT
public:
    explicit PlayWav(QObject *parent = 0);
    void initPlayer();

signals:

public slots:
    void play();
    void stop();
    void setMuted();
    void setLoopCount(int loopCount);
    void setVolume(qreal volume);

private:
    QSoundEffect * mPlayWav;
};

#endif // PLAYWAV_H
