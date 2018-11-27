#include "basemedia.h"

basemedia::basemedia(QObject *parent) :
    QObject(parent)
{
    m_PlayerMapper = new QSignalMapper;
    connect(m_PlayerMapper, SIGNAL(mapped(QObject *)), this, SIGNAL(sFinished(QObject *)));
}

basemedia::~basemedia()
{
    ClearPlayers();
}

void basemedia::ClearPlayers()
{
    MEDIA_PLAYER *tmpPlayer = 0;
    foreach(tmpPlayer, m_PlayerList)
    {
        if(tmpPlayer)
            tmpPlayer->deleteLater();
    }
    m_PlayerList.clear();
}

MEDIA_PLAYER *basemedia::AddPlayer(int pID, QString pConfFile)
{
    MEDIA_PLAYER* tmpPlayer = m_PlayerList.value(pID);
    if(!tmpPlayer)
        tmpPlayer = new MEDIA_PLAYER(this);
    tmpPlayer->SetConfFile(pConfFile);
    m_PlayerList.insert(pID, tmpPlayer);
    connect(tmpPlayer, SIGNAL(sFinished()), m_PlayerMapper, SLOT(map()));
    m_PlayerMapper->setMapping(tmpPlayer, tmpPlayer);
    return tmpPlayer;
}

MEDIA_PLAYER *basemedia::AddPlayer(int pID, QString pVoDriver, QRect pVoRect, int pVoRotate, QString pAoDriver, int pVolume)
{
    MEDIA_PLAYER* tmpPlayer = m_PlayerList.value(pID);
    if(!tmpPlayer)
        tmpPlayer = new MEDIA_PLAYER(this);
    tmpPlayer->SetVoDriver(pVoDriver);
    tmpPlayer->SetRect(pVoRect, pVoRotate);
    tmpPlayer->SetAoDriver(pAoDriver);
    tmpPlayer->SetVolume(AO_NONE, pVolume);
    m_PlayerList.insert(pID, tmpPlayer);
    connect(tmpPlayer, SIGNAL(sFinished()), m_PlayerMapper, SLOT(map()));
    m_PlayerMapper->setMapping(tmpPlayer, tmpPlayer);
    return tmpPlayer;
}

MEDIA_PLAYER *basemedia::AddPlayer(int pID, QString pAoDriver, int pVolume)
{
    MEDIA_PLAYER* tmpPlayer = m_PlayerList.value(pID);
    if(!tmpPlayer)
        tmpPlayer = new MEDIA_PLAYER(this);
    tmpPlayer->SetAoDriver(pAoDriver);
    tmpPlayer->SetVolume(AO_NONE, pVolume);
    m_PlayerList.insert(pID, tmpPlayer);
    connect(tmpPlayer, SIGNAL(sFinished()), m_PlayerMapper, SLOT(map()));
    m_PlayerMapper->setMapping(tmpPlayer, tmpPlayer);
    return tmpPlayer;
}

MEDIA_PLAYER *basemedia::GetPlayer(int pID)
{
    return m_PlayerList.value(pID);
}


