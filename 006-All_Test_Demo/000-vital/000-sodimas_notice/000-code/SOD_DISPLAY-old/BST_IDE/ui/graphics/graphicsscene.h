#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include "uicantainer.h"
#include "graphicsoperate.h"

class GraphicsScene : public QGraphicsScene, public UiCantainerBase
{
    Q_OBJECT
public:
    GraphicsScene(QObject *parent = 0);
    ~GraphicsScene();

    bool UpdateUI();
    bool ReleaseUI(){ DeleteAllCom(); return true; }

    virtual bool AddComponent(COM_TYPE pComType, QDomElement pComElement, QPointF pScenePos);
    bool DelComponent(GraphicsOperate *&pComPtr);
    bool SelectComponent(COM_TYPE pComType);
    bool SelectComponent(GraphicsOperate *pComPtr);

    void DeleteAllCom();  //>@释放Scene中的所有组件

    virtual void showFrame(GraphicsOperate *pComItem) {}
    virtual void hideFrame(bool pDelFlag = false) {}
    virtual void updateFrame() {}

public:
    GraphicsOperate     *m_SelectComponet;   //>@当前选中的控件数据
    QHash<COM_TYPE, GraphicsOperate*> m_ComDiagramGroup;   //>@xx.ui页面中的组件（subui和control）的GraphicsBase指针
};

#endif // GRAPHICSSCENE_H

