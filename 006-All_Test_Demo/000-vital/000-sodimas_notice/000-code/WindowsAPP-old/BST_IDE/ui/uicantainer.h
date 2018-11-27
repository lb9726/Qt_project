#ifndef UICANTAINER_H
#define UICANTAINER_H

#include "global.h"

typedef enum{
    UI_IDEL = 0,
    UI_LOADING,
    UI_ERROR
}UI_STATUS;

class UiCantainerBase
{
public:
    UiCantainerBase()
    {
        m_InitShow = true;
        m_Status = UI_IDEL;
        m_ExtraClass = false;  //>@
        m_UiManager = 0;
        m_UiDirection = Horizontal;
        m_OriginRotate = 0;
        m_UiLayout = 0;
    }
    ~UiCantainerBase()
    {
        Release();
    }

    //>@是否使用外部的ui资源
    void UseExtra(bool pEnable) {m_ExtraClass = pEnable;}

    //>@加载UI
    virtual bool LoadUI(QString pFileName);

    virtual bool Initialize(uiManager *pManager);
    virtual bool Initialize(QString pFileName);
    virtual void Release();

    DIRECTION GetDirection(int pRotate);

    int OriginRotate();  //>@获取原始屏幕的旋转角度，主要针对于LINUX_QWS设备
    virtual bool Rotate(int pNewRotate) {return false;}

    void InitLayout(int pLayout) {m_UiLayout = pLayout;}  //>@在界面初始化前，使用InitLayout更改Layout
    virtual void ModifyLayout(int pLayout);               //>@在界面初始化后，使用ModifyLayout更改Layout
    int GetLayout() {return m_UiLayout;}

    virtual bool UpdateUI() {return false;}  //>@表示根据最新的配置刷新界面

    //>@界面函数
    virtual bool InitUI();      //>@初始化UI文件
    virtual bool ReleaseUI(){return false;}   //>@释放UI文件
    void SaveUI() {if(m_UiManager) m_UiManager->SaveUI();}
    virtual bool OperateUI(AREA_OPERATE pOperate, QList<COM_TYPE> pExceptNodes) {return false;}

    virtual void SetGeometory(QRectF pRect) {}
    virtual QRectF GetGeometory() {return QRectF();}

public:
    int                  m_Status;
    bool                 m_ExtraClass;   //>@指示是否为外部带入的资源类（VIEW和manager）
    int                  m_OriginRotate;
    DIRECTION            m_UiDirection;
    int                  m_UiLayout;   //>@
    uiManager           *m_UiManager;

    bool                 m_InitShow;  //>@指示LoadUI后是否需要显示界面
};

#endif // UICANTAINER_H
