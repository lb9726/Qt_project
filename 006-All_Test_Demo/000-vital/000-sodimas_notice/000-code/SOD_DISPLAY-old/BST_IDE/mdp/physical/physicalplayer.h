#ifndef PHYSICALPLAYER_H
#define PHYSICALPLAYER_H

#include "physicalbase.h"
#include "basemedia.h"

/*
    背景音乐与视频比较特殊：
        视频有两个地址：主题文件夹内+媒体文件夹
        背景音乐有一个地址：媒体文件夹
    媒体文件夹可以在可移动磁盘中，也可以在任何一个绝对地址中。

    如果在绝对地址中，则在启动时会加载并循环列表播放。
    如果在可移动磁盘中，则需要可移动磁盘插入时才加载并循环播放。
*/

class PhysicalPlayer : public PhysicalDevice
{
    Q_OBJECT
public:
    PhysicalPlayer(QObject *parent = 0);
    ~PhysicalPlayer();

    void InitPlayer();
    bool SetEnable(bool pFlag, E_DEVMODE pDevMode, bool pForce=false);
    bool isTrackEnable(int pId);

    //>@当有到站音时，终止楼层音而播放到站音。并且楼层音以及到站音都会将背景音的大小设为0，并在结束后重新恢复背景音
    bool Play(int pAoType, QString pFile, int pLoop=1, bool pPrior=false, bool pTrack=true);
    void Restore(int pAoType, int pStopType = 0);
    void Restore(int pStopType = 0);
    void Abort(int pAoType, int pStopType = 0);  //pStopType表示停止音频的源类型, 一般0表示Enable等参数，1表示检修模式，2表示pre-operation模式
    void Abort(int pStopType = 0);
    void SetVolume(int pId, int pVolume);
    void SetScaler(qreal pScaler, bool pSync = true);//>@pSync表示是否直接同步到最终的播放器里。
    void SetScaler(int pAoType, qreal pScaler, bool pSync = true);
    int GetVolume(int pAoType);

    void Clear(int pId);

    MEDIA_PLAYER *GetMediaPlayer(int pAoType) { return mAoRelationHash.value(pAoType); }
    QString GetCurMedia(int pId, bool &pRunning);  //>@返回当前音频是否在播放
    quint32 GetCurMedia(QString pAudio);

    quint32 InitPara();
    QString GetInformation() {return QString("%1>").arg(mDevName);}

    bool SetValue(QString pValue);

    void SpotsAdMedia(bool pPrior=false, bool pImmediately=false);

public slots:
    void slot_Play(QString pBeepName);
    void slot_Play(int pAoType, QString pFile, int pLoop, bool pPrior, bool pMultiTrack);
    void slot_Play(int pAoType, QString pFile, int pVolume, int pLoop, bool pPrior, bool pMultiTrack);
    void UpdatePara(QObject* pObject);
    void slot_Initialize();
    void slot_AudioStarted(int pTrack);
    void slot_AudioFinished(int pTrack);

    void slot_MobileDiskChange();

public:
    QString                     mAdAddress;  //>@广告地址
    QStringList                 mAdRcList;

    QSignalMapper              *mStartMapper;
    QSignalMapper              *mFinishMapper;

    QString                     mAoDriver;

    MEDIA_PLAYER*               mPlayerList[AOTRACK_MAXNUM];

    AO_PARA                     mAoParas[AO_MAXNUM];
    QHash<int, MEDIA_PLAYER*>   mAoRelationHash;
    QMultiHash<int, int>        mTrackRelationHash;

    bool                        mSoundOn;

    int                         mAoVolsBk[AO_MAXNUM];
};

#endif // PHYSICALPLAYER_H

