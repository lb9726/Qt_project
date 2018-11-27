#ifndef AUDIO_H
#define AUDIO_H

#include <math.h>
#include <QTimer>
#include <QByteArray>
#include <QAudioOutput>
#include <QAudioInput>
#include "global.h"
#include <QIODevice>

typedef enum{
    e_ActiveState=0,
    e_SuspendedState=1,
    e_StoppedState=2,
    e_IdleState=3
}AUDIO_STATUS;

class Audio : public QIODevice
{
    Q_OBJECT
public:
    Audio(QObject *parent);
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
    qint64 bytesAvailable() const;
    void startPlaying(QString FileDir);
    void PraseFile(QByteArray Block,QAudioFormat &FileFormat);
    void stop();

    QFile inputFile;
    QAudioOutput *audioOutput;
    int AudioStatus;
private:
    qint64 m_pos;
    QByteArray m_buffer;
public slots:
    void slot_stateChanged(QAudio::State state);
};

#endif // AUDIO_H
