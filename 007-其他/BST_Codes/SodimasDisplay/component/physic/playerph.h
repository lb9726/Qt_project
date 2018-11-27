#ifndef PLAYERPH_H
#define PLAYERPH_H

#include <QObject>
#include <QSignalMapper>
#include "physicbase.h"
#include "../../util/media/playerbase.h"
#include "../../util/media/mediaplayer.h"

#define AOTRACK_MAXNUM      2  //音频的通道数
#define AO_MAXNUM           4  //Audio Types

#define AOTYPE_SPLIT        QString(',')
#define AOTYPECONF_SPLIT    QString('@')

class PlayerPh : public PhysicBase
{
    Q_OBJECT
public:
    PlayerPh(GlobalSetting *globalSetting);
    ~PlayerPh();

    bool initDevice(PH_Type pPHType, QDomElement pElement);
    void initPlayer();

    bool setEnable(DEV_MODE pmode,bool pFlag, bool pForce=false);
    bool isTrackEnable(int pId);

    MediaPlayer *getMediaPlayer(int pAoType) { return mAoRelationHash.value(pAoType); }
    QString getCurMedia(int pId, bool &pRunning);  //>@返回当前音频是否在播放
    quint32 getCurMedia(QString pAudio);

    //>@当有到站音时，终止楼层音而播放到站音。并且楼层音以及到站音都会将背景音的大小设为0，并在结束后重新恢复背景音
    bool play(int pAoType, QString pFile, int pLoop=1, bool pPrior=false, bool pTrack=true);
    void restore(int pAoType, int pStopType = 0);
    void restore(int pStopType = 0);
    void abort(int pAoType, int pStopType = 0);  //pStopType表示停止音频的源类型, 一般0表示Enable等参数，1表示检修模式，2表示pre-operation模式
    void abort(int pStopType = 0);
    void clear(int pId);

    void setVolume(int pId, int pVolume);
    void setScaler(qreal pScaler, bool pSync = true);//>@pSync表示是否直接同步到最终的播放器里。
    void setScaler(int pAoType, qreal pScaler, bool pSync = true);
    int getVolume(int pAoType);


    bool setValue(QString pValue);

public slots:
    void slot_Play(QString pBeepName);
    void slot_Play(int pAoType, QString pFile, int pLoop, bool pPrior, bool pMultiTrack);
    void slot_Play(int pAoType, QString pFile, int pVolume, int pLoop, bool pPrior, bool pMultiTrack);
    void updatePara(QObject* pObject);
    void slot_Initialize();
    void slot_AudioStarted(int pTrack);
    void slot_AudioFinished(int pTrack);

public:
    bool mEnable;
    bool mSoundOn;
    QString                     mAdAddress;
    QStringList                 mAdRcList;
    QSignalMapper              *mStartMapper;
    QSignalMapper              *mFinishMapper;


    QString                     mAoDriver;  // audio dirvers node path
    MediaPlayer*                mPlayerList[AOTRACK_MAXNUM]; //Player's singal number
    AO_PARA                     mAoParas[AO_MAXNUM];  //audio types buffer
    QHash<int, MediaPlayer*>    mAoRelationHash;// Player's Hash Buffer
    QMultiHash<int, int>        mTrackRelationHash;   //音频类型与播放器之间的映射关系
    int                         mAoVolsBk[AO_MAXNUM]; //音频参数备份

public:
    int m_AdmusicVol;
    int m_BeepVol;
    int m_LiftArvVol;
    int m_LiftFlrVol;
};


#endif // PLAYERPH_H
