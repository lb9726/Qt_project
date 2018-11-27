#include "globalsetting.h"

#include "component/physic/playerph.h"
#include "component/physic/ledph.h"
#include "component/physic/lcdph.h"
#include "component/physic/lancph.h"
#include "component/physic/comph.h"

#include "component/ui/arrowui.h"
#include "component/ui/desktopui.h"
#include "component/ui/messagebarui.h"
#include "component/ui/progressbarui.h"
#include "component/ui/floorui.h"
#include "component/ui/functionui.h"
#include "component/ui/mediaui.h"
#include "component/ui/fulltextui.h"
#include "component/ui/logoui.h"
#include "component/ui/realtimetextui.h"
#include "component/ui/wifilogo.h"
#include "component/ui/usblogo.h"



#include "component/logic/themelg.h"

#include <QDebug>
GlobalSetting *appGlobalSetting;

GlobalSetting * GlobalSetting::getGlobalSetting(){
    return appGlobalSetting;
}

GlobalSetting::GlobalSetting(QObject *parent):QObject(parent)
{
    m_curDirection = HVNone;
    m_curLayout = -1;
    m_themeParser =NULL;
    m_appAbsPath = "";
    m_declarativeView = NULL;
    m_declarativeEngine = NULL;
    m_curRotate = ROTATE_INVALID;
    //m_bootSplash = new BootSplash();
}

GlobalSetting::~GlobalSetting(){
    //clearData();
}

void GlobalSetting::clearData()
{
    QList<COM_TYPE> tmpComList = m_ComComponentGroup.keys();
    for(int i=0;i<tmpComList.count();i++)
    {
        UiBase* tmp = m_ComComponentGroup.value(tmpComList.at(i));
        if(tmp)
            tmp->deleteLater();
    }
    QList<PH_Type> tmpPHList = m_PHDevList.keys();
    for(int i=0;i<tmpPHList.count();i++)
    {
        PhysicBase* tmp = m_PHDevList.value(tmpPHList.at(i));
        if(tmp)
            tmp->deleteLater();
    }

    QList<LG_Type> tmpLGList = m_LGDevList.keys();
    for(int i=0;i<tmpLGList.count();i++)
    {
        LogicBase* tmp = m_LGDevList.value(tmpLGList.at(i));
        if(tmp)
            tmp->deleteLater();
    }

    m_themeParser->deleteLater();
    m_mdpParser->deleteLater();
}

void GlobalSetting::setQuickView(QtQuick1ApplicationViewer* declarativeView){
    m_declarativeView = declarativeView;
    m_declarativeEngine = declarativeView->engine();
    m_rootContent = m_declarativeEngine->rootContext();
    m_declarativeView->addImportPath(QLatin1String("modules"));
}

void GlobalSetting::getAppRunningDir()
{
    this->m_appAbsPath = QApplication::applicationDirPath();
    m_appAbsPath.replace("\\", "/");
    if(!this->m_appAbsPath.endsWith("/"))
        this->m_appAbsPath.append("/");
//    m_appAbsPath = "C:/Users/lishaoxiang.SHBST/Desktop/";
    m_appAbsPath = "/home/libo/Desktop/Repository/Qt_project/BST_Codes/SodimasDisplay/";
    qDebug()<< "before m_appAbsPath ="<< m_appAbsPath;
}

bool GlobalSetting::initApp()
{
    m_rootContent->setContextProperty("GlobalSetting", this); // 设置qml的上下文属性
    m_rootContent->setContextProperty("DeclarativeView", m_declarativeView);
    m_bootSplash = new BootSplash();
    m_rootContent->setContextProperty("BootSplash", m_bootSplash);

    getAppRunningDir();
    initEarly();
    initScreen();
    m_declarativeView->setMainQmlFile(QLatin1String("qrc:///qml/main.qml"));
    QtSleep(1000);///等待加载完成
    initLate();
    initUI();
    QtSleep(1000);///等待加载完成
    m_declarativeView->show();
}

bool GlobalSetting::initEarly(){
    m_declarativeView->setMainQmlFile(QLatin1String("qrc:///qml/bootScreen.qml"));
    m_declarativeView->show();
    QtSleep(100);
    QString ver = getAppVersion(QApplication::applicationFilePath());
    if(ver.isEmpty()){
        m_bootSplash->m_appVersion = "V -.-.-.-";
    }else{
        m_bootSplash->m_appVersion = "V " + ver;
    }
    QString tmpLogoPath = m_appAbsPath + D_BUNDLEDIR + "/" + D_LOGOFILE;

    if(QFile::exists(tmpLogoPath)){
        m_bootSplash->m_bootLogoPath = D_QMLRESSHCEL + tmpLogoPath;
    }else{
        IDE_TRACE_STR(tmpLogoPath);
        m_bootSplash->m_bootLogoPath = "";
    }

    QString tmpbgPath = m_appAbsPath + D_BUNDLEDIR + "/" + D_BOOTBGFILE;

    if(QFile::exists(tmpbgPath)){
        m_bootSplash->m_backgroundPath = D_QMLRESSHCEL + tmpbgPath;
    }else{
        IDE_TRACE_STR(tmpbgPath);
        m_bootSplash->m_backgroundPath = "";
    }

    m_bundlePath = m_appAbsPath + D_BUNDLEDIR + "/" +D_MDPFILENAME;
    m_curRotate = getRotateType(getRotate(m_bundlePath));
    qDebug()<< "m_appAbsPath ="<< m_appAbsPath;
    qDebug()<< "m_bundlePath ="<< m_bundlePath;
    if(m_curRotate == ROTATE_INVALID)
        m_curRotate = ROTATE_0;
    //IDE_TRACE_INT(m_curRotate);
    if(m_curRotate == ROTATE_0 ||m_curRotate == ROTATE_180)
    {
        m_curDirection = Horizontal;
    }

    if(m_curRotate == ROTATE_90 ||m_curRotate == ROTATE_270)
    {
        m_curDirection = Vertical;
    }

    QGraphicsObject *rootObj = m_declarativeView->rootObject();
    if (rootObj) {
         rootObj->setProperty("transformedRot", m_curRotate);
         m_declarativeView->rotateView(m_curRotate);
//        QVariant returnedValue;
//        QVariant angel = m_curRotate;
//        QMetaObject::invokeMethod(rootObj, "rotateUi",
//                                  Q_RETURN_ARG(QVariant, returnedValue),
//                                  Q_ARG(QVariant, angel));
//        m_declarativeView->rotateView(m_curRotate);
    }else{
        IDE_TRACE();
    }

    QDeclarativeItem * item = rootObj->findChild<QDeclarativeItem*>("bootlogo");
    if (item) {
        item->setProperty("source", m_bootSplash->m_bootLogoPath);
    }
    item = rootObj->findChild<QDeclarativeItem*>("bootversion");
    if (item) {
        item->setProperty("text", m_bootSplash->m_appVersion);
    }
    item = rootObj->findChild<QDeclarativeItem*>("bootbackground");
    if (item) {
        item->setProperty("source", m_bootSplash->m_backgroundPath);
    }
    QtSleep(10);
}

bool GlobalSetting::initScreen(){    
    initMdp();
    initTheme();
}
bool GlobalSetting::initMdp()
{
    //QString tmpMdpFile = m_appAbsPath + D_BUNDLEDIR + "/" +D_MDPFILENAME;
    emit sigInfoProgress(QString("Device Booting ..."), 5);
    m_mdpParser = new MdpParser(m_bundlePath);
    if(!m_mdpParser->openMdp())
    {
        return false;
    }
    //IDE_TRACE_STR(tmpMdpFile);
    float tmpPgDiff;
    //emit sigProgress(QString("Device Booting ..."), 5);
    //初始化物理设备
    emit sigInfoProgress(QString("Loading physical Node..."), 10);
    //Mdp数据初始化，进度为30%
    QList<PH_Type> tmpPHList = m_mdpParser->m_PHElementGroup.keys();
    qSort(tmpPHList.begin(), tmpPHList.end());
    tmpPgDiff = 30.0 / tmpPHList.count();
    for(int i=0;i<tmpPHList.count();i++)
    {
        PH_Type tmpType = tmpPHList.at(i);
        if(tmpType == P_INVALID)
            continue;
        QDomElement tmpElement = m_mdpParser->m_PHElementGroup.value(tmpType);
        if(tmpElement.isNull())
            continue;
        emit sigInfoProgress("", 10+tmpPgDiff*(i+1));
        addPHDevice(tmpType, tmpElement);
        QtSleep(200);
    }
    //theme数据初始化，进度为20%
    emit sigInfoProgress(QString("Loading logic Node..."), 45);
    QList<LG_Type> tmpLGList = m_mdpParser->m_LGElementGroup.keys();
    qSort(tmpLGList.begin(), tmpLGList.end());
    tmpPgDiff = 20.0 / tmpLGList.count();
    for(int i=0;i<tmpLGList.count();i++)
    {
        LG_Type tmpType = tmpLGList.at(i);
        if(tmpType == L_INVALID)
            continue;
        QDomElement tmpElement = m_mdpParser->m_LGElementGroup.value(tmpType);
        if(tmpElement.isNull())
            continue;
        addLGDevice(tmpType, tmpElement);
        QtSleep(800);//
        //IDE_TRACE_INT(45+tmpPgDiff*i);
        emit sigInfoProgress("",45+tmpPgDiff*(i+1));
    }
    return true;
}

bool GlobalSetting::initTheme()
{
    m_themeLg  =  (ThemeLg *) m_LGDevList.value(L_ThemeManager);
    if(m_themeLg)
        m_curThemeName = m_themeLg->m_ThemeName;
    else
        m_curThemeName = "";
    emit sigInfoProgress(QString("Init theme resource..."), 70);
    QString themePath = m_appAbsPath + D_THEMEDIR+"/";
    themePath.append(m_curThemeName+"/");
    UiBase::themeResourceDir = themePath;
    UiBase::bundleResourceDir = m_appAbsPath;
    m_themeParser = new ThemeParser(themePath + "theme.xml");
    if(m_themeParser->openUi()){
        QList<COM_TYPE> tmpComTypeList = m_themeParser->m_ComElementGroup.keys();
        int count = tmpComTypeList.count();
        if(count < 0)
        {
            emit sigError("No ui element in theme.xml.");
            return false;
        }

        QString tmpMessage;
        qreal tmpPgDiff = 20.0 / count;
        for(int i=0; i<count; i++)
        {
            COM_TYPE tmpType = tmpComTypeList.at(i);

            QDomElement tmpElement = m_themeParser->m_ComElementGroup.value(tmpType);
            if(tmpElement.isNull())
                continue;
            if(!addComponent(tmpType, tmpElement)){
                tmpMessage = QString("Load:[%1] error").arg(tmpElement.tagName());
                emit sigError(tmpMessage);
            }
            QtSleep(500);//
            emit sigInfoProgress("",70+tmpPgDiff*i);
        }
        m_ComComponentGroup.insert(COM_USBLOGO, new UsbLogo());
        m_ComComponentGroup.insert(COM_WIFILOGO, new WifiLogo());

    }else{
        emit sigError("Parser theme.xml failed.");
        return false;
    }
    return true;
}

bool GlobalSetting::initLate()
{
    m_RootObj = m_declarativeView->rootObject();
    if (m_RootObj) {
        initQmlItemGroup();
        m_RootObj->setProperty("transformedRot", m_curRotate);
        m_declarativeView->rotateView(m_curRotate);
        //        QVariant returnedValue;
        //        QVariant angel = m_curRotate;
        //        QMetaObject::invokeMethod(m_RootObj, "rotateUi",
        //                                  Q_RETURN_ARG(QVariant, returnedValue),
        //                                  Q_ARG(QVariant, angel));
    }else{
        IDE_TRACE();
    }
}
void GlobalSetting::initQmlItemGroup(){
    UiBase* comUi;
    m_QmlItemGroup.clear();
    QDeclarativeItem * item = m_RootObj->findChild<QDeclarativeItem*>("usbLogoObj");
    if (item) {
       // IDE_TRACE();
        m_QmlItemGroup.insert("usbLogo", item);
        comUi = m_ComComponentGroup.value(COM_USBLOGO);
        if(comUi){
            comUi->m_QmlItem = item;
        }
    }
    item = m_RootObj->findChild<QDeclarativeItem*>("wifiLogoObj");
    if (item) {
        //IDE_TRACE();
        m_QmlItemGroup.insert("wifiLogo", item);
        comUi = m_ComComponentGroup.value(COM_WIFILOGO);
        if(comUi){
            comUi->m_QmlItem = item;
        }
    }
    item = m_RootObj->findChild<QDeclarativeItem*>("arrowObj");
    if (item) {
        m_QmlItemGroup.insert("arrow", item);
        comUi = m_ComComponentGroup.value(COM_ARROW);
        if(comUi){
            comUi->m_QmlItem = item;
        }
    }
    item = m_RootObj->findChild<QDeclarativeItem*>("functionObj");
    if (item) {
        m_QmlItemGroup.insert("function", item);
        comUi = m_ComComponentGroup.value(COM_FUNCTION);
        if(comUi){
            comUi->m_QmlItem = item;
        }
    }
    item = m_RootObj->findChild<QDeclarativeItem*>("floorObj");
    if (item) {
        m_QmlItemGroup.insert("floor", item);
        comUi = m_ComComponentGroup.value(COM_FLOOR);
        if(comUi){
            comUi->m_QmlItem = item;
        }
    }
    item = m_RootObj->findChild<QDeclarativeItem*>("fulltextObj");
    if (item) {
        m_QmlItemGroup.insert("fulltext", item);
        comUi = m_ComComponentGroup.value(COM_FULLTEXT);
        if(comUi){
            comUi->m_QmlItem = item;
        }
    }
    item = m_RootObj->findChild<QDeclarativeItem*>("realtimetextObj");
    if (item) {
        m_QmlItemGroup.insert("realtimetext", item);
        comUi = m_ComComponentGroup.value(COM_REALTIMETEXT);
        if(comUi){
            comUi->m_QmlItem = item;
        }
    }
    item = m_RootObj->findChild<QDeclarativeItem*>("mesagebarObj");
    if (item) {
        m_QmlItemGroup.insert("mesagebar", item);
        comUi = m_ComComponentGroup.value(COM_MESSAGEBAR);
        if(comUi){
            comUi->m_QmlItem = item;
        }
    }
    item = m_RootObj->findChild<QDeclarativeItem*>("progressbarObj");
    if (item) {
        m_QmlItemGroup.insert("progressbar", item);
        comUi = m_ComComponentGroup.value(COM_PROGRESSBAR);
        if(comUi){
            comUi->m_QmlItem = item;
        }
    }
    item = m_RootObj->findChild<QDeclarativeItem*>("desktopObj");
    if (item) {
        m_QmlItemGroup.insert("desktop", item);
        comUi = m_ComComponentGroup.value(COM_DESKTOP);
        if(comUi){
            comUi->m_QmlItem = item;
        }
    }
}

void GlobalSetting::initUI(){
    QList<COM_TYPE> tmpComTypeList = m_ComComponentGroup.keys();
    int count = tmpComTypeList.count();
    if(count < 0)
    {
        emit sigError("No ui element.");
        IDE_TRACE();
        return;
    }
    UiBase* uiItem;
    for(int i=0;i<count;i++)
    {
        COM_TYPE tmpType = tmpComTypeList.at(i);
        uiItem = m_ComComponentGroup.value(tmpType);
        if(uiItem){
            uiItem->initGeometry(m_curDirection,m_themeLg->m_Layout);
            uiItem->initUi(m_curDirection);
        }
    }
}

int GlobalSetting::getRotate(QString mdpPath)
{
    if(!QFile::exists(mdpPath)){
        IDE_TRACE();
        return 0;
    }
    XmlParse xmlParse(mdpPath);
    xmlParse.openXml();
    xmlParse.getItemElement("/configure/physical/lcd/rotation");
    if(!xmlParse.itemElement.isNull())
    {
        QString rotate = xmlParse.getItemElementValue();
        int angle = rotate.toInt(NULL);
        return angle;
    }else{
        IDE_TRACE();
        return 0;
    }
}

int GlobalSetting::qmlSlotTest()
{
    IDE_TRACE();
}

bool GlobalSetting::addComponent(COM_TYPE pComType, QDomElement &pComElement)
{
    if(pComType < COM_DESKTOP)
        return false;

    if(m_ComComponentGroup.contains(pComType))
        return true;
    if(pComElement.isNull())
    {
        //IDE_TRACE();
        IDE_TRACE_STR(getComponetName(pComType));
        return false;
    }
    ///如果需要父控件,则需要查看当前场景是否存在父控件，如果没有则退出
    UiBase *tmpParComPtr = 0;
    COM_TYPE tmpParComType = getParentComType(pComType);
    if(tmpParComType >= COM_DESKTOP)
    {
        tmpParComPtr = m_ComComponentGroup.value(tmpParComType);
        if(!tmpParComPtr)
            return false;
    }
    UiBase *tmpComPtr = 0;
    switch(pComType)
    {
    case COM_DESKTOP:
        m_Desktop = new DesktopUi();
        tmpComPtr = m_Desktop;
        break;
    case COM_MESSAGEBAR:
        m_MessageBar = new MessageBarUi();
        tmpComPtr = m_MessageBar;
        break;
    case COM_PROGRESSBAR:
        m_ProgressBar = new ProgressBarUi();
        tmpComPtr = m_ProgressBar;
        break;
    case COM_FLOOR:
        tmpComPtr = new FloorUi();
        break;
    case COM_REALTIMETEXT:
        tmpComPtr = new RealtimeTextUi();
        break;
    case COM_ARROW:
        tmpComPtr = new ArrowUi();
        break;
    case COM_FUNCTION:
        tmpComPtr = new FunctionUi();
        break;
    case COM_MEDIA:
        tmpComPtr = new MediaUi();
        break;
    case COM_FULLTEXT:
        tmpComPtr = new FullTextUi();
        break;
    case COM_LOGO:
    {
        LogoUi *tmpLogo = new LogoUi();
        tmpComPtr = tmpLogo;
        break;
    }
    default:
        return false;
    }
    tmpComPtr->setThemeManager(m_themeParser);
    tmpComPtr->initComponent(pComElement);

    //IDE_TRACE_STR(getComponetName(pComType));
    m_ComComponentGroup.insert(pComType, tmpComPtr);
    return true;
}

bool GlobalSetting::addPHDevice(PH_Type pPHType, QDomElement pPHTypeElement)
{
    if(pPHType == P_INVALID || pPHTypeElement.isNull())
        return false;
    //查询此物理设备是否已经存在
    if(m_PHDevList.contains(pPHType))
    {
        delPHDevice(pPHType);
    }
    PhysicBase *tmpPHDev = 0;
    switch(pPHType)
    {
    case P_LCD:
        tmpPHDev = new LcdPh(this);
        break;
    case P_LED:
        tmpPHDev = new LedPh(this);
        break;
    case P_LANC:
        tmpPHDev = new LancPh(this);
        break;
    case P_Player:
        tmpPHDev = new PlayerPh(this);
        break;
    case P_COM:
        tmpPHDev = new ComPh(this);
        break;
    default:return false;
    }
    if(!tmpPHDev)
    {
        IDE_TRACE_INT(pPHType);
        return false;
    }
    m_PHDevList.insert(pPHType, tmpPHDev);
    tmpPHDev->initDevice(pPHType, pPHTypeElement);
    return true;
}

bool GlobalSetting::delPHDevice(PH_Type pPHType)
{
    PhysicBase *tmpPHDev = m_PHDevList.take(pPHType);
    if(tmpPHDev == 0)
        return true;
    tmpPHDev->deleteLater();
    return true;
}

bool GlobalSetting::addLGDevice(LG_Type pLGType, QDomElement pLGTypeElement)
{
    if(pLGType == L_INVALID || pLGTypeElement.isNull())
        return false;
    ///查询此物理设备是否已经存在
    if(m_LGDevList.contains(pLGType))
    {
        delLGDevice(pLGType);
    }
    ///
    LogicBase *tmpLGDev = 0;
    switch(pLGType)
    {
    case L_ThemeManager:
    {
        ThemeLg *themeLg = new ThemeLg(m_themeParser);
        tmpLGDev = themeLg;
    }
        break;
    default:
        return false;
    }
    if(!tmpLGDev)
        return false;
    m_LGDevList.insert(pLGType, tmpLGDev);
    tmpLGDev->initDevice(pLGType, pLGTypeElement);
    return true;
}

bool GlobalSetting::delLGDevice(LG_Type pLGType)
{
    LogicBase* tmpLGDev = m_LGDevList.take(pLGType);
    if(tmpLGDev == 0)
        return true;
    ///删除物理设备
    tmpLGDev->deleteLater();
    return true;
}

PhysicBase* GlobalSetting::getPHDev(PH_Type pPHType)
{
    return m_PHDevList.value(pPHType);
}

LogicBase* GlobalSetting::getLGDev(LG_Type pLGType)
{
    return m_LGDevList.value(pLGType);
}

QString GlobalSetting::getAppVersion(QString pFile)
{
    int tmpVersionPos[2] = {-1, -1};
    QFile tmpFile;
    tmpFile.setFileName(pFile);
    if(!tmpFile.open(QIODevice::ReadOnly))
    {
        IDE_TRACE();
        return QString("-.-.-.-");
    }
    if(tmpFile.size() < 1024)
    {
        tmpFile.close();
        IDE_TRACE();
        return QString("-.-.-.-");
    }
    int index = tmpFile.size()-100;
    tmpFile.seek(index);
    QByteArray tmpExtend = tmpFile.read(1024);
    tmpFile.close();
    int index1 = tmpExtend.indexOf("versison:", 0); //versison:1.0.0.1;
    if(index1 < 0)
    {
        IDE_TRACE();
        return QString("-.-.-.-");
    }
    index1 += 8;
    tmpVersionPos[0] = index + index1;
    QString tmpVersion;
    int index2 = tmpExtend.indexOf(";", index1);
    if(index2 < 0)
    {
        tmpVersionPos[1] = -1;
        tmpVersion = tmpExtend.mid(index1);
    }
    else
    {
        tmpVersionPos[1] = index + index2;
        tmpVersion = tmpExtend.mid(index2-index1);
    }
    return tmpVersion;
}
