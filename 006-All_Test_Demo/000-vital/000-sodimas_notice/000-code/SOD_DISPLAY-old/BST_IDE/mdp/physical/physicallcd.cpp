#include "physicallcd.h"
#include "deviceinc.h"
#include "logicthememanager.h"

#ifdef LINUX
#define VIDEO_DISPLAY_ON		_IOW('v', 24, unsigned int)
#define VIDEO_DISPLAY_OFF		_IOW('v', 25, unsigned int)

#define D_FB_DEV        QString("/dev/fb0")
#endif

#ifdef WINDOWS
CGammaRamp::CGammaRamp()
{
    //Initialize all variables.
    hGDI32 = NULL;
    hScreenDC = NULL;
    pGetDeviceGammaRamp = NULL;
    pSetDeviceGammaRamp = NULL;
}

CGammaRamp::~CGammaRamp()
{
    FreeLibrary();
}


BOOL CGammaRamp::LoadLibrary()
{
    BOOL bReturn = FALSE;

    FreeLibrary();
    //Load the GDI library.
    hGDI32 = ::LoadLibrary(L"gdi32.dll");
    if (hGDI32 != NULL)
    {
        //Get the addresses of GetDeviceGammaRamp and SetDeviceGammaRamp API functions.
        pGetDeviceGammaRamp = (Type_SetDeviceGammaRamp)GetProcAddress(hGDI32, "GetDeviceGammaRamp");
        pSetDeviceGammaRamp = (Type_SetDeviceGammaRamp)GetProcAddress(hGDI32, "SetDeviceGammaRamp");

        //Return TRUE only if these functions exist.
        if (pGetDeviceGammaRamp == NULL || pSetDeviceGammaRamp == NULL)
            FreeLibrary();
        else
            bReturn = TRUE;
    }

    return bReturn;
}


void CGammaRamp::FreeLibrary()
{
    //Free the GDI library.
    if (hGDI32 != NULL)
    {
        ::FreeLibrary(hGDI32);
        hGDI32 = NULL;
    }
}


BOOL CGammaRamp::LoadLibraryIfNeeded()
{
    BOOL bReturn = FALSE;

    if (hGDI32 == NULL)
        LoadLibrary();

    if (pGetDeviceGammaRamp != NULL && pSetDeviceGammaRamp != NULL)
        bReturn = TRUE;

    return bReturn;
}


BOOL CGammaRamp::SetDeviceGammaRamp(HDC hDC, LPVOID lpRamp)
{
    //Call to SetDeviceGammaRamp only if this function is successfully loaded.
    if (LoadLibraryIfNeeded())
        return pSetDeviceGammaRamp(hDC, lpRamp);
    else
        return FALSE;
}


BOOL CGammaRamp::GetDeviceGammaRamp(HDC hDC, LPVOID lpRamp)
{
    //Call to GetDeviceGammaRamp only if this function is successfully loaded.
    if (LoadLibraryIfNeeded())
        return pGetDeviceGammaRamp(hDC, lpRamp);
    else
        return FALSE;

}


BOOL CGammaRamp::SetBrightness(HDC hDC, WORD wBrightness)
{
    /*
    Changes the brightness of the entire screen.
    This function may not work properly in some video cards.

    The wBrightness value should be a number between 0 and 255.
    128 = Regular brightness
    above 128 = brighter
    below 128 = darker

    If hDC is NULL, SetBrightness automatically load and release
    the display device context for you.

    */
    BOOL bReturn = FALSE;
    HDC hGammaDC = hDC;

    //Load the display device context of the entire screen if hDC is NULL.
    if (hDC == NULL)
        hGammaDC = GetDC(NULL);

    if (hGammaDC != NULL)
    {
        //Generate the 256-colors array for the specified wBrightness value.
        WORD GammaArray[3][256];

        for (int iIndex = 0; iIndex < 256; iIndex++)
        {
            int iArrayValue = iIndex * (wBrightness + 128);

            if (iArrayValue > 65535)
                iArrayValue = 65535;

            GammaArray[0][iIndex] =
                    GammaArray[1][iIndex] =
                    GammaArray[2][iIndex] = (WORD)iArrayValue;

        }

        //Set the GammaArray values into the display device context.
        bReturn = SetDeviceGammaRamp(hGammaDC, GammaArray);
    }

    if (hDC == NULL)
        ReleaseDC(NULL, hGammaDC);

    return bReturn;
}
#endif

PhysicalLCD::PhysicalLCD(QObject *parent) :
    PhysicalDevice(parent)
{
    mTouchEn = false;
    mOpened = true;
    mCurLevel = -1;
#ifdef WINDOWS
    mGammaRamp = new CGammaRamp;
#elif LINUX
    m_File = new QFile;
    m_File->setFileName(D_FB_DEV);
    m_File->open(QIODevice::ReadWrite|QIODevice::Unbuffered);
#elif PHONE

#endif

    mMonitorWait.SetTimeout(500);

    mMonitorCount = 0;

    mStandByTimeoutCount = 0;
    mStandbyEn = true;
    mStandbyMode = 0;
    mSrnSave.setnum(100, 100, 36000);//>@最小10s，最大1小时
    mSrnSaveHold.setnum(10, 50, 600);//>@最小1s，最大1分钟

    mSrnSaveEn = true;   //>@一开机就设置屏保有效。
    mSrnSaveMode = 1;

    mSrnSaveForm = 0;
    mSrnSaveModeFlag = false;

    mMode = false;

    connect(this, SIGNAL(sSrnSaveMode(bool)), this, SLOT(slot_SrnSaveMode(bool)), Qt::QueuedConnection);
}

PhysicalLCD::~PhysicalLCD()
{
    mMonitorCount = 0;
#ifdef WINDOWS
    delete mGammaRamp;
#elif LINUX
    if(m_File)
        m_File->close();
#endif
}

void PhysicalLCD::slot_Initialize()
{
    if(!mSrnSaveForm)  //>@使用此可以去除Scene的蓝色边框。
    {
        mSrnSaveForm = new SrnSaveForm;
#ifndef WINDOWS
        mSrnSaveForm->setGeometry(qApp->desktop()->rect());
#else
        mSrnSaveForm->setGeometry(0,0,100,100);
#endif
        QLabel *tmpLabel = mSrnSaveForm->GetLogoLabel();
        tmpLabel->setScaledContents(true);
        QString tmpSrnPix = "/usr/bst/BOOT/ScrSave.BMP";
        QFile tmpFile(tmpSrnPix);
        if(tmpFile.exists())
            tmpLabel->setPixmap(QPixmap("/usr/bst/BOOT/ScrSave.BMP"));
        else
            tmpLabel->setPixmap(QPixmap("/usr/bst/BOOT/logo.png"));

        mSrnSaveForm->show();
        mSrnSaveForm->lower();
    }
    PhysicalDevice::slot_Initialize();
#ifdef DISPLAY
    m_Com = VAR_CAST<PhysicalCOM*>(m_DevManager->m_PHDevList.value(P_COM));
    m_Saf = VAR_CAST<PhysicalSAF*>(m_DevManager->m_PHDevList.value(P_SAF));
#endif
}

void PhysicalLCD::ReCalibrate(bool pFroce)
{
    //>@查看有无校准文件，如果没有则进行校准。如果pFroce为true，则不论如何都进行校准
    if(!pFroce)
    {
        if(QFile::exists("/etc/pointeal"))
            return;
    }
    //>@开始校准
#ifdef LINUX
#ifdef IMX6X
    QDir tmpDir;
    tmpDir.cd("/usr/local/tslib/bin/");
    QProcess tmpProcess;
    tmpProcess.start("/usr/local/tslib/bin/ts_calibrate");
    tmpProcess.waitForFinished(60000);
    if(tmpProcess.state() != QProcess::NotRunning)
        tmpProcess.kill();
    tmpDir.cd("/");
#else
    QProcess tmpProcess;
    tmpProcess.start("ts_calibrate");
    tmpProcess.waitForFinished(60000);
    if(tmpProcess.state() != QProcess::NotRunning)
        tmpProcess.kill();
#endif
#endif
}


bool PhysicalLCD::SetEnable(bool pFlag, E_DEVMODE pDevMode, bool pForce)
{
    //L_IDE_TRACE_STR("PhysicalLCD::SetEnable");
    if(pDevMode < mDevMode){
        return false;
    }
    if(pFlag)
    {
        mDevMode = DM_NORMAL;
        if(mEnable == false)
        {
            mEnable = true;
#ifdef DISPLAY
            mMonitorWait.update(mMonitorCount, m_DevManager->m_DevMonitorFreq);
            slot_FeedLCD();
#endif
        }


#ifdef DISPLAY
        //>@查询有无箭头
        if(!m_Com.isNull())
        {
            if(m_Com->mHasArrow){
                SetSrnSaveEn(false);
                SetStandbyEn(false);
            }else{
                SetSrnSaveEn(true);
                SetStandbyEn(true);
            }

        }
        if(!m_Saf.isNull())
        {
            SetSrnSaveEn(!m_Saf->mHasArrow);
            SetStandbyEn(!m_Saf->mHasArrow);
        }
#endif

        //>@当从网络连接模式退出时，需要重新初始化可能被修改的参数
        if(pDevMode == DM_NET)
        {
            if(mLuminanceBk != mLuminance.mNum)
                SetPara(QString("Luminance"), QString::number(mLuminanceBk), true);
            if(mStandByBk != mStandBy.mNum)
                SetPara(QString("StandBy"), QString::number(mStandByBk), true);
            if(mDarkLevelBk != mDarkLevel.mNum)
                SetPara(QString("DarkLevel"), QString::number(mDarkLevelBk), true);
            if(mRotationBk != mRotation)
                SetPara(QString("Rotation"), QString::number(mRotationBk), true);
        }
    }
    else
    {
        mDevMode = pDevMode;
        if(pDevMode != DM_STANDBY)
            mEnable = false;
        //>@根据模式设置屏保模式

        if(pDevMode == DM_MDISK || pDevMode == DM_NET){  //>@进入U盘升级以及APP模式时，退出屏保
            //SetSrnSaveEn(false);
        }

        //>@当进入网络连接模式时，需要保存可能被修改的参数
        if(pDevMode == DM_NET)
        {
            mLuminanceBk = mLuminance.mNum;
            mStandByBk = mStandBy.mNum;
            mDarkLevelBk = mDarkLevel.mNum;
            mRotationBk = mRotation;
        }
    }
    return true;
}

//>@表示使能Standby
bool PhysicalLCD::SetStandbyEn(bool pFlag)
{
    if(mStandbyEn != pFlag)
    {
        if(pFlag){
            mStandByTimeoutCount = mMonitorCount + mStandBy.mNum/100;
            //L_IDE_TRACE_STR("SetStandbyEn true");
        }else{
            //L_IDE_TRACE_STR("SetStandbyEn false");
        }
        mStandbyEn = pFlag;
        return true;
    }
    return false;
}

//>@表示使能屏保
bool PhysicalLCD::SetSrnSaveEn(bool pFlag)
{
    if(mSrnSaveEn != pFlag)
    {
        if(pFlag)
        {
            if(mSrnSave.mNum)
                mSrnSaveTimeoutCount = (mMonitorCount + mSrnSave.mNum) % D_MAXTIMEOUT;
            mSrnSaveMode = 1;
        }
        else
        {
            emit sSrnSaveMode(false);
            mSrnSaveMode = 1;
        }
        mSrnSaveEn = pFlag;
        return true;
    }
    return false;
}

void PhysicalLCD::slot_FeedLCD()  //>@重置lcd standby模式
{

    if(mStandbyMode == 0)  //>@如果已经进入Standby，则退出
    {
        emit sStandby(false);
        SetLuminance(mLuminance.mNum);
        mStandbyMode = 1;
    }
    if(mStandBy.mNum)
        mStandByTimeoutCount = mMonitorCount + mStandBy.mNum/100;

    if(mSrnSaveMode == 0)
    {
        emit sSrnSaveMode(false);
        mSrnSaveMode = 1;
    }

    if(mSrnSave.mNum)
        mSrnSaveTimeoutCount = (mMonitorCount + mSrnSave.mNum) % D_MAXTIMEOUT;
}

quint32 PhysicalLCD::Monitor(quint32 pCount)  //>@100ms进入一次
{
    mMonitorCount = pCount;
    if(!mEnable)
    {
        SetLuminance(mLuminance.mNum);
        return 0;
    }
    if(!mInitialized)
        return 0;
#ifdef DISPLAY
    if(mMonitorWait.isValid(pCount, m_DevManager->m_DevMonitorFreq))
    {
        if(mStandbyEn && mStandBy.mNum)
        {
            if(mStandbyMode == 1)
            {
                if(mMonitorCount >= mStandByTimeoutCount) //>@进入standy模式
                {
                    L_IDE_TRACE_STR("Enter Standby");
                    //>@进入standby模式
                    emit sStandby(true);
                    mStandbyMode = 0;
                    SetLuminance(mDarkLevel.mNum);
#ifdef DISPLAY
                    emit sSetDevices(false, DM_STANDBY);
#endif
                }
            }
        }
        else
        {
            SetLuminance(mLuminance.mNum);
        }

        //>@屏保
        if(mSrnSaveEn && mSrnSave.mNum)
        {
            if(mSrnSaveMode == 0)  //>@已经在屏保模式
            {
                //IDE_TRACE();
                if(mMonitorCount >= mSrnSaveHoldCount) //>@退出屏保模式
                {
                    emit sSrnSaveMode(false);
                    mSrnSaveTimeoutCount = (mMonitorCount + mSrnSave.mNum) % D_MAXTIMEOUT;
                    mSrnSaveMode = 1;
                }
            }
            else if(mSrnSaveMode == 1)
            {
                if(mMonitorCount >= mSrnSaveTimeoutCount) //>@进入屏保模式
                {
                    L_IDE_TRACE_STR("Enter SrnSave");
                    emit sSrnSaveMode(true);
                    mSrnSaveHoldCount = (mMonitorCount + mSrnSaveHold.mNum) % D_MAXTIMEOUT;
                    mSrnSaveMode = 0;
                }
            }
            //IDE_TRACE();
        }
        else
        {
            emit sSrnSaveMode(false);
            //IDE_TRACE();
        }
    }
#endif
}

void PhysicalLCD::slot_SrnSaveMode(bool pEnable)
{
    if(!mSrnSaveForm)
    {
        IDE_TRACE();
        return;
    }
    if(mSrnSaveModeFlag == pEnable)
    {
        IDE_TRACE();
        return;
    }
    mSrnSaveModeFlag = pEnable;
    if(pEnable)
    {
        L_IDE_TRACE_STR("mSrnSaveForm raise");
        mSrnSaveForm->raise();
        mSrnSaveForm->activateWindow();
    }
    else
    {
        mSrnSaveForm->lower();
        L_IDE_TRACE_STR(" mSrnSaveForm lower");
        //mSrnSaveForm->hide();  //>@加上会出现蓝框
    }
}










bool PhysicalLCD::SetValue(QString pValue)
{
    if(pValue.isEmpty())
        return true;
    QStringList tmpList = pValue.split(';');
    for(int i=0;i<tmpList.count();i++)
    {
        QString tmpCmd = tmpList.at(i);
        if(tmpCmd.isEmpty())
            continue;
        IDE_TRACE_STR(tmpCmd);
        if(!tmpCmd.compare("close", Qt::CaseInsensitive))
        {
            Close();
        }
        else if(!tmpCmd.compare("open", Qt::CaseInsensitive))
        {
            Open();
        }
    }
    return true;
}

quint32 PhysicalLCD::InitPara()
{
    QDomNodeList tmpParaNodeList = m_ParaElement.childNodes();
    if(tmpParaNodeList.count() < 1)
        return 0;
    quint32 validcount = 0;
    QHBoxLayout* tmpHBoxLayout;
    for(int i=0;i<tmpParaNodeList.count();i++)
    {
        QDomElement tmpElement = tmpParaNodeList.at(i).toElement();
        if(tmpElement.isNull())
            continue;
        QString tmpName = tmpElement.tagName();
        if(!tmpName.compare("OverLayer"))
        {
            mOverlayers = tmpElement.text().split(';');
            tmpHBoxLayout = CreateCheck(tmpName, tmpElement.attribute("List").split(';'), mOverlayers);
        }
        else if(!tmpName.compare("Rotation"))
        {
            QString tmpValue = tmpElement.text();
            mRotation = tmpValue.toInt();
            mRotationBk = mRotation;
            tmpHBoxLayout = CreateEnum(tmpName, tmpElement.attribute("List").split(';'), tmpValue);
#ifdef X86

#endif
        }
        else if(!tmpName.compare("Touch"))
        {
            QString tmpValue = tmpElement.text();
            mTouchEn = (tmpValue.compare("true", Qt::CaseInsensitive))?(false):(true);
            tmpHBoxLayout = CreateBool(tmpName, tmpValue);
        }
        else if(!tmpName.compare("Luminance"))
        {
            QStringList tmpValueList = tmpElement.attribute("List").split(';');
            if(tmpValueList.count()==2)
            {
                mLuminance.setnum(tmpElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
            }
            SetLuminance(mLuminance.mNum);
            mLuminanceBk = mLuminance.mNum;
            tmpHBoxLayout = CreateNum(tmpName, mLuminance.mNum, mLuminance.mMinNum, mLuminance.mMaxNum);
        }
        else if(!tmpName.compare("StandBy"))
        {
            QStringList tmpValueList = tmpElement.attribute("List").split(';');
            if(tmpValueList.count()==2)
            {
                mStandBy.setnum(tmpElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
            }
            mStandByBk = mStandBy.mNum;
            tmpHBoxLayout = CreateNum(tmpName, mStandBy.mNum, mStandBy.mMinNum, mStandBy.mMaxNum);
        }
        else if(!tmpName.compare("DarkLevel"))
        {
            QStringList tmpValueList = tmpElement.attribute("List").split(';');
            if(tmpValueList.count()==2)
            {
                mDarkLevel.setnum(tmpElement.text().toInt(), tmpValueList.at(0).toInt(), tmpValueList.at(1).toInt());
            }
            mDarkLevelBk = mDarkLevel.mNum;
            tmpHBoxLayout = CreateNum(tmpName, mDarkLevel.mNum, mDarkLevel.mMinNum, mDarkLevel.mMaxNum);
        }
        else if(!tmpName.compare("SrnSave"))
        {
            int tmpNum = 0, tmpMinNum = 0, tmpMaxNum = 0;
            QStringList tmpValueList = tmpElement.attribute("List").split(';');
            if(tmpValueList.count()==2)
            {
                tmpNum = tmpElement.text().toInt();
                tmpMinNum = tmpValueList.at(0).toInt();
                tmpMaxNum = tmpValueList.at(1).toInt();
                if(m_DevManager->m_DevMonitorFreq)
                {
                    mSrnSave.setnum(tmpNum/m_DevManager->m_DevMonitorFreq, tmpMinNum/m_DevManager->m_DevMonitorFreq, tmpMaxNum/m_DevManager->m_DevMonitorFreq);
                    if(mSrnSave.mNum)
                        mSrnSaveTimeoutCount = (mMonitorCount + mSrnSave.mNum) % D_MAXTIMEOUT;
                }
            }
            tmpHBoxLayout = CreateNum(tmpName, tmpNum, tmpMinNum, tmpMaxNum);
        }
        else if(!tmpName.compare("SrnSaveHold"))
        {
            int tmpNum = 0, tmpMinNum = 0, tmpMaxNum = 0;
            QStringList tmpValueList = tmpElement.attribute("List").split(';');
            if(tmpValueList.count()==2)
            {
                tmpNum = tmpElement.text().toInt();
                tmpMinNum = tmpValueList.at(0).toInt();
                tmpMaxNum = tmpValueList.at(1).toInt();
                if(m_DevManager->m_DevMonitorFreq)
                    mSrnSaveHold.setnum(tmpNum/m_DevManager->m_DevMonitorFreq, tmpMinNum/m_DevManager->m_DevMonitorFreq, tmpMaxNum/m_DevManager->m_DevMonitorFreq);
            }
            tmpHBoxLayout = CreateNum(tmpName, tmpNum, tmpMinNum, tmpMaxNum);
        }
        else
        {
            continue;
        }
        validcount++;
        m_SettingGroup->m_VLayout->addWidget(CreateLine());
        m_SettingGroup->m_VLayout->addLayout(tmpHBoxLayout);
    }
    slot_FeedLCD();
    return validcount;
}

void PhysicalLCD::UpdatePara(QObject* pObject)
{
    QString tmpObjectName = pObject->objectName();
    QDomElement tmpElement = m_ParaElement.firstChildElement(tmpObjectName);
    if(tmpElement.isNull())
        return;
    QString tmpName = tmpElement.tagName();
    if(!tmpName.compare("OverLayer"))
    {
        CheckList *tmpComponent = VAR_CAST<CheckList *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyCheck(tmpComponent, tmpElement, mOverlayers, tmpComponent->GetSelectList()))
            {
            }
        }
    }
    else if(!tmpName.compare("Rotation"))
    {
        ComboBox *tmpComponent = VAR_CAST<ComboBox *>(pObject);
        if(tmpComponent)
        {
            QString tmpRotation;
            if(!ModifyEnum(tmpComponent, tmpElement, tmpRotation, tmpComponent->currentText()))
            {
            }
            else
            {
                mRotation = tmpRotation.toInt();
                //>@PhysicalDial中会执行一次修改旋转角度，也即会运行此处，但此时LogicThemeManager还没有加载，因此并没有什么BUG。
                LogicThemeManager *tmpTheme = 0;
#ifdef IDE
                tmpTheme = VAR_CAST<LogicThemeManager*>(m_DevManager->m_LGDevList.value(L_ThemeManager));
#elif DISPLAY
                tmpTheme = VAR_CAST<LogicThemeManager*>(m_DevManager->m_LGDevList.value(L_ThemeManager));
#endif
                if(tmpTheme)
                {
                    tmpTheme->RotateTheme(mRotation);
                }
            }
        }
    }
    else if(!tmpName.compare("Touch"))
    {
        Switch *tmpComponent = qobject_cast<Switch *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyBool(tmpComponent, tmpElement, mTouchEn, tmpComponent->isChecked()))
            {
            }
        }
    }
    else if(!tmpName.compare("Luminance"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, tmpElement, mLuminance.mNum, tmpComponent->value(), mLuminance.mMinNum, mLuminance.mMaxNum))
            {
            }
            else
            {
                SetLuminance(mLuminance.mNum);
            }
        }
    }
    else if(!tmpName.compare("StandBy"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, tmpElement, mStandBy.mNum, tmpComponent->value(), mStandBy.mMinNum, mStandBy.mMaxNum))
            {
            }
            else
            {
                if(mStandBy.mNum == 0){
                    L_IDE_TRACE_STR("SetStandbyEn 1");
                    SetStandbyEn(false);
                }else{
                    L_IDE_TRACE_STR("SetStandbyEn 2");
                    SetStandbyEn(true);
                }
            }
        }
    }
    else if(!tmpName.compare("DarkLevel"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            if(!ModifyNum(tmpComponent, tmpElement, mDarkLevel.mNum, tmpComponent->value(), mDarkLevel.mMinNum, mDarkLevel.mMaxNum))
            {
            }
        }
    }
    else if(!tmpName.compare("SrnSave"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            int tmpNum;
            if(!ModifyNum(tmpComponent, tmpElement, tmpNum, tmpComponent->value(), mSrnSave.mMinNum*m_DevManager->m_DevMonitorFreq, mSrnSave.mMaxNum*m_DevManager->m_DevMonitorFreq))
            {
            }
            else
            {
                mSrnSave.mNum = tmpNum;
                if(mSrnSave.mNum == 0){
                    SetSrnSaveEn(false);
                }else{
                    SetSrnSaveEn(true);
                }
            }
        }
    }
    else if(!tmpName.compare("SrnSaveHold"))
    {
        SpinEdit *tmpComponent = VAR_CAST<SpinEdit *>(pObject);
        if(tmpComponent)
        {
            int tmpNum;
            if(!ModifyNum(tmpComponent, tmpElement, tmpNum, tmpComponent->value(), mSrnSaveHold.mMinNum*m_DevManager->m_DevMonitorFreq, mSrnSaveHold.mMaxNum*m_DevManager->m_DevMonitorFreq))
            {
            }
            else
            {
                mSrnSaveHold.mNum = tmpNum;
            }
        }
    }
}

//>@0~100，针对不同的设备进行转换
bool PhysicalLCD::SetLuminance(int pValue)
{
    if(!mOpened)  //>@如果是屏幕关闭，则不能设置背光
    {
        pValue = 0;
    }
    if(pValue < 0)
        pValue = 0;
    else if(pValue > 100)
        pValue = 100;
    if(pValue == mCurLevel)
        return true;
#ifdef DISPLAY
    int tmpValue = 0;
#ifdef WINDOWS
    //>@0~255
    tmpValue = pValue*255/100;
    mGammaRamp->SetBrightness(NULL, 127);
#elif LINUX
#ifdef N329
    //>@10为最亮，190为最暗，也即0~100对应的差度为1.8
    tmpValue = 190-(pValue*18/10);
    IDE_DEBUG(QString("Bright light is %1").arg(tmpValue));
    ioctl(m_File->handle(), IOCTL_LCD_BRIGHTNESS, &tmpValue);
#elif IMX6X
    //>@0最小，248最大
    tmpValue = pValue*248/100;
    QString tmpCmd = QString("echo %1 > /sys/class/backlight/pwm-backlight.0/brightness").arg(tmpValue);
    system(tmpCmd.toLatin1().data());
    IDE_TRACE_STR(tmpCmd);
#endif
#endif
#endif
    mCurLevel = pValue;
    return true;
}

//>@进入特殊模式，进入此模式后，背光一直保持正常，不响应open和close
void PhysicalLCD::GotoMode(bool pEnable)
{
    Open();
    mMode = pEnable;
}

bool PhysicalLCD::Close()
{
    if(!mMode && mOpened)
    {
        SetLuminance(0);
        mOpened = false;  //>@如果是要关闭屏幕，必须在设置背光之后
        emit sSetDevices(false, DM_SLEEP);
    }
    return true;
}

bool PhysicalLCD::Open()
{
    if(!mMode && !mOpened)  //>@Sodimas的CANOPEN控制器会在进入operation后给一个open屏的命令，如果控制器此时在大屏模式下，那么会在发送open命令后再发一个进入大屏的命令，此时如果重复执行open命令（因为设备默认是开启的），会出现大屏文本下楼层与箭头可以显示的bug。
    {
        mOpened = true;  //>@如果是要开启屏幕，必须在设置背光之前
        SetLuminance(mLuminance.mNum);
        emit sSetDevices(true, DM_SLEEP);
    }
    return true;
}



