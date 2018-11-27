#include "graphicsregion.h"
#include "graphicsscene.h"
#include "graphicstext.h"

GraphicsRegion::GraphicsRegion(QGraphicsItem *parent) :
    GraphicsOperate(parent)
{
    m_RegionState = -1;
}

bool GraphicsRegion::Start()
{
    if(GraphicsComponent::Start() == false)
    {
        IDE_TRACE();
        return false;
    }
    m_RegionState = -1;
    updateEffect(OPERATE_CHANGE, QVariant(m_RcList.first()));
    return true;
}

void GraphicsRegion::UpdateCom(int pFrame)
{
    GraphicsComponent::UpdateCom(pFrame);
}

void GraphicsRegion::updateEffect(AREA_OPERATE pOperate, QVariant pPara)
{
    GraphicsComponent::updateEffect(pOperate, pPara);
    switch(pOperate)
    {
        case OPERATE_KEYPRESS:
        {
            //>@分解坐标，如果点击上半区域，则向上滚动
            QPoint tmpPoint = pPara.toPoint();
            if(tmpPoint.y() < size().height()/2)
            {
                updateEffect(getPrevRc(m_RegionState), OPERATE_STEADY);
            }
            else if(tmpPoint.y() > size().height()/2)
            {
                updateEffect(getNextRc(m_RegionState), OPERATE_STEADY);
            }
            break;
        }
        case OPERATE_CHANGE:
        {
            int index = pPara.toInt();
            //>@如果遇到NEXTRC，则默认播放下一张
            if(index == NEXTRC)
                index = getNextRc(m_RegionState);
            else if(index == PREVRC)
                index = getPrevRc(m_RegionState);
            else if(index == HOLDRC)
                return;
            updateEffect(index, OPERATE_STEADY);
            break;
        }
        default:
            break;
    }
}

void GraphicsRegion::updateEffect(int pState, AREA_OPERATE pOperate)
{
    bool tmpStateChange = false;
    bool tmpOperateChange = false;
    if(pState != m_RegionState)
        tmpStateChange = true;
    if(pOperate != m_OperateInfo[STEP1].mOperate)
        tmpOperateChange = true;
    if(tmpStateChange == false && tmpOperateChange == false)
    {
        return;
    }
    m_OperateInfo[STEP0].mValid = false;
    m_OperateInfo[STEP1].mValid = false;
    m_Animator.stop();
    if(tmpStateChange)
    {
        m_OperateInfo[STEP0].mValid = true;
        m_OperateInfo[STEP0].mOperate = OPERATE_CHANGE;
        m_PixmapPointer.mLastPointer = m_PixmapPointer.mCurPointer;
        m_PixmapPointer.mCurPointer = GetPixmap(pState, size());
    }
    m_OperateInfo[STEP1].mValid = true;
    m_OperateInfo[STEP1].mOperate = pOperate;
    m_RegionState = pState;
    //>@执行STEP0中的内容
    OperateStep0();
}

GraphicsDesktop::GraphicsDesktop(QGraphicsItem *parent) :
    GraphicsRegion(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable,false);
}

bool GraphicsDesktop::Start()
{
    if(GraphicsComponent::Start() == false)
    {
        IDE_TRACE();
        return false;
    }
    m_RegionState = -1;
    int tmpKey;
    foreach(tmpKey, m_RcList)
    {
        if(tmpKey < 100)
            m_HRcList.append(tmpKey);
        else
            m_VRcList.append(tmpKey);
    }
    if(m_UiContainer->m_UiDirection == Horizontal)
    {
        if(!m_HRcList.isEmpty())
            updateEffect(OPERATE_CHANGE, QVariant(m_HRcList.first()));
    }
    else if(m_UiContainer->m_UiDirection == Vertical)
    {
        if(!m_VRcList.isEmpty())
            updateEffect(OPERATE_CHANGE, QVariant(m_VRcList.first()));
    }
    return true;
}

void GraphicsDesktop::OperateTurn(QVariant pPara)
{
    SetComGeometory(GetCurRect());
    int tmpIndex = -1;
    if(m_UiContainer->m_UiDirection == Horizontal)
    {
        if(m_RegionState >= 100)
            tmpIndex = m_RegionState - 100;
    }
    else if(m_UiContainer->m_UiDirection == Vertical)
    {
        if(m_RegionState >= 0 && m_RegionState < 100)
            tmpIndex = m_RegionState + 100;
    }
    if(tmpIndex >= 0)
        updateEffect(OPERATE_CHANGE, QVariant(tmpIndex));
}

GraphicsImage::GraphicsImage(QGraphicsItem *parent) :
    GraphicsRegion(parent)
{
}

GraphicsLogo::GraphicsLogo(QGraphicsItem *parent) :
    GraphicsRegion(parent)
{
#ifdef N329
    QString tmpFilePath = D_FSRCPATH + D_BOOTDIR + QString("transparent.png");
    IDE_TRACE_STR(tmpFilePath);
    m_TranparentImg = QImage(tmpFilePath);
#endif
    m_CurIcon.clear();
}

QPixmap GraphicsLogo::LoadIcon(QPixmap pPixmap, QSizeF pSize)
{
    IDE_TRACE_INT(pPixmap.isNull());
    if(!pPixmap.isNull())
    {
        if(pPixmap.size() == pSize.toSize())
            return pPixmap;
        QSize tmpSize = pPixmap.size();
        qreal tmpWidth, tmpHeight;
        qreal tmpWidthRatio = pSize.width()/(qreal)(tmpSize.width());
        qreal tmpHeightRatio = pSize.height()/(qreal)(tmpSize.height());
        if(tmpWidthRatio < tmpHeightRatio)
        {
            tmpWidth = pSize.width();
            tmpHeight = (qreal)(tmpSize.height()) * tmpWidthRatio;
        }
        else
        {
            tmpWidth = (qreal)(tmpSize.width()) * tmpHeightRatio;
            tmpHeight = pSize.height();
        }
        //>@以宽度为准
        QRect tmpResRect = QRect(QPoint(pSize.width()/2.0 - tmpWidth/2.0, pSize.height()/2.0 - tmpHeight/2.0),
                                 QSize(tmpWidth, tmpHeight));
#ifdef N329
        QImage tmpImage = m_TranparentImg.scaled(size().toSize());
        if(tmpImage.isNull())
        {
            IDE_TRACE();
            return QPixmap();
        }
#else
        QImage tmpImage(pSize.width(), pSize.height(), QImage::Format_ARGB32_Premultiplied);
        if(tmpImage.isNull())
            return QPixmap();
        tmpImage.fill(Qt::transparent); //>@创建透明图层
#endif
        QPainter p(&tmpImage);
        p.drawPixmap(tmpResRect, pPixmap);
        p.end();
        return QPixmap::fromImage(tmpImage);
    }
    return pPixmap;
}

void GraphicsLogo::slot_ShowIcon(QString pIconName)
{
    if(pIconName.isEmpty())
        return;
    if(!pIconName.compare(m_CurIcon, Qt::CaseInsensitive))
        return;
    m_CurIcon = pIconName;
    QString tmpFilePath = D_FSRCPATH + D_BOOTDIR + QString("%1.png").arg(pIconName);
    IDE_TRACE_STR(tmpFilePath);
    m_PixmapPointer.mLastPointer = m_PixmapPointer.mCurPointer = LoadIcon(QPixmap(tmpFilePath), size());
    IDE_TRACE_INT(m_PixmapPointer.mCurPointer.isNull());
    emit sUpdateCom(0);
    QtSleep(10);
}

void GraphicsLogo::slot_HideIcon(QString pIconName)
{
    if(pIconName.isEmpty())
        return;
    if(pIconName.compare(m_CurIcon, Qt::CaseInsensitive))  //>@如果是隐藏当前显示图标，则成功
        return;
    m_CurIcon.clear();
    QString tmpFilePath = D_FSRCPATH + D_BOOTDIR + QString("%1.png").arg(pIconName);
    IDE_TRACE_STR(tmpFilePath);
    m_PixmapPointer.mLastPointer = m_PixmapPointer.mCurPointer = QPixmap();
    emit sUpdateCom(0);
    QtSleep(10);
}

GraphicsWeather::GraphicsWeather(QGraphicsItem *parent) :
    GraphicsRegion(parent)
{
}

GraphicsNamePlate::GraphicsNamePlate(QGraphicsItem *parent) :
    GraphicsRegion(parent)
{
}

GraphicsFunction::GraphicsFunction(QGraphicsItem *parent) :
    GraphicsRegion(parent)
{
    m_ShowText = false;
}

bool GraphicsFunction::Start()
{
    GraphicsComponent::Start();
    m_RegionState = HIDERC;
    return true;
}

bool GraphicsFunction::hasText()
{
    return m_ShowText;
}

void GraphicsFunction::updateEffect(int pState, AREA_OPERATE pOperate)
{
    bool tmpStateChange = false;
    bool tmpOperateChange = false;
    if(pState != m_RegionState)
        tmpStateChange = true;
    if(pOperate != m_OperateInfo[STEP1].mOperate)
        tmpOperateChange = true;
    if(tmpStateChange == false && tmpOperateChange == false)
    {
        return;
    }
    m_OperateInfo[STEP0].mValid = false;
    m_OperateInfo[STEP1].mValid = false;
    m_Animator.stop();

    QString tmpString;
    QPixmap tmpPixmap;
    bool tmpLogoFlag = false;
    if(tmpStateChange)
    {
        m_OperateInfo[STEP0].mValid = true;
        m_OperateInfo[STEP0].mOperate = OPERATE_CHANGE;
        m_PixmapPointer.mLastPointer = m_PixmapPointer.mCurPointer;
        RC_INFO *tmpRcInfo = m_EffectRC.value(pState);
        if(tmpRcInfo)
        {
            tmpPixmap = GetPixmap(tmpRcInfo, size());
            //>@此处要求，在ui文件必须是以UTF-8进行编码的，可以在QtCreator中“以UTF-8”进行保存。
            tmpString = tmpRcInfo->mTipText.toUtf8();
            if(pState < 100)
                tmpLogoFlag = true;
        }
        m_PixmapPointer.mCurPointer = tmpPixmap;
    }
    m_OperateInfo[STEP1].mValid = true;
    m_OperateInfo[STEP1].mOperate = pOperate;
    m_RegionState = pState;
    //>@执行STEP0中的内容
    OperateStep0();

    //>@解决文本显示比图片显示快的问题。
    if(tmpStateChange && isVisible())
    {
        if(m_GraphicsText.isNull() && m_UiContainer.data())
            m_GraphicsText = VAR_CAST<GraphicsText*>(m_UiContainer.data()->m_ComDiagramGroup.value(COM_TEXT));
        if(m_GraphicsText.data())
        {
            m_ShowText = tmpLogoFlag;
            m_GraphicsText.data()->ShowTipText(m_ShowText, tmpString);
        }
    }
}


void GraphicsFunction::recoveryfrombigScreen()
{
    if(isVisible())
    {
        if(m_GraphicsText.isNull() && m_UiContainer.data())
            m_GraphicsText = VAR_CAST<GraphicsText*>(m_UiContainer.data()->m_ComDiagramGroup.value(COM_TEXT));
        if(m_GraphicsText.data())
        {
            RC_INFO *tmpRcInfo = m_EffectRC.value(m_RegionState);
            if(tmpRcInfo)
            {
                QString tmpString = tmpRcInfo->mTipText.toUtf8();
                m_GraphicsText.data()->ShowTipText(m_ShowText, tmpString);
            }
        }
    }
}


