#ifndef LOOPMUSICMANAGER_H
#define LOOPMUSICMANAGER_H

#include <QThread>
#include <QTimer>
#include <QTime>

class LoopMusicManager : public QThread
{
    Q_OBJECT
public:
    explicit LoopMusicManager(QThread *parent = 0);
    virtual void run();
    bool getLoopMusicFlag() const;
    void setLoopMusicFlag(bool loopMusicFlag);
    void setMusicNumber(int musicNumber);
    void setBeepvol(int beepvol);
    bool countMusicTimeIsEnd();

signals:

public slots:
    void slotLoopMusic();
    void initLoopMusicProcess();
    void startLoopMusic();
    void playLoopMusic();
    void stopLoopMusic();

private:
    QTimer*       mTimerLoopMusic;
    bool          mLoopMusicFlag;
    QString       mPlayLoopMusicCmd;
    bool          mLoopMusicFlag;
    int           mMusicNumber;
    int           mBeepvol;
};

#endif // LOOPMUSICMANAGER_H
