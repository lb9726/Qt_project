#include "graphicsoperate.h"
#include "graphicsscene.h"

GraphicsOperate::GraphicsOperate(QGraphicsItem *parent, GraphicsScene *pScene, QRectF pRect):
    GraphicsComponent(parent, pScene, pRect)
{
    mDragValid = false;
}

bool GraphicsOperate::RefreshParameter()
{
    mDragValid = false;
    mParComponet = (GraphicsComponent*)(parentWidget());
    if(!mParComponet)
        return false;
    if(m_ComType < COM_AREA)
        return false;
    //>@
    m_CurRect = sceneBoundingRect();
    //>@只有在修改Area的时候才会监测控件不能超出桌面范围
    if(m_UiContainer->m_UiManager->canSwitch() && m_ComType > COM_AREA && m_ComType < COM_COMPONENT)
    {
        DIRECTION tmpOppDirection = (m_UiContainer->m_UiDirection==Horizontal)?Vertical:Horizontal;
        QRectF tmpParOppRect = QRectF(m_UiContainer->m_UiManager->GetComRect(m_UiContainer->m_UiLayout, m_UiContainer->m_UiManager->GetParComDom(m_ComElement), tmpOppDirection));
        m_OppMaxWidth = tmpParOppRect.width();
        m_OppMaxHeight = tmpParOppRect.height();
    }
    m_CurLeft = m_CurRect.left();
    m_CurTop = m_CurRect.top();
    m_CurRight = m_CurRect.right();
    m_CurBottom = m_CurRect.bottom();
    //>@计算可拖动的最大大小
    QRectF tmpParRect = mParComponet->sceneBoundingRect();
    m_LeftLimitMin = tmpParRect.left();
    m_TopLimitMin = tmpParRect.top();
    m_RightLimitMax = tmpParRect.right();
    m_BottomLimitMax = tmpParRect.bottom();
    //>@计算最小大小
    QRectF tmpChildRect = mapRectToScene(childrenBoundingRect());
    if(tmpChildRect.isEmpty())
    {
        m_LeftLimitMax = m_CurRight - COM_MINSIZE;
        m_TopLimitMax = m_CurBottom - COM_MINSIZE;
        m_RightLimitMin = m_CurLeft + COM_MINSIZE;
        m_BottomLimitMin = m_CurTop + COM_MINSIZE;
    }
    else
    {
        //>@父窗口移动时子窗口相对于父窗口位置不变，而进行移动。
        m_LeftLimitMax = m_CurLeft + m_CurRight - tmpChildRect.right();
        m_TopLimitMax = m_CurTop + m_CurBottom - tmpChildRect.bottom();
        m_RightLimitMin = tmpChildRect.right();
        m_BottomLimitMin = tmpChildRect.bottom();
    }
    mDragValid = true;
    return true;
}

//>@修改scene下的坐标
bool GraphicsOperate::ModifySceneRect(QRectF pRect)
{
    if(mParComponet && (parentItem() != (QGraphicsItem*)mParComponet))
        setParentItem(mParComponet);
    //>@只有在修改Area的时候才会监测控件不能超出桌面范围
    if(m_UiContainer->m_UiManager->canSwitch() && m_ComType > COM_AREA && m_ComType < COM_COMPONENT)
    {
        if(pRect.width() > m_OppMaxWidth || pRect.height() > m_OppMaxHeight)
        {
            int ret = QMessageBox::question(0, tr("Question"),
                                           tr("This Component is overload in oppside direction,\n"
                                              "if you click \"OK\", you will never switch to the other direction!"
                                              "Do you want to query now?"),
                                           QMessageBox::Yes | QMessageBox::No,
                                           QMessageBox::No);
            if(ret == QMessageBox::No)
            {
                //>@重新初始化为原来的坐标
                QRectF tmpOldComRect = mParComponet->mapRectFromScene(m_CurRect);
                SetComGeometory(tmpOldComRect);
                RefreshParameter();
                return false;
            }
            //>@否则禁止切换，也即设置ui文件中
            m_UiContainer->m_UiManager->SetDirectionPermisson(m_UiContainer->m_UiDirection);
        }
    }
    QRectF tmpComRect = mParComponet->mapRectFromScene(pRect);
    //>@如果修改坐标失败，则重新初始化为原来的坐标
    if(!m_UiContainer->m_UiManager->ModifyComRect(m_UiContainer->m_UiLayout, m_ComElement, tmpComRect.toRect(), m_UiContainer->m_UiDirection))
    {
        //>@重新初始化为原来的坐标
        QRectF tmpOldComRect = mParComponet->mapRectFromScene(m_CurRect);
        SetComGeometory(tmpOldComRect);
        RefreshParameter();
        return false;
    }
    SetComGeometory(tmpComRect);
    m_CurRect = pRect;
    emit sSceneRectChanged(pRect);
    return true;
}

QRectF GraphicsOperate::GetVirtualSceneRect()
{
    return QRectF(m_CurLeft, m_CurTop, m_CurRight-m_CurLeft, m_CurBottom-m_CurTop);
}

//>@判断是否可以修改左边沿
qreal GraphicsOperate::ModifyComLeft(qreal pValue, bool pSync)
{
    if(pValue < m_LeftLimitMin)
    {
        m_CurLeft = m_LeftLimitMin;
    }
    else if(pValue > m_LeftLimitMax)
    {
        m_CurLeft = m_LeftLimitMax;
    }
    else
    {
        m_CurLeft = pValue;
    }
    if(pSync)
        ModifySceneRect(GetVirtualSceneRect());
    return m_CurLeft;
}

qreal GraphicsOperate::ModifyComTop(qreal pValue, bool pSync)
{
    if(pValue < m_TopLimitMin)
    {
        m_CurTop = m_TopLimitMin;
    }
    else if(pValue > m_TopLimitMax)
    {
        m_CurTop = m_TopLimitMax;
    }
    else
    {
        m_CurTop = pValue;
    }
    if(pSync)
        ModifySceneRect(GetVirtualSceneRect());
    return m_CurTop;
}

qreal GraphicsOperate::ModifyComRight(qreal pValue, bool pSync)
{
    if(pValue < m_RightLimitMin)
    {
        m_CurRight = m_RightLimitMin;
    }
    else if(pValue > m_RightLimitMax)
    {
        m_CurRight = m_RightLimitMax;
    }
    else
    {
        m_CurRight = pValue;
    }
    if(pSync)
        ModifySceneRect(GetVirtualSceneRect());
    return m_CurRight;
}

qreal GraphicsOperate::ModifyComBottom(qreal pValue, bool pSync)
{
    if(pValue < m_BottomLimitMin)
    {
        m_CurBottom = m_BottomLimitMin;
    }
    else if(pValue > m_BottomLimitMax)
    {
        m_CurBottom = m_BottomLimitMax;
    }
    else
    {
        m_CurBottom = pValue;
    }
    if(pSync)
        ModifySceneRect(GetVirtualSceneRect());
    return m_CurBottom;
}

qreal GraphicsOperate::ModifyComWidth(qreal pValue, bool pSync)
{
    qreal tmpRight = m_CurLeft + pValue;
    if(tmpRight < m_RightLimitMin)
    {
        m_CurRight = m_RightLimitMin;
    }
    else if(tmpRight > m_RightLimitMax)
    {
        m_CurRight = m_RightLimitMax;
    }
    else
    {
        m_CurRight = tmpRight;
    }
    if(pSync)
        ModifySceneRect(GetVirtualSceneRect());
    return m_CurRight-m_CurLeft;
}

qreal GraphicsOperate::ModifyComHeight(qreal pValue, bool pSync)
{
    qreal tmpBottom = m_CurTop + pValue;
    if(tmpBottom < m_BottomLimitMin)
    {
        m_CurBottom = m_BottomLimitMin;
    }
    else if(tmpBottom > m_BottomLimitMax)
    {
        m_CurBottom = m_BottomLimitMax;
    }
    else
    {
        m_CurBottom = tmpBottom;
    }
    if(pSync)
        ModifySceneRect(GetVirtualSceneRect());
    return m_CurBottom-m_CurTop;
}

QPointF GraphicsOperate::ModifyComPos(QRectF pComRect, bool pSync)
{
    if(!pComRect.isValid())
        return QPointF();
    m_CurLeft = pComRect.left();
    m_CurTop = pComRect.top();
    m_CurRight = m_CurLeft + pComRect.width();
    m_CurBottom = m_CurTop + pComRect.height();
    if(m_CurLeft<m_LeftLimitMin)
    {
        m_CurLeft = m_LeftLimitMin;
    }
    else if(m_CurRight>m_RightLimitMax)
    {
        m_CurLeft = m_RightLimitMax-pComRect.width();
    }
    if(m_CurTop<m_TopLimitMin)
    {
        m_CurTop = m_TopLimitMin;
    }
    else if(m_CurBottom>m_BottomLimitMax)
    {
        m_CurTop = m_BottomLimitMax-pComRect.height();
    }
    m_CurRight = m_CurLeft + pComRect.width();
    m_CurBottom = m_CurTop + pComRect.height();
    if(pSync)
        ModifySceneRect(GetVirtualSceneRect());
    return QPointF(m_CurLeft, m_CurTop);
}

bool GraphicsOperate::ModifyStateEffect(QString pStateName, QString pEffectName)
{
    //>@如果没有此状态机，则创建
    QString tmpEffectPara = GetDefaultEffectPara(pStateName, pEffectName);
    QDomElement tmpStateElement = m_UiContainer->m_UiManager->ModifyComState(m_ComElement, pStateName, pEffectName, tmpEffectPara);
    if(tmpStateElement.isNull())
        return false;
    //emit sStateEffectChanged(pStateName, pEffectName);
    //>@同步设置mComponent中的值
    InitState(tmpStateElement);
    return true;
}

bool GraphicsOperate::ModifyEffectPara(QString pStateName, QString pParaName, QString pParaValue)
{
    QDomElement tmpElement = m_UiContainer->m_UiManager->GetComState(m_ComElement, pStateName);
    if(tmpElement.isNull())
        return false;
    QString tmpParas = tmpElement.attribute(D_EFFECTPARA);
    //>@如果当前参数列表中没有此参数，则添加
    QString tmpNewParas;
    int index1 = tmpParas.indexOf(pParaName);
    if(index1 == -1)
    {
        if(!tmpParas.isEmpty())
            tmpParas.append(';');
        tmpNewParas = tmpParas + QString("%1:%2").arg(pParaName).arg(pParaValue);
    }
    else
    {
        int index2 = tmpParas.indexOf(':', index1);
        int index3 = tmpParas.indexOf(';', index1);
        if(index3 == -1)  //>@如果为最后一个参数
            index3 = tmpParas.count();
        if(index2 == -1 || index2 > index3)
            return false;
        tmpNewParas = tmpParas.replace(index2+1, index3-index2-1, pParaValue);
    }
    tmpElement.setAttribute(D_EFFECTPARA, tmpNewParas);
    //>@同步设置mComponent中的值
    InitSubPara(pStateName, pParaName, pParaValue);
    //emit sEffectParaChanged(pStateName, pParaName, pParaValue);
    return true;
}

bool GraphicsOperate::ModifyRcFile(QString pStateName, QString pRcName, QString pRcFile)
{
    int index = GetKey(pRcName);
    if(index == -1)
        return false;
    QDomElement tmpStateElement = m_UiContainer->m_UiManager->GetComState(m_ComElement, pStateName);
    if(tmpStateElement.isNull())
        return false;
    QDomElement tmpRcElement = tmpStateElement.firstChildElement(pRcName);
    //>@如果没有则创建
    if(tmpRcElement.isNull())
    {
        if(!m_UiContainer->m_UiManager->CreateItemElement(tmpStateElement, pRcName, pRcFile))
            return false;
    }
    else
    {
        if(!m_UiContainer->m_UiManager->ModifyItemElement(tmpRcElement, pRcFile))
            return false;
    }
    //>@修改图片资源
    LoadPath(index, pRcFile);
    RefreshRC();
    //emit sRcFileChanged(pStateName, pRcName, pRcFile);
    return true;
}

bool GraphicsOperate::DelRcFile(QString pStateName, QString pRcName)
{
    int index = GetKey(pRcName);
    if(index == -1)
        return false;
    QDomElement tmpStateElement = m_UiContainer->m_UiManager->GetComState(m_ComElement, pStateName);
    if(tmpStateElement.isNull())
        return false;
    QDomElement tmpRcElement = tmpStateElement.firstChildElement(pRcName);
    if(tmpRcElement.isNull())
        return true;
    tmpStateElement.removeChild(tmpRcElement);
    RemoveRc(index);
    RefreshRC();
    //emit sRcFileDeleted(pStateName, pRcName);
    return true;
}
