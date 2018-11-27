#include "mainwindow.h"
#include <QTimer>
#include <QDebug>

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

    m_saf = 0;
    m_canopen = 0;

    mLabelWIFI = 0;

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
    mbackgroundset_0 = false;
    mXmlFileChangeFlag = false;
    mProtocolType = QString();
    mLoginPasswd = QString();
    mFtpSendFileName = QString();
    mFtpDownLoadFileName = QString();
    mTempSound =  -1;
    mTcpPort = 60001;
    mZipConcernProgress = 0;

    connect(&mScreenSaver, &ScreenSaver::sigSetBlackPicture, this, &Uiloader::setScreenSave, Qt::UniqueConnection);

    m_screenSaveForm = new SplashForm;
    if (m_screenSaveForm)
    {
        m_screenSaveForm->hide(); // first should hide the screen
    }
    m_messageBar = new MessageBar;
    if (m_messageBar)
    {
        m_messageBar->hide();
    }

    qtimer_m_standby.setSingleShot(true);
    connect(&qtimer_m_standby,SIGNAL(timeout()),this,SLOT(onSigEnterScreenSave()));

    connect(&mZipFunction, &::ZipFunction::zipProgress, this, &Uiloader::sltZipProgress, Qt::QueuedConnection);
    connect(&mZipFunction, &::ZipFunction::unZipProgress, this, &Uiloader::sltUnZipProgress, Qt::QueuedConnection);
    connect(&mZipFunction, &::ZipFunction::end, this, &Uiloader::sltZipOrUnZipEnded, Qt::QueuedConnection);
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
        SetLayout(0);
        SetRotate(270);
    }
    if(s1 == 1 && s2 == 0)
    {
        SetLayout(0);
        SetRotate(180);
    }
    if(s1 == 0 && s2 == 0)
    {
        SetLayout(0);
        SetRotate(90);
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
        if (m_Player)
        {
            m_Player->pauseBackgroundPlayer();
            m_Player->setBackgroundPlayerVolume(m_admusicvol/2);
            m_Player->playBackgroundPlayer();
        }
        m_admusicvol /= 2;
        m_beepvol /= 2;
        m_liftarvvol /= 2;
        m_liftflrvol /= 2;
    }
    if(s3 == 0 && s4 == 1)
    {
        if (m_Player)
        {
            m_Player->pauseBackgroundPlayer();
            m_Player->setBackgroundPlayerVolume(m_admusicvol/3);
            m_Player->playBackgroundPlayer();
        }
        m_admusicvol /= 3;
        m_beepvol /= 3;
        m_liftarvvol /= 3;
        m_liftflrvol /= 3;
    }
    if(s3 == 0 && s4 == 0)
    {
        if (m_Player)
        {
            m_Player->pauseBackgroundPlayer();
            //m_Player->setBackgroundPlayerVolume(0);//zzf
            mbackgroundset_0 = true;
            //m_Player->playBackgroundPlayer();
        }
        m_admusicvol = 0;
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
                QString overloadMusic = MediaPath + QString("M%1.wav").arg(pSoundNumber);
                m_Player->addMedia(AO_BEEP, overloadMusic, 1, false, m_beepvol) ;
            }
        }
    }
}

void Uiloader::slot_FloorArrive(int pGe, int pShi, int pBai, int pSoundNumber, int safFrame)
{
    IDE_TRACE_INT(pGe);
    IDE_TRACE_INT(pShi);
    IDE_TRACE_INT(pBai);
    IDE_TRACE_INT(pSoundNumber);

    if (mBigTextFlag) // if bigtext will return
    {
        return;
    }
    if (mUsbIsInsertFlag) // if usb is inserted, return
    {
        return;
    }

    if(mFloor)
    {

        if(mIcon)
        {
            mIcon->setVisible(false);
        }
        if(mFloor)
        {
            mFloor->setVisible(true);
            mFloor->setFloor(pGe, pShi, pBai, safFrame);
            if(pSoundNumber != -1)
            {
                QString floorMusic = MediaPath + QString("M%1.wav").arg(pSoundNumber);
                // floor map
                QString mapMedia = mFloor->setMapMedia(safFrame);
                if(mapMedia != "false")
                {
                    floorMusic = mapMedia;
                }
                if (m_Player)
                {
                    m_Player->addMedia(AO_LIFTFLR, floorMusic, 1, false, m_liftflrvol) ;
                }
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
                    QString floorMusic = MediaPath + QString("M%1.wav").arg(mMusicNumber);
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
        m_Player->addMedia(AO_BEEP, beepMusic, 1, false, m_beepvol) ;
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
    {
        QString floorMusic = MediaPath + QString("M%1.wav").arg(pDivGong);
        if (m_Player)
        {
            m_Player->addMedia(AO_LIFTARV, floorMusic, 1, false, m_liftarvvol);
        }
    }
    if(pDivBip != -1)
    {
        QString floorMusic = MediaPath + QString("M%1.wav").arg(pDivBip);
        if (m_Player)
        {
            m_Player->addMedia(AO_BEEP, floorMusic, 1, false, m_beepvol);
        }
    }
}

void Uiloader::slot_Frame60Loop(int music, int picture)
{
    if(music == 43 && picture == 45)
    {
        QString floorMusic = MediaPath + QString("M43.wav");
        if (m_Player)
        {
            m_Player->addMedia(AO_BEEP, floorMusic, -1, false, m_beepvol);
        }
        if(mIcon)
        {
            mIcon->setVisible(true);
            mIcon->setIcon(picture);
        }
    }
    else if(music == 0 && picture == 0)
    {
        if (m_Player)
        {
            m_Player->clearMediaBuffer();
        }
    }
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
            //m_Player->setBackgroundPlayerVolume(m_admusicvol);
            //m_Player->playBackgroundPlayer();
            setbackground_0();
            if(qtimer_m_standby.isActive())
            {
                qtimer_m_standby.stop();
            }
            mHasEnterFlag = false;
        }
        else if(pArrows_FLM == false && pArrows_FLD == true && pArrows_SCROL == false)
        {
            mArrow->setArrowType(UiArrow::DownArrow);

            mBackLightCtrl.setBacklightValue(m_luminance);
            setScreenSave(false);
            mScreenSaver.exitScreenSaver();
            //m_Player->setBackgroundPlayerVolume(m_admusicvol);
            //m_Player->playBackgroundPlayer();
            setbackground_0();
            if(qtimer_m_standby.isActive())
            {
                qtimer_m_standby.stop();
            }
            mHasEnterFlag = false;
        }
        else if(pArrows_FLM == true && pArrows_FLD == false && pArrows_SCROL == true)
        {
            mArrow->setArrowType(UiArrow::UpScrollArrow, mScrollSpeed);


            mBackLightCtrl.setBacklightValue(m_luminance);
            setScreenSave(false);
            mScreenSaver.exitScreenSaver();
            //m_Player->setBackgroundPlayerVolume(m_admusicvol);
            //m_Player->playBackgroundPlayer();
            setbackground_0();
            if(qtimer_m_standby.isActive())
            {
                qtimer_m_standby.stop();
            }
            mHasEnterFlag = false;
        }
        else if(pArrows_FLM == false && pArrows_FLD == true && pArrows_SCROL == true)
        {
            mArrow->setArrowType(UiArrow::DownScrollArrow, mScrollSpeed);

            mBackLightCtrl.setBacklightValue(m_luminance);
            setScreenSave(false);
            mScreenSaver.exitScreenSaver();
            //m_Player->setBackgroundPlayerVolume(m_admusicvol);
            //m_Player->playBackgroundPlayer();
            setbackground_0();
            if(qtimer_m_standby.isActive())
            {
                qtimer_m_standby.stop();
            }
            mHasEnterFlag = false;
        }
        else if(pArrows_FLM == true && pArrows_FLD == true)
        {
            IDE_TRACE();
            mArrow->setArrowType(UiArrow::UpDownArrow);

            mBackLightCtrl.setBacklightValue(m_luminance);
            setScreenSave(false);
            mScreenSaver.exitScreenSaver();
            //m_Player->setBackgroundPlayerVolume(m_admusicvol);
            //m_Player->playBackgroundPlayer();
            setbackground_0();
            if(qtimer_m_standby.isActive())
            {
                qtimer_m_standby.stop();
            }
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

void Uiloader::slot_Orient(int pOrientation)
{
    static int horFlag = 0;
    static int vrtFlag = 0;
    IDE_TRACE_INT(pOrientation);
    if (0 == pOrientation)
    {
        ++horFlag;
        if (1 == horFlag)
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

void Uiloader::slot_Backlight(int pBacklight)
{
    IDE_TRACE_INT(pBacklight);
    mBackLightCtrl.setBacklightValue(pBacklight);
}

void Uiloader::slot_Vitscrol(int PVit_scrol)
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

void Uiloader::slot_NodeState(int pState)
{
    IDE_TRACE_INT(pState);
    QString tmpStr(" ");
    if (3 == pState)
    {
        tmpStr = tmpStr + "Initializing\n" +" Pre-Operational\n" + " Operational\n";
        mStatusCanopenProtocol = tmpStr;
    }
    else if (2 == pState)
    {
        tmpStr = tmpStr + "Initializing\n" +" Pre-Operational\n";
        mStatusCanopenProtocol = tmpStr;
    }
    else if (1 == pState)
    {
        tmpStr = tmpStr + "Initializing\n";
        mStatusCanopenProtocol = tmpStr;
    }
    else if(4 == pState)
    {
        tmpStr = tmpStr + "Initializing\n" +" Pre-Operational\n" + "Stop\n";
        mStatusCanopenProtocol = tmpStr;
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
        m_messageBar->setAttribute(Qt::WA_TranslucentBackground);
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
    mWifiNameInfo = str.mid(str.indexOf("ssid=")+5, str.indexOf("channel")-str.indexOf("ssid=") - 5);
    return mWifiNameInfo;
}

void Uiloader::slot_AutoTest()
{
    mAutoTestCount++;

    if(mAutoTestCount < 40)
    {
        SetLayout(m_layout);
        SetRotate(m_rotation);

        if(mIcon)
        {
            mIcon->setBigSrnMode(false);
        }

        if(mIcon)
        {
            mIcon->setVisible(false);
        }
        if(mFloor)
        {
            mFloor->setVisible(true);
            mFloor->setFloor(mAutoTestCount%10, mAutoTestCount/10, 0); //GE SHI BAI
        }


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

        if(mIcon)
        {
            mIcon->setBigSrnMode(false);
        }

        if(mFloor)
        {
            mFloor->setVisible(false);
        }
        if(mIcon)
        {
            mIcon->setVisible(true);
            mIcon->setIcon(mAutoTestCount);
        }

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

        if(mIcon)
        {
            mIcon->setBigSrnMode(true);
            QString tmpText = QString("%1 %1 %1 %1 %1").arg(mAutoTestCount);
            mIcon->setText(tmpText, tmpText, tmpText, tmpText);
        }
    }
    else if(mAutoTestCount < 80)
    {
        SetLayout(1);
        SetRotate(90);

        if(mIcon)
        {
            mIcon->setBigSrnMode(true);
            QString tmpText = QString("%1 %1 %1 %1 %1").arg(mAutoTestCount);
            mIcon->setText(tmpText, tmpText, tmpText, tmpText);
        }
    }
    else if(mAutoTestCount < 92)
    {
        SetLayout(1);
        SetRotate(90);

        if(mIcon)
        {
            mIcon->setBigSrnMode(false);
        }

        if(mArrow)
        {
            mArrow->show();
            mArrow->setArrowType(UiArrow::UpScrollArrow, 3);
        }

        if(mIcon)
        {
            mIcon->setVisible(false);
        }
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

        if(mIcon)
        {
            mIcon->setBigSrnMode(false);
        }

        if(mArrow)
        {
            mArrow->show();
            mArrow->setArrowType(UiArrow::UpScrollArrow, 2);
        }

        if(mIcon)
        {
            mIcon->setVisible(false);
        }
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

        if(mIcon)
        {
            mIcon->setBigSrnMode(false);
        }

        if(mArrow)
        {
            mArrow->show();
            mArrow->setArrowType(UiArrow::UpScrollArrow, 1);
        }

        if(mIcon)
        {
            mIcon->setVisible(false);
        }
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
    //QTimer::singleShot(m_standby, this, SLOT(onSigEnterScreenSave()));
    qtimer_m_standby.start(m_standby);
}

void Uiloader::onSigEnterScreenSave()
{
    IDE_TRACE_INT(mSaverFlag);
    if (m_Player)
    {
        m_Player->pauseBackgroundPlayer();
        m_Player->setBackgroundPlayerVolume(0);//zzf
    }
    //m_Player->clearMediaBuffer();
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
            if (m_Player)
            {
                //m_Player->setBackgroundPlayerVolume(m_admusicvol);
                //m_Player->playBackgroundPlayer();
                setbackground_0();
            }
            if(qtimer_m_standby.isActive())
            {
                qtimer_m_standby.stop();
            }
            mHasEnterFlag = false;
        }
    }
}

void Uiloader::slot_NewWifiSocketAdded()
{
//    mWifiSocket = mTcpServer.nextPendingConnection();
//    connect(mWifiSocket, SIGNAL(readyRead()), this, SLOT(slot_WifiSocketRead()), Qt::UniqueConnection);
//    connect(mWifiSocket, SIGNAL(disconnected()), this, SLOT(slot_WifiSocketRemoved()), Qt::UniqueConnection);
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
//    mWifiSocket = 0;
    if(mLabelWIFI)
    {
        mLabelWIFI->hide();
    }
}

void Uiloader::slot_WifiSocketRead()
{
    if(mLabelWIFI)
    {
        mLabelWIFI->setStyleSheet("QLabel{border-radius:3px; background-color:rgba(10, 250, 10, 200); }");
        mWifiBusyTimer.start();
    }

//    QByteArray tmpArray = mWifiSocket->readAll();
    IDE_TRACE_STR(QString(tmpArray));
}

void Uiloader::slot_WifiBusyStop()
{
    if(mLabelWIFI)
    {
        mLabelWIFI->setStyleSheet("QLabel{border-radius:0px; background-color:rgba(10, 250, 10, 0); }");
    }
}

bool CopyFileContent(QString srcFile, QString dstFile)
{
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
    tmpFileList.append(pDirInfo);
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

    if(pSrcDir.compare(pDstDir) == 0)
        return true;


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

        //m_Player->setBackgroundPlayerVolume(m_admusicvol);
        //m_Player->playBackgroundPlayer();
//        setbackground_0();
        if(qtimer_m_standby.isActive())
        {
            qtimer_m_standby.stop();
        }
        emit sigUsbInsert();
        IDE_TRACE_STR(QString("usb is insert"));
    }
    else if (!mWifiInsertFlag && pName.contains("net", Qt::CaseInsensitive))
    {
        mWifiInsertFlag = true;
        IDE_TRACE_STR(QString("is wifi not usb"));
        // do nothing Temporarily

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

    //    if(mLabelUSB)
    //    {
    //        mLabelUSB->setStyleSheet("QLabel{border-radius:10px; background-color:rgba(10, 250, 10, 200); }");
    //        QtSleep(10);
    //    }

    //    CopyFileContent(tmpUDiskPath+"abc.png", "/usr/bst/usrfs/abc.png");
    //    CopyFolder(tmpUDiskPath + QString("rc/"), QString("/usr/bst/usrfs/rc/"), true);


    //    if(mLabelUSB)
    //    {
    //        mLabelUSB->setStyleSheet("QLabel{border-radius:10px; background-color:rgba(10, 250, 10, 0); }");
    //        QtSleep(10);
    //    }
}

void Uiloader::slot_DeviceRemoved(QString pName)
{
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
        if (!mZipConcernProgress)
        {
            mZipConcernProgress = new QProgressBar;
            if (mZipConcernProgress)
            {
                if (mDesktop)
                {
                    mZipConcernProgress->setParent(mDesktop);
                }
                mZipConcernProgress->hide();
            }
        }
        return tmpDesktop;
    }
    if(!className.compare("UiIcon", Qt::CaseInsensitive))
    {
        UiIcon* tmpIcon = new UiIcon(parent);
        tmpIcon->setObjectName(name);
        mIcon = tmpIcon;
        IDE_TRACE_STR(QString("new UiIcon"));
        if (mIcon)
        {
            connect(this, &Uiloader::sigUsbInsert, mIcon, &UiIcon::showBackUpScreen, Qt::UniqueConnection);
            connect(this, &Uiloader::sigUsbRemove, mIcon, &UiIcon::setUsbRemoveFlag, Qt::UniqueConnection);
        }
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
    else if(pType == COM_MEDIA && mFloor)
    {
        mFloor->initAudio(pElement);
    }
}

void Uiloader::SetLayout(int pLayout)
{
    if(m_themeLayout != pLayout)
    {
        m_themeLayout = pLayout;

        QList<COM_TYPE> tmpComTypeList = m_themeParser->m_ComElementGroup.keys();
        int count = tmpComTypeList.count();
        for(int i = 0; i < count; ++i)
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

        if(pRotate == 0 || pRotate == 180 || pRotate == 360)
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

        QList<COM_TYPE> tmpComTypeList = m_themeParser->m_ComElementGroup.keys();
        int count = tmpComTypeList.count();
        for(int i = 0; i < count; ++i)
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
    mRunPath = pUiPath;
    QWidget* tmpWidget = 0;
    QString tmpRccStr = pUiPath + "Theme.rcc";
    QResource::registerResource(tmpRccStr);

    QString tmpUiStr = pUiPath + "Theme.ui";
    QFile tmpFile(tmpUiStr);

    if(!tmpFile.open(QFile::ReadOnly))
    {
        return tmpWidget;
    }
    tmpWidget = load(&tmpFile);

    m_mdpParser = new MdpParser(pUiPath + "device.xml");
    qDebug()<<__PRETTY_FUNCTION__<<"after new MdpParser";
    if(m_mdpParser)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"before new callOpenMdp()";
        callOpenMdp();
    }

    m_themeParser = new ThemeParser(pUiPath + "theme.xml");
    if(m_themeParser->openUi())
    {
        QList<COM_TYPE> tmpComTypeList = m_themeParser->m_ComElementGroup.keys();
        int count = tmpComTypeList.count();
        for(int i = 0; i < count; ++i)
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
    QString backgroundMusic = backGroundMusicPath + "M0.wav";
    if (m_Player)
    {
        m_Player->setBackgroundMuisc(backgroundMusic, m_admusicvol);
        m_Player->startWork();
    }
    IDE_TRACE_INT(m_admusicvol);
    //m_Player->setBackgroundPlayerVolume(m_admusicvol);

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
        connect(this, SIGNAL(sigNoArrow()), this, SLOT(onSigNoArrow()));
        connect(&mScreenSaverTimers, SIGNAL(timeout()), this, SLOT(autoCallOn()));
        mScreenSaverTimers.start(2000);
#ifdef LINUX
        if (!QString::compare("canopen", mProtocolType, Qt::CaseInsensitive))
        {
            initCanOpenProtocol();
        }
        else if (!QString::compare("saf", mProtocolType, Qt::CaseInsensitive))
        {
            initSafProtocol();
        }
        else
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"protocolType is not legal";
        }
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
        disconncetCanOpenSignal(false);
    }
}

void Uiloader::initSafProtocol()
{
    m_saf = new SafThread(this);
    m_saf->m_openingMessage = m_safOpenningMessage;
    connect(m_saf, SIGNAL(sigArrowAction(bool, bool, bool)), this, SLOT(slot_ArrowAction(bool, bool, bool)));
    connect(m_saf, SIGNAL(sigMainVolumeChange(int, int)), this, SLOT(slot_MainVolume(int, int))); // gong,beep
    connect(m_saf, SIGNAL(sigFloorArrive(int, int, int, int, int)), this, SLOT(slot_FloorArrive(int, int, int, int, int))); // connect(m_saf, SIGNAL(sigFloorArrive(int, int, int, int)), this, SLOT(slot_FloorArrive(int, int, int, int)));    connect(m_saf, SIGNAL(sigShowFuncLogo(int, int)), this, SLOT(slot_ShowFuncLogo(int, int))); // ����ͼ�� ����
    connect(m_saf, SIGNAL(sigVit_scrol(int)), this, SLOT(slot_Vitscrol(int)));
    connect(m_saf, SIGNAL(switchValue(int,int,int,int)), this, SLOT(slot_switchValue(int, int, int, int))); //switch
    m_saf->start();
}

void Uiloader::callOpenMdp()
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
    if(m_mdpParser->openMdp())
    {
        mLoginPasswd = m_mdpParser->getPassword();
        mProtocolType = m_mdpParser->getProtocolType();
        QDomElement tmpPlayerElement = m_mdpParser->m_PHElementGroup.value(P_Player);
        if(!tmpPlayerElement.isNull())
        {
            QDomElement tmpElement = tmpPlayerElement.firstChildElement("admusicvol");
            if(!tmpElement.isNull())
            {
                m_admusicvol = tmpElement.text().toInt();
            }
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
            {
                m_beepvol = tmpElement.text().toInt();
            }
            tmpElement = tmpPlayerElement.firstChildElement("liftarvvol");
            if(!tmpElement.isNull())
            {
                m_liftarvvol = tmpElement.text().toInt();
            }
            tmpElement = tmpPlayerElement.firstChildElement("liftflrvol");
            if(!tmpElement.isNull())
            {
                m_liftflrvol = tmpElement.text().toInt();
            }
        }

        QDomElement tmpLedElement = m_mdpParser->m_PHElementGroup.value(P_LED);
        if(!tmpLedElement.isNull())
        {
            QDomElement tmpElement = tmpLedElement.firstChildElement("normal");
            if(!tmpElement.isNull())
            {
                m_lednormalFreq = tmpElement.text();
            }
            tmpElement = tmpLedElement.firstChildElement("busy");
            if(!tmpElement.isNull())
            {
                m_ledbusyFreq = tmpElement.text();
            }
            tmpElement = tmpLedElement.firstChildElement("error");
            if(!tmpElement.isNull())
            {
                m_lederrorFreq = tmpElement.text();
            }
        }

        QDomElement tmpLcdElement = m_mdpParser->m_PHElementGroup.value(P_LCD);
        if(!tmpLcdElement.isNull())
        {
            QDomElement tmpElement = tmpLcdElement.firstChildElement("rotation");
            if(!tmpElement.isNull())
            {
                m_rotation = tmpElement.text().toInt() + 180;
            }
            tmpElement = tmpLcdElement.firstChildElement("luminance");
            if(!tmpElement.isNull())
            {
                m_luminance = tmpElement.text().toInt();
            }
            tmpElement = tmpLcdElement.firstChildElement("standby");
            if(!tmpElement.isNull())
            {
                m_standby = tmpElement.text().toInt();
            }
            tmpElement = tmpLcdElement.firstChildElement("standbyluminance");
            if(!tmpElement.isNull())
            {
                m_standbyluminance = tmpElement.text().toInt();
            }
            tmpElement = tmpLcdElement.firstChildElement("screensave");
            if(!tmpElement.isNull())
            {
                m_screensave = tmpElement.text().toInt();
            }
            tmpElement = tmpLcdElement.firstChildElement("screensavehold");
            if(!tmpElement.isNull())
            {
                m_screensavehold = tmpElement.text().toInt();
            }
        }

        QDomElement tmpComElement = m_mdpParser->m_PHElementGroup.value(P_COM);
        if(!tmpComElement.isNull())
        {
            QDomElement tmpElement = tmpComElement.firstChildElement("autotest");
            if(!tmpElement.isNull())
            {
                m_autotest = (bool)(!tmpElement.text().compare("true"));
            }
            tmpElement = tmpComElement.firstChildElement("testfreq");
            if(!tmpElement.isNull())
            {
                m_testfreq = tmpElement.text().toInt();
            }

            tmpElement = tmpComElement.firstChildElement("safOpenMessage");
            IDE_TRACE_STR(tmpElement.text());
            if(!tmpElement.isNull())
            {
                m_safOpenningMessage = (bool)(!tmpElement.text().compare("true"));
            }
        }

        QDomElement tmpThemeElement = m_mdpParser->m_LGElementGroup.value(L_ThemeManager);
        if(!tmpThemeElement.isNull())
        {
            QDomElement tmpElement = tmpThemeElement.firstChildElement("layout");
            if(!tmpElement.isNull())
            {
                m_layout = tmpElement.text().toInt();
            }
        }

        QDomElement tmpLancElement = m_mdpParser->m_PHElementGroup.value(P_LANC);
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
        if(!tmpLancElement.isNull())
        {
            QDomElement tmpElement = tmpLancElement.firstChildElement("tcp");
            if(!tmpElement.isNull())
            {
                mTcpPort = tmpElement.text().toInt();
                qDebug()<<"mTcpPort = "<<mTcpPort;
                mWiFi = new WifiCommunication(mTcpPort);
                initWifi();
            }
        }
    }
}

void Uiloader::setbackground_0()
{
    if(!mbackgroundset_0)
    {
        if (m_Player)
        {
            m_Player->setBackgroundPlayerVolume(m_admusicvol);
            m_Player->playBackgroundPlayer();
        }
    }
}

/*-----------------wifi start------------------------- */

void Uiloader::initWifi()
{
    if (mWiFi)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"is call";
        connect(mWiFi, &WifiCommunication::sigLogin, this, &Uiloader::sltAppLogin);
        connect(mWiFi, &WifiCommunication::sigDeviceConnected, this, &Uiloader::sltDeviceConnected);
        connect(mWiFi, &WifiCommunication::sigDeviceDisconnected, this, &Uiloader::sltDeviceDisConnected);
        connect(mWiFi, &WifiCommunication::sigGetAllParametere, this, &Uiloader::sltGetAllParameter);
        connect(mWiFi, &WifiCommunication::sigUpdateFinished, this, &Uiloader::sltUpdateFinished);
        connect(mWiFi, &WifiCommunication::sigParameterSetUp, this, &Uiloader::sltParameterSetUp);        
        connect(mWiFi, &WifiCommunication::sigSendFile, this, &Uiloader::sltSendFille);
        connect(mWiFi, &WifiCommunication::sigDownLoad, this, &Uiloader::sltDownLoad);
    }
}

void Uiloader::disconncetCanOpenSignal(bool pAutoTest)
{
    if (pAutoTest)
    {
        if (m_canopen)
        {
            qDebug()<<__PRETTY_FUNCTION__<<"canopen protocol disconnect";
            disconnect(m_canopen, SIGNAL(sigNodeState(int)), this, SLOT(slot_NodeState(int)));
            disconnect(m_canopen, SIGNAL(sigOrientation(int)), this, SLOT(slot_Orient(int)));
            disconnect(m_canopen, SIGNAL(sigBacklight(int)), this, SLOT(slot_Backlight(int)));
            disconnect(m_canopen, SIGNAL(sigVit_scrol(int)), this, SLOT(slot_Vitscrol(int)));

            disconnect(m_canopen, SIGNAL(sigAFF_EN(bool)), this, SLOT(slot_AFF_EN(bool)));
            disconnect(m_canopen, SIGNAL(sigFCT_AFF(bool)), this, SLOT(slot_FCT_AFF(bool)));
            disconnect(m_canopen, SIGNAL(sigArrowAction(bool, bool, bool)), this, SLOT(slot_ArrowAction(bool, bool, bool)));
            disconnect(m_canopen, SIGNAL(sigMainVolumeChange(int, int)), this, SLOT(slot_CanopenMainVolume(int, int)));
            disconnect(m_canopen, SIGNAL(sigMusicVolumeChange(int)), this, SLOT(slot_CanopenMusicVolume(int)));
            disconnect(m_canopen, SIGNAL(sigBuzz(int)), this, SLOT(slot_CanopenBuzz(int)));
            disconnect(m_canopen, SIGNAL(sigBuzz()), this, SLOT(slot_CanopenshortBuzz()));
            disconnect(m_canopen, SIGNAL(sigFloorArrive(int, int, int, int)), this, SLOT(slot_CanopenFloorArrive(int, int, int, int)));
            disconnect(m_canopen, SIGNAL(sigShowFuncLogo(int, int)), this, SLOT(slot_ShowFuncLogo(int, int)));
            disconnect(m_canopen, SIGNAL(sigShowFloorOrLogo(bool)), this, SLOT(slot_ShowFloorOrLogo(bool)));
            disconnect(m_canopen, SIGNAL(sigMessage(QString, QString, QString, QString)), this, SLOT(slot_Message(QString, QString, QString, QString)));
            disconnect(m_canopen, SIGNAL(sigRealMessage(QString, QString)), this, SLOT(slot_RealMessage(QString, QString)));
        }
    }
    else
    {
        if (m_canopen)
        {
            qDebug()<<__PRETTY_FUNCTION__<<"canopen protocol connect";
            connect(m_canopen, SIGNAL(sigNodeState(int)), this, SLOT(slot_NodeState(int)));
            connect(m_canopen, SIGNAL(sigOrientation(int)), this, SLOT(slot_Orient(int)));
            connect(m_canopen, SIGNAL(sigBacklight(int)), this, SLOT(slot_Backlight(int)));
            connect(m_canopen, SIGNAL(sigVit_scrol(int)), this, SLOT(slot_Vitscrol(int)));

            connect(m_canopen, SIGNAL(sigAFF_EN(bool)), this, SLOT(slot_AFF_EN(bool)));
            connect(m_canopen, SIGNAL(sigFCT_AFF(bool)), this, SLOT(slot_FCT_AFF(bool)));
            connect(m_canopen, SIGNAL(sigArrowAction(bool, bool, bool)), this, SLOT(slot_ArrowAction(bool, bool, bool)));
            connect(m_canopen, SIGNAL(sigMainVolumeChange(int, int)), this, SLOT(slot_CanopenMainVolume(int, int)));
            connect(m_canopen, SIGNAL(sigMusicVolumeChange(int)), this, SLOT(slot_CanopenMusicVolume(int)));
            connect(m_canopen, SIGNAL(sigBuzz(int)), this, SLOT(slot_CanopenBuzz(int)));
            connect(m_canopen, SIGNAL(sigBuzz()), this, SLOT(slot_CanopenshortBuzz()));
            connect(m_canopen, SIGNAL(sigFloorArrive(int, int, int, int)), this, SLOT(slot_CanopenFloorArrive(int, int, int, int)));
            connect(m_canopen, SIGNAL(sigShowFuncLogo(int, int)), this, SLOT(slot_ShowFuncLogo(int, int)));
            connect(m_canopen, SIGNAL(sigShowFloorOrLogo(bool)), this, SLOT(slot_ShowFloorOrLogo(bool)));
            connect(m_canopen, SIGNAL(sigMessage(QString, QString, QString, QString)), this, SLOT(slot_Message(QString, QString, QString, QString)));
            connect(m_canopen, SIGNAL(sigRealMessage(QString, QString)), this, SLOT(slot_RealMessage(QString, QString)));
        }
    }
}

void Uiloader::sltMsgArrive(QByteArray msg, int length)
{

}

void Uiloader::sltAppLogin(QString ssid, QString passwd)
{
    if(!QString::compare(ssid, "sodimas", Qt::CaseInsensitive))
    {
        if(!QString::compare(mLoginPasswd, passwd, Qt::CaseSensitive))
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is check ok";
            if (mWiFi)
            {
                mWiFi->sltloginResult(true);
            }
        }
        else
        {
            if (mWiFi)
            {
                mWiFi->sltloginResult(false);
            }
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is check false";
        }
    }
    else
    {
        qDebug()<<"wrong ssid";
    }
}

void Uiloader::sltDeviceConnected()
{
    if(!mLabelWIFI)
    {
        mLabelWIFI = new QLabel;
        if (mLabelWIFI)
        {
            mLabelWIFI->setScaledContents(true);
            mLabelWIFI->setPixmap(m_themeParser->m_FileDir + "wifi.png");
            if(mDesktop)
            {
                mLabelWIFI->setParent(mDesktop);
                mLabelWIFI->setGeometry(mDesktop->width()*3/4+50, 5, 45, 20);
            }
            mLabelWIFI->show();
        }
    }
    else if (mLabelWIFI)
    {
        mLabelWIFI->show();
    }
    if (m_canopen)
    {
        mStatusCanopenProtocol += " device is Connected";
        if (m_messageBar)
        {
            if (m_messageBar->isVisible())
            {
                m_messageBar->m_messageLabel->setText(mStatusCanopenProtocol);
            }
            else
            {
                m_messageBar->m_messageLabel->setText(mStatusCanopenProtocol);
                m_messageBar->show();
                QTimer::singleShot(8000, this, SLOT(updateMessageBar()));
            }
        }
    }
    else if (m_saf)
    {
        mStatusSafProtocol += " device is Connected";
        if (m_messageBar)
        {
            if (m_messageBar->isVisible())
            {
                m_messageBar->m_messageLabel->setText(mStatusSafProtocol);
            }
            else
            {
                m_messageBar->m_messageLabel->setText(mStatusSafProtocol);
                m_messageBar->show();
                QTimer::singleShot(8000, this, SLOT(updateMessageBar()));
            }
        }
    }
}

void Uiloader::sltDeviceDisConnected()
{
    if (mLabelWIFI)
    {
        mLabelWIFI->hide();
    }
}

void Uiloader::sltGetAllParameter()
{
    QJsonObject map;
    QJsonObject info;

    info.insert("autotest", m_autotest);
    info.insert("liftflrvol", m_liftflrvol);
    info.insert("liftarvvol", m_liftarvvol);
    info.insert("admusicvol", m_admusicvol);
    info.insert("luminance", m_luminance);
    info.insert("darklevel", m_standbyluminance);
    info.insert("standby", m_standby);
    info.insert("rotation", m_rotation);
    info.insert("layout", m_layout);

    map.insert("info", QJsonValue(info));
    map.insert("type", "sendallparameter");
    qDebug()<<__PRETTY_FUNCTION__<<"map = "<<map;
    QByteArray allPara;
    if (mWiFi)
    {
        allPara = mWiFi->convertJsonToByteArray(map);
        allPara = mWiFi->generateBuffer(allPara);
        mWiFi->writeMsgToClient(allPara, allPara.length());
    }
    qDebug()<<__PRETTY_FUNCTION__<<"allPara = "<<allPara;
//    allPara.insert(0, (char)ID_SENDPARA);
}

void Uiloader::sltUpdateFinished(bool flag)
{
    if (flag)
    {
        system("chmod 777 -R /home/ftp/pub/");
        QtSleep(100);
        mZipFunction.setZipFlag(false, true); // setting Zip is work
        mZipFunction.setZipFilePath("/usr/bst/", mFtpSendFileName);
        QtSleep(50);
        mZipFunction.start();
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"flag is false";
    }
}

void Uiloader::sltParameterSetUp(QString key, QVariant value)
{
    int va = 0;

    if(!QString::compare(key, "liftflrvol"))
    {
        va = value.toInt();
        if (m_liftflrvol != va)
        {
            m_liftflrvol = va;
            mXmlFileChangeFlag = true;
        }
    }
    else if(!QString::compare(key, "liftarvvol"))
    {
        va = value.toInt();
        if (m_liftarvvol != va)
        {
            m_liftarvvol = va;
            mXmlFileChangeFlag = true;
        }
    }
    else if(!QString::compare(key, "admusicvol"))
    {
        va = value.toInt();
        if (m_admusicvol != va)
        {
            m_admusicvol = va;
            if (m_Player)
            {
                m_Player->setBackgroundPlayerVolume(m_admusicvol);
                m_Player->playBackgroundPlayer();
            }
            mXmlFileChangeFlag = true;
        }
    }
    else if(!QString::compare(key, "luminance"))
    {
        va = value.toInt();
        if (m_luminance != va)
        {
            m_luminance = va;
            mXmlFileChangeFlag = true;
        }
    }
    else if(!QString::compare(key, "darklevel"))
    {
        va = value.toInt();
        if (m_standbyluminance != va)
        {
            m_standbyluminance = va;
            mXmlFileChangeFlag = true;
        }
    }
    else if(!QString::compare(key, "standby"))
    {
        va = value.toInt();
        if (m_standby != va)
        {
            m_standby = va;
            mXmlFileChangeFlag = true;
        }
    }
    else if(!QString::compare(key, "rotation"))
    {
        va = value.toInt();
        if (m_rotation != va)
        {
            m_rotation = va;
            SetRotate(m_rotation);
            mXmlFileChangeFlag = true;
        }
    }
    else if(!QString::compare(key, "layout"))
    {
        va = value.toInt();
        if (m_layout != va)
        {
            m_layout = va;
            SetLayout(m_layout);
            mXmlFileChangeFlag = true;
        }
    }
    else if(!QString::compare(key, "autotest"))
    {
        bool pAutoTest = value.toBool();
        if (m_autotest != pAutoTest)
        {
            m_autotest = pAutoTest;
            disconncetCanOpenSignal(m_autotest);
            mXmlFileChangeFlag = true;
        }
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"pAutoTest = "<<pAutoTest;
    }
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"va = "<<va;
}

void Uiloader::sltSaveXmlFile()
{
    if (mXmlFileChangeFlag)
    {
        if (m_mdpParser)
        {
            m_mdpParser->modifyItemElement("/configure/physical/com/autotest", QString::number(m_autotest));
            m_mdpParser->modifyItemElement("/configure/physical/player/liftflrvol", QString::number(m_liftflrvol));
            m_mdpParser->modifyItemElement("/configure/physical/player/liftarvvol", QString::number(m_liftarvvol));
            m_mdpParser->modifyItemElement("/configure/physical/player/admusicvol", QString::number(m_admusicvol));
            m_mdpParser->modifyItemElement("/configure/physical/lcd/luminance", QString::number(m_luminance));
            m_mdpParser->modifyItemElement("/configure/physical/lcd/standbyluminance", QString::number(m_standbyluminance));
            m_mdpParser->modifyItemElement("/configure/physical/lcd/standby", QString::number(m_standby));
            m_mdpParser->modifyItemElement("/configure/physical/lcd/rotation", QString::number(m_rotation));
            m_mdpParser->modifyItemElement("/configure/physical/com/autotest", QString::number(m_autotest));
            m_mdpParser->saveXmlAs(mRunPath + "device.xml");
        }
    }
}

void Uiloader::sltSendFille(bool flag, QString fileName)
{
    if (flag)
    {
        mFtpSendFileName = ftpFilePath + fileName;
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"flag = false mFtpSendFileName is empty";
    }
}

void Uiloader::sltDownLoad(bool flag, QString fileName)
{
    if (flag)
    {
        QtSleep(100);
        mZipFunction.setZipFlag(true, false); // setting Zip is work
        mZipFunction.setZipFilePath("/usr/bst/usrfs/", ftpFilePath + fileName);
        QtSleep(50);
        mZipFunction.start();
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"flag = false mFtpSendFileName is empty";
    }
}

void Uiloader::sltZipOrUnZipEnded(QString zip)
{
    if (!QString::compare("zip", zip, Qt::CaseInsensitive))
    {
        system("chmod 777 -R /home/ftp/pub/");
        QtSleep(100);
        if (mWiFi)
        {
            mWiFi->replyDownLoadFile(true);
        }
    }
    else if (!QString::compare("unZip", zip, Qt::CaseInsensitive))
    {

    }
    if (mZipConcernProgress)
    {
        mZipConcernProgress->hide();
    }
}

void Uiloader::sltShowProgress(QString fileName)
{
    if (m_themeDirection == Horizontal)
    {
        if (mZipConcernProgress)
        {
            mZipConcernProgress->setGeometry(100, 210, 280, 30);
        }
    }
    else if (m_themeDirection == Vertical)
    {
        if (mZipConcernProgress)
        {
            mZipConcernProgress->setGeometry(50, 400, 172, 30);
        }
    }
    if (mZipConcernProgress)
    {
        mZipConcernProgress->setStyleSheet("\
                QProgressBar {border:5px solid grey;border-radius:10px;\
                color: rgb(255, 0, 0);text-align:center}\
                QProgressBar::chunk {background-color: orange;border-radius: 5px}");
        mZipConcernProgress->show();
    }
}

void Uiloader::sltZipProgress(quint32 pro)
{
    if (mZipConcernProgress)
    {
        mZipConcernProgress->setValue(pro);
    }
}

void Uiloader::sltUnZipProgress(quint32 pro)
{
    if (mZipConcernProgress)
    {
        mZipConcernProgress->setValue(pro);
    }
}

void Uiloader::sltHideProgress()
{
    if (mZipConcernProgress)
    {
        mZipConcernProgress->hide();
    }
}

