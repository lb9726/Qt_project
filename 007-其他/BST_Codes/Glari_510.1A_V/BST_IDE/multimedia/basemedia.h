#ifndef BASEMEDIA_H
#define BASEMEDIA_H

#include "global.h"

#ifdef MPLAYER
    #include "mplayer.h"
    typedef mplayer MEDIA_PLAYER;
#endif
#ifdef PHONON
    #include "qaduio.h"
    typedef qaduio MEDIA_PLAYER;
#endif
#ifdef GSTREAMER
    #include "gstreamer.h"
    typedef gstreamer MEDIA_PLAYER;
#endif

#define AO_MAXNUM 5

typedef enum{AO_LIFTFLR=0,
             AO_LIFTARV,
             AO_BEEP,
             //>@
             AO_ADMUSIC,
             AO_ADVIDEO,
             AO_NONE
} AO_TYPE;

inline uint qHash(AO_TYPE pType) {  return qHash((int)pType); }

inline AO_TYPE GetAoType(QString pType)
{
    if(pType.isEmpty())
        return AO_NONE;
    if(!pType.compare("LiftFlr", Qt::CaseInsensitive))
        return AO_LIFTFLR;
    else if(!pType.compare("LiftArv", Qt::CaseInsensitive))
        return AO_LIFTARV;
    else if(!pType.compare("Beep", Qt::CaseInsensitive))
        return AO_BEEP;
    else if(!pType.compare("AdMusic", Qt::CaseInsensitive))
        return AO_ADMUSIC;
    else if(!pType.compare("AdVideo", Qt::CaseInsensitive))
        return AO_ADVIDEO;
    return AO_NONE;
}


class basemedia : public QObject
{
    Q_OBJECT
public:
    basemedia(QObject *parent = 0);
    ~basemedia();

    void ClearPlayers();

    MEDIA_PLAYER *AddPlayer(int pID, QString pConfFile);
    MEDIA_PLAYER *AddPlayer(int pID, QString pVoDriver, QRect pVoRect, int pVoRotate, QString pAoDriver, int pVolume);
    MEDIA_PLAYER *AddPlayer(int pID, QString pAoDriver, int pVolume);

    MEDIA_PLAYER *GetPlayer(int pID);

signals:
    void sFinished(QObject *pPlayer);

public:
    QHash<int, MEDIA_PLAYER*>        m_PlayerList;
    QSignalMapper                   *m_PlayerMapper;
};

inline void SystemAudio(QString pAudioPath, int pLoop, bool pPrior)
{
    if(!GlobalPara.m_SystemAudio)
    {
        GlobalPara.m_SystemAudio = new basemedia;
        GlobalPara.m_SystemAudio->AddPlayer(100, D_FSRCPATH + QString("SysAudio.conf"));
    }
    MEDIA_PLAYER *tmpPlayer = GlobalPara.m_SystemAudio->m_PlayerList.value(100);
    if(tmpPlayer)
        tmpPlayer->Play(pAudioPath, pLoop, pPrior);
}

#endif // BASEAUDIO_H
