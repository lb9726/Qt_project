#include "devicesetting.h"
#include "ui_devicesetting.h"

DeviceManager::DeviceManager(QWidget *parent) :
    DeviceSetBase(parent),
    ui(new Ui::DeviceManageForm)
{
    ui->setupUi(this);

    m_SelectPath = QString("/");

    m_LanRouter->m_DevManager = this;
    m_Initialize = true;

    m_ShowFloor = true;
    mFloor = -1;
    mFloorInfo = FLOOR_INFO();
    mArrow = 1;
    mFunction = 40;
    mAudio = 0;

    QString tmpVersion = GetAppVersion(qApp->applicationFilePath());
    if(tmpVersion.isEmpty())
        tmpVersion = "V0.0.0.0";
    if(!tmpVersion.startsWith("V"))
        tmpVersion.insert(0, "V");
    ui->label_version->setText(tmpVersion);

    m_ArrowSpeed[0] = 6000;
    m_ArrowSpeed[1] = 3000;
    m_ArrowSpeed[2] = 2000;
    m_ArrowSpeed[3] = 1500;

    m_LangListOperate = 0;

    mRotation = 0;
    mNetworkList = ui->list_device;

    mAutoTestTimer.setInterval(3000);
    mAutoTestCount = 0;
    connect(&mAutoTestTimer, SIGNAL(timeout()), this, SLOT(slot_AutoTest()));

    m_BaseTimer.setInterval(1000);
    connect(&m_BaseTimer, SIGNAL(timeout()), this, SLOT(slot_TimeTick()));

    //>@创建目录
    CreatPath(GlobalPara.m_SoftwareDir + D_DEV_DIR);
    CreatPath(GlobalPara.m_SoftwareDir + D_DEV_DIR + D_DEV_OFFLINE_DIR);
    CreatPath(GlobalPara.m_SoftwareDir + D_DEV_DIR + D_DEV_ONLINE_DIR);
    CreateDeviceDir(GlobalPara.m_SoftwareDir + D_DEV_DIR + D_DEV_ONLINE_DIR + D_DEV_FACTORY_DIR);  //>@创建工程模式目录

    CreatPath(GlobalPara.m_SoftwareDir + D_COM_DIR);
    CreatPath(GlobalPara.m_SoftwareDir + D_ETC_DIR);
    CreatPath(GlobalPara.m_SoftwareDir + D_UPGRADE_DIR);
    CreatPath(GlobalPara.m_SoftwareDir + D_UPGRADE_DIR + D_EXEDIR);
    CreatPath(GlobalPara.m_SoftwareDir + D_UPGRADE_DIR + D_LOGO_DIR);
    CreatPath(GlobalPara.m_SoftwareDir + D_UPGRADE_DIR + D_SYSTEMDIR);

    connect(ui->btn_exit, SIGNAL(clicked()), this, SLOT(slot_Exit()));
    //connect(ui->btn_enter, SIGNAL(clicked()), this, SLOT(slot_Enter()));

    connect(&mLanguageGroup, SIGNAL(buttonClicked(int)), this, SLOT(slot_SelectLanguage(int)));
    mLanguageGroup.addButton(ui->lang_en_banner, 0);
    mLanguageGroup.addButton(ui->lang_fra_banner, 1);
    mLanguageGroup.addButton(ui->lang_deu_banner, 2);
    mLanguageGroup.addButton(ui->lang_ned_banner, 3);

    connect(&mProjectGroup, SIGNAL(buttonClicked(int)), this, SLOT(slot_ProjectOperate(int)));
    mProjectGroup.addButton(ui->btn_download, 0);
    mProjectGroup.addButton(ui->btn_2_3_import, 11);
    mProjectGroup.addButton(ui->btn_modify, 2);
    mProjectGroup.addButton(ui->btn_2_1_create, 13);
    mProjectGroup.addButton(ui->btn_remove, 4);
    mProjectGroup.addButton(ui->btn_export, 7);
    mProjectGroup.addButton(ui->btn_close, 8);

    mProjectGroup.addButton(ui->btn_1_1_local, 22);
    mProjectGroup.addButton(ui->btn_1_1_new, 23);

    mProjectGroup.addButton(ui->btn_wifi, 31);
    mProjectGroup.addButton(ui->btn_usb, 32);

    connect(&mProtocolGroup, SIGNAL(buttonClicked(int)), this, SLOT(slot_ProtocolOperate(int)));
    mProtocolGroup.addButton(ui->radio_saf, 0);
    mProtocolGroup.addButton(ui->radio_canopen, 1);

    connect(&mProModifyGroup, SIGNAL(buttonClicked(int)), this, SLOT(slot_ProModify(int)));
    //mProModifyGroup.addButton(ui->btn_autotest, 0);
    mProModifyGroup.addButton(ui->btn_language, 1);
    mProModifyGroup.addButton(ui->btn_layout, 2);
    mProModifyGroup.addButton(ui->btn_setting, 4);
    mProModifyGroup.addButton(ui->btn_style, 5);
    mProModifyGroup.addButton(ui->btn_explorer, 6);
    mProModifyGroup.addButton(ui->btn_application, 7);

    connect(&mLayoutModifyGroup, SIGNAL(buttonClicked(int)), this, SLOT(slot_LayoutOperate(int)));
    mLayoutModifyGroup.addButton(ui->radio_srn_arrow, 0);
    mLayoutModifyGroup.addButton(ui->radio_srn_floor, 1);

    connect(&mScreenSizeGroup, SIGNAL(buttonClicked(int)), this, SLOT(slot_ScreenSizeOperate(int)));
    mScreenSizeGroup.addButton(ui->radio_srn_4, 0);
    mScreenSizeGroup.addButton(ui->radio_srn_4_3, 1);

    connect(&mScreenOrientationGroup, SIGNAL(buttonClicked(int)), this, SLOT(slot_ScreenOrientationOperate(int)));
    mScreenOrientationGroup.addButton(ui->radio_srn_90, 0);
    mScreenOrientationGroup.addButton(ui->radio_srn_180, 1);
    mScreenOrientationGroup.addButton(ui->radio_srn_270, 2);

    connect(&mSimpleProGroup, SIGNAL(buttonClicked(int)), this, SLOT(slot_SimpleProOperate(int)));
    mSimpleProGroup.addButton(ui->btn_2_4_ok, 0);
    mSimpleProGroup.addButton(ui->btn_2_4_ok_md, 1);

    connect(&mStyleModifyGroup, SIGNAL(buttonClicked(int)), this, SLOT(slot_StyleOperate(int)));
    //mStyleModifyGroup.addButton(ui->btn_32_create, 0);
    mStyleModifyGroup.addButton(ui->btn_32_import, 1);
    //mStyleModifyGroup.addButton(ui->btn_32_save, 2);
    mStyleModifyGroup.addButton(ui->btn_32_export,3);
    mStyleModifyGroup.addButton(ui->btn_32_remove, 4);
    mStyleModifyGroup.addButton(ui->btn_32_2_save, 5);
    mStyleModifyGroup.addButton(ui->btn_32_text, 7);

    connect(&mFloorOperateGroup, SIGNAL(buttonClicked(int)), this, SLOT(slot_FloorOperate(int)));
    mFloorOperateGroup.addButton(ui->btn_321_import, 0);
    mFloorOperateGroup.addButton(ui->btn_321_test, 2);

    connect(&mArrowOperateGroup, SIGNAL(buttonClicked(int)), this, SLOT(slot_ArrowOperate(int)));
    mArrowOperateGroup.addButton(ui->btn_322_import, 0);
    mArrowOperateGroup.addButton(ui->btn_322_test, 2);

//    connect(&mLogoOperateGroup, SIGNAL(buttonClicked(int)), this, SLOT(slot_LogoOperate(int)));
//    mLogoOperateGroup.addButton(ui->btn_323_import, 0);
//    mLogoOperateGroup.addButton(ui->btn_323_test, 2);

    connect(&mBackgroundOperateGroup, SIGNAL(buttonClicked(int)), this, SLOT(slot_BackgroundOperate(int)));
    mBackgroundOperateGroup.addButton(ui->btn_324_import, 0);
    mBackgroundOperateGroup.addButton(ui->btn_324_test, 2);

    connect(&mLanguageOperateGroup, SIGNAL(buttonClicked(int)), this, SLOT(slot_LanguageOperate(int)));
    mLanguageOperateGroup.addButton(ui->btn_33_import, 0);
    mLanguageOperateGroup.addButton(ui->btn_33_test, 1);
    //mLanguageOperateGroup.addButton(ui->btn_33_save, 2);
    mLanguageOperateGroup.addButton(ui->btn_33_1_import, 5);
    mLanguageOperateGroup.addButton(ui->btn_33_2_next, 6);

    mLanguageOperateGroup.addButton(ui->btn_33_addfile, 7);
    mLanguageOperateGroup.addButton(ui->btn_33_delfile, 8);
    //mLanguageOperateGroup.addButton(ui->btn_33_new, 9);
    mLanguageOperateGroup.addButton(ui->btn_33_delete, 10);

    connect(&mWIFIOperateGroup, SIGNAL(buttonClicked(int)), this, SLOT(slot_WIFIOperate(int)));
    mWIFIOperateGroup.addButton(ui->btn_scandev, 0);
    //mWIFIOperateGroup.addButton(ui->btn_testdev, 1);
    mWIFIOperateGroup.addButton(ui->btn_logindev, 2);

    connect(&mNextBtnGroup, SIGNAL(buttonClicked(int)), this, SLOT(slot_NextStep(int)));
    mNextBtnGroup.addButton(ui->btn_32_2_next, 0);
    mNextBtnGroup.addButton(ui->btn_321_next, 1);
    mNextBtnGroup.addButton(ui->btn_322_next, 2);
    //mNextBtnGroup.addButton(ui->btn_323_next, 3);

    connect(&mRetBtnGroup, SIGNAL(buttonClicked(int)), this, SLOT(slot_BackStep(int)));
    mRetBtnGroup.addButton(ui->btn_2_1_return, 201);
    mRetBtnGroup.addButton(ui->btn_2_2_return, 202);
    mRetBtnGroup.addButton(ui->btn_2_21_return, 2021);
    mRetBtnGroup.addButton(ui->btn_2_3_return, 203);
    mRetBtnGroup.addButton(ui->btn_2_4_return, 204);
    mRetBtnGroup.addButton(ui->btn_3_return, 3);
    mRetBtnGroup.addButton(ui->btn_31_return, 31);
    mRetBtnGroup.addButton(ui->btn_32_return, 32);
    mRetBtnGroup.addButton(ui->btn_32_1_return, 3201);
    mRetBtnGroup.addButton(ui->btn_32_2_return, 3202);
    mRetBtnGroup.addButton(ui->btn_321_return, 321);
    mRetBtnGroup.addButton(ui->btn_322_return, 322);
    mRetBtnGroup.addButton(ui->btn_323_return, 323);
    mRetBtnGroup.addButton(ui->btn_324_return, 324);
    mRetBtnGroup.addButton(ui->btn_33_return, 33);
    mRetBtnGroup.addButton(ui->btn_33_1_return, 3301);
    mRetBtnGroup.addButton(ui->btn_33_2_return, 3302);
    mRetBtnGroup.addButton(ui->btn_34_return, 34);

    connect(&mSelectPathGroup, SIGNAL(buttonClicked(int)), this, SLOT(slot_SelectPath(int)));
    mSelectPathGroup.addButton(ui->btn_2_3_selectfolder, 203);
    mSelectPathGroup.addButton(ui->btn_32_1_selectfolder, 3201);
    mSelectPathGroup.addButton(ui->btn_32_1_selectfolder_pro, 3202);
    mSelectPathGroup.addButton(ui->btn_33_1_selectfolder, 3301);

    connect(&mSettingGroup, SIGNAL(buttonClicked(int)), this, SLOT(slot_SettingPage(int)));
    mSettingGroup.addButton(ui->btn_34_volume, 0);
    mSettingGroup.addButton(ui->btn_34_floors, 1);
    mSettingGroup.addButton(ui->btn_34_backlight, 2);
    //mSettingGroup.addButton(ui->btn_34_logo, 3);
    mSettingGroup.addButton(ui->btn_34_wifi, 4);

    //>@初始化语言
    iniParse *tmpParse = new iniParse(GlobalPara.m_SoftwareDir + D_ETC_DIR + D_ETC_BOOT_FILE);
    QString tmpLanguage;
    bool flag = tmpParse->getProperty("Language", tmpLanguage);
    if(flag)
    {
        if(!tmpLanguage.compare(QString("EN"), Qt::CaseInsensitive))
            ui->lang_en_banner->setChecked(true);
        else if(!tmpLanguage.compare(QString("FR"), Qt::CaseInsensitive))
            ui->lang_fra_banner->setChecked(true);
        else if(!tmpLanguage.compare(QString("GE"), Qt::CaseInsensitive))
            ui->lang_deu_banner->setChecked(true);
        else if(!tmpLanguage.compare(QString("CH"), Qt::CaseInsensitive))
            ui->lang_ned_banner->setChecked(true);
    }
    tmpParse->deleteLater();

    ui->SettingStacked->setCurrentIndex(0);
    m_LoginState = false;
}

DeviceManager::~DeviceManager()
{
    delete ui;
}

void DeviceManager::slot_Initialize()
{
#ifdef QT5
    //>@更改DIP
    //>@计算比例：pixelSize = DPI * pointSize/72
    QScreen *screen = qApp->primaryScreen();
    qreal tmpDpi = screen->logicalDotsPerInch();
    qreal tmpPtDiff = ((qreal)(90) / (qreal)(tmpDpi));
    qreal tmpPxDiff = 1.0 / tmpPtDiff;

    QList<QLayout*>tmpLayoutList = findChildren<QLayout*>();
    QLayout *tmpLayout = 0;
    foreach(tmpLayout, tmpLayoutList)
    {
        if(tmpLayout)
        {
            int space = tmpLayout->spacing();
            if(space > 0)
                tmpLayout->setSpacing(space*tmpPxDiff);
            int left, top, right, bottom;
            tmpLayout->getContentsMargins(&left, &top, &right, &bottom);
            if(left > 0)
                left *= tmpPxDiff;
            if(top > 0)
                top *= tmpPxDiff;
            if(right > 0)
                right *= tmpPxDiff;
            if(bottom > 0)
                bottom *= tmpPxDiff;
            tmpLayout->setContentsMargins(left, top, right, bottom);
        }
    }

    //>@修改Stylesheet中的字体
    QString tmpStyleSheet = styleSheet();
    int index = 0;
    while(1)
    {
        int index1 = tmpStyleSheet.indexOf("pt", index);
        if(index1 < 0)
            break;
        int index2 = tmpStyleSheet.lastIndexOf(" ", index1);
        if(index2 < 0)
            continue;
        int tmpindex = index2 + 1;
        QString tmpSizeStr = tmpStyleSheet.mid(tmpindex, index1-tmpindex);
        int tmpSize = tmpSizeStr.toInt();
        tmpSize*=tmpPtDiff;
        tmpStyleSheet.replace(tmpindex, index1-tmpindex, QString::number(tmpSize));

        index = index1 + 2;
    }
    index = 0;
    while(1)
    {
        int index1 = tmpStyleSheet.indexOf("px", index);
        if(index1 < 0)
            break;
        int index2 = tmpStyleSheet.lastIndexOf(" ", index1);
        if(index2 < 0)
            continue;
        int tmpindex = index2 + 1;
        QString tmpSizeStr = tmpStyleSheet.mid(tmpindex, index1-tmpindex);
        int tmpSize = tmpSizeStr.toInt();
        tmpSize*=tmpPxDiff;
        tmpStyleSheet.replace(tmpindex, index1-tmpindex, QString::number(tmpSize));

        index = index1 + 2;
    }
    setStyleSheet(tmpStyleSheet);
#endif

    DeviceSetBase::slot_Initialize();
}

void DeviceManager::slot_InitDevice(QString pPath)
{
    mOperateState = true;

    m_ShowFloor = true;
    mFloor = -1;
    mFloorInfo = FLOOR_INFO();
    mArrow = 1;
    mFunction = 40;
    mAudio = 0;

    if(!OpenBootMdp(pPath))
    {
        MsgBox tmpMsgBox(tr("MDP file\n %1\n is broken!").arg(getFileName(pPath)));
        tmpMsgBox.exec();
        mOperateState = false;
        return;
    }

    QString tmpAppVersion = GetAppVersion(qApp->applicationDirPath() + QString("/DEV/ONLINE/FACTORY/EXE/SOD_DISPLAY"));

    QDomElement tmpElement = mMdpManager->m_LGElementGroup.value(L_ThemeManager);
    if(!tmpElement.isNull() && tmpElement.hasChildNodes())
    {
        LogicThemeManager *tmpManager = new LogicThemeManager(this);
        tmpManager->InitDevice(L_ThemeManager, tmpElement);
        m_LGDevList.insert(L_ThemeManager, tmpManager);
        mThemeLayout = tmpManager->mThemeLayout[0];
    }

    QString tmpDevType;
    tmpElement = mMdpManager->m_PHElementGroup.value(P_SAF);
    if(!tmpElement.isNull() && tmpElement.hasChildNodes())
    {
        tmpDevType = "SAF";
        ui->stacked_flrmap->setCurrentIndex(0);
        ui->stacked_arrowspeed->setCurrentIndex(0);
        ui->stacked_openmsg->setCurrentIndex(0);

        QDomElement tmpSubElement = tmpElement.firstChildElement("OpenMsg");
        if(!tmpSubElement.isNull())
        {
            QString tmpStr = tmpSubElement.text();
            if(!tmpStr.compare("true", Qt::CaseInsensitive))
                ui->check_openmsg->setChecked(true);
            else
                ui->check_openmsg->setChecked(false);
        }
    }

    tmpElement = mMdpManager->m_PHElementGroup.value(P_COM);
    if(!tmpElement.isNull() && tmpElement.hasChildNodes())
    {
        tmpDevType = "CANOPEN";
        ui->stacked_flrmap->setCurrentIndex(1);
        ui->stacked_arrowspeed->setCurrentIndex(1);
        ui->stacked_openmsg->setCurrentIndex(1);

        QDomElement tmpSubElement = tmpElement.firstChildElement("OpenMsg");
        if(!tmpSubElement.isNull())
        {
            QString tmpStr = tmpSubElement.text();
            if(!tmpStr.compare("true", Qt::CaseInsensitive))
                ui->check_openmsg->setChecked(true);
            else
                ui->check_openmsg->setChecked(false);
        }
    }

    ui->label_info_name_2->setText(GetDirectoryName(mBufferPath));
    ui->label_info_appver_2->setText(tmpAppVersion);
    ui->label_info_sysver_2->setText("0.1.k.141231");
    ui->label_info_devtype_2->setText(tmpDevType);

    QString tmpCode;
    devManager *tmpManager = new devManager(mMdpManager->m_FileDir+"info.dev");
    if(tmpManager->OpenDEV())
        tmpCode = tmpManager->GetCode();
    tmpManager->m_IsChanged = false;
    delete tmpManager;
    ui->edit_ssid->setText(tmpCode);

    ui->edit_passwd->setText(mMdpManager->GetPassword());

    tmpElement = mMdpManager->m_PHElementGroup.value(P_Player);
    if(!tmpElement.isNull() && tmpElement.hasChildNodes())
    {
        PhysicalPlayer *tmpPlayer = new PhysicalPlayer(this);
        tmpPlayer->InitDevice(P_Player, tmpElement);
        m_PHDevList.insert(P_Player, tmpPlayer);
        tmpPlayer->InitPlayer();
        tmpPlayer->SetEnable(true, DM_NORMAL);
        tmpPlayer->mAoParas[AO_BEEP].mEnable[0] = tmpPlayer->mAoParas[AO_BEEP].mEnable[1] = true;
        tmpPlayer->mAoParas[AO_BEEP].mVolume.mNum = 100;
        tmpPlayer->Restore();

        ui->slider_message_volume->mQesp = this;
        ui->slider_message_volume->bindLabel(ui->label_msg_value);
        ui->slider_message_volume->setRange(tmpPlayer->mAoParas[AO_LIFTFLR].mVolume.mMinNum, tmpPlayer->mAoParas[AO_LIFTFLR].mVolume.mMaxNum);
        ui->slider_message_volume->WriteValue(tmpPlayer->mAoParas[AO_LIFTFLR].mVolume.mNum);
        ui->slider_message_volume->setProperty("valueChanged", QString("<Physical><Player><LiftFlrVol>%1</LiftFlrVol></Player></Physical>"));

        ui->slider_gong_volume->mQesp = this;
        ui->slider_gong_volume->bindLabel(ui->label_gong_value);
        ui->slider_gong_volume->setRange(tmpPlayer->mAoParas[AO_LIFTARV].mVolume.mMinNum, tmpPlayer->mAoParas[AO_LIFTARV].mVolume.mMaxNum);
        ui->slider_gong_volume->WriteValue(tmpPlayer->mAoParas[AO_LIFTARV].mVolume.mNum);
        ui->slider_gong_volume->setProperty("valueChanged", QString("<Physical><Player><LiftArvVol>%1</LiftArvVol></Player></Physical>"));

        ui->slider_bgmusic_volume->mQesp = this;
        ui->slider_bgmusic_volume->bindLabel(ui->label_music_value);
        ui->slider_bgmusic_volume->setRange(tmpPlayer->mAoParas[AO_ADMUSIC].mVolume.mMinNum, tmpPlayer->mAoParas[AO_ADMUSIC].mVolume.mMaxNum);
        ui->slider_bgmusic_volume->WriteValue(tmpPlayer->mAoParas[AO_ADMUSIC].mVolume.mNum);
        ui->slider_bgmusic_volume->setProperty("valueChanged", QString("<Physical><Player><AdMusicVol>%1</AdMusicVol></Player></Physical>"));
    }

    tmpElement = mMdpManager->m_PHElementGroup.value(P_LCD);
    if(!tmpElement.isNull() && tmpElement.hasChildNodes())
    {
        PhysicalLCD *tmpLCD = new PhysicalLCD(this);
        tmpLCD->InitDevice(P_LCD, tmpElement);
        m_PHDevList.insert(P_LCD, tmpLCD);

        ui->slider_backlight->mQesp = this;
        ui->slider_backlight->bindLabel(ui->label_normal_max);
        ui->slider_backlight->setRange(tmpLCD->mLuminance.mMinNum, tmpLCD->mLuminance.mMaxNum);
        ui->slider_backlight->WriteValue(tmpLCD->mLuminance.mNum);
        ui->slider_backlight->setProperty("valueChanged", QString("<Physical><LCD><Luminance>%1</Luminance></LCD></Physical>"));

        ui->slider_standby->mQesp = this;
        ui->slider_standby->bindLabel(ui->label_standby_max, QString("%1%"));
        ui->slider_standby->setRange(tmpLCD->mDarkLevel.mMinNum, tmpLCD->mDarkLevel.mMaxNum);
        ui->slider_standby->WriteValue(tmpLCD->mDarkLevel.mNum);
        ui->slider_standby->setProperty("valueChanged", QString("<Physical><LCD><DarkLevel>%1</DarkLevel></LCD></Physical>"));

        ui->slider_delay->mQesp = this;
        ui->slider_delay->bindLabel(ui->label_delay_max, QString("%1min"), QString("x/60000"));
        ui->slider_delay->setRange(tmpLCD->mStandBy.mMinNum, tmpLCD->mStandBy.mMaxNum);
        ui->slider_delay->setSingleStep(60000);
        ui->slider_delay->WriteValue(tmpLCD->mStandBy.mNum);
        ui->slider_delay->setProperty("valueChanged", QString("<Physical><LCD><StandBy>%1</StandBy></LCD></Physical>"));

        mRotation = tmpLCD->mRotation;
    }

    if(mRotation == 180)
    {
        ui->radio_srn_180->setChecked(true);
    }
    else if(mRotation == 90)
    {
        ui->radio_srn_90->setChecked(true);
    }
    else if(mRotation == 270)
    {
        ui->radio_srn_270->setChecked(true);
    }
    if(mThemeLayout < 2)
        ui->radio_srn_4_3->setChecked(true);
    else
        ui->radio_srn_4->setChecked(true);
    if(mThemeLayout == 0 || mThemeLayout == 2)
        ui->radio_srn_arrow->setChecked(true);
    else
        ui->radio_srn_floor->setChecked(true);
}

FLOOR_INFO DeviceManager::GetFloor()
{
    if(m_ShowFloor)
    {
        quint8 baiwei, shiwei, gewei;
        baiwei = 0x20;
        int tmpTen = mFloor % 100 / 10;
        if(tmpTen == 0)
            shiwei = 0x20;
        else
            shiwei = tmpTen + 0x30;
        int tmpSingle = mFloor % 10;
        //if(tmpTen == 0 && tmpSingle == 0)
        //    gewei = 0x20;
        //else
            gewei = tmpSingle + 0x30;
        FLOOR_INFO tmpFloor = FLOOR_INFO(baiwei, shiwei, gewei);
        if(!mFloorMapHash.isEmpty())
        {
            QString tmpKey = tmpFloor.toMapFormat();
            QString tmpMap = mFloorMapHash.value(tmpKey);
            if(!tmpMap.isEmpty())
            {
                if(tmpMap.count() <= 2)
                {
                    tmpFloor = tmpMap;
                    tmpFloor.mReserve = 1;
                }
                else if(tmpMap.count() == 6)
                {
                    tmpFloor = tmpMap;
                    tmpFloor.mReserve = 0;
                }
            }
        }
        mFloorInfo = tmpFloor;
        return tmpFloor;
    }
    return FLOOR_INFO();
}

int DeviceManager::GetAudio()
{
    QString tmpMap = mMediaMapHash.value(QString::number(mAudio));
    if(!tmpMap.isEmpty())
        mAudio = tmpMap.toInt();
    return mAudio;
}

void DeviceManager::slot_TimeTick()
{
    static quint64 sTick = 0;

    if(sTick % 2 == 0 && (m_LangListOperate == 0))
    {
        m_LangListOperate = 1;

        QString tmpPath =GlobalPara.m_SoftwareDir + D_COM_DIR + QString("Audio/");
        QDir tmpLanuDir(tmpPath);
        QStringList folders = tmpLanuDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

        QListWidgetItem* tmpItem = ui->list_33_language->currentItem();
        QString tmpCurLang;
        if(tmpItem)
            tmpCurLang = tmpItem->text();

        ui->list_33_language->clear();

        for (int i = 0; i < folders.size(); ++i)
        {
            QString tmpFolderName = folders.at(i);

            QListWidgetItem* tmpNewItem = AddListItem(ui->list_33_language, tmpPath, tmpFolderName);
            if(tmpNewItem)
            {
                if(tmpCurLang.compare(tmpFolderName, Qt::CaseInsensitive) == 0)
                {
                    ui->list_33_language->setCurrentItem(tmpNewItem);
                }
            }
        }

        m_LangListOperate = 0;
    }

    sTick++;
}

void DeviceManager::slot_AutoTest()
{
    mFloor++;  //>@帧计数器
    if(ui->stacked_flrmap->currentIndex() == 0)
    {
        if(mFloor == 20)
        {
            return;
        }
        else if(mFloor <= 39)
        {
            m_ShowFloor = true;
        }
        else if(mFloor <= 49)
        {
            mFunction = mFloor;
            m_ShowFloor = false;
            if(mFunction == 42 || mFunction == 47)
            {
                mAudio = 43;
            }
        }
        else if(mFloor < 61)
        {
            mFloor = 60;
            return;
        }
        else if(mFloor <= 69)
        {
            m_ShowFloor = true;
        }
        else
        {
            mFloor = -1;
            return;
        }
        mArrow++;
        if(mArrow > 3)
            mArrow = 1;
        if(m_ShowFloor)
            mAudio = mFloor;
    }
    else  //>@CANOPEN
    {
        m_ShowFloor = true;
        if(mFloor <= 84)
        {
            mAudio = mFloor;
        }
        else
        {
            mFloor = -1;
            mAudio = 0;
            return;
        }
    }
    UpdateDevImage(GetFloor(), mArrow, mFunction, GetAudio());
}

//>@pLayout:1表示箭头在前
void DeviceManager::UpdateDevImage(int pRotate, int pLayout, bool pShowFloor)
{
    if(mBufferPath.isEmpty() || !(mUiManager && mUiManager->m_Valid) || !(mMdpManager && mMdpManager->m_Valid))
        return;
    QDomElement tmpElement = mMdpManager->m_PHElementGroup.value(P_LCD);
    if(!tmpElement.isNull() && tmpElement.hasChildNodes())
    {
        mMdpManager->ModifyItemElement("/CfgPara/Physical/LCD/Rotation", QString::number(pRotate));
    }
    tmpElement = mMdpManager->m_LGElementGroup.value(L_ThemeManager);
    if(!tmpElement.isNull() && tmpElement.hasChildNodes())
    {
        mMdpManager->ModifyItemElement("/CfgPara/Logic/ThemeManager/Layout", QString::number(pLayout));
    }

    mThemeLayout = pLayout;
    m_ShowFloor = pShowFloor;
    mRotation = pRotate;
    //>@获取当前坐标
    tmpElement = mUiManager->m_ComElementGroup.value(COM_FLOOR);
    if(!tmpElement.isNull())
    {
        mUiManager->GetComRect(pLayout, tmpElement, tmpFloorRectH, tmpFloorRectV);
    }
    tmpElement = mUiManager->m_ComElementGroup.value(COM_ARROW);
    if(!tmpElement.isNull())
    {
        mUiManager->GetComRect(pLayout, tmpElement, tmpArrowRectH, tmpArrowRectV);
    }
    tmpElement = mUiManager->m_ComElementGroup.value(COM_FUNCTION);
    if(!tmpElement.isNull())
    {
        mUiManager->GetComRect(pLayout, tmpElement, tmpFuncRectH, tmpFuncRectV);
    }
    tmpElement = mUiManager->m_ComElementGroup.value(COM_TEXT);
    if(!tmpElement.isNull())
    {
        mUiManager->GetComRect(pLayout, tmpElement, tmpTextRectH, tmpTextRectV);
    }

    UpdateDevImage(m_ShowFloor?(mFloorInfo):FLOOR_INFO(), mArrow, mFunction, mAudio);
}

void DeviceManager::UpdateDevImage(FLOOR_INFO pFloor, int pArrow, int pFunction, int pAudio)
{
    static int oldFloor = -1;
    QString tmpThemePath = mBufferPath+D_THEMEDIR+QString("Sodimas/");
    QPixmap tmpPixmap;
    if(mRotation == 180)
    {
        QImage tmpImageH(480, 272, QImage::Format_ARGB32_Premultiplied);
        tmpImageH.fill(Qt::transparent); //>@创建透明图层
        QPainter pH(&tmpImageH);
        pH.drawPixmap(0,0,480,272,QPixmap(tmpThemePath+QString("Desktop/Resource/Image/T0.png")));
        pH.drawPixmap(tmpArrowRectH,QPixmap(tmpThemePath+QString("Arrow/Resource/Image/T%1.png").arg(pArrow)));
        if(m_ShowFloor)
        {
            if(pFloor.mReserve == 1)
            {
                pH.drawPixmap(QRect(tmpFloorRectH.x(), tmpFloorRectH.y(), tmpFloorRectH.width(), tmpFloorRectH.height()),
                              QPixmap(tmpThemePath+QString("Floor/Resource/Image/T%1.png").arg(pFloor.mSingleBits)));
            }
            else
            {
                if(isVisibleAscii(pFloor.mHundredBits))
                    pH.drawPixmap(QRect(tmpFloorRectH.x(), tmpFloorRectH.y(), tmpFloorRectH.width()/3, tmpFloorRectH.height()),
                                  QPixmap(tmpThemePath+QString("Floor/Resource/Image/T%1.png").arg(pFloor.mHundredBits)));
                if(isVisibleAscii(pFloor.mTenBits))
                    pH.drawPixmap(QRect(tmpFloorRectH.x() + tmpFloorRectH.width()/3, tmpFloorRectH.y(), tmpFloorRectH.width()/3, tmpFloorRectH.height()),
                                  QPixmap(tmpThemePath+QString("Floor/Resource/Image/T%1.png").arg(pFloor.mTenBits)));
                if(isVisibleAscii(pFloor.mSingleBits))
                    pH.drawPixmap(QRect(tmpFloorRectH.x() + 2*tmpFloorRectH.width()/3, tmpFloorRectH.y(), tmpFloorRectH.width()/3, tmpFloorRectH.height()),
                                  QPixmap(tmpThemePath+QString("Floor/Resource/Image/T%1.png").arg(pFloor.mSingleBits)));
            }
        }
        else
        {
            QString tmpStr = tmpThemePath+QString("Function/Resource/Image/T%1.png").arg(pFunction);
            IDE_TRACE_STR(tmpStr);
            pH.drawPixmap(tmpFuncRectH,QPixmap(tmpStr));
        }
        pH.end();

        tmpPixmap = QPixmap::fromImage(tmpImageH);
    }
    else if(mRotation == 270)
    {
        QImage tmpImageV(480, 272, QImage::Format_ARGB32_Premultiplied);
        tmpImageV.fill(Qt::transparent); //>@创建透明图层
        QPainter pV(&tmpImageV);
        pV.translate(240, 136);
        pV.rotate(270);
        pV.translate(-136, -240);
        pV.drawPixmap(0,0,272,480,QPixmap(tmpThemePath+QString("Desktop/Resource/Image/T100.png")));
        pV.drawPixmap(tmpArrowRectV,QPixmap(tmpThemePath+QString("Arrow/Resource/Image/T%1.png").arg(pArrow)));
        if(m_ShowFloor)
        {
            if(pFloor.mReserve == 1)
            {
                pV.drawPixmap(QRect(tmpFloorRectV.x(), tmpFloorRectV.y(), tmpFloorRectV.width(), tmpFloorRectV.height()),
                              QPixmap(tmpThemePath+QString("Floor/Resource/Image/T%1.png").arg(pFloor.mSingleBits)));
            }
            else
            {
                if(isVisibleAscii(pFloor.mHundredBits))
                    pV.drawPixmap(QRect(tmpFloorRectV.x(), tmpFloorRectV.y(), tmpFloorRectV.width()/3, tmpFloorRectV.height()),
                                  QPixmap(tmpThemePath+QString("Floor/Resource/Image/T%1.png").arg(pFloor.mHundredBits)));
                if(isVisibleAscii(pFloor.mTenBits))
                    pV.drawPixmap(QRect(tmpFloorRectV.x() + tmpFloorRectV.width()/3, tmpFloorRectV.y(), tmpFloorRectV.width()/3, tmpFloorRectV.height()),
                                  QPixmap(tmpThemePath+QString("Floor/Resource/Image/T%1.png").arg(pFloor.mTenBits)));
                if(isVisibleAscii(pFloor.mSingleBits))
                    pV.drawPixmap(QRect(tmpFloorRectV.x() + 2*tmpFloorRectV.width()/3, tmpFloorRectV.y(), tmpFloorRectV.width()/3, tmpFloorRectV.height()),
                                  QPixmap(tmpThemePath+QString("Floor/Resource/Image/T%1.png").arg(pFloor.mSingleBits)));
            }
        }
        else
            pV.drawPixmap(tmpFuncRectV,QPixmap(tmpThemePath+QString("Function/Resource/Image/T%1.png").arg(pFunction)));
        pV.end();

        tmpPixmap = QPixmap::fromImage(tmpImageV);
    }
    else if(mRotation == 90)
    {
        QImage tmpImageV(480, 272, QImage::Format_ARGB32_Premultiplied);
        tmpImageV.fill(Qt::transparent); //>@创建透明图层
        QPainter pV(&tmpImageV);
        pV.translate(240, 136);
        pV.rotate(90);
        pV.translate(-136, -240);
        pV.drawPixmap(0,0,272,480,QPixmap(tmpThemePath+QString("Desktop/Resource/Image/T100.png")));
        pV.drawPixmap(tmpArrowRectV,QPixmap(tmpThemePath+QString("Arrow/Resource/Image/T%1.png").arg(pArrow)));
        if(m_ShowFloor)
        {
            if(pFloor.mReserve == 1)
            {
                pV.drawPixmap(QRect(tmpFloorRectV.x(), tmpFloorRectV.y(), tmpFloorRectV.width(), tmpFloorRectV.height()),
                              QPixmap(tmpThemePath+QString("Floor/Resource/Image/T%1.png").arg(pFloor.mSingleBits)));
            }
            else
            {
                if(isVisibleAscii(pFloor.mHundredBits))
                    pV.drawPixmap(QRect(tmpFloorRectV.x(), tmpFloorRectV.y(), tmpFloorRectV.width()/3, tmpFloorRectV.height()),
                                  QPixmap(tmpThemePath+QString("Floor/Resource/Image/T%1.png").arg(pFloor.mHundredBits)));
                if(isVisibleAscii(pFloor.mTenBits))
                    pV.drawPixmap(QRect(tmpFloorRectV.x() + tmpFloorRectV.width()/3, tmpFloorRectV.y(), tmpFloorRectV.width()/3, tmpFloorRectV.height()),
                                  QPixmap(tmpThemePath+QString("Floor/Resource/Image/T%1.png").arg(pFloor.mTenBits)));
                if(isVisibleAscii(pFloor.mSingleBits))
                    pV.drawPixmap(QRect(tmpFloorRectV.x() + 2*tmpFloorRectV.width()/3, tmpFloorRectV.y(), tmpFloorRectV.width()/3, tmpFloorRectV.height()),
                                  QPixmap(tmpThemePath+QString("Floor/Resource/Image/T%1.png").arg(pFloor.mSingleBits)));
            }
        }
        else
            pV.drawPixmap(tmpFuncRectV,QPixmap(tmpThemePath+QString("Function/Resource/Image/T%1.png").arg(pFunction)));
        pV.end();

        tmpPixmap = QPixmap::fromImage(tmpImageV);
    }

    if(!m_Initialize)
    {
        if(ui->btn_test->isChecked())
        {
            if(m_ShowFloor)
            {
                if(oldFloor != pAudio)
                {
                    QString tmpFile = mUiManager->m_FileDir + QString("Media/Resource/Audio/M%1.wav").arg(pAudio);
                    if(QFile::exists(tmpFile))
                    {
                        m_MediaPlayer->Add(AO_BEEP, tmpFile, false, false, 100);
                    }
                    oldFloor = pAudio;
                }
            }
            else
            {
                if(pAudio == 43)
                {
                    mAudio = 40;
                    QString tmpFile = mUiManager->m_FileDir + QString("Media/Resource/Audio/M%1.wav").arg(pAudio);
                    if(QFile::exists(tmpFile))
                    {
                        m_MediaPlayer->Add(AO_BEEP, tmpFile, false, false, 100);
                    }
                }
            }
        }
    }
    else
    {
        m_Initialize = false;
    }

    ui->img_2_preview->setPixmap(tmpPixmap);
    ui->img_3_preview->setPixmap(tmpPixmap);
    ui->img_31_preview->setPixmap(tmpPixmap);
    ui->img_32_preview->setPixmap(tmpPixmap);
    ui->img_321_preview->setPixmap(tmpPixmap);
    ui->img_322_preview->setPixmap(tmpPixmap);
    //ui->img_323_preview->setPixmap(tmpPixmap);
    ui->img_324_preview->setPixmap(tmpPixmap);
}

void DeviceManager::GetActiveStyle(int &pDtpStyle, int &pFlrStyle, int &pArwStyle, int &pFunStyle, int &pAudStyle, int &pMusicStyle)
{

}

void DeviceManager::slot_FlrmapbarActive()
{
    QWidget *tmpSender = VAR_CAST<QWidget *>(sender());
    if(!tmpSender)
        return;
    int row = -1;
    for(int i=0;i<ui->table_flrmap->rowCount();i++)
    {
        QWidget *tmpWidget = ui->table_flrmap->cellWidget(i, 0);
        if(tmpWidget == tmpSender)
        {
            row = i;
            break;
        }
    }
    if(row != -1)
        ActiveFlrmapbar(row);
}

void DeviceManager::ActiveFlrmapbar(int row)
{
    ui->table_flrmap->selectRow(row);
    for(int i=0;i<ui->table_flrmap->rowCount();i++)
    {
        if(i == row)
            continue;
        FlrmapBar *tmpFlrmap = VAR_CAST<FlrmapBar *>(ui->table_flrmap->cellWidget(i, 0));
        if(tmpFlrmap)
        {
            tmpFlrmap->ClearInputFocus();
        }
    }
}

void DeviceManager::slot_FlrmapbarDel()
{
    QWidget *tmpSender = VAR_CAST<QWidget *>(sender());
    if(!tmpSender)
        return;
    int row = -1;
    for(int i=0;i<ui->table_flrmap->rowCount();i++)
    {
        QWidget *tmpWidget = ui->table_flrmap->cellWidget(i, 0);
        if(tmpWidget == tmpSender)
        {
            row = i;
            break;
        }
    }
    if(row != -1)
        DelFlrmapbar(row);
}

FlrmapBar *DeviceManager::AddFlrmapbar(QString tmpLevel, QString tmpMapping, QString tmpAudSrc, QString tmpAudio)
{
    FlrmapBar *tmpFlrmap = 0;
    bool find = false;
    int row = ui->table_flrmap->rowCount();
    for(int i=0;i<row;i++)
    {
        tmpFlrmap = VAR_CAST<FlrmapBar *>(ui->table_flrmap->cellWidget(i, 0));
        if(tmpFlrmap)
        {
            if(!tmpFlrmap->GetLevle().compare(tmpLevel, Qt::CaseInsensitive))
            {
                find = true;
                break;
            }
        }
    }
    if(!find || !tmpFlrmap)
    {
        tmpFlrmap = new FlrmapBar(this);
        connect(tmpFlrmap, SIGNAL(sActive()), this, SLOT(slot_FlrmapbarActive()));
        connect(tmpFlrmap, SIGNAL(sDelete()), this, SLOT(slot_FlrmapbarDel()));
        //connect(this, SIGNAL(sChangeRcs()), tmpFlrmap, SLOT(slot_ChangeRcs()));

        ui->table_flrmap->insertRow(row);
        ui->table_flrmap->setCellWidget(row, 0, tmpFlrmap);
        ActiveFlrmapbar(row);
    }
    RC_MAP tmpMap1;
    tmpMap1.mSrc = tmpLevel;
    tmpMap1.mMap = tmpMapping;
    RC_MAP tmpMap2;
    tmpMap2.mSrc = tmpAudSrc;
    tmpMap2.mMap = tmpAudio;
    tmpFlrmap->SetMap(tmpMap1, tmpMap2);

    return tmpFlrmap;
}

void DeviceManager::DelFlrmapbar(int row)
{
    FlrmapBar *tmpFlrmap = VAR_CAST<FlrmapBar *>(ui->table_flrmap->cellWidget(row, 0));
    if(tmpFlrmap)
        disconnect(this, SIGNAL(sChangeRcs()), tmpFlrmap, SLOT(slot_ChangeRcs()));
    ui->table_flrmap->removeCellWidget(row, 0);
    ui->table_flrmap->removeRow(row);
}

void DeviceManager::ClrFlrmapbar()
{
    if(ui->table_flrmap->rowCount())
    {
        for(int i = ui->table_flrmap->rowCount()-1;i>=0;i--)
        {
            DelFlrmapbar(i);
        }
    }
}

void DeviceManager::UpdateFlrMap()
{
    if(mUiManager && mUiManager->m_Valid)
    {
        QDomElement tmpFloorElement = mUiManager->m_ComElementGroup.value(COM_FLOOR);
        QDomElement tmpMediaElement = mUiManager->m_ComElementGroup.value(COM_MEDIA);
        QString tmpMapType;
        mFloorMapHash = mUiManager->GetMapHash(tmpFloorElement, tmpMapType);
        mMediaMapHash = mUiManager->GetMapHash(tmpMediaElement, tmpMapType);
    }
}

void DeviceManager::InitFlrMap()
{
    ui->table_flrmap->clear();
    mFloorMapHash.clear();
    mMediaMapHash.clear();

    if(mUiManager && mUiManager->m_Valid)
    {
        QDomElement tmpFloorElement = mUiManager->m_ComElementGroup.value(COM_FLOOR);
        QDomElement tmpMediaElement = mUiManager->m_ComElementGroup.value(COM_MEDIA);
        QString tmpMapType;
        mFloorMapHash = mUiManager->GetMapHash(tmpFloorElement, tmpMapType);
        mMediaMapHash = mUiManager->GetMapHash(tmpMediaElement, tmpMapType);
        ClrFlrmapbar();
        for(int i=0;i<=39;i++)
        {
            if(i == 20)
                continue;
            QString tmpSrc = NumToAscii(i);
            QString tmpMap = mFloorMapHash.value(tmpSrc);
            if(tmpMap.isEmpty())
                tmpMap = tmpSrc;
            QString tmpSrcAudio = QString::number(i);
            QString tmpAudio = mMediaMapHash.value(tmpSrcAudio);
            if(tmpAudio.isEmpty())
                tmpAudio = tmpSrcAudio;
            FlrmapBar *tmBar = AddFlrmapbar(tmpSrc, tmpMap, tmpSrcAudio, tmpAudio);
            if(tmBar)
            {
                tmBar->SetCanDel(false);
                tmBar->SetShowPreview(false);
                tmBar->setFixedWidth(ui->table_flrmap->width()-50);
            }
        }
        for(int i=61;i<=69;i++)
        {
            QString tmpSrc = NumToAscii(i);
            QString tmpMap = mFloorMapHash.value(tmpSrc);
            if(tmpMap.isEmpty())
                tmpMap = tmpSrc;
            QString tmpSrcAudio = QString::number(i);
            QString tmpAudio = mMediaMapHash.value(tmpSrcAudio);
            if(tmpAudio.isEmpty())
                tmpAudio = tmpSrcAudio;
            FlrmapBar *tmBar = AddFlrmapbar(tmpSrc, tmpMap, tmpSrcAudio, tmpAudio);
            if(tmBar)
            {
                tmBar->SetCanDel(false);
                tmBar->SetShowPreview(false);
                tmBar->setFixedWidth(ui->table_flrmap->width()-50);
            }
        }
    }
}

void DeviceManager::SaveFlrMap()
{
    if(mUiManager && mUiManager->m_Valid)
    {
        QDomElement tmpFloorElement = mUiManager->m_ComElementGroup.value(COM_FLOOR);
        QDomElement tmpAudioElement = mUiManager->m_ComElementGroup.value(COM_MEDIA);
        QDomElement tmpFlrMapElement;
        QDomElement tmpAudMapElement;
        if(!tmpFloorElement.isNull())
        {
            tmpFlrMapElement = mUiManager->GetComState(tmpFloorElement, QString("Map"), true);
            if(tmpFlrMapElement.isNull())
            {
                tmpFlrMapElement = mUiManager->createElement( QString("Map"));
                tmpFloorElement.appendChild(tmpFlrMapElement);
            }
            mUiManager->ClearItemElement(tmpFlrMapElement);
        }
        if(!tmpAudioElement.isNull())
        {
            tmpAudMapElement = mUiManager->GetComState(tmpAudioElement, QString("Map"), true);
            if(tmpAudMapElement.isNull())
            {
                tmpAudMapElement = mUiManager->createElement( QString("Map"));
                tmpAudioElement.appendChild(tmpAudMapElement);
            }
            mUiManager->ClearItemElement(tmpAudMapElement);
        }

        if(tmpFlrMapElement.isNull() && tmpAudMapElement.isNull())
            return;

        for(int i=0;i<ui->table_flrmap->rowCount();i++)
        {
            FlrmapBar *tmpFlrmap = VAR_CAST<FlrmapBar *>(ui->table_flrmap->cellWidget(i, 0));
            if(tmpFlrmap)
            {
                RC_MAP tmpFlrMap, tmpAudMap;
                if(tmpFlrmap->GetMap(tmpFlrMap, tmpAudMap))
                {
                    if(!tmpFlrMapElement.isNull())
                    {
                        mUiManager->AddMap(tmpFlrMapElement, tmpFlrMap);
                    }
                    if(!tmpAudMapElement.isNull())
                        mUiManager->AddMap(tmpAudMapElement, tmpAudMap);
                }
            }
        }
        mUiManager->SaveUI();

        UpdateFlrMap();
    }
}

void DeviceManager::InitLogoList()
{
    ui->list_323_logo->clear();
    mFunctionTextHash.clear();
    QDomElement tmpFunctionElement = mUiManager->m_ComElementGroup.value(COM_FUNCTION);
    if(!tmpFunctionElement.isNull())
    {
        QDomElement tmpRcElement = mUiManager->GetComState(tmpFunctionElement, QString("Resource"), false);
        if(!tmpRcElement.isNull())
        {
            QString tmpPath = mBufferPath + D_THEMEDIR + QString("Sodimas/");
            QDomNodeList tmpList = tmpRcElement.childNodes();
            int row = 0;
            for(int i=0;i<tmpList.count();i++)
            {
                QDomElement tmpElement = tmpList.at(i).toElement();
                if(tmpElement.isNull())
                    continue;
                QString tmpLName = tmpElement.text();
                QString tmpFullPath = tmpPath + tmpLName;
                QFileInfo tmpFileInfo(tmpFullPath);
                if(!tmpFileInfo.exists())
                    continue;
                QListWidgetItem* tmpItem = AddListItem(ui->list_323_logo, tmpFileInfo.filePath(), tmpFileInfo.fileName());
                if(!tmpItem)
                    continue;
                tmpItem->setIcon(QIcon(tmpFullPath));
                mFunctionTextHash.insert(row, tmpElement);
                row++;
            }
        }
    }
}

void DeviceManager::on_list_323_logo_itemClicked(QListWidgetItem *item)
{
    int row = ui->list_323_logo->row(item);
    QDomElement tmpElement = mFunctionTextHash.value(row);
    if(tmpElement.isNull())
        return;
    QString tmpPara = tmpElement.attribute("Text");
    QStringList tmpParaList = tmpPara.split(";");
    int count = tmpParaList.count();
    ui->edit_323_text1->clear();
    if(count > 0)
        ui->edit_323_text1->setText(tmpParaList.at(0));
    ui->edit_323_text2->clear();
    if(count > 1)
        ui->edit_323_text2->setText(tmpParaList.at(1));
}

void DeviceManager::on_edit_323_text1_editingFinished()
{
    int row = ui->list_323_logo->currentRow();
    QDomElement tmpElement = mFunctionTextHash.value(row);
    if(tmpElement.isNull())
        return;
    QString tmpPara = QString("%1;%2").arg(ui->edit_323_text1->text()).arg(ui->edit_323_text2->text());
    tmpElement.setAttribute("Text", tmpPara);
    mUiManager->m_IsChanged = true;
}

void DeviceManager::on_edit_323_text2_editingFinished()
{
    int row = ui->list_323_logo->currentRow();
    QDomElement tmpElement = mFunctionTextHash.value(row);
    if(tmpElement.isNull())
        return;
    QString tmpPara = QString("%1;%2").arg(ui->edit_323_text1->text()).arg(ui->edit_323_text2->text());
    tmpElement.setAttribute("Text", tmpPara);
    mUiManager->m_IsChanged = true;
}


/*
    MDP文件中资源序号用于标识哪类资源可用
    UI文件中的资源号仅仅用于资源映射
*/
void DeviceManager::slot_InitTheme(QString pPath)
{
    if(!OpenThemeUI(pPath))
    {
        MsgBox tmpMsgBox(tr("UI file\n%1\n is broken!").arg(getFileName(pPath)));
        tmpMsgBox.exec();
        mOperateState = false;
        return;
    }

    InitFlrMap();
    InitLogoList();

    QDomElement tmpElement = mUiManager->m_ComElementGroup.value(COM_ARROW);
    if(!tmpElement.isNull())
    {
        QDomElement tmpSubElement = mUiManager->GetComState(tmpElement, "Run");
        if(!tmpSubElement.isNull())
        {
            ui->groupBox->setEnabled(true);
            QString tmAttr = tmpSubElement.attribute("EffectPara");
            QStringList tmpList = tmAttr.split(";");
            QString tmpParaString;
            foreach(tmpParaString, tmpList)
            {
                if(tmpParaString.contains("Interval", Qt::CaseInsensitive))
                {
                    QStringList tmpSubList = tmpParaString.split(":");
                    if(tmpSubList.count() == 2)
                    {
                        QString tmpPara = tmpSubList.at(1);
                        int index = tmpPara.indexOf('@');
                        if(index > 0)
                        {
                            bool ok = false;
                            int tmpNum = tmpPara.mid(0, index).toInt(&ok, 10);
                            if(ok)
                            {
                                QStringList tmpValueList = tmpPara.mid(index+1).split(',');
                                if(!tmpValueList.isEmpty())
                                {
                                    int count = qMin(tmpValueList.count(), 4);
                                    for(int m=0;m<count;m++)
                                    {
                                        m_ArrowSpeed[m] = tmpValueList.at(m).toInt();
                                    }
                                    if(tmpNum >= tmpValueList.count())
                                        tmpNum = tmpValueList.count() - 1;
                                    tmpPara = tmpValueList.at(tmpNum);
                                }
                            }
                        }
                        int tmpValue = tmpPara.toInt();
                        //>@6s一圈6000  2圈3000  3圈2000  4圈1500
                        if(tmpValue >= m_ArrowSpeed[0])
                            ui->combo_arrowspeed->setCurrentIndex(0);
                        else if(tmpValue >= m_ArrowSpeed[1])
                            ui->combo_arrowspeed->setCurrentIndex(1);
                        else if(tmpValue >= m_ArrowSpeed[2])
                            ui->combo_arrowspeed->setCurrentIndex(2);
                        else
                            ui->combo_arrowspeed->setCurrentIndex(3);

#if 0
                        ui->slider_arrowspeed->mQesp = this;
                        //ui->slider_arrowspeed->setSingleStep(1500);
                        ui->slider_arrowspeed->bindLabel(ui->label_arrowspeed, QString("%1s"));
                        ui->slider_arrowspeed->setRange(1000, 6000);
                        ui->slider_arrowspeed->WriteValue(tmpValue);
                        ui->slider_arrowspeed->setProperty("valueChanged", QString("<Theme><Arrow><StateMachine><Run EffectPara=\"Interval:%1\"/></StateMachine></Arrow></Theme>"));
#endif
                    }
                }
            }
        }
        else
        {
            ui->groupBox->setDisabled(true);
        }
    }

    QString tmpStylePath = GlobalPara.m_SoftwareDir + D_COM_DIR;
    //>@Style
    QString tmpPath = tmpStylePath + QString("Arrow/");
    QDir tmpArrowDir(tmpPath);
    QStringList folders = tmpArrowDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (int i = 0; i < folders.size(); ++i)
    {
        QString tmpFolderName = folders.at(i);
        QListWidgetItem* tmpItem = AddListItem(ui->list_322_arrows, tmpPath, tmpFolderName);
        if(tmpItem)
        {
            ui->list_322_arrows->setCurrentItem(tmpItem);
            QString tmpPath1 = tmpPath+tmpFolderName+QString("/");
            QDir tmpDir(tmpPath1);
            QStringList tmpFileList = tmpDir.entryList(QDir::Files);
            if(!tmpFileList.isEmpty())
                tmpItem->setIcon(QIcon(tmpPath1+tmpFileList.first()));
        }
    }

    tmpPath = tmpStylePath + QString("Floor/");
    QDir tmpFloorDir(tmpPath);
    folders = tmpFloorDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (int i = 0; i < folders.size(); ++i)
    {
        QString tmpFolderName = folders.at(i);
        QListWidgetItem* tmpItem = AddListItem(ui->list_321_floor, tmpPath, folders.at(i));
        if(tmpItem)
        {
            ui->list_321_floor->setCurrentItem(tmpItem);
            QString tmpPath1 = tmpPath+tmpFolderName+QString("/");
            QDir tmpDir(tmpPath1);
            QStringList tmpFileList = tmpDir.entryList(QDir::Files);
            if(!tmpFileList.isEmpty())
                tmpItem->setIcon(QIcon(tmpPath1+tmpFileList.first()));
        }
    }

//    tmpPath = tmpStylePath + QString("Function/");
//    QDir tmpFuncDir(tmpPath);
//    folders = tmpFuncDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
//    for (int i = 0; i < folders.size(); ++i)
//    {
//        QString tmpFolderName = folders.at(i);
//        QListWidgetItem* tmpItem = AddListItem(ui->list_323_logo, tmpPath, folders.at(i));
//        if(tmpItem)
//        {
//            ui->list_323_logo->setCurrentItem(tmpItem);
//            QString tmpPath1 = tmpPath+tmpFolderName+QString("/");
//            QDir tmpDir(tmpPath1);
//            QStringList tmpFileList = tmpDir.entryList(QDir::Files);
//            if(!tmpFileList.isEmpty())
//                tmpItem->setIcon(QIcon(tmpPath1+tmpFileList.first()));
//        }
//    }

    tmpPath = tmpStylePath + QString("Desktop/");
    QDir tmpDesttopDir(tmpPath);
    folders = tmpDesttopDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (int i = 0; i < folders.size(); ++i)
    {
        QString tmpFolderName = folders.at(i);
        QListWidgetItem* tmpItem = AddListItem(ui->list_324_background, tmpPath, folders.at(i));
        if(tmpItem)
        {
            ui->list_324_background->setCurrentItem(tmpItem);
            QString tmpPath1 = tmpPath+tmpFolderName+QString("/");
            QDir tmpDir(tmpPath1);
            QStringList tmpFileList = tmpDir.entryList(QDir::Files);
            if(!tmpFileList.isEmpty())
                tmpItem->setIcon(QIcon(tmpPath1+tmpFileList.first()));
        }
    }

    QListWidgetItem* tmpCurItem = 0;
    QString tmpCurArrowFile = mBufferPath + D_THEMEDIR + QString("Sodimas/Arrow/Resource/Image/T1.png");
    QFile tmpCurArrowFile1(tmpCurArrowFile);
    qint32 tmpCurArrowSize = tmpCurArrowFile1.size();

    for (int i = 0; i < folders.size(); ++i)
    {
        QString tmpFolderName = folders.at(i);
        QListWidgetItem* tmpItem = AddListItem(ui->list_32_style, tmpStylePath, folders.at(i));
        if(tmpItem)
        {
            //ui->list_32_style->setCurrentItem(tmpItem);
            QString tmpArrowPath = tmpStylePath+QString("Arrow/")+tmpFolderName+QString("/T1.png");
            QString tmpFloorPath = tmpStylePath+QString("Floor/")+tmpFolderName+QString("/T48.png");
            QString tmpDesktopPath = tmpStylePath+QString("Desktop/")+tmpFolderName+QString("/T0.png");

            QImage tmpImage(480, 272, QImage::Format_ARGB32_Premultiplied);
            tmpImage.fill(Qt::transparent); //>@创建透明图层
            QPainter p(&tmpImage);
            p.drawPixmap(0,0,480,272,QPixmap(tmpDesktopPath));
            p.drawPixmap(QRect(12, 36, 223, 205),QPixmap(tmpArrowPath));
            QRect tmpRect = QRect(246, 35, 226, 165);
            tmpRect.setX(tmpRect.x() + tmpRect.width()/3);
            tmpRect.setWidth(tmpRect.width()/3);
            p.drawPixmap(tmpRect,QPixmap(tmpFloorPath));
            p.end();

            tmpItem->setIcon(QIcon(QPixmap::fromImage(tmpImage)));

            QFile tmpFile1(tmpArrowPath);
            qint32 tmpSize1 = tmpFile1.size();
            if(tmpSize1 == tmpCurArrowSize)
            {
                tmpCurItem = tmpItem;
            }
        }
    }
    if(tmpCurItem)
    {
        ui->list_32_style->setCurrentItem(tmpCurItem);
    }

    //>@初始化Language
    ui->list_33_language->clear();
    QString tmpCurLanguage = GetLanguage();
    tmpPath = tmpStylePath + QString("Audio/");
    QDir tmpLanuDir(tmpPath);
    folders = tmpLanuDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (int i = 0; i < folders.size(); ++i)
    {
        QString tmpFolderName = folders.at(i);
        QListWidgetItem* tmpItem = AddListItem(ui->list_33_language, tmpPath, tmpFolderName);
        if(tmpItem)
        {
            IDE_TRACE_STR(tmpFolderName);
            IDE_TRACE_STR(tmpCurLanguage);
            if(tmpCurLanguage.compare(tmpFolderName, Qt::CaseInsensitive) == 0)
            {
                ui->list_33_language->setCurrentItem(tmpItem);
            }
        }
    }

    ui->list_33_audio->clear();
    QString tmpDstPath = mBufferPath + D_THEMEDIR + QString("Sodimas/Media/Resource/Audio/");
    QDir tmpDir(tmpDstPath);
    QStringList files = tmpDir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    for(int i=0;i<files.count();i++)
    {
        QString tmpAudioName = files.at(i);
        AddListItem(ui->list_33_audio, tmpDstPath, tmpAudioName);
    }

    //>@初始化楼层映射

    m_BaseTimer.start();
}

void DeviceManager::slot_Connect()
{
    m_CmdString.clear();
    slot_InitFromNetwork();
    if(m_LoginState)
    {
        MsgBox tmpMsgBox(tr("Completely,\nDisplay will be reboot!"), false);
        tmpMsgBox.exec();
        m_TcpSocket->Cmd("reboot");
        QtSleep(1000);
        emit sDisconnect();
    }
}

void DeviceManager::slot_Disconnect()
{
    Async();
    Disconnected();

    Reboot();
}

void DeviceManager::slot_InitFromNetwork()
{
    m_LoginState = false;
    m_CmdString.clear();
    mCurNetDev = ui->list_device->currentItem();
    if(!mCurNetDev)
        return;
    QString tmpAddr = mCurNetDev->text();
    if(tmpAddr.isEmpty() || ui->edit_wifipasswd->text().isEmpty())
        return;

#ifdef D_JIN_TEST
    tmpAddr = QString("BVT510_WIFI");
#endif

    ShowBusy(true, 0x0fffffffff);
    IDE_TRACE_STR(tmpAddr);
    if(!m_TcpSocket->GetDevInfo(tmpAddr, ui->edit_wifipasswd->text(), 5000))
    {
        MsgBox tmpMsgBox(tr("Usrname or Passwd is incorrect,\n"
                            "Please check it!"), false);
        tmpMsgBox.exec();
        ShowBusy(false);
        return;
    }

    ShowBusy(false);

    m_TcpSocket->HB(1000, false);

    //>@将当前工程下载到芯子
    QString tmpProjectPath = GlobalPara.m_SoftwareDir + D_DEV_DIR + D_DEV_OFFLINE_DIR + tmpAddr;
    tmpProjectPath.replace("\\", "/");
    if(!tmpProjectPath.endsWith("/"))
        tmpProjectPath.append("/");
    ClearFolder(tmpProjectPath);

    MsgBox tmpMsgBox(tr("Do you want to download all or partial?\nIf all, press YES!"), true, LEVEL_QUESTION);
    tmpMsgBox.SetText(QString("NO"), QString("YES"));
    int ret = tmpMsgBox.exec();

    ShowBusy(true, 0x0fffffffff);

    if(ret == QDialog::Accepted)
    {
        if(m_TcpSocket->isConnected())
        {
            QString tmpDiskPath = m_TcpSocket->m_DevInfoList.value("Disk");
            if(!m_TcpSocket->GetDir(tmpDiskPath, tmpProjectPath))
            {
                MsgBox tmpMsgBox(tr("Sync project error!\n"));
                tmpMsgBox.exec();
                ShowBusy(false);
                return;
            }
            else
            {
                MsgBox tmpMsgBox(tr("Sync project Success!\n"));
                tmpMsgBox.exec();
            }
        }
        else
        {
            MsgBox tmpMsgBox(tr("Network is broken!\n"));
            tmpMsgBox.exec();
            ShowBusy(false);
            return;
        }
    }
    else
    {
        if(m_TcpSocket->isConnected())
        {
            CreateDeviceDir(tmpProjectPath);

            QString tmpDiskPath = m_TcpSocket->m_DevInfoList.value("Disk");
            //>@只拷贝常用更新,mdp、ui文件、style相关、程序、系统参数
            if(ui->check_application->isChecked())
            {
                m_TcpSocket.data()->GetFile("Y", tmpDiskPath+QString("EXE/SOD_DISPLAY"), tmpProjectPath+QString("EXE/SOD_DISPLAY"));
            }
            if(ui->check_conffile->isChecked())
            {
                m_TcpSocket.data()->GetFile("Y", tmpDiskPath+QString("BOOT/Device.mdp"), tmpProjectPath+QString("BOOT/Device.mdp"));
                m_TcpSocket.data()->GetFile("Y", tmpDiskPath+QString("THEME/Sodimas/Theme.ui"), tmpProjectPath+QString("THEME/Sodimas/Theme.ui"));
            }
            if(ui->check_sysfile->isChecked())
            {
                m_TcpSocket.data()->GetFile("Y", tmpDiskPath+QString("SYSTEM/Middleware/Qt/init"), tmpProjectPath+QString("SYSTEM/Middleware/Qt/init"));
                m_TcpSocket.data()->GetFile("Y", tmpDiskPath+QString("SYSTEM/Middleware/WIFI/RT2870AP.dat"), tmpProjectPath+QString("SYSTEM/Middleware/WIFI/RT2870AP.dat"));
                m_TcpSocket.data()->GetFile("Y", tmpDiskPath+QString("Physical/Dial/GetRotation"), tmpProjectPath+QString("Physical/Dial/GetRotation"));
            }
            if(ui->check_style->isChecked())
            {
                m_TcpSocket.data()->GetDir(tmpDiskPath+QString("THEME/Sodimas/Arrow/"), tmpProjectPath+QString("THEME/Sodimas/Arrow/"));
                m_TcpSocket.data()->GetDir(tmpDiskPath+QString("THEME/Sodimas/Function/"), tmpProjectPath+QString("THEME/Sodimas/Function/"));
                m_TcpSocket.data()->GetDir(tmpDiskPath+QString("THEME/Sodimas/Floor/"), tmpProjectPath+QString("THEME/Sodimas/Floor/"));
                m_TcpSocket.data()->GetDir(tmpDiskPath+QString("THEME/Sodimas/Desktop/"), tmpProjectPath+QString("THEME/Sodimas/Desktop/"));
                m_TcpSocket.data()->GetDir(tmpDiskPath+QString("THEME/Sodimas/Media/"), tmpProjectPath+QString("THEME/Sodimas/Media/"));
            }
            MsgBox tmpMsgBox(tr("Sync project Success!\n"));
            tmpMsgBox.exec();
        }
        else
        {
            MsgBox tmpMsgBox(tr("Network is broken!\n"));
            tmpMsgBox.exec();
            ShowBusy(false);
            return;
        }
    }
    ShowBusy(false);

    m_LoginState = true;
}

//>@pNew表示是否新建
void DeviceManager::slot_InitFromLocal(QString pPath, bool pNew)
{
    ShowBusy(true, 0x0fffffff);
    if(!pPath.endsWith("/"))
        pPath.append("/");
    //>@在OFFLINE文件夹中创建时间文件夹
    if(pNew)
    {
        QString tmpProPath = GlobalPara.m_SoftwareDir + D_DEV_DIR + D_DEV_OFFLINE_DIR + QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss");
        if(!tmpProPath.endsWith("/"))
            tmpProPath.append("/");
        if(!CreateDeviceDir(tmpProPath))
        {
            IDE_TRACE();
            ShowBusy(false);
            return;
        }
        CopyFolder(pPath, tmpProPath, true, true, true);
        pPath = tmpProPath;
    }
    if(pPath.compare(mBufferPath, Qt::CaseInsensitive))
    {
        mBufferPath = pPath;
        mBufferPath.replace("\\", "/");
        iniParse *tmpParse = new iniParse(GlobalPara.m_SoftwareDir + D_ETC_DIR + D_ETC_BOOT_FILE);
        tmpParse->setProperty("Project", mBufferPath);
        tmpParse->deleteLater();
    }

    QString tmpDeviceFile = mBufferPath + D_BOOTDIR + D_MDPNAME;
    slot_InitDevice(tmpDeviceFile);
    if(mOperateState == false)
    {
        ShowBusy(false);
        slot_GotoStep(1);
        return;
    }

    QString tmpThemeFile = mBufferPath + D_THEMEDIR + QString("Sodimas/") + D_UINAME;
    slot_InitTheme(tmpThemeFile);
    if(mOperateState == false)
    {
        ShowBusy(false);
        slot_GotoStep(1);
        return;
    }

    bool isShowFloor = true;
    UpdateDevImage(mRotation, mThemeLayout, isShowFloor);

    //>@加载当前设备的LOGO图片
    QString tmpDstPath = mBufferPath+D_BOOTDIR+QString("logo.png");
    QPixmap tmpPixmap = QPixmap(tmpDstPath);
    if(!tmpPixmap.isNull())
    {
        QSize pReferSize = tmpPixmap.size();
        QRectF tmpResRect;
        QRectF tmpWidRect = QRectF(ui->wid_logo->rect().adjusted(10, 10, -10, -10));
        qreal tmpLength = qMin(tmpWidRect.width(), tmpWidRect.height());
        QPointF tmpCenter = QPointF(tmpWidRect.center());
        if(pReferSize.width() > pReferSize.height())
        {
            qreal tmpRatio = tmpLength / (qreal)(pReferSize.width());
            qreal tmpHeight = (qreal)(pReferSize.height()) * tmpRatio;
            //>@以宽度为准
            tmpResRect = QRectF(tmpCenter.x()-tmpLength/2.0, tmpCenter.y()-tmpHeight/2.0, tmpLength, tmpHeight);
        }
        else
        {
            qreal tmpRatio = tmpLength / (qreal)(pReferSize.height());
            qreal tmpWidth = (qreal)(pReferSize.width()) * tmpRatio;
            //>@以高度为准
            tmpResRect = QRectF(tmpCenter.x()-tmpWidth/2.0, tmpCenter.y()-tmpLength/2.0, tmpWidth, tmpLength);
        }
        ui->label_344_logo->setGeometry(tmpResRect.toRect());
        ui->label_344_logo->setPixmap(tmpPixmap);
        ui->label_344_logo->setToolTip(tmpDstPath);
    }

    ShowBusy(false);

    slot_GotoStep(2);
}

void DeviceManager::slot_WIFIOperate(int id)
{
    if(id == 0)  //>@scan
    {
        mCurNetDev = 0;
        ShowBusy(true, 0xffffffff);
        QtSleep(10);
        QList<NetworkItem*> tmpList = mNetworkManager->GetNetConfs(QNetworkConfiguration::BearerWLAN);
        IDE_TRACE_INT(tmpList.count());
        NetworkItem *tmpItem = 0;
        foreach(tmpItem, tmpList)
        {
            QtSleep(1);
            AddNetworkToList(tmpItem);
        }
        ShowBusy(false);
    }
    else if(id == 1) //test
    {
        QListWidgetItem *tmpItem = ui->list_device->currentItem();
        if(!tmpItem)
        {
            MsgBox tmpMsgBox(tr("Has't select a valid device,\n"
                                "Please select firstly!"));
            tmpMsgBox.exec();
            return;
        }
        NetworkItem *tmpNetItem = VAR_CAST<NetworkItem*>((QObject*)(tmpItem->data(D_NETSSID).toUInt()));
        if(!tmpNetItem)
        {
            MsgBox tmpMsgBox(tr("This network is invalid,\n"
                                "Please check it!"));
            tmpMsgBox.exec();
            return;
        }
        ShowBusy(true, 0xffffffff);
        QString tmpIPString;
        bool find = false;
#ifndef SUPPORT_LOCALTEST
        if(tmpNetItem->Connect(10000))
#endif
        {
            QStringList tmpArpDevices;
            for(int i=0;i<4;i++)
            {
                QtSleep(10);
                tmpArpDevices = ScanNetwork();
                if(!tmpArpDevices.isEmpty())
                    break;
            }
            IDE_TRACE_INT(tmpArpDevices.count());
            if(!tmpArpDevices.isEmpty())
            {
                QString tmpDevice;
                foreach(tmpDevice, tmpArpDevices)
                {
                    QHostAddress tmpIP;
                    quint16 tmpPort;
                    QString tmpCode;
                    if(!ParseIP(tmpDevice, tmpIP, tmpPort, tmpCode))
                        continue;
                    tmpIPString = tmpIP.toString();
                    tmpItem->setData(D_IPADDR, tmpIPString);
                    IDE_TRACE_STR(tmpIPString);
                    find = true;
                    break;
                }
            }
        }
#ifndef SUPPORT_LOCALTEST
        //tmpNetItem->Disconnect(5000);
#endif
        ShowBusy(false);
        if(find)
        {
            tmpItem->setIcon(QIcon(":/Icon/rc/Icon/on.png"));
        }
        else
        {
            tmpItem->setIcon(QIcon(":/Icon/rc/Icon/off.png"));
            MsgBox tmpMsgBox(tr("Can't find any valid device,\n"
                                "Please check it!"));
            tmpMsgBox.exec();
            return;
        }
    }
    else if(id == 2) //login
    {
        mCurNetDev = ui->list_device->currentItem();
        if(!(mCurNetDev && !mCurNetDev->data(D_IPADDR).toString().isEmpty()))
        {
            MsgBox tmpMsgBox(tr("Has't select a valid device,\n"
                                "Please select firstly!"));
            tmpMsgBox.exec();
            return;
        }
        QString tmpAddr = mCurNetDev->text();
        if(tmpAddr.isEmpty() || ui->edit_wifipasswd->text().isEmpty())
        {
            MsgBox tmpMsgBox(tr("User information is incompletly,\n"
                                "Please check it!"));
            tmpMsgBox.exec();
            return;
        }
        mManualExit = false;
        if(mCurNetDev)
        {
            if(!m_TcpSocket.isNull() && m_TcpSocket->mTcpClient && m_TcpSocket->mTcpClient->isConnected())
            {
                slot_InitFromNetwork();
            }
            else
            {
#ifdef D_JIN_TEST
                //Sync(QHostAddress("172.26.160.1"), D_NMPORT);
                Sync(QHostAddress("127.0.0.1"), D_NMPORT);
                //Sync(QHostAddress("192.168.1.10"), D_NMPORT);
#else
                Sync(QHostAddress(ui->edit_wifiip->text()), D_NMPORT);
#endif
                return;
            }
        }
        MsgBox tmpMsgBox(tr("This network is invalid,\n"
                            "Please check it!"));
        tmpMsgBox.exec();
        return;
    }
    else if(id == 4) //loginout
    {
        mManualExit = true;
        if(isOfflineLogin())
        {
            //>@提示是否导出系统到某个文件夹
            MsgBox tmpMsgBox(tr("Do you want to export configuration?\n"
                                "If not, please click NO!"), true, LEVEL_QUESTION);
            tmpMsgBox.SetText(QString("NO"), QString("YES"));
            int ret = tmpMsgBox.exec();
            if(ret == QDialog::Accepted)
            {
                ShowBusy(true, 0xffffffff);
                //>@选择路径
                static QString sDir = "/";
                sDir = QFileDialog::getExistingDirectory( this, tr("Open Directory"),
                                                          sDir,
                                                          QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
                if(!sDir.endsWith("/"))
                    sDir.append("/");

                MsgBox tmpQuestionBox(tr("Do you want to clear folder?\n"
                                    "If not, please click cancel!"), true, LEVEL_QUESTION);
                int ret = tmpQuestionBox.exec();
                if(ret == QDialog::Accepted)
                    ClearFolder(sDir);
                CopyFolder(mBufferPath, sDir, true, true, true);
                ShowBusy(false);
            }
            Disconnected(false);
#if 0
            qApp->exit();
#endif
        }
        else
        {
            ShowBusy(true, 0xffffffff);
            //>@提示是否将更新同步至设备
            MsgBox tmpMsgBox(tr("Do you want to sync the changed?\n"
                                "If not, please click NO!"), true, LEVEL_QUESTION);
            tmpMsgBox.SetText(QString("NO"), QString("YES"));
            int ret = tmpMsgBox.exec();
            if(ret == QDialog::Accepted)
            {
                //UploadRcs();
            }
#if 0
            mNetTransFlag = false;
            mNetManager->Set("<Physical><LAN>wifi:close</LAN></Physical>");
            QtWait(mNetTransFlag, 1000);
#else
            Reboot(true);
#endif
            ShowBusy(false);
            //>@退出登录后自动关闭
#if 0
            qApp->exit();
#endif
            Async();
        }
        slot_GotoStep(12);
    }
}

void DeviceManager::slot_SettingPage(int id)
{
    switch(id)
    {
        case 0:
            slot_GotoStep(341);
            break;
        case 1:
            slot_GotoStep(342);
            break;
        case 2:
            slot_GotoStep(343);
            break;
        case 3:
            slot_GotoStep(344);
            break;
        case 4:
            slot_GotoStep(345);
            break;
        default:
            break;
    }
}

void DeviceManager::slot_NextStep(int id)
{
    switch(id)
    {
        case 0: //btn_32_2_next
            slot_GotoStep(321);
            break;
        case 1: //btn_321_next
            slot_GotoStep(322);
            break;
        case 2: //btn_322_next
            slot_GotoStep(323);
            break;
        case 3: //btn_323_next
            slot_GotoStep(324);
            break;
        default:
            break;
    }
}

void DeviceManager::slot_BackStep(int id)
{
    switch(id)
    {
        case 201:
            slot_GotoStep(1);
            break;
        case 202:
        case 203:
        {
            if(mBufferPath.isEmpty())
                slot_GotoStep(1);
            else
                slot_GotoStep(2);
            break;
        }
        case 204:
        {
            slot_GotoStep(2);
            break;
        }
        case 2021:
        {
            slot_GotoStep(202);
            break;
        }
        case 3:
            slot_ProModify(3);
            slot_GotoStep(2);
            break;
        case 31:
        {
//            if(mMdpManager && mMdpManager->m_IsChanged)
//                mMdpManager->SaveMDP();
//            if(mUiManager && mUiManager->m_IsChanged)
//                mUiManager->SaveUI();
            slot_GotoStep(3);
            break;
        }
        case 32:
            slot_GotoStep(3);
            break;
        case 3201:
            slot_GotoStep(32);
            break;
        case 3202:
            slot_StyleOperate(5);
            slot_GotoStep(32);
            break;
        case 321:
            slot_GotoStep(3202);
            break;
        case 322:
            slot_GotoStep(3202);
            break;
        case 323:
            mUiManager->SaveUI();
            slot_GotoStep(32);
            break;
        case 324:
            slot_GotoStep(3202);
            break;
        case 33:
            slot_GotoStep(3);
            break;
        case 3301:
            slot_GotoStep(33);
            break;
        case 3302:
            slot_GotoStep(33);
            break;
        case 34:
        {
//            if(mMdpManager && mMdpManager->m_IsChanged)
//                mMdpManager->SaveMDP();
            if(ui->stacked_flrmap->currentIndex() == 0)
                SaveFlrMap();
            slot_GotoStep(3);
            break;
        }
        default:
            break;
    }
}

void DeviceManager::slot_GotoStep(int pStep)
{
    switch(pStep)
    {
        case 1:
        case 12:
        {
            //>@恢复
            mFloorMapHash.clear();
            mMediaMapHash.clear();
            mFunctionTextHash.clear();

            ui->btn_test->setChecked(false);
            on_btn_test_clicked(false);
            m_Initialize = true;
            ui->MainStackedWidget->setCurrentWidget(ui->page1_language);
            break;
        }
        case 2:
        {
            if(mBufferPath.isEmpty())
            {
                slot_GotoStep(1);
                return;
            }
            if(mMdpManager)
                mMdpManager->SaveMDP();
            if(mUiManager)
                mUiManager->SaveUI();
            ui->MainStackedWidget->setCurrentWidget(ui->page2_mainpage);
            break;
        }
        case 201:
            ui->MainStackedWidget->setCurrentWidget(ui->page2_1_createpro);
            break;
        case 202:
            ui->MainStackedWidget->setCurrentWidget(ui->page2_2_downloadpro);
            break;
        case 2021:
            slot_WIFIOperate(0);
            ui->MainStackedWidget->setCurrentWidget(ui->page2_21_fromrwifi);
            break;
        case 203:
            ui->MainStackedWidget->setCurrentWidget(ui->page2_3_importpro);
            break;
        case 204:
            ui->MainStackedWidget->setCurrentWidget(ui->page2_4_simplepro);
            break;
        case 3:
            ui->MainStackedWidget->setCurrentWidget(ui->page3_modify);
            break;
        case 31:
            ui->MainStackedWidget->setCurrentWidget(ui->page31_layout);
            break;
        case 32:
            ui->MainStackedWidget->setCurrentWidget(ui->page32_style);
            break;
        case 3201:
            ui->MainStackedWidget->setCurrentWidget(ui->page32_1_import);
            break;
        case 3202:
            ui->MainStackedWidget->setCurrentWidget(ui->page32_2_create);
            break;
        case 321:
            ui->MainStackedWidget->setCurrentWidget(ui->page321_floor);
            break;
        case 322:
            ui->MainStackedWidget->setCurrentWidget(ui->page322_arrow);
            break;
        case 323:
            ui->MainStackedWidget->setCurrentWidget(ui->page323_logo);
            break;
        case 324:
            ui->MainStackedWidget->setCurrentWidget(ui->page324_background);
            break;
        case 33:
            ui->MainStackedWidget->setCurrentWidget(ui->page33_language);
//            //>@增加语言初始化
//            if(ui->list_33_language->count())
//                on_list_33_language_itemClicked(ui->list_33_language->item(0));
            break;
        case 3301:
            ui->MainStackedWidget->setCurrentWidget(ui->page33_1_import);
            break;
        case 3302:
            ui->MainStackedWidget->setCurrentWidget(ui->page33_2_new);
            break;
        case 34:
            ui->MainStackedWidget->setCurrentWidget(ui->page34_setting);
            break;
        case 341:
            ui->MainStackedWidget->setCurrentWidget(ui->page34_setting);
            ui->SettingStacked->setCurrentWidget(ui->page341_volume);
            break;
        case 342:
            ui->MainStackedWidget->setCurrentWidget(ui->page34_setting);
            ui->SettingStacked->setCurrentWidget(ui->page342_floors);
            break;
        case 343:
            ui->MainStackedWidget->setCurrentWidget(ui->page34_setting);
            ui->SettingStacked->setCurrentWidget(ui->page343_backlight);
            break;
        case 344:
            ui->MainStackedWidget->setCurrentWidget(ui->page34_setting);
            ui->SettingStacked->setCurrentWidget(ui->page344_logo);
            break;
        case 345:
            ui->MainStackedWidget->setCurrentWidget(ui->page34_setting);
            ui->SettingStacked->setCurrentWidget(ui->page345_wifi);
            break;
        default:
            break;
    }
}

void DeviceManager::slot_Enter()
{
    //>@初始化语言
    iniParse *tmpParse = new iniParse(GlobalPara.m_SoftwareDir + D_ETC_DIR + D_ETC_BOOT_FILE);
    QString tmpProPath;
    bool flag = tmpParse->getProperty("Project", tmpProPath);
    if(flag)
    {
        if(CheckDeviceDir(tmpProPath))
        {
            slot_InitFromLocal(tmpProPath, false);
            tmpParse->deleteLater();
            return;
        }
    }
    tmpParse->deleteLater();
    slot_GotoStep(1);
}

void DeviceManager::slot_Exit()
{
    qApp->exit();
}

void DeviceManager::slot_SelectLanguage(int id)
{
    iniParse *tmpParse = new iniParse(GlobalPara.m_SoftwareDir + D_ETC_DIR + D_ETC_BOOT_FILE);
    if(id == 0)
        tmpParse->setProperty("Language", QString("EN"));
    else if(id == 1)
        tmpParse->setProperty("Language", QString("FR"));
    else if(id == 2)
        tmpParse->setProperty("Language", QString("GE"));
    else if(id == 3)
        tmpParse->setProperty("Language", QString("CH"));
    tmpParse->deleteLater();
    Reboot();
}

void DeviceManager::slot_ProjectOperate(int id)
{
    switch(id)
    {
        case 0:  //>@btn_download
            slot_GotoStep(202);
            break;
        case 1:  //>@btn_import
        case 21:
        {
            QString tmpNewPath = QFileDialog::getExistingDirectory( this, tr("Select"),
                                                                    "/",
                                              QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
            if(tmpNewPath.isEmpty())
                return;
            if(!tmpNewPath.endsWith("/"))
                tmpNewPath.append("/");
            slot_InitFromLocal(tmpNewPath, false);
            break;
        }
        case 22: //>@从OFFLINE目录中进行选择
        {
            QString tmpNewPath = QFileDialog::getExistingDirectory( this, tr("Select"),
                                              GlobalPara.m_SoftwareDir + D_DEV_DIR + D_DEV_OFFLINE_DIR,
                                              QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
            if(tmpNewPath.isEmpty())
                return;
            if(!tmpNewPath.endsWith("/"))
                tmpNewPath.append("/");
            slot_InitFromLocal(tmpNewPath, false);
            break;
        }
        case 11:  //>@btn_2_3_import
        {
            QString tmpName = ui->edit_2_3_name->text();
            if(tmpName.isEmpty())
            {
                MsgBox tmpMsgBox(tr("The name is empty,\n"
                                    "Please input it firstly!"));
                tmpMsgBox.exec();
                return;
            }
            QString tmpPath = ui->edit_2_3_path->text();
            if(tmpPath.isEmpty())
            {
                MsgBox tmpMsgBox(tr("The path is empty,\n"
                                    "Please reselect firstly!"));
                tmpMsgBox.exec();
                return;
            }
            QString tmpProPath = GlobalPara.m_SoftwareDir + D_DEV_DIR + D_DEV_OFFLINE_DIR + tmpName;
            ShowBusy(true, 0xffffffff);
            if(CheckDeviceDir(tmpPath))
            {
                CopyFolder(tmpPath, tmpProPath, true, true, true);
                slot_InitFromLocal(tmpProPath, false);
            }
            else
            {
                MsgBox tmpMsgBox(tr("The path is invalid,\n"
                                    "Please reselect!"));
                tmpMsgBox.exec();
            }
            ShowBusy(false);
            break;
        }
        case 2:  //>@btn_modify
            slot_GotoStep(3);
            break;
        case 23:
        case 3:  //>@btn_new
            slot_GotoStep(201);
            break;
        case 13:  //>@btn_2_1_create
        {
            QString tmpName = ui->edit_2_1_name->text();
            if(tmpName.isEmpty())
            {
                MsgBox tmpMsgBox(tr("The name is empty,\n"
                                    "Please input it firstly!"));
                tmpMsgBox.exec();
                return;
            }

            //>@不允许重名构建
            QDir tmpBufferDir(GlobalPara.m_SoftwareDir + D_DEV_DIR + D_DEV_OFFLINE_DIR);
            QStringList folders = tmpBufferDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
            if(folders.contains(tmpName, Qt::CaseInsensitive))
            {
                MsgBox tmpMsgBox(tr("The project has already exist,\n"
                                    "Please recreate!"));
                tmpMsgBox.exec();
                return;
            }

            QString tmpPath = GlobalPara.m_SoftwareDir + D_DEV_DIR + D_DEV_ONLINE_DIR + D_DEV_FACTORY_DIR;
            QString tmpProPath = GlobalPara.m_SoftwareDir + D_DEV_DIR + D_DEV_OFFLINE_DIR + tmpName;

            QDir tmpDir(tmpProPath);
            if(tmpDir.exists())
            {
                MsgBox tmpMsgBox(tr("The project is existed,\nDo you want to Delete?"), true, LEVEL_QUESTION);
                int ret = tmpMsgBox.exec();
                if(ret == QDialog::Accepted)
                {
                    ClearFolder(tmpProPath);
                }
                else
                {
                    return;
                }
            }

            ShowBusy(true, 0xFFFFFFFF);
            CopyFolder(tmpPath, tmpProPath, true, true, true);
            if(ui->radio_saf->isChecked())
            {
                QFile tmpMdpFile(tmpProPath+QString("/")+D_BOOTDIR+QString("Device_saf.mdp"));
                if(tmpMdpFile.exists())
                {
                    tmpMdpFile.rename(tmpProPath+QString("/")+D_BOOTDIR+QString("Device.mdp"));
                }
                QFile tmpMdpFile1(tmpProPath+QString("/")+D_BOOTDIR+QString("Device_com.mdp"));
                tmpMdpFile1.remove();

                QFile tmpUiFile(tmpProPath+QString("/")+D_THEMEDIR+QString("Sodimas/Theme_saf.ui"));
                if(tmpUiFile.exists())
                {
                    tmpUiFile.rename(tmpProPath+QString("/")+D_THEMEDIR+QString("Sodimas/Theme.ui"));
                }
                QFile tmpUiFile1(tmpProPath+QString("/")+D_THEMEDIR+QString("Sodimas/Theme_com.ui"));
                tmpUiFile1.remove();

                QDir tmpDir;
                DelFolder(tmpProPath+QString("/")+D_THEMEDIR+QString("Sodimas/Floor"));
                tmpDir.rename(tmpProPath+QString("/")+D_THEMEDIR+QString("Sodimas/Floor_saf"), tmpProPath+QString("/")+D_THEMEDIR+QString("Sodimas/Floor"));
                DelFolder(tmpProPath+QString("/")+D_THEMEDIR+QString("Sodimas/Floor_com"));
            }
            else if(ui->radio_canopen->isChecked())
            {
                QFile tmpFile(tmpProPath+QString("/")+D_BOOTDIR+QString("Device_com.mdp"));
                if(tmpFile.exists())
                {
                    tmpFile.rename(tmpProPath+QString("/")+D_BOOTDIR+QString("Device.mdp"));
                }
                QFile tmpMdpFile1(tmpProPath+QString("/")+D_BOOTDIR+QString("Device_saf.mdp"));
                tmpMdpFile1.remove();

                QFile tmpUiFile(tmpProPath+QString("/")+D_THEMEDIR+QString("Sodimas/Theme_com.ui"));
                if(tmpUiFile.exists())
                {
                    tmpUiFile.rename(tmpProPath+QString("/")+D_THEMEDIR+QString("Sodimas/Theme.ui"));
                }
                QFile tmpUiFile1(tmpProPath+QString("/")+D_THEMEDIR+QString("Sodimas/Theme_saf.ui"));
                tmpUiFile1.remove();

                QDir tmpDir;
                tmpDir.rename(tmpProPath+QString("/")+D_THEMEDIR+QString("Sodimas/Floor_com"), tmpProPath+QString("/")+D_THEMEDIR+QString("Sodimas/Floor"));
                DelFolder(tmpProPath+QString("/")+D_THEMEDIR+QString("Sodimas/Floor_saf"));
            }
            ShowBusy(false);
            slot_InitFromLocal(tmpProPath, false);
            break;
        }
        case 4:  //>@btn_remove
        {
            if(!mBufferPath.isEmpty())
            {
                MsgBox tmpQuestionBox(tr("Do you want to Delete this project?\n"
                                    "If not, please click cancel!"), true, LEVEL_QUESTION);
                int ret = tmpQuestionBox.exec();
                if(ret != QDialog::Accepted)
                    return;

                //>@删除之前必须先选择一个设备目录，同时要确保此设备是当前OFFLINE下的设备(比如直接打开的U盘设备就不允许删除)
                if(!mBufferPath.contains(GlobalPara.m_SoftwareDir + D_DEV_DIR + D_DEV_OFFLINE_DIR, Qt::CaseInsensitive))
                {
                    MsgBox tmpMsgBox(tr("You can't delete this device!"));
                    tmpMsgBox.exec();
                    return;
                }
                QString tmpOldPath = mBufferPath;
                ShowBusy(true, 0xffffffff);
                DelFolder(tmpOldPath, true);
                ShowBusy(false);
                slot_GotoStep(1);
            }
            break;
        }
        case 6:  //>@open
        {
            QString tmpNewPath = QFileDialog::getExistingDirectory( this, tr("Select"),
                                                                    "/",
                                              QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
            if(tmpNewPath.isEmpty())
                return;
            if(!tmpNewPath.endsWith("/"))
                tmpNewPath.append("/");
            slot_InitFromLocal(tmpNewPath, false);
            break;
        }
        case 7:  //>@export
        {
            slot_GotoStep(204);
            break;
        }
        case 8:
        {
            Reboot();
            break;
        }
        case 31:
        {
            slot_GotoStep(2021);
            break;
        }
        case 32:   //>@从USB上下载
        {
            QString tmpNewPath = QFileDialog::getExistingDirectory( this, tr("Select"),
                                              "/",
                                              QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
            if(tmpNewPath.isEmpty())
                return;
            tmpNewPath.replace("\\", "/");
            if(!tmpNewPath.endsWith("/"))
                tmpNewPath.append("/");
//            MsgBox tmpMsgBox(tr("Do you want to download?"), true, LEVEL_QUESTION);
//            int ret = tmpMsgBox.exec();
//            if(ret == QDialog::Accepted)
            {
                QString tmpDirName;
                QString tmpInfoPath = tmpNewPath + D_BOOTDIR + QString("info.dev");
                devManager *tmpManager = new devManager(tmpInfoPath);
                if(tmpManager->OpenDEV())
                {
                    tmpDirName = tmpManager->GetCode();
                }
                delete tmpManager;

                if(tmpDirName.isEmpty())
                {
                    MsgBox tmpMsgBox(tr("This device is invalid!"));
                    tmpMsgBox.exec();
                    return;
                }

                ShowBusy(true, 0xffffffff);
                FileOperate tmpOperate;
                tmpOperate.CopyFolder(tmpNewPath, GlobalPara.m_SoftwareDir + D_DEV_DIR + D_DEV_OFFLINE_DIR + tmpDirName, true, true, true);
                ShowBusy(false);

                slot_GotoStep(1);
            }
            break;
        }
        default:
            break;
    }
}

bool DeviceManager::SetWIFISSID(QString pSSID)
{
    if(pSSID.isEmpty())
        return false;
    QString tmpOldFilePath = QString(mBufferPath + "SYSTEM/Middleware/WIFI/RT2870AP.dat");
    QString tmpNewFilePath = QString(mBufferPath + "SYSTEM/Middleware/WIFI/RT2870AP_NEW.dat");
    QFile tmpFile(tmpOldFilePath);
    if(!tmpFile.open(QIODevice::ReadOnly))
        return false;
    QFile tmpNewFile(tmpNewFilePath);
    if(!tmpNewFile.open(QIODevice::WriteOnly))
        return false;
    while(!tmpFile.atEnd())
    {
        QByteArray tmpArray = tmpFile.readLine();
        if(tmpArray.startsWith("SSID="))
            tmpArray = QByteArray("SSID=")+pSSID.toLatin1()+QByteArray("\n");
        tmpNewFile.write(tmpArray);
    }
    tmpNewFile.close();
    tmpFile.close();
    tmpFile.remove();
    tmpNewFile.rename(tmpNewFilePath, tmpOldFilePath);
    return true;
}

void DeviceManager::slot_ProtocolOperate(int id)
{
    if(id == 0) //>@设置为SAF设备
    {

    }
    else if(id == 1) //>@设置为CANOPEN设备
    {

    }
}

void DeviceManager::slot_ProModify(int id)
{
    switch(id)
    {
        case 1:  //>@btn_language
            slot_GotoStep(33);
            break;
        case 2: //><@btn_layout
            slot_GotoStep(31);
            break;
        case 3: //><@btn_save
        {
            //>@保存当前工程
            if(mMdpManager)
            {
                SetWIFISSID(ui->edit_ssid->text());
                devManager *tmpManager = new devManager(mMdpManager->m_FileDir+"info.dev");
                if(tmpManager->OpenDEV())
                {
                    tmpManager->SetCode(ui->edit_ssid->text());
                    tmpManager->SaveDEV();
                }
                delete tmpManager;

                QString tmpPasswd = ui->edit_passwd->text();
                if(!tmpPasswd.isEmpty())
                    mMdpManager->SetPassword(tmpPasswd);

                mMdpManager->SaveMDP();
            }
            if(mUiManager && mUiManager->m_IsChanged)
                mUiManager->SaveUI();
            bool isShowFloor = true;
            UpdateDevImage(mRotation, mThemeLayout, isShowFloor);
            break;
        }
        case 7:  //>@导出应用程序包
        {
            QString tmpNewPath = QFileDialog::getExistingDirectory( this, tr("Select"),
                                              m_SelectPath,
                                              QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
            if(tmpNewPath.isEmpty())
                return;

            tmpNewPath.replace("\\", "/");
            if(!tmpNewPath.endsWith("/"))
                tmpNewPath.append("/");

            m_SelectPath = tmpNewPath;

            QString tmpSrcDir = mBufferPath + QString("EXE/");
            QString tmpDstDir = tmpNewPath + QString("EXE/");
            QString tmpZipFile = tmpNewPath + QString("APP_%1.zip").arg(QDate::currentDate().toString("yyMMdd"));

            if(QFile::exists(tmpZipFile))
            {
                MsgBox tmpMsgBox(tr("File existed! \n Do you want to Delete?"), true, LEVEL_QUESTION);
                int ret = tmpMsgBox.exec();
                if(ret == QDialog::Accepted)
                {
                    DelFile(tmpZipFile);
                }
                else
                {
                    return;
                }
            }

            //>@拷贝到某一个文件夹，然后在压缩导出。
            ShowBusy(true, 0xffffffff);

            CopyFolder(tmpSrcDir, tmpDstDir, true, true, true);
            QFile::rename(tmpDstDir + QString("SOD_DISPLAY"), tmpDstDir + QString("SOD_DISPLAY_NEW"));
            Zip(tmpDstDir, tmpZipFile);
            DelFolder(tmpDstDir);
            ShowBusy(false);

            QDesktopServices::openUrl(QUrl(tmpNewPath, QUrl::TolerantMode));
            break;
        }
        case 4: //><@btn_setting
            slot_GotoStep(34);
            break;
        case 5: //><@btn_style
            slot_GotoStep(32);
            break;
        case 6:
            QDesktopServices::openUrl(QUrl(mBufferPath, QUrl::TolerantMode));
            break;
        default:
            break;
    }
}

void DeviceManager::slot_ScreenOrientationOperate(int id)
{
    bool layout = ui->radio_srn_arrow->isChecked();
    bool size = ui->radio_srn_4_3->isChecked();
    int tmpLayout = 0;
    if(layout && size)
        tmpLayout = 0;
    else if(!layout && size)
        tmpLayout = 1;
    else if(layout && !size)
        tmpLayout = 2;
    else if(!layout && !size)
        tmpLayout = 3;
    if(id == 0)  //>@90
    {
        UpdateDevImage(90, tmpLayout, true);
    }
    else if(id == 2)   //>@270
    {
        UpdateDevImage(270, tmpLayout, true);
    }
    else if(id == 1)  //>@180
    {
        UpdateDevImage(180, tmpLayout, true);
    }
}

void DeviceManager::slot_ScreenSizeOperate(int id)
{
    bool layout = ui->radio_srn_arrow->isChecked();
    bool size = (id == 1);
    int rotate = 180;
    if(ui->radio_srn_90->isChecked())
        rotate = 90;
    else if(ui->radio_srn_180->isChecked())
        rotate = 180;
    else if(ui->radio_srn_270->isChecked())
        rotate = 270;
    int tmpLayout;
    if(layout && size)
        tmpLayout = 0;
    else if(!layout && size)
        tmpLayout = 1;
    else if(layout && !size)
        tmpLayout = 2;
    else if(!layout && !size)
        tmpLayout = 3;
    UpdateDevImage(rotate, tmpLayout, true);
}

void DeviceManager::slot_LayoutOperate(int id)
{
    bool layout = (id == 0);
    bool size = ui->radio_srn_4_3->isChecked();
    int rotate = 180;
    if(ui->radio_srn_90->isChecked())
        rotate = 90;
    else if(ui->radio_srn_180->isChecked())
        rotate = 180;
    else if(ui->radio_srn_270->isChecked())
        rotate = 270;
    int tmpLayout;
    if(layout && size)
        tmpLayout = 0;
    else if(!layout && size)
        tmpLayout = 1;
    else if(layout && !size)
        tmpLayout = 2;
    else if(!layout && !size)
        tmpLayout = 3;
    UpdateDevImage(rotate, tmpLayout, true);
}

void DeviceManager::slot_SimpleProOperate(int id)
{
    int flag = 0;
    if(ui->check_all->isChecked() || ui->check_application->isChecked() ||
       ui->check_conffile->isChecked() || ui->check_audio->isChecked() ||
       ui->check_style->isChecked() || ui->check_sysfile->isChecked())
    {
        QString tmpNewPath; //>@导出的路径
        if(id == 0)
        {
            tmpNewPath = QFileDialog::getExistingDirectory( this, tr("Select"),
                                                                    QDir::homePath(),
                                              QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
        }
        else
        {
            QMultiHash<int, DRIVER_INFO> tmpDiskInfo = GetDiskList();
            QList<DRIVER_INFO> tmpDiskList = tmpDiskInfo.values(DRIVE_FIXED);
            for(int i=0;i<tmpDiskList.count();i++)
            {
                DRIVER_INFO tmpInfo = tmpDiskList.at(i);
                if(tmpInfo.mLabel.startsWith("NAND1-2",Qt::CaseInsensitive))
                {
                    flag = 1;
                    tmpNewPath = tmpInfo.mPath;
                }
            }
            if(flag == 0)
            {
                QList<DRIVER_INFO> tmpDiskList = tmpDiskInfo.values(DRIVE_REMOVABLE);
                if(!tmpDiskList.isEmpty())
                {
                    DRIVER_INFO tmpInfo = tmpDiskList.first();
                    flag = 2;
                    tmpNewPath = tmpInfo.mPath + QString("\\DISK\\");
                }
            }
        }
        if(tmpNewPath.isEmpty())
            return;

        if(!tmpNewPath.endsWith("/"))
            tmpNewPath.append("/");
        tmpNewPath.replace("\\", "/");

        if(flag == 2)
        {
            QDir tmpDir(tmpNewPath);
            if(tmpDir.exists())
            {
                MsgBox tmpMsgBox(tr("Directory existed! \n Do you want to Delete?"), true, LEVEL_QUESTION);
                int ret = tmpMsgBox.exec();
                if(ret == QDialog::Accepted)
                {
                    ShowBusy(true, 0xffffffffff);
                    DelFolder(tmpNewPath, true);
                    ShowBusy(false);
                }
            }
        }
        CreateDeviceDir(tmpNewPath);

        if(ui->check_all->isChecked())
        {
            ShowBusy(true, 0xffffffff);
            FileOperate tmpOperate;
            tmpOperate.CopyFolder(mBufferPath, tmpNewPath, true, true, true);

            tmpOperate.CopyFolder(GlobalPara.m_SoftwareDir + D_COM_DIR + QString("Music/Style1/"), tmpNewPath+QString("THEME/Sodimas/Music/"), true, true);

            ShowBusy(false);
        }
        else
        {
            ShowBusy(true, 0xffffffff);
            //>@只拷贝常用更新,mdp、ui文件、style相关、程序、系统参数
            if(ui->check_application->isChecked())
            {
                QtSleep(100);
                FileOperate tmpOperate;
                tmpOperate.CopyFile(mBufferPath+QString("EXE/SOD_DISPLAY"), tmpNewPath+QString("EXE/SOD_DISPLAY"));
                CopyFile(mBufferPath+QString("EXE/CANOPEN.bin"), tmpNewPath+QString("EXE/CANOPEN.bin"));
            }
            if(ui->check_conffile->isChecked())
            {
                QtSleep(100);
                //CopyFile(mBufferPath+QString("BOOT/logo.png"), tmpNewPath+QString("BOOT/logo.png"));
                CopyFile(mBufferPath+QString("BOOT/info.dev"), tmpNewPath+QString("BOOT/info.dev"));
                CopyFile(mBufferPath+QString("BOOT/Device.mdp"), tmpNewPath+QString("BOOT/Device.mdp"));
                CopyFile(mBufferPath+QString("THEME/Sodimas/Theme.ui"), tmpNewPath+QString("THEME/Sodimas/Theme.ui"));
            }
            if(ui->check_sysfile->isChecked())
            {
                QtSleep(100);
                CopyFile(mBufferPath+QString("SYSTEM/Middleware/Qt/init"), tmpNewPath+QString("SYSTEM/Middleware/Qt/init"));
                //CopyFile(mBufferPath+QString("SYSTEM/Middleware/WIFI/RT2870AP.dat"), tmpNewPath+QString("SYSTEM/Middleware/WIFI/RT2870AP.dat"));
                CopyFile(mBufferPath+QString("SYSTEM/Physical/Dial/GetRotation"), tmpNewPath+QString("SYSTEM/Physical/Dial/GetRotation"));
            }
            if(ui->check_audio->isChecked())
            {
                QtSleep(100);
                CopyFolder(mBufferPath+QString("THEME/Sodimas/Media/"), tmpNewPath+QString("THEME/Sodimas/Media/"), true, true);
            }
            if(ui->check_style->isChecked())
            {
                QtSleep(100);
                CopyFolder(mBufferPath+QString("THEME/Sodimas/Arrow/"), tmpNewPath+QString("THEME/Sodimas/Arrow/"), true, true);
                CopyFolder(mBufferPath+QString("THEME/Sodimas/Function/"), tmpNewPath+QString("THEME/Sodimas/Function/"), true, true);
                CopyFolder(mBufferPath+QString("THEME/Sodimas/Floor/"), tmpNewPath+QString("THEME/Sodimas/Floor/"), true, true);
                CopyFolder(mBufferPath+QString("THEME/Sodimas/Desktop/"), tmpNewPath+QString("THEME/Sodimas/Desktop/"), true, true);
    //            if(id == 0)
    //            {
    //                Zip(tmpNewPath+QString("THEME/Sodimas/"), tmpNewPath + QString("THEME/Sodimas/Sodimas.zip"));
    //            }
            }
            ShowBusy(false);
        }
        MsgBox tmpMsgBox(tr("Export Completed! \n Do you want to explorer?"), true, LEVEL_QUESTION);
        int ret = tmpMsgBox.exec();
        if(ret == QDialog::Accepted)
        {
            QDesktopServices::openUrl(QUrl(tmpNewPath, QUrl::TolerantMode));
        }
        slot_GotoStep(2);
    }
    else
    {
        MsgBox tmpMsgBox(tr("Please Select Resources!"));
        tmpMsgBox.exec();
        return;
    }
}

void DeviceManager::slot_ZipTaskEnd()
{
    IDE_TRACE();
    m_ZipTaskState = Success;
}

void DeviceManager::slot_ZipTaskErr()
{
    IDE_TRACE();
    m_ZipTaskState = Failed;
}

void DeviceManager::slot_ZipTaskProgress(int ratio)
{
    IDE_TRACE_INT(ratio);
}

void DeviceManager::Zip(QString pSrc, QString pDst)
{
#ifdef SURRPORT_ZIP
    int pTimeout = 300000;
    QThread ZipThread;
    ZipTask compressZipTask;
    compressZipTask.mContainDir = false;
    connect(this,SIGNAL(sZip(QString, QString, bool, bool)),&compressZipTask,SLOT(Zip(QString, QString, bool, bool)),Qt::QueuedConnection);
    connect(&compressZipTask,SIGNAL(end()),this,SLOT(slot_ZipTaskEnd()),Qt::QueuedConnection);
    connect(&compressZipTask,SIGNAL(error()),this,SLOT(slot_ZipTaskErr()),Qt::QueuedConnection);
    connect(&compressZipTask,SIGNAL(progress(quint32)),this,SIGNAL(sProgress(quint32)),Qt::QueuedConnection);
    compressZipTask.moveToThread(&ZipThread);
    ZipThread.start(QThread::LowPriority);
    ZipThread.wait(10);
    m_ZipTaskState = Start;
    emit sZip(pSrc, pDst, true, false);
    while((m_ZipTaskState == Start) && (pTimeout > 0))
    {
        pTimeout -= 100;
        QtSleep(100);
    }
    disconnect(this,SIGNAL(sZip(QString, QString, bool, bool)), 0, 0);
    disconnect(&compressZipTask,SIGNAL(end()), 0, 0);
    disconnect(&compressZipTask,SIGNAL(error()), 0, 0);
    disconnect(&compressZipTask,SIGNAL(progress(quint32)), 0, 0);
    ZipThread.terminate();
    ZipThread.wait(3000);
#endif
}

void DeviceManager::UnZip(QString pSrc, QString pDst)
{
#ifdef SURRPORT_ZIP
    int pTimeout = 300000;
    QThread ZipThread;
    ZipTask decompressZipTask;
    connect(this,SIGNAL(sUnzip(QString, QString, bool, bool)),&decompressZipTask,SLOT(Unzip(QString, QString, bool, bool)),Qt::QueuedConnection);
    connect(&decompressZipTask,SIGNAL(end()),this,SLOT(slot_ZipTaskEnd()),Qt::QueuedConnection);
    connect(&decompressZipTask,SIGNAL(error()),this,SLOT(slot_ZipTaskErr()),Qt::QueuedConnection);
    connect(&decompressZipTask,SIGNAL(progress(quint32)),this,SIGNAL(sProgress(quint32)),Qt::QueuedConnection);
    decompressZipTask.moveToThread(&ZipThread);
    ZipThread.start(QThread::LowPriority);
    ZipThread.wait(10);
    m_ZipTaskState = Start;
    emit sUnzip(pSrc, pDst, true, false);
    while((m_ZipTaskState == Start) && (pTimeout > 0))
    {
        pTimeout -= 100;
        QtSleep(100);
    }
    disconnect(this,SIGNAL(sUnzip(QString, QString, bool, bool)), 0, 0);
    disconnect(&decompressZipTask,SIGNAL(end()), 0, 0);
    disconnect(&decompressZipTask,SIGNAL(error()), 0, 0);
    disconnect(&decompressZipTask,SIGNAL(progress(quint32)), 0, 0);
    ZipThread.terminate();
    ZipThread.wait(3000);
#endif
}

void DeviceManager::slot_StyleOperate(int id)
{
    switch(id)
    {
        case 7:
        {
            slot_GotoStep(323);
            break;
        }
        case 0:  //>@btn_32_create
            ui->edit_321_cur->setText(QString());
            ui->edit_322_cur->setText(QString());
            //ui->edit_323_cur->setText(QString());
            ui->edit_324_cur->setText(QString());
            slot_GotoStep(3202);
            break;
        case 1:  //>@btn_32_import
            slot_GotoStep(3201);
            break;
        case 2:
        {
            break;
        }
        case 3:  //>@导出压缩为zip包
        {
            QListWidgetItem *tmpItem = ui->list_32_style->currentItem();
            if(!tmpItem)
            {
                MsgBox tmpMsgBox(tr("You did't select style!"));
                tmpMsgBox.exec();
                return;
            }

            QString tmpNewPath = QFileDialog::getExistingDirectory( this, tr("Select"),
                                              "/",
                                              QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
            if(tmpNewPath.isEmpty())
                return;
            if(!tmpNewPath.endsWith("/"))
                tmpNewPath.append("/");

            //>@拷贝到某一个文件夹，然后在压缩导出。
            ShowBusy(true, 0xffffffff);
            QString tmpStylePath = tmpItem->data(D_LISTITEM_PATH).toString();
            QString tmpStyleName = tmpItem->data(D_LISTITEM_NAME).toString();

            QString tmpSrcPath = tmpStylePath + QString("Arrow/%1/").arg(tmpStyleName);
            QString tmpDstPath = tmpNewPath + tmpStyleName + QString("/Arrow/Resource/Image/");
            CopyFolder(tmpSrcPath, tmpDstPath, true, true, true);

            tmpSrcPath = tmpStylePath + QString("Floor/%1/").arg(tmpStyleName);
            tmpDstPath = tmpNewPath + tmpStyleName + QString("/Floor/Resource/Image/");
            CopyFolder(tmpSrcPath, tmpDstPath, true, true, true);

            tmpSrcPath = tmpStylePath + QString("Function/%1/").arg(tmpStyleName);
            tmpDstPath = tmpNewPath + tmpStyleName + QString("/Function/Resource/Image/");
            CopyFolder(tmpSrcPath, tmpDstPath, true, true, true);

            tmpSrcPath = tmpStylePath + QString("Desktop/%1/").arg(tmpStyleName);
            tmpDstPath = tmpNewPath + tmpStyleName + QString("/Desktop/Resource/Image/");
            CopyFolder(tmpSrcPath, tmpDstPath, true, true, true);

            Zip(tmpNewPath + tmpStyleName, tmpNewPath + tmpStyleName + QString(".zip"));
            ShowBusy(false);

            QDesktopServices::openUrl(QUrl(tmpNewPath, QUrl::TolerantMode));

            break;
        }
        case 4:
        {
            QListWidgetItem *tmpItem = ui->list_32_style->currentItem();
            if(!tmpItem)
            {
                MsgBox tmpMsgBox(tr("You did't select style!"));
                tmpMsgBox.exec();
                return;
            }
            MsgBox tmpMsgBox(tr("Do you want to delete %1!").arg(tmpItem->text()), true, LEVEL_QUESTION);
            int ret = tmpMsgBox.exec();
            if(ret == QDialog::Rejected)
                return;

            ShowBusy(true, 0xffffffff);
            QString tmpStylePath = GlobalPara.m_SoftwareDir + D_COM_DIR;
            QString tmpStyleName = tmpItem->data(D_LISTITEM_NAME).toString();
            QString tmpSrcPath = tmpStylePath + QString("Desktop/%1/").arg(tmpStyleName);
            DelFolder(tmpSrcPath, true);

            tmpSrcPath = tmpStylePath + QString("Floor/%1/").arg(tmpStyleName);
            DelFolder(tmpSrcPath, true);

            tmpSrcPath = tmpStylePath + QString("Function/%1/").arg(tmpStyleName);
            DelFolder(tmpSrcPath, true);

            tmpSrcPath = tmpStylePath + QString("Arrow/%1/").arg(tmpStyleName);
            DelFolder(tmpSrcPath, true);

            ShowBusy(false);

            ui->list_32_style->takeItem(ui->list_32_style->currentRow());
            break;
        }
        case 5:   //>@保存当前设置的style
        {
            //>@
            QString tmpStyleName = ui->edit_32_2_name->text();
            if(tmpStyleName.isEmpty())
            {
                MsgBox tmpMsgBox(tr("The name is empty,\n"
                                    "Please input it firstly!"));
                tmpMsgBox.exec();
                return;
            }

            QString tmpStylePath = GlobalPara.m_SoftwareDir + D_COM_DIR;
            //>@Style
            QString tmpPath = tmpStylePath + QString("Arrow/");
            QDir tmpArrowDir(tmpPath);
            QStringList folders = tmpArrowDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

            if(folders.contains(tmpStyleName, Qt::CaseInsensitive))
            {
                MsgBox tmpMsgBox(tr("The style has already exist,\n"
                                    "Please recreate!"));
                tmpMsgBox.exec();
                return;
            }

            ShowBusy(true, 0xffffffff);
            QString tmpSelectStylePath = ui->edit_322_cur->text();
            QString tmpNewStylePath = tmpPath + tmpStyleName;
            CopyFolder(tmpSelectStylePath, tmpNewStylePath, true, true, true);

            tmpPath = tmpStylePath + QString("Floor/");
            tmpSelectStylePath = ui->edit_321_cur->text();
            tmpNewStylePath = tmpPath + tmpStyleName;
            CopyFolder(tmpSelectStylePath, tmpNewStylePath, true, true, true);

            tmpPath = tmpStylePath + QString("Function/");
            //tmpSelectStylePath = ui->edit_323_cur->text();
            tmpNewStylePath = tmpPath + tmpStyleName;
            CopyFolder(tmpSelectStylePath, tmpNewStylePath, true, true, true);

            tmpPath = tmpStylePath + QString("Desktop/");
            tmpSelectStylePath = ui->edit_324_cur->text();
            tmpNewStylePath = tmpPath + tmpStyleName;
            CopyFolder(tmpSelectStylePath, tmpNewStylePath, true, true, true);

            //>@增加一个style
            QListWidgetItem* tmpItem = AddListItem(ui->list_32_style, tmpStylePath, tmpStyleName);
            if(tmpItem)
            {
                ui->list_32_style->setCurrentItem(tmpItem);
                QString tmpArrowPath = tmpStylePath+QString("Arrow/")+tmpStyleName+QString("/T1.png");
                QString tmpFloorPath = tmpStylePath+QString("Floor/")+tmpStyleName+QString("/T48.png");
                QString tmpDesktopPath = tmpStylePath+QString("Desktop/")+tmpStyleName+QString("/T0.png");

                QImage tmpImage(480, 272, QImage::Format_ARGB32_Premultiplied);
                tmpImage.fill(Qt::transparent); //>@创建透明图层
                QPainter p(&tmpImage);
                p.drawPixmap(0,0,480,272,QPixmap(tmpDesktopPath));
                p.drawPixmap(QRect(12, 36, 223, 205),QPixmap(tmpArrowPath));
                QRect tmpRect = QRect(246, 35, 226, 165);
                tmpRect.setX(tmpRect.x() + tmpRect.width()/3);
                tmpRect.setWidth(tmpRect.width()/3);
                p.drawPixmap(tmpRect,QPixmap(tmpFloorPath));
                p.end();

                tmpItem->setIcon(QIcon(QPixmap::fromImage(tmpImage)));
            }
            ShowBusy(false);
            break;
        }
        default:
            break;
    }
}

/*
  Style包的格式
    Arrow
        T1.png
*/
void DeviceManager::on_btn_32_1_import_clicked()
{
    QString tmpNewStylePath = ui->edit_32_1_path->text();
    QString tmpStyleName = ui->edit_32_1_name->text();
    if(tmpStyleName.isEmpty())
    {
        MsgBox tmpMsgBox(tr("The name is empty,\n"
                            "Please input it firstly!"));
        tmpMsgBox.exec();
        return;
    }
    if(tmpNewStylePath.isEmpty())
    {
        MsgBox tmpMsgBox(tr("The path is empty,\n"
                            "Please input it firstly!"));
        tmpMsgBox.exec();
        return;
    }
    tmpNewStylePath.replace("\\", "/");

    QString tmpNewPath;
    //>@如果是zip文件，需要先解压
    if(tmpNewStylePath.endsWith(".zip", Qt::CaseInsensitive))
    {
        int index = tmpNewStylePath.indexOf(".zip");
        tmpNewPath = tmpNewStylePath.mid(0, index);
        ShowBusy(true, 0xffffffffff);
        UnZip(tmpNewStylePath, tmpNewPath);
        ShowBusy(false);
    }
    else
    {
        tmpNewPath = tmpNewStylePath;
    }

    QString tmpStylePath = GlobalPara.m_SoftwareDir + D_COM_DIR;
    //>@Style
    QString tmpPath = tmpStylePath + QString("Arrow/");
    QDir tmpArrowDir(tmpPath);
    QStringList folders = tmpArrowDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    if(folders.contains(tmpStyleName, Qt::CaseInsensitive))
    {
        MsgBox tmpMsgBox(tr("The style has already exist,\n"
                            "Please recreate!"));
        tmpMsgBox.exec();
        return;
    }

    ShowBusy(true, 0xffffffffff);
    QString tmpImportStylePath =  tmpNewPath + QString("/Arrow/Resource/Image/");
    CopyFolder(tmpImportStylePath, tmpPath + tmpStyleName, true, true, true);

    tmpPath = tmpStylePath + QString("Floor/");
    tmpImportStylePath =  tmpNewPath + QString("/Floor/Resource/Image/");
    CopyFolder(tmpImportStylePath, tmpPath + tmpStyleName, true, true, true);

    tmpPath = tmpStylePath + QString("Function/");
    tmpImportStylePath =  tmpNewPath + QString("/Function/Resource/Image/");
    CopyFolder(tmpImportStylePath, tmpPath + tmpStyleName, true, true, true);

    tmpPath = tmpStylePath + QString("Desktop/");
    tmpImportStylePath =  tmpNewPath + QString("/Desktop/Resource/Image/");
    CopyFolder(tmpImportStylePath, tmpPath + tmpStyleName, true, true, true);

    //>@增加一个style
    QListWidgetItem* tmpItem = AddListItem(ui->list_32_style, tmpStylePath, tmpStyleName);
    if(tmpItem)
    {
        ui->list_32_style->setCurrentItem(tmpItem);
        QString tmpArrowPath = tmpStylePath+QString("Arrow/")+tmpStyleName+QString("/T1.png");
        QString tmpFloorPath = tmpStylePath+QString("Floor/")+tmpStyleName+QString("/T48.png");
        QString tmpDesktopPath = tmpStylePath+QString("Desktop/")+tmpStyleName+QString("/T0.png");

        QImage tmpImage(480, 272, QImage::Format_ARGB32_Premultiplied);
        tmpImage.fill(Qt::transparent); //>@创建透明图层
        QPainter p(&tmpImage);
        p.drawPixmap(0,0,480,272,QPixmap(tmpDesktopPath));
        p.drawPixmap(QRect(12, 36, 223, 205),QPixmap(tmpArrowPath));
        QRect tmpRect = QRect(246, 35, 226, 165);
        tmpRect.setX(tmpRect.x() + tmpRect.width()/3);
        tmpRect.setWidth(tmpRect.width()/3);
        p.drawPixmap(tmpRect,QPixmap(tmpFloorPath));
        p.end();

        tmpItem->setIcon(QIcon(QPixmap::fromImage(tmpImage)));
    }
    ShowBusy(false);
    slot_GotoStep(32);
}

void DeviceManager::on_btn_32_1_import_project_clicked()
{
    QString tmpNewStylePath = ui->edit_32_1_path_pro->text();
    QString tmpStyleName = ui->edit_32_1_name->text();
    if(tmpStyleName.isEmpty())
    {
        MsgBox tmpMsgBox(tr("The name is empty,\n"
                            "Please input it firstly!"));
        tmpMsgBox.exec();
        return;
    }
    if(tmpNewStylePath.isEmpty())
    {
        MsgBox tmpMsgBox(tr("The path is empty,\n"
                            "Please input it firstly!"));
        tmpMsgBox.exec();
        return;
    }
    tmpNewStylePath.append("THEME/Sodimas/");
    tmpNewStylePath.replace("\\", "/");

    QString tmpNewPath = tmpNewStylePath;

    QString tmpStylePath = GlobalPara.m_SoftwareDir + D_COM_DIR;
    //>@Style
    QString tmpPath = tmpStylePath + QString("Arrow/");
    QDir tmpArrowDir(tmpPath);
    QStringList folders = tmpArrowDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    if(folders.contains(tmpStyleName, Qt::CaseInsensitive))
    {
        MsgBox tmpMsgBox(tr("The style has already exist,\n"
                            "Please recreate!"));
        tmpMsgBox.exec();
        return;
    }

    ShowBusy(true, 0xffffffff);
    QString tmpImportStylePath =  tmpNewPath + QString("/Arrow/Resource/Image/");
    CopyFolder(tmpImportStylePath, tmpPath + tmpStyleName, true, true, true);

    tmpPath = tmpStylePath + QString("Floor/");
    tmpImportStylePath =  tmpNewPath + QString("/Floor/Resource/Image/");
    CopyFolder(tmpImportStylePath, tmpPath + tmpStyleName, true, true, true);

    tmpPath = tmpStylePath + QString("Function/");
    tmpImportStylePath =  tmpNewPath + QString("/Function/Resource/Image/");
    CopyFolder(tmpImportStylePath, tmpPath + tmpStyleName, true, true, true);

    tmpPath = tmpStylePath + QString("Desktop/");
    tmpImportStylePath =  tmpNewPath + QString("/Desktop/Resource/Image/");
    CopyFolder(tmpImportStylePath, tmpPath + tmpStyleName, true, true, true);

    //>@增加一个style
    QListWidgetItem* tmpItem = AddListItem(ui->list_32_style, tmpStylePath, tmpStyleName);
    if(tmpItem)
    {
        ui->list_32_style->setCurrentItem(tmpItem);
        QString tmpArrowPath = tmpStylePath+QString("Arrow/")+tmpStyleName+QString("/T1.png");
        QString tmpFloorPath = tmpStylePath+QString("Floor/")+tmpStyleName+QString("/T48.png");
        QString tmpDesktopPath = tmpStylePath+QString("Desktop/")+tmpStyleName+QString("/T0.png");

        QImage tmpImage(480, 272, QImage::Format_ARGB32_Premultiplied);
        tmpImage.fill(Qt::transparent); //>@创建透明图层
        QPainter p(&tmpImage);
        p.drawPixmap(0,0,480,272,QPixmap(tmpDesktopPath));
        p.drawPixmap(QRect(12, 36, 223, 205),QPixmap(tmpArrowPath));
        QRect tmpRect = QRect(246, 35, 226, 165);
        tmpRect.setX(tmpRect.x() + tmpRect.width()/3);
        tmpRect.setWidth(tmpRect.width()/3);
        p.drawPixmap(tmpRect,QPixmap(tmpFloorPath));
        p.end();

        tmpItem->setIcon(QIcon(QPixmap::fromImage(tmpImage)));
    }
    ShowBusy(false);
    slot_GotoStep(32);
}

void DeviceManager::slot_FloorOperate(int id)
{
    switch(id)
    {
        case 0:  //>@btn_321_import
        {
            QString tmpOldPath = ui->edit_321_cur->text();
            QString tmpBasePath = QString("/");
            if(!tmpOldPath.isEmpty())
                tmpBasePath = getParentDirectory(tmpOldPath);
            QString tmpNewPath = QFileDialog::getExistingDirectory( this, tr("Select"),
                                              tmpBasePath,
                                              QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
            if(!tmpNewPath.endsWith("/"))
                tmpNewPath.append("/");
            ui->edit_321_cur->setText(tmpNewPath);
            break;
        }
        case 2:  //>@btn_321_test

            break;
        default:
            break;
    }
}

void DeviceManager::slot_ArrowOperate(int id)
{
    switch(id)
    {
        case 0:  //>@btn_322_import
        {
            QString tmpOldPath = ui->edit_322_cur->text();
            QString tmpBasePath = QString("/");
            if(!tmpOldPath.isEmpty())
                tmpBasePath = getParentDirectory(tmpOldPath);
            QString tmpNewPath = QFileDialog::getExistingDirectory( this, tr("Select"),
                                              tmpBasePath,
                                              QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
            if(!tmpNewPath.endsWith("/"))
                tmpNewPath.append("/");
            ui->edit_322_cur->setText(tmpNewPath);
            break;
        }
        case 2:  //>@btn_322_test

            break;
        default:
            break;
    }
}

void DeviceManager::slot_LogoOperate(int id)
{
    switch(id)
    {
        case 0:  //>@btn_323_import
        {
            QString tmpOldPath;// = ui->edit_323_cur->text();
            QString tmpBasePath = QString("/");
            if(!tmpOldPath.isEmpty())
                tmpBasePath = getParentDirectory(tmpOldPath);
            QString tmpNewPath = QFileDialog::getExistingDirectory( this, tr("Select"),
                                              tmpBasePath,
                                              QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
            if(!tmpNewPath.endsWith("/"))
                tmpNewPath.append("/");
            //ui->edit_323_cur->setText(tmpNewPath);
            break;
        }
        case 2:  //>@btn_323_test

            break;
        default:
            break;
    }
}

void DeviceManager::slot_BackgroundOperate(int id)
{
    switch(id)
    {
        case 0:  //>@btn_324_import
        {
            QString tmpOldPath = ui->edit_324_cur->text();
            QString tmpBasePath = QString("/");
            if(!tmpOldPath.isEmpty())
                tmpBasePath = getParentDirectory(tmpOldPath);
            QString tmpNewPath = QFileDialog::getExistingDirectory( this, tr("Select"),
                                              tmpBasePath,
                                              QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
            if(!tmpNewPath.endsWith("/"))
                tmpNewPath.append("/");
            ui->edit_324_cur->setText(tmpNewPath);
            break;
        }
        case 2:  //>@btn_324_test

            break;
        default:
            break;
    }
}

void DeviceManager::slot_LanguageOperate(int id)
{
    switch(id)
    {
        case 0:  //>@btn_33_import
            slot_GotoStep(3301);
            break;
        case 1:  //>@发声
        {
            QListWidgetItem *tmpItem = ui->list_33_audio->currentItem();
            if(tmpItem)
            {
                QString tmpFile = tmpItem->data(D_LISTITEM_PATH).toString() + tmpItem->data(D_LISTITEM_NAME).toString();
                m_MediaPlayer->Add(AO_BEEP, tmpFile, false, false, 100);
            }
            break;
        }
        case 5:  //>@import
        {
            QString tmpNewLangPath = ui->edit_33_1_path->text();
            QString tmpLangName = ui->edit_33_1_name->text();
            if(tmpLangName.isEmpty())
            {
                MsgBox tmpMsgBox(tr("The name is empty,\n"
                                    "Please input it firstly!"));
                tmpMsgBox.exec();
                return;
            }
            if(tmpNewLangPath.isEmpty())
            {
                MsgBox tmpMsgBox(tr("The path is empty,\n"
                                    "Please input it firstly!"));
                tmpMsgBox.exec();
                return;
            }

            QString tmpLangPath = GlobalPara.m_SoftwareDir + D_COM_DIR + QString("Audio/");
            QDir tmpDir(tmpLangPath);
            QStringList folders = tmpDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

            if(folders.contains(tmpLangName, Qt::CaseInsensitive))
            {
                MsgBox tmpMsgBox(tr("The Language has already exist,\n"
                                    "Please recreate!"));
                tmpMsgBox.exec();
                return;
            }

            ShowBusy(true, 0xffffffff);

            tmpNewLangPath.replace("\\", "/");
            QString tmpNewPath;
            //>@如果是zip文件，需要先解压
            if(tmpNewLangPath.endsWith(".zip", Qt::CaseInsensitive))
            {
                int index = tmpNewLangPath.indexOf(".zip");
                tmpNewPath = tmpNewLangPath.mid(0, index);
                UnZip(tmpNewLangPath, tmpNewPath);
            }
            else
            {
                tmpNewPath = tmpNewLangPath;
            }

            CopyFolder(tmpNewPath, tmpLangPath + tmpLangName, true, true, true);
            ShowBusy(false);

            //>@增加一个Language
            AddListItem(ui->list_33_language, GlobalPara.m_SoftwareDir + D_COM_DIR + QString("Audio/"), tmpLangName);

            slot_GotoStep(33);
            break;
        }
        case 6:  //btn_33_2_next
        {
            QString tmpName = ui->edit_33_2_name->text();
            if(tmpName.isEmpty())
            {
                MsgBox tmpMsgBox(tr("The name is empty!"));
                tmpMsgBox.exec();
                return;
            }

            QString tmpPath = GlobalPara.m_SoftwareDir + D_COM_DIR + QString("Audio/");
            CreatPath(tmpPath + tmpName);
            QListWidgetItem* tmpItem = AddListItem(ui->list_33_language, tmpPath, tmpName);
            if(tmpItem)
                ui->list_32_style->setCurrentItem(tmpItem);
            slot_GotoStep(33);
            break;
        }
        case 7:  //>@addfile
        {
            QString tmpFile = QFileDialog::getOpenFileName(this, tr("Select"), "/", QString("Audio (*.wav)"));
            if(tmpFile.isEmpty())
                return;
            QString tmpLangPath = mUiManager->m_FileDir + QString("Media/Resource/Audio/");
            QString tmpFileName = getFileName(tmpFile);

            int tmpKey = GetKey(tmpFileName, "M");

            CopyFile(tmpFile, tmpLangPath+tmpFileName);

            //>@同步Theme.ui文件
            QDomElement tmpMediaElement = mUiManager->m_ComElementGroup.value(COM_MEDIA);
            if(!tmpMediaElement.isNull())
            {
                QDomElement tmpSMElement = tmpMediaElement.firstChildElement("StateMachine");
                if(!tmpSMElement.isNull())
                {
                    QDomElement tmpResElement = tmpSMElement.firstChildElement("Resource");
                    if(!tmpResElement.isNull())
                    {
                        QDomElement tmpRcElement = tmpResElement.firstChildElement(QString("RC%1").arg(tmpKey));
                        if(!tmpRcElement.isNull())
                        {
                            mUiManager->ModifyItemElement(tmpRcElement, QString("Media/Resource/Audio/%1").arg(tmpFileName));
                        }
                        else
                        {
                            mUiManager->CreateItemElement(tmpResElement, QString("RC%1").arg(tmpKey), QString("Media/Resource/Audio/%1").arg(tmpFileName));
                        }
                    }
                }
            }
            mUiManager->SaveUI();

            QListWidgetItem* tmpItem = AddListItem(ui->list_33_audio, tmpLangPath, tmpFileName);
            if(tmpItem)
                ui->list_33_audio->setCurrentItem(tmpItem);
            break;
        }
        case 8:  //>@delfile
        {
            QListWidgetItem *tmpItem = ui->list_33_audio->currentItem();
            if(tmpItem)
            {
                MsgBox tmpMsgBox(tr("Do you want to delete?"), true, LEVEL_QUESTION);
                int ret = tmpMsgBox.exec();
                if(ret == QDialog::Accepted)
                {
                    int tmpKey = GetKey(tmpItem->text(), "M");

                    QString tmpFile = tmpItem->data(D_LISTITEM_PATH).toString() + tmpItem->data(D_LISTITEM_NAME).toString();
                    DelFile(tmpFile);
                    DelListItem(ui->list_33_audio, tmpItem);
/*
                    //>@同步Theme.ui文件
                    QDomElement tmpMediaElement = mUiManager->m_ComElementGroup.value(COM_MEDIA);
                    if(!tmpMediaElement.isNull())
                    {
                        QDomElement tmpSMElement = tmpMediaElement.firstChildElement("StateMachine");
                        if(!tmpSMElement.isNull())
                        {
                            QDomElement tmpResElement = tmpSMElement.firstChildElement("Resource");
                            if(!tmpResElement.isNull())
                            {
                                mUiManager->DeleteItemElement(tmpResElement.firstChildElement(QString("RC%1").arg(tmpKey)));
                            }
                        }
                    }
                    mUiManager->SaveUI();
*/
                }
            }
            break;
        }
        case 9:  //>@new
        {
            slot_GotoStep(3302);
            break;
        }
        case 10:
        {
            if(!QtWait(m_LangListOperate, 0, 5000))
            {
                MsgBox tmpMsgBox(tr("The language list is busy now,\nPlease redo!"));
                tmpMsgBox.exec();
                return;
            }
            m_LangListOperate = 2;

            QListWidgetItem *tmpItem = ui->list_33_language->currentItem();
            if(tmpItem)
            {
                MsgBox tmpMsgBox(tr("Do you want to delete?"), true, LEVEL_QUESTION);
                int ret = tmpMsgBox.exec();
                if(ret == QDialog::Accepted)
                {
                    QString tmpLangPath = GlobalPara.m_SoftwareDir + D_COM_DIR + QString("Audio/%1/").arg(tmpItem->text());
                    ShowBusy(true, 0xffffffff);
                    DelFolder(tmpLangPath, true);
                    DelListItem(ui->list_33_language, tmpItem);
                    ShowBusy(false);

                    ui->list_33_language->clearFocus();
                    QString tmpName = GetLanguage();
                    QList<QListWidgetItem*> tmpList = ui->list_33_language->findItems(tmpName, Qt::MatchExactly);
                    for(int i=0;i<tmpList.count();i++)
                    {
                        QListWidgetItem *tmpItem = tmpList.at(i);
                        if(tmpItem)
                        {
                            ui->list_33_language->setCurrentItem(tmpItem);
                            break;
                        }
                    }
                }
            }

            m_LangListOperate = 0;
            break;
        }
        default:
            break;
    }
}

QString DeviceManager::GetLanguage()
{
    QDomElement tmpElement = mUiManager->m_ComElementGroup.value(COM_MEDIA);
    if(tmpElement.isNull())
        return QString();
    QDomElement tmpSubElement = tmpElement.firstChildElement("StateMachine");
    if(tmpSubElement.isNull())
        return QString();
    QDomElement tmpResElement = tmpSubElement.firstChildElement("Resource");
    if(tmpResElement.isNull())
        return QString();
    return tmpResElement.attribute("Language");
}

bool DeviceManager::SetLanguage(QString pLanguage)
{
    QDomElement tmpElement = mUiManager->m_ComElementGroup.value(COM_MEDIA);
    if(tmpElement.isNull())
        return false;
    QDomElement tmpSubElement = tmpElement.firstChildElement("StateMachine");
    if(tmpSubElement.isNull())
        return false;
    QDomElement tmpResElement = tmpElement.firstChildElement("Resource");
    if(tmpResElement.isNull())
        return false;
    QString tmpStr = "Language";
    tmpResElement.setAttribute(tmpStr, pLanguage);
    return true;
}

void DeviceManager::on_list_33_language_itemDoubleClicked(QListWidgetItem *item)
//void DeviceManager::on_list_33_language_itemClicked(QListWidgetItem *item)
{
    ui->list_33_audio->clear();
    QString tmpPath = item->data(D_LISTITEM_PATH).toString();
    QString tmpName = item->data(D_LISTITEM_NAME).toString();

    ShowBusy(true, 0xffffffff);
    QString tmpAudioPath = tmpPath+tmpName+QString("/");
    QString tmpDstPath = mBufferPath + D_THEMEDIR + QString("Sodimas/Media/Resource/Audio/");
    ClearFolder(tmpDstPath);
    CopyFolder(tmpAudioPath, tmpDstPath, true, true, true);

    SetLanguage(tmpName);

    QDir tmpDir(tmpDstPath);
    QStringList files = tmpDir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    for(int i=0;i<files.count();i++)
    {
        QString tmpAudioName = files.at(i);
        AddListItem(ui->list_33_audio, tmpDstPath, tmpAudioName);
    }
    ShowBusy(false);
}

void DeviceManager::slot_SelectPath(int id)
{
    QLineEdit *tmpNameEdit = 0;
    QLineEdit *tmpPathEdit = 0;
    if(id == 203)
    {
        tmpNameEdit = ui->edit_2_3_name;
        tmpPathEdit = ui->edit_2_3_path;
    }
    else if(id == 3201)
    {
        tmpNameEdit = ui->edit_32_1_name;
        tmpPathEdit = ui->edit_32_1_path;
    }
    else if(id == 3202)
    {
        tmpNameEdit = ui->edit_32_1_name;
        tmpPathEdit = ui->edit_32_1_path_pro;
    }
    else if(id == 3301)
    {
        tmpNameEdit = ui->edit_33_1_name;
        tmpPathEdit = ui->edit_33_1_path;
    }
    else
        return;
    QString tmpOldPath = tmpPathEdit->text();
    QString tmpNewPath = QFileDialog::getExistingDirectory( this, tr("Select"),
                                      tmpOldPath,
                                      QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    if(tmpNewPath.isEmpty())
        return;
    if(!tmpNewPath.endsWith("/"))
        tmpNewPath.append("/");
    tmpPathEdit->setText(tmpNewPath);
    if(tmpNameEdit->text().isEmpty())
    {
        tmpNameEdit->setText(GetDirectoryName(tmpNewPath));
    }
}

void DeviceManager::on_list_321_floor_itemDoubleClicked(QListWidgetItem *item)
{
    QString tmpPath = item->data(D_LISTITEM_PATH).toString();
    QString tmpName = item->data(D_LISTITEM_NAME).toString();
    ui->edit_321_cur->setText(tmpPath+tmpName+QString("/"));
}

void DeviceManager::on_list_322_arrows_itemDoubleClicked(QListWidgetItem *item)
{
    QString tmpPath = item->data(D_LISTITEM_PATH).toString();
    QString tmpName = item->data(D_LISTITEM_NAME).toString();
    ui->edit_322_cur->setText(tmpPath+tmpName+QString("/"));
}


void DeviceManager::on_list_324_background_itemDoubleClicked(QListWidgetItem *item)
{
    QString tmpPath = item->data(D_LISTITEM_PATH).toString();
    QString tmpName = item->data(D_LISTITEM_NAME).toString();
    ui->edit_324_cur->setText(tmpPath+tmpName+QString("/"));
}

void DeviceManager::on_btn_344_select_clicked()
{
    if(mBufferPath.isEmpty())
        return;

    static QString sCurrentPath;
    QString tmpFile = QFileDialog::getOpenFileName(this, tr("Select File"),
                                         sCurrentPath,
                                         QString("*.png"));
    if (tmpFile.isEmpty())
        return;
    sCurrentPath = getFileDirectory(tmpFile);

    QString tmpDstPath = mBufferPath+D_BOOTDIR+QString("logo.png");
    CopyFile(tmpFile, tmpDstPath);

    //>@加载当前设备的LOGO图片
    QPixmap tmpPixmap = QPixmap(tmpDstPath);
    if(!tmpPixmap.isNull())
    {
        QSize pReferSize = tmpPixmap.size();
        QRectF tmpResRect;
        QRectF tmpWidRect = QRectF(ui->wid_logo->rect().adjusted(10, 10, -10, -10));
        qreal tmpLength = qMin(tmpWidRect.width(), tmpWidRect.height());
        QPointF tmpCenter = QPointF(tmpWidRect.center());
        if(pReferSize.width() > pReferSize.height())
        {
            qreal tmpRatio = tmpLength / (qreal)(pReferSize.width());
            qreal tmpHeight = (qreal)(pReferSize.height()) * tmpRatio;
            //>@以宽度为准
            tmpResRect = QRectF(tmpCenter.x()-tmpLength/2.0, tmpCenter.y()-tmpHeight/2.0, tmpLength, tmpHeight);
        }
        else
        {
            qreal tmpRatio = tmpLength / (qreal)(pReferSize.height());
            qreal tmpWidth = (qreal)(pReferSize.width()) * tmpRatio;
            //>@以高度为准
            tmpResRect = QRectF(tmpCenter.x()-tmpWidth/2.0, tmpCenter.y()-tmpLength/2.0, tmpWidth, tmpLength);
        }
        ui->label_344_logo->setGeometry(tmpResRect.toRect());
        ui->label_344_logo->setPixmap(tmpPixmap);
        ui->label_344_logo->setToolTip(tmpDstPath);
    }
}

void DeviceManager::on_list_32_style_itemDoubleClicked(QListWidgetItem *tmpItem)
{
    if(!tmpItem)
    {
        MsgBox tmpMsgBox(tr("You did't select style!"));
        tmpMsgBox.exec();
        return;
    }
    ShowBusy(true, 0xffffffff);
    QString tmpStylePath = tmpItem->data(D_LISTITEM_PATH).toString();
    QString tmpStyleName = tmpItem->data(D_LISTITEM_NAME).toString();
    QString tmpSrcPath = tmpStylePath + QString("Arrow/%1/").arg(tmpStyleName);
    QString tmpDstPath = mBufferPath + D_THEMEDIR + QString("Sodimas/Arrow/Resource/Image/");
    CopyFolder(tmpSrcPath, tmpDstPath, true, true, true);

    tmpSrcPath = tmpStylePath + QString("Floor/%1/").arg(tmpStyleName);
    tmpDstPath = mBufferPath + D_THEMEDIR + QString("Sodimas/Floor/Resource/Image/");
    CopyFolder(tmpSrcPath, tmpDstPath, true, true, true);

    tmpSrcPath = tmpStylePath + QString("Function/%1/").arg(tmpStyleName);
    tmpDstPath = mBufferPath + D_THEMEDIR + QString("Sodimas/Function/Resource/Image/");
    CopyFolder(tmpSrcPath, tmpDstPath, true, true, true);

    tmpSrcPath = tmpStylePath + QString("Desktop/%1/").arg(tmpStyleName);
    tmpDstPath = mBufferPath + D_THEMEDIR + QString("Sodimas/Desktop/Resource/Image/");
    CopyFolder(tmpSrcPath, tmpDstPath, true, true, true);

    InitLogoList();

    ShowBusy(false);

    UpdateDevImage(mRotation, mThemeLayout, true);
}

void DeviceManager::on_btn_test_clicked(bool checked)
{
    if(checked)
    {
        mAutoTestTimer.start();
    }
    else
    {
        mAutoTestTimer.stop();
    }
}

void DeviceManager::on_btn_32_1_selectzip_clicked()
{
    QString tmpOldPath = getParentDirectory(ui->edit_32_1_path->text());
    QString tmpNewPath = QFileDialog::getOpenFileName(this, tr("Select"),
                                      tmpOldPath);
    if(!tmpNewPath.endsWith(".zip", Qt::CaseInsensitive))
    {
        MsgBox tmpMsgBox(tr("Please select zip file!"));
        tmpMsgBox.exec();
        return;
    }
    if(ui->edit_32_1_name->text().isEmpty())
        ui->edit_32_1_name->setText(QFileInfo(tmpNewPath).baseName());
    ui->edit_32_1_path->setText(tmpNewPath);
}

void DeviceManager::on_btn_33_1_selectzip_clicked()
{
    QString tmpOldPath = getParentDirectory(ui->edit_33_1_path->text());
    QString tmpNewPath = QFileDialog::getOpenFileName(this, tr("Select"),
                                      tmpOldPath);
    if(!tmpNewPath.endsWith(".zip", Qt::CaseInsensitive))
    {
        MsgBox tmpMsgBox(tr("Please select zip file!"));
        tmpMsgBox.exec();
        return;
    }
    if(ui->edit_33_1_name->text().isEmpty())
        ui->edit_33_1_name->setText(QFileInfo(tmpNewPath).baseName());
    ui->edit_33_1_path->setText(tmpNewPath);
}

void DeviceManager::on_check_all_clicked(bool checked)
{
    if(checked)
    {
        ui->check_application->setChecked(false);
        ui->check_conffile->setChecked(false);
        ui->check_sysfile->setChecked(false);
        ui->check_style->setChecked(false);
        ui->check_audio->setChecked(false);
    }
}



void DeviceManager::on_check_application_clicked(bool checked)
{
    ui->check_all->setChecked(false);
}

void DeviceManager::on_check_conffile_clicked(bool checked)
{
    ui->check_all->setChecked(false);
}

void DeviceManager::on_check_sysfile_clicked(bool checked)
{
    ui->check_all->setChecked(false);
}

void DeviceManager::on_check_style_clicked(bool checked)
{
    ui->check_all->setChecked(false);
}

void DeviceManager::on_check_audio_clicked(bool checked)
{
    ui->check_all->setChecked(false);
}

void DeviceManager::on_check_backlight_enable_clicked(bool checked)
{
    ui->slider_delay->setEnabled(checked);
    ui->slider_standby->setEnabled(checked);
    ui->label_backlight_enable->setText(QString("Standby %1").arg(checked?QString("Enable"):QString("Disable")));
    QString tmpCmd;
    if(checked)
    {
        tmpCmd = QString("<Physical><LCD><StandBy>%1</StandBy></LCD></Physical>")
                         .arg(ui->slider_delay->value());
    }
    else
    {
        tmpCmd = QString("<Physical><LCD><StandBy>0</StandBy></LCD></Physical>");
    }
    ProcessCommand(0, tmpCmd);
}

void DeviceManager::on_check_volume_enable_clicked(bool checked)
{
    ui->slider_message_volume->setEnabled(checked);
    ui->slider_gong_volume->setEnabled(checked);
    ui->slider_bgmusic_volume->setEnabled(checked);
    ui->label_volume_enable->setText(QString("Volume %1").arg(checked?QString("Enable"):QString("Disable")));
    QString tmpCmd;
    if(checked)
    {
        tmpCmd = QString("<Physical><Player><LiftFlrVol>%1</LiftFlrVol><LiftArvVol>%2</LiftArvVol><AdMusicVol>%3</AdMusicVol></Player></Physical>")
                         .arg(ui->slider_message_volume->value())
                         .arg(ui->slider_gong_volume->value())
                         .arg(ui->slider_bgmusic_volume->value());
    }
    else
    {
        tmpCmd = QString("<Physical><Player><LiftFlrVol>0</LiftFlrVol><LiftArvVol>0</LiftArvVol><AdMusicVol>0</AdMusicVol></Player></Physical>");
    }
    ProcessCommand(0, tmpCmd);
}

void DeviceManager::on_check_openmsg_clicked(bool checked)
{
    QDomElement tmpElement;
    if(ui->radio_saf->isChecked())
    {
        tmpElement = mMdpManager->m_PHElementGroup.value(P_SAF);
    }
    else
    {
        tmpElement = mMdpManager->m_PHElementGroup.value(P_COM);
    }
    if(tmpElement.isNull())
        return;
    QDomElement tmpSubElement = tmpElement.firstChildElement("OpenMsg");
    if(tmpSubElement.isNull())
        return;
    if(checked)
        mMdpManager->ModifyItemElement(tmpSubElement, "true");
    else
        mMdpManager->ModifyItemElement(tmpSubElement, "false");

    //mMdpManager->SaveMDP();
}

void DeviceManager::on_combo_arrowspeed_currentIndexChanged(int index)
{
    //>@6s一圈6000  2圈3000  3圈2000  4圈1500
    QString tmpCmd =  QString("<Theme><Arrow><StateMachine><Run EffectPara=\"Interval:%1\"/></StateMachine></Arrow></Theme>").arg(index);
    ProcessCommand(0, tmpCmd);
}
