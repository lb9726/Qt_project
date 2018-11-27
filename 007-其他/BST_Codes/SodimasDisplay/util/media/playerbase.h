#ifndef PLAYBASE_H
#define PLAYBASE_H

#include "../../global/define.h"
#include "../../global/globalfun.h"
#include "wav_parser.h"

#define D_VOL_FIX  0x00ff0000




#define D_HALFBUFFERSIZE  4
#define D_MAXBUFFERSIZE   50



#define D_MONITORDELAY  500   ///延时检测100ms
#define D_PROCESSWAIT   200   ///等待
#define SPU_ANALOG_VOLUME		32

//uint qHash(AO_TYPE pType) {  return qHash((int)pType); }

typedef enum{
    MD_PLAYBACK=0,
    MD_RECORD
} MD_MEDIA;

typedef enum{
    AO_LIFTFLR=0,
    AO_LIFTARV=1,
    AO_BEEP=2,
    AO_ADMUSIC=3,
    AO_NONE
} AO_TYPE;


typedef enum{
    LOOP_EVER = -1,
    LOOP_NONE = 0,
    LOOP_LIST = 1
} LOOP_TYPE;

typedef enum{
    AO_IDEL=0,
    AO_STARTING,
    AO_PLAYING,
    AO_PAUSE,
    AO_ABORT,
    AO_RCERROR,
    AO_DRVERROR,
    AO_FMTERROR,
    AO_MEMERROR,
    AO_FINISHED,
    AO_UNKNOWERROR
} AO_MODE;


typedef struct _PLAY_MEDIA_INFO
{
    _PLAY_MEDIA_INFO()
    {
        mType = AO_NONE;
        mLoop = 0;
        mVolume = -1;
        mEmitSig = false;
    }
    AO_TYPE         mType;
    QString         mPath;
    QString         mPara;
    int             mLoop;    ///-1 表示无限循环
    int             mVolume;  ///为-1时表示使用使用当前类默认音量
    bool            mEmitSig; ///表示是否发送开始播放、结束播放、停止播放信号
} PLAY_MEDIA_INFO;

class AO_PARA
{
public:
    AO_PARA() {mEnable[0] = mEnable[1] = false; mScaler = 1;}

    int GetVolume() { return mVolume.mNum * mScaler; }

    NUM_INFO mVolume;
    qreal    mScaler;    ///音频缩放参数
    bool     mEnable[2]; ///表明某类型音频是否禁止播放 0:表示有效的，1：表示备份的
};

inline AO_TYPE GetAoType(QString pType)
{
    if(pType.isEmpty())
        return AO_NONE;
    if(!pType.compare("liftflr", Qt::CaseInsensitive))
        return AO_LIFTFLR;
    else if(!pType.compare("liftarv", Qt::CaseInsensitive))
        return AO_LIFTARV;
    else if(!pType.compare("beep", Qt::CaseInsensitive))
        return AO_BEEP;
    else if(!pType.compare("admusic", Qt::CaseInsensitive))
        return AO_ADMUSIC;
    return AO_NONE;
}

inline QString GetAoString(AO_TYPE pType)
{
    if(pType == AO_NONE)
        return QString();
    if(pType == AO_LIFTFLR)
        return QString("liftflr");
    else if(pType == AO_LIFTARV)
        return QString("liftarv");
    else if(pType == AO_BEEP)
        return QString("beep");
    else if(pType == AO_ADMUSIC)
        return QString("admusic");
    return QString();
}

#endif // CMDPLAYER_H


