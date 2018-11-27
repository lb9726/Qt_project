#ifndef SAF_H
#define SAF_H
#include <QThread>

#define     D_PIN_SAF    6
#define     D_PIN_FLM    20
#define     D_PIN_FLD    7
#define     D_PIN_SGO    19
#define     D_PIN_TALK   21
#define     D_PIN_SF     18

#define     D_PIN_SAF_LEVEL   "1"
#define     D_SAF_MAX_TIME    50
#define     D_PIN_SAF_OTHERLEVEL  "0"

class safLine:public QThread
{
    Q_OBJECT
public:
    safLine(QObject *parent = Q_NULLPTR);

    void run();
public:
        quint8 mSafNum;

};

class safBus:public QThread
{
    Q_OBJECT
public:
    safBus(QObject *parent = Q_NULLPTR);

    void run();
    void PraseFrame();

signals:
    void sigArrowAction(bool arrows_FLM,bool arrows_FLD,bool arrows_SCROL);
    void sMessageAudio(int pAudioNum);

public:
    safLine*    mSafLine;
    quint8      mFLMLevel;
    quint8      mFLDLevel;
    quint8      mSGOUpEdge;
    quint8      mTalkUpEdge;
    quint8      mSFLevel;
    bool        mScroll;
    quint8      mOldSAFNum;

};

#endif // SAF_H
