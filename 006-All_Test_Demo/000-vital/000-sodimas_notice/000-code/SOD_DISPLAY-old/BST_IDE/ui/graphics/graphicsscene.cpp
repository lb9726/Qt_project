#include "graphicsscene.h"

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent), UiCantainerBase()
{
    m_SelectComponet = 0;
}

GraphicsScene::~GraphicsScene()
{

}

void GraphicsScene::DeleteAllCom()
{
    hideFrame(true);
    //>@删除scene下的所有item
    QList<COM_TYPE> tmpComList = m_ComDiagramGroup.keys();
    for(int i=tmpComList.count()-1; i>=0; i--)
    {
        GraphicsOperate* tmpComItem = m_ComDiagramGroup.take(tmpComList.at(i));
        if(tmpComItem)
        {
            IDE_TRACE_STR(tmpComItem->m_ComPath);
            removeItem(tmpComItem);
            tmpComItem->deleteLater();
        }
    }
    m_ComDiagramGroup.clear();
}

bool GraphicsScene::AddComponent(COM_TYPE pComType, QDomElement pComElement, QPointF pScenePos)
{
    return true;
}

//>@删除某个控件时主要：
//>@  1、断开与其父的连接、
//>@  2、在Scene/m_UiManager/m_ComDiagramGroup中删掉自己以及子节点
bool GraphicsScene::DelComponent(GraphicsOperate *&pComPtr)
{
    if(!pComPtr)
        return true;
    if(m_SelectComponet == pComPtr)
        hideFrame();
    QList<QGraphicsItem *> tmpChildItems = pComPtr->childItems();
    for(int i=tmpChildItems.count()-1;i>=0;i--)
    {
        GraphicsOperate *tmpChildItem = (GraphicsOperate *)tmpChildItems.at(i);
        if(!tmpChildItem)
            continue;
        DelComponent(tmpChildItem);
    }
    COM_TYPE tmpType = (COM_TYPE)(pComPtr->data(COMTYPE).toInt());
    m_ComDiagramGroup.remove(tmpType);
    removeItem(pComPtr);
    pComPtr->DeleteComponent();
    pComPtr->deleteLater();
    pComPtr = 0;
    return true;
}

bool GraphicsScene::SelectComponent(COM_TYPE pComType)
{
    GraphicsOperate *tmpComItem = m_ComDiagramGroup.value(pComType);
    return SelectComponent(tmpComItem);
}

bool GraphicsScene::SelectComponent(GraphicsOperate *pComPtr)
{
    if(!pComPtr)
        return false;
    if(pComPtr == m_SelectComponet)
        return true;
    //>@如果更换选择控件，则之前被选择的控件需要停止特效操作。
    if(m_SelectComponet)
        m_SelectComponet->StopAnimate();
    m_SelectComponet = pComPtr; //>@保存当前选中的控件
    m_SelectComponet->RefreshParameter();
    updateFrame();  //>@将Frame更新为当前COMINFO的框
    return true;
}

bool GraphicsScene::UpdateUI()
{
    QList<COM_TYPE> tmpList = m_ComDiagramGroup.keys();
    COM_TYPE tmpType;
    foreach(tmpType, tmpList)
    {
        if(tmpType == COM_DESKTOP)
            continue;
        GraphicsOperate* tmpCom = m_ComDiagramGroup.value(tmpType);
        if(!tmpCom)
            continue;
        QRect tmpHRect, tmpVRect;
        if(!m_UiManager->GetComRect(m_UiLayout, tmpCom->m_ComElement, tmpHRect, tmpVRect))
        {
            IDE_TRACE_INT(tmpType);
            continue;
        }
        tmpCom->m_ComRect[Horizontal] = tmpHRect;
        tmpCom->m_ComRect[Vertical] = tmpVRect;
        if(m_UiDirection == Horizontal)
            tmpCom->ModifyGeomotery(tmpCom->m_ComRect[Horizontal]);
        else
            tmpCom->ModifyGeomotery(tmpCom->m_ComRect[Vertical]);
    }
}

