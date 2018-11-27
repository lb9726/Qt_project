#include "uiscene.h"
#ifdef IDE
    #include "devicescene.h"
#elif DISPLAY
    #include "devicemanager.h"
#elif SETTING
    #include "devicesetting.h"
#endif
#include "physicalnet.h"
#include "logicthememanager.h"
#include "physicallcd.h"

UiView::UiView(QWidget *parent) :
    QGraphicsView(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_OpaquePaintEvent, true);
    //setAttribute(Qt::WA_NoSystemBackground, true);
    setRenderHint(QPainter::Antialiasing);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setDragMode(QGraphicsView::NoDrag);

    setBackgroundBrush(Qt::black);
    //update();
}

UiView::~UiView()
{
    IDE_TRACE();
}

/**********************************************************
//>@
**********************************************************/

UiScene::UiScene(UiView *pView, QObject *parent) :
    GraphicsScene(parent)
{
#ifdef SUPPORT_KEY
    m_UiSetMenu = 0;
#endif
    if(pView)
    {
        m_ExtraClass = true;
        m_MainForm = pView;
    }
    else
    {
        m_ExtraClass = false;
        m_MainForm = new UiView;
    }
    if(!m_MainForm)
    {
        IDE_TRACE();
        return;
    }
    m_MainForm->setScene(this);
    setBackgroundBrush(Qt::black);

    m_LogicThemeManager = VAR_CAST<LogicThemeManager *>(parent);
    if(m_LogicThemeManager)
    {
        connect(m_LogicThemeManager->m_DevManager, SIGNAL(sSetMenu()), this, SLOT(slot_SetMenu()));
        connect(this, SIGNAL(sProgress(QString,quint32)), m_LogicThemeManager, SIGNAL(sProgress(QString,quint32)));
    }
    setItemIndexMethod((QGraphicsScene::NoIndex));
    m_IniParse = 0;
    m_MessageBar = 0;
    m_ProgressBar = 0;
}

UiScene::~UiScene()
{
    Release();
}

PhysicalLCD *UiScene::GetLCD()
{
#if (defined(DISPLAY) || defined(SETTING))
    return VAR_CAST<PhysicalLCD*>(m_LogicThemeManager->m_DevManager->m_PHDevList.value(P_LCD));
#else
    return (PhysicalLCD *)0;
#endif
}

bool UiScene::Rotate(int pNewRotate)
{
    if(!m_MainForm)
        return false;
    //>@获取QWS的旋转角度
    int tmpQwsRotate = OriginRotate();
    //>@最终的旋转角度，必须减去QWS的旋转角度才可以
    m_MainForm->resetTransform();
    m_MainForm->rotate((qreal)((tmpQwsRotate-pNewRotate)%360));
    //>@
    m_UiDirection = GetDirection(pNewRotate);
    OperateUI(OPERATE_TURN);
    //>@将Scene的大小设置为与Desktop大小一样，View的大小不需要改变
    m_MainForm->scene()->setSceneRect(QRectF(QPointF(0,0), GetSceneSize()));
    return true;
}

void UiScene::slot_Progress(QString pMsg, quint32 pValue)
{
    emit sProgress(pValue);
    emit sInfo(pMsg, /*MSG_PERMANENT*/10000, DM_NORMAL);
}

void UiScene::slot_Progress(QString pMsg, quint32 pPrg, quint32 pExtPrg)
{
    emit sProgress(pPrg, pExtPrg);
    emit sInfo(pMsg, /*MSG_PERMANENT*/10000, DM_NORMAL);
}

bool UiScene::Initialize(QString pFileName)
{
    bool ret = LoadUI(pFileName);
    if(!ret)
    {
        slot_SetMenu();
    }
    IDE_TRACE();
    return ret;
}

bool UiScene::LoadUI(QString pFileName)
{
    IDE_TRACE();
    m_Status = UI_LOADING;
    DeleteAllCom();
    //>@确定Theme文件的位置
    if(!QFile::exists(pFileName))
    {
        IDE_DEBUG(QString("Theme [%1] Unexist, Searching Default Theme!").arg(pFileName));
        emit sProgress(QString("Theme [%1] Unexist, Searching Default Theme!").arg(pFileName), 60);//进度条
        pFileName = QString(D_FSRCPATH + D_THEMEDIR + D_DEFAULTTHEMEDIR + D_UINAME);
        if(!QFile::exists(pFileName))
        {
            m_Status = UI_ERROR;
            IDE_DEBUG(QString("Default Theme [%1] unexist, Please contact BST!").arg(pFileName));
            emit sProgress(QString("Default Theme [%1] unexist, Please contact BST!").arg(pFileName), 60);//进度条
            return false;
        }
    }
    if(!pFileName.endsWith(QString(".ui"), Qt::CaseInsensitive))
    {
        m_Status = UI_ERROR;
        IDE_DEBUG(QString("The ui name has no suffix, Please correct!"));
        emit sProgress(QString("The ui name has no suffix, Please correct!"), 1000);
        return false;
    }
    m_UiManager = new uiManager(pFileName);
    m_UiManager->SetForcesave(false);
    if(!(m_UiManager->OpenUI()))
    {
        m_Status = UI_ERROR;
        IDE_DEBUG(QString("The ui file is invalid, Please correct!"));
        emit sProgress(QString("The ui file is invalid, Please correct!"), 1000);
        ReleaseUI();
        return false;
    }
    bool ret = Initialize(m_UiManager);
    m_Status = UI_IDEL;
    return ret;
}

bool UiScene::ReleaseUI()
{
    //>@必须这么顺序释放（先释放子，再释放父），否则在频繁切换主题时会出现释放Desktop时释放出错。
    QList<COM_TYPE> tmpList = m_ComDiagramGroup.keys();
    qSort(tmpList.begin(), tmpList.end());
    for(int i=tmpList.count()-1;i>=0;i--)
    {
        GraphicsOperate *tmpWidget = m_ComDiagramGroup.value(tmpList.at(i));
        if(tmpWidget)
            tmpWidget->deleteLater();
    }
    m_ComDiagramGroup.clear();
    IDE_TRACE_INT(m_MainForm.data());
    if(!m_ExtraClass && !m_MainForm.isNull())
        m_MainForm->deleteLater();
}

QSizeF UiScene::GetSceneSize()
{
    if(!m_Desktop)
        return QSizeF();
    return m_Desktop->size();
}

bool UiScene::Initialize(uiManager *pManager)
{
    if(!(pManager && pManager->m_Valid))
    {
        m_Status = UI_ERROR;
        IDE_TRACE();
        return false;
    }
    m_UiManager = pManager;
    //>@打开环境文件
    m_IniParse = new iniParse(m_UiManager->m_FileDir + D_ENVIRONMENT);
    m_IniParse->setProperty("/Time", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    //>@初始化映射表
    m_LogicThemeManager->m_EventMap->Initialize(m_UiManager->m_FileDir + D_MAPNAME);

    //>@根据旋转角度设置主题方向
    PhysicalLCD *tmpLCD = GetLCD();
    if(tmpLCD)
    {
        m_OriginRotate = tmpLCD->mRotation;
        m_UiDirection = GetDirection(tmpLCD->mRotation);
    }
    if(m_UiManager->m_DirectionPermisson != m_UiDirection)
    {
        if(m_UiManager->m_DirectionPermisson != HVBoth)
        {
            m_Status = UI_ERROR;
            emit sProgress(tr("Direction is invalid!"), 1000);
            ReleaseUI();
            return false;
        }
    }

    //>@初始化界面
    return InitUI();
}

void UiScene::slot_SetMenu()
{
#ifdef SUPPORT_MENU
#ifdef SUPPORT_KEY
    if(!m_UiSetMenu)
    {
        m_UiSetMenu = new SetTabBar(this);
        m_UiSetMenu->setGeometry(QRect(qApp->desktop()->geometry().center()-QPoint(400, 240), QSize(800, 480)));
    }
    //>@禁能Theme
    IDE_TRACE();
#ifdef DISPLAY
    m_LogicThemeManager->m_DevManager->SetDevicesEnable(false, DM_SETMENU);
    m_LogicThemeManager->HideTheme();
#endif

    m_UiSetMenu->exec();

    //>@显示Theme
#ifdef DISPLAY
    m_LogicThemeManager->ShowTheme();
    m_LogicThemeManager->m_DevManager->SetDevicesEnable(true, DM_SETMENU);
#endif
#endif
#endif
}

bool UiScene::InitUI()
{
    DeleteAllCom();

    //>@加载当前主题中已有的所有控件
    QList<COM_TYPE> tmpComTypeList = m_UiManager->m_ComElementGroup.keys();
    int count = tmpComTypeList.count();
    if(count < 0)
    {
        m_Status = UI_ERROR;
        return false;
    }
    int initValue = 60;
    qreal diff = (100-initValue)/(qreal)count;
    emit sProgress(QString("Initial Theme:[%1]").arg(m_UiManager->m_FileDir+m_UiManager->m_FileName), initValue);//进度条

    //>@设定Scene大小与Desktop的大小相同
    QDomElement tmpDesktopElement = m_UiManager->m_ComElementGroup.value(COM_DESKTOP);
    if(tmpDesktopElement.isNull())
        return false;
    QRectF rect = m_UiManager->GetComRect(m_UiLayout, tmpDesktopElement, m_UiDirection);
    setSceneRect(rect);
    //>@固定住VIEW的大小
    if(!m_ExtraClass && !m_LogicThemeManager->m_UiContainer->m_MainForm.isNull())
    {
        m_LogicThemeManager->m_UiContainer->m_MainForm->setFixedSize(rect.size().toSize());
    }

    //>@加载控件
    if(AddComponent(COM_DESKTOP, tmpDesktopElement))
    {
        IDE_TRACE();
        QString tmpMessage;
        for(int i=0;i<count;i++)
        {
            COM_TYPE tmpType = tmpComTypeList.at(i);
            COM_TYPE tmpParType = getParentComType(tmpType);
            QDomElement tmpParElement = m_UiManager->m_ComElementGroup.value(tmpParType);
            IDE_TRACE_INT(tmpParType);
            if(!AddComponent(tmpParType, tmpParElement))
            {
                if(!tmpParElement.isNull())
                    emit sProgress(QString("Load:[%1] error").arg(tmpParElement.tagName()), initValue+diff*i);
                continue;
            }
            IDE_TRACE_INT(tmpType);
            QDomElement tmpElement = m_UiManager->m_ComElementGroup.value(tmpType);
            if(tmpElement.isNull())
                continue;
            IDE_TRACE();
            if(!AddComponent(tmpType, tmpElement))
                tmpMessage = QString("Load:[%1] error").arg(tmpElement.tagName());
            else
                tmpMessage = QString("Load:[%1] success").arg(tmpElement.tagName());
            emit sProgress(tmpMessage, initValue+diff*i);
            IDE_TRACE();
            QtSleep(10);
        }
    }
    IDE_TRACE();
    emit sProgress(QString("Theme Load Complete!"), 100);//进度条

    QtSleep(100);
    //>@显示Theme
#ifdef DISPLAY
    m_LogicThemeManager->ShowTheme();
#endif
    QtSleep(100);
    IDE_TRACE();
    return true;
}

bool UiScene::OperateUI(AREA_OPERATE pOperate, QList<COM_TYPE> pExceptNodes)
{
    QList<COM_TYPE> tmpComList = m_ComDiagramGroup.keys();
    for(int i=0;i<tmpComList.count();i++)
    {
        COM_TYPE tmpType = tmpComList.at(i);
        //IDE_DEBUG(QString("Type[%1] Operate[%2]").arg(tmpType).arg(pOperate));
        if(!pExceptNodes.isEmpty() && pExceptNodes.contains(tmpType))
        {
            IDE_TRACE();
            continue;
        }
        GraphicsOperate *tmpCom = m_ComDiagramGroup.value(tmpType);
        if(!tmpCom)
        {
            IDE_TRACE();
            continue;
        }
        tmpCom->updateEffect(pOperate, QVariant());
    }
    return true;
}

void UiScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
}

void UiScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QList<QGraphicsItem *> SelectItems = items(event->scenePos());
    if(SelectItems.count()>0)
    {
        for(int i=0;i<SelectItems.count();i++)
        {
            GraphicsOperate *tmp = qgraphicsitem_cast<GraphicsOperate*>(SelectItems.at(i));
            if(tmp)  //>@是否为QGraphicsWidget的type
            {
                tmp->MoveEvent(event);
                break;
            }
        }
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void UiScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QList<QGraphicsItem *> SelectItems = items(event->scenePos());
    if(SelectItems.count()>0)
    {
        for(int i=0;i<SelectItems.count();i++)
        {
            GraphicsOperate *tmp = qgraphicsitem_cast<GraphicsOperate*>(SelectItems.at(i));
            if(tmp)  //>@是否为QGraphicsWidget的type
            {
                tmp->ReleaseEvent(event);
                break;
            }
        }
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void UiScene::SetGeometory(QRectF pRect)
{
    if(m_MainForm && !m_ExtraClass)
    {
        m_MainForm->setGeometry(pRect.toRect());
    }
    else
    {
        m_ViewGeometery = pRect;
    }
}

QRectF UiScene::GetGeometory()
{
    if(m_MainForm && !m_ExtraClass)
        return QRectF(m_MainForm->geometry());
    return m_ViewGeometery;
}

bool UiScene::AddComponent(COM_TYPE pComType, QDomElement &pComElement, QPointF pScenePos)
{
    if(pComType < COM_DESKTOP)
        return false;
    //>@如果存在此控件，则不再重复创建
    if(m_ComDiagramGroup.contains(pComType))
        return true;
    if(pComElement.isNull())
    {
        IDE_TRACE();
        return false;
    }
    //>@如果需要父控件,则需要查看当前场景是否存在父控件，如果没有则退出
    GraphicsOperate *tmpParComPtr = 0;
    COM_TYPE tmpParComType = getParentComType(pComType);
    if(tmpParComType >= COM_DESKTOP)
    {
        tmpParComPtr = m_ComDiagramGroup.value(tmpParComType);
        if(!tmpParComPtr)
            return false;
    }
    GraphicsOperate *tmpComPtr = 0;
    switch(pComType)
    {
        case COM_DESKTOP:
            m_Desktop = new GraphicsDesktop(tmpParComPtr);
            tmpComPtr = m_Desktop;
            addItem(tmpComPtr);
            tmpComPtr->setZValue(0);
            QObject::connect(m_LogicThemeManager->m_EventMap, SIGNAL(sDesktopAction(AREA_OPERATE,QVariant)),
                             m_Desktop, SLOT(updateEffect(AREA_OPERATE, QVariant)));
            break;
        case COM_MESSAGEBAR:
            m_MessageBar = new GraphicsMessage(tmpParComPtr);
            tmpComPtr = m_MessageBar;
            m_MessageBar->setZValue(D_MSGCTRL_ZVALUE);
            connect(this, SIGNAL(sInfo(QString, int, E_DEVMODE)), m_MessageBar, SLOT(info(QString, int, E_DEVMODE)));
            connect(this, SIGNAL(sWarn(QString, int, E_DEVMODE)), m_MessageBar, SLOT(warn(QString, int, E_DEVMODE)));
            connect(this, SIGNAL(sError(QString, int, E_DEVMODE)), m_MessageBar, SLOT(error(QString, int, E_DEVMODE)));
            break;
        case COM_PROGRESSBAR:
            m_ProgressBar = new GraphicsProgress(tmpParComPtr);
            tmpComPtr = m_ProgressBar;
            m_ProgressBar->setZValue(D_PRGCTRL_ZVALUE);
            connect(this, SIGNAL(sProgress(quint32)), m_ProgressBar, SLOT(slot_Progress(quint32)), Qt::QueuedConnection);
            connect(this, SIGNAL(sProgress(quint32,quint32)), m_ProgressBar, SLOT(slot_Progress(quint32,quint32)), Qt::QueuedConnection);
            break;
        case COM_ANIMAL:
            tmpComPtr = new GraphicsAnimal(tmpParComPtr);
            tmpComPtr->setZValue(tmpParComPtr->zValue()+1);
            break;
        case COM_FLOOR:
            tmpComPtr = new GraphicsFloor(tmpParComPtr);
            tmpComPtr->setZValue(tmpParComPtr->zValue()+1);
            QObject::connect(m_LogicThemeManager->m_EventMap, SIGNAL(sFloorAction(AREA_OPERATE,QVariant)),
                             tmpComPtr, SLOT(updateEffect(AREA_OPERATE, QVariant)));
            break;
        case COM_ARROW:
            tmpComPtr = new GraphicsArrow(tmpParComPtr);
            tmpComPtr->setZValue(tmpParComPtr->zValue()+1);
            QObject::connect(m_LogicThemeManager->m_EventMap, SIGNAL(sArrowAction(AREA_OPERATE,QVariant)),
                             tmpComPtr, SLOT(updateEffect(AREA_OPERATE, QVariant)));
            break;
        case COM_FUNCTION:
            tmpComPtr = new GraphicsFunction(tmpParComPtr);
            tmpComPtr->setZValue(tmpParComPtr->zValue()+1);
            QObject::connect(m_LogicThemeManager->m_EventMap, SIGNAL(sFunctionAction(AREA_OPERATE,QVariant)),
                             tmpComPtr, SLOT(updateEffect(AREA_OPERATE, QVariant)));
            break;
#ifdef SURRPORT_WEBKIT
        case COM_WEBVIEW:
            tmpComPtr = new GraphicsWebview(tmpParComPtr);
            break;
#endif
#ifdef SURRPORT_QML
        case COM_QML:
            tmpComPtr = new GraphicsQML(tmpParComPtr);
            tmpComPtr->setZValue(tmpParComPtr->zValue()+1);
            break;
#endif
#ifdef SURRPORT_3D
        case COM_OPENGL:
            tmpComPtr = new GraphicsOpenGL(tmpParComPtr);
            tmpComPtr->setZValue(tmpParComPtr->zValue()+1);
            break;
#endif
        case COM_IMAGE:
            tmpComPtr = new GraphicsImage(tmpParComPtr);
            tmpComPtr->setZValue(tmpParComPtr->zValue()+1);
            QObject::connect(m_LogicThemeManager->m_EventMap, SIGNAL(sImageAction(AREA_OPERATE,QVariant)),
                             tmpComPtr, SLOT(updateEffect(AREA_OPERATE, QVariant)));
            break;
        case COM_MEDIA:
            tmpComPtr = new GraphicsMedia(tmpParComPtr);
            tmpComPtr->setZValue(tmpParComPtr->zValue()+1);
            QObject::connect(m_LogicThemeManager->m_EventMap, SIGNAL(sMediaAction(AREA_OPERATE,QVariant)),
                             tmpComPtr, SLOT(updateEffect(AREA_OPERATE, QVariant)));
            break;
        case COM_TEXT:
            tmpComPtr = new GraphicsText(tmpParComPtr);
            tmpComPtr->setZValue(D_TEXT_ZVALUE);
            QObject::connect(m_LogicThemeManager->m_EventMap, SIGNAL(sTextAction(AREA_OPERATE,QVariant)),
                             tmpComPtr, SLOT(updateEffect(AREA_OPERATE, QVariant)));
            break;
        case COM_LOGO:
        {
            GraphicsLogo *tmpLogo = new GraphicsLogo(tmpParComPtr);
            tmpComPtr = tmpLogo;
            tmpComPtr->setZValue(tmpParComPtr->zValue()+1);
            QObject::connect(m_LogicThemeManager->m_EventMap, SIGNAL(sLogoAction(AREA_OPERATE,QVariant)),
                             tmpComPtr, SLOT(updateEffect(AREA_OPERATE, QVariant)));
            break;
        }
        case COM_WEATHER:
            tmpComPtr = new GraphicsWeather(tmpParComPtr);
            tmpComPtr->setZValue(tmpParComPtr->zValue()+1);
            QObject::connect(m_LogicThemeManager->m_EventMap, SIGNAL(sWeatherAction(AREA_OPERATE,QVariant)),
                             tmpComPtr, SLOT(updateEffect(AREA_OPERATE, QVariant)));
            break;
        case COM_CLOCK:
            tmpComPtr = new GraphicsClock(tmpParComPtr);
            tmpComPtr->setZValue(tmpParComPtr->zValue()+1);
            QObject::connect(m_LogicThemeManager->m_EventMap, SIGNAL(sClockAction(AREA_OPERATE,QVariant)),
                             tmpComPtr, SLOT(updateEffect(AREA_OPERATE, QVariant)));
            break;
        case COM_NAMEPLATE:
            tmpComPtr = new GraphicsNamePlate(tmpParComPtr);
            tmpComPtr->setZValue(tmpParComPtr->zValue()+1);
            break;
        case COM_FLRBTN:
            tmpComPtr = new GraphicsFloorButton(tmpParComPtr);
            tmpComPtr->setZValue(tmpParComPtr->zValue()+1);
            break;
        case COM_KEYBOARDBTN:
            tmpComPtr = new GraphicsKeyboard(tmpParComPtr);
            tmpComPtr->setZValue(tmpParComPtr->zValue()+1);
            break;
        case COM_CALLBTN:
            break;
        case COM_PHONEBTN:
        case COM_EMERGENCYBTN:
        case COM_TALKBACKBTN:
        case COM_OPENBTN:
        case COM_CLOSEBTN:
        case COM_DISABILITYBTN:
            tmpComPtr = new GraphicsButton(tmpParComPtr);
            tmpComPtr->setZValue(tmpParComPtr->zValue()+1);
            break;
        default:
            return false;
    }
    if(tmpComPtr == 0)
        return false;
    if(tmpParComPtr == 0)
        tmpComPtr->setZValue(0);
    else
        tmpComPtr->setZValue(tmpParComPtr->zValue()+2);
    connect(tmpComPtr, SIGNAL(sInfo(QString,int,E_DEVMODE)), this, SIGNAL(sInfo(QString,int,E_DEVMODE)), Qt::AutoConnection);
    connect(tmpComPtr, SIGNAL(sWarn(QString,int,E_DEVMODE)), this, SIGNAL(sWarn(QString,int,E_DEVMODE)), Qt::AutoConnection);
    connect(tmpComPtr, SIGNAL(sError(QString,int,E_DEVMODE)), this, SIGNAL(sError(QString,int,E_DEVMODE)), Qt::AutoConnection);
    connect(tmpComPtr, SIGNAL(sProgress(quint32)), this, SIGNAL(sProgress(quint32)), Qt::AutoConnection);
    tmpComPtr->InitScene(this);
    tmpComPtr->InitComponent(pComElement, (pComType==COM_DESKTOP)?(true):(false));

    QString tmpEnvirString;
    if(m_IniParse && m_IniParse->getProperty(tmpComPtr->m_ComElement.tagName(), tmpEnvirString))
        tmpComPtr->SetEnvironment(tmpEnvirString);

    m_ComDiagramGroup.insert(pComType, tmpComPtr);
    return true;
}

bool UiScene::RemoveComponent(COM_TYPE pComType)
{
    GraphicsOperate* tmpComItem = m_ComDiagramGroup.take(pComType);
    if(tmpComItem)
    {
        QString tmpEnvirString;
        if(m_IniParse && tmpComItem->GetEnvironment(tmpEnvirString))
            m_IniParse->setProperty(tmpComItem->m_ComElement.tagName(), tmpEnvirString);
        removeItem(tmpComItem);
        tmpComItem->deleteLater();
    }
    return true;
}

