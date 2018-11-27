#include "playerph.h"

PlayerPh::PlayerPh(GlobalSetting *globalSetting) :
    PhysicBase(globalSetting)
{
    mSoundOn = true;
    for(int i=0;i<AOTRACK_MAXNUM;i++){
        mPlayerList[i] = 0;
    }
    mStartMapper = new QSignalMapper;
    connect(mStartMapper, SIGNAL(mapped(int)), this, SLOT(slot_AudioStarted(int)));

    mFinishMapper = new QSignalMapper;
    connect(mFinishMapper, SIGNAL(mapped(int)), this, SLOT(slot_AudioFinished(int)));
}

PlayerPh::~PlayerPh()
{
    for(int i=0;i<AOTRACK_MAXNUM;i++)
    {
        if(mPlayerList[i])
            mPlayerList[i]->deleteLater();
    }
}

bool PlayerPh::initDevice(PH_Type pPHType, QDomElement pElement)
{
    m_NodeElement = pElement;
    QDomNodeList tmpParaNodeList = m_NodeElement.childNodes();
    if(tmpParaNodeList.count() < 1)
        return 0;
    quint32 validcount = 0;
    for(int i = 0; i<tmpParaNodeList.count(); i++)
    {
        QDomElement tmpElement = tmpParaNodeList.at(i).toElement();
        if(tmpElement.isNull())
            continue;
        QString tmpName = tmpElement.tagName();

        if(!tmpName.compare("liftflrvol"))
        {
            QStringList tmpValueList = tmpElement.attribute("list").split(';');
            if(tmpValueList.count() == 2)
            {
                mAoParas[AO_LIFTFLR].mVolume.setnum(tmpElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
            }
            mAoVolsBk[AO_LIFTFLR] = mAoParas[AO_LIFTFLR].mVolume.mNum;

        }
        else if(!tmpName.compare("liftarvvol"))
        {
            QStringList tmpValueList = tmpElement.attribute("list").split(';');
            if(tmpValueList.count()==2)
            {
                mAoParas[AO_LIFTARV].mVolume.setnum(tmpElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
            }
            mAoVolsBk[AO_LIFTARV] = mAoParas[AO_LIFTARV].mVolume.mNum;

        }
        else if(!tmpName.compare("beepvol"))
        {
            QStringList tmpValueList = tmpElement.attribute("list").split(';');
            if(tmpValueList.count() == 2)
            {
                mAoParas[AO_BEEP].mVolume.setnum(tmpElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
            }
            mAoVolsBk[AO_BEEP] = mAoParas[AO_BEEP].mVolume.mNum;

        }
        else if(!tmpName.compare("admusicvol"))
        {
            QStringList tmpValueList = tmpElement.attribute("list").split(';');
            if(tmpValueList.count() == 2)
            {
                mAoParas[AO_ADMUSIC].mVolume.setnum(tmpElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
            }
            mAoVolsBk[AO_ADMUSIC] = mAoParas[AO_ADMUSIC].mVolume.mNum;
        }
        else
        {
            continue;
        }
        validcount++;
    }

    initPlayer();

}


void PlayerPh::slot_Initialize()
{
    //disconnect(m_DevManager, SIGNAL(sBeep(QString)), 0, 0);
    //connect(m_DevManager, SIGNAL(sBeep(QString)), this, SLOT(slot_Play(QString)));
}

bool PlayerPh::setEnable(DEV_MODE pDevMode, bool pFlag, bool pForce)
{
/*
    bool tmpEnable = PhysicBase::setEnable(pFlag);
    if(pFlag)
    {
        if(m_initialized)
        {
            if(m_enabled)
            {
                IDE_TRACE();
                restore();
            }
            //当从网络连接模式退出时，需要重新初始化可能被修改的参数
            if(pDevMode == DM_NET)
            {
                if(mAoVolsBk[0] != mAoParas[0].mVolume.mNum)
                    setPara(QString("liftflrvol"), QString::number(mAoVolsBk[0]), true);
                if(mAoVolsBk[1] != mAoParas[1].mVolume.mNum)
                    setPara(QString("liftarvvol"), QString::number(mAoVolsBk[1]), true);
                if(mAoVolsBk[2] != mAoParas[2].mVolume.mNum)
                    setPara(QString("beepvol"), QString::number(mAoVolsBk[2]), true);
                if(mAoVolsBk[3] != mAoParas[3].mVolume.mNum)
                    setPara(QString("admusicvol"), QString::number(mAoVolsBk[3]), true);
            }
        }
    }
    else
    {
        if(tmpEnable)
        {
            IDE_TRACE();
            abort();
        }

        //当进入网络连接模式时，需要保存可能被修改的参数
        if(pDevMode == DM_NET)
        {
            for(int i=0;i<AO_MAXNUM;i++)
                mAoVolsBk[i] = mAoParas[i].mVolume.mNum;
        }
    }
*/
    return true;
}

void PlayerPh::initPlayer()
{
    mAoRelationHash.clear();
    mAoDriver = "liftflr,liftarv,beep@/dev/dsp,/dev/mixer;admusic@/dev/dsp2,/dev/mixer2";
    QStringList tmpAoDrvList = mAoDriver.split(';');
    IDE_TRACE_STR(mAoDriver);
    int count = qMin(tmpAoDrvList.count(), AOTRACK_MAXNUM);
    for(int i = 0; i < count; i++)
    {
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
            //建立声道
            if(mPlayerList[i])
                mPlayerList[i]->deleteLater();
            mPlayerList[i] = new MediaPlayer;

            QStringList tmpDrvList = tmpParameter.split(',');
            if(tmpDrvList.count() != 2)
            {
                IDE_DEBUG(QString("Track[%1] : driver error!").arg(i));
                continue;
            }
            mPlayerList[i]->setAoDriver(tmpDrvList.at(0));
            mPlayerList[i]->setMixerDriver(tmpDrvList.at(1));

            connect(mPlayerList[i], SIGNAL(sStarted()), mStartMapper, SLOT(map()));
            mStartMapper->setMapping(mPlayerList[i], i);
            connect(mPlayerList[i], SIGNAL(sFinished()), mFinishMapper, SLOT(map()));
            mFinishMapper->setMapping(mPlayerList[i], i);
            mPlayerList[i]->setTrackNum(i);
            //>@建立音频类型与声道的关系
            for(int j = 0; j < tmpAoList.count(); j++)
            {
                AO_TYPE tmpType = GetAoType(tmpAoList.at(j));
                if(tmpType == AO_NONE)
                    continue;
                mAoParas[tmpType].mEnable[0] = mAoParas[tmpType].mEnable[1] = true;
                int tmpVolume = mAoParas[tmpType].GetVolume();
                mPlayerList[i]->setVolume(tmpType, tmpVolume, true);
                mAoRelationHash.insert(tmpType, mPlayerList[i]);
                mTrackRelationHash.insert(i, tmpType);
            }
        }
    }
}

void PlayerPh::slot_Play(QString pFile)
{
    play(AO_BEEP, pFile, LOOP_NONE, false, true);
}

void PlayerPh::slot_Play(int pAoType, QString pFile, int pLoop, bool pPrior, bool pMultiTrack)
{
    play(pAoType, pFile, pLoop, pPrior, pMultiTrack);
}

void PlayerPh::slot_Play(int pAoType, QString pFile, int pVolume, int pLoop, bool pPrior, bool pMultiTrack)
{
    int oldVolume = mAoParas[pAoType].mVolume.mNum;
    qreal oldDiv = mAoParas[pAoType].mScaler;
    mAoParas[pAoType].mScaler = 1;
    mAoParas[pAoType].mVolume.setnum(pVolume);
    play(pAoType, pFile, pLoop, pPrior, pMultiTrack);
    mAoParas[pAoType].mVolume.setnum(oldVolume);
    mAoParas[pAoType].mScaler = oldDiv;
}

void PlayerPh::setVolume(int pId, int pVolume)
{
    if(!mPlayerList[pId])
        return;
    mPlayerList[pId]->setVolume(AO_NONE, pVolume, true);
}

void PlayerPh::setScaler(qreal pScaler, bool pSync)
{
    for(int i=0;i<AO_MAXNUM;i++)
        setScaler(i, pScaler, pSync);
}

//此处的pSync与tmpPlayer->SetVolume的sync不同。
void PlayerPh::setScaler(int pAoType, qreal pScaler, bool pSync)
{
    mAoParas[pAoType].mScaler = pScaler;
    if(pSync)
    {
        MediaPlayer *tmpPlayer = mAoRelationHash.value(pAoType);
        if(tmpPlayer)
            tmpPlayer->setVolume(pAoType, mAoParas[pAoType].GetVolume(), false);
    }
}

int PlayerPh::getVolume(int pAoType)
{
    return mAoParas[pAoType].GetVolume();
}

bool PlayerPh::play(int pAoType, QString pFile, int pLoop, bool pPrior, bool pTrack)
{
    QFileInfo tmpFileInfo(pFile);
    //if(!(tmpFileInfo.exists() && (tmpFileInfo.size()>1024)))
    if(!tmpFileInfo.exists())
    {
        IDE_TRACE_STR(pFile);
        return false;
    }

    MediaPlayer *tmpPlayer = mAoRelationHash.value(pAoType);
    if(!(tmpPlayer && mAoParas[pAoType].mEnable[0]))
    {
        IDE_DEBUG(QString("Can't Play ao type: %1").arg(pAoType));
        return false;
    }

    int tmpVolume = getVolume(pAoType);
    //IDE_DEBUG(QString("Play ao track[%1] type: %2 file=%3 volume=%4 loop=%5!").arg(pTrack).arg(pAoType).arg(pFile).arg(tmpVolume).arg(pLoop));
    return tmpPlayer->add(pAoType, pFile, pLoop, pPrior, tmpVolume, pTrack);
}

void PlayerPh::slot_AudioStarted(int pTrack) //只要发出开始信号的，都是要让其它声道暂停的。
{
    PLAY_MEDIA_INFO *tmpMedia = mPlayerList[pTrack]->getMedia();
    if(!tmpMedia){
        IDE_DEBUG(QString("Track[%1] is started!").arg(pTrack));
    }else{
        IDE_DEBUG(QString("Track[%1] %2 is started!").arg(pTrack).arg(tmpMedia->mPath));
    }

    for(int i = 0; i < AOTRACK_MAXNUM; i++)
    {
        if(pTrack == i)
            continue;
        if(!mPlayerList[i])
            continue;
        mPlayerList[i]->pause(true);
    }
}

void PlayerPh::slot_AudioFinished(int pTrack) //只要发出结束信号的，都是要让其它声道运行的。
{
    PLAY_MEDIA_INFO *tmpMedia = mPlayerList[pTrack]->getMedia();
    if(!tmpMedia){
        IDE_DEBUG(QString("Track[%1] is finished!").arg(pTrack));
    }else{
        IDE_DEBUG(QString("Track[%1] %2 is finished!").arg(pTrack).arg(tmpMedia->mPath));
    }
    for(int i = 0; i<AOTRACK_MAXNUM; i++)
    {
        if(pTrack == i)
            continue;
        if(!mPlayerList[i])
            continue;
        mPlayerList[i]->pause(false);
    }
}

void PlayerPh::clear(int pId)
{
    MediaPlayer* tmpPlayer = mPlayerList[pId];
    if(tmpPlayer)
    {
        tmpPlayer->clear();
    }
}

void PlayerPh::restore(int pAoType, int pStopType)
{
    if(!mEnable)
        return;
    mAoParas[pAoType].mEnable[0] = mAoParas[pAoType].mEnable[1];
    //IDE_TRACE_INT(mAoParas[pAoType].mEnable[0]);
    if(mAoParas[pAoType].mEnable[0])
    {
        MediaPlayer *tmpPlayer = mAoRelationHash.value(pAoType);
        if(tmpPlayer)
            tmpPlayer->stop(false, pStopType);
    }
}

void PlayerPh::abort(int pAoType, int pStopType)
{
    MediaPlayer *tmpPlayer = mAoRelationHash.value(pAoType);
    if(tmpPlayer)
    {
        IDE_TRACE();
        tmpPlayer->stop(true, pStopType);
    }

    mAoParas[pAoType].mEnable[0] = false;
}

//只要有一种音频有效，则整个声道有效
bool PlayerPh::isTrackEnable(int pId)
{
    bool tmpEnable = false;
    QList<int> tmpList = mTrackRelationHash.values(pId);
    for(int j = 0; j<tmpList.count(); j++)
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

void PlayerPh::restore(int pStopType)
{
    if(!(mEnable && mSoundOn))
        return;

    for(int i = 0; i < AO_MAXNUM; i++)
        mAoParas[i].mEnable[0] = mAoParas[i].mEnable[1];

    for(int i = 0; i<AOTRACK_MAXNUM; i++)
    {
        //IDE_TRACE_INT(i);
        MediaPlayer *tmpTrackPlayer = mPlayerList[i];
        if(!tmpTrackPlayer || !isTrackEnable(i))
            continue;
        tmpTrackPlayer->stop(false, pStopType);  //仅仅是设置enable，但如果pause依旧为true，则仍然不播放。
        //IDE_TRACE_INT(i);
        QtSleep(100);
    }
}

void PlayerPh::abort(int pStopType)
{
    for(int i = 0; i<AOTRACK_MAXNUM; i++)
    {
        if(mPlayerList[i])
        {
            //IDE_TRACE_INT(i);
            mPlayerList[i]->stop(true, pStopType);
        }
    }

    for(int i = 0; i<AO_MAXNUM; i++){
        mAoParas[i].mEnable[0] = false;
    }
}

QString PlayerPh::getCurMedia(int pId, bool &pRunning)
{
    PLAY_MEDIA_INFO *tmpMedia = 0;
    if(mPlayerList[pId] && (tmpMedia = mPlayerList[pId]->getMedia()))
    {
        pRunning = mPlayerList[pId]->isRunning();
        return tmpMedia->mPath;
    }
    return QString();
}


void PlayerPh::updatePara(QObject* pObject)
{
/*
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
                MediaPlayer *tmpPlayer = mAoRelationHash.value(AO_LIFTFLR);
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
                MediaPlayer *tmpPlayer = mAoRelationHash.value(AO_LIFTARV);
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
                MediaPlayer *tmpPlayer = mAoRelationHash.value(AO_BEEP);
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
                MediaPlayer *tmpPlayer = mAoRelationHash.value(AO_ADMUSIC);
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
                MediaPlayer *tmpPlayer = mAoRelationHash.value(AO_VIDEO);
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
*/
}

bool PlayerPh::setValue(QString pValue)
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
        MediaPlayer *tmpPlayer = mAoRelationHash.value(tmpType);
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
                ////tmpPlayer->Add(tmpType, D_FSRCPATH + D_BOOTDIR + QString("beep.wav"), (int)LOOP_NONE, true, mAoParas[tmpType].GetVolume());
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
                    setScaler(tmpType, 0, tmpSync);
                else
                    setScaler(tmpType, (qreal)((qreal)(9-tmpDiv)/8.0), tmpSync);
            }
        }
        else if(!tmpPara.compare("next", Qt::CaseInsensitive))
        {
            if(mAoParas[tmpType].mEnable[0])
                tmpPlayer->playback(LOOP_NONE);
        }
        else if(!tmpPara.compare("test", Qt::CaseInsensitive))
        {
            if(tmpCmdList.count()>=3)
            {
                tmpPlayer->setEnable(true);
                //tmpPlayer->TestMedia(tmpType, D_FSRCPATH + D_BOOTDIR + QString("test.wav"), tmpCmdList.at(2).toInt());
            }
        }
        else
        {
            if(mAoParas[tmpType].mEnable[0]){
                //tmpPlayer->Add(tmpType, D_FSRCPATH + D_BOOTDIR + QString("%1.wav").arg(tmpPara), (int)LOOP_NONE, false, mAoParas[tmpType].GetVolume());
            }
        }
    }
    return true;
}
