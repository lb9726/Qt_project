#include "mainwindow.h"
#include <QTimer>
#include <QDebug>

Uiloader *gUiLoader = 0;

Uiloader::Uiloader(QObject *parent)
    :QUiLoader(parent)
{
    gUiLoader = this;

    m_Player = 0;
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

    m_saf = 0;
    m_canopen = 0;

    mLabelWIFI = 0;

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

    mIsSendZipFile = false;
    mIsSendZipFileProgress = 0;

    mZipFunction = 0;
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

    mCanopenPreStatus = " Initializing\n";
    mCanopenCurStatus = " Pre-Operation\n";
    m_DailSwitchAutoTest = false;
    qtimer_m_standby.setSingleShot(true);
    connect(&qtimer_m_standby, SIGNAL(timeout()), this, SLOT(onSigEnterScreenSave()));
    connect(this, &Uiloader::sigLoginMessage, this, &Uiloader::loginMessage);
    connect(this, &Uiloader::sigDestroyZip, this, &Uiloader::destroyZipFuntion);
}

void Uiloader::slot_Message(QString pM1, QString pM2, QString pM3, QString pM4)
{
    if (mUsbIsInsertFlag) // if usb is inserted, return
    {
        return;
    }
    if (mIcon)
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
    if (s1 == 1 && s2 == 1)
    {
        SetLayout(0);
        SetRotate(180);
    }
    if (s1 == 0 && s2 == 1)
    {
        SetLayout(0);
        SetRotate(270);
    }
    if (s1 == 1 && s2 == 0)
    {
        SetLayout(0);
        SetRotate(180);
    }
    if (s1 == 0 && s2 == 0)
    {
        SetLayout(0);
        SetRotate(90);
    }

    //volume
    if (s3 == 1 && s4 == 1)
    {
        //        m_Player->pauseBackgroundPlayer();
        //        m_Player->setBackgroundPlayerVolume(m_admusicvol);
        //        m_Player->playBackgroundPlayer();
    }
    if (s3 == 1 && s4 == 0)
    {
        if (m_Player)
        {
//            m_Player->pauseBackgroundPlayer();
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is "<<m_admusicvol/2;
            m_Player->setBackgroundPlayerVolume(m_admusicvol/2);
//            m_Player->playBackgroundPlayer();
        }
        m_admusicvol /= 2;
        m_beepvol /= 2;
        m_liftarvvol /= 2;
        m_liftflrvol /= 2;
    }
    if (s3 == 0 && s4 == 1)
    {
        if (m_Player)
        {
//            m_Player->pauseBackgroundPlayer();
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is "<<m_admusicvol/3;
            m_Player->setBackgroundPlayerVolume(m_admusicvol/3);
//            m_Player->playBackgroundPlayer();
        }
        m_admusicvol /= 3;
        m_beepvol /= 3;
        m_liftarvvol /= 3;
        m_liftflrvol /= 3;
    }
    if (s3 == 0 && s4 == 0)
    {
        if (m_Player)
        {
//            m_Player->pauseBackgroundPlayer();
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
//    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<pFuncLogoIndex <<", "<< pSoundNumber;

    if (mUsbIsInsertFlag) // if usb is inserted, return
    {
        return;
    }

    if (mIsShowLogo)
    {
        if (mFloor)
        {
            mFloor->setVisible(false);
        }
        if (mIcon)
        {
            mIcon->setVisible(true);
            mIcon->setIcon(pFuncLogoIndex);
        }
        if (m_Player)
        {
            mFunctionMusicNum = pSoundNumber;
            QString overloadMusic = MediaPath + QString("M%1.wav").arg(pSoundNumber);
            m_Player->addMedia(AO_BEEP, overloadMusic, 1, false, m_beepvol) ;
//            if (mFunctionMusicNum != pSoundNumber)
//            {

//            }
        }
    }
}

void Uiloader::slot_FloorArrive(int pGe, int pShi, int pBai, int pSoundNumber, int safFrame)
{
    IDE_TRACE_INT(pGe);
    IDE_TRACE_INT(pShi);
    IDE_TRACE_INT(pBai);
    IDE_TRACE_INT(pSoundNumber);

    if (mUsbIsInsertFlag) // if usb is inserted, return
    {
        return;
    }

    if (mFloor)
    {
        if (mIcon)
        {
            mIcon->setVisible(false);
        }
        if (mFloor)
        {
            mFloor->setVisible(true);
            mFloor->setFloor(pGe, pShi, pBai, safFrame);
            if (pSoundNumber != -1)
            {
                QString floorMusic = MediaPath + QString("M%1.wav").arg(pSoundNumber);
                // floor map
                QString mapMedia = mFloor->setMapMedia(safFrame);
                if (mapMedia != "false")
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
//    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"pGe = "<<pGe<<"shi = "<<pShi<<"pBai = "<<pBai<<"pSound = "<<pSoundNumber;
    if (mUsbIsInsertFlag) // if usb is inserted, return
    {
        return;
    }
    if (mFloor) //show Floor
    {
        if (mIcon)
        {
            mIcon->setVisible(false);
        }
        if (mFloor)
        {
            mFloor->setVisible(true);
            mFloor->setFloor(pGe, pShi, pBai);
            if (pSoundNumber != -1)
            {
                if (mMusicNumber != pSoundNumber)
                {
                    mMusicNumber = pSoundNumber;
                    QString floorMusic = MediaPath + QString("M%1.wav").arg(mMusicNumber);
                    if (41 == mMusicNumber || 42 == mMusicNumber || 43 == mMusicNumber)
                    {
                        if (m_Player)
                        {
                            m_Player->addMedia(AO_LIFTARV, floorMusic, 1, false, m_liftarvvol);
                        }
                    }
                    else
                    {
                        if (m_Player)
                        {
                            m_Player->addMedia(AO_LIFTFLR, floorMusic, 1, false, m_liftflrvol);
                        }
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

//void Uiloader::slot_CanopenshortBuzz()
//{
//    if (m_Player)
//    {
//        QString beepMusic = MediaPath + "beep.wav";
//        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"add beep";
//        m_Player->addMedia(AO_BEEP, beepMusic, 1, false, m_beepvol) ;
//    }
//}

void Uiloader::slot_MusicVolume(int pDivMusic)
{
    IDE_TRACE_INT(pDivMusic);
}

void Uiloader::slot_CanopenMusicVolume(int pDivMusic)
{
    IDE_TRACE_INT(pDivMusic);
    if (m_Player)
    {
        if (pDivMusic * 10 >= 99)
        {
            m_admusicvol = 95;
        }
        else
        {
            m_admusicvol = pDivMusic * 10;
        }
        mTempSound = m_admusicvol;
        if (!mBigTextFlag)
        {
            m_Player->setBackgroundPlayerVolume(m_admusicvol);
        }
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is "<<m_admusicvol;
    }
}

void Uiloader::slot_MainVolume(int pDivGong, int pDivBip)
{
    IDE_TRACE_INT(pDivGong);
    IDE_TRACE_INT(pDivBip);
    if (pDivGong != -1)
    {
        QString floorMusic = MediaPath + QString("M%1.wav").arg(pDivGong);
        if (m_Player)
        {
            m_Player->addMedia(AO_LIFTARV, floorMusic, 1, false, m_liftarvvol);
        }
    }
    if (pDivBip != -1)
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
    if (music == 43 && picture == 45)
    {
        QString floorMusic = MediaPath + QString("M43.wav");
        if (m_Player)
        {
            m_Player->addMedia(AO_BEEP, floorMusic, -1, false, m_beepvol);
        }
        if (mIcon)
        {
            mIcon->setVisible(true);
            mIcon->setIcon(picture);
        }
    }
    else if (music == 0 && picture == 0)
    {
        if (m_Player)
        {
            m_Player->clearMediaBuffer();
        }
    }
}

void Uiloader::slot_CanopenMainVolume(int pDivGong, int pDivBip)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<" "<<pDivGong<<" "<<pDivBip;
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
        if (m_canopen)
        {
            m_canopen->setBeepVolume(m_beepvol);
        }
    }
}

void Uiloader::slot_ArrowAction(bool pArrows_FLM, bool pArrows_FLD, bool pArrows_SCROL)
{
    IDE_TRACE_INT(pArrows_FLM);
    IDE_TRACE_INT(pArrows_FLD);
    IDE_TRACE_INT(pArrows_SCROL);
    if (mArrow)
    {
        if (pArrows_FLM == true && pArrows_FLD == false && pArrows_SCROL == false)
        {
            mArrow->setArrowType(UiArrow::UpArrow, mArrow->mScrollLevel);

            mBackLightCtrl.setBacklightValue(m_luminance);
            setScreenSave(false);
            mScreenSaver.exitScreenSaver();
            //m_Player->setBackgroundPlayerVolume(m_admusicvol);
            //m_Player->playBackgroundPlayer();
            setbackground_0();
            if (qtimer_m_standby.isActive())
            {
                qtimer_m_standby.stop();
            }
            mHasEnterFlag = false;
        }
        else if (pArrows_FLM == false && pArrows_FLD == true && pArrows_SCROL == false)
        {
            mArrow->setArrowType(UiArrow::DownArrow, mArrow->mScrollLevel);

            mBackLightCtrl.setBacklightValue(m_luminance);
            setScreenSave(false);
            mScreenSaver.exitScreenSaver();
            //m_Player->setBackgroundPlayerVolume(m_admusicvol);
            //m_Player->playBackgroundPlayer();
            setbackground_0();
            if (qtimer_m_standby.isActive())
            {
                qtimer_m_standby.stop();
            }
            mHasEnterFlag = false;
        }
        else if (pArrows_FLM == true && pArrows_FLD == false && pArrows_SCROL == true)
        {
            mArrow->setArrowType(UiArrow::UpScrollArrow, mArrow->mScrollLevel);


            mBackLightCtrl.setBacklightValue(m_luminance);
            setScreenSave(false);
            mScreenSaver.exitScreenSaver();
            //m_Player->setBackgroundPlayerVolume(m_admusicvol);
            //m_Player->playBackgroundPlayer();
            setbackground_0();
            if (qtimer_m_standby.isActive())
            {
                qtimer_m_standby.stop();
            }
            mHasEnterFlag = false;
        }
        else if (pArrows_FLM == false && pArrows_FLD == true && pArrows_SCROL == true)
        {
            mArrow->setArrowType(UiArrow::DownScrollArrow, mArrow->mScrollLevel);

            mBackLightCtrl.setBacklightValue(m_luminance);
            setScreenSave(false);
            mScreenSaver.exitScreenSaver();
            //m_Player->setBackgroundPlayerVolume(m_admusicvol);
            //m_Player->playBackgroundPlayer();
            setbackground_0();
            if (qtimer_m_standby.isActive())
            {
                qtimer_m_standby.stop();
            }
            mHasEnterFlag = false;
        }
        else if (pArrows_FLM == true && pArrows_FLD == true)
        {
            IDE_TRACE();
            mArrow->setArrowType(UiArrow::UpDownArrow, mArrow->mScrollLevel);

            mBackLightCtrl.setBacklightValue(m_luminance);
            setScreenSave(false);
            mScreenSaver.exitScreenSaver();
            //m_Player->setBackgroundPlayerVolume(m_admusicvol);
            //m_Player->playBackgroundPlayer();
            setbackground_0();
            if (qtimer_m_standby.isActive())
            {
                qtimer_m_standby.stop();
            }
            mHasEnterFlag = false;
        }
        else if (pArrows_FLM == false && pArrows_FLD == false)
        {
            mArrow->setArrowType(UiArrow::NoArrow, mArrow->mScrollLevel);
            if (!qtimer_m_standby.isActive())
            {
                qtimer_m_standby.start(m_standby);
                qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"has not arrow start timer = "<<m_standby;
            }
        }
    }
    judgeArrowExist(pArrows_FLM, pArrows_FLD);
}

void Uiloader::slot_ArrowAction_CanOpen(bool pArrows_FLM, bool pArrows_FLD, bool pArrows_SCROL)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<" FLM = "<<pArrows_FLM<<" FLD = "<<pArrows_FLD<<" SCROL = "<<pArrows_SCROL;
    if (mArrow)
    {
        if (pArrows_FLM == true && pArrows_FLD == false && pArrows_SCROL == false)
        {
            mArrow->setArrowType(UiArrow::UpArrow);

            mBackLightCtrl.setBacklightValue(m_luminance);
            setScreenSave(false);
            mScreenSaver.exitScreenSaver();
//            m_Player->setBackgroundPlayerVolume(m_admusicvol);
//            m_Player->playBackgroundPlayer();
            setbackground_0();
            if (qtimer_m_standby.isActive())
            {
                qtimer_m_standby.stop();
            }
            mHasEnterFlag = false;
        }
        else if (pArrows_FLM == false && pArrows_FLD == true && pArrows_SCROL == false)
        {
            mArrow->setArrowType(UiArrow::DownArrow);

            mBackLightCtrl.setBacklightValue(m_luminance);
            setScreenSave(false);
            mScreenSaver.exitScreenSaver();
//            m_Player->setBackgroundPlayerVolume(m_admusicvol);
//            m_Player->playBackgroundPlayer();
            setbackground_0();
            if (qtimer_m_standby.isActive())
            {
                qtimer_m_standby.stop();
            }
            mHasEnterFlag = false;
        }
        else if (pArrows_FLM == true && pArrows_FLD == false && pArrows_SCROL == true)
        {
            mArrow->setArrowType(UiArrow::UpScrollArrow, mScrollSpeed);


            mBackLightCtrl.setBacklightValue(m_luminance);
            setScreenSave(false);
            mScreenSaver.exitScreenSaver();
//            m_Player->setBackgroundPlayerVolume(m_admusicvol);
//            m_Player->playBackgroundPlayer();
            setbackground_0();
            if (qtimer_m_standby.isActive())
            {
                qtimer_m_standby.stop();
            }
            mHasEnterFlag = false;
        }
        else if (pArrows_FLM == false && pArrows_FLD == true && pArrows_SCROL == true)
        {
            mArrow->setArrowType(UiArrow::DownScrollArrow, mScrollSpeed);

            mBackLightCtrl.setBacklightValue(m_luminance);
            setScreenSave(false);
            mScreenSaver.exitScreenSaver();
            //m_Player->setBackgroundPlayerVolume(m_admusicvol);
            //m_Player->playBackgroundPlayer();
            setbackground_0();
            if (qtimer_m_standby.isActive())
            {
                qtimer_m_standby.stop();
            }
            mHasEnterFlag = false;
        }
        else if (pArrows_FLM == true && pArrows_FLD == true)
        {
            IDE_TRACE();
            mArrow->setArrowType(UiArrow::UpDownArrow);

            mBackLightCtrl.setBacklightValue(m_luminance);
            setScreenSave(false);
            mScreenSaver.exitScreenSaver();
            //m_Player->setBackgroundPlayerVolume(m_admusicvol);
            //m_Player->playBackgroundPlayer();
            setbackground_0();
            if (qtimer_m_standby.isActive())
            {
                qtimer_m_standby.stop();
            }
            mHasEnterFlag = false;
        }
        else if (pArrows_FLM == false && pArrows_FLD == false)
        {
            mArrow->setArrowType(UiArrow::NoArrow);
            if (!qtimer_m_standby.isActive())
            {
                qtimer_m_standby.start(m_standby);
                qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"has not arrow start timer = "<<m_standby;
            }
        }
    }
    judgeArrowExist(pArrows_FLM, pArrows_FLD);
}

void Uiloader::slot_FCT_AFF(bool pIsFCT)
{
    IDE_TRACE_INT(pIsFCT);
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<" pIsFCT = "<<pIsFCT;
    if (mUsbIsInsertFlag)
    {
        IDE_DEBUG(QString("Usb is inserting cannot enter other screen!"));
        return;
    }
    if (mIcon)
    {
        mIcon->setBigSrnMode(pIsFCT);
        mBigTextFlag = pIsFCT;
        if (mBigTextFlag)
        {
            if (m_Player)
            {
                m_Player->setBackgroundPlayerVolume(0);
                qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is 0";
            }
        }
        else
        {
            if (m_Player)
            {
                if (mTempSound != -1)
                {
                    m_Player->setBackgroundPlayerVolume(mTempSound);
                }
                qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<" mTempSound is "<<mTempSound;
            }
        }
        QtSleep(500);
    }
}

void Uiloader::slot_Orient(int pOrientation)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"pOrientation = "<<pOrientation;
    if (0 == pOrientation)
    {
        SetLayout(0);
        SetRotate(180);
    }
    else if (1 == pOrientation)
    {
        SetLayout(0);
        SetRotate(270);
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
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"pState = "<<pState;
    QString tmpStr;
    if (3 == pState)
    {
        mCanopenPreStatus = mCanopenCurStatus;
        mCanopenCurStatus = " Operational\n";
        tmpStr = mCanopenPreStatus + mCanopenCurStatus;
        mCanopenMessageStatus = tmpStr;
    }
    else if (2 == pState)
    {
        mCanopenPreStatus = mCanopenCurStatus;
        mCanopenCurStatus = " Pre-Operational\n";
        tmpStr = mCanopenPreStatus + mCanopenCurStatus;
        mCanopenMessageStatus = tmpStr;
    }
    else if (1 == pState)
    {
        mCanopenPreStatus = mCanopenCurStatus;
        mCanopenCurStatus = " Initializing\n";
        tmpStr = mCanopenPreStatus + mCanopenCurStatus;
        mCanopenMessageStatus = tmpStr;
    }
    else if (4 == pState)
    {
        mCanopenPreStatus = mCanopenCurStatus;
        mCanopenCurStatus = " Stopping\n";
        tmpStr = mCanopenPreStatus + mCanopenCurStatus;
        mCanopenMessageStatus = tmpStr;
    }
    if (m_messageBar)
    {
        QString tmpText = m_messageBar->m_messageLabel->text();
        if ((2 == pState || 1 == pState) && tmpText.contains(QString("Code"), Qt::CaseInsensitive))
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is include code";
        }
        else
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"isnot include code";
            m_messageBar->m_messageLabel->setText(tmpStr);
        }
        m_messageBar->show();
        if (3 == pState)
        {
            QTimer::singleShot(5000, this, SLOT(updateMessageBar()));
        }
    }
    if (3 != pState)
    {
        if (mIcon)
        {
            mIcon->setEmptyIcon();
            mIcon->setBigTextEmpty();
        }
        if (mFloor)
        {
            mFloor->setEmptyFloor();
            mFloor->setSmallTextEmpty();
        }
        if (mArrow)
        {
            mArrow->setEmptyPixmap();
        }
        if (m_canopen)
        {
            m_canopen->setBigOrSmallTextEmpty();
        }
    }
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
            if (pFlag)
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
    if (m_autotest)
    {
        m_messageBar->hide();
    }
    else if (m_saf)
    {
        m_messageBar->hide();
    }
    else if (m_canopen)
    {
        m_messageBar->hide();
    }
}

void Uiloader::loginMessage(bool flag)
{
    if (flag)
    {
        if (m_messageBar)
        {
            if (m_messageBar->m_messageLabel)
            {
                QString tmp = " login Successed!\n";
                m_messageBar->m_messageLabel->setText(tmp);
            }
            m_messageBar->show();
            QTimer::singleShot(8000, this, SLOT(updateMessageBar()));
        }
    }
    else
    {
        if (m_messageBar)
        {
            if (m_messageBar->m_messageLabel)
            {
                QString tmp = " login Failed!\n";
                m_messageBar->m_messageLabel->setText(tmp);
            }
            m_messageBar->show();
            QTimer::singleShot(8000, this, SLOT(updateMessageBar()));
        }
    }
}

void Uiloader::destroyZipFuntion()
{
    if (mZipFunction)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"destroyzipFunction";
        disconnect(mZipFunction, 0, 0, 0);
        mZipFunction->deleteLater();
        mZipFunction = 0;
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
    if (m_messageBar)
    {
        m_messageBar->setWindowFlags(Qt::FramelessWindowHint | Qt::Widget);
        m_messageBar->setAttribute(Qt::WA_TranslucentBackground);
        m_messageBar->setStyleSheet("border: 4px solid blue;border-radius: 14px;");
        mView->scene()->addWidget(m_messageBar);

        QString wifi = wifiNameInfo();
        if (m_saf)
        {
            m_messageBar->m_messageLabel->setText(" Wifi: "+ wifi);
            QTimer::singleShot(5000, this, SLOT(updateMessageBar()));
            m_messageBar->show();
        }
        else if (m_canopen)
        {
            if (!m_DailSwitchAutoTest) // 255 is AutoTest will cannot show
            {
                QString code = QString(" Code[%1]\n").arg(wifiNameInfo());
                code = code + " Initializing\n" +" Pre-Operational\n";
                m_messageBar->m_messageLabel->setText(code);
                m_messageBar->show();
            }
        }
    }
}

QString Uiloader::wifiNameInfo()
{
    QFile file("/usr/module/hostapd.conf");
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"Can't open the file!"<<endl;
    }
    QByteArray line = file.readAll();
    QString str(line);
    mWifiNameInfo = str.mid(str.indexOf("ssid=")+5, str.indexOf("channel")-str.indexOf("ssid=") - 5);
    mWifiNameInfo = mWifiNameInfo.simplified();
    return mWifiNameInfo;
}

void Uiloader::slot_AutoTest()
{    
    if (mAutoTestCount <= 31) // 0 ~ 31
    {
        if (mIcon)
        {
            mIcon->setBigSrnMode(false);
            mIcon->setVisible(false);
        }
        if (mFloor)
        {
            mFloor->setVisible(true);
            mFloor->setFloor(mAutoTestCount%10, mAutoTestCount/10, 0); //GE SHI BAI
        }
        if (m_Player)
        {
            QString floorMusic = MediaPath + QString("M%1.wav").arg(mAutoTestCount);
            m_Player->addMedia(AO_LIFTFLR, floorMusic, 1, false, m_liftflrvol);
        }
    }
    else if (mAutoTestCount <= 38) // mAutoTestCount(32 ~ 38) -1 ~ -7
    {
        if (mIcon)
        {
            mIcon->setBigSrnMode(false);
            mIcon->setVisible(false);
        }
        if (mFloor)
        {
            mFloor->setVisible(true);
            mFloor->setFloor((mAutoTestCount - 31)%10, -3, 0); // GE SHI BAI
        }
        if (m_Player) //  -1 ~ -7 music // M39.wav ~ M33.wav
        {
            QString floorMusic = MediaPath + QString("M%1.wav").arg(71 - mAutoTestCount);
            m_Player->addMedia(AO_LIFTFLR, floorMusic, 1, false, m_liftflrvol);
        }
    }
    else if (mAutoTestCount <= 48) //function LOGO mAutoTestCount(39 ~ 48) T40 ~ T49
    {
        if (mFloor)
        {
            mFloor->setVisible(false);
        }
        if (mIcon)
        {
            mIcon->setBigSrnMode(false);
            mIcon->setVisible(true);
            mIcon->setIcon(mAutoTestCount + 1);
        }

        if (mAutoTestCount + 1 == 42 || mAutoTestCount + 1 == 47)
        {
            if (m_Player) // play music overload and force close door
            {
                QString floorMusic = MediaPath + QString("M%1.wav").arg(43);
                m_Player->addMedia(AO_LIFTFLR, floorMusic, 1, false, m_liftflrvol);
            }
        }
    }
    else if (49 == mAutoTestCount) // UpArrow play musicn ding
    {
        if (mIcon)
        {
            mIcon->setVisible(false);
        }
        if (mArrow)
        {
            mArrow->show();
            mArrow->setArrowType(UiArrow::UpScrollArrow, 4);
        }
        if (m_Player) //  play music ding
        {
            QString floorMusic = MediaPath + QString("M%1.wav").arg(40);
            m_Player->addMedia(AO_LIFTFLR, floorMusic, 1, false, m_liftflrvol);
        }
    }
    else if (50 == mAutoTestCount)
    {
        if (mArrow)
        {
            mArrow->show();
            mArrow->setArrowType(UiArrow::DownScrollArrow, 4);
        }
        if (m_Player) //  play music dingdong
        {
            QString floorMusic = MediaPath + QString("M%1.wav").arg(41);
            m_Player->addMedia(AO_LIFTFLR, floorMusic, 1, false, m_liftflrvol);
        }
    }
    else if (51 == mAutoTestCount ) // show UpDownArrow play music dingdongding
    {
        if (mArrow)
        {
            mArrow->show();
            mArrow->setArrowType(UiArrow::UpDownArrow, 4);
        }
        if (m_Player) //  play music dingdongding
        {
            QString floorMusic = MediaPath + QString("M%1.wav").arg(42);
            m_Player->addMedia(AO_LIFTFLR, floorMusic, 1, false, m_liftflrvol);
        }
    }
    else if (mAutoTestCount <= 66)
    {
        if (mArrow)
        {
            mArrow->setVisible(false);
        }
        if (mFloor)
        {
            mFloor->setVisible(false);
        }
        if (mIcon)
        {
            mIcon->setVisible(true);
            mIcon->setIcon(mAutoTestCount - 52);
        }
        if (m_Player) //  play music M70.wav ~ M84.wav
        {
            QString floorMusic = MediaPath + QString("M%1.wav").arg(mAutoTestCount + 18);
            m_Player->addMedia(AO_LIFTFLR, floorMusic, 1, false, m_liftflrvol);
        }
    }
    else if (mAutoTestCount <= 84)
    {
        if (mFloor)
        {
            mFloor->setVisible(false);
        }
        if (mIcon) // T15.png ~ T21.png T22.png ~ T32.png
        {
            mIcon->setVisible(true);
            mIcon->setIcon(mAutoTestCount - 52);
        }
        if (mAutoTestCount == 84)
        {
            mAutoTestCount = -1;
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"set mAutoTestCount = "<<mAutoTestCount;
        }
    }
    mAutoTestCount++;
}

void Uiloader::setArrowScrollSpeed(int arrowSpeed)
{
    mScrollSpeed = arrowSpeed;
}

void Uiloader::onSigNoArrow()
{
    IDE_TRACE_STR(QString("will start timer"));
    //QTimer::singleShot(m_standby, this, SLOT(onSigEnterScreenSave()));
    if (!qtimer_m_standby.isActive())
    {
        qtimer_m_standby.start(m_standby);
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"has not arrow start timer = "<<m_standby;
    }
}

void Uiloader::onSigEnterScreenSave()
{
    IDE_TRACE_INT(mSaverFlag);
    if (m_Player)
    {
//        m_Player->pauseBackgroundPlayer();
        m_Player->setBackgroundPlayerVolume(0);//zzf
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"volume set 0";
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
    if (!mArrows_FLD && !mArrows_FLM)
    {
        if (!mHasEnterFlag)
        {
            mSaverFlag = true;
            emit sigNoArrow();
            mHasEnterFlag = true;
        }
    }
    else
    {
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
            if (qtimer_m_standby.isActive())
            {
                qtimer_m_standby.stop();
            }
            mHasEnterFlag = false;
        }
    }
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
        setbackground_0();
        if (qtimer_m_standby.isActive())
        {
            qtimer_m_standby.stop();
        }
        emit sigUsbInsert();
        IDE_TRACE_STR(QString("usb is insert"));
    }
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
}

QWidget* Uiloader::createWidget(const QString &className, QWidget *parent, const QString &name)
{
    if (!className.compare("UiArrow", Qt::CaseInsensitive))
    {
        UiArrow* tmpArrow = new UiArrow(parent);
        tmpArrow->setObjectName(name);
        mArrow = tmpArrow;
        return tmpArrow;
    }
    if (!className.compare("UiFloor", Qt::CaseInsensitive))
    {
        UiFloor* tmpFloor = new UiFloor(parent);
        tmpFloor->setObjectName(name);
        mFloor = tmpFloor;
        return tmpFloor;
    }
    //    if (!className.compare("UiClock", Qt::CaseInsensitive))
    //    {
    //        UiClock* tmpClock = new UiClock;
    //        tmpClock->setObjectName(name);
    //        tmpClock->Init();
    //        return tmpClock;
    //    }
    if (!className.compare("UiDesktop", Qt::CaseInsensitive))
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
                mZipConcernProgress->setStyleSheet("\
                        QProgressBar {border:5px solid grey;border-radius:10px;\
                        color: rgb(255, 0, 0);text-align:center}\
                        QProgressBar::chunk {background-color: orange;border-radius: 5px}");
                mZipConcernProgress->hide();
            }
        }
        return tmpDesktop;
    }
    if (!className.compare("UiIcon", Qt::CaseInsensitive))
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
    //    if (!className.compare("UiVideo", Qt::CaseInsensitive))
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
    if (pType == COM_DESKTOP && mDesktop)
    {
        mDesktop->Init(pElement);
    }
    else if (pType == COM_FLOOR && mFloor)
    {
        mFloor->Init(pElement);
    }
    else if (pType == COM_FUNCTION && mIcon)
    {
        mIcon->Init(pElement);
    }
    else if (pType == COM_ARROW && mArrow)
    {
        mArrow->Init(pElement);
    }
    else if (pType == COM_MEDIA && mFloor)
    {
        mFloor->initAudio(pElement);
    }
}

void Uiloader::SetLayout(int pLayout)
{
    if (m_themeLayout != pLayout)
    {
        m_themeLayout = pLayout;

        QList<COM_TYPE> tmpComTypeList = m_themeParser->m_ComElementGroup.keys();
        int count = tmpComTypeList.count();
        for(int i = 0; i < count; ++i)
        {
            COM_TYPE tmpType = tmpComTypeList.at(i);

            QDomElement tmpElement = m_themeParser->m_ComElementGroup.value(tmpType);
            if (tmpElement.isNull())
            {
                continue;
            }
            InitComponent(tmpType, tmpElement);
        }
    }
}

void Uiloader::SetRotate(int pRotate)
{
    if (m_themeRotate != pRotate)
    {
        mView->setGeometry(0, 0, 480, 272);

        if (pRotate == 0 || pRotate == 180 || pRotate == 360)
        {
            m_themeDirection = Horizontal;
        }
        else
        {
            m_themeDirection = Vertical;
        }
        setProgressGeometry();
        setWifiIconGeometry();
        mView->rotate(pRotate - m_themeRotate);
        mView->setGeometry(-5, -5, 480+10, 272+10);
        m_themeRotate = pRotate;

        QList<COM_TYPE> tmpComTypeList = m_themeParser->m_ComElementGroup.keys();
        int count = tmpComTypeList.count();
        for(int i = 0; i < count; ++i)
        {
            COM_TYPE tmpType = tmpComTypeList.at(i);

            QDomElement tmpElement = m_themeParser->m_ComElementGroup.value(tmpType);
            if (tmpElement.isNull())
            {
                continue;
            }
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

    if (!tmpFile.open(QFile::ReadOnly))
    {
        return tmpWidget;
    }
    tmpWidget = load(&tmpFile);

    m_mdpParser = new MdpParser(pUiPath + "device.xml");
    if (m_mdpParser)
    {
        callOpenMdp();
    }

    m_themeParser = new ThemeParser(pUiPath + "theme.xml");
    if (m_themeParser->openUi())
    {
        QList<COM_TYPE> tmpComTypeList = m_themeParser->m_ComElementGroup.keys();
        int count = tmpComTypeList.count();
        for(int i = 0; i < count; ++i)
        {
            COM_TYPE tmpType = tmpComTypeList.at(i);

            QDomElement tmpElement = m_themeParser->m_ComElementGroup.value(tmpType);
            if (tmpElement.isNull())
            {
                continue;
            }
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
//    IDE_TRACE_INT(m_admusicvol);
//    m_Player->setBackgroundPlayerVolume(m_admusicvol);
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is "<<m_admusicvol;
    //autotest
    if (m_autotest)
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
#ifdef LINUX
        if (!QString::compare("saf", mProtocolType, Qt::CaseInsensitive))
        {
            initSafProtocol();
            connect(this, SIGNAL(sigNoArrow()), this, SLOT(onSigNoArrow()));
            connect(&mScreenSaverTimers, SIGNAL(timeout()), this, SLOT(autoCallOn()));
            mScreenSaverTimers.start(2000);
        }
        else if (!QString::compare("canopen", mProtocolType, Qt::CaseInsensitive))
        {
            initCanOpenProtocol();
        }
        else
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"protocolType is not legal";
        }
        //Theme
        SetLayout(m_layout);
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"will setRotate = "<<m_rotation;
        SetRotate(m_rotation);
#endif
    }
    return tmpWidget;
}

void Uiloader::initCanOpenProtocol()
{
    m_canopen = CanOpenClientFunction::getInstace();
    if (m_canopen)
    {
        if (m_Player)
        {
            m_canopen->setMplayer(m_Player);
        }
        quint8 tmp = m_canopen->getDailSwitchNumber();
        if (255 == tmp) // AutoTest
        {
            //Theme
            m_DailSwitchAutoTest = true;
            SetLayout(m_layout);
            SetRotate(m_rotation);
            slot_AutoTest();
            mAutoTestTimer.setInterval(m_testfreq);
            mAutoTestTimer.start();
        }
        else
        {
            IDE_TRACE_STR(QString("canopen protocol connect first"));
            m_canopen->initCanOpenFunction();
            disconncetCanOpenSignal(false);
            connect(this, SIGNAL(sigNoArrow()), this, SLOT(onSigNoArrow()));
            connect(&mScreenSaverTimers, SIGNAL(timeout()), this, SLOT(autoCallOn()));
            mScreenSaverTimers.start(2000);
        }
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"m_canopen is Null";
        return;
    }
}

void Uiloader::initSafProtocol()
{
    m_saf = new SafThread(this);
    m_saf->m_openingMessage = m_safOpenningMessage;
    connect(m_saf, SIGNAL(sigArrowAction(bool, bool, bool)), this, SLOT(slot_ArrowAction(bool, bool, bool)));
    connect(m_saf, SIGNAL(sigMainVolumeChange(int, int)), this, SLOT(slot_MainVolume(int, int))); // gong,beep
    connect(m_saf, SIGNAL(sigFloorArrive(int, int, int, int, int)), this, SLOT(slot_FloorArrive(int, int, int, int, int))); //
    connect(m_saf, SIGNAL(sigShowFuncLogo(int, int)), this, SLOT(slot_ShowFuncLogo(int, int)));
    connect(m_saf, SIGNAL(sigVit_scrol(int)), this, SLOT(slot_Vitscrol(int)));
    connect(m_saf, SIGNAL(switchValue(int,int,int,int)), this, SLOT(slot_switchValue(int, int, int, int))); //switch
    connect(m_saf,SIGNAL(sFrame60Loop(int,int)),this,SLOT(slot_Frame60Loop(int,int)));
    m_saf->start();
}

void Uiloader::callOpenMdp()
{
    if (m_mdpParser->openMdp())
    {
        mLoginPasswd = m_mdpParser->getPassword();
        mProtocolType = m_mdpParser->getProtocolType();
        QDomElement tmpPlayerElement = m_mdpParser->m_PHElementGroup.value(P_Player);
        if (!tmpPlayerElement.isNull())
        {
            QDomElement tmpElement = tmpPlayerElement.firstChildElement("admusicvol");
            if (!tmpElement.isNull())
            {
                m_admusicvol = tmpElement.text().toInt();
            }
            if (m_admusicvol >= 100)
            {
                m_admusicvol = 99;
            }
            else if (m_admusicvol < 0)
            {
                m_admusicvol = 0;
            }
            tmpElement = tmpPlayerElement.firstChildElement("beepvol");
            if (!tmpElement.isNull())
            {
                m_beepvol = tmpElement.text().toInt();
            }
            tmpElement = tmpPlayerElement.firstChildElement("liftarvvol");
            if (!tmpElement.isNull())
            {
                m_liftarvvol = tmpElement.text().toInt();
            }
            tmpElement = tmpPlayerElement.firstChildElement("liftflrvol");
            if (!tmpElement.isNull())
            {
                m_liftflrvol = tmpElement.text().toInt();
            }
        }

        QDomElement tmpLedElement = m_mdpParser->m_PHElementGroup.value(P_LED);
        if (!tmpLedElement.isNull())
        {
            QDomElement tmpElement = tmpLedElement.firstChildElement("normal");
            if (!tmpElement.isNull())
            {
                m_lednormalFreq = tmpElement.text();
            }
            tmpElement = tmpLedElement.firstChildElement("busy");
            if (!tmpElement.isNull())
            {
                m_ledbusyFreq = tmpElement.text();
            }
            tmpElement = tmpLedElement.firstChildElement("error");
            if (!tmpElement.isNull())
            {
                m_lederrorFreq = tmpElement.text();
            }
        }

        QDomElement tmpLcdElement = m_mdpParser->m_PHElementGroup.value(P_LCD);
        if (!tmpLcdElement.isNull())
        {
            QDomElement tmpElement = tmpLcdElement.firstChildElement("rotation");
            if (!tmpElement.isNull())
            {
                m_rotation = tmpElement.text().toInt() + 180;
                if (0 == m_rotation || 180 == m_rotation || 360 == m_rotation)
                {
                    m_themeDirection = Horizontal;
                }
                else
                {
                    m_themeDirection = Vertical;
                }
            }
            tmpElement = tmpLcdElement.firstChildElement("luminance");
            if (!tmpElement.isNull())
            {
                m_luminance = tmpElement.text().toInt();
            }
            tmpElement = tmpLcdElement.firstChildElement("standby");
            if (!tmpElement.isNull())
            {
                m_standby = tmpElement.text().toInt();
            }
            tmpElement = tmpLcdElement.firstChildElement("standbyluminance");
            if (!tmpElement.isNull())
            {
                m_standbyluminance = tmpElement.text().toInt();
            }
            tmpElement = tmpLcdElement.firstChildElement("screensave");
            if (!tmpElement.isNull())
            {
                m_screensave = tmpElement.text().toInt();
            }
            tmpElement = tmpLcdElement.firstChildElement("screensavehold");
            if (!tmpElement.isNull())
            {
                m_screensavehold = tmpElement.text().toInt();
            }
        }

        QDomElement tmpComElement = m_mdpParser->m_PHElementGroup.value(P_COM);
        if (!tmpComElement.isNull())
        {
            QDomElement tmpElement = tmpComElement.firstChildElement("autotest");
            if (!tmpElement.isNull())
            {
                m_autotest = (bool)(!tmpElement.text().compare("true"));
            }
            tmpElement = tmpComElement.firstChildElement("testfreq");
            if (!tmpElement.isNull())
            {
                m_testfreq = tmpElement.text().toInt();
            }

            tmpElement = tmpComElement.firstChildElement("safOpenMessage");
            IDE_TRACE_STR(tmpElement.text());
            if (!tmpElement.isNull())
            {
                m_safOpenningMessage = (bool)(!tmpElement.text().compare("true"));
            }
        }

        QDomElement tmpThemeElement = m_mdpParser->m_LGElementGroup.value(L_ThemeManager);
        if (!tmpThemeElement.isNull())
        {
            QDomElement tmpElement = tmpThemeElement.firstChildElement("layout");
            if (!tmpElement.isNull())
            {
                m_layout = tmpElement.text().toInt();
            }
        }

        QDomElement tmpLancElement = m_mdpParser->m_PHElementGroup.value(P_LANC);
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__;
        if (!tmpLancElement.isNull())
        {
            QDomElement tmpElement = tmpLancElement.firstChildElement("tcp");
            if (!tmpElement.isNull())
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
    if (!mbackgroundset_0)
    {
        if (m_Player)
        {
            m_Player->setBackgroundPlayerVolume(m_admusicvol);
//            m_Player->playBackgroundPlayer();
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
        connect(mWiFi, &WifiCommunication::sigUploadProgress, this, &Uiloader::sltUploadProgress);
        connect(mWiFi, &WifiCommunication::sigDownLoad, this, &Uiloader::sltDownLoad);
        connect(mWiFi, &WifiCommunication::sigRebootMachine, this, &Uiloader::sltRebootMachine);
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
            disconnect(m_canopen, SIGNAL(sigArrowAction(bool, bool, bool)), this, SLOT(slot_ArrowAction_CanOpen(bool,bool,bool)));
            disconnect(m_canopen, SIGNAL(sigMainVolumeChange(int, int)), this, SLOT(slot_CanopenMainVolume(int, int)));
            disconnect(m_canopen, SIGNAL(sigMusicVolumeChange(int)), this, SLOT(slot_CanopenMusicVolume(int)));
            disconnect(m_canopen, SIGNAL(sigBuzz(int)), this, SLOT(slot_CanopenBuzz(int)));
//            disconnect(m_canopen, SIGNAL(sigBuzz()), this, SLOT(slot_CanopenshortBuzz()));
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
            connect(m_canopen, SIGNAL(sigArrowAction(bool, bool, bool)), this, SLOT(slot_ArrowAction_CanOpen(bool, bool, bool)));
            connect(m_canopen, SIGNAL(sigMainVolumeChange(int, int)), this, SLOT(slot_CanopenMainVolume(int, int)));
            connect(m_canopen, SIGNAL(sigMusicVolumeChange(int)), this, SLOT(slot_CanopenMusicVolume(int)));
            connect(m_canopen, SIGNAL(sigBuzz(int)), this, SLOT(slot_CanopenBuzz(int)));
//            connect(m_canopen, SIGNAL(sigBuzz()), this, SLOT(slot_CanopenshortBuzz()));
            connect(m_canopen, SIGNAL(sigFloorArrive(int, int, int, int)), this, SLOT(slot_CanopenFloorArrive(int, int, int, int)));
            connect(m_canopen, SIGNAL(sigShowFuncLogo(int, int)), this, SLOT(slot_ShowFuncLogo(int, int)));
            connect(m_canopen, SIGNAL(sigShowFloorOrLogo(bool)), this, SLOT(slot_ShowFloorOrLogo(bool)));
            connect(m_canopen, SIGNAL(sigMessage(QString, QString, QString, QString)), this, SLOT(slot_Message(QString, QString, QString, QString)));
            connect(m_canopen, SIGNAL(sigRealMessage(QString, QString)), this, SLOT(slot_RealMessage(QString, QString)));
        }
    }
}

void Uiloader::disconnectSafSignal(bool pAutoTest)
{
    if (pAutoTest)
    {
        if (m_saf)
        {
            disconnect(m_saf, SIGNAL(sigArrowAction(bool, bool, bool)), this, SLOT(slot_ArrowAction(bool, bool, bool)));
            disconnect(m_saf, SIGNAL(sigMainVolumeChange(int, int)), this, SLOT(slot_MainVolume(int, int))); // gong,beep
            disconnect(m_saf, SIGNAL(sigFloorArrive(int, int, int, int, int)), this, SLOT(slot_FloorArrive(int, int, int, int, int))); //
            disconnect(m_saf, SIGNAL(sigShowFuncLogo(int, int)), this, SLOT(slot_ShowFuncLogo(int, int)));
            disconnect(m_saf, SIGNAL(sigVit_scrol(int)), this, SLOT(slot_Vitscrol(int)));
            disconnect(m_saf, SIGNAL(switchValue(int,int,int,int)), this, SLOT(slot_switchValue(int, int, int, int))); //switch
        }
    }
    else
    {
        if (m_saf)
        {
            connect(m_saf, SIGNAL(sigArrowAction(bool, bool, bool)), this, SLOT(slot_ArrowAction(bool, bool, bool)));
            connect(m_saf, SIGNAL(sigMainVolumeChange(int, int)), this, SLOT(slot_MainVolume(int, int))); // gong,beep
            connect(m_saf, SIGNAL(sigFloorArrive(int, int, int, int, int)), this, SLOT(slot_FloorArrive(int, int, int, int, int))); //
            connect(m_saf, SIGNAL(sigShowFuncLogo(int, int)), this, SLOT(slot_ShowFuncLogo(int, int)));
            connect(m_saf, SIGNAL(sigVit_scrol(int)), this, SLOT(slot_Vitscrol(int)));
            connect(m_saf, SIGNAL(switchValue(int,int,int,int)), this, SLOT(slot_switchValue(int, int, int, int))); //switch
        }
    }
}

void Uiloader::sltMsgArrive(QByteArray msg, int length)
{

}

void Uiloader::sltAppLogin(QString ssid, QString passwd)
{
    if (!QString::compare(ssid, "sodimas", Qt::CaseInsensitive))
    {
        if (!QString::compare(mLoginPasswd, passwd, Qt::CaseSensitive))
        {
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is check ok";
            if (mWiFi)
            {
                mWiFi->sltloginResult(true);
                emit sigLoginMessage(true);
            }
        }
        else
        {
            if (mWiFi)
            {
                mWiFi->sltloginResult(false);
                emit sigLoginMessage(false);
            }
            qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is check false";
        }
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"wrong ssid";
    }
}

void Uiloader::sltDeviceConnected()
{
    if (!mLabelWIFI)
    {
        mLabelWIFI = new QLabel;
        if (mLabelWIFI)
        {
            mLabelWIFI->setScaledContents(true);
            mLabelWIFI->setPixmap(m_themeParser->m_FileDir + "wifi.png");
            if (mDesktop)
            {
                mLabelWIFI->setParent(mDesktop);
                setWifiIconGeometry();
            }
            mLabelWIFI->show();
        }
    }
    else if (mLabelWIFI)
    {
        setWifiIconGeometry();
        mLabelWIFI->show();
    }
    if (m_canopen)
    {
        QString tmp = mCanopenMessageStatus + " device is Connected\n";
        if (m_messageBar)
        {
            m_messageBar->m_messageLabel->clear();
            if (m_messageBar->isVisible())
            {                
                m_messageBar->m_messageLabel->setText(tmp);
            }
            else
            {
                m_messageBar->m_messageLabel->setText(tmp);
                m_messageBar->show();
                QTimer::singleShot(8000, this, SLOT(updateMessageBar()));
            }
        }
    }
    else if (m_saf)
    {
        QString tmp = mStatusSafProtocol + " device is Connected\n";
        if (m_messageBar)
        {
            m_messageBar->m_messageLabel->clear();
            if (m_messageBar->isVisible())
            {
                m_messageBar->m_messageLabel->setText(tmp);
            }
            else
            {
                m_messageBar->m_messageLabel->setText(tmp);
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
        if (m_canopen)
        {
            QString tmp = mCanopenMessageStatus + " device disconnected\n";
            if (mIsSendZipFile && mIsSendZipFileProgress > 0 && mIsSendZipFileProgress < 100)
            {
                tmp = tmp + " sendFile is stoped!\n";
                if (mZipConcernProgress)
                {
                    mZipConcernProgress->hide();
                }
                mIsSendZipFile = false;
                mIsSendZipFileProgress = 0;
            }
            if (m_messageBar)
            {
                m_messageBar->m_messageLabel->clear();
                if (m_messageBar->isVisible())
                {
                    m_messageBar->m_messageLabel->setText(tmp);
                }
                else
                {
                    m_messageBar->m_messageLabel->setText(tmp);
                    m_messageBar->show();
                    QTimer::singleShot(8000, this, SLOT(updateMessageBar()));
                }
            }
        }
        else if (m_saf)
        {
            QString tmp = mStatusSafProtocol + " device disconnected\n";
            if (mIsSendZipFile && mIsSendZipFileProgress > 0 && mIsSendZipFileProgress < 100)
            {
                tmp = tmp + " sendFile is stoped!\n";
                if (mZipConcernProgress)
                {
                    mZipConcernProgress->hide();
                }
                mIsSendZipFile = false;
                mIsSendZipFileProgress = 0;
            }
            if (m_messageBar)
            {
                m_messageBar->m_messageLabel->clear();
                if (m_messageBar->isVisible())
                {
                    m_messageBar->m_messageLabel->setText(tmp);
                }
                else
                {
                    m_messageBar->m_messageLabel->setText(tmp);
                    m_messageBar->show();
                    QTimer::singleShot(8000, this, SLOT(updateMessageBar()));
                }
            }
        }
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
//    qDebug()<<__PRETTY_FUNCTION__<<"map = "<<map;
    QByteArray allPara;
    if (mWiFi)
    {
        allPara = mWiFi->convertJsonToByteArray(map);
        allPara = mWiFi->generateBuffer(allPara);
        mWiFi->writeMsgToClient(allPara, allPara.length());
    }
    qDebug()<<__PRETTY_FUNCTION__<<"allPara = "<<allPara;
}

void Uiloader::sltUpdateFinished(bool flag)
{
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"flag = "<<flag;
    if (flag)
    {
        if (mZipConcernProgress)
        {
            mZipConcernProgress->hide();
            mZipConcernProgress->reset();
        }
        system("sync");
        QtSleep(1000);
        system("chmod 777 -R /home/ftp/pub/");
        QtSleep(100);
        initZipFunction();
        if (mZipFunction)
        {
            mZipFunction->setZipFlag(false, true); // setting unZip is work
            mZipFunction->setZipFilePath("/usr/bst/", mFtpSendFileName);
//            QtSleep(2);
            if (mZipFunction->isRunning())
            {
                qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"Thread is Runing";
                mZipFunction->requestInterruption();
                mZipFunction->quit();
                qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"Thread is quit";
                QtSleep(1000);
                qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"Thread is quit and will start";
                mZipFunction->start();
            }
            else
            {
                qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"will start";
                mZipFunction->start();
            }
        }

        if (mZipConcernProgress)
        {
            mZipConcernProgress->setValue(0);
            mZipConcernProgress->show();
        }
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"flag is false";
    }
}

void Uiloader::sltParameterSetUp(QString key, QVariant value)
{
    int va = 0;

    if (!QString::compare(key, "liftflrvol"))
    {
        va = value.toInt();
        if (m_liftflrvol != va)
        {
            m_liftflrvol = va;
            mXmlFileChangeFlag = true;
        }
    }
    else if (!QString::compare(key, "liftarvvol"))
    {
        va = value.toInt();
        if (m_liftarvvol != va)
        {
            m_liftarvvol = va;
            mXmlFileChangeFlag = true;
        }
    }
    else if (!QString::compare(key, "admusicvol"))
    {
        va = value.toInt();
        if (m_admusicvol != va)
        {
            m_admusicvol = va;
            if (m_Player)
            {
                m_Player->setBackgroundPlayerVolume(m_admusicvol);
                qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"is "<<m_admusicvol;
//                m_Player->playBackgroundPlayer();
            }
            mXmlFileChangeFlag = true;
        }
    }
    else if (!QString::compare(key, "luminance"))
    {
        va = value.toInt();
        if (m_luminance != va)
        {
            m_luminance = va;
            mBackLightCtrl.setBacklightValue(m_luminance); // 这个背光亮度需要实时生效，使用 mBackLightCtrl 直接调用
            mXmlFileChangeFlag = true;
        }
    }
    else if (!QString::compare(key, "darklevel"))
    {
        va = value.toInt();
        if (m_standbyluminance != va)
        {
            m_standbyluminance = va;
            mScreenSaver.setStandByLuminance(m_standbyluminance);
            mXmlFileChangeFlag = true;
        }
    }
    else if (!QString::compare(key, "standby"))
    {
        va = value.toInt();
        if (m_standby != va)
        {
            m_standby = va;   // 只需要赋值，mScreenSaver不需要使用这个值
            mXmlFileChangeFlag = true;
        }
    }
    else if (!QString::compare(key, "rotation"))
    {
        va = value.toInt();
        if (va == 0 || va == 180)
        {
            if (0 == va)
            {
                m_rotation = 180;
                SetRotate(m_rotation);
            }
            else if (180 == va)
            {
                m_rotation = 0;
                SetRotate(m_rotation);
            }
            mXmlFileChangeFlag = true;
        }
        else if (va == 90 || va == 270)
        {
            if (m_rotation != va)
            {
                m_rotation = va;
                SetRotate(m_rotation);
            }
            mXmlFileChangeFlag = true;
        }
    }
    else if (!QString::compare(key, "layout"))
    {
        va = value.toInt();
        if (m_layout != va)
        {
            m_layout = va;
            SetLayout(m_layout);
            mXmlFileChangeFlag = true;
        }
    }
    else if (!QString::compare(key, "autotest"))
    {
        bool pAutoTest = value.toBool();
        if (m_autotest != pAutoTest)
        {
            m_autotest = pAutoTest;
            if (m_autotest)
            {
                recordSnapShot();
                if (!QString::compare("canopen", mProtocolType, Qt::CaseInsensitive))
                {
                    disconncetCanOpenSignal(true);
                }
                else if (!QString::compare("saf", mProtocolType, Qt::CaseInsensitive))
                {
                    disconnectSafSignal(true);
                }
                slot_AutoTest();
                QtSleep(10);
                mAutoTestTimer.setInterval(m_testfreq);
                mAutoTestTimer.start();
            }
            else
            {
                mAutoTestTimer.stop();
                QtSleep(10);
                if (!QString::compare("canopen", mProtocolType, Qt::CaseInsensitive))
                {
                    disconncetCanOpenSignal(false);
                }
                else if (!QString::compare("saf", mProtocolType, Qt::CaseInsensitive))
                {
                    disconnectSafSignal(false);
                }
                QtSleep(10);
                recoverySnapShot();
            }
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
        initZipFunction();
        QtSleep(100);
        mFtpDownLoadFileName = fileName;
        if (mZipFunction)
        {
            mZipFunction->setZipFlag(true, false); // setting Zip is work
            mZipFunction->setZipFilePath("/usr/bst/usrfs/", ftpFilePath + fileName);
            QtSleep(50);
            mZipFunction->start();
        }

        if (mZipConcernProgress)
        {
            mZipConcernProgress->setValue(0);
            mZipConcernProgress->show();
        }
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
        QtSleep(50);
        system("chmod 777 -R /home/ftp/pub/");
        QtSleep(10);
        if (mZipConcernProgress)
        {
            mZipConcernProgress->hide();
            mZipConcernProgress->reset();
        }
        QtSleep(100);
        if (mWiFi)
        {
            mWiFi->replyDownLoadFile(true);
        }
        if (m_messageBar)
        {
            QtSleep(1000);
            m_messageBar->hide();
        }
    }
    else if (!QString::compare("unZip", zip, Qt::CaseInsensitive))
    {
        if (m_messageBar)
        {
            QtSleep(1000);
            m_messageBar->hide();
        }
        if (mWiFi)
        {
            mWiFi->unZipStatus("success");
        }
    }
    if (mZipConcernProgress)
    {
        mZipConcernProgress->hide();
    }
    emit sigDestroyZip();
}

void Uiloader::setProgressGeometry()
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
}

void Uiloader::setWifiIconGeometry()
{
    if (mLabelWIFI)
    {
        if (Horizontal == m_themeDirection)
        {
            mLabelWIFI->setGeometry(420, 5, 45, 20);
        }
        else if (Vertical == m_themeDirection)
        {
            mLabelWIFI->setGeometry(200, 5, 45, 20);
        }
    }
}

void Uiloader::recordSnapShot()
{
    if (mFloor)
    {
        mLastGe = mFloor->getGeWei();
        mLastShi = mFloor->getShiWei();
        mLastBai = mFloor->getBaiWei();
    }
    if (mIcon)
    {
        mLastStackWidgetIndex = mIcon->getStackWidgetIndex();
    }
}

void Uiloader::recoverySnapShot()
{
    SetLayout(m_layout);
    SetRotate(m_rotation);
    if (mIcon)
    {
        mIcon->setChangedGround(mLastStackWidgetIndex);
    }
    if (mFloor)
    {
        mFloor->setVisible(true);
        mFloor->setFloor(mLastGe, mLastShi, mLastBai);
    }
    if (mArrow)
    {
        mArrow->setArrowType(UiArrow::NoArrow, mScrollSpeed);
    }
}

void Uiloader::initZipFunction()
{
    mZipFunction = new ZipFunction();
    qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"initZipFunction";
    if (mZipFunction)
    {
        connect(mZipFunction, &ZipFunction::zipProgress, this, &Uiloader::sltZipProgress, Qt::QueuedConnection);
        connect(mZipFunction, &ZipFunction::unZipProgress, this, &Uiloader::sltUnZipProgress, Qt::QueuedConnection);
        connect(mZipFunction, &ZipFunction::end, this, &Uiloader::sltZipOrUnZipEnded, Qt::QueuedConnection);
        connect(mZipFunction, &ZipFunction::error, this, &Uiloader::sltUnzipFileErrors, Qt::QueuedConnection);
    //    connect(&mZipFunction, &ZipFunction::message, this, &Uiloader::sltShowZipConcernMessage, Qt::QueuedConnection);
        connect(mZipFunction, &ZipFunction::workStart, this, &Uiloader::sltShowZipConcernMessage, Qt::QueuedConnection);
    }
}

void Uiloader::sltZipProgress(quint32 pro)
{
    if (mZipConcernProgress)
    {
        mZipConcernProgress->setValue(pro);
        if (!mZipConcernProgress->isVisible())
        {
            mZipConcernProgress->reset();
            mZipConcernProgress->show();
        }
    }
    if (m_messageBar)
    {
        if (pro < 100)
        {
            if (m_messageBar->m_messageLabel)
            {
//                QString tmp = " Zip " + mFtpDownLoadFileName.split("/").last() + "...\n";
                QString tmp = " Zip...\n";
                m_messageBar->m_messageLabel->setText(tmp);
                m_messageBar->show();
            }
        }
    }
    if (100 == pro)
    {
        if (m_messageBar->m_messageLabel)
        {
//            QString tmp = " Zip " + mFtpDownLoadFileName.split("/").last() + "Successed!\n";
            QString tmp = QString(" Zip Successed!\n");
            m_messageBar->m_messageLabel->setText(tmp);
            m_messageBar->show();
        }
    }
}

void Uiloader::sltUnZipProgress(quint32 pro)
{
    static int num = 0;
    if (mZipConcernProgress)
    {
        if (!mZipConcernProgress->isVisible())
        {
            mZipConcernProgress->reset();
            mZipConcernProgress->show();
        }
        mZipConcernProgress->setValue(pro);
    }
    if (0 == num)
    {
        num = 1;
        if (m_messageBar)
        {
            if (pro < 100)
            {
                if (m_messageBar->m_messageLabel)
                {
//                    QString tmp = " unZip " + mFtpSendFileName.split("/").last() + "...\n";
                    QString tmp = QString(" unZip ...\n");
                    m_messageBar->m_messageLabel->setText(tmp);
                }
            }
        }
    }
    if (100 == pro)
    {
        if (m_messageBar->m_messageLabel)
        {
//            QString tmp = " unZip file " + mFtpSendFileName.split("/").last() + "Successed!\n";
            QString tmp = QString(" unZip file Successed!\n");
            m_messageBar->m_messageLabel->setText(tmp);
        }
        num = 0;
    }
}

void Uiloader::sltHideProgress()
{
    if (mZipConcernProgress)
    {
        mZipConcernProgress->hide();
        mZipConcernProgress->reset();
    }
}

void Uiloader::sltUploadProgress(int progress)
{
    if (mZipConcernProgress)
    {
        if (mIsSendZipFileProgress != progress)
        {
            mIsSendZipFileProgress = progress;
        }
        mIsSendZipFile = true;
        mZipConcernProgress->setValue(progress);
        if (!mZipConcernProgress->isVisible())
        {
            mZipConcernProgress->show();
        }
    }
    if (m_messageBar)
    {
        if (progress < 100)
        {
            if (m_messageBar->m_messageLabel)
            {
//                QString tmp = " Transfer " + mFtpSendFileName.split("/").last() + "...\n";
                QString tmp = QString(" Transfer zip file ...\n");
                m_messageBar->m_messageLabel->setText(tmp);
                m_messageBar->show();
            }
        }
        else
        {
            if (m_messageBar->m_messageLabel)
            {
//                QString tmp = " Transfer file" + mFtpSendFileName.split("/").last() + " Successed!\n";
                QString tmp = QString(" Transfer file Successed!\n");
                m_messageBar->m_messageLabel->setText(tmp);
                m_messageBar->show();
            }
        }
    }
}

void Uiloader::sltUnzipFileErrors()
{
    qDebug()<<__PRETTY_FUNCTION__<<"is error!";
    if (mWiFi)
    {
        mWiFi->unZipStatus("error");
    }
    if (m_messageBar->m_messageLabel)
    {
//        QString tmp = " UnZip " + mFtpSendFileName.split("/").last() + " Failed!\n";
        QString tmp = QString(" UnZip Failed!\n");
        m_messageBar->m_messageLabel->setText(tmp);
        m_messageBar->show();
    }
}

void Uiloader::sltRebootMachine(bool flag)
{
    if (flag)
    {
        QtSleep(10);
        sltSaveXmlFile();
        QtSleep(100);
        system("sync");
        QtSleep(100);
        system("reboot");
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<flag<<"is false do nothing";
    }
}

void Uiloader::sltShowZipConcernMessage(QString msg)
{
    if (m_messageBar)
    {
        if (m_messageBar->m_messageLabel)
        {
            QString tmp;
            QString msg2 = msg;
            msg2.insert(0, " ");
            msg2.append("\n");
            m_messageBar->m_messageLabel->clear();
            if (msg.contains("unzip", Qt::CaseInsensitive))
            {
                tmp += " start unZip...\n";
            }
            else if (msg.contains("zip", Qt::CaseInsensitive))
            {
                tmp += " start zip...\n";
            }
            tmp += msg2;
            m_messageBar->m_messageLabel->setText(tmp);
        }
        m_messageBar->show();
    }
}
