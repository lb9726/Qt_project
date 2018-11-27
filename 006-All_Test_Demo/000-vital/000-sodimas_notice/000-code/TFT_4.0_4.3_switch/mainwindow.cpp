#include "mainwindow.h"

Uiloader *gUiLoader = 0;


const quint8 BACKLIGHT_LEVEL = 10;

bool setBacklightValue(quint8 brightness)
{
    //echo 4.6 > /sys/class/backlight/backlight/brightness
    if(brightness >100.0){
        brightness = 100.0;
    }
    IDE_TRACE_FLOAT(brightness);
    qreal level = (brightness/100.0);
    IDE_TRACE_FLOAT(level);
    QByteArray ba;
    ba.append(QString("%1").arg(qFloor(level*BACKLIGHT_LEVEL)));
    qDebug()<<"brightness "<<ba;
    int fd = open("/sys/class/backlight/backlight/brightness",O_WRONLY);
    if(fd == -1)
    {
        IDE_TRACE_STR("Open backlight node failed.");
        return false;
    }
    else
    {
        int ret = write(fd,ba.data(),ba.count());
        if(ret == ba.count()){
            ///IDE_TRACE();
            close(fd);
            return true;
        }else{
            IDE_TRACE();
            close(fd);
            return false;
        }

    }
}

Uiloader::Uiloader(QObject *parent)
    :QUiLoader(parent)
{
    gUiLoader = this;

    mView = 0;

    m_mdpParser = 0;
    m_themeParser = 0;
    m_themeLayout = 0;
    m_themeRotate = 0;
    m_themeDirection = Horizontal;

    mAutoTestCount = 0;
    connect(&mAutoTestTimer, SIGNAL(timeout()), this, SLOT(slot_AutoTest()));
    mAutoTestTimer.setInterval(300);
    mAutoTestTimer.start();
    connect(&mDeviceWatcher, SIGNAL(deviceAdded(QString)), this, SLOT(slot_DeviceAdded(QString)));
    connect(&mDeviceWatcher, SIGNAL(deviceRemoved(QString)), this, SLOT(slot_DeviceRemoved(QString)));
    mDeviceWatcher.start();
    mLabelUSB = 0;

    mWifiSocket = 0;
    mLabelWIFI = 0;
    mTcpServer.listen(QHostAddress::Any, 60001);
    connect(&mTcpServer, SIGNAL(newConnection()), this, SLOT(slot_NewWifiSocketAdded()));
    mWifiBusyTimer.setInterval(500);
    mWifiBusyTimer.setSingleShot(true);
    connect(&mWifiBusyTimer, SIGNAL(timeout()), this, SLOT(slot_WifiBusyStop()));

    mDesktop = 0;
    mArrow = 0;
    mFloor = 0;
    mIcon = 0;
    mIsShowLogo = false;
    mScrollSpeed = 0;

    mPort = 0;

    connect(&m_standbyTimer, SIGNAL(timeout()), this, SLOT(slot_StandBy()));
    connect(&m_screensaveTimer, SIGNAL(timeout()), this, SLOT(slot_ScreenSave()));
    connect(&m_screensaveHoldTimer, SIGNAL(timeout()), this, SLOT(slot_ScreenSaveHold()));

    connect(&mnewTest, &QTimer::timeout, this, &Uiloader::setScreenSave);
    mnewTest.start(2000);
    m_screenSaveForm = 0;
}

void Uiloader::slot_Message(QString pM1,QString pM2,QString pM3,QString pM4)
{
     IDE_TRACE_STR(pM1);
     IDE_TRACE_STR(pM2);
     IDE_TRACE_STR(pM3);
     IDE_TRACE_STR(pM4);

     if(mIsShowLogo)
     {
         if(mFloor)
             mFloor->setVisible(false);
         if(mIcon)
         {
             mIcon->setVisible(true);
             mIcon->setText(pM1, pM2, pM3, pM4);
         }
     }
}

void Uiloader::slot_ShowFloorOrLogo(bool PIsShowLogo)
{
    IDE_TRACE_INT(PIsShowLogo);
    mIsShowLogo = PIsShowLogo;
}

void Uiloader::slot_ShowFuncLogo(int pFuncLogoIndex,int pSoundNumber)
{
    IDE_TRACE_INT(pFuncLogoIndex);
    IDE_TRACE_INT(pSoundNumber);

    //显示ICON的代码
    if(mIsShowLogo)
    {
        if(mFloor)
            mFloor->setVisible(false);
        if(mIcon)
        {
            mIcon->setVisible(true);
            mIcon->setIcon(pFuncLogoIndex);
        }
    }
}
void Uiloader::slot_FloorArrive(int pGe,int pShi,int pBai,int pSoundNumber)
{
    IDE_TRACE_INT(pGe);
    IDE_TRACE_INT(pShi);
    IDE_TRACE_INT(pBai);
    IDE_TRACE_INT(pSoundNumber);
    if(mFloor)
    {
        //显示Floor的代码
        if(mIcon)
            mIcon->setVisible(false);
        if(mFloor)
        {
            mFloor->setVisible(true);
            mFloor->setFloor(pGe,pShi,pBai);
        }
    }
}
void Uiloader::slot_Buzz(int pBuzzIndex)
{
    IDE_TRACE_INT(pBuzzIndex);
}

void Uiloader::slot_ButtonLightOff(int pBtLightOffIndex)
{
    IDE_TRACE_INT(pBtLightOffIndex);
}

void Uiloader::slot_ButtonLightOn(int pBtLightOnIndex,bool pIsBuzz)
{
    IDE_TRACE_INT(pBtLightOnIndex);
    IDE_TRACE_INT(pIsBuzz);
}
void Uiloader::slot_MusicVolume(int pDivMusic)
{
    IDE_TRACE_INT(pDivMusic);
}
void Uiloader::slot_MainVolume(int pDivGong,int pDivBip)
{
    IDE_TRACE_INT(pDivGong);
    IDE_TRACE_INT(pDivBip);
}

void Uiloader::setScreenSave(/*bool pFlag*/)
{
    static int falg = 0;
    bool pFlag = true;
    if(!m_screenSaveForm)
    {
        m_screenSaveForm = new SplashForm;
        m_screenSaveForm->setWindowFlags(Qt::FramelessWindowHint);
//        mView->scene()->addWidget(m_screenSaveForm);
    }
    if(pFlag)
    {
//        ++ falg;
//        if (1 == falg)
//        {
//            this->SetLayout(0);
//            this->SetRotate(0);
//        }
//        else if (2 == falg)
//        {
//            this->SetLayout(0);
//            this->SetRotate(90);
//        }
//        else if (3 == falg)
//        {
//            this->SetLayout(0);
//            this->SetRotate(180);
//        }
//        else if (4 == falg)
//        {
//            this->SetLayout(0);
//            this->SetRotate(270);
//        }
//        else if (5 == falg)
//        {
//            this->SetLayout(1);
//            this->SetRotate(0);
//        }
//        else if (6 == falg)
//        {
//            this->SetLayout(1);
//            this->SetRotate(90);
//        }
//        else if (7 == falg)
//        {
//            this->SetLayout(1);
//            this->SetRotate(180);
//        }
//        else if (8 == falg)
//        {
//            this->SetLayout(1);
//            this->SetRotate(270);
//            falg = 0;
//        }
//        if (gUiLoader->m_themeDirection == Horizontal)
//        {
//            m_screenSaveForm->setGeometry(0, 0, 480, 272);
//        }
//        else if (gUiLoader->m_themeDirection == Vertical)
        {
            m_screenSaveForm->setGeometry(0, 0, 272, 480);
        }
//        m_screenSaveForm->showMaximized();
    }
    else
    {
        m_screenSaveForm->hide();
    }
}

void Uiloader::slot_ArrowAction(bool pArrows_FLM,bool pArrows_FLD,bool pArrows_SCROL)
{
    IDE_TRACE_INT(pArrows_FLM);
    IDE_TRACE_INT(pArrows_FLD);
    IDE_TRACE_INT(pArrows_SCROL);
    if(mArrow)
    {
        if(pArrows_FLM == true && pArrows_FLD == false && pArrows_SCROL == false)
        {
            mArrow->setArrowType(UiArrow::UpArrow);

            m_standbyTimer.stop();
//            setBacklightValue(m_luminance);

//            setScreenSave(false);
            m_screensaveTimer.stop();
            m_screensaveHoldTimer.stop();
        }
        else if(pArrows_FLM == false && pArrows_FLD == true && pArrows_SCROL == false)
        {
            mArrow->setArrowType(UiArrow::DownArrow);

            m_standbyTimer.stop();
//            setBacklightValue(m_luminance);

//            setScreenSave(false);
            m_screensaveTimer.stop();
            m_screensaveHoldTimer.stop();
        }
        else if(pArrows_FLM == true && pArrows_FLD == false && pArrows_SCROL == true)
        {
            mArrow->setArrowType(UiArrow::UpScrollArrow,mScrollSpeed);

            m_standbyTimer.stop();
//            setBacklightValue(m_luminance);

//            setScreenSave(false);
            m_screensaveTimer.stop();
            m_screensaveHoldTimer.stop();
        }
        else if(pArrows_FLM == false && pArrows_FLD == true && pArrows_SCROL == true)
        {
            mArrow->setArrowType(UiArrow::DownScrollArrow,mScrollSpeed);

            m_standbyTimer.stop();
//            setBacklightValue(m_luminance);

//            setScreenSave(false);
            m_screensaveTimer.stop();
            m_screensaveHoldTimer.stop();
        }
        else if(pArrows_FLM == true && pArrows_FLD == true)
        {
            mArrow->setArrowType(UiArrow::UpDownArrow);

            m_standbyTimer.stop();
//            setBacklightValue(m_luminance);

//            setScreenSave(false);
            m_screensaveTimer.stop();
            m_screensaveHoldTimer.stop();
        }
        else
        {
            mArrow->setArrowType(UiArrow::NoArrow);
            if(m_standby && !m_standbyTimer.isActive())
                m_standbyTimer.start();

            if(m_screensave && !m_screensaveTimer.isActive() && !m_screensaveTimer.isActive())
                m_screensaveTimer.start();
        }
    }
}

void Uiloader::slot_FCT_AFF(bool pIsFCT)
{
    IDE_TRACE_INT(pIsFCT);
}

void Uiloader::slot_Orient(int pOrientation) //sdo旋转横竖屏
{
    IDE_TRACE_INT(pOrientation);
}
void Uiloader::slot_Backlight(int pBacklight) //sdo背光
{
    IDE_TRACE_INT(pBacklight);
}
void Uiloader::slot_Vitscrol(int PVit_scrol)  //sdo箭头速度
{
    IDE_TRACE_INT(PVit_scrol);
    mScrollSpeed = PVit_scrol;
}

void Uiloader::slot_Debug(quint8 pDipsValue)
{
    IDE_TRACE_STR(QString::number(pDipsValue,16));
}

void Uiloader::slot_NodeState(int pState)  //NMT状态
{
    IDE_TRACE_INT(pState);
}

void Uiloader::slot_readAll()
{
    QByteArray tmpArray;
    tmpArray = mPort->readAll();
    if(tmpArray.count()!= 5)
        return;
    quint8 tmpValue0 = (quint8)tmpArray.at(0);
    quint8 tmpValue1 = (quint8)tmpArray.at(1);
    quint8 tmpValue2 = (quint8)tmpArray.at(2);

    QString tmpHexId= QString::number(tmpValue0,16);
    QString tmpHexValue1= QString::number(tmpValue1,16);
    QString tmpHexValue2= QString::number(tmpValue2,16);
    QString tmpString = QString("%1  %2  %3").arg(tmpHexId,tmpHexValue1,tmpHexValue2);
    IDE_DEBUG(tmpString);

    if(tmpValue0 == 1)
        mArrow->setArrowType(UiArrow::UpArrow);
    else if(tmpValue0 == 0)
        mArrow->setArrowType(UiArrow::DownArrow);
}

void Uiloader::slot_AutoTest()
{
    //显示Arroow的代码
    if(mArrow)
    {
        mArrow->show();
        mArrow->setArrowType(UiArrow::UpArrow, 0);
//        mArrow->setArrowType(UiArrow::UpScrollArrow, mAutoTestCount);
    }
    mAutoTestCount++;
    if (30 == mAutoTestCount)
    {
        SetLayout(0);
        IDE_TRACE_STR(QString("is layout 0"));
    }
    else if (60 == mAutoTestCount)
    {
        SetLayout(2);
        IDE_TRACE_STR(QString("is layout 2"));
    }
    else if (90 == mAutoTestCount)
    {
        SetLayout(1);
        IDE_TRACE_STR(QString("is layout 1"));
    }
    else if (120 == mAutoTestCount)
    {
        SetLayout(3);
        IDE_TRACE_STR(QString("is layout 3"));
    }
    if (mAutoTestCount > 120)
    {
        mAutoTestCount = 1;
    }
}

//void Uiloader::slot_AutoTest()
//{
//    mAutoTestCount++;

//    if(mAutoTestCount < 5)
//    {
//        SetLayout(0);
//        SetRotate(0);

//        //小屏模式
//        if(mIcon)
//        {
//            mIcon->setBigSrnMode(false);
//        }

//        //显示Floor的代码
//        if(mIcon)
//            mIcon->setVisible(false);
//        if(mFloor)
//        {
//            mFloor->setVisible(true);
//            mFloor->setFloor(mAutoTestCount%10, mAutoTestCount/10, 0);  //GE SHI BAI
//        }

//        //显示Arroow的代码
//        if(mArrow)
//        {
//            mArrow->show();
//            mArrow->setArrowType(UiArrow::UpScrollArrow, 5);
//        }
//    }
//    else if(mAutoTestCount < 10)
//    {
//        SetLayout(0);
//        SetRotate(270);
//    }
//    else if(mAutoTestCount < 20)
//    {
//        SetLayout(0);
//        SetRotate(90);
//    }
//    else if(mAutoTestCount < 30)
//    {
//        SetLayout(0);
//        SetRotate(180);
//    }
//    else if(mAutoTestCount < 40)
//    {
//        SetLayout(0);
//        SetRotate(270);
//    }
//    else if(mAutoTestCount < 49)
//    {
//        SetLayout(0);
//        SetRotate(0);

//        //小屏模式
//        if(mIcon)
//        {
//            mIcon->setBigSrnMode(false);
//        }

//        //显示ICON的代码
//        if(mFloor)
//            mFloor->setVisible(false);
//        if(mIcon)
//        {
//            mIcon->setVisible(true);
//            mIcon->setIcon(mAutoTestCount);
//        }

//        //显示Arroow的代码
//        if(mArrow)
//        {
//            mArrow->show();
//            mArrow->setArrowType(UiArrow::DownScrollArrow, 2);
//        }
//    }
//    else if(mAutoTestCount < 55)
//    {
//        SetLayout(0);
//        SetRotate(0);

//        //大屏模式
//        if(mIcon)
//        {
//            mIcon->setBigSrnMode(true);
//            QString tmpText = QString("%1 %1 %1 %1 %1 %1 %1").arg(mAutoTestCount);
//            mIcon->setText(tmpText, tmpText, tmpText, tmpText);
//        }
//    }
//    else if(mAutoTestCount < 60)
//    {
//        SetLayout(0);
//        SetRotate(90);

//        //大屏模式
//        if(mIcon)
//        {
//            mIcon->setBigSrnMode(true);
//            QString tmpText = QString("%1 %1 %1 %1 %1 %1 %1").arg(mAutoTestCount);
//            mIcon->setText(tmpText, tmpText, tmpText, tmpText);
//        }
//    }
//    else if(mAutoTestCount < 65)
//    {
//        SetLayout(0);
//        SetRotate(90);

//        //小屏模式
//        if(mIcon)
//        {
//            mIcon->setBigSrnMode(false);
//        }

//        //显示Arroow的代码
//        if(mArrow)
//        {
//            mArrow->show();
//            mArrow->setArrowType(UiArrow::UpScrollArrow, 1);
//        }

//        //显示Floor的代码
//        if(mIcon)
//            mIcon->setVisible(false);
//        if(mFloor)
//        {
//            mFloor->setVisible(true);
//            mFloor->setFloor(mAutoTestCount%10, mAutoTestCount/10, 0);  //GE SHI BAI
//        }
//    }
//    else if(mAutoTestCount < 70)
//    {
//        SetLayout(1);
//        SetRotate(90);

//        //小屏模式
//        if(mIcon)
//        {
//            mIcon->setBigSrnMode(false);
//        }

//        //显示Arroow的代码
//        if(mArrow)
//        {
//            mArrow->show();
//            mArrow->setArrowType(UiArrow::UpScrollArrow, 1);
//        }

//        //显示Floor的代码
//        if(mIcon)
//            mIcon->setVisible(false);
//        if(mFloor)
//        {
//            mFloor->setVisible(true);
//            mFloor->setFloor(mAutoTestCount%10, mAutoTestCount/10, 0);  //GE SHI BAI
//        }
//    }
//    else if(mAutoTestCount >= 70)
//    {
//        SetRotate(0);
//        mAutoTestCount = 0;
//    }
//}

void Uiloader::slot_NewWifiSocketAdded()
{
    mWifiSocket = mTcpServer.nextPendingConnection();
    connect(mWifiSocket, SIGNAL(readyRead()), this, SLOT(slot_WifiSocketRead()));
    connect(mWifiSocket, SIGNAL(disconnected()), this, SLOT(slot_WifiSocketRemoved()));

    //显示图标
    if(!mLabelWIFI)
    {
        mLabelWIFI = new QLabel;
        mLabelWIFI->setScaledContents(true);
        mLabelWIFI->setPixmap(m_themeParser->m_FileDir + "wifi.png");
    }
    if(mDesktop)
    {
        mLabelWIFI->setParent(mDesktop);
        mLabelWIFI->setGeometry(mDesktop->width()*3/4+50, 5, 45, 20);
    }
    mLabelWIFI->show();
}

void Uiloader::slot_WifiSocketRemoved()
{
    mWifiSocket = 0;

    if(mLabelWIFI)
    {
        mLabelWIFI->hide();
    }
}

void Uiloader::slot_WifiSocketRead()
{
    //制作WIFI数据传输时的动态效果
    if(mLabelWIFI)
    {
        mLabelWIFI->setStyleSheet("QLabel{border-radius:3px;background-color:rgba(10,250,10,200);}");
        mWifiBusyTimer.start();
    }

    QByteArray tmpArray = mWifiSocket->readAll();
    IDE_TRACE_STR(QString(tmpArray));
}

void Uiloader::slot_WifiBusyStop()
{
    //制作WIFI数据传输时的动态效果
    if(mLabelWIFI)
    {
        mLabelWIFI->setStyleSheet("QLabel{border-radius:0px;background-color:rgba(10,250,10,0);}");
    }
}

//这个不是类函数
bool CopyFileContent(QString srcFile, QString dstFile)
{
    //>@如果路径相同，则不进行拷贝
    if(srcFile.compare(dstFile) == 0)
        return true;

    QFileInfo tmpSrcFileInfo(srcFile);
    if(!tmpSrcFileInfo.isFile())
    {
        return false;
    }

    if (dstFile.isEmpty())
        return false;
    dstFile.replace("\\", "/");
    if(dstFile.endsWith('/'))
        return false;
    int index = dstFile.lastIndexOf('/');
    if(index < 0)
        return false;
    QString tmpDstPath = dstFile.mid(0, index + 1);
    if(tmpDstPath.isEmpty())
    {
        return false;
    }
    QDir tmpDstDir;
    if(!tmpDstDir.mkpath(tmpDstPath))
    {
        return false;
    }
    bool flag = QFile::copy(srcFile, dstFile);
    return flag;
}

QFileInfoList ListFolder(QFileInfo pDirInfo, bool pSubDir)
{
    QFileInfoList tmpFileList;
    if(!pDirInfo.isDir())
        return tmpFileList;
    QDir tmpDir(pDirInfo.filePath());
    //>@先处理文件夹
    if(pSubDir)
    {
        QFileInfoList tmpFolders = tmpDir.entryInfoList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        int count = tmpFolders.count();
        for (int i = 0; i < count; ++i)
        {
            QFileInfo tmpSubFolder = tmpFolders.at(i);
            tmpFileList.append(ListFolder(tmpSubFolder, pSubDir));
        }
    }
    //>@再处理文件
    QFileInfoList tmpFiles = tmpDir.entryInfoList(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    for(int i = 0;i<tmpFiles.count();i++)
    {
        QFileInfo tmpFileInfo = tmpFiles.at(i);
        if(tmpFileInfo.isFile())
        {
            {
                tmpFileList.append(tmpFileInfo);
            }
        }
    }
    tmpFileList.append(pDirInfo);  //>@包括文件夹，但文件夹没有大小
    return tmpFileList;
}

bool CopyFolder(QString pSrcDir, QString pDstDir, bool pSubfolder)
{
    QFileInfo tmpFileInfo(pSrcDir);
    if(!tmpFileInfo.isDir())
    {
        return false;
    }
    pSrcDir.replace(QString("//"),QString("/"));
    pDstDir.replace(QString("//"),QString("/"));
    if(!pSrcDir.endsWith('/'))
        pSrcDir.append('/');
    if(!pDstDir.endsWith('/'))
        pDstDir.append('/');
    //>@如果路径相同，则不进行拷贝
    if(pSrcDir.compare(pDstDir) == 0)
        return true;

    //创建文件夹
    QDir dir(pDstDir);
    if(!dir.exists())
    {
        if(!dir.mkpath(pDstDir))
        {
            return false;
        }
    }

    QFileInfoList tmpFileList = ListFolder(pSrcDir, pSubfolder);
    if(tmpFileList.isEmpty())
        return false;
    foreach(QFileInfo tmpFileInfo, tmpFileList)
    {
        if(!tmpFileInfo.isFile())
            continue;
        QString tmpSrcFilePath = tmpFileInfo.filePath();
        QString tmpSrcFileName = tmpFileInfo.fileName();
        QString tmpDstFilePath = pDstDir + tmpSrcFileName;
        CopyFileContent(tmpSrcFilePath, tmpDstFilePath);
    }
    return true;
}

void Uiloader::slot_DeviceAdded(QString pName)
{
#ifdef Q_OS_WIN
    QStringList tmpList = pName.split(">");
    if(tmpList.count() < 2)
        return;
    QString tmpUDiskPath = tmpList.at(1);
#else
    QString tmpUDiskPath = "/mnt/sda1/";
    QtSleep(1000);
#endif

    //显示图标
    if(!mLabelUSB)
    {
        mLabelUSB = new QLabel;
        mLabelUSB->setScaledContents(true);
        mLabelUSB->setPixmap(m_themeParser->m_FileDir + "usb.png");
    }
    if(mDesktop)
    {
        mLabelUSB->setParent(mDesktop);
        mLabelUSB->setGeometry(mDesktop->width()*3/4, 5, 45, 20);
    }
    mLabelUSB->show();

    //制作U盘拷贝数据传输时的动态效果
    if(mLabelUSB)
    {
        mLabelUSB->setStyleSheet("QLabel{border-radius:10px;background-color:rgba(10,250,10,200);}");
        QtSleep(10);
    }

    CopyFileContent(tmpUDiskPath+"abc.png", "/usr/bst/usrfs/abc.png");
    CopyFolder(tmpUDiskPath + QString("rc/"), QString("/usr/bst/usrfs/rc/"), true);

    //制作U盘拷贝数据传输时的动态效果
    if(mLabelUSB)
    {
        mLabelUSB->setStyleSheet("QLabel{border-radius:10px;background-color:rgba(10,250,10,0);}");
        QtSleep(10);
    }
}

void Uiloader::slot_DeviceRemoved(QString pName)
{
    //隐藏图标
    //显示图标
    if(mLabelUSB)
    {
        mLabelUSB->hide();
    }

    system("sync");
    system("reboot");
}

QWidget* Uiloader::createWidget(const QString &className, QWidget *parent, const QString &name)
{
    if(!className.compare("UiArrow", Qt::CaseInsensitive))
    {
        UiArrow* tmpArrow = new UiArrow(parent);
        tmpArrow->setObjectName(name);
        mArrow = tmpArrow;
        return tmpArrow;
    }
    if(!className.compare("UiFloor", Qt::CaseInsensitive))
    {
        UiFloor* tmpFloor = new UiFloor(parent);
        tmpFloor->setObjectName(name);
        mFloor = tmpFloor;
        return tmpFloor;
    }
//    if(!className.compare("UiClock", Qt::CaseInsensitive))
//    {
//        UiClock* tmpClock = new UiClock;
//        tmpClock->setObjectName(name);
//        tmpClock->Init();
//        return tmpClock;
//    }
    if(!className.compare("UiDesktop", Qt::CaseInsensitive))
    {
        UiDesktop* tmpDesktop = new UiDesktop(parent);
        tmpDesktop->setObjectName(name);
        mDesktop = tmpDesktop;
        return tmpDesktop;
    }
    if(!className.compare("UiIcon", Qt::CaseInsensitive))
    {
        UiIcon* tmpIcon = new UiIcon(parent);
        tmpIcon->setObjectName(name);
        mIcon = tmpIcon;
        return tmpIcon;
    }
//    if(!className.compare("UiVideo", Qt::CaseInsensitive))
//    {
//        UiVideo* tmpVideo = new UiVideo;
//        tmpVideo->setObjectName(name);
//        tmpVideo->Init();
//        return tmpVideo;
//    }
    return QUiLoader::createWidget(className, parent, name);
}

void Uiloader::InitComponent(COM_TYPE pType, QDomElement pElement)
{
    if(pType == COM_DESKTOP && mDesktop)
    {
        mDesktop->Init(pElement);
    }
    else if(pType == COM_FLOOR && mFloor)
    {
        mFloor->Init(pElement);
    }
    else if(pType == COM_FUNCTION && mIcon)
    {
        mIcon->Init(pElement);
    }
    else if(pType == COM_ARROW && mArrow)
    {
        mArrow->Init(pElement);
    }
}

void Uiloader::SetLayout(int pLayout)
{
    if(m_themeLayout != pLayout)
    {
        m_themeLayout = pLayout;

        //重新初始化所有控件
        QList<COM_TYPE> tmpComTypeList = m_themeParser->m_ComElementGroup.keys();
        int count = tmpComTypeList.count();
        for(int i=0;i<count;i++)
        {
            COM_TYPE tmpType = tmpComTypeList.at(i);

            QDomElement tmpElement = m_themeParser->m_ComElementGroup.value(tmpType);
            if(tmpElement.isNull())
                continue;
            InitComponent(tmpType, tmpElement);
        }
    }
}

void Uiloader::SetRotate(int pRotate)
{
    if(m_themeRotate != pRotate)
    {
        if(pRotate == 0 || pRotate == 180)
        {
            m_themeDirection = Horizontal;
        }
        else
        {
            m_themeDirection = Vertical;
        }

        mView->rotate(pRotate - m_themeRotate);
        m_themeRotate = pRotate;

        //重新初始化所有控件
        QList<COM_TYPE> tmpComTypeList = m_themeParser->m_ComElementGroup.keys();
        int count = tmpComTypeList.count();
        for(int i=0;i<count;i++)
        {
            COM_TYPE tmpType = tmpComTypeList.at(i);

            QDomElement tmpElement = m_themeParser->m_ComElementGroup.value(tmpType);
            if(tmpElement.isNull())
                continue;
            InitComponent(tmpType, tmpElement);
        }
    }
}


QWidget *Uiloader::Init(QString pUiPath)
{
    QWidget* tmpWidget = 0;
    QString tmpRccStr = pUiPath + "Theme.rcc";
    QResource::registerResource(tmpRccStr);

    QString tmpUiStr = pUiPath + "Theme.ui";
    QFile tmpFile(tmpUiStr);
    if(!tmpFile.open(QFile::ReadOnly))
        return tmpWidget;
    tmpWidget = load(&tmpFile);

    m_mdpParser = new MdpParser(pUiPath + "device.xml");
    if(m_mdpParser->openMdp())
    {
        QDomElement tmpPlayerElement = m_mdpParser->m_PHElementGroup.value(P_Player);
        if(!tmpPlayerElement.isNull())
        {
            QDomElement tmpElement = tmpPlayerElement.firstChildElement("admusicvol");
            if(!tmpElement.isNull())
                m_admusicvol = tmpElement.text().toInt();
            tmpElement = tmpPlayerElement.firstChildElement("beepvol");
            if(!tmpElement.isNull())
                m_beepvol = tmpElement.text().toInt();
            tmpElement = tmpPlayerElement.firstChildElement("liftarvvol");
            if(!tmpElement.isNull())
                m_liftarvvol = tmpElement.text().toInt();
            tmpElement = tmpPlayerElement.firstChildElement("liftflrvol");
            if(!tmpElement.isNull())
                m_liftflrvol = tmpElement.text().toInt();
        }

        QDomElement tmpLedElement = m_mdpParser->m_PHElementGroup.value(P_LED);
        if(!tmpLedElement.isNull())
        {
            QDomElement tmpElement = tmpLedElement.firstChildElement("normal");
            if(!tmpElement.isNull())
                m_lednormalFreq = tmpElement.text();
            tmpElement = tmpLedElement.firstChildElement("busy");
            if(!tmpElement.isNull())
                m_ledbusyFreq = tmpElement.text();
            tmpElement = tmpLedElement.firstChildElement("error");
            if(!tmpElement.isNull())
                m_lederrorFreq = tmpElement.text();
        }

        QDomElement tmpLcdElement = m_mdpParser->m_PHElementGroup.value(P_LCD);
        if(!tmpLcdElement.isNull())
        {
            QDomElement tmpElement = tmpLcdElement.firstChildElement("rotation");
            if(!tmpElement.isNull())
                m_rotation = tmpElement.text().toInt();
            tmpElement = tmpLcdElement.firstChildElement("luminance");
            if(!tmpElement.isNull())
                m_luminance = tmpElement.text().toInt();
            tmpElement = tmpLcdElement.firstChildElement("standby");
            if(!tmpElement.isNull())
                m_standby = tmpElement.text().toInt();
            tmpElement = tmpLcdElement.firstChildElement("standbyluminance");
            if(!tmpElement.isNull())
                m_standbyluminance = tmpElement.text().toInt();
            tmpElement = tmpLcdElement.firstChildElement("screensave");
            if(!tmpElement.isNull())
                m_screensave = tmpElement.text().toInt();
            tmpElement = tmpLcdElement.firstChildElement("screensavehold");
            if(!tmpElement.isNull())
                m_screensavehold = tmpElement.text().toInt();
        }

        QDomElement tmpComElement = m_mdpParser->m_PHElementGroup.value(P_COM);
        if(!tmpComElement.isNull())
        {
            QDomElement tmpElement = tmpComElement.firstChildElement("autotest");
            if(!tmpElement.isNull())
                m_autotest = (bool)(!tmpElement.text().compare("true"));
            tmpElement = tmpComElement.firstChildElement("testfreq");
            if(!tmpElement.isNull())
                m_testfreq = tmpElement.text().toInt();
        }

        QDomElement tmpThemeElement = m_mdpParser->m_LGElementGroup.value(L_ThemeManager);
        if(!tmpThemeElement.isNull())
        {
            QDomElement tmpElement = tmpThemeElement.firstChildElement("layout");
            if(!tmpElement.isNull())
                m_layout = tmpElement.text().toInt();
        }
    }

    m_themeParser = new ThemeParser(pUiPath + "theme.xml");
    if(m_themeParser->openUi())
    {
        QList<COM_TYPE> tmpComTypeList = m_themeParser->m_ComElementGroup.keys();
        int count = tmpComTypeList.count();
        for(int i=0;i<count;i++)
        {
            COM_TYPE tmpType = tmpComTypeList.at(i);

            QDomElement tmpElement = m_themeParser->m_ComElementGroup.value(tmpType);
            if(tmpElement.isNull())
                continue;
            InitComponent(tmpType, tmpElement);
        }
    }

    //LCD
//    setBacklightValue(m_luminance);
    if(m_standby>0)
    {
        m_standbyTimer.setInterval(m_standby);
        m_standbyTimer.setSingleShot(true);
        m_standbyTimer.start();
    }
    if(m_screensave>0)
    {
        m_screensaveTimer.setInterval(m_screensave);
        m_screensaveTimer.setSingleShot(true);
        m_screensaveTimer.start();
    }

    //Theme
    SetLayout(m_layout);

    //LED
    //闪烁代码，因不知LED管脚，所以未做

    //Player
    m_Player = new Mplayer;
    QString backgroundMusic = "/usr/bst/usrfs/music/M0.mp3";
    m_Player->setBackgroundMuisc(backgroundMusic, m_admusicvol); //100是音量
    m_Player->startWork();

    m_Player->addMedia(AO_LIFTFLR,"/usr/bst/usrfs/media/M8.mp3", 1, false, m_liftflrvol) ;  //100是音量
    m_Player->setBackgroundPlayerVolume(m_admusicvol);

    //autotest
    IDE_TRACE_INT(m_autotest);
    if(m_autotest)
    {
//        slot_AutoTest();
//        mAutoTestTimer.setInterval(m_testfreq);
//        mAutoTestTimer.start();
    }
    else
    {
        mPort = new QSerialPort;
        mPort->setPortName("COM1");
        mPort->setBaudRate(9600);
        connect(mPort,SIGNAL(readyRead()),this,SLOT(slot_readAll()));
        mPort->open(QIODevice::ReadWrite);

    #ifdef LINUX
        DipsIO* tmpDipIO = new DipsIO;
        connect(tmpDipIO,SIGNAL(sValueChanged(quint8)),this,SLOT(slot_Debug(quint8)),Qt::QueuedConnection);

        BCButtons *tmpBcButtons = new BCButtons;

        OpenClient *tmpCanOpen = new OpenClient;
        QThread *m_canopenThread = new QThread;
        tmpCanOpen->moveToThread(m_canopenThread);
        tmpCanOpen->initCanopenClient(60, 250, "0");

        connect(tmpCanOpen,SIGNAL(sigNodeState(int)),this,SLOT(slot_NodeState(int))); //NMT状态

        connect(tmpCanOpen,SIGNAL(sigOrientation(int)),this,SLOT(slot_Orient(int))); //sdo旋转横竖屏
        connect(tmpCanOpen,SIGNAL(sigBacklight(int)),this,SLOT(slot_Backlight(int))); //sdo背光
        connect(tmpCanOpen,SIGNAL(sigVit_scrol(int)),this,SLOT(slot_Vitscrol(int))); //sdo箭头速度

        connect(tmpCanOpen,SIGNAL(sigFCT_AFF(bool)),this,SLOT(slot_FCT_AFF(bool))); //大小屏
        connect(tmpCanOpen,SIGNAL(sigArrowAction(bool,bool,bool)),this,SLOT(slot_ArrowAction(bool,bool,bool)));  //上下箭头滚动
        connect(tmpCanOpen,SIGNAL(sigMainVolumeChange(int,int)),this,SLOT(slot_MainVolume(int,int)));  //gong、beep
        connect(tmpCanOpen,SIGNAL(sigMusicVolumeChange(int)),this,SLOT(slot_MusicVolume(int)));  //背景音
        connect(tmpCanOpen,SIGNAL(sigButtonLightOn(int,bool)),this,SLOT(slot_ButtonLightOn(int,bool)));
        connect(tmpCanOpen,SIGNAL(sigButtonLightOff(int)),this,SLOT(slot_ButtonLightOff(int)));
        connect(tmpCanOpen,SIGNAL(sigBuzz(int)),this,SLOT(slot_Buzz(int)));

        connect(tmpCanOpen,SIGNAL(sigFloorArrive(int,int,int,int)),this,SLOT(slot_FloorArrive(int,int,int,int)));  //个十百声音
        connect(tmpCanOpen,SIGNAL(sigShowFuncLogo(int,int)),this,SLOT(slot_ShowFuncLogo(int,int)));  //功能图标 声音
        connect(tmpCanOpen,SIGNAL(sigShowFloorOrLogo(bool)),this,SLOT(slot_ShowFloorOrLogo(bool)));  //是否显示logo
        connect(tmpCanOpen,SIGNAL(sigMessage(QString,QString,QString,QString)),this,SLOT(slot_Message(QString,QString,QString,QString)));//4个文本

        m_canopenThread->start();
    #endif

        mSaf = new SafThread;
        connect(mSaf,SIGNAL(sigArrowAction(bool,bool,bool)),this,SLOT(slot_ArrowAction(bool,bool,bool)));  //上下箭头滚动
        connect(mSaf,SIGNAL(sMessageAudio(int)),this,SLOT(slot_ArrowAction(bool,bool,bool)));
        connect(mSaf,SIGNAL(sigShowFuncLogo(int,int)),this,SLOT(slot_ShowFuncLogo(int,int)));  //功能图标 声音
    }

    return tmpWidget;
}

void Uiloader::slot_StandBy()
{
    //如果有箭头，则不会进入standby模式，同时如果楼层变化，在m_standby时间内也不会进入standby模式，但从实际控制柜的功能来讲，楼层改变时应该一定会有箭头，所以可以只判断箭头即可
    //如果有例外情况，那么再来加判断语句
    if(m_standby > 0)
    {
//        setBacklightValue(m_standbyluminance);
        m_standbyTimer.setInterval(m_standby);
        m_standbyTimer.start();
    }
}

void Uiloader::slot_ScreenSave()
{
//    setScreenSave(true);

    if(m_screensavehold>0)
    {
        m_screensaveHoldTimer.setInterval(m_screensavehold);
        m_screensaveHoldTimer.setSingleShot(true);
        m_screensaveHoldTimer.start();
    }
}

void Uiloader::slot_ScreenSaveHold()
{
//    setScreenSave(false);

    if(m_screensave>0)
    {
        m_screensaveTimer.setInterval(m_screensave);
        m_screensaveTimer.start();
    }
}
