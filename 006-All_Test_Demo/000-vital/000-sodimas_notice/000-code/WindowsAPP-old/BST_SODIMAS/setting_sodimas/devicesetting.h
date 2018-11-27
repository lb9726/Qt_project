#ifndef DEVICEMANAGE_H
#define DEVICEMANAGE_H

#include "../BST_IDE/setting/devicesetbase.h"
#include "../BST_IDE/setting/flrmapbar.h"
#include "zipfun.h"
#include "logiccontentmanager.h"
#include "logicthememanager.h"
#include "physicalplayer.h"
#include "physicallcd.h"

namespace Ui {
class DeviceManageForm;
}

//#define D_JIN_TEST

/*
    APP
        DEVICE
            OFFLINE     //>@离线设备所在目录
                Device.mdp
                Theme.ui
            N3YYXXX1403040001
            N3YYXXX1403040002
        THEME
            Layout0
            Layout1
        COM
            Floor
            Arrow
*/

class DeviceManager : public DeviceSetBase
{
    Q_OBJECT
public:
    explicit DeviceManager(QWidget *parent = 0);
    ~DeviceManager();

    void GetActiveStyle(int &pDtpStyle, int &pFlrStyle, int &pArwStyle, int &pFunStyle, int &pAudStyle, int &pMusicStyle);

    void SaveFlrMap();
    void InitFlrMap();
    void ActiveFlrmapbar(int row);
    FlrmapBar *AddFlrmapbar(QString tmpLevel, QString tmpMapping, QString tmpAudSrc, QString tmpAudio);
    void DelFlrmapbar(int row);
    void ClrFlrmapbar();

    void UpdateDevImage(int pRotate, int pLayout, bool pShowFloor);
    void UpdateDevImage(FLOOR_INFO pFloor, int pArrow, int pFunction, int pAudio);

    FLOOR_INFO GetFloor();
    void UpdateFlrMap();
    int GetAudio();

    bool SetWIFISSID(QString pSSID);

    void Zip(QString pSrc, QString pDst);
    void UnZip(QString pSrc, QString pDst);

    void InitLogoList();

    QString GetLanguage();
    bool SetLanguage(QString pLanguage);

signals:
    void sZip(QString pSrc, QString pDir, bool pCover, bool pIngore);
    void sUnzip(QString pSrc, QString pDir, bool pCover, bool pIngore);

public slots:
    void slot_Initialize();
    //>@
    void slot_InitDevice(QString pPath);
    void slot_InitTheme(QString pPath);

    void slot_Connect();
    void slot_Disconnect();

    void slot_InitFromLocal(QString pPath, bool pNew=true);
    void slot_InitFromNetwork();

    void slot_GotoStep(int pStep);
    void slot_NextStep(int id);
    void slot_BackStep(int id);

    void slot_Enter();
    void slot_Exit();

    void slot_FlrmapbarActive();
    void slot_FlrmapbarDel();

    void slot_ScreenSizeOperate(int id);
    void slot_ScreenOrientationOperate(int id);

    void slot_ProtocolOperate(int id);
    void slot_SimpleProOperate(int id);

    void slot_SelectLanguage(int id);
    void slot_ProjectOperate(int id);
    void slot_ProModify(int id);
    void slot_LayoutOperate(int id);
    void slot_StyleOperate(int id);

    void slot_FloorOperate(int id);
    void slot_ArrowOperate(int id);
    void slot_LogoOperate(int id);
    void slot_BackgroundOperate(int id);
    void slot_WIFIOperate(int id);
    void slot_LanguageOperate(int id);
    void slot_SettingPage(int id);

    void slot_SelectPath(int id);

    void slot_ZipTaskEnd();
    void slot_ZipTaskErr();
    void slot_ZipTaskProgress(int ratio);

    void slot_AutoTest();
    void slot_TimeTick();

    void on_list_321_floor_itemDoubleClicked(QListWidgetItem *item);

    void on_list_322_arrows_itemDoubleClicked(QListWidgetItem *item);

    void on_list_324_background_itemDoubleClicked(QListWidgetItem *item);

    void on_btn_32_1_import_clicked();

    void on_btn_344_select_clicked();

    void on_list_32_style_itemDoubleClicked(QListWidgetItem *tmpItem);

    void on_btn_test_clicked(bool checked);

    void on_btn_32_1_selectzip_clicked();

    //void on_list_33_language_itemClicked(QListWidgetItem *item);

    void on_btn_33_1_selectzip_clicked();

    void on_check_all_clicked(bool checked);

    void on_check_application_clicked(bool checked);

    void on_check_conffile_clicked(bool checked);

    void on_check_sysfile_clicked(bool checked);

    void on_check_style_clicked(bool checked);

    void on_check_audio_clicked(bool checked);

    void on_list_323_logo_itemClicked(QListWidgetItem *item);

    void on_edit_323_text1_editingFinished();

    void on_edit_323_text2_editingFinished();

    void on_check_backlight_enable_clicked(bool checked);

    void on_check_volume_enable_clicked(bool checked);

    void on_check_openmsg_clicked(bool checked);

    void on_combo_arrowspeed_currentIndexChanged(int index);

    void on_list_33_language_itemDoubleClicked(QListWidgetItem *item);

public:
    QButtonGroup        mLanguageGroup;
    QButtonGroup        mProjectGroup;
    QButtonGroup        mProModifyGroup;
    QButtonGroup        mLayoutModifyGroup;
    QButtonGroup        mStyleModifyGroup;

    QButtonGroup        mScreenSizeGroup;
    QButtonGroup        mScreenOrientationGroup;

    QButtonGroup        mProtocolGroup;
    QButtonGroup        mSimpleProGroup;

    QButtonGroup        mFloorOperateGroup;
    QButtonGroup        mArrowOperateGroup;
    QButtonGroup        mLogoOperateGroup;
    QButtonGroup        mBackgroundOperateGroup;

    QButtonGroup        mLanguageOperateGroup;
    QButtonGroup        mWIFIOperateGroup;

    QButtonGroup        mSettingGroup;

    QButtonGroup        mSelectPathGroup;

    QButtonGroup        mNextBtnGroup;
    QButtonGroup        mRetBtnGroup;

    bool                m_Initialize;

    int                 mRotation;
    int                 mThemeLayout;

    QTimer                  m_BaseTimer;

    QTimer              mAutoTestTimer;
    int                 mAutoTestCount;
    QRect tmpFloorRectH, tmpFloorRectV,
          tmpArrowRectH, tmpArrowRectV,
          tmpFuncRectH, tmpFuncRectV,
          tmpTextRectH, tmpTextRectV;
    bool m_ShowFloor;

    int                 m_ArrowSpeed[4];

    QString             m_SelectPath;

    int                m_LangListOperate;  //>@表示正在操作语言列表

    int mFloor;
    FLOOR_INFO mFloorInfo;
    int mArrow;
    int mFunction;
    int mAudio;

    QHash<int, QDomElement> mFunctionTextHash;

    QHash<QString, QString> mFloorMapHash;
    QHash<QString, QString> mMediaMapHash;

    TaskState           m_ZipTaskState;

private slots:
    void on_btn_32_1_import_project_clicked();

private:
    Ui::DeviceManageForm *ui;
};

#endif // DEVICEMANAGE_H
