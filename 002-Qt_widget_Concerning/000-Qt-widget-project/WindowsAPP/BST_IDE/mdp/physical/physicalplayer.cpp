#include "physicalplayer.h"
#ifdef IDE
    #include "tabpage.h"
    #include "mdppage.h"
#endif
#include "deviceinc.h"

PhysicalPlayer::PhysicalPlayer(QObject *parent) :
    PhysicalDevice(parent)
{
    mSoundOn = true;
#ifdef DISPLAY
    ClosePlayer();
#endif

    for(int i=0;i<AOTRACK_MAXNUM;i++)
        mPlayerList[i] = 0;

    mStartMapper = new QSignalMapper;
    connect(mStartMapper, SIGNAL(mapped(int)), this, SLOT(slot_AudioStarted(int)));

    mFinishMapper = new QSignalMapper;
    connect(mFinishMapper, SIGNAL(mapped(int)), this, SLOT(slot_AudioFinished(int)));
}

PhysicalPlayer::~PhysicalPlayer()
{
    for(int i=0;i<AOTRACK_MAXNUM;i++)
    {
        if(mPlayerList[i])
            mPlayerList[i]->deleteLater();
    }
}

//>@如果U盘插入，则进行插播音视频
void PhysicalPlayer::SpotsAdMedia(bool pPrior, bool pImmediately)
{
    //>@加载背景音乐，背景音乐与视频互斥
    MEDIA_PLAYER *tmpPlayer = mAoRelationHash.value(AO_ADMUSIC);
    if(tmpPlayer)
    {
        tmpPlayer->SetLoop(LOOP_LIST);
        for(int m=0;m<mAdRcList.count();m++)
        {
            QString tmpFile = mAdRcList.at(m);
            RC_TYPE tmpType = getRcType(tmpFile);
            if(tmpType == RC_INVALID)
                continue;
            if(!tmpPlayer->CanPlay(tmpFile))
                continue;
#ifdef N329
            if(mAoParas[AO_ADMUSIC].mEnable[0] && (tmpType == RC_WAV))
#else
            if(mAoParas[AO_ADMUSIC].mEnable[0] && (tmpType == RC_MP3 || tmpType == RC_WAV))
#endif
#ifdef SODIMAS
                //>@如果是CANOPEN，默认背景音量为0，必须等到接收到第一帧设置背景音乐的指令后才会开始播放音乐
                if(m_DevManager->m_PHDevList.contains(P_COM))
                    tmpPlayer->Add(AO_ADMUSIC, tmpFile, LOOP_NONE, pPrior, 0);
                else
                    tmpPlayer->Add(AO_ADMUSIC, tmpFile, LOOP_NONE, pPrior, GetVolume(AO_ADMUSIC));
#else
                tmpPlayer->Add(AO_ADMUSIC, tmpFile, LOOP_NONE, pPrior, GetVolume(AO_ADMUSIC));
#endif
        }
        if(pImmediately)
        {
            tmpPlayer->PlayHead();
        }
        return;
    }
    tmpPlayer = mAoRelationHash.value(AO_VIDEO);
    if(tmpPlayer)
    {
        tmpPlayer->SetLoop(LOOP_LIST);
        for(int m=0;m<mAdRcList.count();m++)
        {
            QString tmpFile = mAdRcList.at(m);
            RC_TYPE tmpType = getRcType(tmpFile);
            if(tmpType == RC_INVALID)
                continue;
            if(!tmpPlayer->CanPlay(tmpFile))
                continue;
            if(mAoParas[AO_VIDEO].mEnable[0] && ((tmpType == RC_AVI) || (tmpType == RC_MP4)))
                tmpPlayer->Add(AO_VIDEO, tmpFile, LOOP_NONE, pPrior, GetVolume(AO_VIDEO));
        }
        if(pImmediately)
        {
            tmpPlayer->PlayHead();
        }
        return;
    }
}

void PhysicalPlayer::slot_MobileDiskChange()
{
#ifdef DISPLAY
    PhysicalMobileDisk *tmpMDisk = VAR_CAST<PhysicalMobileDisk*>(sender());
    if(tmpMDisk)
    {
        MEDIA_PLAYER *tmpPlayer = mAoRelationHash.value(AO_VIDEO);
        if(!tmpPlayer)
            return;
        QString tmpMediaPath = tmpMDisk->mDevPath + QString("MEDIA/");
        if(tmpMediaPath.compare(mAdAddress, Qt::CaseInsensitive))
            return;
        if(tmpMDisk->mDevState == S_ON)
        {
            //>@如果没有播放U盘中的视频，则开始播放U盘中的视频
            mAdRcList << getAllFileInFloders(tmpMediaPath);
            mAdRcList.removeDuplicates();
            SpotsAdMedia(true, true);
        }
        else
        {
            PLAY_MEDIA_INFO *tmpMedia = 0;
            foreach(tmpMedia, tmpPlayer->m_MediaBuffer)
            {
                if(!tmpMedia)
                    continue;
                QString tmpPath = tmpMedia->mPath;
                if(!tmpPath.startsWith(tmpMediaPath, Qt::CaseInsensitive))
                    continue;
                mAdRcList.removeOne(tmpPath);
                tmpPlayer->m_MediaBuffer.removeOne(tmpMedia);
                delete tmpMedia;
            }
            //>@如果正在播放U盘中的视频，则删除所有U盘中的视频
            if(tmpPlayer->m_CurMedia)
            {
                QString tmpPath = tmpPlayer->m_CurMedia->mPath;
                if(tmpPath.startsWith(tmpMediaPath, Qt::CaseInsensitive))
                {
                    mAdRcList.removeOne(tmpPath);
                    tmpPlayer->PlayNext();
                    return;
                }
                else
                    return;
            }
        }
    }
#endif
}

void PhysicalPlayer::slot_Initialize()
{
    IODeviceBase::slot_Initialize();

    IDE_DEBUG("Player");

    //>@在initial之后在遍历广告媒体文件夹。
    mAdRcList.clear();
    if(!mAdAddress.isEmpty())
    {
        mAdRcList = getAllFileInFloders(mAdAddress);
        mAdRcList.removeDuplicates();
    }

#ifdef IDE
    disconnect(m_DevManager, SIGNAL(sBeep(QString)), 0, 0);
    connect(m_DevManager, SIGNAL(sBeep(QString)), this, SLOT(slot_Play(QString)));
#elif DISPLAY
    disconnect(m_DevManager, SIGNAL(sBeep(QString)), 0, 0);
    connect(m_DevManager, SIGNAL(sBeep(QString)), this, SLOT(slot_Play(QString)));
#endif

#ifdef DISPLAY
    PhysicalMobileDisk *tmpMDisk = VAR_CAST<PhysicalMobileDisk *>(m_DevManager->m_PHDevList.value(P_MDISK));
    if(tmpMDisk)
    {
        connect(tmpMDisk, SIGNAL(sDeviceStateChanged()), this, SLOT(slot_MobileDiskChange()));
    }
#endif

    SpotsAdMedia();
}

bool PhysicalPlayer::SetEnable(bool pFlag, E_DEVMODE pDevMode, bool pForce)
{
    bool tmpEnable = IODeviceBase::SetEnable(pFlag, pDevMode);
    if(pFlag)
    {
        if(mInitialized)
        {
            if(tmpEnable)
            {
                IDE_TRACE();
                Restore();
            }

            //>@当从网络连接模式退出时，需要重新初始化可能被修改的参数
            if(pDevMode == DM_NET)
            {
                if(mAoVolsBk[0] != mAoParas[0].mVolume.mNum)
                    SetPara(QString("LiftFlrVol"), QString::number(mAoVolsBk[0]), true);
                if(mAoVolsBk[1] != mAoParas[1].mVolume.mNum)
                    SetPara(QString("LiftArvVol"), QString::number(mAoVolsBk[1]), true);
                if(mAoVolsBk[2] != mAoParas[2].mVolume.mNum)
                    SetPara(QString("BeepVol"), QString::number(mAoVolsBk[2]), true);
                if(mAoVolsBk[3] != mAoParas[3].mVolume.mNum)
                    SetPara(QString("AdMusicVol"), QString::number(mAoVolsBk[3]), true);
                if(mAoVolsBk[4] != mAoParas[4].mVolume.mNum)
                    SetPara(QString("AdVideoVol"), QString::number(mAoVolsBk[4]), true);
            }
        }
    }
    else
    {
        if(tmpEnable)
        {
            IDE_TRACE();
#if 1
            Abort();
#else
            //>@只关闭背景音乐即可
            Abort(AO_ADMUSIC);
#endif
        }

        //>@当进入网络连接模式时，需要保存可能被修改的参数
        if(pDevMode == DM_NET)
        {
            for(int i=0;i<AO_MAXNUM;i++)
                mAoVolsBk[i] = mAoParas[i].mVolume.mNum;
        }
    }
    return true;
}

//LiftFlr,LiftArv,Beep:/ao_lift.conf;
void PhysicalPlayer::InitPlayer()
{
    mAoRelationHash.clear();
    QStringList tmpAoDrvList = mAoDriver.split(';');
    IDE_TRACE_STR(mAoDriver);
    int count = qMin(tmpAoDrvList.count(), AOTRACK_MAXNUM);
    for(int i=0;i<count;i++)
    {
        //>@判断是否为conf文件
        QString tmpString = tmpAoDrvList.at(i);
        if(tmpString.isEmpty())
            continue;
        int index = tmpString.indexOf(AOTYPECONF_SPLIT);
        if(index < 0)
            continue;
        QStringList tmpAoList = tmpString.left(index).split(AOTYPE_SPLIT);
        if(!tmpAoList.isEmpty())
        {
            QString tmpParameter = tmpString.mid(index+1);
            bool isConf = tmpParameter.endsWith(".conf", Qt::CaseInsensitive);
            IDE_TRACE_STR(tmpParameter);
            //>@建立声道
            if(mPlayerList[i])
                mPlayerList[i]->deleteLater();
            mPlayerList[i] = new MEDIA_PLAYER;
            if(isConf)
            {
                mPlayerList[i]->SetConfFile(tmpParameter);
            }
            else
            {
                QStringList tmpDrvList = tmpParameter.split(',');
                if(tmpDrvList.count() != 2)
                {
                    IDE_DEBUG(QString("Track[%1] : driver error!").arg(i));
                    continue;
                }
                mPlayerList[i]->SetAoDriver(tmpDrvList.at(0));
                mPlayerList[i]->SetMixerDriver(tmpDrvList.at(1));
            }
            connect(mPlayerList[i], SIGNAL(sStarted()), mStartMapper, SLOT(map()));
            mStartMapper->setMapping(mPlayerList[i], i);
            connect(mPlayerList[i], SIGNAL(sFinished()), mFinishMapper, SLOT(map()));
            mFinishMapper->setMapping(mPlayerList[i], i);
            mPlayerList[i]->SetTrackNum(i);
            //>@建立音频类型与声道的关系
            for(int j=0;j<tmpAoList.count();j++)
            {
                AO_TYPE tmpType = GetAoType(tmpAoList.at(j));
                if(tmpType == AO_NONE)
                    continue;
                mAoParas[tmpType].mEnable[0] = mAoParas[tmpType].mEnable[1] = true;
                mPlayerList[i]->SetVolume(tmpType, mAoParas[tmpType].GetVolume(), true);
                mAoRelationHash.insert(tmpType, mPlayerList[i]);
                mTrackRelationHash.insert(i, tmpType);
            }
        }
    }
}

void PhysicalPlayer::slot_Play(QString pBeepName)
{
    Play(AO_BEEP, D_FSRCPATH + D_BOOTDIR + QString("%1.wav").arg(pBeepName), LOOP_NONE, false, true);
}

void PhysicalPlayer::slot_Play(int pAoType, QString pFile, int pLoop, bool pPrior, bool pMultiTrack)
{
    Play(pAoType, pFile, pLoop, pPrior, pMultiTrack);
}

void PhysicalPlayer::slot_Play(int pAoType, QString pFile, int pVolume, int pLoop, bool pPrior, bool pMultiTrack)
{
    int oldVolume = mAoParas[pAoType].mVolume.mNum;
    qreal oldDiv = mAoParas[pAoType].mScaler;
    mAoParas[pAoType].mScaler = 1;
    mAoParas[pAoType].mVolume.setnum(pVolume);
    Play(pAoType, pFile, pLoop, pPrior, pMultiTrack);
    mAoParas[pAoType].mVolume.setnum(oldVolume);
    mAoParas[pAoType].mScaler = oldDiv;
}

void PhysicalPlayer::SetVolume(int pId, int pVolume)
{
    if(!mPlayerList[pId])
        return;
    mPlayerList[pId]->SetVolume(AO_NONE, pVolume, true);
}

void PhysicalPlayer::SetScaler(qreal pScaler, bool pSync)
{
    for(int i=0;i<AO_MAXNUM;i++)
        SetScaler(i, pScaler, pSync);
}

void PhysicalPlayer::SetScaler(int pAoType, qreal pScaler, bool pSync)//此处的pSync与tmpPlayer->SetVolume的sync不同。
{
    mAoParas[pAoType].mScaler = pScaler;
    if(pSync)
    {
        MEDIA_PLAYER *tmpPlayer = mAoRelationHash.value(pAoType);
        if(tmpPlayer)
            tmpPlayer->SetVolume(pAoType, mAoParas[pAoType].GetVolume(), false);
    }
}

int PhysicalPlayer::GetVolume(int pAoType)
{
    return mAoParas[pAoType].GetVolume();
}

bool PhysicalPlayer::Play(int pAoType, QString pFile, int pLoop, bool pPrior, bool pTrack)
{
    QFileInfo tmpFileInfo(pFile);
    if(!(tmpFileInfo.exists() && (tmpFileInfo.size()>1024)))
    {
        IDE_TRACE_STR(pFile);
        return false;
    }

    MEDIA_PLAYER *tmpPlayer = mAoRelationHash.value(pAoType);
    if(!(tmpPlayer && mAoParas[pAoType].mEnable[0]))
    {
        IDE_DEBUG(QString(">>>>Can't Play ao type: %1").arg(pAoType));
        return false;
    }

    int tmpVolume = GetVolume(pAoType);
    IDE_DEBUG(QString(">>>>Play ao track[%1] type: %2 file=%3 volume=%4 loop=%5!").arg(pTrack).arg(pAoType).arg(pFile).arg(tmpVolume).arg(pLoop));
    return tmpPlayer->Add(pAoType, pFile, pLoop, pPrior, tmpVolume, pTrack);
}

void PhysicalPlayer::slot_AudioStarted(int pTrack) //>@只要发出开始信号的，都是要让其它声道暂停的。
{
    PLAY_MEDIA_INFO *tmpMedia = mPlayerList[pTrack]->GetMedia();
    if(!tmpMedia)
        IDE_DEBUG(QString("Track[%1] is started!").arg(pTrack));
    else
        IDE_DEBUG(QString("Track[%1] %2 is started!").arg(pTrack).arg(tmpMedia->mPath));

    for(int i=0;i<AOTRACK_MAXNUM;i++)
    {
        if(pTrack == i)
            continue;
        if(!mPlayerList[i])
            continue;
        mPlayerList[i]->Pause(true);
    }
}

void PhysicalPlayer::slot_AudioFinished(int pTrack) //>@只要发出结束信号的，都是要让其它声道运行的。
{
    PLAY_MEDIA_INFO *tmpMedia = mPlayerList[pTrack]->GetMedia();
    if(!tmpMedia)
        IDE_DEBUG(QString("Track[%1] is finished!").arg(pTrack));
    else
        IDE_DEBUG(QString("Track[%1] %2 is finished!").arg(pTrack).arg(tmpMedia->mPath));
    for(int i=0;i<AOTRACK_MAXNUM;i++)
    {
        if(pTrack == i)
            continue;
        if(!mPlayerList[i])
            continue;
        mPlayerList[i]->Pause(false);
    }
}

void PhysicalPlayer::Clear(int pId)
{
    MEDIA_PLAYER* tmpPlayer = mPlayerList[pId];
    if(tmpPlayer)
    {
        tmpPlayer->Clear();
    }
}

void PhysicalPlayer::Restore(int pAoType, int pStopType)
{
//    if(!mEnable)
//        return;
    mAoParas[pAoType].mEnable[0] = mAoParas[pAoType].mEnable[1];
    IDE_TRACE_INT(mAoParas[pAoType].mEnable[0]);
    if(mAoParas[pAoType].mEnable[0])
    {
        MEDIA_PLAYER *tmpPlayer = mAoRelationHash.value(pAoType);
        if(tmpPlayer)
            tmpPlayer->Stop(false, pStopType);
    }
}

void PhysicalPlayer::Abort(int pAoType, int pStopType)
{
    MEDIA_PLAYER *tmpPlayer = mAoRelationHash.value(pAoType);
    if(tmpPlayer)
    {
        IDE_TRACE();
        tmpPlayer->Stop(true, pStopType);
    }

    mAoParas[pAoType].mEnable[0] = false;
}

//>@只要有一种音频有效，则整个声道有效
bool PhysicalPlayer::isTrackEnable(int pId)
{
    bool tmpEnable = false;
    QList<int> tmpList = mTrackRelationHash.values(pId);
    for(int j=0;j<tmpList.count();j++)
    {
        int id = tmpList.at(j);
        if(mAoParas[id].mEnable[0] == true)
        {
            tmpEnable = true;
            break;
        }
    }
    return tmpEnable;
}

void PhysicalPlayer::Restore(int pStopType)
{
    if(!(mEnable && mSoundOn))
        return;

    for(int i=0;i<AO_MAXNUM;i++)
        mAoParas[i].mEnable[0] = mAoParas[i].mEnable[1];

    for(int i=0;i<AOTRACK_MAXNUM;i++)
    {
        IDE_TRACE_INT(i);
        MEDIA_PLAYER *tmpTrackPlayer = mPlayerList[i];
        if(!tmpTrackPlayer || !isTrackEnable(i))
            continue;
        tmpTrackPlayer->Stop(false, pStopType);  //>@仅仅是设置enable，但如果pause依旧为true，则仍然不播放。
        IDE_TRACE_INT(i);
        QtSleep(100);
    }
}

void PhysicalPlayer::Abort(int pStopType)
{
    for(int i=0;i<AOTRACK_MAXNUM;i++)
    {
        if(mPlayerList[i])
        {
            //IDE_TRACE_INT(i);
            mPlayerList[i]->Stop(true, pStopType);
        }
    }

    for(int i=0;i<AO_MAXNUM;i++)
        mAoParas[i].mEnable[0] = false;
}

QString PhysicalPlayer::GetCurMedia(int pId, bool &pRunning)
{
    PLAY_MEDIA_INFO *tmpMedia = 0;
    if(mPlayerList[pId] && (tmpMedia = mPlayerList[pId]->GetMedia()))
    {
        pRunning = mPlayerList[pId]->IsRunning();
        return tmpMedia->mPath;
    }
    return QString();
}

quint32 PhysicalPlayer::InitPara()
{
    QDomNodeList tmpParaNodeList = m_ParaElement.childNodes();
    if(tmpParaNodeList.count() < 1)
        return 0;
    quint32 validcount = 0;
    QHBoxLayout* tmpHBoxLayout;
    for(int i=0;i<tmpParaNodeList.count();i++)
    {
        QDomElement tmpElement = tmpParaNodeList.at(i).toElement();
        if(tmpElement.isNull())
            continue;
        QString tmpName = tmpElement.tagName();
        if(!tmpName.compare("AoDriver"))
        {
            mAoDriver = tmpElement.text();
            tmpHBoxLayout = CreateTextEdit(tmpName, mAoDriver);
        }
        else if(!tmpName.compare("LiftFlrVol"))
        {
            QStringList tmpValueList = tmpElement.attribute("List").split(';');
            if(tmpValueList.count()==2)
            {
                mAoParas[AO_LIFTFLR].mVolume.setnum(tmpElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
            }
            mAoVolsBk[AO_LIFTFLR] = mAoParas[AO_LIFTFLR].mVolume.mNum;
            tmpHBoxLayout = CreateNum(tmpName, mAoParas[AO_LIFTFLR].mVolume.mNum, mAoParas[AO_LIFTFLR].mVolume.mMinNum, mAoParas[AO_LIFTFLR].mVolume.mMaxNum);
        }
        else if(!tmpName.compare("LiftArvVol"))
        {
            QStringList tmpValueList = tmpElement.attribute("List").split(';');
            if(tmpValueList.count()==2)
            {
                mAoParas[AO_LIFTARV].mVolume.setnum(tmpElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
            }
            mAoVolsBk[AO_LIFTARV] = mAoParas[AO_LIFTARV].mVolume.mNum;
            tmpHBoxLayout = CreateNum(tmpName, mAoParas[AO_LIFTARV].mVolume.mNum, mAoParas[AO_LIFTARV].mVolume.mMinNum, mAoParas[AO_LIFTARV].mVolume.mMaxNum);
        }
        else if(!tmpName.compare("BeepVol"))
        {
            QStringList tmpValueList = tmpElement.attribute("List").split(';');
            if(tmpValueList.count()==2)
            {
                mAoParas[AO_BEEP].mVolume.setnum(tmpElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
            }
            mAoVolsBk[AO_BEEP] = mAoParas[AO_BEEP].mVolume.mNum;
            tmpHBoxLayout = CreateNum(tmpName, mAoParas[AO_BEEP].mVolume.mNum, mAoParas[AO_BEEP].mVolume.mMinNum, mAoParas[AO_BEEP].mVolume.mMaxNum);
        }
        else if(!tmpName.compare("AdMusicVol"))
        {
            QStringList tmpValueList = tmpElement.attribute("List").split(';');
            if(tmpValueList.count()==2)
            {
                mAoParas[AO_ADMUSIC].mVolume.setnum(tmpElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
            }
            mAoVolsBk[AO_ADMUSIC] = mAoParas[AO_ADMUSIC].mVolume.mNum;
            tmpHBoxLayout = CreateNum(tmpName, mAoParas[AO_ADMUSIC].mVolume.mNum, mAoParas[AO_ADMUSIC].mVolume.mMinNum, mAoParas[AO_ADMUSIC].mVolume.mMaxNum);
        }
        else if(!tmpName.compare("VideoVol"))
        {
            QStringList tmpValueList = tmpElement.attribute("List").split(';');
            if(tmpValueList.count()==2)
            {
                mAoParas[AO_VIDEO].mVolume.setnum(tmpElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
            }
            mAoVolsBk[AO_VIDEO] = mAoParas[AO_VIDEO].mVolume.mNum;
            tmpHBoxLayout = CreateNum(tmpName, mAoParas[AO_VIDEO].mVolume.mNum, mAoParas[AO_VIDEO].mVolume.mMinNum, mAoParas[AO_VIDEO].mVolume.mMaxNum);
        }
        else if(!tmpName.compare("AdAddr"))  //>@指示广告媒体的地址，可以是移动磁盘地址。
        {
            mAdAddress = tmpElement.text();
            mAdAddress.replace("\\", "/");
            if(!mAdAddress.endsWith("/"))
                mAdAddress.append("/");
            tmpHBoxLayout = CreateTextEdit(tmpName, mAdAddress);
        }
        else
        {
            continue;
        }
        validcount++;
        m_SettingGroup->m_VLayout->addWidget(CreateLine());
        m_SettingGroup->m_VLayout->addLayout(tmpHBoxLayout);
    }
    //>@
#ifdef DISPLAY
    InitPlayer();
#endif
    return validcount;
}

void PhysicalPlayer::UpdatePara(QObject* pObject)
{
    QString tmpObjectName = pObject->objectName();
    QDomElement tmpElement = m_ParaElement.firstChildElement(tmpObjectName);
    if(tmpElement.isNull())
        return;
    QString tmpName = tmpElement.tagName();
    if(!tmpName.compare("AoDriver"))
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            if(ModifyTextEdit(tmpComponent, tmpElement, mAoDriver, tmpComponent->text()))
                InitPlayer();
        }
    }
    else if(!tmpName.compare("LiftFlrVol"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, tmpElement, mAoParas[AO_LIFTFLR].mVolume.mNum, tmpComponent->value(), mAoParas[AO_LIFTFLR].mVolume.mMinNum, mAoParas[AO_LIFTFLR].mVolume.mMaxNum))
            {
            }
            else
            {
                MEDIA_PLAYER *tmpPlayer = mAoRelationHash.value(AO_LIFTFLR);
                if(tmpPlayer)
                    tmpPlayer->SetVolume(AO_LIFTFLR, mAoParas[AO_LIFTFLR].mVolume.mNum, true);
            }
        }
    }
    else if(!tmpName.compare("LiftArvVol"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, tmpElement, mAoParas[AO_LIFTARV].mVolume.mNum, tmpComponent->value(), mAoParas[AO_LIFTARV].mVolume.mMinNum, mAoParas[AO_LIFTARV].mVolume.mMaxNum))
            {
            }
            else
            {
                MEDIA_PLAYER *tmpPlayer = mAoRelationHash.value(AO_LIFTARV);
                if(tmpPlayer)
                    tmpPlayer->SetVolume(AO_LIFTARV, mAoParas[AO_LIFTARV].mVolume.mNum, true);
            }
        }
    }
    else if(!tmpName.compare("BeepVol"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, tmpElement, mAoParas[AO_BEEP].mVolume.mNum, tmpComponent->value(), mAoParas[AO_BEEP].mVolume.mMinNum, mAoParas[AO_BEEP].mVolume.mMaxNum))
            {
            }
            else
            {
                MEDIA_PLAYER *tmpPlayer = mAoRelationHash.value(AO_BEEP);
                if(tmpPlayer)
                    tmpPlayer->SetVolume(AO_BEEP, mAoParas[AO_BEEP].mVolume.mNum, true);
            }
        }
    }
    else if(!tmpName.compare("AdMusicVol"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, tmpElement, mAoParas[AO_ADMUSIC].mVolume.mNum, tmpComponent->value(), mAoParas[AO_ADMUSIC].mVolume.mMinNum, mAoParas[AO_ADMUSIC].mVolume.mMaxNum))
            {
            }
            else
            {
                MEDIA_PLAYER *tmpPlayer = mAoRelationHash.value(AO_ADMUSIC);
                if(tmpPlayer)
                    tmpPlayer->SetVolume(AO_ADMUSIC, mAoParas[AO_ADMUSIC].mVolume.mNum, true);
            }
        }
    }
    else if(!tmpName.compare("VideoVol"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, tmpElement, mAoParas[AO_VIDEO].mVolume.mNum, tmpComponent->value(), mAoParas[AO_VIDEO].mVolume.mMinNum, mAoParas[AO_VIDEO].mVolume.mMaxNum))
            {
            }
            else
            {
                MEDIA_PLAYER *tmpPlayer = mAoRelationHash.value(AO_VIDEO);
                if(tmpPlayer)
                    tmpPlayer->SetVolume(AO_VIDEO, mAoParas[AO_VIDEO].mVolume.mNum, true);
            }
        }
    }
    else if(!tmpName.compare("AdAddr"))
    {
        TextEdit *tmpComponent = VAR_CAST<TextEdit *>(pObject);
        if(tmpComponent)
        {
            ModifyTextEdit(tmpComponent, tmpElement, mAdAddress, tmpComponent->text());
            if(!mAdAddress.isEmpty())
            {
                mAdRcList = getAllFileInFloders(mAdAddress);
                mAdRcList.removeDuplicates();
            }
            else
                mAdRcList.clear();
        }
    }
}

bool PhysicalPlayer::SetValue(QString pValue)
{
    if(pValue.isEmpty())
        return true;
    QStringList tmpList = pValue.split(';');
    for(int i=0;i<tmpList.count();i++)
    {
        QStringList tmpCmdList = tmpList.at(i).split(':');
        if(tmpCmdList.count()<2)
            continue;
        AO_TYPE tmpType = GetAoType(tmpCmdList.at(0));
        if(tmpType == AO_NONE)
            continue;
        MEDIA_PLAYER *tmpPlayer = mAoRelationHash.value(tmpType);
        if(!tmpPlayer)
            continue;
        QString tmpPara = tmpCmdList.at(1);
        if(!tmpPara.compare("on", Qt::CaseInsensitive))  //>@表明开启
        {
            mAoParas[tmpType].mEnable[0] = true;
        }
        else if(!tmpPara.compare("off", Qt::CaseInsensitive))
        {
            mAoParas[tmpType].mEnable[0] = false;
        }
        else if(!tmpPara.compare("bip", Qt::CaseInsensitive))
        {
            if(mAoParas[tmpType].mEnable[0])
            {
                tmpPlayer->Add(tmpType, D_FSRCPATH + D_BOOTDIR + QString("beep.wav"), (int)LOOP_NONE, true, mAoParas[tmpType].GetVolume());
            }
        }
        else if(!tmpPara.compare("div", Qt::CaseInsensitive))  //>@表明开启
        {
            IDE_TRACE_INT(tmpType);
            if(tmpCmdList.count()>=3)  //>@AdMusic:div:%d:sync(表示是否同步更改正在播放的音频)
            {
                int tmpDiv = tmpCmdList.at(2).toInt();
                IDE_TRACE_INT(tmpDiv);
                bool tmpSync = false;
                if(tmpCmdList.count() >= 4)
                    tmpSync = (bool)(tmpCmdList.at(3).compare("sync", Qt::CaseInsensitive) == 0);
                IDE_TRACE_INT(tmpSync);
                if(tmpDiv == 0)
                    SetScaler(tmpType, 0, tmpSync);
                else
                    SetScaler(tmpType, (qreal)((qreal)(9-tmpDiv)/8.0), tmpSync);
            }
        }
        else if(!tmpPara.compare("next", Qt::CaseInsensitive))
        {
            if(mAoParas[tmpType].mEnable[0])
                tmpPlayer->Playback(LOOP_NONE);
        }
        else if(!tmpPara.compare("test", Qt::CaseInsensitive))
        {
            if(tmpCmdList.count()>=3)
            {
                tmpPlayer->SetEnable(true);
                tmpPlayer->TestMedia(tmpType, D_FSRCPATH + D_BOOTDIR + QString("test.wav"), tmpCmdList.at(2).toInt());
            }
        }
        else
        {
            if(mAoParas[tmpType].mEnable[0])
                tmpPlayer->Add(tmpType, D_FSRCPATH + D_BOOTDIR + QString("%1.wav").arg(tmpPara), (int)LOOP_NONE, false, mAoParas[tmpType].GetVolume());
        }
    }
    return true;
}



