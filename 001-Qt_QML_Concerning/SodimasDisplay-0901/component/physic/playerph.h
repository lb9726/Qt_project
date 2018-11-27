#ifndef PLAYERPH_H
#define PLAYERPH_H

#include <QObject>
#include <QSignalMapper>
#include "physicbase.h"
//#include "../../util/media/playerbase.h"
//#include "../../util/media/mediaplayer.h"
#include "../../util/media/mplayer.h"
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
    void starWork(QString musicPath);

    bool setEnable(DEV_MODE pmode,bool pFlag, bool pForce=false);

    bool play(int pAoType, QString pFile, int pLoop=1, bool pPrior=false);

    void clear();
    void setVolume(AO_TYPE AoduiType,int pVolume);
    int getVolume(int pAoType);

    bool setValue(QString pValue);

public slots:
    void sltBeep();
    void sltPlay(int index);
    void sltPlay(int pAoType, QString pFile, int pLoop = 0, bool pPrior = false);
    void updatePara(QObject* pObject);
    ///void slot_Initialize();

public:
    bool mEnable;
    bool mSoundOn;

    int m_AdmusicVol;
    int m_BeepVol;
    int m_LiftArvVol;
    int m_LiftFlrVol;

    Mplayer* m_Player;

};


#endif // PLAYERPH_H
