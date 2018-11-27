#ifndef ICMANAGER_H
#define ICMANAGER_H

#include <QWidget>
#include <QButtonGroup>
#include "sqlparser.h"
#include "icreader.h"
#include "carditem.h"
#include "icdata.h"

#include "managerdialog.h"

namespace Ui {
class ICManager;
}

typedef enum{
    E_PAGE_USER = 0,
    E_PAGE_ADMIN,
    E_PAGE_CARDLIST,
    E_PAGE_CARDLOG,
    E_PAGE_MAKEOWERCARD,
    E_PAGE_MAKEADMINCARD,
    E_PAGE_MAKETOOLCARD,
    E_PAGE_LOSSCARD,
    E_PAGE_REMAKECARD,
    E_PAGE_CELLINFO,
    E_PAGE_ELEVATOR
} UD_PAGE_TYPE;



class ICManager : public QWidget
{
    Q_OBJECT

public:
    explicit ICManager(QWidget *parent = 0);
    ~ICManager();

    void InitFromDB();

    void GotoPage(int pIndex, bool pNew = true);

    void ShowMsgPrg(QString pMsg, int pPrg);
    void SetPasswd(QString pText);

    void AddUserToDB(QString pName, QString pUnitNum, QString pLayerNum, QString pFamilyNum, QString pPhone, QString pDetail);
    void DelUserFromDB(QString pName);
    void AddUser(QString pName,QString pAddress,QString pPhone,int pCardCount,QString pInfo);

    void AddAdmin(QString pName,QString pAddress,QString pPhone,int pCardCount,QString pInfo);
    void AddAdminToDB(QString pName,QString pPhone,QString pAddress,int pPower,QString pTopUp,QString pPassword);
    void DelAdminFromDB(QString pName);

    void AddCardlist(QString pCardNum,QString pType,QString pName,QString pPermission,QString pState,QString pMaker);
    void DelCardFromDB(QString CardNum, int index);
    void AddCardLog(QString pType,QString pCardNum,QString pName,QDateTime pTime,QString pMode);
    void AddCellInfo(QString pUnitsNum,QString pLayersNum,QString pFamilysNum);
    void AddElevator(QString pElevNum,QString pUnitNum,QString pCtrLayerCnt);
    void AddMakeCardToDB(QString pName,QString pCardNum,QString pPassword);
    void AddValidFloorsToDB(int index);
    void AddToolCardToDB(QString pName,QString pCardNum,QString pPassword);
    void AddCellInfoToDB(QString pUnit,QString pLayer,QString pFamily);
    void AddElevatorToDB(QString pElevNum,QString pUnitNum,QString pCtrlLayerNum);
    void AddLostCardToDB(QString pCardNum, QString pPassword);

    void AddMakeAdminCardToDB(QString pName,QString pCardNum,QString pPassword);

    void AddMapFloorToDB(QString pUnit,QString pActualLayer,QString pMapLayer,QString pRoomList);
    void AddMapTable(QString pLayersNum,QString pLayersMap,QString pFamilyNum);

    void Init_user_table();
    void Init_admin_table();
    void Init_cardlist_table();
    void Init_cardlog_table();
    void Init_cellInfo_table();
    void Init_elevator_table();
    void Init_map_table();

    void SetComboxText(QComboBox *pBox, QString pText);

    QByteArray GetCorrespdFlrs(int pNum);
    void SetCorrespdFlrs(int pNum, char *pBuffer);

    void InitCorrespdElevt(int pUnitNum);

    QByteArray GetOpenFlrs();
    QByteArray GetUserOpenFlrs();

public slots:
    void slot_Ticks();

    void slot_MainMenuClicked(int index);
    //void slot_UsrOperateClicked(int index);
    //void slot_AdminOperateClicked(int index);
    void slot_MaketoolcardClicked(int index);

    void slot_AutoHideMsgPrg();

    void slot_CardRead();
    void slot_MakeUsrCard();
    void slot_DestoryUsrCard();

    void slot_MakeAdminCard();
    void slot_MadeCardsNum();
    void slot_DestoryAdminCard();
    void slot_SetAdminPassword();

    void slot_CardLog();
    void slot_ReDoCard();
    void slot_CardRepair();
    void slot_CardLoss();
    void slot_CardLogRemove();

    void slot_TableUser_Click(int pRow, int pColumn);
    void slot_TableAdmin_Click(int pRow, int pColumn);
   //void slot_TableCardList_Click(int pRow, int pColumn);
    void slot_TableCellInfo_click(int pRow,int pColumn);
    void slot_TableElevator_click(int pRow,int pColumn);

public:
    QList<int>          mPageList;
    int                 mReadCardFlag = 0;
    int                 mLossCardFlag = 0;
    int                 index = 0;
    bool                resetPassword;
    QString             mPasswd;
    QString             mAdminName;

    QTimer              mAutoHideMsgPrg;

    QButtonGroup        mMainMenuBtns;
    //QButtonGroup        mUsrOperateBtns;
    //QButtonGroup        mAdminOperateBtns;
    QButtonGroup        mCardlistOperateBtns;
    QButtonGroup        mMaketoolcardBtns;

    SQL_PARSER         *mSqlParser;

    IcReader            mIcReader;
    UsrCard_OpenFloor   usrOpenFloors;

    QList<QString>      mUserList;
    QList<QString>      mAdminList;
    QList<QString>      mCardList;
    QList<QString>      mCellUnitList;
    QStringList        mElevatorList;
    QList<QString>      mLayersList;

    QTimer              mTickTimer;

    QHash<int, QCheckBox*>  mValidFlrs[8];
    QHash<QString, QCheckBox*>  mUnitElevt;
    QHash<QString, QCheckBox*>  mOpenFlrs;
    QHash<QString, QCheckBox*>  mUserUnitElevt;
    QHash<QString, QCheckBox*>  mUserOpenFlrs;

    //FlrMap
public:
    void InitFlrMap(QString pUnitNum);
    QString GetFlrMap(QString pFlrNum);
    QString GetFlrNum(QString pFlrMap);

    QString                 mFlrMapUnitNum;
    QHash<QString, QString> mFlrMaps;

private slots:
    void on_btn_user_edit_ok_clicked();

    void on_btn_admin_ok_clicked();

    void on_btn_makecard_ok_clicked();

    void on_btn_cellinfo_ok_clicked();

    void on_btn_elevator_ok_clicked();

    void on_btn_cardlist_search_clicked();

    void on_btn_user_search_clicked();

    void on_btn_admin_search_clicked();

    void on_comboBox_toolcard_units_currentIndexChanged(const QString &arg1);

    void on_btn_toolcard_ok_clicked();


    void on_btn_maskelevt_clicked();

    void on_btn_enablelevt_clicked();

    void on_btn_maskfloor_clicked();

    void on_btn_enablefloor_clicked();

    void on_btn_owner_list_clicked();

    void on_btn_back_clicked();

    void on_combox_user_unit_num_currentIndexChanged(const QString &arg1);

    void on_combox_user_layer_num_currentIndexChanged(const QString &arg1);

    void on_btn_user_recharge_clicked();

    void on_btn_admin_list_clicked();

    void on_btn_manager_clicked();

    void on_btn_quit_clicked();

    void on_btn_makeadmincard_ok_clicked();

    void on_btn_readcard_selectowner_clicked();


    void on_table_user_cellDoubleClicked(int row, int column);


    void on_combo_cardfee_type_currentTextChanged(const QString &arg1);

    void on_checkBox_available_time_clicked(bool checked);

    void on_table_admin_cellDoubleClicked(int row, int column);

    void on_checkBox_admin_cardValidtime_clicked(bool checked);

    void on_checkBox_admin_availabletime_clicked(bool checked);

    void on_checkBox_time1_clicked(bool checked);

    void on_checkBox_time2_clicked(bool checked);

    void on_checkBox_time3_clicked(bool checked);

    void on_combo_unitnum_currentIndexChanged(const QString &arg1);

    void on_table_map_cellClicked(int row, int column);

    void on_btn_elevt_ok_clicked();

    void on_btn_addliftmap_clicked();
    
    void on_btn_delliftmap_clicked();

    void on_btn_cell_del_clicked();

    void on_btn_dellift_clicked();

    void on_btn_elector_list_clicked();

    void on_btn_cardlist_ok_4_clicked();

    void on_btn_card_list_clicked();

    void on_btn_cardlist_cancel_4_clicked();

    void on_table_elevator_cellDoubleClicked(int row, int column);

    void on_table_cardlist_cellDoubleClicked(int row, int column);

    void on_btn_owner_replacecard_clicked();

    void on_btn_tools_replacecard_clicked();


    void on_btn_GetLostCardNum_clicked();

    void on_btn_saveFloor_clicked();

    void on_combox_floor_num_currentIndexChanged(const QString &arg1);


private:
    Ui::ICManager *ui;
};
#endif // ICMANAGER_H
