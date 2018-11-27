#include "graphicstext.h"
#include "logicthememanager.h"
#include "deviceinc.h"
#include "physicallcd.h"
#include "physicalelevator.h"
#ifdef SUPPORT_SCENE
#include "uiscene.h"
#elif SUPPORT_COMEDIT
#include "diagramscene.h"
#endif
#include "graphicsregion.h"

GraphicsText::GraphicsText(QGraphicsItem *parent):
    GraphicsOperate(parent)
{
    m_SpaceExt = 1;
    m_ScreenMode = 0;
    m_MaxRow = 0;
    m_Row = m_Column = 0;
    m_RowSpace = 0;
    m_MaxColumn = 0;

    m_AlignS = Qt::AlignCenter;
    m_AlignB = Qt::AlignLeft | Qt::AlignVCenter;
    m_Alignment = m_AlignS;

    m_TextMode = D_TEXT_MSG;
    m_TextColor = Qt::white;

    connect(this, SIGNAL(sSetGeomotery(QRectF)), this, SLOT(slot_ModifyGeomotery(QRectF)), Qt::QueuedConnection);
}

GraphicsText::~GraphicsText()
{
    for(int i=0;i<D_TEXT_MODE;i++)
    {
        for(int j=0;j<m_MaxRow;j++)
        {
            free(m_TextInfo[i][j].mString);
        }
    }
}

bool GraphicsText::InitSubRcPara(AREA_STYLE* pAreaStyle, QString pLabel, QString pContent)
{
    RcContainer::InitSubRcPara(pAreaStyle, pLabel, pContent);
    if(pLabel.compare("Row", Qt::CaseInsensitive) == 0)
    {
        m_MaxRow = qMin((uint)D_TEXT_NUM, pContent.toUInt());
    }
    else if(pLabel.compare("Column", Qt::CaseInsensitive) == 0)
    {
        m_MaxColumn = pContent.toUInt();
    }
    else if(pLabel.compare("RowSpace", Qt::CaseInsensitive) == 0)
    {
        m_RowSpace = pContent.toUInt();
    }
    else if(pLabel.compare("SpaceExt", Qt::CaseInsensitive) == 0)
    {
        m_SpaceExt = pContent.toUInt();
    }
    else if(pLabel.compare("Font", Qt::CaseInsensitive) == 0)
    {
        m_TextFont.setFamily(pContent);
    }
    else if(pLabel.compare("Weigth", Qt::CaseInsensitive) == 0)
    {
        if(!pContent.compare("DemiBold", Qt::CaseInsensitive))
            m_TextFont.setWeight(QFont::DemiBold);
        else if(!pContent.compare("Bold", Qt::CaseInsensitive))
            m_TextFont.setWeight(QFont::Bold);
        else
            m_TextFont.setWeight(QFont::Normal);
    }
    else if(pLabel.compare("Color", Qt::CaseInsensitive) == 0)
    {
        if(!pContent.compare("red", Qt::CaseInsensitive))
            m_TextColor = Qt::red;
        else if(!pContent.compare("blue", Qt::CaseInsensitive))
            m_TextColor = Qt::blue;
        else if(!pContent.compare("black", Qt::CaseInsensitive))
            m_TextColor = Qt::black;
        else
            m_TextColor = Qt::white;
    }
    else if(pLabel.compare("AlignS", Qt::CaseInsensitive) == 0)
    {
        if(!pContent.compare("Center", Qt::CaseInsensitive))
            m_AlignS = Qt::AlignCenter;
        else if(!pContent.compare("Left", Qt::CaseInsensitive))
            m_AlignS = Qt::AlignLeft | Qt::AlignVCenter;
        else if(!pContent.compare("Right", Qt::CaseInsensitive))
            m_AlignS = Qt::AlignRight | Qt::AlignVCenter;
        else
            m_AlignS = Qt::AlignCenter;
    }
    else if(pLabel.compare("AlignB", Qt::CaseInsensitive) == 0)
    {
        if(!pContent.compare("Center", Qt::CaseInsensitive))
            m_AlignB = Qt::AlignCenter;
        else if(!pContent.compare("Left", Qt::CaseInsensitive))
            m_AlignB = Qt::AlignLeft | Qt::AlignVCenter;
        else if(!pContent.compare("Right", Qt::CaseInsensitive))
            m_AlignB = Qt::AlignRight | Qt::AlignVCenter;
        else
            m_AlignB = Qt::AlignCenter;
    }
    return true;
}

bool GraphicsText::SetGrid(QRectF pRect, int pRow, int pColumn)
{
    if(pRow == 0 || pColumn == 0 || pRect.isEmpty())
        return false;
    if(pRow > m_MaxRow || pColumn > m_MaxColumn)
        return false;
    m_Row = pRow;
    m_Column = pColumn;

    //>@以满足宽度为主
    qreal tmpCharWidth = (qreal)(pRect.width())/(qreal)pColumn;
    int fontsize=6;
    while(fontsize < 200) //>@确保fontsize最大为200，防止死循环
    {
        int newfontsize;
        newfontsize = fontsize+1;
        m_TextFont.setPointSize(newfontsize);
        QFontMetricsF fm(m_TextFont);
        qreal pixelsWidth = fm.width("W");
        if(pixelsWidth > tmpCharWidth)
        {
            m_TextFont.setPointSize(fontsize);
            break;
        }
        fontsize = newfontsize;
    }
    qreal tmpCharHeight = (qreal)(pRect.height() - m_RowSpace * (pRow-1)) / (qreal)pRow;
    for(int i=0;i<D_TEXT_MODE;i++)
    {
        for(int j=0;j<pRow;j++)
        {
            m_TextInfo[i][j].mRect = QRectF(0, j*(tmpCharHeight+m_RowSpace), pRect.width(), tmpCharHeight+4);
            //m_TextInfo[i][j].mRect = QRectF(0, j*(tmpCharHeight+m_RowSpace) - tmpCharHeight/2.0, pRect.width(), tmpCharHeight*2);
        }
    }

    if(m_TextMode != D_TEXT_TIP)
        m_TextMode = D_TEXT_MSG;

    //>@必须先刷新界面，则设置大小，否则会出现大屏每行最后出现原来字符的残影。
    emit sUpdateCom(0);

    //>@
    //if(m_ComRect[m_UiContainer->m_UiDirection] != pRect)
        emit sSetGeomotery(pRect);
    return true;
}

QRectF GraphicsText::GetCurRect()
{
    if(m_ScreenMode == 0)
        return m_ComRect[m_UiContainer->m_UiDirection];
#ifdef SUPPORT_SCENE
    UiScene *tmpScene = VAR_CAST<UiScene*>(m_UiContainer.data());
    if(tmpScene && tmpScene->m_Desktop)
    {
        QRectF tmpRect = tmpScene->m_Desktop->rect();
        qreal tmpWidth = GetMinSrnLen();
        qreal tmpHeight = tmpWidth * 2 / 3;
        return QRectF((tmpRect.width()-tmpWidth)/2, (tmpRect.height()-tmpHeight)/2, tmpWidth, tmpHeight);
    }
#endif
    return QRectF();
}

void GraphicsText::InitGrid()
{

}

bool GraphicsText::Start()
{
    GraphicsOperate::Start();

    m_OldRect = geometry();
    if(m_RcStyle)
    {
        if(m_RcStyle->mRcType == COMRC_TEXT)  //>@根据大小，计算字体大小
        {
            for(int j=0;j<D_TEXT_MODE;j++)
            {
                for(int i=0;i<m_MaxRow;i++)
                {
                    m_TextInfo[j][i].mString = (char*)malloc(m_MaxColumn*sizeof(char));
                    memset(m_TextInfo[j][i].mString, D_FILLCHAR, D_TEXT_MAXCOL+1);
                }
            }
            //>@默认为2x12显示方式，
            SetGrid(m_OldRect, 2, 24);
            m_ScreenMode = 0;
            m_Alignment = m_AlignS;
        }
        else
        {
            m_TextState = -1;
            updateEffect(OPERATE_CHANGE, QVariant(m_EffectRC.keys().first()));
        }
    }
#ifdef DISPLAY
    UiScene *tmpScene = VAR_CAST<UiScene*>(m_UiContainer.data());
    if(tmpScene)
    {
        if(tmpScene->m_LogicThemeManager)
        {
            m_LCD = tmpScene->GetLCD();
#ifdef N329
            m_EffectRC.insert(BGRC, CreateRcInfo(tmpScene->m_LogicThemeManager->m_TransparentPix));
            m_PixmapPointer.mCurPointer = tmpScene->m_LogicThemeManager->m_TransparentPix;
#endif
            m_Com = VAR_CAST<PhysicalCOM*>(tmpScene->m_LogicThemeManager->m_DevManager->m_PHDevList.value(P_COM));

            m_Player = VAR_CAST<PhysicalPlayer*>(tmpScene->m_LogicThemeManager->m_DevManager->m_PHDevList.value(P_Player));
        }
        m_Function = VAR_CAST<GraphicsFunction*>(tmpScene->m_ComDiagramGroup.value(COM_FUNCTION));
    }
#endif
    UpdateBigRect();
    show();

    return true;
}

void GraphicsText::slot_ChangeText()
{
    L_IDE_TRACE();
    if(m_ScreenMode == 1){        
        SetTextShowMode(0);
    }else{
        SetTextShowMode(1);
    }
}

void GraphicsText::OperateTurn(QVariant pPara)
{
    GraphicsComponent::OperateTurn(pPara);
    if(m_ScreenMode == 1)  //>@停止所有的楼层信号连接
    {
        UpdateBigRect();
        SetGrid(m_BigRect, 4, 16);
        m_OldRect = /*GetCurRect();*/m_ComRect[m_UiContainer->m_UiDirection];
    }
    else
    {
        m_OldRect = /*GetCurRect();*/m_ComRect[m_UiContainer->m_UiDirection];
        SetGrid(m_OldRect, 2, 24);
    }
}

void GraphicsText::ModifyGeomotery(QRectF pRect)  //>@逻辑层更改坐标。
{
    GraphicsComponent::ModifyGeomotery(pRect);
    m_OldRect = pRect;
}

void GraphicsText::SetComGeometory(QRectF pRect)  //>@底层修改坐标参数。
{
    GraphicsComponent::SetComGeometory(pRect);
    emit sUpdateCom(0);
}

bool GraphicsText::PaintEffect(QPainter *p)
{
    if(m_RcStyle)
    {
        if(m_RcStyle->mRcType == COMRC_TEXT)  //>@根据大小，计算字体大小
        {
            Paint2D_None(p, m_BigRect);
            p->save();
            p->setFont(m_TextFont);
            p->setPen(m_TextColor);
            for(int i=0;i<m_Row;i++)
            {
                if(!m_TextInfo[m_TextMode][i].mString)
                    continue;
                if(m_TextInfo[m_TextMode][i].mRect.isEmpty())
                    continue;
                char tmpString[m_MaxColumn+1];
                memset(tmpString, D_FILLCHAR, m_MaxColumn+1);
                memcpy(tmpString+m_TextInfo[m_TextMode][i].mShowStart,
                       m_TextInfo[m_TextMode][i].mString+m_TextInfo[m_TextMode][i].mShowStart,
                       m_TextInfo[m_TextMode][i].mShowEnd-m_TextInfo[m_TextMode][i].mShowStart);
                QByteArray tmpArray = QByteArray(tmpString);
                if(m_SpaceExt > 1)
                {
                    for(int m=tmpArray.count()-1;m>=0;m--)
                    {
                        if(tmpArray.at(m) == ' ')
                        {
                            for(int n=1;n<m_SpaceExt;n++)
                            {
                                tmpArray.insert(m, ' ');
                            }
                        }
                    }
                }
                QString tmpText = QString(tmpArray);
                //tmpText.resize(m_Column+1);
                p->drawText(m_TextInfo[m_TextMode][i].mRect, m_Alignment, tmpText);
            }
            p->restore();
        }
        else
        {
        }
    }
    return true;
}

bool GraphicsText::ShowTipText(bool pEnable, QString pText)
{
    if(m_RcStyle)
    {
        if(m_RcStyle->mRcType == COMRC_TEXT)  //>@根据大小，计算字体大小
        {
            QStringList tmpList = pText.split(';');
            int count = tmpList.count();
            bool allEmpty = true;   //>@是否全为空
            for(int i=0;i<m_MaxRow;i++)
            {
                if(i < count)
                {
                    QString tmpText = tmpList.at(i);
                    if(tmpText.isEmpty())
                    {
                        memset(m_TextInfo[D_TEXT_TIP][i].mString, D_FILLCHAR, m_MaxColumn);
                        m_TextInfo[D_TEXT_TIP][i].mShowStart = 0;
                        m_TextInfo[D_TEXT_TIP][i].mShowEnd = 0;
                    }
                    else
                    {
                        int strsize = tmpText.toLocal8Bit().size();
                        IDE_TRACE_INT(strsize);
                        //>@支持UTF-8，支持所有国家语言。
                        memcpy(m_TextInfo[D_TEXT_TIP][i].mString, tmpText.toLocal8Bit().data(), strsize);
                        m_TextInfo[D_TEXT_TIP][i].mShowStart = 0;
                        m_TextInfo[D_TEXT_TIP][i].mShowEnd = strsize;
                        allEmpty = false;
                    }
                }
                else
                {
                    memset(m_TextInfo[D_TEXT_TIP][i].mString, D_FILLCHAR, m_MaxColumn);
                    m_TextInfo[D_TEXT_TIP][i].mShowStart = 0;
                    m_TextInfo[D_TEXT_TIP][i].mShowEnd = 0;
                }
            }
            //>@修改为“即使logo没有解释文本，也不会显示控制柜发来的文本”
            if(!pEnable)
                m_TextMode = D_TEXT_MSG;
            else
                m_TextMode = D_TEXT_TIP;
            emit sUpdateCom(0);
            return true;
        }
    }
    return false;
}

int GraphicsText::GetTextShowMode()
{
    if(m_TestMode)
        return m_ScreenModeBk;
    return m_ScreenMode;
}

bool GraphicsText::SetTextShowMode(int pMode, bool pTest) //>@0：小屏， 1：大屏
{
    if(pMode == 0) //>@开启所有的楼层信号连接
    {
        if(m_Alignment == m_AlignS)
            return true;
        m_Alignment = m_AlignS;
#ifdef DISPLAY
        if(!m_LCD.isNull()){
            L_IDE_TRACE();
            if(m_Com->mHasArrow){
                m_LCD->SetSrnSaveEn(false);
            }else{
                m_LCD->SetSrnSaveEn(true);
            }
        }
        emit sSetDevices(true, DM_BIGSRN);
#endif
        SetGrid(m_OldRect, 2, 24);

        m_TestMode = pTest;
        if(m_TestMode)
            m_ScreenModeBk = m_ScreenMode;
        m_ScreenMode = 0;

        if(!m_Function.isNull()){
            m_Function.data()->recoveryfrombigScreen();
        }

        //emit sInfo("Small Screen");
    }
    else if(pMode == 1)  //>@停止所有的楼层信号连接
    {
        if(m_Alignment == m_AlignB)
            return true;
        m_Alignment = m_AlignB;
#ifdef DISPLAY
        if(!m_LCD.isNull()){
            L_IDE_TRACE();
            m_LCD->SetSrnSaveEn(false);
        }
        if(!m_Player.isNull())
        {
            MEDIA_PLAYER* tmpPlayer = m_Player->mAoRelationHash.value(AO_LIFTFLR);
            if(tmpPlayer){
                tmpPlayer->Clear();
            }
        }
        emit sSetDevices(false, DM_BIGSRN);
#endif
        UpdateBigRect();
        SetGrid(m_BigRect, 4, 16);

        m_TestMode = pTest;
        if(m_TestMode)
            m_ScreenModeBk = m_ScreenMode;
        m_ScreenMode = 1;

        //emit sInfo("Big Screen");
    }
    m_UiContainer->update();
    return true;
}


qreal GraphicsText::GetMinSrnLen()
{
#ifdef SUPPORT_SCENE
    UiScene *tmpScene = VAR_CAST<UiScene*>(m_UiContainer.data());
    if(tmpScene && tmpScene->m_Desktop)
    {
        QRectF tmpRect = tmpScene->m_Desktop->rect();
        return qMin(tmpRect.width(), tmpRect.height())-50;//>@兼顾4寸显示器
    }
#endif
    return 0;
}

void GraphicsText::UpdateBigRect()  //>@当屏幕旋转是调用
{
#ifdef SUPPORT_SCENE
    UiScene *tmpScene = VAR_CAST<UiScene*>(m_UiContainer.data());
    if(tmpScene && tmpScene->m_Desktop)
    {
        QRectF tmpRect = tmpScene->m_Desktop->rect();
        qreal tmpWidth = GetMinSrnLen();
        qreal tmpHeight = tmpWidth * 2 / 3;
        m_BigRect = QRectF((tmpRect.width()-tmpWidth)/2, (tmpRect.height()-tmpHeight)/2, tmpWidth, tmpHeight);
    }
#endif
}

bool GraphicsText::CanUpdateText()
{
    bool tmpShow = true;
    if((m_ScreenMode == 0) && !m_Function.isNull() && m_Function.data()->hasText())
    {
        tmpShow = false;
    }
    return tmpShow;
}
//
//>@  xx:xx;xx:xx
QString GraphicsText::ExecCommand(QString pCommand)
{
    QString tmpRet;
    IDE_TRACE_INT(isVisible());
    if(m_RcStyle)
    {
        if(m_RcStyle->mRcType == COMRC_TEXT)  //>@根据大小，计算字体大小
        {
            QStringList tmpCmdList = pCommand.split(';');
            for(int i=0;i<tmpCmdList.count();i++)
            {
                QString tmpCmd = tmpCmdList.at(i);
                if(tmpCmd.isEmpty())
                    continue;
                if(tmpCmd.contains("size:", Qt::CaseInsensitive))
                {
                    QString tmpCmdName = "size";
                    QString tmpCmdContent = tmpCmd.mid(tmpCmdName.count()+1);
                    int index = tmpCmdContent.toInt();
                    if(index)
                    {
                        SetTextShowMode(1);
                    }
                    else
                    {
                        L_IDE_TRACE();
                        SetTextShowMode(0);
                    }
                    if(!tmpRet.isEmpty())
                        tmpRet.append(';');
                    tmpRet.append(QString("%1:success").arg(tmpCmdName));
                }
                else if(tmpCmd.contains("content:", Qt::CaseInsensitive))
                {
                    QString tmpCmdName = "content";
                    QString tmpString = tmpCmd.mid(tmpCmdName.count()+1);
                    //>@content:row|column|string

                    //emit sInfo(tmpString);

                    QStringList tmpList = tmpString.split('|');
                    if(tmpList.count() >= 3)
                    {
                        int row = tmpList.at(0).toInt();
                        if(row < m_MaxRow)
                        {
                            int column = tmpList.at(1).toInt();
                            QString tmpStr = tmpList.at(2);
                            int strsize = tmpStr.toLocal8Bit().size();
                            memcpy(m_TextInfo[D_TEXT_MSG][row].mString+column, tmpStr.toLocal8Bit().data(), strsize);
                            //>@计算此行的起始与终止点
                            m_TextInfo[D_TEXT_MSG][row].mShowStart = column;
                            m_TextInfo[D_TEXT_MSG][row].mShowEnd = column + strsize;
                            m_TextInfo[D_TEXT_MSG][row].mString[m_TextInfo[D_TEXT_MSG][row].mShowEnd+1] = 0;

                            if(!CanUpdateText())
                            {
                                GraphicsFunction *tmpFunction = m_Function.data();
                                //m_TextMode = D_TEXT_MSG;
                                IDE_TRACE();
                            }
                            else
                            {
                                IDE_TRACE();
                                m_TextMode = D_TEXT_MSG;
                                emit sUpdateCom(0);
                            }
                        }
                    }
                    if(!tmpRet.isEmpty())
                        tmpRet.append(';');
                    tmpRet.append(QString("%1:success").arg(tmpCmdName));
                }
                else if(tmpCmd.contains("clear:", Qt::CaseInsensitive))
                {
                    QString tmpCmdName = "clear";
                    QString tmpString = tmpCmd.mid(tmpCmdName.count()+1);
                    //>@clear:row
                    int row = tmpString.toInt();
                    if(row < m_MaxRow)
                    {
                        m_TextInfo[D_TEXT_MSG][row].mShowStart = 0;
                        m_TextInfo[D_TEXT_MSG][row].mShowEnd = 0;
                        memset(m_TextInfo[D_TEXT_MSG][row].mString, D_FILLCHAR, m_MaxColumn);
                        emit sUpdateCom(0);
                    }
                    if(!tmpRet.isEmpty())
                        tmpRet.append(';');
                    tmpRet.append(QString("%1:success").arg(tmpCmdName));
                }
            }
        }
    }
    return tmpRet;
}

void GraphicsText::OperateClear(QVariant pPara)
{
    for(int i=0;i<D_TEXT_MODE;i++)
    {
        for(int j=0;j<D_TEXT_NUM;j++)
        {
            m_TextInfo[i][j].mShowStart = 0;
            m_TextInfo[i][j].mShowEnd = 0;
            memset(m_TextInfo[i][j].mString, D_FILLCHAR, m_MaxColumn);
        }
    }
    if(m_ScreenMode == 1)
    {
        L_IDE_TRACE();
        SetTextShowMode(0);
    }
    emit sUpdateCom(0);
}

void GraphicsText::updateEffect(AREA_OPERATE pOperate, QVariant pPara)
{
    GraphicsOperate::updateEffect(pOperate, pPara);
    switch(pOperate)
    {
        case OPERATE_KEYPRESS:
        {
            //>@分解坐标，如果点击上半区域，则向上滚动
            QPoint tmpPoint = pPara.toPoint();
            if(tmpPoint.y() < size().height()/2)
            {
                updateEffect(getPrevRc(m_TextState), OPERATE_STEADY);
            }
            else if(tmpPoint.y() > size().height()/2)
            {
                updateEffect(getNextRc(m_TextState), OPERATE_STEADY);
            }
            break;
        }
        case OPERATE_CHANGE:
        {
            int index = pPara.toInt();
            //>@如果遇到0xffff，则默认播放下一张
            if(index == NEXTRC)
                index = getNextRc(m_TextState);
            updateEffect(index, OPERATE_STEADY);
            break;
        }
        default:
            break;
    }
}

void GraphicsText::updateEffect(int pState, AREA_OPERATE pOperate)
{
    bool tmpStateChange = false;
    bool tmpOperateChange = false;
    if(pState != m_TextState)
        tmpStateChange = true;
    if(pOperate != m_OperateInfo[STEP1].mOperate)
        tmpOperateChange = true;
    if(tmpStateChange == false && tmpOperateChange == false)
        return;
    m_OperateInfo[STEP0].mValid = false;
    m_OperateInfo[STEP1].mValid = false;
    m_Animator.stop();
    if(tmpStateChange)
    {
        m_OperateInfo[STEP0].mValid = true;
        m_OperateInfo[STEP0].mOperate = OPERATE_CHANGE;
        m_PixmapPointer.mLastPointer = m_PixmapPointer.mCurPointer;
        m_PixmapPointer.mCurPointer = GetPixmap(pState);
    }
    m_OperateInfo[STEP1].mValid = true;
    m_OperateInfo[STEP1].mOperate = pOperate;
    m_TextState = pState;
    //>@执行STEP0中的内容
    OperateStep0();
}












DesignText::DesignText(QGraphicsItem *parent):
    GraphicsText(parent)
{
    QAction *tmpDefault = m_ActionGroup->addAction(tr("0"));
    for(int i=1;i<10;i++)
    {
        m_ActionGroup->addAction(QString("%1").arg(i));
    }
    tmpDefault->setChecked(true);
}

QList<QAction*> DesignText::GetActionList()
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

void DesignText::ExecAction(QAction *pAction)
{
    updateEffect(pAction->text().toInt(), OPERATE_STEADY);
}

