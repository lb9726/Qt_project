#include <QDateTime>
#include <QtCore/qmath.h>

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

#include "button/buttoncontroller.h"
#include "button/iddailer.h"
#include "button/backlightctrl.h"
#include "wifi/appclient.h"


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
    m_RealtimeTextLine1 = "";
    m_RealtimeTextLine2 = "";
    m_FullScreenTextLine1 = "";
    m_FullScreenTextLine2 = "";
    m_FullScreenTextLine3 = "";
    m_FullScreenTextLine4 = "";
    m_showFloorOrLogo =false;//
    m_beepIndex = -1;
    m_beeped = false;
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
    ////this->m_appAbsPath = "/home/lisx/Desktop";
    this->m_appAbsPath = QApplication::applicationDirPath();
    m_appAbsPath.replace("\\", "/");
    if(!this->m_appAbsPath.endsWith("/"))
        this->m_appAbsPath.append("/");
}

bool GlobalSetting::initApp()
{
    m_rootContent->setContextProperty("GlobalSetting",this);
    m_rootContent->setContextProperty("DeclarativeView",m_declarativeView);
    m_bootSplash = new BootSplash();

    m_declarativeView->setMainQmlFile(QLatin1String("qrc:///qml/main.qml"));
    m_declarativeView->show();

    getAppRunningDir();
    initEarly();
    initScreen();
    updateInfoProgess(QString("Init theme resource ..."), 99);
    QtSleep(500);///等待加载完成
    initLate();
    initUI();
    updateInfoProgess(QString("Init firmware  ..."), 101);
    QtSleep(500);///等待加载完成
    clearInfoBuffer();
    updateInfoProgess("Init CANOPEN ...",-1);

    initCanopen();
    initButtonController();///Must called 'After' Canopen Init.
    startDevice();
}

bool GlobalSetting::initEarly(){
    QtSleep(100);
    //    QString ver = getAppVersion(QApplication::applicationFilePath());
    //    if(ver.isEmpty()){
    m_bootSplash->m_appVersion = "V 1.1.0.0";
    //    }else{
    //        m_bootSplash->m_appVersion = "V " + ver;
    //    }
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

    m_RootObj = m_declarativeView->rootObject();
    if (m_RootObj) {
        rotateView();
    }else{
        IDE_TRACE();
    }

    QDeclarativeItem * item = m_RootObj->findChild<QDeclarativeItem*>("bootlogo");
    if (item) {
        item->setProperty("source", m_bootSplash->m_bootLogoPath);
    }
    item = m_RootObj->findChild<QDeclarativeItem*>("bootversion");
    if (item) {
        item->setProperty("text", m_bootSplash->m_appVersion);
    }
    item = m_RootObj->findChild<QDeclarativeItem*>("bootbackground");
    if (item) {
        item->setProperty("source", m_bootSplash->m_backgroundPath);
    }

    QtSleep(10);
}
void GlobalSetting::rotateView(){
    if(m_curRotate == ROTATE_0)
        m_RootObj->setProperty("transformedRot", QVariant(0));
    if(m_curRotate == ROTATE_90)
        m_RootObj->setProperty("transformedRot", QVariant(90));
    if(m_curRotate == ROTATE_180)
        m_RootObj->setProperty("transformedRot", QVariant(180));
    if(m_curRotate == ROTATE_270)
        m_RootObj->setProperty("transformedRot", QVariant(270));
    m_declarativeView->rotateView(m_curRotate);
    IDE_TRACE();
}

void GlobalSetting::rotateDesktop(int angle){
    ////m_declarativeView->rotateView(angle);
}

bool GlobalSetting::initScreen(){    
    initMdp();
    initTheme();
}
bool GlobalSetting::initMdp()
{
    //QString tmpMdpFile = m_appAbsPath + D_BUNDLEDIR + "/" +D_MDPFILENAME;
    updateInfoProgess(QString("Device Booting ..."), 5);
    m_mdpParser = new MdpParser(m_bundlePath);
    if(!m_mdpParser->openMdp())
    {
        return false;
    }
    //IDE_TRACE_STR(tmpMdpFile);
    float tmpPgDiff;
    //emit sigProgress(QString("Device Booting ..."), 5);
    //初始化物理设备
    updateInfoProgess(QString("Loading physical Node..."), 10);
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
        updateInfoProgess("",10+tmpPgDiff*(i+1));
        addPHDevice(tmpType, tmpElement);
        QtSleep(200);
    }
    //theme数据初始化，进度为20%
    updateInfoProgess(QString("Loading logic Node..."), 45);
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
        updateInfoProgess("",45+tmpPgDiff*(i+1));
    }
    return true;
}

bool GlobalSetting::initTheme()
{
    m_themeLg  =  (ThemeLg *) m_LGDevList.value(L_ThemeManager);
    m_curLayout = m_themeLg->m_Layout;
    if(m_themeLg)
        m_curThemeName = m_themeLg->m_ThemeName;
    else
        m_curThemeName = "";
    updateInfoProgess(QString("Init theme resource..."), 70);
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
        for(int i=0;i<count;i++)
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
            updateInfoProgess("",70+tmpPgDiff*i);
        }
        m_ComComponentGroup.insert(COM_USBLOGO,new UsbLogo());
        m_ComComponentGroup.insert(COM_WIFILOGO,new WifiLogo());
    }else{
        emit sigError("Parser theme.xml failed.");
        return false;
    }
    return true;
}
bool GlobalSetting::initLate()
{
    if (m_RootObj) {
        initQmlItemGroup();
    }else{
        IDE_TRACE();
    }
}
void GlobalSetting::initQmlItemGroup(){
    UiBase* comUi;
    m_QmlItemGroup.clear();

    QDeclarativeItem * item = m_RootObj->findChild<QDeclarativeItem*>("desktopObj");
    if (item) {
        m_QmlItemGroup.insert("desktop", item);
        comUi = m_ComComponentGroup.value(COM_DESKTOP);
        if(comUi){
            comUi->m_QmlItem = item;
        }
    }

    item = m_RootObj->findChild<QDeclarativeItem*>("usbLogoObj");
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
        //IDE_TRACE();
        m_QmlItemGroup.insert("fulltext", item);
        comUi = m_ComComponentGroup.value(COM_FULLTEXT);
        if(comUi){
            comUi->m_QmlItem = item;
        }
    }else{
        IDE_TRACE();
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

void GlobalSetting::initButtonController(){
    m_buttoncontroller = new ButtonController;

    connect(m_buttoncontroller,SIGNAL(sigBoardButtonPressed(int)),m_canopenClient,SLOT(reportButtonPressed(int)));
    connect(m_buttoncontroller,SIGNAL(sigBoardButtonReleased(int)),m_canopenClient,SLOT(reportButtonReleased(int)));
    connect(m_buttoncontroller,SIGNAL(sigButtonBeep()),this,SLOT(sltBuzz()));

    connect(m_canopenClient,SIGNAL(sigButtonLightOff(int)),m_buttoncontroller,SLOT(sltLightOffBoardbutton(int)));
    connect(m_canopenClient,SIGNAL(sigButtonLightOn(int,bool)),m_buttoncontroller,SLOT(sltLightOnBoardbutton(int,bool)));
    m_buttoncontroller->init();
}

/**********************CANOPEN FUNCTIONS ******************************/
void GlobalSetting::initCanopen(){
    m_customLogo = new CustomLogo();
#ifdef CANOPEN
    m_canopenThread = new QThread;
    m_canopenClient = new OpenClient();
    m_canopenClient->moveToThread(m_canopenThread);
    connect(m_canopenClient,SIGNAL(sigNodeState(int)),this,SLOT(nodeStateChanged(int)));
    connect(m_canopenClient,SIGNAL(sigFCT_AFF(bool)),this,SLOT(fct_affChanged(bool)));
    connect(m_canopenClient,SIGNAL(sigArrowAction(bool,bool,bool)),this,SLOT(arrowAction(bool,bool,bool)));

    connect(m_canopenClient,SIGNAL(sigMainVolumeChange(int,int)),this,SLOT(mainVolumeChange(int,int)));
    connect(m_canopenClient,SIGNAL(sigMusicVolumeChange(int)),this,SLOT(musicVolumeChange(int)));
    //connect(m_canopenClient,SIGNAL(sigBuzz(int)),this,SLOT(sltBuzz(int)));
    connect(m_canopenClient,SIGNAL(sigBuzz()),this,SLOT(sltBuzz()));
    connect(m_canopenClient,SIGNAL(sigButtonLightOff(int)),this,SLOT(buttonLightOff(int)));

    connect(m_canopenClient,SIGNAL(sigFloorArrive(int,int,int,int)),this,SLOT(floorArrive(int,int,int,int)));
    connect(m_canopenClient,SIGNAL(sigShowFuncLogo(int,int)),this,SLOT(showFuncLogo(int,int)));
    connect(m_canopenClient,SIGNAL(sigShowFloorOrLogo(bool)),this,SLOT(showFloorOrLogo(bool)));
    //pdo3
    connect(m_canopenClient,SIGNAL(sigMessage(QString,QString,QString,QString)),this,SLOT(textMessage(QString,QString,QString,QString)));
    connect(m_canopenClient,SIGNAL(sigRealMessage(QString,QString)),this,SLOT(realtimeMessage(QString,QString)));
    //sdo1
    connect(m_canopenClient,SIGNAL(sigOrientation(int)),this,SLOT(sltOrientation(int)));
    connect(m_canopenClient,SIGNAL(sigContraste(int)),this,SLOT(sltContraste(int)));
    connect(m_canopenClient,SIGNAL(sigBacklight(int)),this,SLOT(sltBacklight(int)));
    connect(m_canopenClient,SIGNAL(sigVit_scrol(int)),this,SLOT(sltVitscrol(int)));

#endif    
}

void GlobalSetting::startDevice(){
    QString backgroundMusic = m_appAbsPath + D_THEMEDIR+"/"+m_curThemeName+"/music/M0.mp3";
    m_Player->starWork(backgroundMusic);

    m_buttoncontroller->startWork();
#ifdef CANOPEN
    IdDailer idDailer;
    emit sigInfo( idDailer.getDailerStateString());
    quint8 nodeId = idDailer.getNodeId();
    if(nodeId){
        m_canopenClient->initCanopenClient(nodeId,idDailer.getBoutrate(),"0");
    }
    m_canopenThread->start();//QThread::HighPriority
#endif
    initWifi();
    emit sigInfo("enter [Initialisation] mode");
}

///NMT
void GlobalSetting::nodeStateChanged(int state){
    switch (state) {
    case 1:
        emit sigInfo("enter [Initialisation] mode");
        m_RootObj->setProperty("inOperational", false);
        break;
    case 2:
        emit sigInfo("enter [PreOperational] mode");
        m_RootObj->setProperty("inOperational", false);
        break;
    case 3:
        emit sigInfo("enter [Operational] mode");
        m_RootObj->setProperty("inOperational", true);
        break;
    case 4:
        emit sigInfo("enter [Stopped] mode");
        m_RootObj->setProperty("inOperational", false);
        break;
    default:
        break;
    }
}
///RPDO1
void GlobalSetting::fct_affChanged(bool isFCT){    
    m_RootObj->setProperty("isFCT_AFF", isFCT);
}

void GlobalSetting::arrowAction(bool arrows_FLM,bool arrows_FLD,bool arrows_SCROL){
    ArrowUi* arrow = (ArrowUi*)m_ComComponentGroup.value(COM_ARROW);

    int direction = 0;
    if(arrows_FLM && arrows_FLD){
        direction = 3;
    }else if(!arrows_FLM && arrows_FLD){
        direction = 2;
    }else if(arrows_FLM && !arrows_FLD){
        direction = 1;
    }else{

    }

    //    if(direction){
    //        m_Lcd->standbyFilter(true);
    //    }else{
    //        m_Lcd->standbyFilter(false);
    //    }

    arrow->arrowCtrl(direction,arrows_SCROL);
}

void GlobalSetting::mainVolumeChange(int divGong,int divBip){
    if(divGong >=0 && divGong <=8){
        qreal vo = (divGong/8.00)*100.00;
        m_Player->setVolume(AO_LIFTFLR,qFloor(vo));
        m_Player->setVolume(AO_LIFTARV,qFloor(vo));
    }

    if(divBip >=0 && divBip <=8){
        qreal vo = (divBip/8.00)*100.00;
        m_Player->setVolume(AO_BEEP,qFloor(vo));
    }
}

void GlobalSetting::musicVolumeChange(int value){

    IDE_TRACE_INT(value);
    if(value <0 ){
        value = 0;
    }

    if(value >255 ){
        value = 255;
    }

    qreal vo = (value/255.00)*100.00;
    IDE_TRACE_FLOAT(vo);
    m_Player->setVolume(AO_ADMUSIC,qFloor(vo));
}

void GlobalSetting::playMusic(int index){
    QString path = m_MediaUi->getResLocalPath(index);
    if(path.isEmpty()){
        return;
    }
    if(index ==40 ||index ==41||index ==42){
        //IDE_TRACE_STR(path);
        m_Player->play(AO_LIFTARV,path);
    }else{
        m_Player->play(AO_LIFTFLR,path);
    }
}

void GlobalSetting::sltBuzz(){
    QString beepFile = m_appAbsPath + D_BUNDLEDIR + "/" + D_BEEPFILE;
    //IDE_TRACE_STR(beepFile);
    m_Player->play(AO_BEEP,beepFile);
}

void GlobalSetting::sltBuzz(int index){
    //IDE_TRACE_INT(index);
    if(m_beepIndex == index ){
        if(m_beeped)
            return;
        //IDE_TRACE_INT(m_beepIndex);
        m_beeped = true;
        QString beepFile = m_appAbsPath + D_BUNDLEDIR + "/" + D_BEEPFILE;
        m_Player->play(AO_BEEP,beepFile);
    }else{
        m_beepIndex = index;
        //IDE_TRACE_INT(m_beepIndex);
        QString beepFile = m_appAbsPath + D_BUNDLEDIR + "/" + D_BEEPFILE;
        m_Player->play(AO_BEEP,beepFile);
        m_beeped = true;
    }
}

///RPDO2
void GlobalSetting::floorArrive(int ge,int shi,int bai,int soundNumber){
    FloorUi* floor = (FloorUi*)m_ComComponentGroup.value(COM_FLOOR);
    floor->floorCtrl(ge,shi,bai);
    playMusic(soundNumber);
}

void GlobalSetting::showFuncLogo(int funcLogoIndex,int soundNumber){
    FunctionUi* function = (FunctionUi*)m_ComComponentGroup.value(COM_FUNCTION);
    function->setFunction(funcLogoIndex);
    playMusic(soundNumber);
    QString tiptext =  function->tipText(funcLogoIndex);
    //if(!tiptext.isEmpty()){
    if(m_showFloorOrLogo){
        QDeclarativeItem *comUi = m_QmlItemGroup.value("realtimetext");
        comUi->setProperty("msgL1", tiptext);
        comUi->setProperty("msgL2", "");
    }
    //}
}

bool GlobalSetting::showFloorOrLogo(bool isShowLogo){
    m_showFloorOrLogo = isShowLogo;
    m_RootObj->setProperty("isShowLogo", isShowLogo);
    QDeclarativeItem *comUi = m_QmlItemGroup.value("realtimetext");
    if(!m_showFloorOrLogo){
        comUi->setProperty("msgL1", m_RealtimeTextLine1);
        comUi->setProperty("msgL2", m_RealtimeTextLine2);
    }
}

///RPDO3&RPDO4
void GlobalSetting::textMessage(QString m1,QString m2,QString m3,QString m4){
    ///sigMessage
    QDeclarativeItem *comUi = m_QmlItemGroup.value("fulltext");
    if(QString::compare(m_FullScreenTextLine1,m1,Qt::CaseSensitive)){
        m_FullScreenTextLine1 = m1;
        comUi->setProperty("msgLine1", m1);
    }
    if(QString::compare(m_FullScreenTextLine2,m2,Qt::CaseSensitive)){
        m_FullScreenTextLine2 = m2;
        comUi->setProperty("msgLine2", m2);
    }
    if(QString::compare(m_FullScreenTextLine3,m3,Qt::CaseSensitive)){
        m_FullScreenTextLine3 = m3;
        comUi->setProperty("msgLine3", m3);
    }
    if(QString::compare(m_FullScreenTextLine4,m4,Qt::CaseSensitive)){
        m_FullScreenTextLine4 = m4;
        comUi->setProperty("msgLine4", m4);
    }
}

void GlobalSetting::realtimeMessage(QString m1,QString m2){
    if(m_showFloorOrLogo){
        return;
    }
    QDeclarativeItem *comUi = m_QmlItemGroup.value("realtimetext");
    if(QString::compare(m_RealtimeTextLine1,m1,Qt::CaseSensitive)){
        m_RealtimeTextLine1 = m1;
        comUi->setProperty("msgL1", m1);
    }

    if(QString::compare(m_RealtimeTextLine2,m2,Qt::CaseSensitive)){
        m_RealtimeTextLine2 = m2;
        comUi->setProperty("msgL2", m2);
    }
}

void GlobalSetting::sltOrientation(int  value){
    if(value == 0 && m_curRotate == ROTATE_0){
        return;
    }

    if(value == 1 && m_curRotate == ROTATE_90){
        return;
    }

    if(value == 0 ){
        m_curRotate = ROTATE_0;
        //IDE_TRACE_INT(value);
        rotateView();
    }else if(value == 1) {
        m_curRotate = ROTATE_90;
        //IDE_TRACE_INT(value);
        rotateView();
    }

    if(m_curRotate == ROTATE_0 ||m_curRotate == ROTATE_180)
    {
        m_curDirection = Horizontal;
    }

    if(m_curRotate == ROTATE_90 ||m_curRotate == ROTATE_270)
    {
        m_curDirection = Vertical;
    }

    reinitUI();
}

void GlobalSetting::reinitUI(){
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
            //IDE_TRACE();
            uiItem->initGeometry(m_curDirection,m_curLayout);
            ///uiItem->initUi(m_curDirection);
        }
    }
    m_Desktop->initGeometry(m_curDirection,m_curLayout);

    if(!m_showFloorOrLogo){
        QDeclarativeItem *comUi = m_QmlItemGroup.value("realtimetext");
        comUi->setProperty("msgL1", m_RealtimeTextLine1);
        comUi->setProperty("msgL2", m_RealtimeTextLine2);
    }
}

void GlobalSetting::sltContraste(int  value){
    //IDE_TRACE_INT(value);
}

void GlobalSetting::sltBacklight(int  value){
    //IDE_TRACE_INT(value);
    if(value <0 ){
        value = 0;
    }

    if(value >255 ){
        value = 255;
    }

    qreal brightness = (value/255.00)*100.00;
    //IDE_TRACE_FLOAT(brightness);
    BacklightCtrl::setBacklightValue(qFloor(brightness));
}

void GlobalSetting::sltVitscrol(int  value){
    QDeclarativeItem *comUi = m_QmlItemGroup.value("arrow");
    if(comUi){
        //        if(value<=2){
        //            comUi->setProperty("inervalPerScroll", 6000);//6s/scroll
        //        }
        //        if(value>=3 && value<=5){
        //            comUi->setProperty("inervalPerScroll", 4000);//6s/scroll
        //        }
        //        if(value>=6 && value<=8){
        //            comUi->setProperty("inervalPerScroll", 3000);//6s/scroll
        //        }
        //        if(value>=9 && value<=10){
        //            comUi->setProperty("inervalPerScroll", 2000);//6s/scroll
        //        }
        if(value<=2){
            comUi->setProperty("inervalPerScroll", 2000);//6s/scroll
        }
        if(value>=3 && value<=5){
            comUi->setProperty("inervalPerScroll", 3000);//6s/scroll
        }
        if(value>=6 && value<=8){
            comUi->setProperty("inervalPerScroll", 4000);//6s/scroll
        }
        if(value>=9 && value<=10){
            comUi->setProperty("inervalPerScroll", 6000);//6s/scroll
        }
    }
}

///SDO1
void GlobalSetting::sDO1(int orientation,int contraste,int backlight,int vit_scrol){
    if(orientation == 0 &&m_curRotate!=ROTATE_0&&m_curRotate!=ROTATE_180){
        m_curRotate = ROTATE_0;
        rotateView();
    }else if(orientation == 0 &&m_curRotate!=ROTATE_90&&m_curRotate!=ROTATE_270) {
        m_curRotate = ROTATE_90;
        rotateView();
    }else{
        IDE_TRACE();
    }

    Q_UNUSED(contraste);
    m_Lcd->setbacklight(backlight);

    ArrowUi* arrow = (ArrowUi*)m_ComComponentGroup.value(COM_ARROW);
    if(arrow){
        arrow->setArrowSpeed(vit_scrol);
    }
}

///SDO2
void GlobalSetting::sDO2(int logo,int periode,int declenchement){
    m_customLogo->logoTrigger(logo,periode,declenchement);
}
/**********************CANOPEN FUNCTIONS ******************************/




/**********************WIFI FUNCTIONS ******************************/
void GlobalSetting::initWifi(){
    m_appClient = new AppClient();
    connect(m_appClient,SIGNAL(sigAppConnected()),this,SLOT(sltAppConnectedSucceed()));
    connect(m_appClient,SIGNAL(sigAppDisconnected()),this,SLOT(sltAppDisconnected()));
    connect(m_appClient,SIGNAL(sigLogin(QString,QString)),this,SLOT(sltAppLogin(QString,QString)));
    connect(m_appClient,SIGNAL(sigParameterSetting(QString,QVariant)),this,SLOT(sltParameterSetting(QString,QVariant)));
    connect(m_appClient,SIGNAL(sigSyncParametere()),this,SLOT(sltSyncParametere()));
    connect(m_appClient,SIGNAL(sigUpdateFinished()),this,SLOT(sltUpdateFinished()));
    m_appClient->startWork();
}

void GlobalSetting::sltAppConnectedSucceed(){
    IDE_TRACE();
    QDeclarativeItem *comUi = m_QmlItemGroup.value("wifiLogo");
    if(!comUi){
        comUi->setProperty("visible", true);
    }
}

void GlobalSetting::sltAppDisconnected(){
    QDeclarativeItem *comUi = m_QmlItemGroup.value("wifiLogo");
    IDE_TRACE();
    if(!comUi){
        comUi->setProperty("visible", false);
    }
}

void GlobalSetting::sltAppLogin(QString ssid,QString passwd){
    if(!QString::compare(ssid,"sodimas",Qt::CaseSensitive)){
        if(!QString::compare(m_mdpParser->getPassword(),passwd,Qt::CaseSensitive)){
            m_appClient->sltloginResult(true);
        }else{
            m_appClient->sltloginResult(false);
        }
    }else{
        IDE_TRACE_STR("Wrong SSID.");
    }
}

void GlobalSetting::sltParameterSetting(QString key,QVariant value){
    int va = 0;
    IDE_TRACE_STR(key);
    qDebug()<<"sltParameterSetting value "<<value;
    if(!QString::compare(key,"liftflrvol")){
        va = value.toInt();
        IDE_TRACE_INT(va);
        m_Player->setVolume(AO_LIFTFLR,qFloor(va));
    }else if(!QString::compare(key,"liftarvvol")){
        va = value.toInt();
        IDE_TRACE_INT(va);
        m_Player->setVolume(AO_LIFTARV,qFloor(va));
        m_Player->setVolume(AO_BEEP,qFloor(va));
    }else if(!QString::compare(key,"admusicvol")){
        va = value.toInt();
        IDE_TRACE_INT(va);
        m_Player->setVolume(AO_ADMUSIC,qFloor(va));
    }else if(!QString::compare(key,"luminance")){
        va = value.toInt();
        IDE_TRACE_INT(va);
        m_Lcd->setbacklight(va);
    }else if(!QString::compare(key,"darklevel")){
        va = value.toInt();
        m_Lcd->m_Standbyluminance = va;
    }else if(!QString::compare(key,"standby")){
        va = value.toInt();
        m_Lcd->m_Standby = va;
    }else if(!QString::compare(key,"rotation")){
        va = value.toInt();
        IDE_TRACE_INT(va);
        if(va == 0 ){
            m_curDirection = Horizontal;
            m_curRotate = ROTATE_0;
            rotateView();
            reinitUI();
        }else if(va == 90){
            m_curDirection = Vertical;
            m_curRotate = ROTATE_90;
            rotateView();
            reinitUI();
        }else if(va == 180){
            m_curDirection = Horizontal;
            m_curRotate = ROTATE_180;
            rotateView();
            reinitUI();
        }else if(va == 270){
            m_curDirection = Vertical;
            m_curRotate = ROTATE_270;
            rotateView();
            reinitUI();
        }
        m_Lcd->m_Rotation = va;

    }else if(!QString::compare(key,"layout")){
        va = value.toInt();
        m_curLayout = va;
    }else if(!QString::compare(key,"autotest")){
        //bool autotest = value.toBool();
    }

}

void GlobalSetting::sltSyncParametere(){
    QVariantMap map;
    map.insert("type","sendallparameter");
    QVariantMap info;
    info.insert("autotest",false);
    info.insert("liftflrvol",m_Player->m_LiftFlrVol);
    info.insert("liftarvvol",m_Player->m_LiftArvVol);
    info.insert("admusicvol",m_Player->m_AdmusicVol);
    info.insert("luminance",m_Lcd->m_Luminance);
    info.insert("darklevel",m_Lcd->m_Standbyluminance);
    info.insert("standby",m_Lcd->m_Standby);
    info.insert("rotation",m_Lcd->m_Rotation);
    info.insert("layout",m_themeLg->m_Layout);
    map.insert("info",info);
    m_appClient->sendJson(map);
}

void GlobalSetting::sltUpdateFinished(){
    //save files and reboot device
    IDE_TRACE();
    m_themeParser->saveUi();
    m_mdpParser->saveMdp();
    system("sync");
    system("reboot");
}
/**********************WIFI FUNCTIONS ******************************/


/**********************UTILLY FUNCTIONS ******************************/

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

void  GlobalSetting::clearInfoBuffer(){
    m_infoList.clear();
}

void  GlobalSetting::updateInfoProgess(QString msg,int progress){
    if(!msg.isEmpty()){
        if(m_infoList.count() >= 5){
            m_infoList.pop_front();
        }
        m_infoList.push_back(msg);
        emit sigInfo(m_infoList.join("\n"));
    }
    if(progress>0)
        emit sigProgress(progress);
}

int GlobalSetting::qmlSlotTest()
{

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
        m_MediaUi = new MediaUi();
        tmpComPtr = m_MediaUi;
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
         m_Lcd = new LcdPh(this);
        tmpPHDev = m_Lcd;
        break;
    case P_LED:
        tmpPHDev = new LedPh(this);
        break;
    case P_LANC:
        tmpPHDev = new LancPh(this);
        break;
    case P_Player:
        m_Player = new PlayerPh(this);
        tmpPHDev = m_Player;
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

QString GlobalSetting::getAppVersion(QString pFile) {
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
