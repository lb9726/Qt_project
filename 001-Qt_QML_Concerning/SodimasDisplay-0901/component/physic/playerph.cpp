#include "playerph.h"

PlayerPh::PlayerPh(GlobalSetting *globalSetting): PhysicBase(globalSetting)
{
    mSoundOn = true;
}

PlayerPh::~PlayerPh()
{
}

bool PlayerPh::initDevice(PH_Type pPHType, QDomElement pElement)
{
    Q_UNUSED(pPHType);
    m_NodeElement = pElement;
    QDomNodeList tmpParaNodeList = m_NodeElement.childNodes();
    if(tmpParaNodeList.count() < 1)
        return 0;
    quint32 validcount = 0;
    for(int i=0;i<tmpParaNodeList.count();i++)
    {
        QDomElement tmpElement = tmpParaNodeList.at(i).toElement();
        if(tmpElement.isNull())
            continue;
        QString tmpName = tmpElement.tagName();
        if(!tmpName.compare("liftflrvol"))
        {
            m_LiftFlrVol = tmpElement.text().toInt();
        }
        else if(!tmpName.compare("liftarvvol"))
        {
            m_LiftArvVol = tmpElement.text().toInt();
        }
        else if(!tmpName.compare("beepvol"))
        {
            m_BeepVol = tmpElement.text().toInt();
        }
        else if(!tmpName.compare("admusicvol"))
        {
            m_AdmusicVol = tmpElement.text().toInt();
        }
        else
        {
            continue;
        }
        validcount++;
    }

//    IDE_TRACE_INT(m_AdmusicVol);
//    IDE_TRACE_INT(m_BeepVol);
//    IDE_TRACE_INT(m_LiftArvVol);
//    IDE_TRACE_INT(m_LiftFlrVol);
    initPlayer();
}

bool PlayerPh::setEnable(DEV_MODE pDevMode, bool pFlag, bool pForce)
{    
    mEnable = true;
    return true;
}

void PlayerPh::initPlayer()
{
    m_Player = new Mplayer;

}

void PlayerPh::starWork(QString musicPath){
    m_Player->setBackgroundMuisc(musicPath,m_AdmusicVol);
    m_Player->startWork();
}

void PlayerPh::sltBeep()
{
    QString pFile;/////////////
    play(AO_BEEP, pFile, LOOP_NONE);
}

void PlayerPh::sltPlay(int index){

}

void PlayerPh::sltPlay(int pAoType, QString pFile, int pLoop, bool pPrior)
{
    play(pAoType, pFile, pLoop, pPrior);
}

void PlayerPh::setVolume(AO_TYPE AoduiType,int pVolume)
{
    IDE_TRACE_INT(pVolume);
    switch (AoduiType) {
    case AO_LIFTFLR:
        //IDE_TRACE();
        m_LiftFlrVol = pVolume;
        break;
    case AO_LIFTARV:
         ///IDE_TRACE();
        m_LiftArvVol = pVolume;
        break;
    case AO_BEEP:
         //IDE_TRACE();
        m_BeepVol = pVolume;
        break;
    case AO_ADMUSIC:

        m_AdmusicVol = pVolume;
        ///IDE_TRACE_INT(m_AdmusicVol);
        m_Player->setBackgroundPlayerVolume(m_AdmusicVol);
        break;
    default:
        break;
    }
}

int PlayerPh::getVolume(int pAoType)
{
    switch (pAoType) {
    case AO_LIFTFLR:
        return m_LiftFlrVol;
        break;
    case AO_LIFTARV:
        return m_LiftArvVol;
        break;
    case AO_BEEP:
        return m_BeepVol;
        break;
    case AO_ADMUSIC:
        return m_AdmusicVol;
        break;
    default:
        break;
    }
    return 0;
}

bool PlayerPh::play(int pAoType, QString pFile, int pLoop, bool pPrior)
{
    ///IDE_TRACE_STR(pFile);
    QFileInfo tmpFileInfo(pFile);
    if(!tmpFileInfo.exists())
    {
        IDE_TRACE_STR(pFile);
        return false;
    }
    if(!m_Player)
    {
        IDE_DEBUG(QString("Can't Play ao type: %1").arg(pAoType));
        return false;
    }

    int tmpVolume = getVolume(pAoType);
    //IDE_DEBUG(QString("Play ao track[%1] type: %2 file=%3 volume=%4 loop=%5!").arg(pTrack).arg(pAoType).arg(pFile).arg(tmpVolume).arg(pLoop));
    return m_Player->addMedia(pAoType, pFile, pLoop, pPrior, tmpVolume);
}

void PlayerPh::clear()
{
    if(m_Player)
    {
        m_Player->clearMediaBuffer();
    }
}

void PlayerPh::updatePara(QObject* pObject)
{

}

bool PlayerPh::setValue(QString pValue)
{
    return true;
}
