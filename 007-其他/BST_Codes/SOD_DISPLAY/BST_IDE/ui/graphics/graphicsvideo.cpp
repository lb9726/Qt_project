#include "graphicsvideo.h"
#ifdef DISPLAY
#include "logicthememanager.h"
#include "devicemanager.h"
#endif

GraphicsVideo::GraphicsVideo(QGraphicsItem *parent):
    GraphicsOperate(parent)
{
}

GraphicsVideo::~GraphicsVideo()
{
}

int GraphicsVideo::GetScreenRotate()
{
    int tmpRotate = 0;
#ifdef DISPLAY
    UiScene *tmpScene = VAR_CAST<UiScene *>(m_UiContainer.data());
    if(tmpScene && tmpScene->m_LogicThemeManager && tmpScene->m_LogicThemeManager->m_DevManager)
    {
        PhysicalLCD *tmpLCD = VAR_CAST<PhysicalLCD *>(tmpScene->m_LogicThemeManager->m_DevManager->m_PHDevList.value(P_LCD));
        if(tmpLCD)
            tmpRotate = tmpLCD->mRotation;
    }
#endif
    return tmpRotate;
}

bool GraphicsVideo::Start()
{
    if(GraphicsOperate::Start() == false)
        return false;
    m_PixmapPointer.mLastPointer = m_PixmapPointer.mCurPointer = GetPixmap(BGRC);
    UpdateCom(0);
    return true;
}

void GraphicsVideo::updateEffect(AREA_OPERATE pOperate, QVariant pPara)
{
    if(m_MediaPlayer.isNull())
    {
#ifdef DISPLAY
        UiScene *tmpScene = VAR_CAST<UiScene*>(m_UiContainer.data());
        if(tmpScene && tmpScene->m_LogicThemeManager && tmpScene->m_LogicThemeManager->m_DevManager)
        {
            m_MediaPlayer = VAR_CAST<PhysicalPlayer*>(tmpScene->m_LogicThemeManager->m_DevManager->m_PHDevList.value(P_Player));
        }
        if(!m_MediaPlayer.isNull())
            connect(this, SIGNAL(sPlay(int,QString,int,bool,bool)), m_MediaPlayer.data(), SLOT(slot_Play(int,QString,int,bool,bool)));
        else
            return;
#endif
    }
    RC_INFO *tmpRcInfo = m_EffectRC.value(pPara.toInt());
    if(!tmpRcInfo)
    {
        IDE_TRACE();
        return;
    }
    switch(pOperate)
    {
        case OPERATE_REPEAT:
        {
            emit sPlay(AO_ADVIDEO, tmpRcInfo->mRcFile, LOOP_EVER, false, false);
            break;
        }
        case OPERATE_CHANGE:
        case OPERATE_PLAY:
        {
            emit sPlay(AO_ADVIDEO, tmpRcInfo->mRcFile, LOOP_NONE, false, false);
            break;
        }
        default:
            break;
    }
}

DesignVideo::DesignVideo(QGraphicsItem *parent):
    GraphicsVideo(parent)
{
    m_CurRcLabel = INVALIDRC;
    QAction *tmpDefault = m_ActionGroup->addAction(tr("0"));
    for(int i=1;i<10;i++)
    {
        m_ActionGroup->addAction(QString("%1").arg(i));
    }
    tmpDefault->setChecked(true);
}

void DesignVideo::updateEffect(int pIndex)
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


QList<QAction*> DesignVideo::GetActionList()
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

void DesignVideo::ExecAction(QAction *pAction)
{
    updateEffect(pAction->text().toInt());
}

