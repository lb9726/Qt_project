#ifndef BASEMEDIA_H
#define BASEMEDIA_H

#include "global.h"

#ifdef MPLAYER
    #include "mplayer.h"
    typedef mplayer MEDIA_PLAYER;
#endif
#ifdef PHONON
    #include "phonon.h"
    typedef phonon MEDIA_PLAYER;
#endif
#ifdef GSTREAMER
    #include "gstreamer.h"
    typedef gstreamer MEDIA_PLAYER;
#endif
#ifdef OSS
    #include "ossplayer.h"
    typedef ossplayer MEDIA_PLAYER;
#endif
#ifdef QPLAYER
    #include "qplayer.h"
    typedef qplayer MEDIA_PLAYER;
#endif

inline void closePlayer()
{
#ifdef MPLAYER
    #if (defined(UBUNTU) || defined(LINUX))
        SYSTEM_CMD::KillProcess("mplayer");
    #else
        SYSTEM_CMD::KillProcess("mplayer.exe");
    #endif
#endif
}

#define D_REPEAT_EN         1
#define D_SOLOTRACK_EN      2
#define D_VOLUME_NUM        3

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

/*
    RIFF WAVE Chun 12
    ==================================
    |       |所占字节数|  具体内容   |
    ==================================
    | ID    |  4 Bytes |   'RIFF'    |
    ----------------------------------
    | Size  |  4 Bytes |             |
    ----------------------------------
    | Type  |  4 Bytes |   'WAVE'    |
    ----------------------------------
    */
    /*
    Format Chunk 26
    ====================================================================
    |               |   字节数  |              具体内容                |
    ====================================================================
    | ID            |  4 Bytes  |   'fmt '                             |
    --------------------------------------------------------------------
    | Size          |  4 Bytes  | 数值为16或18，18则最后又附加信息     |
    --------------------------------------------------------------------  ----
    | FormatTag     |  2 Bytes  | 编码方式，一般为0x0001               |     |
    --------------------------------------------------------------------     |
    | Channels      |  2 Bytes  | 声道数目，1--单声道；2--双声道       |     |
    --------------------------------------------------------------------     |
    | SamplesPerSec |  4 Bytes  | 采样频率                             |     |
    --------------------------------------------------------------------     |
    | AvgBytesPerSec|  4 Bytes  | 每秒所需字节数                       |     |===> WAVE_FORMAT
    --------------------------------------------------------------------     |
    | BlockAlign    |  2 Bytes  | 数据块对齐单位(每个采样需要的字节数) |     |
    --------------------------------------------------------------------     |
    | BitsPerSample |  2 Bytes  | 每个采样需要的bit数                  |     |
    --------------------------------------------------------------------     |
    |         bsp;    |  2 Bytes  | 附加信息（可选，通过Size来判断有无） |     |
    --------------------------------------------------------------------  ----
    */
    /*
    Fact Chunk (可选)
    ==================================
    |       |所占字节数|  具体内容   |
    ==================================
    | ID    |  4 Bytes |   'fact'    |
    ----------------------------------
    | Size  |  4 Bytes |   数值为4   |
    ----------------------------------
    | data  |  4 Bytes |             |
    ----------------------------------
    */
    /*
    Data Chunk 8
    ==================================
    |       |所占字节数|  具体内容   |
    ==================================
    | ID    |  4 Bytes |   'data'    |
    ----------------------------------
    | Size  |  4 Bytes |             |
    ----------------------------------
    | data  |          |             |
    ----------------------------------
*/

#endif // BASEAUDIO_H

