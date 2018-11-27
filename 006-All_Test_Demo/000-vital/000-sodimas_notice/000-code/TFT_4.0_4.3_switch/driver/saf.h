#ifndef SAF_H
#define SAF_H

#include "define.h"
#include <QThread>
#include "bcbuttons.h"

#define D_PIN_SAF   6
#define D_PIN_FLM   20
#define D_PIN_FLD   7
#define D_PIN_SGO   19
#define D_PIN_TALK  21
#define D_PIN_SF    18

#define D_PIN_SAF_LEVAL   '1'     //有效电平
#define D_SAF_INVALID_COUNT  50

#define D_PIN_OTHERS_LEVAL   '0'

class safLine : public QThread
{
    Q_OBJECT
public:
    safLine(QObject *parent = 0);
    ~safLine();

    void run();

signals:
    void sSafNum(quint32 mSafFrameNum);

public:
    quint32     mSafFrameNum;
    bool        mSafChanged;

};

class safBus : public QThread
{
    Q_OBJECT
public:
    safBus(QObject *parent = 0);
    ~safBus();

    void run();
    void parseFrame();

signals:
    void sigArrowAction(bool arrows_FLM,bool arrows_FLD,bool arrows_SCROL);
    void sMessageAudio(int pAudioNum);
public:
    safLine         *mSAFLine;
    quint8           mFLMLevel;
    quint8           mFLDLevel;
    quint8           mSGOUpEdge;
    quint8           mTALKUpEdge;
    quint8           mSFLevel;

private:
    bool            mScroll;
    quint32         mOldFrameNum;

};

#endif // SAF_H
