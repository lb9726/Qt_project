#include "graphicsmedia.h"
#include "logicthememanager.h"
#include "devicemanager.h"

GraphicsMedia::GraphicsMedia(QGraphicsItem *parent):
    GraphicsOperate(parent)
{
}

GraphicsMedia::~GraphicsMedia()
{
}

int GraphicsMedia::StrToEnum(QString pString)
{
    if(!pString.compare("Repeat", Qt::CaseInsensitive))//Repeat:默认为false
        return D_REPEAT_EN;
    else if(!pString.compare("SoloTrack", Qt::CaseInsensitive))//MultiTrack:默认为TRUE
        return D_SOLOTRACK_EN;
    else if(!pString.compare("Volume", Qt::CaseInsensitive))//MultiTrack:默认为TRUE
        return D_VOLUME_NUM;
    return -1;
}

QVariant GraphicsMedia::StrToValue(int pEnum, QString pString)
{
    if(pEnum == D_REPEAT_EN)            //Repeat:默认为false
        return QVariant(pString.compare("true", Qt::CaseInsensitive) ? false : true);
    else if(pEnum == D_SOLOTRACK_EN)    //MultiTrack:默认为TRUE
        return QVariant(pString.compare("false", Qt::CaseInsensitive) ? true : false);
    else if(pEnum == D_VOLUME_NUM)      //Volume
        return QVariant(pString.toUInt());
    return QVariant();
}

bool GraphicsMedia::Start()
{
    if(GraphicsOperate::Start() == false)
        return false;

    return true;
}

void GraphicsMedia::Abort(int pAoType)
{
}

void GraphicsMedia::updateEffect(AREA_OPERATE pOperate, QVariant pPara)
{
#ifdef DISPLAY
#ifdef SUPPORT_SCENE
    UiScene *tmpScene = VAR_CAST<UiScene*>(m_UiContainer.data());
    if(!(tmpScene && tmpScene->m_LogicThemeManager))
    {
        IDE_TRACE();
        return;
    }
    if(m_MediaPlayer.isNull())
    {
        if(tmpScene->m_LogicThemeManager->m_DevManager)
        {
            m_MediaPlayer = VAR_CAST<PhysicalPlayer*>(tmpScene->m_LogicThemeManager->m_DevManager->m_PHDevList.value(P_Player));
        }
        if(!m_MediaPlayer.isNull())
        {
            connect(this, SIGNAL(sPlay(int,QString,int,bool,bool)), m_MediaPlayer.data(), SLOT(slot_Play(int,QString,int,bool,bool)));
            connect(this, SIGNAL(sPlay(int,QString,int,int,bool,bool)), m_MediaPlayer.data(), SLOT(slot_Play(int,QString,int,int,bool,bool)));
        }
        else
            return;
    }
#endif
#endif
    int tmpMediaNum = pPara.toInt()/* + tmpScene->m_LogicThemeManager->mLanguage*100*/;
    if(m_MapList.contains(tmpMediaNum))
    {
        MAP_INFO *tmpInfo = m_MapList.value(tmpMediaNum);
        if(tmpInfo)
            tmpMediaNum = tmpInfo->mMap;
        IDE_TRACE_INT(tmpMediaNum);
    }
    RC_INFO *tmpRcInfo = m_EffectRC.value(tmpMediaNum);
    if(!tmpRcInfo)
        return;
    QString tmpMediaFile = tmpRcInfo->mRcFile;
    if(!QFile::exists(tmpMediaFile))
        return;
    switch(pOperate)
    {
        case OPERATE_REPEAT:
        {
            if(tmpMediaNum >= 40 && tmpMediaNum <= 42)
                emit sPlay(AO_LIFTARV, tmpMediaFile, LOOP_EVER, false, false);
            else
                emit sPlay(AO_LIFTFLR, tmpMediaFile, LOOP_EVER, false, false);
            break;
        }
        case OPERATE_CHANGE:
        case OPERATE_PLAY:
        {
            bool tmpSoloTrackEnable = (bool)(tmpRcInfo->mParaHash.value(D_SOLOTRACK_EN).toBool());
            bool tmpRepeatEnable = (bool)(tmpRcInfo->mParaHash.value(D_REPEAT_EN).toBool());
            if(tmpRcInfo->mParaHash.contains(D_VOLUME_NUM))
            {
                IDE_TRACE();
                quint32 tmpVolume = (quint32)(tmpRcInfo->mParaHash.value(D_VOLUME_NUM).toUInt());
                if(tmpMediaNum >= 40 && tmpMediaNum <= 42)
                    emit sPlay(AO_LIFTARV, tmpMediaFile, tmpVolume, tmpRepeatEnable ? LOOP_EVER : LOOP_NONE, false, tmpSoloTrackEnable);
                else
                    emit sPlay(AO_LIFTFLR, tmpMediaFile, tmpVolume, tmpRepeatEnable ? LOOP_EVER : LOOP_NONE, false, tmpSoloTrackEnable);
            }
            else
            {
                IDE_TRACE();
                if(tmpMediaNum >= 40 && tmpMediaNum <= 42)
                    emit sPlay(AO_LIFTARV, tmpMediaFile, tmpRepeatEnable ? LOOP_EVER : LOOP_NONE, false, tmpSoloTrackEnable);
                else
                    emit sPlay(AO_LIFTFLR, tmpMediaFile, tmpRepeatEnable ? LOOP_EVER : LOOP_NONE, false, tmpSoloTrackEnable);
            }
            break;
        }
        default:
            break;
    }
}

DesignMedia::DesignMedia(QGraphicsItem *parent):
    GraphicsMedia(parent)
{
    m_CurRcLabel = INVALIDRC;
    QAction *tmpDefault = m_ActionGroup->addAction(tr("0"));
    for(int i=1;i<20;i++)
    {
        m_ActionGroup->addAction(QString("%1").arg(i));
    }
    tmpDefault->setChecked(true);
}

void DesignMedia::updateEffect(int pIndex)
{
    bool tmpIndexChange = false;
    if(pIndex != m_CurRcLabel)
        tmpIndexChange = true;
    m_OperateInfo[STEP0].mValid = false;
    m_OperateInfo[STEP1].mValid = false;
    m_Animator.stop();
    if(tmpIndexChange)
    {
        m_OperateInfo[STEP0].mValid = true;
        m_OperateInfo[STEP0].mOperate = OPERATE_CHANGE;
        m_PixmapPointer.mCurPointer = GenerateSpectrum(m_EffectRC.value(pIndex)->mRcFile, rect(), m_PixmapPointer.mLastPointer);
    }
    m_OperateInfo[STEP1].mValid = true;
    m_OperateInfo[STEP1].mOperate = OPERATE_NONE;
    m_CurRcLabel = pIndex;
    //>@执行STEP0中的内容
    OperateStep0();
}

QList<QAction*> DesignMedia::GetActionList()
{
    QList<QAction*> tmpList;
    if(m_ActionGroup)
    {
        tmpList = m_ActionGroup->actions();
        //>@根据当前已有的资源使能响应功能Action
        for(int i=0;i<tmpList.count();i++)
        {
            QAction *tmpAction = tmpList.at(i);
            if(tmpAction == 0)
                continue;
            int tmpState = tmpAction->text().toInt();
            if(m_EffectRC.contains(tmpState))
                tmpAction->setEnabled(true);
            else
                tmpAction->setDisabled(true);
        }
    }
    return tmpList;
}

void DesignMedia::ExecAction(QAction *pAction)
{
    updateEffect(pAction->text().toInt());
}


