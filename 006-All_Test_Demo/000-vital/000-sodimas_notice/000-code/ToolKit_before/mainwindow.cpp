#include "mainwindow.h"
#include <QTimer>

Uiloader *gUiLoader = 0;

Uiloader::Uiloader(QObject *parent)
    :QUiLoader(parent)
{
    gUiLoader = this;

    mView = 0;
    m_themeParser = 0;
    m_themeLayout = 0;
    m_themeRotate = 0;
    m_themeDirection = Horizontal;
    m_standby = 30000;
    mArrows_FLM = false;
    mArrows_FLD = false;
    mSaverFlag = false;
    mHasEnterFlag = false;
    m_autotest = false;

    mAutoTestCount = 0;

    connect(&mAutoTestTimer, SIGNAL(timeout()), this, SLOT(slot_AutoTest()));

    connect(&mDeviceWatcher, SIGNAL(deviceAdded(QString)), this, SLOT(slot_DeviceAdded(QString)), Qt::UniqueConnection);
    connect(&mDeviceWatcher, SIGNAL(deviceRemoved(QString)), this, SLOT(slot_DeviceRemoved(QString)), Qt::UniqueConnection);
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
    mIsShowLogo = true;
    mScrollSpeed = 3;
    mUsbIsInsertFlag = false;
    mMusicNumber = -1;
    mWifiInsertFlag = false;
    mFunctionMusicNum = -1;
    mBigTextFlag = false;
    mLastBackLight = 10;

    mTempSound =  -1;
    connect(&mScreenSaver, &ScreenSaver::sigSetBlackPicture, this, &Uiloader::setScreenSave, Qt::UniqueConnection);
    m_screenSaveForm = new SplashForm;
    m_screenSaveForm->hide(); // first should hide the screen
    m_messageBar = new MessageBar;
    m_messageBar->hide();

}

void Uiloader::slot_Message(QString pM1, QString pM2, QString pM3, QString pM4)
{
     IDE_TRACE_STR(pM1);
     IDE_TRACE_STR(pM2);
     IDE_TRACE_STR(pM3);
     IDE_TRACE_STR(pM4);
     if (mUsbIsInsertFlag) // if usb is inserted, return
     {
         return;
     }
     if (mBigTextFlag)
     {
         if (m_Player)
         {
             m_Player->pauseBackgroundPlayer();
             m_Player->setBackgroundPlayerVolume(0);
         }
     }

     if(mIcon)
     {
         mIcon->setText(pM1, pM2, pM3, pM4);
     }
}

void Uiloader::slot_RealMessage(QString pM1, QString pM2)
{
    if (mUsbIsInsertFlag)
    {
        return;
    }
    if (mFloor)
    {
        mFloor->setTimeAndDest(pM1, pM2);
    }
}

void Uiloader::slot_switchValue(int s1, int s2, int s3, int s4)
{
    //saf display
    if(s1 == 1 && s2 == 1)
    {
        SetLayout(0);
        SetRotate(180);
    }
    if(s1 == 0 && s2 == 1)
    {
        SetLayout(1);
        SetRotate(90);
    }
    if(s1 == 1 && s2 == 0)
    {
        SetLayout(0);
        SetRotate(180);
    }
    if(s1 == 0 && s2 == 0)
    {
        SetLayout(1);
        SetRotate(270);
    }

    //volume
    if(s3 == 1 && s4 == 1)
    {
//        m_Player->pauseBackgroundPlayer();
//        m_Player->setBackgroundPlayerVolume(m_admusicvol);
//        m_Player->playBackgroundPlayer();
    }
    if(s3 == 1 && s4 == 0)
    {
        m_Player->pauseBackgroundPlayer();
        m_Player->setBackgroundPlayerVolume(m_admusicvol/2);
        m_Player->playBackgroundPlayer();
        m_beepvol /= 2;
        m_liftarvvol /= 2;
        m_liftflrvol /= 2;
    }
    if(s3 == 0 && s4 == 1)
    {
        m_Player->pauseBackgroundPlayer();
        m_Player->setBackgroundPlayerVolume(m_admusicvol/3);
        m_Player->playBackgroundPlayer();

        m_beepvol /= 3;
        m_liftarvvol /= 3;
        m_liftflrvol /= 3;
    }
    if(s3 == 0 && s4 == 0)
    {
        m_Player->pauseBackgroundPlayer();
        m_Player->setBackgroundPlayerVolume(0);
        m_Player->playBackgroundPlayer();

        m_beepvol = 0;
        m_liftarvvol = 0;
        m_liftflrvol = 0;
    }
}

void Uiloader::slot_ShowFloorOrLogo(bool PIsShowLogo)
{
    IDE_TRACE_INT(PIsShowLogo);
    mIsShowLogo = PIsShowLogo;
}

void Uiloader::slot_ShowFuncLogo(int pFuncLogoIndex, int pSoundNumber)
{
    IDE_TRACE_INT(pFuncLogoIndex);
    IDE_TRACE_INT(pSoundNumber);

    if (mBigTextFlag) // if bigtext will return
    {
        return;
    }
    if (mUsbIsInsertFlag) // if usb is inserted, return
    {
        return;
    }
//    mIcon->setBigSrnMode(false); // set small text
    // 显示ICON的代码
    if(mIsShowLogo)
    {
        if(mFloor)
            mFloor->setVisible(false);
        if(mIcon)
        {
            mIcon->setVisible(true);
            mIcon->setIcon(pFuncLogoIndex);
        }
        if (m_Player)
        {
            if (mFunctionMusicNum != pSoundNumber)
            {
                mFunctionMusicNum = pSoundNumber;
                QString overloadMusic = MediaPath + QString("M%1.mp3").arg(pSoundNumber);
                m_Player->addMedia(AO_BEEP, overloadMusic, 1, false, m_beepvol) ; // m_liftFLrVol是音量
            }
        }
    }
}

void Uiloader::slot_FloorArrive(int pGe, int pShi, int pBai, int pSoundNumber)
{
    IDE_TRACE_INT(pGe);
    IDE_TRACE_INT(pShi);
    IDE_TRACE_INT(pBai);
    IDE_TRACE_INT(pSoundNumber);

//    if (mBigTextFlag) // if bigtext will return
//    {
//        return;
//    }
    if (mUsbIsInsertFlag) // if usb is inserted, return
    {
        return;
    }
    if(mFloor)
    {
        //显示Floor的代码
        if(mIcon)
        {
            mIcon->setVisible(false);
        }
        if(mFloor)
        {
            mFloor->setVisible(true);
            mFloor->setFloor(pGe, pShi, pBai);
            if(pSoundNumber != -1)
            {
                m_Player->addMedia(AO_LIFTFLR, QString("/usr/bst/usrfs/media/M%1.mp3").arg(pSoundNumber), 1, false, m_liftflrvol) ; // 100是音量
            }
        }
    }
}

void Uiloader::slot_CanopenFloorArrive(int pGe, int pShi, int pBai, int pSoundNumber)
{
    IDE_TRACE_INT(pGe);
    IDE_TRACE_INT(pShi);
    IDE_TRACE_INT(pBai);
    IDE_TRACE_INT(pSoundNumber);

    if (mBigTextFlag)
    {
        return;
    }
    if (mUsbIsInsertFlag) // if usb is inserted, return
    {
        return;
    }
    if(mFloor) //show Floor
    {
        if(mIcon)
        {
            mIcon->setVisible(false);
        }
        if(mFloor)
        {
            mFloor->setVisible(true);
            mFloor->setFloor(pGe, pShi, pBai);
            if(pSoundNumber != -1)
            {
                if (mMusicNumber != pSoundNumber)
                {
                    mMusicNumber = pSoundNumber;
                    QString floorMusic = MediaPath + QString("M%1.mp3").arg(mMusicNumber);
                    if (41 == mMusicNumber || 42 == mMusicNumber || 43 == mMusicNumber)
                    {
                        m_Player->addMedia(AO_LIFTARV, floorMusic, 1, false, m_liftarvvol);
                    }
                    else
                    {
                        m_Player->addMedia(AO_LIFTFLR, floorMusic, 1, false, m_liftflrvol);
                    }
                }
            }
        }
    }
}

void Uiloader::slot_CanopenBuzz(int pBuzzIndex)
{
    IDE_TRACE_INT(pBuzzIndex);
}

void Uiloader::slot_CanopenshortBuzz()
{
    if (m_Player)
    {
        QString beepMusic = MediaPath + "beep.wav";
        IDE_TRACE_STR(beepMusic);
        m_Player->addMedia(AO_BEEP, beepMusic, 1, false, m_beepvol) ; //m_beepvol 是音量
    }
}

void Uiloader::slot_MusicVolume(int pDivMusic)
{
    IDE_TRACE_INT(pDivMusic);
}

void Uiloader::slot_CanopenMusicVolume(int pDivMusic)
{
    IDE_TRACE_INT(pDivMusic);
    if (m_Player)
    {
        m_Player->pauseBackgroundPlayer();
        if (pDivMusic * 10 >= 99)
        {
            m_admusicvol = 95;
        }
        else
        {
            m_admusicvol = pDivMusic * 10;
        }
        mTempSound = m_admusicvol;
        m_Player->setBackgroundPlayerVolume(m_admusicvol);
    }
}

void Uiloader::slot_MainVolume(int pDivGong, int pDivBip)
{
    IDE_TRACE_INT(pDivGong);
    IDE_TRACE_INT(pDivBip);
    if(pDivGong != -1)
        m_Player->addMedia(AO_LIFTARV, QString("/usr/bst/usrfs/media/M%1.mp3").arg(pDivGong), 1, false, m_liftarvvol) ; //100是音量
    if(pDivBip != -1)
        m_Player->addMedia(AO_BEEP, QString("/usr/bst/usrfs/media/M%1.mp3").arg(pDivBip), 1, false, m_beepvol) ; //100是音量
}

void Uiloader::slot_CanopenMainVolume(int pDivGong, int pDivBip)
{
    IDE_TRACE_INT(pDivGong);
    IDE_TRACE_INT(pDivBip);
    if (m_Player)
    {
        if (pDivBip *10 >= 99)
        {
            m_beepvol = 95;
        }
        else
        {
            m_beepvol = pDivBip * 10;
        }
        if (pDivGong * 10 >= 99)
        {
            m_liftarvvol = 95;
            m_liftflrvol = 95;
        }
        else
        {
            m_liftarvvol = pDivGong * 10;
            m_liftflrvol = pDivGong * 10;
        }
    }
}

void Uiloader::slot_ArrowAction(bool pArrows_FLM, bool pArrows_FLD, bool pArrows_SCROL)
{
    IDE_TRACE_INT(pArrows_FLM);
    IDE_TRACE_INT(pArrows_FLD);
    IDE_TRACE_INT(pArrows_SCROL);
    if(mArrow)
    {
        if(pArrows_FLM == true && pArrows_FLD == false && pArrows_SCROL == false)
        {
            mArrow->setArrowType(UiArrow::UpArrow);

            mBackLightCtrl.setBacklightValue(m_luminance);
            setScreenSave(false);
            mScreenSaver.exitScreenSaver();
            mHasEnterFlag = false;
        }
        else if(pArrows_FLM == false && pArrows_FLD == true && pArrows_SCROL == false)
        {
            mArrow->setArrowType(UiArrow::DownArrow);

            mBackLightCtrl.setBacklightValue(m_luminance);
            setScreenSave(false);
            mScreenSaver.exitScreenSaver();
            mHasEnterFlag = false;
        }
        else if(pArrows_FLM == true && pArrows_FLD == false && pArrows_SCROL == true)
        {
            mArrow->setArrowType(UiArrow::UpScrollArrow, mScrollSpeed);


            mBackLightCtrl.setBacklightValue(m_luminance);
            setScreenSave(false);
            mScreenSaver.exitScreenSaver();
            mHasEnterFlag = false;
        }
        else if(pArrows_FLM == false && pArrows_FLD == true && pArrows_SCROL == true)
        {
            mArrow->setArrowType(UiArrow::DownScrollArrow, mScrollSpeed);

            mBackLightCtrl.setBacklightValue(m_luminance);
            setScreenSave(false);
            mScreenSaver.exitScreenSaver();
            mHasEnterFlag = false;
        }
        else if(pArrows_FLM == true && pArrows_FLD == true)
        {
            IDE_TRACE();
            mArrow->setArrowType(UiArrow::UpDownArrow);

            mBackLightCtrl.setBacklightValue(m_luminance);
            setScreenSave(false);
            mScreenSaver.exitScreenSaver();
            mHasEnterFlag = false;
        }
        else if(pArrows_FLM == false && pArrows_FLD == false)
        {
            mArrow->setArrowType(UiArrow::NoArrow);
        }
    }
    judgeArrowExist(pArrows_FLM, pArrows_FLD);
}

void Uiloader::slot_FCT_AFF(bool pIsFCT)
{
    IDE_TRACE_INT(pIsFCT);
    if (mUsbIsInsertFlag)
    {
        IDE_DEBUG(QString("Usb is inserting cannot enter other screen!"));
        return;
    }
    if(mIcon)
    {
        mIcon->setBigSrnMode(pIsFCT);
        mBigTextFlag = pIsFCT;
        QtSleep(3000);
        if (mBigTextFlag)
        {
            if (m_Player)
            {
                m_Player->pauseBackgroundPlayer();
                m_Player->setBackgroundPlayerVolume(0);
            }
        }
        else
        {
            if (m_Player)
            {
                m_Player->pauseBackgroundPlayer();
                m_Player->setBackgroundPlayerVolume(mTempSound);
            }
        }
    }
}

void Uiloader::slot_Orient(int pOrientation) //sdo旋转横竖屏
{
    static int horFlag = 0;
    static int vrtFlag = 0;
    IDE_TRACE_INT(pOrientation);
    if (0 == pOrientation)
    {
        ++horFlag;
        if ( 1== horFlag)
        {
            SetLayout(0);
            SetRotate(180);
        }
        else if (2 == horFlag)
        {
            SetLayout(0);
            SetRotate(180);
            horFlag = 0;
        }
//        else if (3 == horFlag)
//        {
//            SetLayout(1);
//            SetRotate(0);
//        }
//        else if (4 == horFlag)
//        {
//            SetLayout(1);
//            SetRotate(180);
//            horFlag = 0;
//        }
    }
    else if (1 == pOrientation)
    {
        ++vrtFlag;
        if (1 == vrtFlag)
        {
            SetLayout(0);
            SetRotate(90);
        }
        else if (2 == vrtFlag)
        {
            SetLayout(0);
            SetRotate(90);
            vrtFlag = 0;
        }
//        else if (3 == vrtFlag)
//        {
//            SetLayout(1);
//            SetRotate(90);
//        }
//        else if (4 == vrtFlag)
//        {
//            SetLayout(1);
//            SetRotate(270);
//            vrtFlag = 0;
//        }
    }
}

void Uiloader::slot_Backlight(int pBacklight) //sdo背光
{
    IDE_TRACE_INT(pBacklight);
    mBackLightCtrl.setBacklightValue(pBacklight);
}

void Uiloader::slot_Vitscrol(int PVit_scrol)  //sdo箭头速度
{
    IDE_TRACE_INT(PVit_scrol);
    if (mScrollSpeed != PVit_scrol)
    {
        mScrollSpeed = PVit_scrol;
        if (mArrow)
        {
            mArrow->changeArrowSpeed(mScrollSpeed);
        }
    }
}

void Uiloader::slot_AFF_EN(bool pIsAFF)
{
    IDE_TRACE_INT(pIsAFF);
    if (pIsAFF)
    {
        mBackLightCtrl.setBacklightValue(mLastBackLight);
    }
    else
    {
        mLastBackLight = mBackLightCtrl.getLightGrade();
        IDE_TRACE_INT(mLastBackLight);
        mBackLightCtrl.setBacklightValue(0);
    }
}

void Uiloader::slot_NodeState(int pState)  //NMT状态
{
    IDE_TRACE_INT(pState);
    QString tmpStr(" ");
    if (3 == pState)
    {
        tmpStr = tmpStr + "Initializing\n" +" Pre-Operational\n" + " Operational\n";
    }
    else if (2 == pState)
    {
        tmpStr = tmpStr + "Initializing\n" +" Pre-Operational\n";
    }
    else if (1 == pState)
    {
        tmpStr = tmpStr + "Initializing\n";
    }
    else if(4 == pState)
    {
        tmpStr = tmpStr + "Initializing\n" +" Pre-Operational\n" + "Stop\n";
    }
    if (m_messageBar)
    {
        if (m_messageBar->isVisible())
        {
            m_messageBar->m_messageLabel->setText(tmpStr);
        }
        else
        {
            m_messageBar->m_messageLabel->setText(tmpStr);
            m_messageBar->show();
            QTimer::singleShot(8000, this, SLOT(updateMessageBar()));
        }
    }

    IDE_TRACE_STR(tmpStr);
}

void Uiloader::setScreenSave(bool pFlag)
{
    IDE_TRACE_STR(QString("is enter setScreenSave"));
    IDE_TRACE_INT(pFlag);
    if (m_screenSaveForm)
    {
        if (!mUsbIsInsertFlag)
        {
            IDE_TRACE_STR(QString("is not insert usb"));
            if(pFlag)
            {
                m_screenSaveForm->showMaximized();
                IDE_TRACE_STR(QString("will set the top"));
            }
            else
            {
                m_screenSaveForm->hide();
                IDE_TRACE_STR(QString("will hide"));
            }
        }
        else
        {
            m_screenSaveForm->hide();
            IDE_TRACE_STR(QString("is insert usb will hide"));
        }
    }
}

void Uiloader::updateMessageBar()
{
    if(m_autotest)
    {
        m_messageBar->hide();
    }
    else if(m_saf)
    {
        m_messageBar->hide();
    }
    else if(m_canopen)
    {
        m_messageBar->hide();
    }
}

void Uiloader::createSaveScreen()
{
    IDE_TRACE_STR(QString("create saverScreen"));
    if (m_screenSaveForm)
    {
        m_screenSaveForm->setWindowFlags(Qt::FramelessWindowHint);
        mView->scene()->addWidget(m_screenSaveForm);
    }
}

void Uiloader::createMessagebar()
{
    if(m_messageBar)
    {
        m_messageBar->setWindowFlags(Qt::FramelessWindowHint | Qt::Widget);
        m_messageBar->setAttribute(Qt::WA_TranslucentBackground);//设置窗口背景透明
        m_messageBar->setStyleSheet("border: 2px solid black;border-radius: 8px;");
        mView->scene()->addWidget(m_messageBar);
        m_messageBar->show();
        QString wifi = wifiNameInfo();
        if (m_saf)
        {
            m_messageBar->m_messageLabel->setText(" Wifi: "+ wifi);
            QTimer::singleShot(5000, this, SLOT(updateMessageBar()));
        }
        else if (m_canopen)
        {
            QString code = QString(" Code[%1]\n").arg(wifi);
            m_messageBar->m_messageLabel->setText(code);
            QTimer::singleShot(5000, this, SLOT(updateMessageBar()));
        }
    }
}

QString Uiloader::wifiNameInfo()
{
    QFile file("/usr/module/hostapd.conf");
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"Can't open the file!"<<endl;
    }
    QByteArray line = file.readAll();
    QString str(line);
    mWifiNameInfo = str.mid(str.indexOf("ssid=")+5,str.indexOf("channel")-str.indexOf("ssid=")-5);
    return mWifiNameInfo;
}

//void Uiloader::slot_readAll()
//{
//    QByteArray tmpArray;
//    tmpArray = mPort->readAll();
//    if(tmpArray.count()!= 5)
//        return;
//    quint8 tmpValue0 = (quint8)tmpArray.at(0);
//    quint8 tmpValue1 = (quint8)tmpArray.at(1);
//    quint8 tmpValue2 = (quint8)tmpArray.at(2);

//    QString tmpHexId= QString::number(tmpValue0, 16);
//    QString tmpHexValue1= QString::number(tmpValue1, 16);
//    QString tmpHexValue2= QString::number(tmpValue2, 16);
//    QString tmpString = QString("%1  %2  %3").arg(tmpHexId, tmpHexValue1, tmpHexValue2);
//    IDE_DEBUG(tmpString);

//    if(tmpValue0 == 1)
//        mArrow->setArrowType(UiArrow::UpArrow);
//    else if(tmpValue0 == 0)
//        mArrow->setArrowType(UiArrow::DownArrow);
//}

void Uiloader::slot_AutoTest()
{
    mAutoTestCount++;

    if(mAutoTestCount < 40)
    {
        SetLayout(m_layout);
        SetRotate(m_rotation);

        //小屏模式
        if(mIcon)
        {
            mIcon->setBigSrnMode(false);
        }

        //显示Floor的代码
        if(mIcon)
            mIcon->setVisible(false);
        if(mFloor)
        {
            mFloor->setVisible(true);
            mFloor->setFloor(mAutoTestCount%10, mAutoTestCount/10, 0); //GE SHI BAI
        }

        //显示Arroow的代码
        if(mArrow)
        {
            mArrow->show();
            mArrow->setArrowType(UiArrow::UpScrollArrow, 5);
        }
    }
    else if(mAutoTestCount < 49)
    {
        SetLayout(0);
        SetRotate(180);

        //小屏模式
        if(mIcon)
        {
            mIcon->setBigSrnMode(false);
        }

        //显示ICON的代码
        if(mFloor)
            mFloor->setVisible(false);
        if(mIcon)
        {
            mIcon->setVisible(true);
            mIcon->setIcon(mAutoTestCount);
        }

        //显示Arrow的代码
        if(mArrow)
        {
            mArrow->show();
            mArrow->setArrowType(UiArrow::DownScrollArrow, 4);
        }
    }
    else if(mAutoTestCount < 65)
    {
        SetLayout(0);
        SetRotate(0);

        //大屏模式
        if(mIcon)
        {
            mIcon->setBigSrnMode(true);
            QString tmpText = QString("%1 %1 %1 %1 %1 %1 %1").arg(mAutoTestCount);
            mIcon->setText(tmpText, tmpText, tmpText, tmpText);
        }
    }
    else if(mAutoTestCount < 80)
    {
        SetLayout(1);
        SetRotate(90);

        //大屏模式
        if(mIcon)
        {
            mIcon->setBigSrnMode(true);
            QString tmpText = QString("%1 %1 %1 %1 %1 %1 %1").arg(mAutoTestCount);
            mIcon->setText(tmpText, tmpText, tmpText, tmpText);
        }
    }
    else if(mAutoTestCount < 92)
    {
        SetLayout(1);
        SetRotate(90);

        //小屏模式
        if(mIcon)
        {
            mIcon->setBigSrnMode(false);
        }

        //显示Arroow的代码
        if(mArrow)
        {
            mArrow->show();
            mArrow->setArrowType(UiArrow::UpScrollArrow, 3);
        }

        //显示Floor的代码
        if(mIcon)
            mIcon->setVisible(false);
        if(mFloor)
        {
            mFloor->setVisible(true);
            mFloor->setFloor(mAutoTestCount%10, mAutoTestCount/10, 0); //GE SHI BAI
        }
    }
    else if(mAutoTestCount < 100)
    {
        SetLayout(1);
        SetRotate(270);

        //小屏模式
        if(mIcon)
        {
            mIcon->setBigSrnMode(false);
        }

        //显示Arroow的代码
        if(mArrow)
        {
            mArrow->show();
            mArrow->setArrowType(UiArrow::UpScrollArrow, 2);
        }

        //显示Floor的代码
        if(mIcon)
            mIcon->setVisible(false);
        if(mFloor)
        {
            mFloor->setVisible(true);
            mFloor->setFloor(mAutoTestCount%10, mAutoTestCount/10, 0); //GE SHI BAI
        }
    }
    else if(mAutoTestCount <= 110)
    {
        SetLayout(0);
        SetRotate(180);
        //小屏模式
        if(mIcon)
        {
            mIcon->setBigSrnMode(false);
        }

        //显示Arrow的代码
        if(mArrow)
        {
            mArrow->show();
            mArrow->setArrowType(UiArrow::UpScrollArrow, 1);
        }

        //显示Floor的代码
        if(mIcon)
            mIcon->setVisible(false);
        if(mFloor)
        {
            mFloor->setVisible(true);
            mFloor->setFloor(mAutoTestCount%10, mAutoTestCount/10%10, mAutoTestCount/100); //GE SHI BAI
        }
    }
    else if(mAutoTestCount > 110)
    {
        mAutoTestCount = 0;
    }
}

void Uiloader::setArrowScrollSpeed(int arrowSpeed)
{
    mScrollSpeed = arrowSpeed;
}

void Uiloader::onSigNoArrow()
{
    IDE_TRACE_STR(QString("will start timer"));
    QTimer::singleShot(m_standby, this, SLOT(onSigEnterScreenSave()));
}

void Uiloader::onSigEnterScreenSave()
{
    IDE_TRACE_INT(mSaverFlag);
    if (mSaverFlag)
    {
        IDE_TRACE_STR(QString("will set screensave atrributte"));
        mScreenSaver.setStandyByFlag(true);
        mScreenSaver.setScreenSaveHoldTime(m_screensavehold);
        mScreenSaver.setScreenSaverTime(m_screensave);
        mScreenSaver.setStandByLuminance(m_luminance);
        mHasEnterFlag = true;
    }
}

void Uiloader::judgeArrowExist(bool up, bool down)
{
    IDE_DEBUG(QString("%1 %2").arg(up).arg(down));
    if (mArrows_FLM != up || mArrows_FLD != down)
    {
        mArrows_FLD = down;
        mArrows_FLM = up;
    }
}

void Uiloader::autoCallOn()
{
//    IDE_TRACE();
    if (!mArrows_FLD && !mArrows_FLM)
    {
        if (!mHasEnterFlag)
        {
//            IDE_TRACE_STR(QString("No Arrow emit sigNoArrow()"));
            mSaverFlag = true;
            emit sigNoArrow();
            mHasEnterFlag = true;
        }
    }
    else
    {
//        IDE_TRACE_STR(QString("have Arrow"));
        mSaverFlag = false;
        if (mHasEnterFlag) // if already enter Screensaver should be exit
        {
            mScreenSaver.exitScreenSaver();
            mHasEnterFlag = false;
        }
    }
}

void Uiloader::slot_NewWifiSocketAdded()
{
    mWifiSocket = mTcpServer.nextPendingConnection();
    connect(mWifiSocket, SIGNAL(readyRead()), this, SLOT(slot_WifiSocketRead()), Qt::UniqueConnection);
    connect(mWifiSocket, SIGNAL(disconnected()), this, SLOT(slot_WifiSocketRemoved()), Qt::UniqueConnection);

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
        mLabelWIFI->setStyleSheet("QLabel{border-radius:3px; background-color:rgba(10, 250, 10, 200); }");
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
        mLabelWIFI->setStyleSheet("QLabel{border-radius:0px; background-color:rgba(10, 250, 10, 0); }");
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
    for(int i = 0; i<tmpFiles.count(); i++)
    {
        QFileInfo tmpFileInfo = tmpFiles.at(i);
        if(tmpFileInfo.isFile())
        {
            {
                tmpFileList.append(tmpFileInfo);
            }
        }
    }
    tmpFileList.append(pDirInfo); //>@包括文件夹，但文件夹没有大小
    return tmpFileList;
}

bool CopyFolder(QString pSrcDir, QString pDstDir, bool pSubfolder)
{
    QFileInfo tmpFileInfo(pSrcDir);
    if(!tmpFileInfo.isDir())
    {
        return false;
    }
    pSrcDir.replace(QString("//"), QString("/"));
    pDstDir.replace(QString("//"), QString("/"));
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
    IDE_TRACE_STR(pName);
    if (!mUsbIsInsertFlag && pName.contains("storage", Qt::CaseInsensitive))
    {
        IDE_TRACE_STR(QString("is usb not wifi"));
        QtSleep(1000);
        mAutoTestTimer.stop();
        setScreenSave(false);
        mUsbIsInsertFlag = true;
        mScreenSaver.exitScreenSaver();
        emit sigUsbInsert();
        IDE_TRACE_STR(QString("usb is insert"));
    }
    else if (!mWifiInsertFlag && pName.contains("net", Qt::CaseInsensitive))
    {
        mWifiInsertFlag = true;
        IDE_TRACE_STR(QString("is wifi not usb"));
        // do nothing Temporarily
        //    显示图标
        if(!mLabelUSB)
        {
            mLabelUSB = new QLabel;
            mLabelUSB->setScaledContents(true);
            mLabelUSB->setPixmap(m_themeParser->m_FileDir + "wifi.png");
        }
        if(mDesktop)
        {
            mLabelUSB->setParent(mDesktop);
            mLabelUSB->setGeometry(mDesktop->width()*3/4, 5, 45, 20);
        }
        mLabelUSB->show();
    }
    //制作U盘拷贝数据传输时的动态效果
//    if(mLabelUSB)
//    {
//        mLabelUSB->setStyleSheet("QLabel{border-radius:10px; background-color:rgba(10, 250, 10, 200); }");
//        QtSleep(10);
//    }

//    CopyFileContent(tmpUDiskPath+"abc.png", "/usr/bst/usrfs/abc.png");
//    CopyFolder(tmpUDiskPath + QString("rc/"), QString("/usr/bst/usrfs/rc/"), true);

//    制作U盘拷贝数据传输时的动态效果
//    if(mLabelUSB)
//    {
//        mLabelUSB->setStyleSheet("QLabel{border-radius:10px; background-color:rgba(10, 250, 10, 0); }");
//        QtSleep(10);
//    }
}

void Uiloader::slot_DeviceRemoved(QString pName)
{
    //隐藏图标
    //显示图标
    if (mUsbIsInsertFlag)
    {
        mUsbIsInsertFlag = false;
        IDE_TRACE_STR(QString("usb is remove, will reboot!"));
        mIcon->setUpdateCompleteFlag(false); // stop while
        QtSleep(2000);
        system("sync");
        QtSleep(3000);
        system("reboot");
    }
    else if (mWifiInsertFlag)
    {
        mWifiInsertFlag = false;
        if(mLabelUSB)
        {
            mLabelUSB->hide();
        }
        QtSleep(3000);
        IDE_TRACE_STR(QString("wifi is remove!"));
    }
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
        mDesktop->setSplashForm(m_screenSaveForm);
        mDesktop->setMessageBar(m_messageBar);

//      mDesktop->setScreenText(m_textScreen);
        return tmpDesktop;
    }
    if(!className.compare("UiIcon", Qt::CaseInsensitive))
    {
        UiIcon* tmpIcon = new UiIcon(parent);
        tmpIcon->setObjectName(name);
        mIcon = tmpIcon;
        IDE_TRACE_STR(QString("new UiIcon"));
        connect(this, &Uiloader::sigUsbInsert, mIcon, &UiIcon::showBackUpScreen, Qt::UniqueConnection);
        connect(this, &Uiloader::sigUsbRemove, mIcon, &UiIcon::setUsbRemoveFlag, Qt::UniqueConnection);
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
        for(int i=0; i<count; i++)
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
        mView->setGeometry(0, 0, 480, 272);

        if(pRotate == 0 || pRotate == 180)
        {
            m_themeDirection = Horizontal;
        }
        else
        {
            m_themeDirection = Vertical;
        }
        mView->rotate(pRotate - m_themeRotate);
        mView->setGeometry(-5, -5, 480+10, 272+10);
        m_themeRotate = pRotate;

        //重新初始化所有控件
        QList<COM_TYPE> tmpComTypeList = m_themeParser->m_ComElementGroup.keys();
        int count = tmpComTypeList.count();
        for(int i=0; i<count; i++)
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
    if(m_mdpParser)
    {
        callOpenMdp();
    }

    m_themeParser = new ThemeParser(pUiPath + "theme.xml");
    if(m_themeParser->openUi())
    {
        QList<COM_TYPE> tmpComTypeList = m_themeParser->m_ComElementGroup.keys();
        int count = tmpComTypeList.count();
        for(int i=0; i<count; i++)
        {
            COM_TYPE tmpType = tmpComTypeList.at(i);

            QDomElement tmpElement = m_themeParser->m_ComElementGroup.value(tmpType);
            if(tmpElement.isNull())
                continue;
            InitComponent(tmpType, tmpElement);
        }
    }

    //LCD
    mBackLightCtrl.setBacklightValue(m_luminance);

    //Player
    m_Player = new Mplayer;
    QString backgroundMusic = backGroundMusicPath + "M0.mp3";
    m_Player->setBackgroundMuisc(backgroundMusic, m_admusicvol); //100是音量
    m_Player->startWork();
    IDE_TRACE_INT(m_admusicvol);
//    m_Player->setBackgroundPlayerVolume(m_admusicvol);

    //autotest
    if(m_autotest)
    {
        //Theme
        SetLayout(m_layout);
        SetRotate(m_rotation);
        slot_AutoTest();
        mAutoTestTimer.setInterval(m_testfreq);
        mAutoTestTimer.start();
    }
    else
    {
        //屏保
        connect(this, SIGNAL(sigNoArrow()), this, SLOT(onSigNoArrow()));
        connect(&mScreenSaverTimers, SIGNAL(timeout()), this, SLOT(autoCallOn()));  // 启动后无任何操作主动进入屏保模式
        mScreenSaverTimers.start(2000);
#ifdef LINUX
        //saf
//        initSafProtocol();
        //canopen
        initCanOpenProtocol();
#endif
        //Theme
        SetLayout(m_layout);
        SetRotate(m_rotation);
    }
    return tmpWidget;
}

void Uiloader::initCanOpenProtocol()
{
    m_canopen = CanOpenClientFunction::getInstace();
    if (!m_canopen)
    {
        IDE_TRACE_STR(QString("new CanOpenClientFunction error !"));
        return;
    }
    else
    {
        IDE_TRACE_STR(QString("canopen protocol connect first"));
        connect(m_canopen, SIGNAL(sigNodeState(int)), this, SLOT(slot_NodeState(int))); // NMT状态
        connect(m_canopen, SIGNAL(sigOrientation(int)), this, SLOT(slot_Orient(int)));  // sdo旋转横竖屏
        connect(m_canopen, SIGNAL(sigBacklight(int)), this, SLOT(slot_Backlight(int))); // sdo背光
        connect(m_canopen, SIGNAL(sigVit_scrol(int)), this, SLOT(slot_Vitscrol(int)));  // sdo箭头速度

        connect(m_canopen, SIGNAL(sigAFF_EN(bool)), this, SLOT(slot_AFF_EN(bool)));     // backlight 使能开关
        connect(m_canopen, SIGNAL(sigFCT_AFF(bool)), this, SLOT(slot_FCT_AFF(bool)));   // 大小屏
        connect(m_canopen, SIGNAL(sigArrowAction(bool, bool, bool)), this, SLOT(slot_ArrowAction(bool, bool, bool))); // 上下箭头滚动
        connect(m_canopen, SIGNAL(sigMainVolumeChange(int, int)), this, SLOT(slot_CanopenMainVolume(int, int))); // gong,beep
        connect(m_canopen, SIGNAL(sigMusicVolumeChange(int)), this, SLOT(slot_CanopenMusicVolume(int))); // 背景音
        connect(m_canopen, SIGNAL(sigBuzz(int)), this, SLOT(slot_CanopenBuzz(int)));
        connect(m_canopen, SIGNAL(sigBuzz()), this, SLOT(slot_CanopenshortBuzz()));
        connect(m_canopen, SIGNAL(sigFloorArrive(int, int, int, int)), this, SLOT(slot_CanopenFloorArrive(int, int, int, int))); // 个 十，百，声音
        connect(m_canopen, SIGNAL(sigShowFuncLogo(int, int)), this, SLOT(slot_ShowFuncLogo(int, int))); // 功能图标 声音
        connect(m_canopen, SIGNAL(sigShowFloorOrLogo(bool)), this, SLOT(slot_ShowFloorOrLogo(bool))); // 是否显示logo
        connect(m_canopen, SIGNAL(sigMessage(QString, QString, QString, QString)), this, SLOT(slot_Message(QString, QString, QString, QString))); // 4个文本
        connect(m_canopen, SIGNAL(sigRealMessage(QString, QString)), this, SLOT(slot_RealMessage(QString, QString)));
    }
}

void Uiloader::initSafProtocol()
{
    m_saf = new SafThread(this);
    m_saf->m_openingMessage = m_safOpenningMessage;
    connect(m_saf, SIGNAL(sigArrowAction(bool, bool, bool)), this, SLOT(slot_ArrowAction(bool, bool, bool))); // 上下箭头滚动
    connect(m_saf, SIGNAL(sigMainVolumeChange(int, int)), this, SLOT(slot_MainVolume(int, int))); // gong,beep
    connect(m_saf, SIGNAL(sigFloorArrive(int, int, int, int)), this, SLOT(slot_FloorArrive(int, int, int, int))); // 个十，百，声音
    connect(m_saf, SIGNAL(sigShowFuncLogo(int, int)), this, SLOT(slot_ShowFuncLogo(int, int))); // 功能图标 声音
    connect(m_saf, SIGNAL(sigVit_scrol(int)), this, SLOT(slot_Vitscrol(int))); //sdo箭头速度
    connect(m_saf, SIGNAL(switchValue(int,int,int,int)), this, SLOT(slot_switchValue(int, int, int, int))); //switch
    m_saf->start();
}

void Uiloader::callOpenMdp()
{
    if(m_mdpParser->openMdp())
    {
        QDomElement tmpPlayerElement = m_mdpParser->m_PHElementGroup.value(P_Player);
        if(!tmpPlayerElement.isNull())
        {
            QDomElement tmpElement = tmpPlayerElement.firstChildElement("admusicvol");
            if(!tmpElement.isNull())
                m_admusicvol = tmpElement.text().toInt();
            if(m_admusicvol >= 100)
            {
                m_admusicvol = 99;
            }
            else if (m_admusicvol < 0)
            {
                m_admusicvol = 0;
            }
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
                m_rotation = tmpElement.text().toInt() + 180; // 以宽边为上视角
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

            tmpElement = tmpComElement.firstChildElement("safOpenMessage");
            IDE_TRACE_STR(tmpElement.text());
            if(!tmpElement.isNull())
                m_safOpenningMessage = (bool)(!tmpElement.text().compare("true"));
        }

        QDomElement tmpThemeElement = m_mdpParser->m_LGElementGroup.value(L_ThemeManager);
        if(!tmpThemeElement.isNull())
        {
            QDomElement tmpElement = tmpThemeElement.firstChildElement("layout");
            if(!tmpElement.isNull())
                m_layout = tmpElement.text().toInt();
        }
    }
}
