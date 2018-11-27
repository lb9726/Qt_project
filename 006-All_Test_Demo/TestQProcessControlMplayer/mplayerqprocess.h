#ifndef MPLAYERQPROCESS_H
#define MPLAYERQPROCESS_H

#include <QObject>
#include <QProcess>
#include <QStringList>
#include <QTimer>

class MPlayerQProcess : public QObject
{
    Q_OBJECT
public:
    explicit MPlayerQProcess(QObject *parent = 0);
    void stop();
    void play();
    void pause();
    void setMusicVolume(int pVolume);

private:
    void connectSignalAndSlot();
    void initParameter();

signals:

public slots:
    void onStartStatus();
    void onOccurError(QProcess::ProcessError pErrorNo);
    void onFinished(int exitCode, QProcess::ExitStatus pErrorNo);
    void onTimeOut();
    void onReadyReadStandardOutput();
private:
    QProcess mBackGroundProcess;
    QProcess mLoopMusicProcess;
    QStringList mArgumentsList;
    QString mCmd;
    bool mBackGroundLoopFlag;
    QString mBackGroundMusicPath;
    QString mBackGroundMuiscStatus;

    bool mLoopMusicFlag;
    QString mLoopMusicPath;

    QTimer mTimer1;
    QTimer mTimer2;
    int lastVolume;
};

#endif // MPLAYERQPROCESS_H
