#include "graphicsmessage.h"
#include "bggen.h"
#ifdef SUPPORT_SCENE
#include "uiscene.h"
#elif SUPPORT_WIDGET
#include "uiwidget.h"
#elif SUPPORT_COMEDIT
#include "diagramscene.h"
#endif

GraphicsMessage::GraphicsMessage(QGraphicsItem *parent) :
    GraphicsOperate(parent)
{
    mOldDelay = 0;
    m_ShowTitle = true;
    mDevMode = DM_NORMAL;
    mShowMode = SM_NORMAL;
    connect(&m_Animator, SIGNAL(finished()), this, SLOT(slot_Finished()));
    connect(&m_DelayTimer, SIGNAL(timeout()), this, SLOT(slot_CloseMessage()));
    m_DelayTimer.setSingleShot(true);
}

bool GraphicsMessage::InitSubRcPara(AREA_STYLE* pAreaStyle, QString pLabel, QString pContent)
{
    if(pLabel.compare("Title", Qt::CaseInsensitive) == 0)
    {
        if(!pContent.compare("false", Qt::CaseInsensitive))
            m_ShowTitle = false;
        else
            m_ShowTitle = true;
    }
    else if(pLabel.compare("Font", Qt::CaseInsensitive) == 0)
    {
        m_TextFont[0].setFamily(pContent);
        m_TextFont[1].setFamily(pContent);
    }
    else if(pLabel.compare("Size", Qt::CaseInsensitive) == 0)
    {
        int tmpSize = pContent.toInt();
        m_TextFont[0].setPointSize(tmpSize);
#ifdef N329
        m_TextFont[1].setPointSize(tmpSize);
#else
        m_TextFont[1].setPointSize(tmpSize);
#endif
    }
    else if(pLabel.compare("Bold", Qt::CaseInsensitive) == 0)
    {
        int bold = QFont::Normal;
        if(pContent.compare("DemiBold") == 0)
            bold = QFont::DemiBold;
        else if(pContent.compare("Bold") == 0)
            bold = QFont::Bold;
        m_TextFont[0].setWeight(bold);
        m_TextFont[1].setWeight(bold);
    }
    return true;
}

QPixmap GraphicsMessage::RefreshMessage()
{
#ifdef QT_V453
    QImage tmpImage = QImage(D_FSRCPATH + D_BOOTDIR + QString("transparent.png")).scaled(size().toSize());
    if(tmpImage.isNull())
        return QPixmap();
#else
    QImage tmpImage(size().width(), size().height(), QImage::Format_ARGB32_Premultiplied);
    if(tmpImage.isNull())
        return QPixmap();
    tmpImage.fill(Qt::transparent); //>@´´½¨Í¸Ã÷Í¼²ã
#endif
    QPainter p(&tmpImage);
    p.save();
    p.setOpacity(0.8);
    QPixmap tmpPixmap = GetPixmap(LEVEL_INFO);
    if(tmpPixmap.isNull())
    {
        BaseBgGen tmpGen;
        if(m_ShowTitle)
            tmpGen.SetTitle(GetMsgLevel(LEVEL_INFO));
        tmpGen.Resize(size().toSize());
        tmpGen.Refresh();
        tmpPixmap = QPixmap::fromImage(tmpGen.m_Image);
        tmpGen.deleteLater();
        LoadPixmap(LEVEL_INFO, tmpPixmap);
    }
    if(!tmpPixmap.isNull())
        p.drawPixmap(0, 0, size().width(), size().height(), tmpPixmap);
    p.restore();
    //>@¼ÆËã×îÓÅµÄÎÄ±¾»æÖÆÎ»ÖÃ
    int count = m_MessageQueue.count();
    if(m_UiContainer->m_UiDirection == Vertical)
        p.setFont(m_TextFont[1]);
    else if(m_UiContainer->m_UiDirection == Horizontal)
        p.setFont(m_TextFont[0]);
    int tmpCount;
    if(count > m_MsgMaxCount)
        tmpCount = m_MsgMaxCount;
    else
        tmpCount = count;
    for(int i=m_MsgMaxCount-tmpCount; i<m_MsgMaxCount; i++)
    {
        MSG_INFO tmpMsgInfo = m_MessageQueue.at(i+tmpCount-m_MsgMaxCount);
        p.save();
        p.setPen(GetColor(tmpMsgInfo.mMsgLevel));
        if(!tmpMsgInfo.mMessage.isEmpty())
        {
            QString tmpMsg;
            if(tmpMsgInfo.mCount <= 1)
                tmpMsg = tmpMsgInfo.mMessage;
            else
                tmpMsg = QString("%1...(%2)").arg(tmpMsgInfo.mMessage).arg(tmpMsgInfo.mCount);
            p.drawText(QRect(m_MsgRect.left(),
                             m_MsgRect.top()+m_TextHeight*i,
                             m_MsgRect.width(),
                             m_TextHeight),
                       tmpMsg);
        }
        p.restore();
    }
    p.end();
    return QPixmap::fromImage(tmpImage);
}

void GraphicsMessage::slot_UpdateFont()
{
    QRectF tmpRect = rect();
    if(m_ShowTitle)
        m_MsgRect = tmpRect.adjusted(20, 70, -20, -20);
    else
        m_MsgRect = tmpRect.adjusted(20, 20, -20, -20);
    if(m_UiContainer->m_UiDirection == Horizontal)
    {
        QFontMetricsF fm(m_TextFont[0]);
        m_MsgMaxCount = m_MsgRect.height() / fm.height();
        m_TextHeight = fm.height();
    }
    else if(m_UiContainer->m_UiDirection == Vertical)
    {
        QFontMetricsF fm(m_TextFont[1]);
        m_MsgMaxCount = m_MsgRect.height() / fm.height();
        m_TextHeight = fm.height();
    }
    m_PixmapPointer.mCurPointer = RefreshMessage();
    IDE_TRACE_INT(m_MsgMaxCount);
    UpdateCom(0);
}

void GraphicsMessage::OperateTurn(QVariant pPara)
{
    GraphicsComponent::OperateTurn(pPara);
    slot_UpdateFont();
}

bool GraphicsMessage::Start()
{
    GraphicsOperate::Start();

    slot_UpdateFont();

    //>@Ä¬ÈÏÒþ²ØMessage´°¿Ú
#ifndef IDE
    slot_Finished();
#endif
    return true;
}

void GraphicsMessage::updateEffect(AREA_OPERATE pOperate, QVariant pPara)
{
    GraphicsOperate::updateEffect(pOperate, pPara);
    switch(pOperate)
    {
        case OPERATE_CHANGE:
        {
            QStringList tmpList = pPara.toStringList();
            if(tmpList.count() != 3)
                return;
            MSG_LEVEL tmpLevel = GetMsgLevel(tmpList.at(0));
            if(tmpLevel == LEVEL_INFO)
                info(tmpList.at(1), tmpList.at(2).toInt());
            else if(tmpLevel == LEVEL_WARN)
                warn(tmpList.at(1), tmpList.at(2).toInt());
            else if(tmpLevel == LEVEL_ERROR)
                error(tmpList.at(1), tmpList.at(2).toInt());
            break;
        }
        default:
            break;
    }
}

void GraphicsMessage::updateEffect()
{
    m_OperateInfo[STEP0].mValid = false;
    m_OperateInfo[STEP1].mValid = false;
    m_Animator.stop();
    m_OperateInfo[STEP0].mValid = true;
    m_OperateInfo[STEP0].mOperate = OPERATE_CHANGE;

    m_PixmapPointer.mLastPointer = m_PixmapPointer.mCurPointer;
    m_PixmapPointer.mCurPointer = QPixmap();
    OperateStep0();
}

void GraphicsMessage::slot_CloseMessage()
{
    //updateEffect();
    slot_Finished();
}

void GraphicsMessage::slot_Finished()
{
    hide();
    QGraphicsItem *tmpItem;
    QList<QGraphicsItem *>  tmpList = collidingItems();
    foreach(tmpItem, tmpList)
    {
        tmpItem->update();
    }
}

void GraphicsMessage::AppendMessage(MSG_INFO pMsg)
{
    if(m_MessageQueue.isEmpty())
        m_MessageQueue.append(pMsg);
    else
    {
        MSG_INFO tmpMsg = m_MessageQueue.last();
        if((tmpMsg.mMsgLevel == pMsg.mMsgLevel) && (tmpMsg.mMessage == pMsg.mMessage))
            tmpMsg.mCount++;
        else
            m_MessageQueue.append(pMsg);
    }
}

QPixmap GraphicsMessage::LoadMessage(MSG_INFO pMsgInfo)
{
    if(m_MsgMaxCount <= 0)
        return QPixmap();
    if(!pMsgInfo.isValid())
        return m_PixmapPointer.mCurPointer;
    if(m_MsgMaxCount)
    {
        while(m_MessageQueue.count() >= m_MsgMaxCount)
            m_MessageQueue.removeFirst();
    }
    else
        return QPixmap();
    AppendMessage(pMsgInfo);

#ifdef QT_V453
    QImage tmpImage = QImage(D_FSRCPATH + D_BOOTDIR + QString("transparent.png")).scaled(size().toSize());
    if(tmpImage.isNull())
        return QPixmap();
#else
    QImage tmpImage(size().width(), size().height(), QImage::Format_ARGB32_Premultiplied);
    if(tmpImage.isNull())
        return QPixmap();
    tmpImage.fill(Qt::transparent); //>@´´½¨Í¸Ã÷Í¼²ã
#endif
    QPainter p(&tmpImage);
    p.save();
    p.setOpacity(0.8);
    QPixmap tmpPixmap = GetPixmap(pMsgInfo.mMsgLevel);
    if(tmpPixmap.isNull())
    {
        BaseBgGen tmpGen;
        if(m_ShowTitle)
            tmpGen.SetTitle(GetMsgLevel(pMsgInfo.mMsgLevel));
        tmpGen.Resize(size().toSize());
        tmpGen.Refresh();
        tmpPixmap = QPixmap::fromImage(tmpGen.m_Image);
        tmpGen.deleteLater();
        LoadPixmap(pMsgInfo.mMsgLevel, tmpPixmap);
    }
    if(!tmpPixmap.isNull())
        p.drawPixmap(0, 0, size().width(), size().height(), tmpPixmap);
    p.restore();
    //>@¼ÆËã×îÓÅµÄÎÄ±¾»æÖÆÎ»ÖÃ
    int count = m_MessageQueue.count();
    if(m_UiContainer->m_UiDirection == Vertical)
        p.setFont(m_TextFont[1]);
    else if(m_UiContainer->m_UiDirection == Horizontal)
        p.setFont(m_TextFont[0]);
    int tmpCount;
    if(count > m_MsgMaxCount)
        tmpCount = m_MsgMaxCount;
    else
        tmpCount = count;
    for(int i=m_MsgMaxCount-count; i<m_MsgMaxCount; i++)
    {
        MSG_INFO tmpMsgInfo = m_MessageQueue.at(i+count-m_MsgMaxCount);
        p.save();
        p.setPen(GetColor(tmpMsgInfo.mMsgLevel));
        if(!tmpMsgInfo.mMessage.isEmpty())
        {
            QString tmpMsg;
            if(tmpMsgInfo.mCount <= 1)
                tmpMsg = tmpMsgInfo.mMessage;
            else
                tmpMsg = QString("%1...(%2)").arg(tmpMsgInfo.mMessage).arg(tmpMsgInfo.mCount);
            p.drawText(QRect(m_MsgRect.left(),
                             m_MsgRect.top()+m_TextHeight*i,
                             m_MsgRect.width(),
                             m_TextHeight),
                       tmpMsg);
        }
        p.restore();
    }
    p.end();
    return QPixmap::fromImage(tmpImage);
}

bool GraphicsMessage::ProcessMessage(MSG_LEVEL pLevel, QString pContent, int pDelay, E_DEVMODE pDevMode)
{
    E_SHOWMODE tmpSM = SM_NORMAL;
    if(pDelay == MSG_PERMANENT)
        tmpSM = SM_PERMANENT;
    else if(pDelay == MSG_TEMPORARY)
        tmpSM = SM_TEMPORARY;
    else if(pDelay == MSG_HOLD)
        tmpSM = SM_HOLD;
    SetMode(tmpSM, pDevMode);
    //>
    if(mShowMode == SM_TEMPORARY)//>@Èç¹û¸øÁËMSG_TEMPORARYÐèÒªÁ¢¼´¹Ø±ÕÆÁÄ»£¬Õâ ÇÎªÁË½â¾öÉÏÒ»¸ötimerÒÑ¾­½á ø£¬ÎÞ·¨¹Ø±ÕÏûÏ¢¿òµÄbug
    {
        mShowMode = SM_NORMAL;
#if 0
        m_DelayTimer.stop();
        slot_Finished();
#else
        m_DelayTimer.start(1000);
#endif
        return false;
    }
    if(isVisible() == false)
    {
        setZValue(D_MSGCTRL_ZVALUE);
        show();
    }
    MSG_INFO tmpMsgInfo(pLevel, pContent, pDelay);
    m_PixmapPointer.mCurPointer = LoadMessage(tmpMsgInfo);
    if(mShowMode == SM_PERMANENT)
    {
        m_DelayTimer.stop();
    }
    else if(mShowMode == SM_HOLD)
    {
        //>@
    }
    else
    {
        m_DelayTimer.start(pDelay);
        mOldDelay = pDelay;
    }
    return true;
}

bool GraphicsMessage::PaintEffect(QPainter *p)
{
    if(m_EffectValid == false)
    {
        return false;
    }
    Paint2D_None(p, rect());
    return true;
}

bool GraphicsMessage::SetMode(E_SHOWMODE pShowMode, E_DEVMODE pDevMode)
{
    if(pDevMode < mDevMode)
    {
        IDE_DEBUG(QString("new devmode %1 is smaller than %2").arg(pDevMode).arg(mDevMode));
        return false;
    }
    if(pShowMode == SM_TEMPORARY || pShowMode == SM_NORMAL)
        mDevMode = DM_NORMAL;
    else
        mDevMode = pDevMode;
    mShowMode = pShowMode;
    return true;
}

void GraphicsMessage::info(QString pContent, int pDelay, E_DEVMODE pDevMode)
{
    if(ProcessMessage(LEVEL_INFO, pContent, pDelay, pDevMode))
        UpdateCom(0);
}

void GraphicsMessage::warn(QString pContent, int pDelay, E_DEVMODE pDevMode)
{
    if(ProcessMessage(LEVEL_WARN, pContent, pDelay, pDevMode))
        UpdateCom(0);
}

void GraphicsMessage::error(QString pContent, int pDelay, E_DEVMODE pDevMode)
{
    if(ProcessMessage(LEVEL_ERROR, pContent, pDelay, pDevMode))
        UpdateCom(0);
}

DesignMessage::DesignMessage(QGraphicsItem *parent) :
    GraphicsMessage(parent)
{
    QAction *tmpDefault = m_ActionGroup->addAction(tr("INFO"));
    m_ActionGroup->addAction(tr("WARN"));
    m_ActionGroup->addAction(tr("ERROR"));
    tmpDefault->setChecked(true);
}

QList<QAction*> DesignMessage::GetActionList()
{
    QList<QAction*> tmpList;
    if(m_ActionGroup)
    {
        tmpList = m_ActionGroup->actions();
        //>@¸ù¾Ýµ±Ç°ÒÑÓÐµÄ× Ô´ ¹ÄÜÏìÓ¦¹¦ÄÜAction
        for(int i=0;i<tmpList.count();i++)
        {
            QAction *tmpAction = tmpList.at(i);
            if(tmpAction == 0)
                continue;
            tmpAction->setEnabled(true);
        }
    }
    return tmpList;
}

void DesignMessage::ExecAction(QAction *pAction)
{
    updateEffect(OPERATE_CHANGE, QVariant(QStringList()<<pAction->text()<<QString("test")<<QString("1000")));
}

