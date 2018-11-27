/********************************************************************************
** Form generated from reading UI file 'icmanager.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ICMANAGER_H
#define UI_ICMANAGER_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <flickcharm.h>

QT_BEGIN_NAMESPACE

class Ui_ICManager
{
public:
    QGridLayout *gridLayout;
    QWidget *top;
    QHBoxLayout *horizontalLayout_51;
    QWidget *widget_5;
    QVBoxLayout *verticalLayout_37;
    QHBoxLayout *horizontalLayout_50;
    QPushButton *btn_manager;
    QSpacerItem *horizontalSpacer;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_46;
    QWidget *widget_person;
    QHBoxLayout *horizontalLayout_21;
    QLabel *label_person;
    QWidget *widget_addr;
    QHBoxLayout *horizontalLayout_44;
    QLabel *label_addr;
    QPushButton *btn_quit;
    QPushButton *btn_back;
    ScrollArea *scrollArea;
    QWidget *Left;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout_38;
    QWidget *left;
    QPushButton *btn_maketoolcard;
    QPushButton *btn_cardlog;
    QPushButton *btn_losscard;
    QPushButton *btn_remakecard;
    QPushButton *btn_user;
    QPushButton *btn_cellinfo;
    QPushButton *btn_admin;
    QPushButton *btn_makeadmincard;
    QPushButton *btn_elevatorinfo;
    QPushButton *btn_makeownercard;
    QPushButton *btn_cardlist;
    QWidget *widget_left_bottom;
    QStackedWidget *stacked_pages;
    QWidget *page_user;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_6;
    QLineEdit *line_user_search;
    QPushButton *btn_user_search;
    QSplitter *splitter;
    QTableWidget *table_user;
    QWidget *widget;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_24;
    QLabel *label_tip;
    QSpacerItem *horizontalSpacer_5;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_9;
    QLineEdit *line_user_name;
    QLabel *label_10;
    QLineEdit *line_user_phone;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_12;
    QComboBox *combox_user_unit_num;
    QLabel *label_34;
    QComboBox *combox_user_layer_num;
    QLabel *label_35;
    QComboBox *combox_user_family_num;
    QLabel *label_36;
    QSpacerItem *horizontalSpacer_7;
    QVBoxLayout *verticalLayout_6;
    QWidget *widget_validElevt;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_7;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QSpacerItem *verticalSpacer_15;
    ScrollArea *scrollArea_2;
    QWidget *wid_valideles;
    QWidget *widget_validLayers;
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *horizontalLayout_35;
    QVBoxLayout *verticalLayout_29;
    QLabel *label;
    QSpacerItem *verticalSpacer_16;
    ScrollArea *scrollArea_3;
    QWidget *wid_validlayers;
    QWidget *wid_prgmsg;
    QHBoxLayout *horizontalLayout_67;
    QLabel *label_msg;
    QProgressBar *progress;
    QLabel *label_26;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *btn_user_edit_ok;
    QWidget *page_admin;
    QVBoxLayout *verticalLayout_12;
    QHBoxLayout *horizontalLayout_11;
    QLineEdit *line_admin_search;
    QPushButton *btn_admin_search;
    QSplitter *splitter_2;
    QTableWidget *table_admin;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_10;
    QHBoxLayout *horizontalLayout_15;
    QSpacerItem *horizontalSpacer_25;
    QLabel *label_tip_2;
    QSpacerItem *horizontalSpacer_10;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_admin_name;
    QLineEdit *line_admin_name;
    QLabel *label_admin_phone;
    QLineEdit *line_admin_phone;
    QLabel *label_41;
    QLineEdit *line_admin_password;
    QSpacerItem *horizontalSpacer_15;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_7;
    QLineEdit *line_admin_address;
    QSpacerItem *horizontalSpacer_9;
    QHBoxLayout *horizontalLayout_22;
    QVBoxLayout *verticalLayout_14;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_manager_permission;
    QGroupBox *groupBox_5;
    QHBoxLayout *horizontalLayout_19;
    QVBoxLayout *verticalLayout_9;
    QCheckBox *checkBox_adminPower_makeownercard;
    QCheckBox *checkBox_adminPower_makeadmincard;
    QSpacerItem *verticalSpacer_17;
    QHBoxLayout *horizontalLayout_18;
    QLabel *label_8;
    QComboBox *comboBox_topup;
    QLabel *label_18;
    QSpacerItem *horizontalSpacer_11;
    QSpacerItem *verticalSpacer_6;
    QVBoxLayout *verticalLayout_34;
    QCheckBox *checkBox_adminPower_makesyntimecard;
    QCheckBox *checkBox_adminPower_makereadcard;
    QCheckBox *checkBox_adminPower_makemaskroomcard;
    QCheckBox *checkBox_adminPower_makeopenfloorcard;
    QCheckBox *checkBox_adminPower_makelevatimeallow;
    QSpacerItem *horizontalSpacer_34;
    QHBoxLayout *horizontalLayout_17;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *btn_admin_ok;
    QWidget *page_cardlist;
    QVBoxLayout *verticalLayout_33;
    QHBoxLayout *horizontalLayout_23;
    QLineEdit *line_search_card;
    QPushButton *btn_cardlist_search;
    QTableWidget *table_cardlist;
    QWidget *page_cardlog;
    QVBoxLayout *verticalLayout_21;
    QHBoxLayout *horizontalLayout_28;
    QLineEdit *lineEdit_7;
    QPushButton *btn_cardlog_search;
    QWidget *widget_datetime;
    QHBoxLayout *horizontalLayout_8;
    QDateEdit *dateEdit_start;
    QTimeEdit *timeEdit_start;
    QLabel *label_19;
    QDateEdit *dateEdit_end;
    QTimeEdit *timeEdit_end;
    QSpacerItem *horizontalSpacer_16;
    QTableWidget *table_cardlog;
    QWidget *page_makeownercard;
    QVBoxLayout *verticalLayout_32;
    QWidget *widget_makeownercard;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_39;
    QHBoxLayout *horizontalLayout_38;
    QLabel *label_30;
    QLineEdit *line_business_name;
    QPushButton *btn_owner_list;
    QHBoxLayout *horizontalLayout_36;
    QLabel *label_37;
    QLineEdit *line_business_cardnum;
    QHBoxLayout *horizontalLayout_37;
    QLabel *label_38;
    QLineEdit *line_business_password;
    QSpacerItem *horizontalSpacer_20;
    QGroupBox *group_card_fee;
    QVBoxLayout *verticalLayout_22;
    QHBoxLayout *horizontalLayout_41;
    QLabel *label_20;
    QComboBox *combo_cardfee_type;
    QWidget *widget_chargestandard;
    QHBoxLayout *horizontalLayout_68;
    QLabel *label_21;
    QLineEdit *lineEdit_chargestandard;
    QLabel *label_22;
    QLabel *label_16;
    QLineEdit *lineEdit_freefee;
    QComboBox *combox_recharge_value;
    QPushButton *btn_user_recharge;
    QSpacerItem *horizontalSpacer_22;
    QWidget *widget_card_value;
    QVBoxLayout *verticalLayout_47;
    QHBoxLayout *horizontalLayout_27;
    QLabel *label_3;
    QLabel *label_43;
    QComboBox *combox_floor_num;
    QSpacerItem *horizontalSpacer_27;
    QPushButton *btn_saveFloor;
    QSpacerItem *horizontalSpacer_53;
    QWidget *widget_validFloors;
    QHBoxLayout *horizontalLayout_29;
    QHBoxLayout *horizontalLayout_13;
    QVBoxLayout *verticalLayout_11;
    QLabel *label_44;
    QSpacerItem *verticalSpacer_8;
    QScrollArea *scrollArea_4;
    QWidget *widget_validfloors;
    QGroupBox *group_card_time;
    QVBoxLayout *verticalLayout_18;
    QHBoxLayout *horizontalLayout_10;
    QComboBox *comboBox_response_time;
    QLabel *label_28;
    QSpacerItem *horizontalSpacer_35;
    QComboBox *comboBox_calltype;
    QLabel *label_29;
    QSpacerItem *horizontalSpacer_13;
    QLabel *label_45;
    QDateEdit *date_card_start;
    QLabel *label_27;
    QDateEdit *date_card_end;
    QSpacerItem *horizontalSpacer_36;
    QHBoxLayout *horizontalLayout_70;
    QCheckBox *checkBox_available_time;
    QHBoxLayout *horizontalLayout_25;
    QWidget *widget_available_time;
    QHBoxLayout *horizontalLayout_69;
    QTimeEdit *time_card_start;
    QLabel *label_40;
    QTimeEdit *time_card_end;
    QLabel *label_23;
    QCheckBox *check_card_week_1;
    QCheckBox *check_card_week_2;
    QCheckBox *check_card_week_3;
    QCheckBox *check_card_week_4;
    QCheckBox *check_card_week_5;
    QCheckBox *check_card_week_7;
    QCheckBox *check_card_week_6;
    QSpacerItem *horizontalSpacer_21;
    QHBoxLayout *horizontalLayout_45;
    QSpacerItem *horizontalSpacer_23;
    QPushButton *btn_makecard_ok;
    QWidget *page_makeadmincard;
    QVBoxLayout *verticalLayout_36;
    QWidget *widget_makeadmincard;
    QVBoxLayout *verticalLayout_35;
    QHBoxLayout *horizontalLayout_31;
    QHBoxLayout *horizontalLayout_92;
    QLabel *label_59;
    QLineEdit *line_makeadmin_name;
    QPushButton *btn_admin_list;
    QHBoxLayout *horizontalLayout_93;
    QLabel *label_60;
    QLineEdit *line_makeadmin_cardnum;
    QHBoxLayout *horizontalLayout_94;
    QLabel *label_61;
    QLineEdit *line_makeadmin_password;
    QSpacerItem *horizontalSpacer_52;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_30;
    QHBoxLayout *horizontalLayout_77;
    QCheckBox *checkBox_admin_cardValidtime;
    QHBoxLayout *horizontalLayout_61;
    QWidget *widget_admin_cardvalidtime;
    QHBoxLayout *horizontalLayout_78;
    QLabel *label_46;
    QDateEdit *date_admincard_start;
    QLabel *label_47;
    QDateEdit *date_admincard_end;
    QSpacerItem *horizontalSpacer_44;
    QHBoxLayout *horizontalLayout_84;
    QCheckBox *checkBox_admin_availabletime;
    QHBoxLayout *horizontalLayout_88;
    QWidget *widget_admin_availabletime;
    QHBoxLayout *horizontalLayout_91;
    QLabel *label_48;
    QTimeEdit *time_admincard_start;
    QLabel *label_49;
    QTimeEdit *time_admincard_end;
    QCheckBox *check_admincard_week_1;
    QCheckBox *check_admincard_week_2;
    QCheckBox *check_admincard_week_3;
    QCheckBox *check_admincard_week_4;
    QCheckBox *check_admincard_week_5;
    QCheckBox *check_admincard_week_6;
    QCheckBox *check_admincard_week_7;
    QSpacerItem *horizontalSpacer_51;
    QTextEdit *textEdit;
    QSpacerItem *verticalSpacer_9;
    QHBoxLayout *horizontalLayout_57;
    QSpacerItem *horizontalSpacer_38;
    QPushButton *btn_makeadmincard_ok;
    QWidget *page_maketoolcard;
    QVBoxLayout *verticalLayout_16;
    QWidget *widget_maketoolcard_1;
    QVBoxLayout *verticalLayout_15;
    QHBoxLayout *horizontalLayout_83;
    QHBoxLayout *horizontalLayout_85;
    QLabel *label_111;
    QLineEdit *line_maketoolcard_cardnum;
    QHBoxLayout *horizontalLayout_86;
    QLabel *label_112;
    QLineEdit *line_maketoolcard_password;
    QSpacerItem *horizontalSpacer_12;
    QGroupBox *groupBox_4;
    QHBoxLayout *horizontalLayout_40;
    QVBoxLayout *verticalLayout_20;
    QSpacerItem *verticalSpacer_18;
    QHBoxLayout *horizontalLayout_24;
    QLabel *label_117;
    QComboBox *comboBox_toolcard_units;
    QSpacerItem *verticalSpacer_7;
    QHBoxLayout *horizontalLayout_30;
    QVBoxLayout *verticalLayout_28;
    QSpacerItem *verticalSpacer_19;
    QHBoxLayout *horizontalLayout_66;
    QLabel *label_118;
    QSpacerItem *verticalSpacer_13;
    ScrollArea *scrollArea_12;
    QWidget *scroll_unitElevt;
    QWidget *widget_maketoolcard_2;
    QVBoxLayout *verticalLayout_13;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_20;
    QWidget *wid_toolcard_group;
    QVBoxLayout *verticalLayout_49;
    QPushButton *btn_maketoolcard_syntime;
    QPushButton *btn_maketoolcard_readcard;
    QPushButton *btn_maketoolcard_maskroom;
    QPushButton *btn_maketoolcard_openfloor;
    QPushButton *btn_maketoolcard_elevtimeallow;
    QPushButton *btn_maketoolcard_acticatecard;
    QPushButton *btn_maketoolcard_elevNum;
    QPushButton *btn_maketoolcard_lostCart;
    QSpacerItem *verticalSpacer_2;
    QStackedWidget *stacked_maketoolcard;
    QWidget *page_maketoolcard_syntime;
    QVBoxLayout *verticalLayout_41;
    QVBoxLayout *verticalLayout_40;
    QLabel *label_93;
    QHBoxLayout *horizontalLayout_60;
    QDateEdit *dateEdit_syntime;
    QTimeEdit *timeEdit_syntime;
    QSpacerItem *horizontalSpacer_39;
    QSpacerItem *verticalSpacer_10;
    QLabel *label_99;
    QWidget *page_maketoolcard_readcard;
    QVBoxLayout *verticalLayout_42;
    QHBoxLayout *horizontalLayout_79;
    QLabel *label_124;
    QLineEdit *line_readcard_selectowner;
    QPushButton *btn_readcard_selectowner;
    QLabel *label_125;
    QComboBox *combo_readcard_selectfloor;
    QSpacerItem *horizontalSpacer_40;
    QHBoxLayout *horizontalLayout_80;
    QLabel *label_100;
    QDateEdit *dateEdit_readcard_start;
    QTimeEdit *timeEdit_readcard_start;
    QLabel *label_101;
    QDateEdit *dateEdit_readcard_end;
    QTimeEdit *timeEdit_readcard_end;
    QSpacerItem *horizontalSpacer_41;
    QSpacerItem *verticalSpacer_11;
    QWidget *page_maketoolcard_maskroom;
    QVBoxLayout *verticalLayout_53;
    QLabel *label_102;
    QTabWidget *tabWidget_rooms;
    QHBoxLayout *horizontalLayout_81;
    QSpacerItem *horizontalSpacer_42;
    QPushButton *btn_maskelevt;
    QPushButton *btn_enablelevt;
    QPushButton *btn_maskfloor;
    QPushButton *btn_enablefloor;
    QWidget *page_maketoolcard_openfloor;
    QVBoxLayout *verticalLayout_43;
    QVBoxLayout *verticalLayout_54;
    QLabel *label_104;
    ScrollArea *scrollArea_openfloor;
    QWidget *scrollContents_openfloor;
    QHBoxLayout *horizontalLayout_82;
    QSpacerItem *horizontalSpacer_43;
    QWidget *page_maketoolcard_elevtimeallow;
    QVBoxLayout *verticalLayout_44;
    QHBoxLayout *horizontalLayout_62;
    QCheckBox *checkBox_time1;
    QWidget *widget_time1;
    QHBoxLayout *horizontalLayout_16;
    QLabel *label_106;
    QTimeEdit *timeEdit_timeallowstart1;
    QLabel *label_25;
    QTimeEdit *timeEdit_timeallowend1;
    QSpacerItem *horizontalSpacer_48;
    QHBoxLayout *horizontalLayout_64;
    QCheckBox *checkBox_time2;
    QWidget *widget_time2;
    QHBoxLayout *horizontalLayout_75;
    QLabel *label_107;
    QTimeEdit *timeEdit_timeallowstart2;
    QLabel *label_31;
    QTimeEdit *timeEdit_timeallowend2;
    QSpacerItem *horizontalSpacer_49;
    QHBoxLayout *horizontalLayout_65;
    QCheckBox *checkBox_time3;
    QWidget *widget_time3;
    QHBoxLayout *horizontalLayout_76;
    QLabel *label_108;
    QTimeEdit *timeEdit_timeallowstart3;
    QLabel *label_32;
    QTimeEdit *timeEdit_timeallowend3;
    QSpacerItem *horizontalSpacer_50;
    QHBoxLayout *horizontalLayout_63;
    QCheckBox *check_week_1;
    QCheckBox *check_week_2;
    QCheckBox *check_week_3;
    QCheckBox *check_week_4;
    QCheckBox *check_week_5;
    QCheckBox *check_week_6;
    QCheckBox *check_week_7;
    QSpacerItem *horizontalSpacer_47;
    QSpacerItem *verticalSpacer_12;
    QLabel *label_105;
    QWidget *page_maketoolcard_activatevCard;
    QCheckBox *activate_card;
    QLabel *label_15;
    QWidget *page_maketoolcard_elevatorNnum;
    QLabel *label_24;
    QWidget *page_maketoolcard_lostCard;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout_73;
    QLabel *label_39;
    QLineEdit *line_LostCardNum;
    QPushButton *btn_GetLostCardNum;
    QHBoxLayout *horizontalLayout_47;
    QSpacerItem *horizontalSpacer_29;
    QPushButton *btn_toolcard_ok;
    QWidget *page_losscard;
    QVBoxLayout *verticalLayout_45;
    QWidget *widget_losscard;
    QVBoxLayout *verticalLayout_39;
    QHBoxLayout *horizontalLayout_87;
    QHBoxLayout *horizontalLayout_89;
    QLabel *label_114;
    QLineEdit *line_cardloss_cardnum;
    QHBoxLayout *horizontalLayout_90;
    QLabel *label_115;
    QLineEdit *line_cardloss_password;
    QSpacerItem *horizontalSpacer_45;
    QSpacerItem *horizontalSpacer_54;
    QHBoxLayout *horizontalLayout_52;
    QHBoxLayout *horizontalLayout_58;
    QLabel *label_5;
    QLineEdit *lineEdit_Elevator_number;
    QPushButton *btn_elector_list;
    QHBoxLayout *horizontalLayout_59;
    QLabel *label_6;
    QLineEdit *lineEdit_card_number;
    QPushButton *btn_card_list;
    QSpacerItem *horizontalSpacer_37;
    QSpacerItem *horizontalSpacer_28;
    QSpacerItem *horizontalSpacer_26;
    QSpacerItem *verticalSpacer_5;
    QHBoxLayout *horizontalLayout_48;
    QSpacerItem *horizontalSpacer_46;
    QPushButton *btn_cardlist_ok_4;
    QPushButton *btn_cardlist_cancel_4;
    QSpacerItem *verticalSpacer_3;
    QWidget *page_remakecard;
    QHBoxLayout *horizontalLayout_71;
    QWidget *widget_remakecard;
    QHBoxLayout *horizontalLayout_72;
    QPushButton *btn_owner_replacecard;
    QPushButton *btn_tools_replacecard;
    QWidget *page_cellinfo;
    QVBoxLayout *verticalLayout_27;
    QTableWidget *table_cellinfo;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_25;
    QHBoxLayout *horizontalLayout_34;
    QLabel *label_tip_3;
    QPushButton *btn_cell_del;
    QSpacerItem *horizontalSpacer_17;
    QGroupBox *group_cell_content;
    QVBoxLayout *verticalLayout_24;
    QHBoxLayout *horizontalLayout_32;
    QCheckBox *check_addAuto;
    QLineEdit *line_cellunit;
    QLabel *label_11;
    QLineEdit *line_celllayer;
    QLabel *label_13;
    QLineEdit *line_cellfamily;
    QLabel *label_14;
    QSpacerItem *horizontalSpacer_19;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_33;
    QSpacerItem *horizontalSpacer_18;
    QPushButton *btn_cellinfo_ok;
    QPushButton *btn_cellinfo_cancel;
    QWidget *page_elevator;
    QVBoxLayout *verticalLayout_19;
    QHBoxLayout *horizontalLayout_26;
    QLineEdit *lineEdit_elevator_search;
    QPushButton *btn_elevator_search;
    QTableWidget *table_elevator;
    QWidget *wid_elevator;
    QVBoxLayout *verticalLayout_31;
    QHBoxLayout *horizontalLayout_42;
    QLabel *label_tip_4;
    QPushButton *btn_dellift;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *group_elevator_content;
    QVBoxLayout *verticalLayout_26;
    QHBoxLayout *horizontalLayout_43;
    QLabel *label_33;
    QComboBox *combo_unitnum;
    QLabel *label_52;
    QLineEdit *line_elevnum;
    QLabel *label_4;
    QLineEdit *line_ctrl_layernum;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_tip_5;
    QSplitter *splitter_3;
    QTableWidget *table_map;
    QWidget *widget_map;
    QVBoxLayout *verticalLayout_23;
    QHBoxLayout *horizontalLayout_54;
    QLabel *label_53;
    QLineEdit *line_actualLayer;
    QSpacerItem *horizontalSpacer_31;
    QHBoxLayout *horizontalLayout_55;
    QLabel *label_56;
    QLineEdit *line_mapLayer;
    QSpacerItem *horizontalSpacer_32;
    QHBoxLayout *horizontalLayout_56;
    QLabel *label_57;
    QLineEdit *line_roomlist;
    QCheckBox *checkBox_4;
    QLabel *label_17;
    QSpacerItem *horizontalSpacer_33;
    QSpacerItem *verticalSpacer_4;
    QHBoxLayout *horizontalLayout_49;
    QPushButton *btn_addliftmap;
    QPushButton *btn_delliftmap;
    QSpacerItem *horizontalSpacer_14;
    QHBoxLayout *horizontalLayout_53;
    QSpacerItem *horizontalSpacer_30;
    QPushButton *btn_elevt_ok;
    QPushButton *btn_elevt_cancel;

    void setupUi(QWidget *ICManager)
    {
        if (ICManager->objectName().isEmpty())
            ICManager->setObjectName(QStringLiteral("ICManager"));
        ICManager->resize(1366, 768);
        ICManager->setMinimumSize(QSize(1366, 768));
        ICManager->setMaximumSize(QSize(1366, 768));
        ICManager->setStyleSheet(QString::fromUtf8("QScrollArea{border:none;background-color:rgba(0,0,0,0);}\n"
"QWidget{background-color:rgba(0,0,0,0);}\n"
"\n"
"QScrollArea#scrollArea_12{border:1px solid rgba(189, 198, 206, 255);border-radius: 3px;background-color:rgba(0,0,0,0);}\n"
"\n"
"QScrollBar:vertical { background: rgba(0, 0, 0, 0);  width: 9px;  margin: 0px 0 0px 0; }\n"
"QScrollBar::handle:vertical { background: rgba(195, 195, 195, 255); min-height: 20px;  margin: 2px 0px 1px 0px;   border-radius: 3px;  border: 0px dashed rgba(180,130,100,255); }\n"
"QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }\n"
"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {  background: none; }\n"
"\n"
"QScrollBar:horizontal  { background: rgba(0, 0, 0, 0);  height: 9px;  margin: 0px 0 0px 0; }\n"
"QScrollBar::handle:horizontal  { background: rgba(195, 195, 195, 255);  min-width: 20px;  margin: 2px 0px 1px 0px;   border-radius: 3px;  border: 0px dashed rgba(180,130,100,255); }\n"
"QScrollBar::add-line:horizontal , QScrollBar::sub-l"
                        "ine:horizontal  { height: 0px; }\n"
"QScrollBar::add-page:horizontal , QScrollBar::sub-page:horizontal {  background: none; }\n"
"\n"
"/*QWidget#left{border-image:url(D:/MyProject/IC/rc/mainform_left.png);}*/\n"
"QWidget#left{border-image:url(D:/MyProject/IC/rc/mainform_left_bg.png);}\n"
"\n"
"QWidget#page_user,QWidget#page_admin,QWidget#page_cardlist,QWidget#page_cardlog,QWidget#page_makeownercard,QWidget#page_makeadmincard,QWidget#page_maketoolcard,QWidget#page_remakecard,QWidget#page_losscard,QWidget#page_cellinfo,QWidget#page_elevator{border-image:url(D:/MyProject/IC/rc/page_bg.png);}\n"
"\n"
"/*\346\257\217\344\270\252\344\270\273\350\246\201\351\241\265\351\235\242\344\270\212\347\232\204widget\346\267\241\347\273\277\350\211\262\345\272\225\345\233\276*/\n"
"QWidget#wid_elevator,QWidget#widget_makeownercard,QWidget#widget_maketoolcard_1,QWidget#widget_maketoolcard_2,QWidget#widget_losscard,QWidget#widget_remakecard,QWidget#widget_makeadmincard{background: rgba(230,239,252,255);}\n"
"\n"
"/*QWidget#wid_t"
                        "oolcard_group{background:rgba(24, 73, 163, 255);}*/\n"
"\n"
"QWidget#widget_datetime{background-color:rgba(230,239,252,255);}\n"
"\n"
"QWidget#top{border-image:url(D:/MyProject/IC/rc/mainform_top.png);}\n"
"\n"
"QWidget#page_top{border-image:url(D:/MyProject/IC/rc/page_top.png);}\n"
"\n"
"QWidget#widget_map{border:1px solid rgba(197, 116, 239, 255);}\n"
"\n"
"QWidget#widget_addr{border-image:url(D:/MyProject/IC/rc/person/3.png);}   /*\347\273\237\344\270\200\345\271\263\345\217\260\350\264\235\346\200\235\347\211\271*/\n"
"\n"
"QWidget#widget_left_bottom{border-image:url(D:/MyProject/IC/rc/left_bottom.png);}\n"
"\n"
"QPushButton#btn_search{border-image:url(D:/MyProject/IC/rc/btn_search.png);}\n"
"QLineEdit#lineEdit{border:none;background-color:rgba(0,0,0,0);}\n"
"\n"
"QPushButton#btn_user{border-image:url(D:/MyProject/IC/rc/btn_user.png);}\n"
"QPushButton#btn_user::checked{border-image:url(D:/MyProject/IC/rc/btn_user_light.png);}\n"
"\n"
"QPushButton#btn_admin{border-image:url(D:/MyProject/IC/rc/btn_administra"
                        "tor.png);}\n"
"QPushButton#btn_admin::checked{border-image:url(D:/MyProject/IC/rc/btn_administrator_light.png);}\n"
"\n"
"QPushButton#btn_cardlist{border-image:url(D:/MyProject/IC/rc/btn_cardlist.png);}\n"
"QPushButton#btn_cardlist::checked{border-image:url(D:/MyProject/IC/rc/btn_cardlist_light.png);}\n"
"\n"
"QPushButton#btn_cardlog{border-image:url(D:/MyProject/IC/rc/btn_cardreport.png);}\n"
"QPushButton#btn_cardlog::checked{border-image:url(D:/MyProject/IC/rc/btn_cardreport_light.png);}\n"
"\n"
"QPushButton#btn_makeownercard{border-image:url(D:/MyProject/IC/rc/btn_makeownercard.png);}\n"
"QPushButton#btn_makeownercard::checked{border-image:url(D:/MyProject/IC/rc/btn_makeownercard_light.png);}\n"
"\n"
"QPushButton#btn_makeadmincard{border-image:url(D:/MyProject/IC/rc/btn_makeadmincard.png);}\n"
"QPushButton#btn_makeadmincard::checked{border-image:url(D:/MyProject/IC/rc/btn_makeadmincard_light.png);}\n"
"\n"
"QPushButton#btn_maketoolcard{border-image:url(D:/MyProject/IC/rc/btn_maketoolcard.png);}\n"
"QPushBut"
                        "ton#btn_maketoolcard::checked{border-image:url(D:/MyProject/IC/rc/btn_maketoolcard_light.png);}\n"
"\n"
"QPushButton#btn_losscard{border-image:url(D:/MyProject/IC/rc/btn_losscard.png);}\n"
"QPushButton#btn_losscard::checked{border-image:url(D:/MyProject/IC/rc/btn_losscard_light.png);}\n"
"\n"
"QPushButton#btn_cellinfo{border-image:url(D:/MyProject/IC/rc/btn_cellinfo.png);}\n"
"QPushButton#btn_cellinfo::checked{border-image:url(D:/MyProject/IC/rc/btn_cellinfo_light.png);}\n"
"\n"
"QPushButton#btn_remakecard{border-image:url(D:/MyProject/IC/rc/btn_remakecard.png);}\n"
"QPushButton#btn_remakecard::checked{border-image:url(D:/MyProject/IC/rc/btn_remakecard_light.png);}\n"
"\n"
"QPushButton#btn_elevatorinfo{border-image:url(D:/MyProject/IC/rc/btn_elevatorinfo.png);}\n"
"QPushButton#btn_elevatorinfo::checked{border-image:url(D:/MyProject/IC/rc/btn_elevatorinfo_light.png);}\n"
"\n"
"QPushButton#btn_back{border-image:url(D:/MyProject/IC/rc/back_dark.png);}\n"
"QPushButton#btn_back::pressed{border-image:url(D:/MyProjec"
                        "t/IC/rc/back_light.png);}\n"
"\n"
"QPushButton#btn_quit{border-image:url(D:/MyProject/IC/rc/btn_quit.png);}\n"
"QPushButton#btn_quit::pressed{border-image:url(D:/MyProject/IC/rc/btn_quit_light.png);}\n"
"\n"
"QPushButton#btn_manager{border-image:url(D:/MyProject/IC/rc/btn_manager.png);}\n"
"QPushButton#btn_manager::pressed{border-image:url(D:/MyProject/IC/rc/btn_manager_light.png);}\n"
"\n"
"QWidget#widget_person{border-image:url(D:/MyProject/IC/rc/person/1.png);}\n"
"QPushButton#btn_person::pressed{border-image:url(D:/MyProject/IC/rc/person/1.png);}\n"
"\n"
"QPushButton#btn_admin_search,QPushButton#btn_user_search,QPushButton#btn_cardlist_search,QPushButton#btn_cardlog_search,QPushButton#btn_elevator_search{   /*\346\220\234\347\264\242\346\214\211\351\222\256\347\232\204\346\270\220\345\217\230*/\n"
"	font: large \"\345\256\213\344\275\223\"; font-size:10pt;\n"
"	color:white;\n"
" 	border: 1px solid rgba(127, 153, 189, 255); \n"
"    border-radius: 3px;\n"
"	background-color:qconicalgradient(cx:0.5, cy:0.5, "
                        "angle:0,\n"
"					stop:0 rgba(55,119,195,255), stop:0.373979 rgba(69, 140, 203,255), stop:0.373991 rgba(69, 140, 										203, 255), stop:0.624018 rgba(44, 103, 189, 255),stop:0.624043 rgba(44, 103, 189, 255),stop:1 										rgba(55,119,195,255));  \n"
"}\n"
"\n"
"QPushButton#btn_user_add,QPushButton#btn_admin_edit,QPushButton#btn_cell_add,QPushButton#btn_cell_del,QPushButton#btn_addlift,QPushButton#btn_dellift,QPushButton#btn_elevt_ok,QPushButton#btn_elevt_cancel,QPushButton#btn_cellinfo_ok,QPushButton#btn_cellinfo_cancel,QPushButton#btn_toolcard_ok,QPushButton#btn_admin_ok,QPushButton#btn_user_edit_ok,QPushButton#btn_owner_list,QPushButton#btn_user_recharge,QPushButton#btn_admin_list,QPushButton#btn_readcard_selectowner,QPushButton#btn_addliftmap,QPushButton#btn_delliftmap,QPushButton#btn_makecard_ok,QPushButton#btn_maskelevt,QPushButton#btn_enablelevt,QPushButton#btn_maskfloor,QPushButton#btn_enablefloor,QPushButton#btn_elector_list,QPushButton#btn_card_list,QPushButton#btn_owner_replacecard,QPushButton#b"
                        "tn_tools_replacecard,QPushButton#btn_cardlist_cancel_4,QPushButton#btn_cardlist_ok_4,QPushButton#btn_GetLostCardNum,QPushButton#btn_makeadmincard_ok,QPushButton#btn_saveFloor{   /*\346\267\273\345\212\240\345\210\240\351\231\244\346\214\211\351\222\256\347\232\204\346\270\220\345\217\230*/\n"
"	font: large \"\345\256\213\344\275\223\"; font-size:10pt;\n"
"	color:white;\n"
" 	border: 1px solid rgba(177, 194, 218, 255); \n"
"    border-radius: 3px;\n"
"	background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, \n"
"					stop:0 rgba(109, 184, 240, 255), stop:0.495 rgba(86, 151, 226, 255), stop:0.505 rgba(78, 139, 221, 255), 							stop:1 rgba(70, 129, 217, 255));\n"
"}\n"
"\n"
"QPushButton#btn_maketoolcard_readcard,QPushButton#btn_maketoolcard_maskroom,QPushButton#btn_maketoolcard_openfloor,QPushButton#btn_maketoolcard_acticatecard,QPushButton#btn_maketoolcard_elevNum,QPushButton#btn_maketoolcard_lostCart{	\n"
"	font: large \"\345\256\213\344\275\223\"; font-size:10pt;\n"
"	color:white;\n"
" 	border-t"
                        "op: 1px solid black;\n"
" 	border-bottom: 0px solid rgba(129, 132, 138, 255);\n"
"	border-left:1px solid rgba(129, 132, 138, 255);\n"
"	border-right:1px solid rgba(129, 132, 138, 255);\n"
"    border-radius: 0px;\n"
"	background-color:rgba(88, 143, 222, 255);\n"
"}\n"
"\n"
"\n"
"QPushButton#btn_maketoolcard_syntime{\n"
"	font: large \"\345\256\213\344\275\223\"; font-size:10pt;\n"
"	color:white;\n"
" 	border-top: 1px solid black;\n"
" 	border-bottom: 0px solid rgba(129, 132, 138, 255);\n"
"	border-left:1px solid rgba(129, 132, 138, 255);\n"
"	border-right:1px solid rgba(129, 132, 138, 255);\n"
"    border-radius: 0px;\n"
"	background-color:rgba(88, 143, 222, 255);\n"
"}\n"
"\n"
"QPushButton#btn_maketoolcard_elevtimeallow{\n"
"	font: large \"\345\256\213\344\275\223\"; font-size:10pt;\n"
"	color:white;\n"
" 	border-top: 1px solid black;\n"
" 	border-bottom: 1px solid rgba(129, 132, 138, 255);\n"
"	border-left:1px solid rgba(129, 132, 138, 255);\n"
"	border-right:1px solid rgba(129, 132, 138, 255);\n"
"    borde"
                        "r-radius: 0px;\n"
"	background-color:rgba(88, 143, 222, 255);\n"
"}\n"
"QPushButton#btn_maketoolcard_syntime::pressed,QPushButton#btn_maketoolcard_readcard::pressed,QPushButton#btn_maketoolcard_maskroom::pressed,QPushButton#btn_maketoolcard_openfloor::pressed,QPushButton#btn_maketoolcard_elevtimeallow::pressed,QPushButton#btn_maketoolcard_acticatecard::pressed,QPushButton#btn_maketoolcard_lostCart::pressed{\n"
"	font: large \"\345\256\213\344\275\223\"; font-size:10pt;\n"
"	color:white;\n"
" 	border-top-width: 1px;\n"
"    border-top-color: rgba(201, 211, 221, 255);\n"
"    border-top-style: solid; \n"
"    border-radius: 0px;\n"
"	background-color: rgba(85, 140, 222, 255);\n"
"}\n"
"\n"
"QPushButton#btn_user_add::pressed,QPushButton#btn_admin_edit::pressed,QPushButton#btn_cell_add::pressed,QPushButton#btn_cell_del::pressed,QPushButton#btn_addlift::pressed,QPushButton#btn_dellift::pressed,QPushButton#btn_owner_list::pressed,QPushButton#btn_admin_list::pressed,QPushButton#btn_user_recharge::pressed,QPushButton#"
                        "btn_readcard_selectowner::pressed,QPushButton#btn_addliftmap::pressed,QPushButton#btn_delliftmap::pressed,QPushButton#btn_admin_search::pressed,QPushButton#btn_user_search::pressed,QPushButton#btn_cardlist_search::pressed,QPushButton#btn_cardlog_search::pressed,QPushButton#btn_elevator_search::pressed,QPushButton#btn_elevt_ok::pressed,QPushButton#btn_elevt_cancel::pressed,QPushButton#btn_cellinfo_ok::pressed,QPushButton#btn_cellinfo_cancel::pressed,QPushButton#btn_toolcard_ok::pressed,QPushButton#btn_admin_ok::pressed,QPushButton#btn_user_edit_ok::pressed,QPushButton#btn_elevt_ok::pressed,QPushButton#btn_elevt_cancel::pressed,QPushButton#btn_maskelevt::pressed,QPushButton#btn_enablelevt::pressed,QPushButton#btn_maskfloor::pressed,QPushButton#btn_enablefloor::pressed,QPushButton#btn_elector_list::pressed,QPushButton#btn_card_list::pressed,QPushButton#btn_cardlist_cancel_4::pressed,QPushButton#btn_cardlist_ok_4::pressed,QPushButton#btn_GetLostCardNum::pressed,QPushButton#btn_makeadmincard_ok::pressed,QPushButton"
                        "#btn_saveFloor::pressed{\n"
"	font: large \"\345\256\213\344\275\223\"; font-size:10pt;\n"
"	color:white;\n"
" 	border: 1px solid rgba(127, 153, 189, 255); \n"
"    border-radius: 3px;\n"
"    background-color:  qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, \n"
"					stop:0 rgba(156, 219, 255, 255), stop:0.495 rgba(118, 202, 255, 255), stop:0.505 rgba(51, 172, 255, 255), 							stop:1 rgba(43, 170, 255, 255))\n"
"}\n"
"\n"
"QWidget#widget,QWidget#widget_2, QWidget#widget_3{background-color: rgba(231, 240, 252, 255);}/*\347\273\277\350\211\262*/\n"
"\n"
"QLabel#label_person,QLabel#label_addr{color:rgba(233,233,233,233);}\n"
"/*QWidget#page_user{background-clolr:rgba(39,46,62);}*/\n"
"\n"
"\n"
"QCheckBox{\n"
"    font: large \"\345\256\213\344\275\223\"; font-size:10pt;\n"
"	color:rgba(24,74, 163, 250);\n"
"}\n"
"\n"
"QCheckBox::indicator {\n"
"    width: 15px;\n"
"    height: 15px;\n"
"}\n"
"\n"
"QCheckBox::indicator:unchecked {\n"
"    image: url(D:/MyProject/IC/rc/check_off.png);\n"
"}\n"
"\n"
"QCheckBo"
                        "x::indicator:checked {\n"
"    image: url(D:/MyProject/IC/rc/check_on.png);\n"
"}\n"
"\n"
"QWidget#wid_valideles,QWidget#wid_validlayers{background-clolr:rgba(231, 240, 252, 255);}\n"
"\n"
"QLineEdit {\n"
"    border: 1px solid rgba(201, 211, 221, 255);\n"
"    border-radius: 3px;\n"
"    padding: 0 8px;\n"
"    background: rgba(231, 240, 252, 255);\n"
"    selection-background-color:  rgba(70, 129, 218, 255);\n"
"	color:rgba(24,74, 163, 250);\n"
"	font: large \"\345\256\213\344\275\223\"; font-size:10pt;\n"
"}\n"
"\n"
"QLabel{background-color: rgb(255, 255, 255, 0);font:large \"\345\256\213\344\275\223\"; font-size:10pt;  color:rgba(24,74, 163, 255);}\n"
"QLabel:disabled{font:large \"\345\256\213\344\275\223\"; font-size:10pt;  color:rgba(100, 100, 100, 255);}\n"
"\n"
"\n"
"QLabel#label_tip,QLabel#label_tip_2,QLabel#label_tip_3,QLabel#label_tip_4{border-image:url(D:/MyProject/IC/rc/tip.png);color:white;font:large \"\351\273\221\344\275\223\";}\n"
"\n"
"QComboBox {\n"
"    border: 1px solid rgba(201, 211, 221,"
                        " 255);\n"
"    border-radius: 3px;\n"
"    padding: 1px 6px 1px 3px;\n"
"	color:rgba(24,74, 163, 250);\n"
"	font: large \"\345\256\213\344\275\223\"; font-size:10pt;\n"
"}\n"
"\n"
"QComboBox:editable {\n"
"    background: rgba(231, 240, 252, 255);    /*\345\217\257\347\274\226\350\276\221\347\212\266\346\200\201\344\270\213\357\274\214\345\211\215\351\235\242ComboBox\350\203\214\346\231\257\351\242\234\350\211\262\357\274\214\344\270\215\345\214\205\346\213\254drop_down*/\n"
"}\n"
"\n"
"QComboBox:!editable, QComboBox::drop-down:editable {    /*combox\345\234\250\344\270\215\345\217\257\347\274\226\350\276\221\347\212\266\346\200\201\343\200\201\345\220\216\351\235\242\347\232\204drop_down\346\214\211\351\222\256\345\217\257\347\274\226\350\276\221\347\212\266\346\200\201\344\270\213\357\274\214\346\262\241\346\234\211\345\274\271\345\207\272\344\270\213\346\213\211\346\241\206\346\255\243\345\270\270\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262(combox\345\222\214drop_down)*/\n"
"     background:"
                        " rgba(231, 240, 252, 255);\n"
"}\n"
"\n"
"/* QComboBox gets the \"on\" state when the popup is open */ \n"
"QComboBox:!editable:on, QComboBox::drop-down:editable:on {   /*combox\345\234\250\344\270\215\345\217\257\347\274\226\350\276\221\347\212\266\346\200\201\343\200\201\345\220\216\351\235\242\347\232\204drop_down\346\214\211\351\222\256\345\217\257\347\274\226\350\276\221\347\212\266\346\200\201\344\270\213\357\274\214\345\274\271\345\207\272\344\270\213\346\213\211\346\241\206\345\220\216\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262(combox\345\222\214drop_down)*/\n"
"   background: rgba(255, 255, 255, 255);\n"
"}\n"
"\n"
"QComboBox:on { /* shift the text when the popup opens \345\274\271\345\207\272\344\270\213\346\213\211\346\241\206\345\220\216combox\344\270\212\346\226\207\345\255\227\347\247\273\345\212\250*/\n"
"    padding-top: 3px;\n"
"    padding-left: 4px;\n"
"}\n"
"\n"
"QComboBox::drop-down {         /*\345\257\271drop_down\346\214\211\351\222\256\347\232\204\350\256\276\347\275\256"
                        "*/\n"
"    subcontrol-origin: padding;\n"
"    subcontrol-position: top right;\n"
"    width: 15px;\n"
"\n"
"    border-left-width: 1px;\n"
"    border-left-color: rgba(201, 211, 221, 255);\n"
"    border-left-style: solid; /* just a single line */\n"
"    border-top-right-radius: 3px; /* same radius as the QComboBox */\n"
"    border-bottom-right-radius: 3px;\n"
"}\n"
"\n"
"QComboBox::down-arrow {       /*\345\257\271drop_down\346\214\211\351\222\256\344\270\212\347\232\204\347\256\255\345\244\264\345\233\276\347\211\207\350\277\233\350\241\214\350\256\276\347\275\256*/\n"
"    image: url(D:/MyProject/IC/rc/combox_arrow.png);\n"
"}\n"
"\n"
"QComboBox::down-arrow:on { /* shift the arrow when popup is open \344\270\213\346\213\211\346\241\206\345\220\216\357\274\214\347\256\255\345\244\264\344\275\215\347\275\256\347\232\204\345\217\230\345\214\226*/\n"
"    top: 0px;\n"
"    left: 0px;\n"
"}\n"
"\n"
"\n"
"QComboBox#combo_cardfee_type::drop-down,QComboBox#combox_recharge_value::drop-down,QComboBox#comboBox_call"
                        "type::drop-down,QComboBox#comboBox_response_time::drop-down,QComboBox#comboBox_toolcard_units::drop-down,QComboBox#combo_readcard_selectfloor::drop-down {         /*\345\257\271drop_down\346\214\211\351\222\256\347\232\204\350\256\276\347\275\256*/\n"
"    subcontrol-origin: padding;\n"
"    subcontrol-position: top right;\n"
"    width: 32px;\n"
"\n"
"    border-left-width: 1px;\n"
"    border-left-color: rgba(201, 211, 221, 255);\n"
"    border-left-style: solid; /* just a single line */\n"
"    border-top-right-radius: 3px; /* same radius as the QComboBox */\n"
"    border-bottom-right-radius: 3px;\n"
"}\n"
"\n"
"QComboBox#combo_unitnum::drop-down{\n"
"	width: 22px;\n"
"}\n"
"\n"
"/* QTableWidget\350\256\276\347\275\256 QHeaderView\357\274\232\346\225\264\344\270\252\350\241\250\345\244\264\357\274\214\344\270\215\346\255\242\346\230\257section\345\235\227*/\n"
"QHeaderView {\n"
"	margin: 0px;\n"
"    border: 0px solid rgba(201, 211, 221, 255);\n"
"    border-radius: 3px;\n"
"	padding: 0px 0px 0px 0px;\n"
""
                        "    background-color:qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                                      stop:0 rgba(159,195,235,255), stop: 0.5 rgba(107,155,218,255), /*\350\223\235\347\231\275\346\270\220\345\217\230*/\n"
"                                      stop: 0.6 rgba(79,133,209,255), stop:1 rgba(77,130,208,255));\n"
"	min-height:35px;\n"
"}\n"
"\n"
"QHeaderView#table_map{ \n"
"	border: 1px solid rgba(201, 211, 221, 255);\n"
"    background-color:qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                                      stop:0 rgba(159,195,235,255), stop: 0.5 rgba(107,155,218,255), /*\350\223\235\347\231\275\346\270\220\345\217\230*/\n"
"                                      stop: 0.6 rgba(79,133,209,255), stop:1 rgba(24,73,163,255));\n"
"}\n"
"\n"
"\n"
"/*\350\241\250\345\244\264\345\235\227*/\n"
"QHeaderView::section {\n"
"    background-color:rgba(0,0,0,0);\n"
"    color: white;\n"
"	font: large \"\345\256\213\344\275\223\"; font-size:10pt;\n"
"    padding: 0px 0px 0px 0px;\n"
"	border: 1px;\n"
""
                        "    border-left-color: rgba(127, 149, 172, 255);\n"
"    border-left-style: solid; \n"
"}\n"
"\n"
"\n"
"QTableView {\n"
"	color:rgba(24,73, 163, 255);\n"
"	font: large \"\345\256\213\344\275\223\"; font-size:10pt;\n"
"	background-color:rgba(228, 238, 251, 255);\n"
"	selection-color:white;\n"
"    selection-background-color:rgba(157, 188, 221, 255);\n"
"	border:0px;\n"
"}\n"
"\n"
"QTableView#table_map{       /*\350\241\250\346\240\274\350\276\271\346\241\206\347\272\277*/\n"
"	border: 1px solid rgba(197, 116, 239, 255);/*\346\230\240\345\260\204\350\276\271\346\241\206\346\267\241\347\264\253\350\211\262*/\n"
"}\n"
"\n"
"QSplitter::handle {\n"
"    background-color:rgba(228, 238, 251, 0);\n"
"}\n"
"\n"
"QSplitter::handle:vertical {\n"
"    height: 10px;\n"
"}\n"
"\n"
"QWidget#widget_validLayers,QWidget#widget_validElevt,QWidget#widget_validFloors { border: 1px solid rgba(201, 211, 221, 255);}\n"
"\n"
"/* QDateEdit\347\232\204\345\274\271\345\207\272\346\227\245\345\216\206\344\270\255\347\232\204\345\271\264Spi"
                        "nBox\346\214\211\351\222\256\347\232\204\350\256\276\347\275\256 */ \n"
"QAbstractSpinBox {\n"
"    border: 1px solid rgba(201, 211, 221, 255);\n"
"    border-radius: 3px;\n"
"    padding: 1px 10px 1px 3px;\n"
"	color:rgba(24,74, 163, 250);\n"
"	font: large \"\345\256\213\344\275\223\"; font-size:10pt;\n"
"}\n"
"\n"
"QAbstractSpinBox::down-arrow {         /*\346\227\245\345\216\206\344\270\255\347\232\204\345\271\264SpinBox\346\214\211\351\222\256\347\256\255\345\244\264\347\232\204\350\256\276\347\275\256*/\n"
"    image: url(D:/MyProject/IC/rc/combox_arrow.png);\n"
"\n"
"    subcontrol-origin: padding;\n"
"    subcontrol-position: top right;\n"
"    width: 15px;\n"
"\n"
"    border-left-width: 1px;\n"
"    border-left-color: rgba(201, 211, 221, 255);\n"
"    border-left-style: solid; /* just a single line */\n"
"    border-top-right-radius: 3px; /* same radius as the QComboBox */\n"
"    border-bottom-right-radius: 3px;\n"
"}\n"
"\n"
"QAbstractSpinBox::up-arrow {\n"
"    image: url(D:/MyProject/IC/rc/combox_"
                        "arrow_up.png);\n"
"\n"
"    width: 15px;\n"
"\n"
"    border-left-width: 1px;\n"
"    border-left-color: rgba(201, 211, 221, 255);\n"
"    border-left-style: solid; /* just a single line */\n"
"    border-top-right-radius: 3px; /* same radius as the QComboBox */\n"
"    border-bottom-right-radius: 3px;\n"
"}\n"
"/**/\n"
"\n"
"/*\350\256\276\347\275\256QDateEdit\346\234\200\345\217\263\350\276\271\347\232\204\344\270\213\346\213\211\346\214\211\351\222\256\357\274\210Calender\344\275\277\350\203\275\347\232\204\346\203\205\345\206\265\344\270\213\357\274\211*/\n"
"QDateEdit::down-arrow {\n"
"	image: none;\n"
"    border:none;\n"
"	subcontrol-origin: margin;\n"
"    subcontrol-position: top right;\n"
"}\n"
"\n"
"QAbstractSpinBox::drop-down {    /*\350\256\276\347\275\256QDateEdit\346\234\200\345\217\263\350\276\271\347\232\204\344\270\213\346\213\211\346\214\211\351\222\256*/\n"
"	image: url(D:/MyProject/IC/rc/combox_arrow.png);\n"
"\n"
"    subcontrol-origin: padding;\n"
"    subcontrol-position: top right;\n"
""
                        "    width: 32px;\n"
"\n"
"    border-left-width: 1px;    /*QDateEdit\344\270\213\346\213\211\346\214\211\351\222\256\347\232\204\345\267\246\350\276\271\347\272\277\347\232\204\347\272\277\345\256\275*/\n"
"    border-left-color: rgba(201, 211, 221, 255);\n"
"    border-left-style: solid; /* just a single line */\n"
"    border-top-right-radius: 3px; /* same radius as the QComboBox */\n"
"    border-bottom-right-radius: 3px;;\n"
"}\n"
"\n"
"/*\350\256\276\347\275\256QDateEdit\347\232\204\345\274\271\345\207\272\346\227\245\345\216\206\344\270\255\347\232\204\345\257\274\350\210\252\346\235\241\347\232\204\346\214\211\351\222\256*/\n"
"QToolButton#qt_calendar_prevmonth,QToolButton#qt_calendar_nextmonth,QToolButton#qt_calendar_monthbutton,QToolButton#qt_calendar_yearbutton{\n"
"	border:0px solid #000000;\n"
"	border-radius:3px;\n"
"	margin:3px 3px 3px 3px;	\n"
"	padding:3px;\n"
"	background-color:rgba(0,0,0,0);\n"
"}\n"
"\n"
"QToolButton#qt_calendar_prevmonth:hover,QToolButton#qt_calendar_nextmonth:hover,QToolBu"
                        "tton#qt_calendar_monthbutton:hover,QToolButton#qt_calendar_yearbutton:hover,QToolButton#qt_calendar_prevmonth:pressed,QToolButton#qt_calendar_nextmonth:pressed,QToolButton#qt_calendar_monthbutton:pressed,QToolButton#qt_calendar_yearbutton:pressed{\n"
"	border:1px solid #D8D8D8;\n"
"}\n"
"\n"
"/*\350\256\276\347\275\256\345\274\271\345\207\272\346\227\245\345\216\206Calender\344\270\255\347\232\204\345\271\264\347\274\226\350\276\221Spinbox*/\n"
"\n"
"QCalendarWidget QSpinBox#qt_calendar_yearedit{\n"
"    border: 1px solid rgba(201, 211, 221, 255);\n"
"    border-radius: 3px;\n"
"    padding: 1px 10px 1px 3px;\n"
"	color:rgba(24,74, 163, 250);\n"
"	font: large \"\345\256\213\344\275\223\"; font-size:10pt;\n"
"}\n"
"\n"
"QCalendarWidget QSpinBox#qt_calendar_yearedit::down-button {       /* QDateEdit\347\232\204\345\274\271\345\207\272\346\227\245\345\216\206\344\270\255\347\232\204\345\271\264\347\274\226\350\276\221\346\241\206\347\232\204\345\220\221\344\270\213\346\214\211\351\222\256\347\232\204\350\256\276\347"
                        "\275\256 */\n"
"    border:none;\n"
"	width: 15px;\n"
"}\n"
"QCalendarWidget QSpinBox#qt_calendar_yearedit::down-arrow:on { /* shift the arrow when popup is open */\n"
"    top: 1px;\n"
"    left: 1px;\n"
"}\n"
"\n"
"QCalendarWidget QSpinBox#qt_calendar_yearedit::up-button {\n"
"    border:none;\n"
"	width: 15px;\n"
"}\n"
"QCalendarWidget QSpinBox#qt_calendar_yearedit::up-arrow:on { /* shift the arrow when popup is open */\n"
"    top: 1px;\n"
"    left: 1px;\n"
"}\n"
"\n"
"/**/\n"
"\n"
"QCalendarWidget QToolButton::menu-indicator{\n"
"	image:None;\n"
"}\n"
"\n"
"QCalendarWidget QTableView{\n"
"	border-width:0px;\n"
"}\n"
"\n"
"QCalendarWidget QWidget{\n"
"    background: white;\n"
"}\n"
"\n"
"QCalendarWidget QToolButton#qt_calendar_monthbutton QMenu{    /* QDateEdit\347\232\204\345\274\271\345\207\272\346\227\245\345\216\206\344\270\255\347\232\204\346\234\210\346\214\211\351\222\256\346\241\206\347\232\204\344\270\213\346\213\211\350\217\234\345\215\225\347\232\204\346\225\264\344\275\223\350\256\276\347\275"
                        "\256*/\n"
"	/*background: rgba(180, 180, 180, 255);*/\n"
"	color:white;\n"
"	background:rgba(77,130,208,255);\n"
"	border: 0px solid black;\n"
"}\n"
"\n"
"QCalendarWidget QToolButton#qt_calendar_monthbutton QMenu::item {    /* QDateEdit\347\232\204\345\274\271\345\207\272\346\227\245\345\216\206\344\270\255\347\232\204\346\234\210\346\214\211\351\222\256\346\241\206\347\232\204\344\270\213\346\213\211\350\217\234\345\215\225\345\215\225\347\213\254\351\241\271\347\232\204\350\256\276\347\275\256*/\n"
"    /* sets background of menu item. set this to something non-transparent\n"
"        if you want menu color and menu item color to be different */\n"
"    background-color: transparent;\n"
"}\n"
"/* QDateEdi\346\227\245\345\216\206\344\270\255\346\234\210\346\214\211\351\222\256\347\232\204\344\270\213\346\213\211\350\217\234\345\215\225\344\270\255item\351\200\211\344\270\255\347\232\204\350\203\214\346\231\257\351\242\234\350\211\262*/\n"
"QCalendarWidget QToolButton#qt_calendar_monthbutton QMenu::item:select"
                        "ed { /* when user selects item using mouse or keyboard */\n"
"    background-color: rgba(26,151,242,255);\n"
"}\n"
"\n"
"QCalendarWidget QWidget#qt_calendar_navigationbar{\n"
"	border:0px solid #575757;\n"
"	border-width:0px 0px 0px 0px;\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                                      stop:0 rgba(159,195,235,255), stop: 0.5 rgba(107,155,218,255),\n"
"                                      stop: 0.6 rgba(79,133,209,255), stop:1 rgba(77,130,208,255),);\n"
"}\n"
"\n"
"\n"
"QTimeEdit::down-button {         /*QTimeEdit\347\232\204SpinBox\346\214\211\351\222\256\347\256\255\345\244\264\347\232\204\350\256\276\347\275\256*/\n"
"    image: url(D:/MyProject/IC/rc/combox_arrow.png);\n"
"\n"
"    width: 32px;\n"
"\n"
"    border-left-width: 1px;\n"
"    border-left-color: rgba(201, 211, 221, 255);\n"
"    border-left-style: solid; /* just a single line */\n"
"    border-top-right-radius: 3px; /* same radius as the QComboBox */\n"
"    border-bottom-right-radius: 3px;\n"
""
                        "}\n"
"\n"
"QTimeEdit::up-button {\n"
"    image: url(D:/MyProject/IC/rc/combox_arrow_up.png);\n"
"\n"
"    width: 32px;\n"
"\n"
"    border-left-width: 1px;\n"
"    border-left-color: rgba(201, 211, 221, 255);\n"
"    border-left-style: solid; /* just a single line */\n"
"    border-top-right-radius: 3px; /* same radius as the QComboBox */\n"
"    border-bottom-right-radius: 3px;\n"
"}\n"
"\n"
"QTimeEdit::down-arrow {         /*QTimeEdit\347\232\204SpinBox\346\214\211\351\222\256\347\256\255\345\244\264\347\232\204\350\256\276\347\275\256*/\n"
"    image: none;\n"
"	border-left-width: 0px;\n"
"}\n"
"\n"
"QTimeEdit::up-arrow {\n"
"    image: none;\n"
"	border-left-width: 0px;\n"
"}\n"
"\n"
"QGroupBox {\n"
"    /*background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #E0E0E0, stop: 1 #FFFFFF);*/\n"
"    border: 1px solid rgb(189,198,206);\n"
"    border-radius: 3px;\n"
"    margin-top: 1ex; /* leave space at the top for the title */\n"
"}\n"
"\n"
"QTabWidg"
                        "et#tabWidget_rooms{\n"
"	background-color:rgba(230,239,252,255);\n"
"}\n"
"\n"
"\n"
"QTabWidget::pane { /* The tab widget frame */\n"
"    border: 1px solid #C2C7CB;\n"
"}\n"
"\n"
"QTabWidget::tab-bar {\n"
"    left: 0px; /* move to the right by 5px */\n"
"}\n"
"\n"
"/* Style the tab using the tab sub-control. Note that\n"
"    it reads QTabBar _not_ QTabWidget */\n"
"QTabBar::tab {\n"
"    background: rgba(232,240,251,255);\n"
"    border: 1px solid #C4C4C3;\n"
"    font: large \"\345\256\213\344\275\223\"; font-size:10pt;\n"
"	color:rgba(24,74, 163, 250);\n"
"    border-radius: 2px;\n"
"\n"
"    min-width: 15ex;\n"
"    padding: 2px;  /*\345\255\227\344\270\216\350\276\271\346\241\206\347\232\204\350\267\235\347\246\273*/\n"
"	margin: 2px;	/*\350\276\271\346\241\206\344\270\216\350\276\271\346\241\206\347\232\204\350\267\235\347\246\273*/\n"
"}\n"
"\n"
"QTabBar::tab:selected, QTabBar::tab:hover {\n"
"    background: rgba(230,239,252,255);\n"
"	border-bottom: none;\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
" "
                        "   border-color: rgba(177,194,218,255);     /*\347\201\260\350\211\262*/\n"
"    border-top: 4px solid rgba(88,143,222,255); /* \350\223\235\350\211\262same as the pane color */\n"
"	border-bottom:none;\n"
"	margin-bottom: 0px;\n"
"}\n"
"\n"
"QTabBar::tab:!selected {\n"
"    margin-top: 2px; /* make non-selected tabs look smaller */\n"
"}\n"
"\n"
"QPushButton#btn_owner_replacecard::pressed,QPushButton#btn_tools_replacecard::pressed{\n"
"        font: large \"\345\256\213\344\275\223\"; font-size:10pt;\n"
"        color:white;\n"
"        border: 1px solid rgba(127, 153, 189, 255);\n"
"    border-radius: 3px;\n"
"    background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,\n"
"                                        stop:0 rgba(156, 219, 255, 255), stop:0.495 rgba(118, 202, 255, 255), stop:0.505 rgba(51, 172, 255, 255),stop:1 rgba(43, 170, 255, 255));\n"
"}\n"
""));
        gridLayout = new QGridLayout(ICManager);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        top = new QWidget(ICManager);
        top->setObjectName(QStringLiteral("top"));
        top->setMinimumSize(QSize(1358, 138));
        top->setMaximumSize(QSize(16777215, 138));
        horizontalLayout_51 = new QHBoxLayout(top);
        horizontalLayout_51->setSpacing(6);
        horizontalLayout_51->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_51->setObjectName(QStringLiteral("horizontalLayout_51"));
        widget_5 = new QWidget(top);
        widget_5->setObjectName(QStringLiteral("widget_5"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget_5->sizePolicy().hasHeightForWidth());
        widget_5->setSizePolicy(sizePolicy);
        widget_5->setMinimumSize(QSize(0, 41));
        widget_5->setMaximumSize(QSize(320, 41));

        horizontalLayout_51->addWidget(widget_5);

        verticalLayout_37 = new QVBoxLayout();
        verticalLayout_37->setSpacing(15);
        verticalLayout_37->setObjectName(QStringLiteral("verticalLayout_37"));
        horizontalLayout_50 = new QHBoxLayout();
        horizontalLayout_50->setSpacing(20);
        horizontalLayout_50->setObjectName(QStringLiteral("horizontalLayout_50"));
        btn_manager = new QPushButton(top);
        btn_manager->setObjectName(QStringLiteral("btn_manager"));
        btn_manager->setMinimumSize(QSize(90, 34));
        btn_manager->setMaximumSize(QSize(90, 34));

        horizontalLayout_50->addWidget(btn_manager);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_50->addItem(horizontalSpacer);

        widget_4 = new QWidget(top);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        horizontalLayout_46 = new QHBoxLayout(widget_4);
        horizontalLayout_46->setSpacing(25);
        horizontalLayout_46->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_46->setObjectName(QStringLiteral("horizontalLayout_46"));
        horizontalLayout_46->setContentsMargins(0, 0, 0, 0);
        widget_person = new QWidget(widget_4);
        widget_person->setObjectName(QStringLiteral("widget_person"));
        widget_person->setMinimumSize(QSize(182, 33));
        widget_person->setMaximumSize(QSize(182, 33));
        horizontalLayout_21 = new QHBoxLayout(widget_person);
        horizontalLayout_21->setSpacing(6);
        horizontalLayout_21->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_21->setObjectName(QStringLiteral("horizontalLayout_21"));
        horizontalLayout_21->setContentsMargins(20, 9, 9, -1);
        label_person = new QLabel(widget_person);
        label_person->setObjectName(QStringLiteral("label_person"));
        label_person->setMinimumSize(QSize(131, 21));
        label_person->setMaximumSize(QSize(131, 21));
        label_person->setAlignment(Qt::AlignCenter);

        horizontalLayout_21->addWidget(label_person);


        horizontalLayout_46->addWidget(widget_person);

        widget_addr = new QWidget(widget_4);
        widget_addr->setObjectName(QStringLiteral("widget_addr"));
        widget_addr->setMinimumSize(QSize(214, 30));
        widget_addr->setMaximumSize(QSize(214, 30));
        horizontalLayout_44 = new QHBoxLayout(widget_addr);
        horizontalLayout_44->setSpacing(6);
        horizontalLayout_44->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_44->setObjectName(QStringLiteral("horizontalLayout_44"));
        horizontalLayout_44->setContentsMargins(-1, 8, -1, -1);
        label_addr = new QLabel(widget_addr);
        label_addr->setObjectName(QStringLiteral("label_addr"));
        label_addr->setAlignment(Qt::AlignCenter);

        horizontalLayout_44->addWidget(label_addr);


        horizontalLayout_46->addWidget(widget_addr);

        btn_quit = new QPushButton(widget_4);
        btn_quit->setObjectName(QStringLiteral("btn_quit"));
        btn_quit->setMinimumSize(QSize(80, 27));
        btn_quit->setMaximumSize(QSize(80, 27));

        horizontalLayout_46->addWidget(btn_quit);


        horizontalLayout_50->addWidget(widget_4);


        verticalLayout_37->addLayout(horizontalLayout_50);

        btn_back = new QPushButton(top);
        btn_back->setObjectName(QStringLiteral("btn_back"));
        btn_back->setMinimumSize(QSize(104, 40));
        btn_back->setMaximumSize(QSize(104, 40));

        verticalLayout_37->addWidget(btn_back);


        horizontalLayout_51->addLayout(verticalLayout_37);


        gridLayout->addWidget(top, 0, 0, 1, 2);

        scrollArea = new ScrollArea(ICManager);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy1);
        scrollArea->setMinimumSize(QSize(247, 0));
        scrollArea->setMaximumSize(QSize(247, 16777215));
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        Left = new QWidget();
        Left->setObjectName(QStringLiteral("Left"));
        Left->setGeometry(QRect(0, 0, 249, 1000));
        Left->setMinimumSize(QSize(0, 1000));
        Left->setMaximumSize(QSize(16777215, 1000));
        gridLayout_2 = new QGridLayout(Left);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_38 = new QVBoxLayout();
        verticalLayout_38->setSpacing(0);
        verticalLayout_38->setObjectName(QStringLiteral("verticalLayout_38"));
        left = new QWidget(Left);
        left->setObjectName(QStringLiteral("left"));
        left->setMinimumSize(QSize(247, 580));
        left->setMaximumSize(QSize(247, 580));
        btn_maketoolcard = new QPushButton(left);
        btn_maketoolcard->setObjectName(QStringLiteral("btn_maketoolcard"));
        btn_maketoolcard->setGeometry(QRect(10, 337, 212, 38));
        btn_maketoolcard->setCheckable(true);
        btn_cardlog = new QPushButton(left);
        btn_cardlog->setObjectName(QStringLiteral("btn_cardlog"));
        btn_cardlog->setGeometry(QRect(10, 184, 212, 38));
        btn_cardlog->setCheckable(true);
        btn_losscard = new QPushButton(left);
        btn_losscard->setObjectName(QStringLiteral("btn_losscard"));
        btn_losscard->setGeometry(QRect(10, 374, 212, 38));
        btn_losscard->setCheckable(true);
        btn_remakecard = new QPushButton(left);
        btn_remakecard->setObjectName(QStringLiteral("btn_remakecard"));
        btn_remakecard->setGeometry(QRect(10, 410, 212, 38));
        btn_remakecard->setCheckable(true);
        btn_user = new QPushButton(left);
        btn_user->setObjectName(QStringLiteral("btn_user"));
        btn_user->setGeometry(QRect(10, 30, 212, 38));
        btn_user->setCheckable(true);
        btn_user->setChecked(false);
        btn_cellinfo = new QPushButton(left);
        btn_cellinfo->setObjectName(QStringLiteral("btn_cellinfo"));
        btn_cellinfo->setGeometry(QRect(10, 491, 212, 38));
        btn_cellinfo->setCheckable(true);
        btn_admin = new QPushButton(left);
        btn_admin->setObjectName(QStringLiteral("btn_admin"));
        btn_admin->setGeometry(QRect(10, 67, 212, 38));
        btn_admin->setCheckable(true);
        btn_makeadmincard = new QPushButton(left);
        btn_makeadmincard->setObjectName(QStringLiteral("btn_makeadmincard"));
        btn_makeadmincard->setGeometry(QRect(10, 301, 212, 38));
        btn_makeadmincard->setCheckable(true);
        btn_elevatorinfo = new QPushButton(left);
        btn_elevatorinfo->setObjectName(QStringLiteral("btn_elevatorinfo"));
        btn_elevatorinfo->setGeometry(QRect(10, 527, 212, 38));
        btn_elevatorinfo->setCheckable(true);
        btn_makeownercard = new QPushButton(left);
        btn_makeownercard->setObjectName(QStringLiteral("btn_makeownercard"));
        btn_makeownercard->setGeometry(QRect(10, 264, 212, 38));
        btn_makeownercard->setCheckable(true);
        btn_cardlist = new QPushButton(left);
        btn_cardlist->setObjectName(QStringLiteral("btn_cardlist"));
        btn_cardlist->setGeometry(QRect(10, 147, 212, 38));
        btn_cardlist->setCheckable(true);

        verticalLayout_38->addWidget(left);

        widget_left_bottom = new QWidget(Left);
        widget_left_bottom->setObjectName(QStringLiteral("widget_left_bottom"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(widget_left_bottom->sizePolicy().hasHeightForWidth());
        widget_left_bottom->setSizePolicy(sizePolicy2);
        widget_left_bottom->setMinimumSize(QSize(247, 0));
        widget_left_bottom->setMaximumSize(QSize(247, 16777215));

        verticalLayout_38->addWidget(widget_left_bottom);


        gridLayout_2->addLayout(verticalLayout_38, 0, 0, 1, 1);

        scrollArea->setWidget(Left);

        gridLayout->addWidget(scrollArea, 1, 0, 1, 1);

        stacked_pages = new QStackedWidget(ICManager);
        stacked_pages->setObjectName(QStringLiteral("stacked_pages"));
        stacked_pages->setEnabled(true);
        stacked_pages->setMinimumSize(QSize(0, 25));
        page_user = new QWidget();
        page_user->setObjectName(QStringLiteral("page_user"));
        verticalLayout_5 = new QVBoxLayout(page_user);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(-1, 0, -1, 9);
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        line_user_search = new QLineEdit(page_user);
        line_user_search->setObjectName(QStringLiteral("line_user_search"));
        line_user_search->setMinimumSize(QSize(1028, 25));
        line_user_search->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_6->addWidget(line_user_search);

        btn_user_search = new QPushButton(page_user);
        btn_user_search->setObjectName(QStringLiteral("btn_user_search"));
        btn_user_search->setMinimumSize(QSize(63, 30));
        btn_user_search->setMaximumSize(QSize(63, 30));

        horizontalLayout_6->addWidget(btn_user_search);


        verticalLayout_5->addLayout(horizontalLayout_6);

        splitter = new QSplitter(page_user);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Vertical);
        table_user = new QTableWidget(splitter);
        if (table_user->columnCount() < 6)
            table_user->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        table_user->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        table_user->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        table_user->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        table_user->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        table_user->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        table_user->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        table_user->setObjectName(QStringLiteral("table_user"));
        table_user->setCornerButtonEnabled(false);
        splitter->addWidget(table_user);
        table_user->verticalHeader()->setVisible(false);
        table_user->verticalHeader()->setDefaultSectionSize(30);
        table_user->verticalHeader()->setMinimumSectionSize(30);
        widget = new QWidget(splitter);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(0, 100));
        verticalLayout_8 = new QVBoxLayout(widget);
        verticalLayout_8->setSpacing(15);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(9, 15, 9, 4);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_24 = new QSpacerItem(15, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_24);

        label_tip = new QLabel(widget);
        label_tip->setObjectName(QStringLiteral("label_tip"));
        label_tip->setMinimumSize(QSize(112, 30));
        label_tip->setMaximumSize(QSize(112, 30));
        label_tip->setTextFormat(Qt::AutoText);
        label_tip->setScaledContents(false);
        label_tip->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(label_tip);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);


        verticalLayout_8->addLayout(horizontalLayout_4);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(15);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_9 = new QLabel(widget);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setEnabled(true);
        label_9->setMinimumSize(QSize(40, 0));
        label_9->setMaximumSize(QSize(40, 16777215));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(label_9);

        line_user_name = new QLineEdit(widget);
        line_user_name->setObjectName(QStringLiteral("line_user_name"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(line_user_name->sizePolicy().hasHeightForWidth());
        line_user_name->setSizePolicy(sizePolicy3);
        line_user_name->setMinimumSize(QSize(113, 21));
        line_user_name->setMaximumSize(QSize(113, 21));

        horizontalLayout_2->addWidget(line_user_name);

        label_10 = new QLabel(widget);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setMinimumSize(QSize(40, 0));
        label_10->setMaximumSize(QSize(40, 16777215));
        label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(label_10);

        line_user_phone = new QLineEdit(widget);
        line_user_phone->setObjectName(QStringLiteral("line_user_phone"));
        sizePolicy3.setHeightForWidth(line_user_phone->sizePolicy().hasHeightForWidth());
        line_user_phone->setSizePolicy(sizePolicy3);
        line_user_phone->setMinimumSize(QSize(113, 21));
        line_user_phone->setMaximumSize(QSize(113, 21));

        horizontalLayout_2->addWidget(line_user_phone);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_6);


        verticalLayout_4->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_12 = new QLabel(widget);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setMinimumSize(QSize(40, 0));
        label_12->setMaximumSize(QSize(40, 16777215));
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(label_12);

        combox_user_unit_num = new QComboBox(widget);
        combox_user_unit_num->setObjectName(QStringLiteral("combox_user_unit_num"));
        sizePolicy3.setHeightForWidth(combox_user_unit_num->sizePolicy().hasHeightForWidth());
        combox_user_unit_num->setSizePolicy(sizePolicy3);
        combox_user_unit_num->setMinimumSize(QSize(78, 21));
        combox_user_unit_num->setMaximumSize(QSize(78, 21));
        combox_user_unit_num->setEditable(false);
        combox_user_unit_num->setFrame(true);

        horizontalLayout_3->addWidget(combox_user_unit_num);

        label_34 = new QLabel(widget);
        label_34->setObjectName(QStringLiteral("label_34"));
        sizePolicy3.setHeightForWidth(label_34->sizePolicy().hasHeightForWidth());
        label_34->setSizePolicy(sizePolicy3);
        label_34->setMinimumSize(QSize(30, 0));
        label_34->setMaximumSize(QSize(30, 16777215));
        label_34->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(label_34);

        combox_user_layer_num = new QComboBox(widget);
        combox_user_layer_num->setObjectName(QStringLiteral("combox_user_layer_num"));
        sizePolicy3.setHeightForWidth(combox_user_layer_num->sizePolicy().hasHeightForWidth());
        combox_user_layer_num->setSizePolicy(sizePolicy3);
        combox_user_layer_num->setMinimumSize(QSize(78, 21));
        combox_user_layer_num->setMaximumSize(QSize(78, 21));
        combox_user_layer_num->setEditable(false);

        horizontalLayout_3->addWidget(combox_user_layer_num);

        label_35 = new QLabel(widget);
        label_35->setObjectName(QStringLiteral("label_35"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(label_35->sizePolicy().hasHeightForWidth());
        label_35->setSizePolicy(sizePolicy4);
        label_35->setMinimumSize(QSize(20, 0));
        label_35->setMaximumSize(QSize(20, 16777215));
        label_35->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(label_35);

        combox_user_family_num = new QComboBox(widget);
        combox_user_family_num->setObjectName(QStringLiteral("combox_user_family_num"));
        sizePolicy3.setHeightForWidth(combox_user_family_num->sizePolicy().hasHeightForWidth());
        combox_user_family_num->setSizePolicy(sizePolicy3);
        combox_user_family_num->setMinimumSize(QSize(78, 21));
        combox_user_family_num->setMaximumSize(QSize(78, 21));
        combox_user_family_num->setEditable(false);

        horizontalLayout_3->addWidget(combox_user_family_num);

        label_36 = new QLabel(widget);
        label_36->setObjectName(QStringLiteral("label_36"));
        label_36->setMinimumSize(QSize(20, 0));
        label_36->setMaximumSize(QSize(20, 16777215));
        label_36->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(label_36);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_7);


        verticalLayout_4->addLayout(horizontalLayout_3);


        verticalLayout_8->addLayout(verticalLayout_4);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(10);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        widget_validElevt = new QWidget(widget);
        widget_validElevt->setObjectName(QStringLiteral("widget_validElevt"));
        verticalLayout_3 = new QVBoxLayout(widget_validElevt);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(16, 10, 16, 10);
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_2 = new QLabel(widget_validElevt);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout->addWidget(label_2);

        verticalSpacer_15 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_15);


        horizontalLayout_7->addLayout(verticalLayout);

        scrollArea_2 = new ScrollArea(widget_validElevt);
        scrollArea_2->setObjectName(QStringLiteral("scrollArea_2"));
        scrollArea_2->setWidgetResizable(true);
        wid_valideles = new QWidget();
        wid_valideles->setObjectName(QStringLiteral("wid_valideles"));
        wid_valideles->setGeometry(QRect(0, 0, 16, 16));
        scrollArea_2->setWidget(wid_valideles);

        horizontalLayout_7->addWidget(scrollArea_2);


        verticalLayout_3->addLayout(horizontalLayout_7);


        verticalLayout_6->addWidget(widget_validElevt);

        widget_validLayers = new QWidget(widget);
        widget_validLayers->setObjectName(QStringLiteral("widget_validLayers"));
        horizontalLayout = new QHBoxLayout(widget_validLayers);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(16, 10, 16, 10);
        horizontalLayout_35 = new QHBoxLayout();
        horizontalLayout_35->setSpacing(6);
        horizontalLayout_35->setObjectName(QStringLiteral("horizontalLayout_35"));
        horizontalLayout_35->setContentsMargins(0, 0, 0, 0);
        verticalLayout_29 = new QVBoxLayout();
        verticalLayout_29->setSpacing(6);
        verticalLayout_29->setObjectName(QStringLiteral("verticalLayout_29"));
        label = new QLabel(widget_validLayers);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_29->addWidget(label);

        verticalSpacer_16 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_29->addItem(verticalSpacer_16);


        horizontalLayout_35->addLayout(verticalLayout_29);

        scrollArea_3 = new ScrollArea(widget_validLayers);
        scrollArea_3->setObjectName(QStringLiteral("scrollArea_3"));
        scrollArea_3->setWidgetResizable(true);
        wid_validlayers = new QWidget();
        wid_validlayers->setObjectName(QStringLiteral("wid_validlayers"));
        wid_validlayers->setGeometry(QRect(0, 0, 16, 16));
        wid_prgmsg = new QWidget(wid_validlayers);
        wid_prgmsg->setObjectName(QStringLiteral("wid_prgmsg"));
        wid_prgmsg->setGeometry(QRect(-27, 20, 1083, 30));
        wid_prgmsg->setMinimumSize(QSize(0, 30));
        wid_prgmsg->setMaximumSize(QSize(16777215, 30));
        horizontalLayout_67 = new QHBoxLayout(wid_prgmsg);
        horizontalLayout_67->setSpacing(0);
        horizontalLayout_67->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_67->setObjectName(QStringLiteral("horizontalLayout_67"));
        horizontalLayout_67->setContentsMargins(0, 0, 0, -1);
        label_msg = new QLabel(wid_prgmsg);
        label_msg->setObjectName(QStringLiteral("label_msg"));
        label_msg->setMinimumSize(QSize(200, 0));
        label_msg->setMaximumSize(QSize(200, 16777215));

        horizontalLayout_67->addWidget(label_msg);

        progress = new QProgressBar(wid_prgmsg);
        progress->setObjectName(QStringLiteral("progress"));
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(progress->sizePolicy().hasHeightForWidth());
        progress->setSizePolicy(sizePolicy5);
        progress->setValue(0);

        horizontalLayout_67->addWidget(progress);

        label_26 = new QLabel(wid_prgmsg);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setMinimumSize(QSize(200, 0));
        label_26->setMaximumSize(QSize(200, 16777215));

        horizontalLayout_67->addWidget(label_26);

        scrollArea_3->setWidget(wid_validlayers);

        horizontalLayout_35->addWidget(scrollArea_3);


        horizontalLayout->addLayout(horizontalLayout_35);


        verticalLayout_6->addWidget(widget_validLayers);


        verticalLayout_8->addLayout(verticalLayout_6);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_8);

        btn_user_edit_ok = new QPushButton(widget);
        btn_user_edit_ok->setObjectName(QStringLiteral("btn_user_edit_ok"));
        btn_user_edit_ok->setMinimumSize(QSize(64, 30));
        btn_user_edit_ok->setMaximumSize(QSize(64, 30));

        horizontalLayout_5->addWidget(btn_user_edit_ok);


        verticalLayout_8->addLayout(horizontalLayout_5);

        splitter->addWidget(widget);

        verticalLayout_5->addWidget(splitter);

        stacked_pages->addWidget(page_user);
        page_admin = new QWidget();
        page_admin->setObjectName(QStringLiteral("page_admin"));
        verticalLayout_12 = new QVBoxLayout(page_admin);
        verticalLayout_12->setSpacing(5);
        verticalLayout_12->setContentsMargins(11, 11, 11, 11);
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        verticalLayout_12->setContentsMargins(9, 0, 9, 9);
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(9);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        line_admin_search = new QLineEdit(page_admin);
        line_admin_search->setObjectName(QStringLiteral("line_admin_search"));
        line_admin_search->setMinimumSize(QSize(0, 25));
        line_admin_search->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_11->addWidget(line_admin_search);

        btn_admin_search = new QPushButton(page_admin);
        btn_admin_search->setObjectName(QStringLiteral("btn_admin_search"));
        btn_admin_search->setMinimumSize(QSize(63, 30));
        btn_admin_search->setMaximumSize(QSize(63, 30));
        btn_admin_search->setCheckable(false);
        btn_admin_search->setChecked(false);

        horizontalLayout_11->addWidget(btn_admin_search);


        verticalLayout_12->addLayout(horizontalLayout_11);

        splitter_2 = new QSplitter(page_admin);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        table_admin = new QTableWidget(splitter_2);
        if (table_admin->columnCount() < 6)
            table_admin->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        table_admin->setHorizontalHeaderItem(0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        table_admin->setHorizontalHeaderItem(1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        table_admin->setHorizontalHeaderItem(2, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        table_admin->setHorizontalHeaderItem(3, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        table_admin->setHorizontalHeaderItem(4, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        table_admin->setHorizontalHeaderItem(5, __qtablewidgetitem11);
        table_admin->setObjectName(QStringLiteral("table_admin"));
        splitter_2->addWidget(table_admin);
        table_admin->verticalHeader()->setVisible(false);
        widget_2 = new QWidget(splitter_2);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setMinimumSize(QSize(0, 300));
        verticalLayout_10 = new QVBoxLayout(widget_2);
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setContentsMargins(11, 11, 11, 11);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        horizontalSpacer_25 = new QSpacerItem(18, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_25);

        label_tip_2 = new QLabel(widget_2);
        label_tip_2->setObjectName(QStringLiteral("label_tip_2"));
        label_tip_2->setMinimumSize(QSize(112, 30));
        label_tip_2->setMaximumSize(QSize(112, 30));
        label_tip_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_15->addWidget(label_tip_2);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_10);


        verticalLayout_10->addLayout(horizontalLayout_15);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(2);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        label_admin_name = new QLabel(widget_2);
        label_admin_name->setObjectName(QStringLiteral("label_admin_name"));
        label_admin_name->setMinimumSize(QSize(40, 0));
        label_admin_name->setMaximumSize(QSize(40, 16777215));
        label_admin_name->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_12->addWidget(label_admin_name);

        line_admin_name = new QLineEdit(widget_2);
        line_admin_name->setObjectName(QStringLiteral("line_admin_name"));
        sizePolicy3.setHeightForWidth(line_admin_name->sizePolicy().hasHeightForWidth());
        line_admin_name->setSizePolicy(sizePolicy3);
        line_admin_name->setMinimumSize(QSize(115, 23));
        line_admin_name->setMaximumSize(QSize(115, 23));

        horizontalLayout_12->addWidget(line_admin_name);

        label_admin_phone = new QLabel(widget_2);
        label_admin_phone->setObjectName(QStringLiteral("label_admin_phone"));
        label_admin_phone->setMinimumSize(QSize(60, 0));
        label_admin_phone->setMaximumSize(QSize(30, 16777215));
        label_admin_phone->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_12->addWidget(label_admin_phone);

        line_admin_phone = new QLineEdit(widget_2);
        line_admin_phone->setObjectName(QStringLiteral("line_admin_phone"));
        sizePolicy3.setHeightForWidth(line_admin_phone->sizePolicy().hasHeightForWidth());
        line_admin_phone->setSizePolicy(sizePolicy3);
        line_admin_phone->setMinimumSize(QSize(115, 23));
        line_admin_phone->setMaximumSize(QSize(115, 23));

        horizontalLayout_12->addWidget(line_admin_phone);

        label_41 = new QLabel(widget_2);
        label_41->setObjectName(QStringLiteral("label_41"));

        horizontalLayout_12->addWidget(label_41);

        line_admin_password = new QLineEdit(widget_2);
        line_admin_password->setObjectName(QStringLiteral("line_admin_password"));
        sizePolicy3.setHeightForWidth(line_admin_password->sizePolicy().hasHeightForWidth());
        line_admin_password->setSizePolicy(sizePolicy3);
        line_admin_password->setMinimumSize(QSize(115, 23));
        line_admin_password->setMaximumSize(QSize(115, 23));
        line_admin_password->setCursor(QCursor(Qt::IBeamCursor));
        line_admin_password->setEchoMode(QLineEdit::Password);

        horizontalLayout_12->addWidget(line_admin_password);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_15);


        verticalLayout_10->addLayout(horizontalLayout_12);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(2);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        label_7 = new QLabel(widget_2);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setMinimumSize(QSize(38, 0));
        label_7->setMaximumSize(QSize(38, 16777215));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_14->addWidget(label_7);

        line_admin_address = new QLineEdit(widget_2);
        line_admin_address->setObjectName(QStringLiteral("line_admin_address"));
        sizePolicy3.setHeightForWidth(line_admin_address->sizePolicy().hasHeightForWidth());
        line_admin_address->setSizePolicy(sizePolicy3);
        line_admin_address->setMinimumSize(QSize(300, 23));
        line_admin_address->setMaximumSize(QSize(300, 23));

        horizontalLayout_14->addWidget(line_admin_address);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_9);


        verticalLayout_10->addLayout(horizontalLayout_14);

        horizontalLayout_22 = new QHBoxLayout();
        horizontalLayout_22->setSpacing(6);
        horizontalLayout_22->setObjectName(QStringLiteral("horizontalLayout_22"));
        verticalLayout_14 = new QVBoxLayout();
        verticalLayout_14->setSpacing(0);
        verticalLayout_14->setObjectName(QStringLiteral("verticalLayout_14"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        label_manager_permission = new QLabel(widget_2);
        label_manager_permission->setObjectName(QStringLiteral("label_manager_permission"));
        label_manager_permission->setMaximumSize(QSize(16777215, 20));

        verticalLayout_7->addWidget(label_manager_permission);

        groupBox_5 = new QGroupBox(widget_2);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setMinimumSize(QSize(500, 100));
        horizontalLayout_19 = new QHBoxLayout(groupBox_5);
        horizontalLayout_19->setSpacing(0);
        horizontalLayout_19->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_19->setObjectName(QStringLiteral("horizontalLayout_19"));
        horizontalLayout_19->setContentsMargins(0, 0, 0, 0);
        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setSpacing(10);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(10, 6, -1, -1);
        checkBox_adminPower_makeownercard = new QCheckBox(groupBox_5);
        checkBox_adminPower_makeownercard->setObjectName(QStringLiteral("checkBox_adminPower_makeownercard"));
        checkBox_adminPower_makeownercard->setMinimumSize(QSize(100, 20));

        verticalLayout_9->addWidget(checkBox_adminPower_makeownercard);

        checkBox_adminPower_makeadmincard = new QCheckBox(groupBox_5);
        checkBox_adminPower_makeadmincard->setObjectName(QStringLiteral("checkBox_adminPower_makeadmincard"));
        checkBox_adminPower_makeadmincard->setMinimumSize(QSize(100, 20));

        verticalLayout_9->addWidget(checkBox_adminPower_makeadmincard);

        verticalSpacer_17 = new QSpacerItem(20, 8, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_9->addItem(verticalSpacer_17);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setSpacing(6);
        horizontalLayout_18->setObjectName(QStringLiteral("horizontalLayout_18"));
        label_8 = new QLabel(groupBox_5);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setMinimumSize(QSize(0, 30));
        label_8->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_18->addWidget(label_8);

        comboBox_topup = new QComboBox(groupBox_5);
        comboBox_topup->setObjectName(QStringLiteral("comboBox_topup"));
        QSizePolicy sizePolicy6(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(comboBox_topup->sizePolicy().hasHeightForWidth());
        comboBox_topup->setSizePolicy(sizePolicy6);
        comboBox_topup->setMinimumSize(QSize(0, 25));
        comboBox_topup->setMaximumSize(QSize(150, 25));

        horizontalLayout_18->addWidget(comboBox_topup);

        label_18 = new QLabel(groupBox_5);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setMinimumSize(QSize(0, 30));
        label_18->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_18->addWidget(label_18);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_18->addItem(horizontalSpacer_11);


        verticalLayout_9->addLayout(horizontalLayout_18);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_6);


        horizontalLayout_19->addLayout(verticalLayout_9);

        verticalLayout_34 = new QVBoxLayout();
        verticalLayout_34->setSpacing(10);
        verticalLayout_34->setObjectName(QStringLiteral("verticalLayout_34"));
        verticalLayout_34->setContentsMargins(-1, 6, -1, 6);
        checkBox_adminPower_makesyntimecard = new QCheckBox(groupBox_5);
        checkBox_adminPower_makesyntimecard->setObjectName(QStringLiteral("checkBox_adminPower_makesyntimecard"));
        checkBox_adminPower_makesyntimecard->setMinimumSize(QSize(100, 20));

        verticalLayout_34->addWidget(checkBox_adminPower_makesyntimecard);

        checkBox_adminPower_makereadcard = new QCheckBox(groupBox_5);
        checkBox_adminPower_makereadcard->setObjectName(QStringLiteral("checkBox_adminPower_makereadcard"));
        checkBox_adminPower_makereadcard->setMinimumSize(QSize(100, 20));

        verticalLayout_34->addWidget(checkBox_adminPower_makereadcard);

        checkBox_adminPower_makemaskroomcard = new QCheckBox(groupBox_5);
        checkBox_adminPower_makemaskroomcard->setObjectName(QStringLiteral("checkBox_adminPower_makemaskroomcard"));
        checkBox_adminPower_makemaskroomcard->setMinimumSize(QSize(100, 20));

        verticalLayout_34->addWidget(checkBox_adminPower_makemaskroomcard);

        checkBox_adminPower_makeopenfloorcard = new QCheckBox(groupBox_5);
        checkBox_adminPower_makeopenfloorcard->setObjectName(QStringLiteral("checkBox_adminPower_makeopenfloorcard"));
        checkBox_adminPower_makeopenfloorcard->setMinimumSize(QSize(100, 20));

        verticalLayout_34->addWidget(checkBox_adminPower_makeopenfloorcard);

        checkBox_adminPower_makelevatimeallow = new QCheckBox(groupBox_5);
        checkBox_adminPower_makelevatimeallow->setObjectName(QStringLiteral("checkBox_adminPower_makelevatimeallow"));
        checkBox_adminPower_makelevatimeallow->setMinimumSize(QSize(100, 20));

        verticalLayout_34->addWidget(checkBox_adminPower_makelevatimeallow);


        horizontalLayout_19->addLayout(verticalLayout_34);

        horizontalSpacer_34 = new QSpacerItem(456, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_19->addItem(horizontalSpacer_34);


        verticalLayout_7->addWidget(groupBox_5);


        verticalLayout_14->addLayout(verticalLayout_7);


        horizontalLayout_22->addLayout(verticalLayout_14);


        verticalLayout_10->addLayout(horizontalLayout_22);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setSpacing(0);
        horizontalLayout_17->setObjectName(QStringLiteral("horizontalLayout_17"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_17->addItem(horizontalSpacer_3);

        btn_admin_ok = new QPushButton(widget_2);
        btn_admin_ok->setObjectName(QStringLiteral("btn_admin_ok"));
        btn_admin_ok->setMinimumSize(QSize(63, 29));
        btn_admin_ok->setMaximumSize(QSize(63, 29));

        horizontalLayout_17->addWidget(btn_admin_ok);


        verticalLayout_10->addLayout(horizontalLayout_17);

        splitter_2->addWidget(widget_2);

        verticalLayout_12->addWidget(splitter_2);

        stacked_pages->addWidget(page_admin);
        page_cardlist = new QWidget();
        page_cardlist->setObjectName(QStringLiteral("page_cardlist"));
        verticalLayout_33 = new QVBoxLayout(page_cardlist);
        verticalLayout_33->setSpacing(6);
        verticalLayout_33->setContentsMargins(11, 11, 11, 11);
        verticalLayout_33->setObjectName(QStringLiteral("verticalLayout_33"));
        verticalLayout_33->setContentsMargins(-1, 0, -1, 9);
        horizontalLayout_23 = new QHBoxLayout();
        horizontalLayout_23->setSpacing(6);
        horizontalLayout_23->setObjectName(QStringLiteral("horizontalLayout_23"));
        line_search_card = new QLineEdit(page_cardlist);
        line_search_card->setObjectName(QStringLiteral("line_search_card"));
        line_search_card->setMinimumSize(QSize(0, 25));

        horizontalLayout_23->addWidget(line_search_card);

        btn_cardlist_search = new QPushButton(page_cardlist);
        btn_cardlist_search->setObjectName(QStringLiteral("btn_cardlist_search"));
        btn_cardlist_search->setMinimumSize(QSize(63, 30));
        btn_cardlist_search->setMaximumSize(QSize(63, 30));

        horizontalLayout_23->addWidget(btn_cardlist_search);


        verticalLayout_33->addLayout(horizontalLayout_23);

        table_cardlist = new QTableWidget(page_cardlist);
        if (table_cardlist->columnCount() < 8)
            table_cardlist->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        table_cardlist->setHorizontalHeaderItem(0, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        table_cardlist->setHorizontalHeaderItem(1, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        table_cardlist->setHorizontalHeaderItem(2, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        table_cardlist->setHorizontalHeaderItem(3, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        table_cardlist->setHorizontalHeaderItem(4, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        table_cardlist->setHorizontalHeaderItem(5, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        table_cardlist->setHorizontalHeaderItem(6, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        table_cardlist->setHorizontalHeaderItem(7, __qtablewidgetitem19);
        table_cardlist->setObjectName(QStringLiteral("table_cardlist"));
        sizePolicy5.setHeightForWidth(table_cardlist->sizePolicy().hasHeightForWidth());
        table_cardlist->setSizePolicy(sizePolicy5);
        table_cardlist->verticalHeader()->setVisible(false);

        verticalLayout_33->addWidget(table_cardlist);

        stacked_pages->addWidget(page_cardlist);
        page_cardlog = new QWidget();
        page_cardlog->setObjectName(QStringLiteral("page_cardlog"));
        verticalLayout_21 = new QVBoxLayout(page_cardlog);
        verticalLayout_21->setSpacing(6);
        verticalLayout_21->setContentsMargins(11, 11, 11, 11);
        verticalLayout_21->setObjectName(QStringLiteral("verticalLayout_21"));
        verticalLayout_21->setContentsMargins(-1, 0, -1, 9);
        horizontalLayout_28 = new QHBoxLayout();
        horizontalLayout_28->setSpacing(6);
        horizontalLayout_28->setObjectName(QStringLiteral("horizontalLayout_28"));
        lineEdit_7 = new QLineEdit(page_cardlog);
        lineEdit_7->setObjectName(QStringLiteral("lineEdit_7"));
        lineEdit_7->setMinimumSize(QSize(0, 25));
        lineEdit_7->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_28->addWidget(lineEdit_7);

        btn_cardlog_search = new QPushButton(page_cardlog);
        btn_cardlog_search->setObjectName(QStringLiteral("btn_cardlog_search"));
        btn_cardlog_search->setMinimumSize(QSize(63, 30));
        btn_cardlog_search->setMaximumSize(QSize(63, 30));

        horizontalLayout_28->addWidget(btn_cardlog_search);


        verticalLayout_21->addLayout(horizontalLayout_28);

        widget_datetime = new QWidget(page_cardlog);
        widget_datetime->setObjectName(QStringLiteral("widget_datetime"));
        horizontalLayout_8 = new QHBoxLayout(widget_datetime);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        dateEdit_start = new QDateEdit(widget_datetime);
        dateEdit_start->setObjectName(QStringLiteral("dateEdit_start"));
        dateEdit_start->setMinimumSize(QSize(121, 33));
        dateEdit_start->setMaximumSize(QSize(100, 30));
        dateEdit_start->setStyleSheet(QStringLiteral(""));
        dateEdit_start->setCalendarPopup(true);

        horizontalLayout_8->addWidget(dateEdit_start);

        timeEdit_start = new QTimeEdit(widget_datetime);
        timeEdit_start->setObjectName(QStringLiteral("timeEdit_start"));
        timeEdit_start->setMinimumSize(QSize(121, 33));
        timeEdit_start->setMaximumSize(QSize(121, 33));

        horizontalLayout_8->addWidget(timeEdit_start);

        label_19 = new QLabel(widget_datetime);
        label_19->setObjectName(QStringLiteral("label_19"));

        horizontalLayout_8->addWidget(label_19);

        dateEdit_end = new QDateEdit(widget_datetime);
        dateEdit_end->setObjectName(QStringLiteral("dateEdit_end"));
        dateEdit_end->setMinimumSize(QSize(121, 33));
        dateEdit_end->setMaximumSize(QSize(121, 33));
        dateEdit_end->setCalendarPopup(true);

        horizontalLayout_8->addWidget(dateEdit_end);

        timeEdit_end = new QTimeEdit(widget_datetime);
        timeEdit_end->setObjectName(QStringLiteral("timeEdit_end"));
        timeEdit_end->setMinimumSize(QSize(121, 33));
        timeEdit_end->setMaximumSize(QSize(121, 33));

        horizontalLayout_8->addWidget(timeEdit_end);

        horizontalSpacer_16 = new QSpacerItem(553, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_16);


        verticalLayout_21->addWidget(widget_datetime);

        table_cardlog = new QTableWidget(page_cardlog);
        if (table_cardlog->columnCount() < 5)
            table_cardlog->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        table_cardlog->setHorizontalHeaderItem(0, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        table_cardlog->setHorizontalHeaderItem(1, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        table_cardlog->setHorizontalHeaderItem(2, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        table_cardlog->setHorizontalHeaderItem(3, __qtablewidgetitem23);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        table_cardlog->setHorizontalHeaderItem(4, __qtablewidgetitem24);
        table_cardlog->setObjectName(QStringLiteral("table_cardlog"));
        table_cardlog->verticalHeader()->setVisible(false);

        verticalLayout_21->addWidget(table_cardlog);

        stacked_pages->addWidget(page_cardlog);
        page_makeownercard = new QWidget();
        page_makeownercard->setObjectName(QStringLiteral("page_makeownercard"));
        verticalLayout_32 = new QVBoxLayout(page_makeownercard);
        verticalLayout_32->setSpacing(6);
        verticalLayout_32->setContentsMargins(11, 11, 11, 11);
        verticalLayout_32->setObjectName(QStringLiteral("verticalLayout_32"));
        verticalLayout_32->setContentsMargins(-1, 0, -1, 9);
        widget_makeownercard = new QWidget(page_makeownercard);
        widget_makeownercard->setObjectName(QStringLiteral("widget_makeownercard"));
        widget_makeownercard->setMinimumSize(QSize(0, 560));
        verticalLayout_2 = new QVBoxLayout(widget_makeownercard);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_39 = new QHBoxLayout();
        horizontalLayout_39->setSpacing(12);
        horizontalLayout_39->setObjectName(QStringLiteral("horizontalLayout_39"));
        horizontalLayout_38 = new QHBoxLayout();
        horizontalLayout_38->setSpacing(6);
        horizontalLayout_38->setObjectName(QStringLiteral("horizontalLayout_38"));
        label_30 = new QLabel(widget_makeownercard);
        label_30->setObjectName(QStringLiteral("label_30"));
        label_30->setMinimumSize(QSize(60, 25));
        label_30->setMaximumSize(QSize(60, 16777215));
        label_30->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_38->addWidget(label_30);

        line_business_name = new QLineEdit(widget_makeownercard);
        line_business_name->setObjectName(QStringLiteral("line_business_name"));
        sizePolicy3.setHeightForWidth(line_business_name->sizePolicy().hasHeightForWidth());
        line_business_name->setSizePolicy(sizePolicy3);
        line_business_name->setMinimumSize(QSize(114, 33));
        line_business_name->setMaximumSize(QSize(114, 33));
        line_business_name->setReadOnly(true);

        horizontalLayout_38->addWidget(line_business_name);

        btn_owner_list = new QPushButton(widget_makeownercard);
        btn_owner_list->setObjectName(QStringLiteral("btn_owner_list"));
        btn_owner_list->setMinimumSize(QSize(100, 32));
        btn_owner_list->setMaximumSize(QSize(100, 32));

        horizontalLayout_38->addWidget(btn_owner_list);


        horizontalLayout_39->addLayout(horizontalLayout_38);

        horizontalLayout_36 = new QHBoxLayout();
        horizontalLayout_36->setSpacing(6);
        horizontalLayout_36->setObjectName(QStringLiteral("horizontalLayout_36"));
        label_37 = new QLabel(widget_makeownercard);
        label_37->setObjectName(QStringLiteral("label_37"));
        label_37->setMinimumSize(QSize(60, 0));
        label_37->setMaximumSize(QSize(60, 16777215));
        label_37->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_36->addWidget(label_37);

        line_business_cardnum = new QLineEdit(widget_makeownercard);
        line_business_cardnum->setObjectName(QStringLiteral("line_business_cardnum"));
        sizePolicy3.setHeightForWidth(line_business_cardnum->sizePolicy().hasHeightForWidth());
        line_business_cardnum->setSizePolicy(sizePolicy3);
        line_business_cardnum->setMinimumSize(QSize(160, 33));
        line_business_cardnum->setMaximumSize(QSize(160, 33));
        line_business_cardnum->setReadOnly(true);

        horizontalLayout_36->addWidget(line_business_cardnum);


        horizontalLayout_39->addLayout(horizontalLayout_36);

        horizontalLayout_37 = new QHBoxLayout();
        horizontalLayout_37->setSpacing(6);
        horizontalLayout_37->setObjectName(QStringLiteral("horizontalLayout_37"));
        label_38 = new QLabel(widget_makeownercard);
        label_38->setObjectName(QStringLiteral("label_38"));
        label_38->setMinimumSize(QSize(60, 0));
        label_38->setMaximumSize(QSize(60, 16777215));
        label_38->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_37->addWidget(label_38);

        line_business_password = new QLineEdit(widget_makeownercard);
        line_business_password->setObjectName(QStringLiteral("line_business_password"));
        line_business_password->setMinimumSize(QSize(189, 33));
        line_business_password->setMaximumSize(QSize(189, 33));
        line_business_password->setEchoMode(QLineEdit::Normal);
        line_business_password->setReadOnly(true);

        horizontalLayout_37->addWidget(line_business_password);

        horizontalSpacer_20 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_37->addItem(horizontalSpacer_20);


        horizontalLayout_39->addLayout(horizontalLayout_37);


        verticalLayout_2->addLayout(horizontalLayout_39);

        group_card_fee = new QGroupBox(widget_makeownercard);
        group_card_fee->setObjectName(QStringLiteral("group_card_fee"));
        verticalLayout_22 = new QVBoxLayout(group_card_fee);
        verticalLayout_22->setSpacing(0);
        verticalLayout_22->setContentsMargins(11, 11, 11, 11);
        verticalLayout_22->setObjectName(QStringLiteral("verticalLayout_22"));
        verticalLayout_22->setContentsMargins(28, 0, 0, 0);
        horizontalLayout_41 = new QHBoxLayout();
        horizontalLayout_41->setSpacing(6);
        horizontalLayout_41->setObjectName(QStringLiteral("horizontalLayout_41"));
        horizontalLayout_41->setContentsMargins(0, 12, -1, 11);
        label_20 = new QLabel(group_card_fee);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setMinimumSize(QSize(60, 25));
        label_20->setMaximumSize(QSize(16777215, 25));
        label_20->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_41->addWidget(label_20);

        combo_cardfee_type = new QComboBox(group_card_fee);
        combo_cardfee_type->setObjectName(QStringLiteral("combo_cardfee_type"));
        sizePolicy6.setHeightForWidth(combo_cardfee_type->sizePolicy().hasHeightForWidth());
        combo_cardfee_type->setSizePolicy(sizePolicy6);
        combo_cardfee_type->setMinimumSize(QSize(0, 33));
        combo_cardfee_type->setMaximumSize(QSize(150, 33));

        horizontalLayout_41->addWidget(combo_cardfee_type);

        widget_chargestandard = new QWidget(group_card_fee);
        widget_chargestandard->setObjectName(QStringLiteral("widget_chargestandard"));
        widget_chargestandard->setMinimumSize(QSize(110, 40));
        horizontalLayout_68 = new QHBoxLayout(widget_chargestandard);
        horizontalLayout_68->setSpacing(0);
        horizontalLayout_68->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_68->setObjectName(QStringLiteral("horizontalLayout_68"));
        horizontalLayout_68->setContentsMargins(9, 0, 0, 0);
        label_21 = new QLabel(widget_chargestandard);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setMinimumSize(QSize(80, 25));
        label_21->setMaximumSize(QSize(80, 25));
        label_21->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_68->addWidget(label_21);

        lineEdit_chargestandard = new QLineEdit(widget_chargestandard);
        lineEdit_chargestandard->setObjectName(QStringLiteral("lineEdit_chargestandard"));
        sizePolicy5.setHeightForWidth(lineEdit_chargestandard->sizePolicy().hasHeightForWidth());
        lineEdit_chargestandard->setSizePolicy(sizePolicy5);
        lineEdit_chargestandard->setMinimumSize(QSize(0, 33));
        lineEdit_chargestandard->setMaximumSize(QSize(150, 33));

        horizontalLayout_68->addWidget(lineEdit_chargestandard);

        label_22 = new QLabel(widget_chargestandard);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setMinimumSize(QSize(0, 25));
        label_22->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_68->addWidget(label_22);

        label_22->raise();
        label_21->raise();
        lineEdit_chargestandard->raise();

        horizontalLayout_41->addWidget(widget_chargestandard);

        label_16 = new QLabel(group_card_fee);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setMinimumSize(QSize(60, 0));
        label_16->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_41->addWidget(label_16);

        lineEdit_freefee = new QLineEdit(group_card_fee);
        lineEdit_freefee->setObjectName(QStringLiteral("lineEdit_freefee"));
        sizePolicy5.setHeightForWidth(lineEdit_freefee->sizePolicy().hasHeightForWidth());
        lineEdit_freefee->setSizePolicy(sizePolicy5);
        lineEdit_freefee->setMinimumSize(QSize(115, 33));
        lineEdit_freefee->setMaximumSize(QSize(115, 33));

        horizontalLayout_41->addWidget(lineEdit_freefee);

        combox_recharge_value = new QComboBox(group_card_fee);
        combox_recharge_value->setObjectName(QStringLiteral("combox_recharge_value"));
        sizePolicy6.setHeightForWidth(combox_recharge_value->sizePolicy().hasHeightForWidth());
        combox_recharge_value->setSizePolicy(sizePolicy6);
        combox_recharge_value->setMinimumSize(QSize(115, 33));
        combox_recharge_value->setMaximumSize(QSize(115, 33));

        horizontalLayout_41->addWidget(combox_recharge_value);

        btn_user_recharge = new QPushButton(group_card_fee);
        btn_user_recharge->setObjectName(QStringLiteral("btn_user_recharge"));
        btn_user_recharge->setMinimumSize(QSize(65, 30));
        btn_user_recharge->setMaximumSize(QSize(65, 30));

        horizontalLayout_41->addWidget(btn_user_recharge);

        horizontalSpacer_22 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_41->addItem(horizontalSpacer_22);


        verticalLayout_22->addLayout(horizontalLayout_41);

        widget_card_value = new QWidget(group_card_fee);
        widget_card_value->setObjectName(QStringLiteral("widget_card_value"));
        widget_card_value->setMinimumSize(QSize(100, 40));
        verticalLayout_47 = new QVBoxLayout(widget_card_value);
        verticalLayout_47->setSpacing(0);
        verticalLayout_47->setContentsMargins(11, 11, 11, 11);
        verticalLayout_47->setObjectName(QStringLiteral("verticalLayout_47"));
        verticalLayout_47->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_27 = new QHBoxLayout();
        horizontalLayout_27->setSpacing(6);
        horizontalLayout_27->setObjectName(QStringLiteral("horizontalLayout_27"));
        horizontalLayout_27->setContentsMargins(0, 12, -1, 11);
        label_3 = new QLabel(widget_card_value);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMinimumSize(QSize(15, 0));

        horizontalLayout_27->addWidget(label_3);

        label_43 = new QLabel(widget_card_value);
        label_43->setObjectName(QStringLiteral("label_43"));

        horizontalLayout_27->addWidget(label_43);

        combox_floor_num = new QComboBox(widget_card_value);
        combox_floor_num->setObjectName(QStringLiteral("combox_floor_num"));
        QSizePolicy sizePolicy7(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(combox_floor_num->sizePolicy().hasHeightForWidth());
        combox_floor_num->setSizePolicy(sizePolicy7);
        combox_floor_num->setMinimumSize(QSize(100, 0));
        combox_floor_num->setMaximumSize(QSize(16777215, 33));

        horizontalLayout_27->addWidget(combox_floor_num);

        horizontalSpacer_27 = new QSpacerItem(80, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_27->addItem(horizontalSpacer_27);

        btn_saveFloor = new QPushButton(widget_card_value);
        btn_saveFloor->setObjectName(QStringLiteral("btn_saveFloor"));
        QSizePolicy sizePolicy8(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(btn_saveFloor->sizePolicy().hasHeightForWidth());
        btn_saveFloor->setSizePolicy(sizePolicy8);
        btn_saveFloor->setMinimumSize(QSize(65, 30));

        horizontalLayout_27->addWidget(btn_saveFloor);

        horizontalSpacer_53 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_27->addItem(horizontalSpacer_53);


        verticalLayout_47->addLayout(horizontalLayout_27);


        verticalLayout_22->addWidget(widget_card_value);


        verticalLayout_2->addWidget(group_card_fee);

        widget_validFloors = new QWidget(widget_makeownercard);
        widget_validFloors->setObjectName(QStringLiteral("widget_validFloors"));
        sizePolicy5.setHeightForWidth(widget_validFloors->sizePolicy().hasHeightForWidth());
        widget_validFloors->setSizePolicy(sizePolicy5);
        horizontalLayout_29 = new QHBoxLayout(widget_validFloors);
        horizontalLayout_29->setSpacing(6);
        horizontalLayout_29->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_29->setObjectName(QStringLiteral("horizontalLayout_29"));
        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        label_44 = new QLabel(widget_validFloors);
        label_44->setObjectName(QStringLiteral("label_44"));
        QSizePolicy sizePolicy9(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy9.setHorizontalStretch(0);
        sizePolicy9.setVerticalStretch(0);
        sizePolicy9.setHeightForWidth(label_44->sizePolicy().hasHeightForWidth());
        label_44->setSizePolicy(sizePolicy9);

        verticalLayout_11->addWidget(label_44);

        verticalSpacer_8 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_11->addItem(verticalSpacer_8);


        horizontalLayout_13->addLayout(verticalLayout_11);

        scrollArea_4 = new QScrollArea(widget_validFloors);
        scrollArea_4->setObjectName(QStringLiteral("scrollArea_4"));
        sizePolicy5.setHeightForWidth(scrollArea_4->sizePolicy().hasHeightForWidth());
        scrollArea_4->setSizePolicy(sizePolicy5);
        scrollArea_4->setWidgetResizable(true);
        widget_validfloors = new QWidget();
        widget_validfloors->setObjectName(QStringLiteral("widget_validfloors"));
        widget_validfloors->setGeometry(QRect(0, 0, 990, 151));
        scrollArea_4->setWidget(widget_validfloors);

        horizontalLayout_13->addWidget(scrollArea_4);


        horizontalLayout_29->addLayout(horizontalLayout_13);


        verticalLayout_2->addWidget(widget_validFloors);

        group_card_time = new QGroupBox(widget_makeownercard);
        group_card_time->setObjectName(QStringLiteral("group_card_time"));
        verticalLayout_18 = new QVBoxLayout(group_card_time);
        verticalLayout_18->setSpacing(0);
        verticalLayout_18->setContentsMargins(11, 11, 11, 11);
        verticalLayout_18->setObjectName(QStringLiteral("verticalLayout_18"));
        verticalLayout_18->setContentsMargins(28, 0, 0, 0);
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(-1, 12, -1, 11);
        comboBox_response_time = new QComboBox(group_card_time);
        comboBox_response_time->setObjectName(QStringLiteral("comboBox_response_time"));
        sizePolicy5.setHeightForWidth(comboBox_response_time->sizePolicy().hasHeightForWidth());
        comboBox_response_time->setSizePolicy(sizePolicy5);
        comboBox_response_time->setMinimumSize(QSize(0, 33));
        comboBox_response_time->setMaximumSize(QSize(150, 33));

        horizontalLayout_10->addWidget(comboBox_response_time);

        label_28 = new QLabel(group_card_time);
        label_28->setObjectName(QStringLiteral("label_28"));
        label_28->setMinimumSize(QSize(80, 25));
        label_28->setMaximumSize(QSize(80, 25));
        label_28->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_10->addWidget(label_28);

        horizontalSpacer_35 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_35);

        comboBox_calltype = new QComboBox(group_card_time);
        comboBox_calltype->setObjectName(QStringLiteral("comboBox_calltype"));
        sizePolicy5.setHeightForWidth(comboBox_calltype->sizePolicy().hasHeightForWidth());
        comboBox_calltype->setSizePolicy(sizePolicy5);
        comboBox_calltype->setMinimumSize(QSize(0, 33));
        comboBox_calltype->setMaximumSize(QSize(150, 33));

        horizontalLayout_10->addWidget(comboBox_calltype);

        label_29 = new QLabel(group_card_time);
        label_29->setObjectName(QStringLiteral("label_29"));
        label_29->setMinimumSize(QSize(60, 25));
        label_29->setMaximumSize(QSize(60, 25));
        label_29->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_10->addWidget(label_29);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_13);

        label_45 = new QLabel(group_card_time);
        label_45->setObjectName(QStringLiteral("label_45"));
        label_45->setMinimumSize(QSize(80, 25));
        label_45->setMaximumSize(QSize(80, 25));
        label_45->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_10->addWidget(label_45);

        date_card_start = new QDateEdit(group_card_time);
        date_card_start->setObjectName(QStringLiteral("date_card_start"));
        sizePolicy5.setHeightForWidth(date_card_start->sizePolicy().hasHeightForWidth());
        date_card_start->setSizePolicy(sizePolicy5);
        date_card_start->setMinimumSize(QSize(121, 33));
        date_card_start->setMaximumSize(QSize(121, 33));
        date_card_start->setMinimumDate(QDate(2000, 1, 1));
        date_card_start->setCalendarPopup(true);

        horizontalLayout_10->addWidget(date_card_start);

        label_27 = new QLabel(group_card_time);
        label_27->setObjectName(QStringLiteral("label_27"));
        label_27->setMinimumSize(QSize(0, 25));
        label_27->setMaximumSize(QSize(16777215, 25));
        label_27->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_10->addWidget(label_27);

        date_card_end = new QDateEdit(group_card_time);
        date_card_end->setObjectName(QStringLiteral("date_card_end"));
        sizePolicy5.setHeightForWidth(date_card_end->sizePolicy().hasHeightForWidth());
        date_card_end->setSizePolicy(sizePolicy5);
        date_card_end->setMinimumSize(QSize(121, 33));
        date_card_end->setMaximumSize(QSize(121, 33));
        date_card_end->setMinimumDate(QDate(2000, 1, 1));
        date_card_end->setCalendarPopup(true);

        horizontalLayout_10->addWidget(date_card_end);

        horizontalSpacer_36 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_36);


        verticalLayout_18->addLayout(horizontalLayout_10);

        horizontalLayout_70 = new QHBoxLayout();
        horizontalLayout_70->setSpacing(6);
        horizontalLayout_70->setObjectName(QStringLiteral("horizontalLayout_70"));
        horizontalLayout_70->setContentsMargins(-1, 0, -1, -1);
        checkBox_available_time = new QCheckBox(group_card_time);
        checkBox_available_time->setObjectName(QStringLiteral("checkBox_available_time"));

        horizontalLayout_70->addWidget(checkBox_available_time);

        horizontalLayout_25 = new QHBoxLayout();
        horizontalLayout_25->setSpacing(6);
        horizontalLayout_25->setObjectName(QStringLiteral("horizontalLayout_25"));
        widget_available_time = new QWidget(group_card_time);
        widget_available_time->setObjectName(QStringLiteral("widget_available_time"));
        widget_available_time->setMinimumSize(QSize(100, 0));
        horizontalLayout_69 = new QHBoxLayout(widget_available_time);
        horizontalLayout_69->setSpacing(6);
        horizontalLayout_69->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_69->setObjectName(QStringLiteral("horizontalLayout_69"));
        horizontalLayout_69->setContentsMargins(0, 12, 0, 11);
        time_card_start = new QTimeEdit(widget_available_time);
        time_card_start->setObjectName(QStringLiteral("time_card_start"));
        sizePolicy5.setHeightForWidth(time_card_start->sizePolicy().hasHeightForWidth());
        time_card_start->setSizePolicy(sizePolicy5);
        time_card_start->setMinimumSize(QSize(121, 33));
        time_card_start->setMaximumSize(QSize(121, 33));
        time_card_start->setCalendarPopup(true);

        horizontalLayout_69->addWidget(time_card_start);

        label_40 = new QLabel(widget_available_time);
        label_40->setObjectName(QStringLiteral("label_40"));
        label_40->setMinimumSize(QSize(0, 25));
        label_40->setMaximumSize(QSize(16777215, 25));
        label_40->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_69->addWidget(label_40);

        time_card_end = new QTimeEdit(widget_available_time);
        time_card_end->setObjectName(QStringLiteral("time_card_end"));
        sizePolicy5.setHeightForWidth(time_card_end->sizePolicy().hasHeightForWidth());
        time_card_end->setSizePolicy(sizePolicy5);
        time_card_end->setMinimumSize(QSize(121, 33));
        time_card_end->setMaximumSize(QSize(121, 33));
        time_card_end->setCalendarPopup(true);

        horizontalLayout_69->addWidget(time_card_end);

        label_23 = new QLabel(widget_available_time);
        label_23->setObjectName(QStringLiteral("label_23"));
        label_23->setMinimumSize(QSize(50, 0));

        horizontalLayout_69->addWidget(label_23);

        check_card_week_1 = new QCheckBox(widget_available_time);
        check_card_week_1->setObjectName(QStringLiteral("check_card_week_1"));
        check_card_week_1->setMinimumSize(QSize(0, 25));
        check_card_week_1->setMaximumSize(QSize(16777215, 25));
        check_card_week_1->setTristate(false);

        horizontalLayout_69->addWidget(check_card_week_1);

        check_card_week_2 = new QCheckBox(widget_available_time);
        check_card_week_2->setObjectName(QStringLiteral("check_card_week_2"));
        check_card_week_2->setMinimumSize(QSize(0, 25));
        check_card_week_2->setMaximumSize(QSize(16777215, 25));
        check_card_week_2->setCheckable(true);

        horizontalLayout_69->addWidget(check_card_week_2);

        check_card_week_3 = new QCheckBox(widget_available_time);
        check_card_week_3->setObjectName(QStringLiteral("check_card_week_3"));
        check_card_week_3->setMinimumSize(QSize(0, 25));
        check_card_week_3->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_69->addWidget(check_card_week_3);

        check_card_week_4 = new QCheckBox(widget_available_time);
        check_card_week_4->setObjectName(QStringLiteral("check_card_week_4"));
        check_card_week_4->setMinimumSize(QSize(0, 25));
        check_card_week_4->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_69->addWidget(check_card_week_4);

        check_card_week_5 = new QCheckBox(widget_available_time);
        check_card_week_5->setObjectName(QStringLiteral("check_card_week_5"));
        check_card_week_5->setMinimumSize(QSize(0, 25));
        check_card_week_5->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_69->addWidget(check_card_week_5);

        check_card_week_7 = new QCheckBox(widget_available_time);
        check_card_week_7->setObjectName(QStringLiteral("check_card_week_7"));
        check_card_week_7->setMinimumSize(QSize(0, 25));
        check_card_week_7->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_69->addWidget(check_card_week_7);

        check_card_week_6 = new QCheckBox(widget_available_time);
        check_card_week_6->setObjectName(QStringLiteral("check_card_week_6"));
        check_card_week_6->setMinimumSize(QSize(0, 25));
        check_card_week_6->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_69->addWidget(check_card_week_6);


        horizontalLayout_25->addWidget(widget_available_time);

        horizontalSpacer_21 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_25->addItem(horizontalSpacer_21);


        horizontalLayout_70->addLayout(horizontalLayout_25);


        verticalLayout_18->addLayout(horizontalLayout_70);


        verticalLayout_2->addWidget(group_card_time);

        horizontalLayout_45 = new QHBoxLayout();
        horizontalLayout_45->setSpacing(2);
        horizontalLayout_45->setObjectName(QStringLiteral("horizontalLayout_45"));
        horizontalSpacer_23 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_45->addItem(horizontalSpacer_23);

        btn_makecard_ok = new QPushButton(widget_makeownercard);
        btn_makecard_ok->setObjectName(QStringLiteral("btn_makecard_ok"));
        btn_makecard_ok->setMinimumSize(QSize(63, 29));
        btn_makecard_ok->setMaximumSize(QSize(63, 29));

        horizontalLayout_45->addWidget(btn_makecard_ok);


        verticalLayout_2->addLayout(horizontalLayout_45);


        verticalLayout_32->addWidget(widget_makeownercard);

        stacked_pages->addWidget(page_makeownercard);
        page_makeadmincard = new QWidget();
        page_makeadmincard->setObjectName(QStringLiteral("page_makeadmincard"));
        verticalLayout_36 = new QVBoxLayout(page_makeadmincard);
        verticalLayout_36->setSpacing(6);
        verticalLayout_36->setContentsMargins(11, 11, 11, 11);
        verticalLayout_36->setObjectName(QStringLiteral("verticalLayout_36"));
        verticalLayout_36->setContentsMargins(-1, 0, -1, 9);
        widget_makeadmincard = new QWidget(page_makeadmincard);
        widget_makeadmincard->setObjectName(QStringLiteral("widget_makeadmincard"));
        verticalLayout_35 = new QVBoxLayout(widget_makeadmincard);
        verticalLayout_35->setSpacing(6);
        verticalLayout_35->setContentsMargins(11, 11, 11, 11);
        verticalLayout_35->setObjectName(QStringLiteral("verticalLayout_35"));
        horizontalLayout_31 = new QHBoxLayout();
        horizontalLayout_31->setSpacing(6);
        horizontalLayout_31->setObjectName(QStringLiteral("horizontalLayout_31"));
        horizontalLayout_92 = new QHBoxLayout();
        horizontalLayout_92->setSpacing(6);
        horizontalLayout_92->setObjectName(QStringLiteral("horizontalLayout_92"));
        label_59 = new QLabel(widget_makeadmincard);
        label_59->setObjectName(QStringLiteral("label_59"));
        label_59->setMinimumSize(QSize(0, 25));
        label_59->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_92->addWidget(label_59);

        line_makeadmin_name = new QLineEdit(widget_makeadmincard);
        line_makeadmin_name->setObjectName(QStringLiteral("line_makeadmin_name"));
        sizePolicy3.setHeightForWidth(line_makeadmin_name->sizePolicy().hasHeightForWidth());
        line_makeadmin_name->setSizePolicy(sizePolicy3);
        line_makeadmin_name->setMinimumSize(QSize(115, 33));
        line_makeadmin_name->setMaximumSize(QSize(115, 33));
        line_makeadmin_name->setReadOnly(true);

        horizontalLayout_92->addWidget(line_makeadmin_name);

        btn_admin_list = new QPushButton(widget_makeadmincard);
        btn_admin_list->setObjectName(QStringLiteral("btn_admin_list"));
        btn_admin_list->setMinimumSize(QSize(100, 32));
        btn_admin_list->setMaximumSize(QSize(100, 32));

        horizontalLayout_92->addWidget(btn_admin_list);


        horizontalLayout_31->addLayout(horizontalLayout_92);

        horizontalLayout_93 = new QHBoxLayout();
        horizontalLayout_93->setSpacing(6);
        horizontalLayout_93->setObjectName(QStringLiteral("horizontalLayout_93"));
        label_60 = new QLabel(widget_makeadmincard);
        label_60->setObjectName(QStringLiteral("label_60"));
        label_60->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_93->addWidget(label_60);

        line_makeadmin_cardnum = new QLineEdit(widget_makeadmincard);
        line_makeadmin_cardnum->setObjectName(QStringLiteral("line_makeadmin_cardnum"));
        sizePolicy3.setHeightForWidth(line_makeadmin_cardnum->sizePolicy().hasHeightForWidth());
        line_makeadmin_cardnum->setSizePolicy(sizePolicy3);
        line_makeadmin_cardnum->setMinimumSize(QSize(160, 33));
        line_makeadmin_cardnum->setMaximumSize(QSize(160, 33));
        line_makeadmin_cardnum->setReadOnly(true);

        horizontalLayout_93->addWidget(line_makeadmin_cardnum);


        horizontalLayout_31->addLayout(horizontalLayout_93);

        horizontalLayout_94 = new QHBoxLayout();
        horizontalLayout_94->setSpacing(6);
        horizontalLayout_94->setObjectName(QStringLiteral("horizontalLayout_94"));
        label_61 = new QLabel(widget_makeadmincard);
        label_61->setObjectName(QStringLiteral("label_61"));
        label_61->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_94->addWidget(label_61);

        line_makeadmin_password = new QLineEdit(widget_makeadmincard);
        line_makeadmin_password->setObjectName(QStringLiteral("line_makeadmin_password"));
        line_makeadmin_password->setMinimumSize(QSize(189, 33));
        line_makeadmin_password->setMaximumSize(QSize(189, 33));
        line_makeadmin_password->setEchoMode(QLineEdit::Normal);
        line_makeadmin_password->setReadOnly(true);

        horizontalLayout_94->addWidget(line_makeadmin_password);

        horizontalSpacer_52 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_94->addItem(horizontalSpacer_52);


        horizontalLayout_31->addLayout(horizontalLayout_94);


        verticalLayout_35->addLayout(horizontalLayout_31);

        groupBox_3 = new QGroupBox(widget_makeadmincard);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        verticalLayout_30 = new QVBoxLayout(groupBox_3);
        verticalLayout_30->setSpacing(6);
        verticalLayout_30->setContentsMargins(11, 11, 11, 11);
        verticalLayout_30->setObjectName(QStringLiteral("verticalLayout_30"));
        horizontalLayout_77 = new QHBoxLayout();
        horizontalLayout_77->setSpacing(6);
        horizontalLayout_77->setObjectName(QStringLiteral("horizontalLayout_77"));
        checkBox_admin_cardValidtime = new QCheckBox(groupBox_3);
        checkBox_admin_cardValidtime->setObjectName(QStringLiteral("checkBox_admin_cardValidtime"));
        checkBox_admin_cardValidtime->setCheckable(true);
        checkBox_admin_cardValidtime->setChecked(false);

        horizontalLayout_77->addWidget(checkBox_admin_cardValidtime);

        horizontalLayout_61 = new QHBoxLayout();
        horizontalLayout_61->setSpacing(0);
        horizontalLayout_61->setObjectName(QStringLiteral("horizontalLayout_61"));
        widget_admin_cardvalidtime = new QWidget(groupBox_3);
        widget_admin_cardvalidtime->setObjectName(QStringLiteral("widget_admin_cardvalidtime"));
        widget_admin_cardvalidtime->setMinimumSize(QSize(100, 30));
        horizontalLayout_78 = new QHBoxLayout(widget_admin_cardvalidtime);
        horizontalLayout_78->setSpacing(6);
        horizontalLayout_78->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_78->setObjectName(QStringLiteral("horizontalLayout_78"));
        horizontalLayout_78->setContentsMargins(0, 0, 0, 0);
        label_46 = new QLabel(widget_admin_cardvalidtime);
        label_46->setObjectName(QStringLiteral("label_46"));
        label_46->setMinimumSize(QSize(65, 25));
        label_46->setMaximumSize(QSize(65, 25));
        label_46->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_78->addWidget(label_46);

        date_admincard_start = new QDateEdit(widget_admin_cardvalidtime);
        date_admincard_start->setObjectName(QStringLiteral("date_admincard_start"));
        sizePolicy.setHeightForWidth(date_admincard_start->sizePolicy().hasHeightForWidth());
        date_admincard_start->setSizePolicy(sizePolicy);
        date_admincard_start->setMinimumSize(QSize(121, 33));
        date_admincard_start->setMaximumSize(QSize(121, 33));
        date_admincard_start->setMinimumDate(QDate(2000, 1, 1));
        date_admincard_start->setCalendarPopup(true);

        horizontalLayout_78->addWidget(date_admincard_start);

        label_47 = new QLabel(widget_admin_cardvalidtime);
        label_47->setObjectName(QStringLiteral("label_47"));
        label_47->setMinimumSize(QSize(0, 25));
        label_47->setMaximumSize(QSize(30, 25));
        label_47->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_78->addWidget(label_47);

        date_admincard_end = new QDateEdit(widget_admin_cardvalidtime);
        date_admincard_end->setObjectName(QStringLiteral("date_admincard_end"));
        sizePolicy.setHeightForWidth(date_admincard_end->sizePolicy().hasHeightForWidth());
        date_admincard_end->setSizePolicy(sizePolicy);
        date_admincard_end->setMinimumSize(QSize(121, 33));
        date_admincard_end->setMaximumSize(QSize(121, 33));
        date_admincard_end->setMinimumDate(QDate(2000, 1, 1));
        date_admincard_end->setCalendarPopup(true);

        horizontalLayout_78->addWidget(date_admincard_end);

        horizontalSpacer_44 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_78->addItem(horizontalSpacer_44);


        horizontalLayout_61->addWidget(widget_admin_cardvalidtime);


        horizontalLayout_77->addLayout(horizontalLayout_61);


        verticalLayout_30->addLayout(horizontalLayout_77);

        horizontalLayout_84 = new QHBoxLayout();
        horizontalLayout_84->setSpacing(6);
        horizontalLayout_84->setObjectName(QStringLiteral("horizontalLayout_84"));
        checkBox_admin_availabletime = new QCheckBox(groupBox_3);
        checkBox_admin_availabletime->setObjectName(QStringLiteral("checkBox_admin_availabletime"));
        sizePolicy3.setHeightForWidth(checkBox_admin_availabletime->sizePolicy().hasHeightForWidth());
        checkBox_admin_availabletime->setSizePolicy(sizePolicy3);

        horizontalLayout_84->addWidget(checkBox_admin_availabletime);

        horizontalLayout_88 = new QHBoxLayout();
        horizontalLayout_88->setSpacing(6);
        horizontalLayout_88->setObjectName(QStringLiteral("horizontalLayout_88"));
        widget_admin_availabletime = new QWidget(groupBox_3);
        widget_admin_availabletime->setObjectName(QStringLiteral("widget_admin_availabletime"));
        widget_admin_availabletime->setMinimumSize(QSize(100, 30));
        horizontalLayout_91 = new QHBoxLayout(widget_admin_availabletime);
        horizontalLayout_91->setSpacing(6);
        horizontalLayout_91->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_91->setObjectName(QStringLiteral("horizontalLayout_91"));
        horizontalLayout_91->setContentsMargins(0, 0, 0, 0);
        label_48 = new QLabel(widget_admin_availabletime);
        label_48->setObjectName(QStringLiteral("label_48"));
        label_48->setMinimumSize(QSize(65, 25));
        label_48->setMaximumSize(QSize(65, 25));
        label_48->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_91->addWidget(label_48);

        time_admincard_start = new QTimeEdit(widget_admin_availabletime);
        time_admincard_start->setObjectName(QStringLiteral("time_admincard_start"));
        sizePolicy.setHeightForWidth(time_admincard_start->sizePolicy().hasHeightForWidth());
        time_admincard_start->setSizePolicy(sizePolicy);
        time_admincard_start->setMinimumSize(QSize(121, 33));
        time_admincard_start->setMaximumSize(QSize(121, 33));
        time_admincard_start->setCalendarPopup(true);

        horizontalLayout_91->addWidget(time_admincard_start);

        label_49 = new QLabel(widget_admin_availabletime);
        label_49->setObjectName(QStringLiteral("label_49"));
        label_49->setMinimumSize(QSize(0, 25));
        label_49->setMaximumSize(QSize(16777215, 25));
        label_49->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_91->addWidget(label_49);

        time_admincard_end = new QTimeEdit(widget_admin_availabletime);
        time_admincard_end->setObjectName(QStringLiteral("time_admincard_end"));
        sizePolicy.setHeightForWidth(time_admincard_end->sizePolicy().hasHeightForWidth());
        time_admincard_end->setSizePolicy(sizePolicy);
        time_admincard_end->setMinimumSize(QSize(121, 33));
        time_admincard_end->setMaximumSize(QSize(121, 33));
        time_admincard_end->setCalendarPopup(true);

        horizontalLayout_91->addWidget(time_admincard_end);

        check_admincard_week_1 = new QCheckBox(widget_admin_availabletime);
        check_admincard_week_1->setObjectName(QStringLiteral("check_admincard_week_1"));
        check_admincard_week_1->setMinimumSize(QSize(0, 25));
        check_admincard_week_1->setMaximumSize(QSize(16777215, 25));
        check_admincard_week_1->setTristate(false);

        horizontalLayout_91->addWidget(check_admincard_week_1);

        check_admincard_week_2 = new QCheckBox(widget_admin_availabletime);
        check_admincard_week_2->setObjectName(QStringLiteral("check_admincard_week_2"));
        check_admincard_week_2->setMinimumSize(QSize(0, 25));
        check_admincard_week_2->setMaximumSize(QSize(16777215, 25));
        check_admincard_week_2->setCheckable(true);

        horizontalLayout_91->addWidget(check_admincard_week_2);

        check_admincard_week_3 = new QCheckBox(widget_admin_availabletime);
        check_admincard_week_3->setObjectName(QStringLiteral("check_admincard_week_3"));
        check_admincard_week_3->setMinimumSize(QSize(0, 25));
        check_admincard_week_3->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_91->addWidget(check_admincard_week_3);

        check_admincard_week_4 = new QCheckBox(widget_admin_availabletime);
        check_admincard_week_4->setObjectName(QStringLiteral("check_admincard_week_4"));
        check_admincard_week_4->setMinimumSize(QSize(0, 25));
        check_admincard_week_4->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_91->addWidget(check_admincard_week_4);

        check_admincard_week_5 = new QCheckBox(widget_admin_availabletime);
        check_admincard_week_5->setObjectName(QStringLiteral("check_admincard_week_5"));
        check_admincard_week_5->setMinimumSize(QSize(0, 25));
        check_admincard_week_5->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_91->addWidget(check_admincard_week_5);

        check_admincard_week_6 = new QCheckBox(widget_admin_availabletime);
        check_admincard_week_6->setObjectName(QStringLiteral("check_admincard_week_6"));
        check_admincard_week_6->setMinimumSize(QSize(0, 25));
        check_admincard_week_6->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_91->addWidget(check_admincard_week_6);

        check_admincard_week_7 = new QCheckBox(widget_admin_availabletime);
        check_admincard_week_7->setObjectName(QStringLiteral("check_admincard_week_7"));
        check_admincard_week_7->setMinimumSize(QSize(0, 25));
        check_admincard_week_7->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_91->addWidget(check_admincard_week_7);

        horizontalSpacer_51 = new QSpacerItem(199, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_91->addItem(horizontalSpacer_51);


        horizontalLayout_88->addWidget(widget_admin_availabletime);


        horizontalLayout_84->addLayout(horizontalLayout_88);


        verticalLayout_30->addLayout(horizontalLayout_84);


        verticalLayout_35->addWidget(groupBox_3);

        textEdit = new QTextEdit(widget_makeadmincard);
        textEdit->setObjectName(QStringLiteral("textEdit"));

        verticalLayout_35->addWidget(textEdit);

        verticalSpacer_9 = new QSpacerItem(17, 359, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_35->addItem(verticalSpacer_9);

        horizontalLayout_57 = new QHBoxLayout();
        horizontalLayout_57->setSpacing(2);
        horizontalLayout_57->setObjectName(QStringLiteral("horizontalLayout_57"));
        horizontalSpacer_38 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_57->addItem(horizontalSpacer_38);

        btn_makeadmincard_ok = new QPushButton(widget_makeadmincard);
        btn_makeadmincard_ok->setObjectName(QStringLiteral("btn_makeadmincard_ok"));
        btn_makeadmincard_ok->setMinimumSize(QSize(63, 29));
        btn_makeadmincard_ok->setMaximumSize(QSize(63, 29));

        horizontalLayout_57->addWidget(btn_makeadmincard_ok);


        verticalLayout_35->addLayout(horizontalLayout_57);


        verticalLayout_36->addWidget(widget_makeadmincard);

        stacked_pages->addWidget(page_makeadmincard);
        page_maketoolcard = new QWidget();
        page_maketoolcard->setObjectName(QStringLiteral("page_maketoolcard"));
        verticalLayout_16 = new QVBoxLayout(page_maketoolcard);
        verticalLayout_16->setSpacing(8);
        verticalLayout_16->setContentsMargins(11, 11, 11, 11);
        verticalLayout_16->setObjectName(QStringLiteral("verticalLayout_16"));
        verticalLayout_16->setContentsMargins(9, 0, -1, 9);
        widget_maketoolcard_1 = new QWidget(page_maketoolcard);
        widget_maketoolcard_1->setObjectName(QStringLiteral("widget_maketoolcard_1"));
        widget_maketoolcard_1->setMinimumSize(QSize(0, 200));
        widget_maketoolcard_1->setMaximumSize(QSize(16777215, 200));
        verticalLayout_15 = new QVBoxLayout(widget_maketoolcard_1);
        verticalLayout_15->setSpacing(20);
        verticalLayout_15->setContentsMargins(11, 11, 11, 11);
        verticalLayout_15->setObjectName(QStringLiteral("verticalLayout_15"));
        verticalLayout_15->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout_15->setContentsMargins(10, 0, -1, 20);
        horizontalLayout_83 = new QHBoxLayout();
        horizontalLayout_83->setSpacing(12);
        horizontalLayout_83->setObjectName(QStringLiteral("horizontalLayout_83"));
        horizontalLayout_85 = new QHBoxLayout();
        horizontalLayout_85->setSpacing(6);
        horizontalLayout_85->setObjectName(QStringLiteral("horizontalLayout_85"));
        label_111 = new QLabel(widget_maketoolcard_1);
        label_111->setObjectName(QStringLiteral("label_111"));
        label_111->setMinimumSize(QSize(45, 0));
        label_111->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_85->addWidget(label_111);

        line_maketoolcard_cardnum = new QLineEdit(widget_maketoolcard_1);
        line_maketoolcard_cardnum->setObjectName(QStringLiteral("line_maketoolcard_cardnum"));
        sizePolicy3.setHeightForWidth(line_maketoolcard_cardnum->sizePolicy().hasHeightForWidth());
        line_maketoolcard_cardnum->setSizePolicy(sizePolicy3);
        line_maketoolcard_cardnum->setMinimumSize(QSize(160, 33));
        line_maketoolcard_cardnum->setMaximumSize(QSize(160, 33));
        line_maketoolcard_cardnum->setReadOnly(true);

        horizontalLayout_85->addWidget(line_maketoolcard_cardnum);


        horizontalLayout_83->addLayout(horizontalLayout_85);

        horizontalLayout_86 = new QHBoxLayout();
        horizontalLayout_86->setSpacing(6);
        horizontalLayout_86->setObjectName(QStringLiteral("horizontalLayout_86"));
        label_112 = new QLabel(widget_maketoolcard_1);
        label_112->setObjectName(QStringLiteral("label_112"));
        label_112->setMinimumSize(QSize(40, 33));
        label_112->setMaximumSize(QSize(40, 33));
        label_112->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_86->addWidget(label_112);

        line_maketoolcard_password = new QLineEdit(widget_maketoolcard_1);
        line_maketoolcard_password->setObjectName(QStringLiteral("line_maketoolcard_password"));
        line_maketoolcard_password->setMinimumSize(QSize(160, 33));
        line_maketoolcard_password->setMaximumSize(QSize(160, 33));
        line_maketoolcard_password->setEchoMode(QLineEdit::Normal);
        line_maketoolcard_password->setReadOnly(true);

        horizontalLayout_86->addWidget(line_maketoolcard_password);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_86->addItem(horizontalSpacer_12);


        horizontalLayout_83->addLayout(horizontalLayout_86);


        verticalLayout_15->addLayout(horizontalLayout_83);

        groupBox_4 = new QGroupBox(widget_maketoolcard_1);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setMinimumSize(QSize(0, 105));
        groupBox_4->setMaximumSize(QSize(16777215, 105));
        horizontalLayout_40 = new QHBoxLayout(groupBox_4);
        horizontalLayout_40->setSpacing(6);
        horizontalLayout_40->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_40->setObjectName(QStringLiteral("horizontalLayout_40"));
        verticalLayout_20 = new QVBoxLayout();
        verticalLayout_20->setSpacing(6);
        verticalLayout_20->setObjectName(QStringLiteral("verticalLayout_20"));
        verticalSpacer_18 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_20->addItem(verticalSpacer_18);

        horizontalLayout_24 = new QHBoxLayout();
        horizontalLayout_24->setSpacing(6);
        horizontalLayout_24->setObjectName(QStringLiteral("horizontalLayout_24"));
        label_117 = new QLabel(groupBox_4);
        label_117->setObjectName(QStringLiteral("label_117"));
        label_117->setMinimumSize(QSize(60, 30));
        label_117->setMaximumSize(QSize(16777215, 30));
        label_117->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_24->addWidget(label_117);

        comboBox_toolcard_units = new QComboBox(groupBox_4);
        comboBox_toolcard_units->setObjectName(QStringLiteral("comboBox_toolcard_units"));
        comboBox_toolcard_units->setMinimumSize(QSize(121, 33));
        comboBox_toolcard_units->setMaximumSize(QSize(121, 33));

        horizontalLayout_24->addWidget(comboBox_toolcard_units);


        verticalLayout_20->addLayout(horizontalLayout_24);

        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_20->addItem(verticalSpacer_7);


        horizontalLayout_40->addLayout(verticalLayout_20);

        horizontalLayout_30 = new QHBoxLayout();
        horizontalLayout_30->setSpacing(6);
        horizontalLayout_30->setObjectName(QStringLiteral("horizontalLayout_30"));
        verticalLayout_28 = new QVBoxLayout();
        verticalLayout_28->setSpacing(6);
        verticalLayout_28->setObjectName(QStringLiteral("verticalLayout_28"));
        verticalSpacer_19 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_28->addItem(verticalSpacer_19);

        horizontalLayout_66 = new QHBoxLayout();
        horizontalLayout_66->setSpacing(6);
        horizontalLayout_66->setObjectName(QStringLiteral("horizontalLayout_66"));
        label_118 = new QLabel(groupBox_4);
        label_118->setObjectName(QStringLiteral("label_118"));
        label_118->setMinimumSize(QSize(70, 30));
        label_118->setMaximumSize(QSize(16777215, 30));
        label_118->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_66->addWidget(label_118);


        verticalLayout_28->addLayout(horizontalLayout_66);

        verticalSpacer_13 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_28->addItem(verticalSpacer_13);


        horizontalLayout_30->addLayout(verticalLayout_28);

        scrollArea_12 = new ScrollArea(groupBox_4);
        scrollArea_12->setObjectName(QStringLiteral("scrollArea_12"));
        scrollArea_12->setMinimumSize(QSize(300, 80));
        scrollArea_12->setMaximumSize(QSize(16777215, 80));
        scrollArea_12->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea_12->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea_12->setWidgetResizable(true);
        scroll_unitElevt = new QWidget();
        scroll_unitElevt->setObjectName(QStringLiteral("scroll_unitElevt"));
        scroll_unitElevt->setGeometry(QRect(0, 0, 1000, 78));
        scroll_unitElevt->setMinimumSize(QSize(1000, 0));
        scrollArea_12->setWidget(scroll_unitElevt);

        horizontalLayout_30->addWidget(scrollArea_12);


        horizontalLayout_40->addLayout(horizontalLayout_30);


        verticalLayout_15->addWidget(groupBox_4);


        verticalLayout_16->addWidget(widget_maketoolcard_1);

        widget_maketoolcard_2 = new QWidget(page_maketoolcard);
        widget_maketoolcard_2->setObjectName(QStringLiteral("widget_maketoolcard_2"));
        verticalLayout_13 = new QVBoxLayout(widget_maketoolcard_2);
        verticalLayout_13->setSpacing(6);
        verticalLayout_13->setContentsMargins(11, 11, 11, 11);
        verticalLayout_13->setObjectName(QStringLiteral("verticalLayout_13"));
        groupBox = new QGroupBox(widget_maketoolcard_2);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        horizontalLayout_20 = new QHBoxLayout(groupBox);
        horizontalLayout_20->setSpacing(6);
        horizontalLayout_20->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_20->setObjectName(QStringLiteral("horizontalLayout_20"));
        wid_toolcard_group = new QWidget(groupBox);
        wid_toolcard_group->setObjectName(QStringLiteral("wid_toolcard_group"));
        wid_toolcard_group->setMinimumSize(QSize(169, 0));
        wid_toolcard_group->setMaximumSize(QSize(169, 16777215));
        wid_toolcard_group->setStyleSheet(QStringLiteral(""));
        verticalLayout_49 = new QVBoxLayout(wid_toolcard_group);
        verticalLayout_49->setSpacing(0);
        verticalLayout_49->setContentsMargins(11, 11, 11, 11);
        verticalLayout_49->setObjectName(QStringLiteral("verticalLayout_49"));
        verticalLayout_49->setContentsMargins(0, 0, 0, 0);
        btn_maketoolcard_syntime = new QPushButton(wid_toolcard_group);
        btn_maketoolcard_syntime->setObjectName(QStringLiteral("btn_maketoolcard_syntime"));
        btn_maketoolcard_syntime->setMinimumSize(QSize(169, 29));
        btn_maketoolcard_syntime->setMaximumSize(QSize(169, 29));

        verticalLayout_49->addWidget(btn_maketoolcard_syntime);

        btn_maketoolcard_readcard = new QPushButton(wid_toolcard_group);
        btn_maketoolcard_readcard->setObjectName(QStringLiteral("btn_maketoolcard_readcard"));
        btn_maketoolcard_readcard->setMinimumSize(QSize(169, 29));
        btn_maketoolcard_readcard->setMaximumSize(QSize(169, 29));

        verticalLayout_49->addWidget(btn_maketoolcard_readcard);

        btn_maketoolcard_maskroom = new QPushButton(wid_toolcard_group);
        btn_maketoolcard_maskroom->setObjectName(QStringLiteral("btn_maketoolcard_maskroom"));
        btn_maketoolcard_maskroom->setMinimumSize(QSize(169, 29));
        btn_maketoolcard_maskroom->setMaximumSize(QSize(169, 29));

        verticalLayout_49->addWidget(btn_maketoolcard_maskroom);

        btn_maketoolcard_openfloor = new QPushButton(wid_toolcard_group);
        btn_maketoolcard_openfloor->setObjectName(QStringLiteral("btn_maketoolcard_openfloor"));
        btn_maketoolcard_openfloor->setMinimumSize(QSize(169, 29));
        btn_maketoolcard_openfloor->setMaximumSize(QSize(169, 29));

        verticalLayout_49->addWidget(btn_maketoolcard_openfloor);

        btn_maketoolcard_elevtimeallow = new QPushButton(wid_toolcard_group);
        btn_maketoolcard_elevtimeallow->setObjectName(QStringLiteral("btn_maketoolcard_elevtimeallow"));
        sizePolicy6.setHeightForWidth(btn_maketoolcard_elevtimeallow->sizePolicy().hasHeightForWidth());
        btn_maketoolcard_elevtimeallow->setSizePolicy(sizePolicy6);
        btn_maketoolcard_elevtimeallow->setMinimumSize(QSize(169, 29));
        btn_maketoolcard_elevtimeallow->setMaximumSize(QSize(169, 29));

        verticalLayout_49->addWidget(btn_maketoolcard_elevtimeallow);

        btn_maketoolcard_acticatecard = new QPushButton(wid_toolcard_group);
        btn_maketoolcard_acticatecard->setObjectName(QStringLiteral("btn_maketoolcard_acticatecard"));
        btn_maketoolcard_acticatecard->setMinimumSize(QSize(169, 29));
        btn_maketoolcard_acticatecard->setMaximumSize(QSize(169, 29));

        verticalLayout_49->addWidget(btn_maketoolcard_acticatecard);

        btn_maketoolcard_elevNum = new QPushButton(wid_toolcard_group);
        btn_maketoolcard_elevNum->setObjectName(QStringLiteral("btn_maketoolcard_elevNum"));
        btn_maketoolcard_elevNum->setMinimumSize(QSize(169, 29));

        verticalLayout_49->addWidget(btn_maketoolcard_elevNum);

        btn_maketoolcard_lostCart = new QPushButton(wid_toolcard_group);
        btn_maketoolcard_lostCart->setObjectName(QStringLiteral("btn_maketoolcard_lostCart"));
        btn_maketoolcard_lostCart->setMinimumSize(QSize(169, 29));

        verticalLayout_49->addWidget(btn_maketoolcard_lostCart);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_49->addItem(verticalSpacer_2);


        horizontalLayout_20->addWidget(wid_toolcard_group);

        stacked_maketoolcard = new QStackedWidget(groupBox);
        stacked_maketoolcard->setObjectName(QStringLiteral("stacked_maketoolcard"));
        sizePolicy2.setHeightForWidth(stacked_maketoolcard->sizePolicy().hasHeightForWidth());
        stacked_maketoolcard->setSizePolicy(sizePolicy2);
        page_maketoolcard_syntime = new QWidget();
        page_maketoolcard_syntime->setObjectName(QStringLiteral("page_maketoolcard_syntime"));
        verticalLayout_41 = new QVBoxLayout(page_maketoolcard_syntime);
        verticalLayout_41->setSpacing(6);
        verticalLayout_41->setContentsMargins(11, 11, 11, 11);
        verticalLayout_41->setObjectName(QStringLiteral("verticalLayout_41"));
        verticalLayout_41->setContentsMargins(0, 0, 0, 0);
        verticalLayout_40 = new QVBoxLayout();
        verticalLayout_40->setSpacing(2);
        verticalLayout_40->setObjectName(QStringLiteral("verticalLayout_40"));
        label_93 = new QLabel(page_maketoolcard_syntime);
        label_93->setObjectName(QStringLiteral("label_93"));
        label_93->setMinimumSize(QSize(0, 25));
        label_93->setMaximumSize(QSize(16777215, 25));

        verticalLayout_40->addWidget(label_93);

        horizontalLayout_60 = new QHBoxLayout();
        horizontalLayout_60->setSpacing(6);
        horizontalLayout_60->setObjectName(QStringLiteral("horizontalLayout_60"));
        dateEdit_syntime = new QDateEdit(page_maketoolcard_syntime);
        dateEdit_syntime->setObjectName(QStringLiteral("dateEdit_syntime"));
        dateEdit_syntime->setMinimumSize(QSize(121, 33));
        dateEdit_syntime->setMaximumSize(QSize(121, 33));
        dateEdit_syntime->setCalendarPopup(true);

        horizontalLayout_60->addWidget(dateEdit_syntime);

        timeEdit_syntime = new QTimeEdit(page_maketoolcard_syntime);
        timeEdit_syntime->setObjectName(QStringLiteral("timeEdit_syntime"));
        timeEdit_syntime->setMinimumSize(QSize(121, 33));
        timeEdit_syntime->setMaximumSize(QSize(121, 33));

        horizontalLayout_60->addWidget(timeEdit_syntime);

        horizontalSpacer_39 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_60->addItem(horizontalSpacer_39);


        verticalLayout_40->addLayout(horizontalLayout_60);


        verticalLayout_41->addLayout(verticalLayout_40);

        verticalSpacer_10 = new QSpacerItem(20, 96, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_41->addItem(verticalSpacer_10);

        label_99 = new QLabel(page_maketoolcard_syntime);
        label_99->setObjectName(QStringLiteral("label_99"));

        verticalLayout_41->addWidget(label_99);

        stacked_maketoolcard->addWidget(page_maketoolcard_syntime);
        page_maketoolcard_readcard = new QWidget();
        page_maketoolcard_readcard->setObjectName(QStringLiteral("page_maketoolcard_readcard"));
        verticalLayout_42 = new QVBoxLayout(page_maketoolcard_readcard);
        verticalLayout_42->setSpacing(6);
        verticalLayout_42->setContentsMargins(11, 11, 11, 11);
        verticalLayout_42->setObjectName(QStringLiteral("verticalLayout_42"));
        verticalLayout_42->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_79 = new QHBoxLayout();
        horizontalLayout_79->setSpacing(6);
        horizontalLayout_79->setObjectName(QStringLiteral("horizontalLayout_79"));
        label_124 = new QLabel(page_maketoolcard_readcard);
        label_124->setObjectName(QStringLiteral("label_124"));
        label_124->setMinimumSize(QSize(30, 25));
        label_124->setMaximumSize(QSize(30, 16777215));
        label_124->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_79->addWidget(label_124);

        line_readcard_selectowner = new QLineEdit(page_maketoolcard_readcard);
        line_readcard_selectowner->setObjectName(QStringLiteral("line_readcard_selectowner"));
        line_readcard_selectowner->setMinimumSize(QSize(160, 33));
        line_readcard_selectowner->setMaximumSize(QSize(160, 33));

        horizontalLayout_79->addWidget(line_readcard_selectowner);

        btn_readcard_selectowner = new QPushButton(page_maketoolcard_readcard);
        btn_readcard_selectowner->setObjectName(QStringLiteral("btn_readcard_selectowner"));
        btn_readcard_selectowner->setMinimumSize(QSize(100, 32));
        btn_readcard_selectowner->setMaximumSize(QSize(100, 32));

        horizontalLayout_79->addWidget(btn_readcard_selectowner);

        label_125 = new QLabel(page_maketoolcard_readcard);
        label_125->setObjectName(QStringLiteral("label_125"));
        label_125->setMinimumSize(QSize(80, 25));
        label_125->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_79->addWidget(label_125);

        combo_readcard_selectfloor = new QComboBox(page_maketoolcard_readcard);
        combo_readcard_selectfloor->setObjectName(QStringLiteral("combo_readcard_selectfloor"));
        sizePolicy3.setHeightForWidth(combo_readcard_selectfloor->sizePolicy().hasHeightForWidth());
        combo_readcard_selectfloor->setSizePolicy(sizePolicy3);
        combo_readcard_selectfloor->setMinimumSize(QSize(121, 33));
        combo_readcard_selectfloor->setMaximumSize(QSize(121, 33));

        horizontalLayout_79->addWidget(combo_readcard_selectfloor);

        horizontalSpacer_40 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_79->addItem(horizontalSpacer_40);


        verticalLayout_42->addLayout(horizontalLayout_79);

        horizontalLayout_80 = new QHBoxLayout();
        horizontalLayout_80->setSpacing(6);
        horizontalLayout_80->setObjectName(QStringLiteral("horizontalLayout_80"));
        label_100 = new QLabel(page_maketoolcard_readcard);
        label_100->setObjectName(QStringLiteral("label_100"));

        horizontalLayout_80->addWidget(label_100);

        dateEdit_readcard_start = new QDateEdit(page_maketoolcard_readcard);
        dateEdit_readcard_start->setObjectName(QStringLiteral("dateEdit_readcard_start"));
        dateEdit_readcard_start->setMinimumSize(QSize(121, 33));
        dateEdit_readcard_start->setMaximumSize(QSize(121, 33));
        dateEdit_readcard_start->setCalendarPopup(true);

        horizontalLayout_80->addWidget(dateEdit_readcard_start);

        timeEdit_readcard_start = new QTimeEdit(page_maketoolcard_readcard);
        timeEdit_readcard_start->setObjectName(QStringLiteral("timeEdit_readcard_start"));
        timeEdit_readcard_start->setMinimumSize(QSize(121, 33));
        timeEdit_readcard_start->setMaximumSize(QSize(121, 33));
        timeEdit_readcard_start->setMinimumTime(QTime(0, 0, 0));
        timeEdit_readcard_start->setCalendarPopup(false);

        horizontalLayout_80->addWidget(timeEdit_readcard_start);

        label_101 = new QLabel(page_maketoolcard_readcard);
        label_101->setObjectName(QStringLiteral("label_101"));
        label_101->setMaximumSize(QSize(30, 16777215));
        label_101->setAlignment(Qt::AlignCenter);

        horizontalLayout_80->addWidget(label_101);

        dateEdit_readcard_end = new QDateEdit(page_maketoolcard_readcard);
        dateEdit_readcard_end->setObjectName(QStringLiteral("dateEdit_readcard_end"));
        dateEdit_readcard_end->setMinimumSize(QSize(121, 33));
        dateEdit_readcard_end->setMaximumSize(QSize(121, 33));
        dateEdit_readcard_end->setCalendarPopup(true);

        horizontalLayout_80->addWidget(dateEdit_readcard_end);

        timeEdit_readcard_end = new QTimeEdit(page_maketoolcard_readcard);
        timeEdit_readcard_end->setObjectName(QStringLiteral("timeEdit_readcard_end"));
        timeEdit_readcard_end->setMinimumSize(QSize(121, 33));
        timeEdit_readcard_end->setMaximumSize(QSize(121, 33));

        horizontalLayout_80->addWidget(timeEdit_readcard_end);

        horizontalSpacer_41 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_80->addItem(horizontalSpacer_41);


        verticalLayout_42->addLayout(horizontalLayout_80);

        verticalSpacer_11 = new QSpacerItem(20, 114, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_42->addItem(verticalSpacer_11);

        stacked_maketoolcard->addWidget(page_maketoolcard_readcard);
        page_maketoolcard_maskroom = new QWidget();
        page_maketoolcard_maskroom->setObjectName(QStringLiteral("page_maketoolcard_maskroom"));
        verticalLayout_53 = new QVBoxLayout(page_maketoolcard_maskroom);
        verticalLayout_53->setSpacing(6);
        verticalLayout_53->setContentsMargins(11, 11, 11, 11);
        verticalLayout_53->setObjectName(QStringLiteral("verticalLayout_53"));
        verticalLayout_53->setContentsMargins(0, 0, 0, 0);
        label_102 = new QLabel(page_maketoolcard_maskroom);
        label_102->setObjectName(QStringLiteral("label_102"));

        verticalLayout_53->addWidget(label_102);

        tabWidget_rooms = new QTabWidget(page_maketoolcard_maskroom);
        tabWidget_rooms->setObjectName(QStringLiteral("tabWidget_rooms"));
        tabWidget_rooms->setDocumentMode(false);
        tabWidget_rooms->setTabsClosable(false);
        tabWidget_rooms->setMovable(false);
        tabWidget_rooms->setTabBarAutoHide(false);

        verticalLayout_53->addWidget(tabWidget_rooms);

        horizontalLayout_81 = new QHBoxLayout();
        horizontalLayout_81->setSpacing(6);
        horizontalLayout_81->setObjectName(QStringLiteral("horizontalLayout_81"));
        horizontalSpacer_42 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_81->addItem(horizontalSpacer_42);

        btn_maskelevt = new QPushButton(page_maketoolcard_maskroom);
        btn_maskelevt->setObjectName(QStringLiteral("btn_maskelevt"));
        btn_maskelevt->setMinimumSize(QSize(90, 25));
        btn_maskelevt->setMaximumSize(QSize(90, 25));

        horizontalLayout_81->addWidget(btn_maskelevt);

        btn_enablelevt = new QPushButton(page_maketoolcard_maskroom);
        btn_enablelevt->setObjectName(QStringLiteral("btn_enablelevt"));
        btn_enablelevt->setMinimumSize(QSize(90, 25));
        btn_enablelevt->setMaximumSize(QSize(90, 25));

        horizontalLayout_81->addWidget(btn_enablelevt);

        btn_maskfloor = new QPushButton(page_maketoolcard_maskroom);
        btn_maskfloor->setObjectName(QStringLiteral("btn_maskfloor"));
        btn_maskfloor->setMinimumSize(QSize(90, 0));
        btn_maskfloor->setMaximumSize(QSize(90, 25));

        horizontalLayout_81->addWidget(btn_maskfloor);

        btn_enablefloor = new QPushButton(page_maketoolcard_maskroom);
        btn_enablefloor->setObjectName(QStringLiteral("btn_enablefloor"));
        btn_enablefloor->setMinimumSize(QSize(90, 25));
        btn_enablefloor->setMaximumSize(QSize(90, 25));

        horizontalLayout_81->addWidget(btn_enablefloor);


        verticalLayout_53->addLayout(horizontalLayout_81);

        stacked_maketoolcard->addWidget(page_maketoolcard_maskroom);
        page_maketoolcard_openfloor = new QWidget();
        page_maketoolcard_openfloor->setObjectName(QStringLiteral("page_maketoolcard_openfloor"));
        verticalLayout_43 = new QVBoxLayout(page_maketoolcard_openfloor);
        verticalLayout_43->setSpacing(6);
        verticalLayout_43->setContentsMargins(11, 11, 11, 11);
        verticalLayout_43->setObjectName(QStringLiteral("verticalLayout_43"));
        verticalLayout_43->setContentsMargins(0, 0, 0, 0);
        verticalLayout_54 = new QVBoxLayout();
        verticalLayout_54->setSpacing(6);
        verticalLayout_54->setObjectName(QStringLiteral("verticalLayout_54"));
        label_104 = new QLabel(page_maketoolcard_openfloor);
        label_104->setObjectName(QStringLiteral("label_104"));
        label_104->setMinimumSize(QSize(0, 25));

        verticalLayout_54->addWidget(label_104);

        scrollArea_openfloor = new ScrollArea(page_maketoolcard_openfloor);
        scrollArea_openfloor->setObjectName(QStringLiteral("scrollArea_openfloor"));
        scrollArea_openfloor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea_openfloor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea_openfloor->setWidgetResizable(true);
        scrollContents_openfloor = new QWidget();
        scrollContents_openfloor->setObjectName(QStringLiteral("scrollContents_openfloor"));
        scrollContents_openfloor->setGeometry(QRect(0, 0, 1000, 16));
        scrollContents_openfloor->setMinimumSize(QSize(1000, 0));
        scrollArea_openfloor->setWidget(scrollContents_openfloor);

        verticalLayout_54->addWidget(scrollArea_openfloor);


        verticalLayout_43->addLayout(verticalLayout_54);

        horizontalLayout_82 = new QHBoxLayout();
        horizontalLayout_82->setSpacing(6);
        horizontalLayout_82->setObjectName(QStringLiteral("horizontalLayout_82"));
        horizontalSpacer_43 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_82->addItem(horizontalSpacer_43);


        verticalLayout_43->addLayout(horizontalLayout_82);

        stacked_maketoolcard->addWidget(page_maketoolcard_openfloor);
        page_maketoolcard_elevtimeallow = new QWidget();
        page_maketoolcard_elevtimeallow->setObjectName(QStringLiteral("page_maketoolcard_elevtimeallow"));
        verticalLayout_44 = new QVBoxLayout(page_maketoolcard_elevtimeallow);
        verticalLayout_44->setSpacing(6);
        verticalLayout_44->setContentsMargins(11, 11, 11, 11);
        verticalLayout_44->setObjectName(QStringLiteral("verticalLayout_44"));
        verticalLayout_44->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_62 = new QHBoxLayout();
        horizontalLayout_62->setSpacing(6);
        horizontalLayout_62->setObjectName(QStringLiteral("horizontalLayout_62"));
        checkBox_time1 = new QCheckBox(page_maketoolcard_elevtimeallow);
        checkBox_time1->setObjectName(QStringLiteral("checkBox_time1"));

        horizontalLayout_62->addWidget(checkBox_time1);

        widget_time1 = new QWidget(page_maketoolcard_elevtimeallow);
        widget_time1->setObjectName(QStringLiteral("widget_time1"));
        widget_time1->setMinimumSize(QSize(100, 0));
        horizontalLayout_16 = new QHBoxLayout(widget_time1);
        horizontalLayout_16->setSpacing(6);
        horizontalLayout_16->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));
        horizontalLayout_16->setContentsMargins(0, 0, 0, 0);
        label_106 = new QLabel(widget_time1);
        label_106->setObjectName(QStringLiteral("label_106"));
        label_106->setMinimumSize(QSize(50, 0));
        label_106->setMaximumSize(QSize(50, 16777215));
        label_106->setAlignment(Qt::AlignCenter);

        horizontalLayout_16->addWidget(label_106);

        timeEdit_timeallowstart1 = new QTimeEdit(widget_time1);
        timeEdit_timeallowstart1->setObjectName(QStringLiteral("timeEdit_timeallowstart1"));
        timeEdit_timeallowstart1->setMinimumSize(QSize(121, 33));
        timeEdit_timeallowstart1->setMaximumSize(QSize(121, 33));

        horizontalLayout_16->addWidget(timeEdit_timeallowstart1);

        label_25 = new QLabel(widget_time1);
        label_25->setObjectName(QStringLiteral("label_25"));

        horizontalLayout_16->addWidget(label_25);

        timeEdit_timeallowend1 = new QTimeEdit(widget_time1);
        timeEdit_timeallowend1->setObjectName(QStringLiteral("timeEdit_timeallowend1"));
        timeEdit_timeallowend1->setMinimumSize(QSize(121, 33));
        timeEdit_timeallowend1->setMaximumSize(QSize(121, 33));

        horizontalLayout_16->addWidget(timeEdit_timeallowend1);


        horizontalLayout_62->addWidget(widget_time1);

        horizontalSpacer_48 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_62->addItem(horizontalSpacer_48);


        verticalLayout_44->addLayout(horizontalLayout_62);

        horizontalLayout_64 = new QHBoxLayout();
        horizontalLayout_64->setSpacing(6);
        horizontalLayout_64->setObjectName(QStringLiteral("horizontalLayout_64"));
        checkBox_time2 = new QCheckBox(page_maketoolcard_elevtimeallow);
        checkBox_time2->setObjectName(QStringLiteral("checkBox_time2"));

        horizontalLayout_64->addWidget(checkBox_time2);

        widget_time2 = new QWidget(page_maketoolcard_elevtimeallow);
        widget_time2->setObjectName(QStringLiteral("widget_time2"));
        widget_time2->setMinimumSize(QSize(100, 0));
        horizontalLayout_75 = new QHBoxLayout(widget_time2);
        horizontalLayout_75->setSpacing(6);
        horizontalLayout_75->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_75->setObjectName(QStringLiteral("horizontalLayout_75"));
        horizontalLayout_75->setContentsMargins(0, 0, 0, 0);
        label_107 = new QLabel(widget_time2);
        label_107->setObjectName(QStringLiteral("label_107"));
        label_107->setMinimumSize(QSize(50, 0));
        label_107->setMaximumSize(QSize(50, 16777215));
        label_107->setAlignment(Qt::AlignCenter);

        horizontalLayout_75->addWidget(label_107);

        timeEdit_timeallowstart2 = new QTimeEdit(widget_time2);
        timeEdit_timeallowstart2->setObjectName(QStringLiteral("timeEdit_timeallowstart2"));
        timeEdit_timeallowstart2->setMinimumSize(QSize(121, 33));
        timeEdit_timeallowstart2->setMaximumSize(QSize(121, 33));

        horizontalLayout_75->addWidget(timeEdit_timeallowstart2);

        label_31 = new QLabel(widget_time2);
        label_31->setObjectName(QStringLiteral("label_31"));

        horizontalLayout_75->addWidget(label_31);

        timeEdit_timeallowend2 = new QTimeEdit(widget_time2);
        timeEdit_timeallowend2->setObjectName(QStringLiteral("timeEdit_timeallowend2"));
        timeEdit_timeallowend2->setMinimumSize(QSize(121, 33));
        timeEdit_timeallowend2->setMaximumSize(QSize(121, 33));

        horizontalLayout_75->addWidget(timeEdit_timeallowend2);


        horizontalLayout_64->addWidget(widget_time2);

        horizontalSpacer_49 = new QSpacerItem(128, 27, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_64->addItem(horizontalSpacer_49);


        verticalLayout_44->addLayout(horizontalLayout_64);

        horizontalLayout_65 = new QHBoxLayout();
        horizontalLayout_65->setSpacing(6);
        horizontalLayout_65->setObjectName(QStringLiteral("horizontalLayout_65"));
        checkBox_time3 = new QCheckBox(page_maketoolcard_elevtimeallow);
        checkBox_time3->setObjectName(QStringLiteral("checkBox_time3"));

        horizontalLayout_65->addWidget(checkBox_time3);

        widget_time3 = new QWidget(page_maketoolcard_elevtimeallow);
        widget_time3->setObjectName(QStringLiteral("widget_time3"));
        widget_time3->setMinimumSize(QSize(100, 0));
        horizontalLayout_76 = new QHBoxLayout(widget_time3);
        horizontalLayout_76->setSpacing(6);
        horizontalLayout_76->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_76->setObjectName(QStringLiteral("horizontalLayout_76"));
        horizontalLayout_76->setContentsMargins(0, 0, 0, 0);
        label_108 = new QLabel(widget_time3);
        label_108->setObjectName(QStringLiteral("label_108"));
        label_108->setMinimumSize(QSize(50, 0));
        label_108->setMaximumSize(QSize(50, 16777215));
        label_108->setAlignment(Qt::AlignCenter);

        horizontalLayout_76->addWidget(label_108);

        timeEdit_timeallowstart3 = new QTimeEdit(widget_time3);
        timeEdit_timeallowstart3->setObjectName(QStringLiteral("timeEdit_timeallowstart3"));
        timeEdit_timeallowstart3->setMinimumSize(QSize(121, 33));
        timeEdit_timeallowstart3->setMaximumSize(QSize(121, 33));

        horizontalLayout_76->addWidget(timeEdit_timeallowstart3);

        label_32 = new QLabel(widget_time3);
        label_32->setObjectName(QStringLiteral("label_32"));

        horizontalLayout_76->addWidget(label_32);

        timeEdit_timeallowend3 = new QTimeEdit(widget_time3);
        timeEdit_timeallowend3->setObjectName(QStringLiteral("timeEdit_timeallowend3"));
        timeEdit_timeallowend3->setMinimumSize(QSize(121, 33));
        timeEdit_timeallowend3->setMaximumSize(QSize(121, 33));

        horizontalLayout_76->addWidget(timeEdit_timeallowend3);


        horizontalLayout_65->addWidget(widget_time3);

        horizontalSpacer_50 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_65->addItem(horizontalSpacer_50);


        verticalLayout_44->addLayout(horizontalLayout_65);

        horizontalLayout_63 = new QHBoxLayout();
        horizontalLayout_63->setSpacing(6);
        horizontalLayout_63->setObjectName(QStringLiteral("horizontalLayout_63"));
        check_week_1 = new QCheckBox(page_maketoolcard_elevtimeallow);
        check_week_1->setObjectName(QStringLiteral("check_week_1"));

        horizontalLayout_63->addWidget(check_week_1);

        check_week_2 = new QCheckBox(page_maketoolcard_elevtimeallow);
        check_week_2->setObjectName(QStringLiteral("check_week_2"));

        horizontalLayout_63->addWidget(check_week_2);

        check_week_3 = new QCheckBox(page_maketoolcard_elevtimeallow);
        check_week_3->setObjectName(QStringLiteral("check_week_3"));

        horizontalLayout_63->addWidget(check_week_3);

        check_week_4 = new QCheckBox(page_maketoolcard_elevtimeallow);
        check_week_4->setObjectName(QStringLiteral("check_week_4"));

        horizontalLayout_63->addWidget(check_week_4);

        check_week_5 = new QCheckBox(page_maketoolcard_elevtimeallow);
        check_week_5->setObjectName(QStringLiteral("check_week_5"));

        horizontalLayout_63->addWidget(check_week_5);

        check_week_6 = new QCheckBox(page_maketoolcard_elevtimeallow);
        check_week_6->setObjectName(QStringLiteral("check_week_6"));

        horizontalLayout_63->addWidget(check_week_6);

        check_week_7 = new QCheckBox(page_maketoolcard_elevtimeallow);
        check_week_7->setObjectName(QStringLiteral("check_week_7"));

        horizontalLayout_63->addWidget(check_week_7);

        horizontalSpacer_47 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_63->addItem(horizontalSpacer_47);


        verticalLayout_44->addLayout(horizontalLayout_63);

        verticalSpacer_12 = new QSpacerItem(20, 48, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_44->addItem(verticalSpacer_12);

        label_105 = new QLabel(page_maketoolcard_elevtimeallow);
        label_105->setObjectName(QStringLiteral("label_105"));

        verticalLayout_44->addWidget(label_105);

        stacked_maketoolcard->addWidget(page_maketoolcard_elevtimeallow);
        page_maketoolcard_activatevCard = new QWidget();
        page_maketoolcard_activatevCard->setObjectName(QStringLiteral("page_maketoolcard_activatevCard"));
        activate_card = new QCheckBox(page_maketoolcard_activatevCard);
        activate_card->setObjectName(QStringLiteral("activate_card"));
        activate_card->setGeometry(QRect(70, 10, 141, 61));
        label_15 = new QLabel(page_maketoolcard_activatevCard);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(70, 80, 391, 51));
        stacked_maketoolcard->addWidget(page_maketoolcard_activatevCard);
        page_maketoolcard_elevatorNnum = new QWidget();
        page_maketoolcard_elevatorNnum->setObjectName(QStringLiteral("page_maketoolcard_elevatorNnum"));
        label_24 = new QLabel(page_maketoolcard_elevatorNnum);
        label_24->setObjectName(QStringLiteral("label_24"));
        label_24->setGeometry(QRect(10, 10, 421, 41));
        stacked_maketoolcard->addWidget(page_maketoolcard_elevatorNnum);
        page_maketoolcard_lostCard = new QWidget();
        page_maketoolcard_lostCard->setObjectName(QStringLiteral("page_maketoolcard_lostCard"));
        horizontalLayoutWidget = new QWidget(page_maketoolcard_lostCard);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 20, 381, 41));
        horizontalLayout_73 = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout_73->setSpacing(6);
        horizontalLayout_73->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_73->setObjectName(QStringLiteral("horizontalLayout_73"));
        horizontalLayout_73->setContentsMargins(0, 0, 0, 0);
        label_39 = new QLabel(horizontalLayoutWidget);
        label_39->setObjectName(QStringLiteral("label_39"));

        horizontalLayout_73->addWidget(label_39);

        line_LostCardNum = new QLineEdit(horizontalLayoutWidget);
        line_LostCardNum->setObjectName(QStringLiteral("line_LostCardNum"));
        line_LostCardNum->setMinimumSize(QSize(160, 33));

        horizontalLayout_73->addWidget(line_LostCardNum);

        btn_GetLostCardNum = new QPushButton(horizontalLayoutWidget);
        btn_GetLostCardNum->setObjectName(QStringLiteral("btn_GetLostCardNum"));
        btn_GetLostCardNum->setMinimumSize(QSize(100, 32));

        horizontalLayout_73->addWidget(btn_GetLostCardNum);

        stacked_maketoolcard->addWidget(page_maketoolcard_lostCard);

        horizontalLayout_20->addWidget(stacked_maketoolcard);


        verticalLayout_13->addWidget(groupBox);

        horizontalLayout_47 = new QHBoxLayout();
        horizontalLayout_47->setSpacing(2);
        horizontalLayout_47->setObjectName(QStringLiteral("horizontalLayout_47"));
        horizontalLayout_47->setContentsMargins(-1, -1, 9, -1);
        horizontalSpacer_29 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_47->addItem(horizontalSpacer_29);

        btn_toolcard_ok = new QPushButton(widget_maketoolcard_2);
        btn_toolcard_ok->setObjectName(QStringLiteral("btn_toolcard_ok"));
        btn_toolcard_ok->setMinimumSize(QSize(63, 30));
        btn_toolcard_ok->setMaximumSize(QSize(63, 30));

        horizontalLayout_47->addWidget(btn_toolcard_ok);


        verticalLayout_13->addLayout(horizontalLayout_47);


        verticalLayout_16->addWidget(widget_maketoolcard_2);

        stacked_pages->addWidget(page_maketoolcard);
        page_losscard = new QWidget();
        page_losscard->setObjectName(QStringLiteral("page_losscard"));
        verticalLayout_45 = new QVBoxLayout(page_losscard);
        verticalLayout_45->setSpacing(6);
        verticalLayout_45->setContentsMargins(11, 11, 11, 11);
        verticalLayout_45->setObjectName(QStringLiteral("verticalLayout_45"));
        widget_losscard = new QWidget(page_losscard);
        widget_losscard->setObjectName(QStringLiteral("widget_losscard"));
        verticalLayout_39 = new QVBoxLayout(widget_losscard);
        verticalLayout_39->setSpacing(6);
        verticalLayout_39->setContentsMargins(11, 11, 11, 11);
        verticalLayout_39->setObjectName(QStringLiteral("verticalLayout_39"));
        horizontalLayout_87 = new QHBoxLayout();
        horizontalLayout_87->setSpacing(12);
        horizontalLayout_87->setObjectName(QStringLiteral("horizontalLayout_87"));
        horizontalLayout_89 = new QHBoxLayout();
        horizontalLayout_89->setSpacing(6);
        horizontalLayout_89->setObjectName(QStringLiteral("horizontalLayout_89"));
        label_114 = new QLabel(widget_losscard);
        label_114->setObjectName(QStringLiteral("label_114"));
        label_114->setMinimumSize(QSize(40, 0));
        label_114->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_89->addWidget(label_114);

        line_cardloss_cardnum = new QLineEdit(widget_losscard);
        line_cardloss_cardnum->setObjectName(QStringLiteral("line_cardloss_cardnum"));
        sizePolicy3.setHeightForWidth(line_cardloss_cardnum->sizePolicy().hasHeightForWidth());
        line_cardloss_cardnum->setSizePolicy(sizePolicy3);
        line_cardloss_cardnum->setMinimumSize(QSize(160, 33));
        line_cardloss_cardnum->setMaximumSize(QSize(160, 33));
        line_cardloss_cardnum->setReadOnly(true);

        horizontalLayout_89->addWidget(line_cardloss_cardnum);


        horizontalLayout_87->addLayout(horizontalLayout_89);

        horizontalLayout_90 = new QHBoxLayout();
        horizontalLayout_90->setSpacing(6);
        horizontalLayout_90->setObjectName(QStringLiteral("horizontalLayout_90"));
        label_115 = new QLabel(widget_losscard);
        label_115->setObjectName(QStringLiteral("label_115"));
        label_115->setMinimumSize(QSize(50, 0));
        label_115->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_90->addWidget(label_115);

        line_cardloss_password = new QLineEdit(widget_losscard);
        line_cardloss_password->setObjectName(QStringLiteral("line_cardloss_password"));
        line_cardloss_password->setMinimumSize(QSize(189, 33));
        line_cardloss_password->setMaximumSize(QSize(189, 33));
        line_cardloss_password->setEchoMode(QLineEdit::Normal);
        line_cardloss_password->setReadOnly(true);

        horizontalLayout_90->addWidget(line_cardloss_password);

        horizontalSpacer_45 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_90->addItem(horizontalSpacer_45);


        horizontalLayout_87->addLayout(horizontalLayout_90);

        horizontalSpacer_54 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_87->addItem(horizontalSpacer_54);


        verticalLayout_39->addLayout(horizontalLayout_87);

        horizontalLayout_52 = new QHBoxLayout();
        horizontalLayout_52->setSpacing(6);
        horizontalLayout_52->setObjectName(QStringLiteral("horizontalLayout_52"));
        horizontalLayout_58 = new QHBoxLayout();
        horizontalLayout_58->setSpacing(6);
        horizontalLayout_58->setObjectName(QStringLiteral("horizontalLayout_58"));
        label_5 = new QLabel(widget_losscard);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setMinimumSize(QSize(40, 0));
        label_5->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_58->addWidget(label_5);

        lineEdit_Elevator_number = new QLineEdit(widget_losscard);
        lineEdit_Elevator_number->setObjectName(QStringLiteral("lineEdit_Elevator_number"));
        lineEdit_Elevator_number->setMinimumSize(QSize(160, 33));
        lineEdit_Elevator_number->setMaximumSize(QSize(160, 33));
        lineEdit_Elevator_number->setReadOnly(true);

        horizontalLayout_58->addWidget(lineEdit_Elevator_number);

        btn_elector_list = new QPushButton(widget_losscard);
        btn_elector_list->setObjectName(QStringLiteral("btn_elector_list"));
        btn_elector_list->setEnabled(true);
        btn_elector_list->setMinimumSize(QSize(100, 32));
        btn_elector_list->setMaximumSize(QSize(100, 32));
        btn_elector_list->setSizeIncrement(QSize(0, 0));

        horizontalLayout_58->addWidget(btn_elector_list);


        horizontalLayout_52->addLayout(horizontalLayout_58);

        horizontalLayout_59 = new QHBoxLayout();
        horizontalLayout_59->setSpacing(6);
        horizontalLayout_59->setObjectName(QStringLiteral("horizontalLayout_59"));
        label_6 = new QLabel(widget_losscard);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setMinimumSize(QSize(50, 33));
        label_6->setMaximumSize(QSize(50, 33));

        horizontalLayout_59->addWidget(label_6);

        lineEdit_card_number = new QLineEdit(widget_losscard);
        lineEdit_card_number->setObjectName(QStringLiteral("lineEdit_card_number"));
        lineEdit_card_number->setMinimumSize(QSize(160, 33));
        lineEdit_card_number->setMaximumSize(QSize(160, 33));
        lineEdit_card_number->setMouseTracking(true);
        lineEdit_card_number->setReadOnly(true);
        lineEdit_card_number->setCursorMoveStyle(Qt::LogicalMoveStyle);

        horizontalLayout_59->addWidget(lineEdit_card_number);

        btn_card_list = new QPushButton(widget_losscard);
        btn_card_list->setObjectName(QStringLiteral("btn_card_list"));
        btn_card_list->setMinimumSize(QSize(100, 32));
        btn_card_list->setMaximumSize(QSize(100, 32));

        horizontalLayout_59->addWidget(btn_card_list);

        horizontalSpacer_37 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_59->addItem(horizontalSpacer_37);


        horizontalLayout_52->addLayout(horizontalLayout_59);

        horizontalSpacer_28 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_52->addItem(horizontalSpacer_28);

        horizontalSpacer_26 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_52->addItem(horizontalSpacer_26);


        verticalLayout_39->addLayout(horizontalLayout_52);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_39->addItem(verticalSpacer_5);

        horizontalLayout_48 = new QHBoxLayout();
        horizontalLayout_48->setSpacing(2);
        horizontalLayout_48->setObjectName(QStringLiteral("horizontalLayout_48"));
        horizontalSpacer_46 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_48->addItem(horizontalSpacer_46);

        btn_cardlist_ok_4 = new QPushButton(widget_losscard);
        btn_cardlist_ok_4->setObjectName(QStringLiteral("btn_cardlist_ok_4"));
        btn_cardlist_ok_4->setMinimumSize(QSize(100, 32));
        btn_cardlist_ok_4->setMaximumSize(QSize(100, 32));

        horizontalLayout_48->addWidget(btn_cardlist_ok_4);

        btn_cardlist_cancel_4 = new QPushButton(widget_losscard);
        btn_cardlist_cancel_4->setObjectName(QStringLiteral("btn_cardlist_cancel_4"));
        btn_cardlist_cancel_4->setMinimumSize(QSize(100, 32));
        btn_cardlist_cancel_4->setMaximumSize(QSize(100, 32));

        horizontalLayout_48->addWidget(btn_cardlist_cancel_4);


        verticalLayout_39->addLayout(horizontalLayout_48);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_39->addItem(verticalSpacer_3);


        verticalLayout_45->addWidget(widget_losscard);

        stacked_pages->addWidget(page_losscard);
        page_remakecard = new QWidget();
        page_remakecard->setObjectName(QStringLiteral("page_remakecard"));
        horizontalLayout_71 = new QHBoxLayout(page_remakecard);
        horizontalLayout_71->setSpacing(0);
        horizontalLayout_71->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_71->setObjectName(QStringLiteral("horizontalLayout_71"));
        horizontalLayout_71->setContentsMargins(0, 0, 0, 0);
        widget_remakecard = new QWidget(page_remakecard);
        widget_remakecard->setObjectName(QStringLiteral("widget_remakecard"));
        horizontalLayout_72 = new QHBoxLayout(widget_remakecard);
        horizontalLayout_72->setSpacing(6);
        horizontalLayout_72->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_72->setObjectName(QStringLiteral("horizontalLayout_72"));
        btn_owner_replacecard = new QPushButton(widget_remakecard);
        btn_owner_replacecard->setObjectName(QStringLiteral("btn_owner_replacecard"));
        btn_owner_replacecard->setMinimumSize(QSize(100, 33));
        btn_owner_replacecard->setMaximumSize(QSize(100, 33));
        btn_owner_replacecard->setStyleSheet(QString::fromUtf8("QPushButton#btn_owner_replacecard::pressed{\n"
"	font: large \"\345\256\213\344\275\223\"; font-size:10pt;\n"
"	color:white;\n"
" 	border: 1px solid rgba(127, 153, 189, 255); \n"
"    border-radius: 3px;\n"
"    background-color:  qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, \n"
"					stop:0 rgba(156, 219, 255, 255), stop:0.495 rgba(118, 202, 255, 255), stop:0.505 rgba(51, 172, 255, 255),stop:1 rgba(43, 170, 255, 255));\n"
"}"));

        horizontalLayout_72->addWidget(btn_owner_replacecard);

        btn_tools_replacecard = new QPushButton(widget_remakecard);
        btn_tools_replacecard->setObjectName(QStringLiteral("btn_tools_replacecard"));
        btn_tools_replacecard->setMinimumSize(QSize(100, 30));
        btn_tools_replacecard->setMaximumSize(QSize(100, 30));

        horizontalLayout_72->addWidget(btn_tools_replacecard);


        horizontalLayout_71->addWidget(widget_remakecard);

        stacked_pages->addWidget(page_remakecard);
        page_cellinfo = new QWidget();
        page_cellinfo->setObjectName(QStringLiteral("page_cellinfo"));
        verticalLayout_27 = new QVBoxLayout(page_cellinfo);
        verticalLayout_27->setSpacing(6);
        verticalLayout_27->setContentsMargins(11, 11, 11, 11);
        verticalLayout_27->setObjectName(QStringLiteral("verticalLayout_27"));
        verticalLayout_27->setContentsMargins(-1, 0, -1, 9);
        table_cellinfo = new QTableWidget(page_cellinfo);
        if (table_cellinfo->columnCount() < 3)
            table_cellinfo->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        table_cellinfo->setHorizontalHeaderItem(0, __qtablewidgetitem25);
        QTableWidgetItem *__qtablewidgetitem26 = new QTableWidgetItem();
        table_cellinfo->setHorizontalHeaderItem(1, __qtablewidgetitem26);
        QTableWidgetItem *__qtablewidgetitem27 = new QTableWidgetItem();
        table_cellinfo->setHorizontalHeaderItem(2, __qtablewidgetitem27);
        table_cellinfo->setObjectName(QStringLiteral("table_cellinfo"));
        table_cellinfo->verticalHeader()->setVisible(false);

        verticalLayout_27->addWidget(table_cellinfo);

        widget_3 = new QWidget(page_cellinfo);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        widget_3->setMinimumSize(QSize(0, 100));
        horizontalLayout_9 = new QHBoxLayout(widget_3);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        verticalLayout_25 = new QVBoxLayout();
        verticalLayout_25->setSpacing(6);
        verticalLayout_25->setObjectName(QStringLiteral("verticalLayout_25"));
        horizontalLayout_34 = new QHBoxLayout();
        horizontalLayout_34->setSpacing(6);
        horizontalLayout_34->setObjectName(QStringLiteral("horizontalLayout_34"));
        label_tip_3 = new QLabel(widget_3);
        label_tip_3->setObjectName(QStringLiteral("label_tip_3"));
        label_tip_3->setMinimumSize(QSize(112, 30));
        label_tip_3->setMaximumSize(QSize(112, 30));
        label_tip_3->setAlignment(Qt::AlignCenter);

        horizontalLayout_34->addWidget(label_tip_3);

        btn_cell_del = new QPushButton(widget_3);
        btn_cell_del->setObjectName(QStringLiteral("btn_cell_del"));
        btn_cell_del->setMinimumSize(QSize(91, 30));
        btn_cell_del->setMaximumSize(QSize(91, 30));
        btn_cell_del->setCheckable(true);

        horizontalLayout_34->addWidget(btn_cell_del);

        horizontalSpacer_17 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_34->addItem(horizontalSpacer_17);


        verticalLayout_25->addLayout(horizontalLayout_34);

        group_cell_content = new QGroupBox(widget_3);
        group_cell_content->setObjectName(QStringLiteral("group_cell_content"));
        verticalLayout_24 = new QVBoxLayout(group_cell_content);
        verticalLayout_24->setSpacing(6);
        verticalLayout_24->setContentsMargins(11, 11, 11, 11);
        verticalLayout_24->setObjectName(QStringLiteral("verticalLayout_24"));
        horizontalLayout_32 = new QHBoxLayout();
        horizontalLayout_32->setSpacing(6);
        horizontalLayout_32->setObjectName(QStringLiteral("horizontalLayout_32"));
        check_addAuto = new QCheckBox(group_cell_content);
        check_addAuto->setObjectName(QStringLiteral("check_addAuto"));

        horizontalLayout_32->addWidget(check_addAuto);

        line_cellunit = new QLineEdit(group_cell_content);
        line_cellunit->setObjectName(QStringLiteral("line_cellunit"));
        sizePolicy3.setHeightForWidth(line_cellunit->sizePolicy().hasHeightForWidth());
        line_cellunit->setSizePolicy(sizePolicy3);
        line_cellunit->setMinimumSize(QSize(0, 25));

        horizontalLayout_32->addWidget(line_cellunit);

        label_11 = new QLabel(group_cell_content);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setMinimumSize(QSize(40, 0));

        horizontalLayout_32->addWidget(label_11);

        line_celllayer = new QLineEdit(group_cell_content);
        line_celllayer->setObjectName(QStringLiteral("line_celllayer"));
        sizePolicy3.setHeightForWidth(line_celllayer->sizePolicy().hasHeightForWidth());
        line_celllayer->setSizePolicy(sizePolicy3);
        line_celllayer->setMinimumSize(QSize(0, 25));

        horizontalLayout_32->addWidget(line_celllayer);

        label_13 = new QLabel(group_cell_content);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setMinimumSize(QSize(40, 0));

        horizontalLayout_32->addWidget(label_13);

        line_cellfamily = new QLineEdit(group_cell_content);
        line_cellfamily->setObjectName(QStringLiteral("line_cellfamily"));
        sizePolicy3.setHeightForWidth(line_cellfamily->sizePolicy().hasHeightForWidth());
        line_cellfamily->setSizePolicy(sizePolicy3);
        line_cellfamily->setMinimumSize(QSize(0, 25));

        horizontalLayout_32->addWidget(line_cellfamily);

        label_14 = new QLabel(group_cell_content);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setMinimumSize(QSize(40, 0));

        horizontalLayout_32->addWidget(label_14);

        horizontalSpacer_19 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_32->addItem(horizontalSpacer_19);


        verticalLayout_24->addLayout(horizontalLayout_32);


        verticalLayout_25->addWidget(group_cell_content);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_25->addItem(verticalSpacer);

        horizontalLayout_33 = new QHBoxLayout();
        horizontalLayout_33->setSpacing(6);
        horizontalLayout_33->setObjectName(QStringLiteral("horizontalLayout_33"));
        horizontalSpacer_18 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_33->addItem(horizontalSpacer_18);

        btn_cellinfo_ok = new QPushButton(widget_3);
        btn_cellinfo_ok->setObjectName(QStringLiteral("btn_cellinfo_ok"));
        btn_cellinfo_ok->setMinimumSize(QSize(63, 29));
        btn_cellinfo_ok->setMaximumSize(QSize(63, 29));

        horizontalLayout_33->addWidget(btn_cellinfo_ok);

        btn_cellinfo_cancel = new QPushButton(widget_3);
        btn_cellinfo_cancel->setObjectName(QStringLiteral("btn_cellinfo_cancel"));
        btn_cellinfo_cancel->setMinimumSize(QSize(63, 29));
        btn_cellinfo_cancel->setMaximumSize(QSize(63, 29));

        horizontalLayout_33->addWidget(btn_cellinfo_cancel);


        verticalLayout_25->addLayout(horizontalLayout_33);


        horizontalLayout_9->addLayout(verticalLayout_25);


        verticalLayout_27->addWidget(widget_3);

        stacked_pages->addWidget(page_cellinfo);
        page_elevator = new QWidget();
        page_elevator->setObjectName(QStringLiteral("page_elevator"));
        verticalLayout_19 = new QVBoxLayout(page_elevator);
        verticalLayout_19->setSpacing(6);
        verticalLayout_19->setContentsMargins(11, 11, 11, 11);
        verticalLayout_19->setObjectName(QStringLiteral("verticalLayout_19"));
        verticalLayout_19->setContentsMargins(-1, 0, -1, 9);
        horizontalLayout_26 = new QHBoxLayout();
        horizontalLayout_26->setSpacing(6);
        horizontalLayout_26->setObjectName(QStringLiteral("horizontalLayout_26"));
        lineEdit_elevator_search = new QLineEdit(page_elevator);
        lineEdit_elevator_search->setObjectName(QStringLiteral("lineEdit_elevator_search"));
        lineEdit_elevator_search->setMinimumSize(QSize(0, 25));
        lineEdit_elevator_search->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_26->addWidget(lineEdit_elevator_search);

        btn_elevator_search = new QPushButton(page_elevator);
        btn_elevator_search->setObjectName(QStringLiteral("btn_elevator_search"));
        btn_elevator_search->setMinimumSize(QSize(63, 30));
        btn_elevator_search->setMaximumSize(QSize(63, 30));

        horizontalLayout_26->addWidget(btn_elevator_search);


        verticalLayout_19->addLayout(horizontalLayout_26);

        table_elevator = new QTableWidget(page_elevator);
        if (table_elevator->columnCount() < 3)
            table_elevator->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem28 = new QTableWidgetItem();
        table_elevator->setHorizontalHeaderItem(0, __qtablewidgetitem28);
        QTableWidgetItem *__qtablewidgetitem29 = new QTableWidgetItem();
        table_elevator->setHorizontalHeaderItem(1, __qtablewidgetitem29);
        QTableWidgetItem *__qtablewidgetitem30 = new QTableWidgetItem();
        table_elevator->setHorizontalHeaderItem(2, __qtablewidgetitem30);
        table_elevator->setObjectName(QStringLiteral("table_elevator"));
        table_elevator->verticalHeader()->setVisible(false);

        verticalLayout_19->addWidget(table_elevator);

        wid_elevator = new QWidget(page_elevator);
        wid_elevator->setObjectName(QStringLiteral("wid_elevator"));
        sizePolicy2.setHeightForWidth(wid_elevator->sizePolicy().hasHeightForWidth());
        wid_elevator->setSizePolicy(sizePolicy2);
        verticalLayout_31 = new QVBoxLayout(wid_elevator);
        verticalLayout_31->setSpacing(6);
        verticalLayout_31->setContentsMargins(11, 11, 11, 11);
        verticalLayout_31->setObjectName(QStringLiteral("verticalLayout_31"));
        horizontalLayout_42 = new QHBoxLayout();
        horizontalLayout_42->setSpacing(6);
        horizontalLayout_42->setObjectName(QStringLiteral("horizontalLayout_42"));
        label_tip_4 = new QLabel(wid_elevator);
        label_tip_4->setObjectName(QStringLiteral("label_tip_4"));
        label_tip_4->setMinimumSize(QSize(112, 30));
        label_tip_4->setMaximumSize(QSize(112, 30));
        label_tip_4->setAlignment(Qt::AlignCenter);

        horizontalLayout_42->addWidget(label_tip_4);

        btn_dellift = new QPushButton(wid_elevator);
        btn_dellift->setObjectName(QStringLiteral("btn_dellift"));
        btn_dellift->setMinimumSize(QSize(91, 30));
        btn_dellift->setMaximumSize(QSize(91, 30));
        btn_dellift->setCheckable(true);

        horizontalLayout_42->addWidget(btn_dellift);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_42->addItem(horizontalSpacer_2);


        verticalLayout_31->addLayout(horizontalLayout_42);

        group_elevator_content = new QGroupBox(wid_elevator);
        group_elevator_content->setObjectName(QStringLiteral("group_elevator_content"));
        verticalLayout_26 = new QVBoxLayout(group_elevator_content);
        verticalLayout_26->setSpacing(6);
        verticalLayout_26->setContentsMargins(11, 11, 11, 11);
        verticalLayout_26->setObjectName(QStringLiteral("verticalLayout_26"));
        horizontalLayout_43 = new QHBoxLayout();
        horizontalLayout_43->setSpacing(6);
        horizontalLayout_43->setObjectName(QStringLiteral("horizontalLayout_43"));
        label_33 = new QLabel(group_elevator_content);
        label_33->setObjectName(QStringLiteral("label_33"));
        label_33->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_43->addWidget(label_33);

        combo_unitnum = new QComboBox(group_elevator_content);
        combo_unitnum->setObjectName(QStringLiteral("combo_unitnum"));
        combo_unitnum->setMinimumSize(QSize(71, 23));
        combo_unitnum->setMaximumSize(QSize(71, 23));

        horizontalLayout_43->addWidget(combo_unitnum);

        label_52 = new QLabel(group_elevator_content);
        label_52->setObjectName(QStringLiteral("label_52"));
        label_52->setMinimumSize(QSize(80, 0));
        label_52->setMaximumSize(QSize(80, 16777215));
        label_52->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_43->addWidget(label_52);

        line_elevnum = new QLineEdit(group_elevator_content);
        line_elevnum->setObjectName(QStringLiteral("line_elevnum"));
        line_elevnum->setMinimumSize(QSize(115, 22));
        line_elevnum->setMaximumSize(QSize(115, 22));

        horizontalLayout_43->addWidget(line_elevnum);

        label_4 = new QLabel(group_elevator_content);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setMinimumSize(QSize(120, 0));
        label_4->setMaximumSize(QSize(120, 16777215));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_43->addWidget(label_4);

        line_ctrl_layernum = new QLineEdit(group_elevator_content);
        line_ctrl_layernum->setObjectName(QStringLiteral("line_ctrl_layernum"));
        line_ctrl_layernum->setMinimumSize(QSize(115, 22));
        line_ctrl_layernum->setMaximumSize(QSize(115, 22));

        horizontalLayout_43->addWidget(line_ctrl_layernum);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_43->addItem(horizontalSpacer_4);


        verticalLayout_26->addLayout(horizontalLayout_43);

        label_tip_5 = new QLabel(group_elevator_content);
        label_tip_5->setObjectName(QStringLiteral("label_tip_5"));
        label_tip_5->setMinimumSize(QSize(0, 25));

        verticalLayout_26->addWidget(label_tip_5);

        splitter_3 = new QSplitter(group_elevator_content);
        splitter_3->setObjectName(QStringLiteral("splitter_3"));
        splitter_3->setOrientation(Qt::Horizontal);
        table_map = new QTableWidget(splitter_3);
        if (table_map->columnCount() < 3)
            table_map->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem31 = new QTableWidgetItem();
        table_map->setHorizontalHeaderItem(0, __qtablewidgetitem31);
        QTableWidgetItem *__qtablewidgetitem32 = new QTableWidgetItem();
        table_map->setHorizontalHeaderItem(1, __qtablewidgetitem32);
        QTableWidgetItem *__qtablewidgetitem33 = new QTableWidgetItem();
        table_map->setHorizontalHeaderItem(2, __qtablewidgetitem33);
        table_map->setObjectName(QStringLiteral("table_map"));
        splitter_3->addWidget(table_map);
        table_map->verticalHeader()->setVisible(false);
        widget_map = new QWidget(splitter_3);
        widget_map->setObjectName(QStringLiteral("widget_map"));
        verticalLayout_23 = new QVBoxLayout(widget_map);
        verticalLayout_23->setSpacing(6);
        verticalLayout_23->setContentsMargins(11, 11, 11, 11);
        verticalLayout_23->setObjectName(QStringLiteral("verticalLayout_23"));
        horizontalLayout_54 = new QHBoxLayout();
        horizontalLayout_54->setSpacing(6);
        horizontalLayout_54->setObjectName(QStringLiteral("horizontalLayout_54"));
        label_53 = new QLabel(widget_map);
        label_53->setObjectName(QStringLiteral("label_53"));
        label_53->setMinimumSize(QSize(80, 0));
        label_53->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_54->addWidget(label_53);

        line_actualLayer = new QLineEdit(widget_map);
        line_actualLayer->setObjectName(QStringLiteral("line_actualLayer"));
        line_actualLayer->setMinimumSize(QSize(0, 30));

        horizontalLayout_54->addWidget(line_actualLayer);

        horizontalSpacer_31 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_54->addItem(horizontalSpacer_31);


        verticalLayout_23->addLayout(horizontalLayout_54);

        horizontalLayout_55 = new QHBoxLayout();
        horizontalLayout_55->setSpacing(6);
        horizontalLayout_55->setObjectName(QStringLiteral("horizontalLayout_55"));
        label_56 = new QLabel(widget_map);
        label_56->setObjectName(QStringLiteral("label_56"));
        label_56->setMinimumSize(QSize(80, 0));
        label_56->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_55->addWidget(label_56);

        line_mapLayer = new QLineEdit(widget_map);
        line_mapLayer->setObjectName(QStringLiteral("line_mapLayer"));
        line_mapLayer->setMinimumSize(QSize(0, 30));

        horizontalLayout_55->addWidget(line_mapLayer);

        horizontalSpacer_32 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_55->addItem(horizontalSpacer_32);


        verticalLayout_23->addLayout(horizontalLayout_55);

        horizontalLayout_56 = new QHBoxLayout();
        horizontalLayout_56->setSpacing(6);
        horizontalLayout_56->setObjectName(QStringLiteral("horizontalLayout_56"));
        label_57 = new QLabel(widget_map);
        label_57->setObjectName(QStringLiteral("label_57"));
        label_57->setMinimumSize(QSize(80, 0));
        label_57->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_56->addWidget(label_57);

        line_roomlist = new QLineEdit(widget_map);
        line_roomlist->setObjectName(QStringLiteral("line_roomlist"));
        line_roomlist->setMinimumSize(QSize(0, 30));

        horizontalLayout_56->addWidget(line_roomlist);

        checkBox_4 = new QCheckBox(widget_map);
        checkBox_4->setObjectName(QStringLiteral("checkBox_4"));

        horizontalLayout_56->addWidget(checkBox_4);

        label_17 = new QLabel(widget_map);
        label_17->setObjectName(QStringLiteral("label_17"));

        horizontalLayout_56->addWidget(label_17);

        horizontalSpacer_33 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_56->addItem(horizontalSpacer_33);


        verticalLayout_23->addLayout(horizontalLayout_56);

        verticalSpacer_4 = new QSpacerItem(20, 36, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_23->addItem(verticalSpacer_4);

        horizontalLayout_49 = new QHBoxLayout();
        horizontalLayout_49->setSpacing(6);
        horizontalLayout_49->setObjectName(QStringLiteral("horizontalLayout_49"));
        btn_addliftmap = new QPushButton(widget_map);
        btn_addliftmap->setObjectName(QStringLiteral("btn_addliftmap"));
        btn_addliftmap->setMinimumSize(QSize(93, 30));
        btn_addliftmap->setMaximumSize(QSize(93, 30));

        horizontalLayout_49->addWidget(btn_addliftmap);

        btn_delliftmap = new QPushButton(widget_map);
        btn_delliftmap->setObjectName(QStringLiteral("btn_delliftmap"));
        btn_delliftmap->setMinimumSize(QSize(93, 30));
        btn_delliftmap->setMaximumSize(QSize(93, 30));

        horizontalLayout_49->addWidget(btn_delliftmap);

        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_49->addItem(horizontalSpacer_14);


        verticalLayout_23->addLayout(horizontalLayout_49);

        splitter_3->addWidget(widget_map);

        verticalLayout_26->addWidget(splitter_3);


        verticalLayout_31->addWidget(group_elevator_content);

        horizontalLayout_53 = new QHBoxLayout();
        horizontalLayout_53->setSpacing(6);
        horizontalLayout_53->setObjectName(QStringLiteral("horizontalLayout_53"));
        horizontalSpacer_30 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_53->addItem(horizontalSpacer_30);

        btn_elevt_ok = new QPushButton(wid_elevator);
        btn_elevt_ok->setObjectName(QStringLiteral("btn_elevt_ok"));
        btn_elevt_ok->setMinimumSize(QSize(63, 29));
        btn_elevt_ok->setMaximumSize(QSize(63, 29));

        horizontalLayout_53->addWidget(btn_elevt_ok);

        btn_elevt_cancel = new QPushButton(wid_elevator);
        btn_elevt_cancel->setObjectName(QStringLiteral("btn_elevt_cancel"));
        btn_elevt_cancel->setMinimumSize(QSize(63, 29));
        btn_elevt_cancel->setMaximumSize(QSize(63, 29));

        horizontalLayout_53->addWidget(btn_elevt_cancel);


        verticalLayout_31->addLayout(horizontalLayout_53);


        verticalLayout_19->addWidget(wid_elevator);

        stacked_pages->addWidget(page_elevator);

        gridLayout->addWidget(stacked_pages, 1, 1, 1, 1);


        retranslateUi(ICManager);

        stacked_pages->setCurrentIndex(4);
        stacked_maketoolcard->setCurrentIndex(3);
        tabWidget_rooms->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(ICManager);
    } // setupUi

    void retranslateUi(QWidget *ICManager)
    {
        ICManager->setWindowTitle(QApplication::translate("ICManager", "ICManager", 0));
        btn_manager->setText(QString());
        label_person->setText(QApplication::translate("ICManager", "Hello!", 0));
        label_addr->setText(QApplication::translate("ICManager", "\347\273\237\344\270\200\345\271\263\345\217\260    |    \350\264\235\346\200\235\347\211\271", 0));
        btn_quit->setText(QString());
        btn_back->setText(QString());
        btn_maketoolcard->setText(QString());
        btn_cardlog->setText(QString());
        btn_losscard->setText(QString());
        btn_remakecard->setText(QString());
        btn_user->setText(QString());
        btn_cellinfo->setText(QString());
        btn_admin->setText(QString());
        btn_makeadmincard->setText(QString());
        btn_elevatorinfo->setText(QString());
        btn_makeownercard->setText(QString());
        btn_cardlist->setText(QString());
        line_user_search->setText(QString());
        btn_user_search->setText(QApplication::translate("ICManager", "\346\220\234\347\264\242", 0));
        QTableWidgetItem *___qtablewidgetitem = table_user->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ICManager", "\345\247\223\345\220\215", 0));
        QTableWidgetItem *___qtablewidgetitem1 = table_user->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("ICManager", "\345\234\260\345\235\200", 0));
        QTableWidgetItem *___qtablewidgetitem2 = table_user->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("ICManager", "\347\224\265\350\257\235", 0));
        QTableWidgetItem *___qtablewidgetitem3 = table_user->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("ICManager", "\345\215\241\347\211\207", 0));
        QTableWidgetItem *___qtablewidgetitem4 = table_user->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("ICManager", "\346\223\215\344\275\234", 0));
        QTableWidgetItem *___qtablewidgetitem5 = table_user->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("ICManager", "\345\244\207\346\263\250", 0));
        label_tip->setText(QApplication::translate("ICManager", " \346\226\260\345\273\272/\347\274\226\350\276\221\357\274\232", 0));
        label_9->setText(QApplication::translate("ICManager", "\345\247\223\345\220\215", 0));
        line_user_name->setText(QString());
        label_10->setText(QApplication::translate("ICManager", "\346\211\213\346\234\272", 0));
        label_12->setText(QApplication::translate("ICManager", "\345\234\260\345\235\200", 0));
        label_34->setText(QApplication::translate("ICManager", "\345\215\225\345\205\203", 0));
        label_35->setText(QApplication::translate("ICManager", "\345\261\202", 0));
        label_36->setText(QApplication::translate("ICManager", "\345\256\244", 0));
        label_2->setText(QApplication::translate("ICManager", "\345\217\257\347\224\250\347\224\265\346\242\257\357\274\232", 0));
        label->setText(QApplication::translate("ICManager", "\345\217\257\347\224\250\346\245\274\345\261\202\357\274\232", 0));
        label_msg->setText(QString());
        label_26->setText(QString());
        btn_user_edit_ok->setText(QApplication::translate("ICManager", "\347\241\256\350\256\244", 0));
        btn_admin_search->setText(QApplication::translate("ICManager", "\346\220\234\347\264\242", 0));
        QTableWidgetItem *___qtablewidgetitem6 = table_admin->horizontalHeaderItem(0);
        ___qtablewidgetitem6->setText(QApplication::translate("ICManager", "\347\256\241\347\220\206\345\221\230\345\247\223\345\220\215", 0));
        QTableWidgetItem *___qtablewidgetitem7 = table_admin->horizontalHeaderItem(1);
        ___qtablewidgetitem7->setText(QApplication::translate("ICManager", "\345\200\274\347\217\255\345\256\244\345\234\260\345\235\200", 0));
        QTableWidgetItem *___qtablewidgetitem8 = table_admin->horizontalHeaderItem(2);
        ___qtablewidgetitem8->setText(QApplication::translate("ICManager", "\347\224\265\350\257\235\345\217\267\347\240\201", 0));
        QTableWidgetItem *___qtablewidgetitem9 = table_admin->horizontalHeaderItem(3);
        ___qtablewidgetitem9->setText(QApplication::translate("ICManager", "\345\215\241\347\211\207", 0));
        QTableWidgetItem *___qtablewidgetitem10 = table_admin->horizontalHeaderItem(4);
        ___qtablewidgetitem10->setText(QApplication::translate("ICManager", "\346\223\215\344\275\234", 0));
        QTableWidgetItem *___qtablewidgetitem11 = table_admin->horizontalHeaderItem(5);
        ___qtablewidgetitem11->setText(QApplication::translate("ICManager", "\345\244\207\346\263\250", 0));
        label_tip_2->setText(QApplication::translate("ICManager", "  \346\226\260\345\273\272/\347\274\226\350\276\221\357\274\232", 0));
        label_admin_name->setText(QApplication::translate("ICManager", "\345\247\223\345\220\215", 0));
        label_admin_phone->setText(QApplication::translate("ICManager", "\346\211\213\346\234\272", 0));
        line_admin_phone->setText(QString());
        label_41->setText(QApplication::translate("ICManager", "\347\231\273\345\275\225\345\257\206\347\240\201", 0));
        line_admin_password->setText(QString());
        label_7->setText(QApplication::translate("ICManager", "\345\234\260\345\235\200", 0));
        label_manager_permission->setText(QApplication::translate("ICManager", "  \346\235\203\351\231\220\357\274\232", 0));
        groupBox_5->setTitle(QString());
        checkBox_adminPower_makeownercard->setText(QApplication::translate("ICManager", "\345\217\221\344\270\232\344\270\273\345\215\241", 0));
        checkBox_adminPower_makeadmincard->setText(QApplication::translate("ICManager", "\345\217\221\347\256\241\347\220\206\345\221\230\345\215\241", 0));
        label_8->setText(QApplication::translate("ICManager", "\345\205\205\345\200\274\344\270\212\351\231\220", 0));
        comboBox_topup->clear();
        comboBox_topup->insertItems(0, QStringList()
         << QApplication::translate("ICManager", "500", 0)
         << QApplication::translate("ICManager", "1000", 0)
        );
        label_18->setText(QApplication::translate("ICManager", "RMB", 0));
        checkBox_adminPower_makesyntimecard->setText(QApplication::translate("ICManager", "\345\217\221\345\220\214\346\255\245\346\227\266\351\227\264\345\215\241", 0));
        checkBox_adminPower_makereadcard->setText(QApplication::translate("ICManager", "\345\217\221\350\257\273\345\217\226\347\224\250\346\210\267\350\256\260\345\275\225\345\215\241", 0));
        checkBox_adminPower_makemaskroomcard->setText(QApplication::translate("ICManager", "\345\217\221\345\261\217\350\224\275\346\210\277\351\227\264/\346\245\274\345\261\202/\347\224\265\346\242\257\345\215\241", 0));
        checkBox_adminPower_makeopenfloorcard->setText(QApplication::translate("ICManager", "\345\217\221\345\274\200\346\224\276\345\205\254\345\205\261\346\245\274\345\261\202\345\215\241", 0));
        checkBox_adminPower_makelevatimeallow->setText(QApplication::translate("ICManager", "\345\217\221\347\224\265\346\242\257\350\277\220\350\241\214\346\227\266\346\256\265\345\215\241", 0));
        btn_admin_ok->setText(QApplication::translate("ICManager", "\347\241\256\350\256\244", 0));
        btn_cardlist_search->setText(QApplication::translate("ICManager", "\346\220\234\347\264\242", 0));
        QTableWidgetItem *___qtablewidgetitem12 = table_cardlist->horizontalHeaderItem(0);
        ___qtablewidgetitem12->setText(QApplication::translate("ICManager", "\345\215\241\345\217\267", 0));
        QTableWidgetItem *___qtablewidgetitem13 = table_cardlist->horizontalHeaderItem(1);
        ___qtablewidgetitem13->setText(QApplication::translate("ICManager", "\347\261\273\345\210\253", 0));
        QTableWidgetItem *___qtablewidgetitem14 = table_cardlist->horizontalHeaderItem(2);
        ___qtablewidgetitem14->setText(QApplication::translate("ICManager", "\345\247\223\345\220\215", 0));
        QTableWidgetItem *___qtablewidgetitem15 = table_cardlist->horizontalHeaderItem(3);
        ___qtablewidgetitem15->setText(QApplication::translate("ICManager", "\346\235\203\351\231\220", 0));
        QTableWidgetItem *___qtablewidgetitem16 = table_cardlist->horizontalHeaderItem(4);
        ___qtablewidgetitem16->setText(QApplication::translate("ICManager", "\347\212\266\346\200\201", 0));
        QTableWidgetItem *___qtablewidgetitem17 = table_cardlist->horizontalHeaderItem(5);
        ___qtablewidgetitem17->setText(QApplication::translate("ICManager", "\345\217\221\345\215\241\344\272\272", 0));
        QTableWidgetItem *___qtablewidgetitem18 = table_cardlist->horizontalHeaderItem(6);
        ___qtablewidgetitem18->setText(QApplication::translate("ICManager", "\346\223\215\344\275\234", 0));
        QTableWidgetItem *___qtablewidgetitem19 = table_cardlist->horizontalHeaderItem(7);
        ___qtablewidgetitem19->setText(QApplication::translate("ICManager", "\345\244\207\346\263\250", 0));
        btn_cardlog_search->setText(QApplication::translate("ICManager", "\346\220\234\347\264\242", 0));
        label_19->setText(QApplication::translate("ICManager", "\350\207\263", 0));
        QTableWidgetItem *___qtablewidgetitem20 = table_cardlog->horizontalHeaderItem(0);
        ___qtablewidgetitem20->setText(QApplication::translate("ICManager", "\347\261\273\345\210\253", 0));
        QTableWidgetItem *___qtablewidgetitem21 = table_cardlog->horizontalHeaderItem(1);
        ___qtablewidgetitem21->setText(QApplication::translate("ICManager", "\345\215\241\345\217\267", 0));
        QTableWidgetItem *___qtablewidgetitem22 = table_cardlog->horizontalHeaderItem(2);
        ___qtablewidgetitem22->setText(QApplication::translate("ICManager", "\345\247\223\345\220\215", 0));
        QTableWidgetItem *___qtablewidgetitem23 = table_cardlog->horizontalHeaderItem(3);
        ___qtablewidgetitem23->setText(QApplication::translate("ICManager", "\346\227\266\351\227\264", 0));
        QTableWidgetItem *___qtablewidgetitem24 = table_cardlog->horizontalHeaderItem(4);
        ___qtablewidgetitem24->setText(QApplication::translate("ICManager", "\345\221\274\346\242\257\346\226\271\345\274\217", 0));
        label_30->setText(QApplication::translate("ICManager", "\345\247\223\345\220\215", 0));
        line_business_name->setText(QString());
        btn_owner_list->setText(QApplication::translate("ICManager", "\344\272\272\345\221\230\345\210\227\350\241\250", 0));
        label_37->setText(QApplication::translate("ICManager", "\345\215\241\345\217\267", 0));
        line_business_cardnum->setText(QString());
        label_38->setText(QApplication::translate("ICManager", "\345\257\206\347\240\201", 0));
        line_business_password->setText(QString());
        group_card_fee->setTitle(QString());
        label_20->setText(QApplication::translate("ICManager", "\346\224\266\350\264\271\347\261\273\345\236\213", 0));
        combo_cardfee_type->clear();
        combo_cardfee_type->insertItems(0, QStringList()
         << QApplication::translate("ICManager", "\344\270\215\346\224\266\350\264\271", 0)
         << QApplication::translate("ICManager", "\346\214\211\346\254\241\346\224\266\350\264\271", 0)
         << QApplication::translate("ICManager", "\346\214\211\345\244\251\346\224\266\350\264\271", 0)
         << QApplication::translate("ICManager", "\345\214\205\346\234\210", 0)
         << QApplication::translate("ICManager", "\345\214\205\345\271\264", 0)
        );
        label_21->setText(QApplication::translate("ICManager", "\346\224\266\350\264\271\346\240\207\345\207\206", 0));
        label_22->setText(QApplication::translate("ICManager", "\345\205\203/\357\274\210\346\254\241\343\200\201\345\244\251\343\200\201\346\234\210\343\200\201\345\271\264\357\274\211", 0));
        label_16->setText(QApplication::translate("ICManager", "\345\215\241\345\206\205\344\275\231\351\242\235", 0));
        combox_recharge_value->clear();
        combox_recharge_value->insertItems(0, QStringList()
         << QApplication::translate("ICManager", "100", 0)
         << QApplication::translate("ICManager", "200", 0)
         << QApplication::translate("ICManager", "300", 0)
         << QApplication::translate("ICManager", "400", 0)
         << QApplication::translate("ICManager", "500", 0)
        );
        btn_user_recharge->setText(QApplication::translate("ICManager", "\345\205\205\345\200\274", 0));
        label_3->setText(QString());
        label_43->setText(QApplication::translate("ICManager", "\350\257\267\344\276\235\346\254\241\351\200\211\346\213\251\347\224\265\346\242\257\345\217\267\346\235\245\346\267\273\345\212\240\345\217\257\347\224\250\346\245\274\345\261\202", 0));
        btn_saveFloor->setText(QApplication::translate("ICManager", "\344\277\235\345\255\230\346\245\274\345\261\202\345\200\274", 0));
        label_44->setText(QApplication::translate("ICManager", "\345\217\257\347\224\250\346\245\274\345\261\202\357\274\232", 0));
        group_card_time->setTitle(QString());
        comboBox_response_time->clear();
        comboBox_response_time->insertItems(0, QStringList()
         << QApplication::translate("ICManager", "1", 0)
         << QApplication::translate("ICManager", "2", 0)
         << QApplication::translate("ICManager", "3", 0)
         << QApplication::translate("ICManager", "4", 0)
         << QApplication::translate("ICManager", "5", 0)
        );
        label_28->setText(QApplication::translate("ICManager", "\346\214\211\351\222\256\345\223\215\345\272\224\346\227\266\351\227\264", 0));
        comboBox_calltype->clear();
        comboBox_calltype->insertItems(0, QStringList()
         << QApplication::translate("ICManager", "\350\207\252\345\212\250\345\221\274\346\242\257", 0)
         << QApplication::translate("ICManager", "\346\211\213\345\212\250\345\221\274\346\242\257", 0)
        );
        label_29->setText(QApplication::translate("ICManager", "\345\221\274\346\242\257\346\226\271\345\274\217", 0));
        label_45->setText(QApplication::translate("ICManager", "\345\215\241\346\234\211\346\225\210\346\227\245\346\234\237", 0));
        label_27->setText(QApplication::translate("ICManager", "\350\207\263", 0));
        checkBox_available_time->setText(QApplication::translate("ICManager", "\345\217\257\347\224\250\346\227\266\351\227\264\346\256\265", 0));
        time_card_start->setDisplayFormat(QApplication::translate("ICManager", "H:mm:ss", 0));
        label_40->setText(QApplication::translate("ICManager", "\350\207\263", 0));
        time_card_end->setDisplayFormat(QApplication::translate("ICManager", "H:mm:ss", 0));
        label_23->setText(QString());
        check_card_week_1->setText(QApplication::translate("ICManager", "\345\221\250\344\270\200", 0));
        check_card_week_2->setText(QApplication::translate("ICManager", "\345\221\250\344\272\214", 0));
        check_card_week_3->setText(QApplication::translate("ICManager", "\345\221\250\344\270\211", 0));
        check_card_week_4->setText(QApplication::translate("ICManager", "\345\221\250\345\233\233", 0));
        check_card_week_5->setText(QApplication::translate("ICManager", "\345\221\250\344\272\224", 0));
        check_card_week_7->setText(QApplication::translate("ICManager", "\345\221\250\345\205\255", 0));
        check_card_week_6->setText(QApplication::translate("ICManager", "\345\221\250\346\227\245", 0));
        btn_makecard_ok->setText(QApplication::translate("ICManager", "\347\241\256\350\256\244", 0));
        label_59->setText(QApplication::translate("ICManager", "\345\247\223\345\220\215", 0));
        line_makeadmin_name->setText(QString());
        btn_admin_list->setText(QApplication::translate("ICManager", "\344\272\272\345\221\230\345\210\227\350\241\250", 0));
        label_60->setText(QApplication::translate("ICManager", "\345\215\241\345\217\267", 0));
        line_makeadmin_cardnum->setText(QString());
        label_61->setText(QApplication::translate("ICManager", "\345\257\206\347\240\201", 0));
        line_makeadmin_password->setText(QString());
        groupBox_3->setTitle(QString());
        checkBox_admin_cardValidtime->setText(QString());
        label_46->setText(QApplication::translate("ICManager", "\345\215\241\347\211\207\346\234\211\346\225\210\346\234\237", 0));
        label_47->setText(QApplication::translate("ICManager", "\350\207\263", 0));
        checkBox_admin_availabletime->setText(QString());
        label_48->setText(QApplication::translate("ICManager", "\345\217\257\347\224\250\346\227\266\351\227\264\346\256\265", 0));
        time_admincard_start->setDisplayFormat(QApplication::translate("ICManager", "H:mm:ss", 0));
        label_49->setText(QApplication::translate("ICManager", "\350\207\263", 0));
        time_admincard_end->setDisplayFormat(QApplication::translate("ICManager", "H:mm:ss", 0));
        check_admincard_week_1->setText(QApplication::translate("ICManager", "\345\221\250\344\270\200", 0));
        check_admincard_week_2->setText(QApplication::translate("ICManager", "\345\221\250\344\272\214", 0));
        check_admincard_week_3->setText(QApplication::translate("ICManager", "\345\221\250\344\270\211", 0));
        check_admincard_week_4->setText(QApplication::translate("ICManager", "\345\221\250\345\233\233", 0));
        check_admincard_week_5->setText(QApplication::translate("ICManager", "\345\221\250\344\272\224", 0));
        check_admincard_week_6->setText(QApplication::translate("ICManager", "\345\221\250\345\205\255", 0));
        check_admincard_week_7->setText(QApplication::translate("ICManager", "\345\221\250\346\227\245", 0));
        btn_makeadmincard_ok->setText(QApplication::translate("ICManager", "\347\241\256\350\256\244", 0));
        label_111->setText(QApplication::translate("ICManager", "\345\215\241\345\217\267", 0));
        line_maketoolcard_cardnum->setText(QString());
        label_112->setText(QApplication::translate("ICManager", "\345\257\206\347\240\201", 0));
        line_maketoolcard_password->setText(QString());
        groupBox_4->setTitle(QString());
        label_117->setText(QApplication::translate("ICManager", "\351\200\211\346\213\251\345\215\225\345\205\203", 0));
        label_118->setText(QApplication::translate("ICManager", "\351\200\211\346\213\251\347\224\265\346\242\257", 0));
        groupBox->setTitle(QString());
        btn_maketoolcard_syntime->setText(QApplication::translate("ICManager", "\345\220\214\346\255\245\346\227\266\351\227\264\345\215\241", 0));
        btn_maketoolcard_readcard->setText(QApplication::translate("ICManager", "\350\257\273\345\217\226\347\224\250\346\210\267\346\225\260\346\215\256\345\215\241", 0));
        btn_maketoolcard_maskroom->setText(QApplication::translate("ICManager", "\345\261\217\350\224\275\346\210\277\351\227\264/\346\245\274\345\261\202/\347\224\265\346\242\257\345\215\241", 0));
        btn_maketoolcard_openfloor->setText(QApplication::translate("ICManager", "\345\274\200\346\224\276\345\205\254\345\205\261\346\245\274\345\261\202\345\215\241", 0));
        btn_maketoolcard_elevtimeallow->setText(QApplication::translate("ICManager", "\347\224\265\346\242\257\350\277\220\350\241\214\346\227\266\346\256\265\345\215\241", 0));
        btn_maketoolcard_acticatecard->setText(QApplication::translate("ICManager", "\347\224\265\346\242\257\346\277\200\346\264\273\345\215\241", 0));
        btn_maketoolcard_elevNum->setText(QApplication::translate("ICManager", "\347\224\265\346\242\257\347\274\226\345\217\267\345\215\241", 0));
        btn_maketoolcard_lostCart->setText(QApplication::translate("ICManager", "\346\214\202\345\244\261\345\215\241", 0));
        label_93->setText(QApplication::translate("ICManager", "\347\224\265\346\242\257\346\216\247\345\210\266\345\231\250\346\227\266\351\227\264", 0));
        timeEdit_syntime->setDisplayFormat(QApplication::translate("ICManager", "H:mm:ss", 0));
        label_99->setText(QApplication::translate("ICManager", "\350\257\264\346\230\216\357\274\232\346\255\244\345\215\241\347\224\250\344\270\200\346\254\241\345\260\261\351\234\200\350\246\201\351\207\215\345\210\266\344\275\234\344\270\200\346\254\241", 0));
        label_124->setText(QApplication::translate("ICManager", "\344\270\232\344\270\273", 0));
        btn_readcard_selectowner->setText(QApplication::translate("ICManager", "\351\200\211\346\213\251\344\270\232\344\270\273", 0));
        label_125->setText(QApplication::translate("ICManager", "\351\200\211\346\213\251\346\245\274\345\261\202", 0));
        label_100->setText(QApplication::translate("ICManager", "\350\257\273\345\217\226\345\210\267\345\215\241\346\227\266\351\227\264\346\256\265", 0));
        timeEdit_readcard_start->setDisplayFormat(QApplication::translate("ICManager", "HH:mm", 0));
        label_101->setText(QApplication::translate("ICManager", "\350\207\263", 0));
        timeEdit_readcard_end->setDisplayFormat(QApplication::translate("ICManager", "HH:mm", 0));
        label_102->setText(QApplication::translate("ICManager", "\345\261\217\350\224\275\346\245\274\345\261\202\346\210\226\346\210\277\351\227\264\357\274\232", 0));
        btn_maskelevt->setText(QApplication::translate("ICManager", "\345\261\217\350\224\275\347\224\265\346\242\257", 0));
        btn_enablelevt->setText(QApplication::translate("ICManager", "\345\220\257\347\224\250\347\224\265\346\242\257", 0));
        btn_maskfloor->setText(QApplication::translate("ICManager", "\345\261\217\350\224\275\346\245\274\345\261\202", 0));
        btn_enablefloor->setText(QApplication::translate("ICManager", "\345\220\257\347\224\250\346\245\274\345\261\202", 0));
        label_104->setText(QApplication::translate("ICManager", "\345\274\200\346\224\276\345\205\254\345\205\261\346\245\274\345\261\202\357\274\232", 0));
        checkBox_time1->setText(QString());
        label_106->setText(QApplication::translate("ICManager", "\346\227\266\351\227\264\346\256\2651", 0));
        timeEdit_timeallowstart1->setDisplayFormat(QApplication::translate("ICManager", "HH:mm:ss", 0));
        label_25->setText(QApplication::translate("ICManager", "\350\207\263", 0));
        timeEdit_timeallowend1->setDisplayFormat(QApplication::translate("ICManager", "HH:mm:ss", 0));
        checkBox_time2->setText(QString());
        label_107->setText(QApplication::translate("ICManager", "\346\227\266\351\227\264\346\256\2652", 0));
        timeEdit_timeallowstart2->setDisplayFormat(QApplication::translate("ICManager", "HH:mm:ss", 0));
        label_31->setText(QApplication::translate("ICManager", "\350\207\263", 0));
        timeEdit_timeallowend2->setDisplayFormat(QApplication::translate("ICManager", "HH:mm:ss", 0));
        checkBox_time3->setText(QString());
        label_108->setText(QApplication::translate("ICManager", "\346\227\266\351\227\264\346\256\2653", 0));
        timeEdit_timeallowstart3->setDisplayFormat(QApplication::translate("ICManager", "HH:mm:ss", 0));
        label_32->setText(QApplication::translate("ICManager", "\350\207\263", 0));
        timeEdit_timeallowend3->setDisplayFormat(QApplication::translate("ICManager", "HH:mm:ss", 0));
        check_week_1->setText(QApplication::translate("ICManager", "\345\221\250\344\270\200", 0));
        check_week_2->setText(QApplication::translate("ICManager", "\345\221\250\344\272\214", 0));
        check_week_3->setText(QApplication::translate("ICManager", "\345\221\250\344\270\211", 0));
        check_week_4->setText(QApplication::translate("ICManager", "\345\221\250\345\233\233", 0));
        check_week_5->setText(QApplication::translate("ICManager", "\345\221\250\344\272\224", 0));
        check_week_6->setText(QApplication::translate("ICManager", "\345\221\250\345\205\255", 0));
        check_week_7->setText(QApplication::translate("ICManager", "\345\221\250\346\227\245", 0));
        label_105->setText(QApplication::translate("ICManager", "\346\263\250\357\274\232\345\220\214\344\270\200\345\215\225\345\205\203\346\245\274\346\211\200\346\234\211\345\217\257\347\224\250\347\224\265\346\242\257\351\201\265\345\276\252\347\232\204\346\227\266\346\256\265\344\270\200\350\207\264\357\274\2143\344\270\252\346\227\266\351\227\264\346\256\265\344\270\215\347\224\250\345\210\267\345\215\241", 0));
        activate_card->setText(QApplication::translate("ICManager", "\346\267\273\345\212\240\346\277\200\346\264\273\345\215\241\346\214\207\344\273\244", 0));
        label_15->setText(QApplication::translate("ICManager", "\345\244\207\346\263\250\357\274\232\346\277\200\346\264\273\345\215\241\345\257\271\346\211\200\346\234\211\347\232\204\347\224\265\346\242\257\346\234\211\346\225\210\357\274\214\344\270\215\351\234\200\350\246\201\351\200\211\346\213\251\345\215\225\345\205\203\347\224\265\346\242\257", 0));
        label_24->setText(QApplication::translate("ICManager", "\345\244\207\346\263\250\357\274\232\351\200\211\346\213\251\351\234\200\347\274\226\345\217\267\347\224\265\346\242\257\346\211\200\345\234\250\347\232\204\345\215\225\345\205\203\357\274\214\345\271\266\344\270\224\345\234\250\345\217\263\350\276\271\345\213\276\351\200\211\344\270\200\345\217\260\347\224\265\346\242\257", 0));
        label_39->setText(QApplication::translate("ICManager", "\344\270\242\345\244\261\345\215\241\345\217\267:", 0));
        btn_GetLostCardNum->setText(QApplication::translate("ICManager", "\350\216\267\345\217\226\344\270\242\345\244\261\345\215\241\345\217\267", 0));
        btn_toolcard_ok->setText(QApplication::translate("ICManager", "\347\241\256\350\256\244", 0));
        label_114->setText(QApplication::translate("ICManager", "\345\215\241\345\217\267", 0));
        line_cardloss_cardnum->setText(QString());
        label_115->setText(QApplication::translate("ICManager", "\345\257\206\347\240\201", 0));
        line_cardloss_password->setText(QString());
        label_5->setText(QApplication::translate("ICManager", "\347\224\265\346\242\257\345\217\267", 0));
        btn_elector_list->setText(QApplication::translate("ICManager", "\350\216\267\345\217\226\347\224\265\346\242\257\345\217\267", 0));
        label_6->setText(QApplication::translate("ICManager", "\346\214\202\345\244\261\345\215\241\345\217\267", 0));
        btn_card_list->setText(QApplication::translate("ICManager", "\350\216\267\345\217\226\345\215\241\345\217\267", 0));
        btn_cardlist_ok_4->setText(QApplication::translate("ICManager", "\346\214\202\345\244\261", 0));
        btn_cardlist_cancel_4->setText(QApplication::translate("ICManager", "\345\217\226\346\266\210", 0));
        btn_owner_replacecard->setText(QApplication::translate("ICManager", "\344\270\232\344\270\273\350\241\245\345\215\241", 0));
        btn_tools_replacecard->setText(QApplication::translate("ICManager", "\345\267\245\345\205\267\350\241\245\345\215\241", 0));
        QTableWidgetItem *___qtablewidgetitem25 = table_cellinfo->horizontalHeaderItem(0);
        ___qtablewidgetitem25->setText(QApplication::translate("ICManager", "\345\215\225\345\205\203", 0));
        QTableWidgetItem *___qtablewidgetitem26 = table_cellinfo->horizontalHeaderItem(1);
        ___qtablewidgetitem26->setText(QApplication::translate("ICManager", "\345\261\202", 0));
        QTableWidgetItem *___qtablewidgetitem27 = table_cellinfo->horizontalHeaderItem(2);
        ___qtablewidgetitem27->setText(QApplication::translate("ICManager", "\345\256\244", 0));
        label_tip_3->setText(QApplication::translate("ICManager", " \346\226\260\345\242\236\357\274\232", 0));
        btn_cell_del->setText(QApplication::translate("ICManager", "\345\210\240\351\231\244", 0));
        group_cell_content->setTitle(QString());
        check_addAuto->setText(QApplication::translate("ICManager", "\350\207\252\345\212\250\346\267\273\345\212\240\344\277\241\346\201\257", 0));
        line_cellunit->setText(QString());
        label_11->setText(QApplication::translate("ICManager", "\345\215\225\345\205\203", 0));
        line_celllayer->setText(QString());
        label_13->setText(QApplication::translate("ICManager", "\345\261\202", 0));
        line_cellfamily->setText(QString());
        label_14->setText(QApplication::translate("ICManager", "\345\256\244", 0));
        btn_cellinfo_ok->setText(QApplication::translate("ICManager", "\347\241\256\345\256\232", 0));
        btn_cellinfo_cancel->setText(QApplication::translate("ICManager", "\345\217\226\346\266\210", 0));
        btn_elevator_search->setText(QApplication::translate("ICManager", "\346\220\234\347\264\242", 0));
        QTableWidgetItem *___qtablewidgetitem28 = table_elevator->horizontalHeaderItem(0);
        ___qtablewidgetitem28->setText(QApplication::translate("ICManager", "\347\224\265\346\242\257\347\274\226\345\217\267", 0));
        QTableWidgetItem *___qtablewidgetitem29 = table_elevator->horizontalHeaderItem(1);
        ___qtablewidgetitem29->setText(QApplication::translate("ICManager", "\345\215\225\345\205\203", 0));
        QTableWidgetItem *___qtablewidgetitem30 = table_elevator->horizontalHeaderItem(2);
        ___qtablewidgetitem30->setText(QApplication::translate("ICManager", "\347\224\265\346\242\257\346\200\273\346\216\247\346\245\274\345\261\202\346\225\260", 0));
        label_tip_4->setText(QApplication::translate("ICManager", " \346\226\260\345\242\236\347\224\265\346\242\257\357\274\232", 0));
        btn_dellift->setText(QApplication::translate("ICManager", "\345\210\240\351\231\244\347\224\265\346\242\257", 0));
        group_elevator_content->setTitle(QString());
        label_33->setText(QApplication::translate("ICManager", "\345\215\225\345\205\203", 0));
        label_52->setText(QApplication::translate("ICManager", "\347\224\265\346\242\257\347\274\226\345\217\267", 0));
        line_elevnum->setText(QString());
        label_4->setText(QApplication::translate("ICManager", "\347\224\265\346\242\257\346\200\273\346\216\247\346\245\274\345\261\202\346\225\260", 0));
        label_tip_5->setText(QApplication::translate("ICManager", "\346\245\274\345\261\202\346\230\240\345\260\204\350\256\276\347\275\256\357\274\232", 0));
        QTableWidgetItem *___qtablewidgetitem31 = table_map->horizontalHeaderItem(0);
        ___qtablewidgetitem31->setText(QApplication::translate("ICManager", "\347\211\251\347\220\206\346\245\274\345\261\202", 0));
        QTableWidgetItem *___qtablewidgetitem32 = table_map->horizontalHeaderItem(1);
        ___qtablewidgetitem32->setText(QApplication::translate("ICManager", "\346\245\274\345\261\202\346\240\207\350\257\206", 0));
        QTableWidgetItem *___qtablewidgetitem33 = table_map->horizontalHeaderItem(2);
        ___qtablewidgetitem33->setText(QApplication::translate("ICManager", "\346\210\277\351\227\264\345\210\227\350\241\250", 0));
        label_53->setText(QApplication::translate("ICManager", "\347\211\251\347\220\206\346\245\274\345\261\202", 0));
        line_actualLayer->setText(QString());
        label_56->setText(QApplication::translate("ICManager", "\346\245\274\345\261\202\346\240\207\350\257\206", 0));
        line_mapLayer->setText(QString());
        label_57->setText(QApplication::translate("ICManager", "\346\210\277\351\227\264\345\210\227\350\241\250", 0));
        line_roomlist->setText(QApplication::translate("ICManager", "1\357\274\2144\357\274\2146", 0));
        checkBox_4->setText(QApplication::translate("ICManager", "\350\207\252\345\212\250\345\210\206\351\205\215", 0));
        label_17->setText(QApplication::translate("ICManager", "     \345\244\232\344\270\252\346\210\277\351\227\264\344\271\213\351\227\264\344\273\245'\357\274\214'\345\210\206\351\232\224", 0));
        btn_addliftmap->setText(QApplication::translate("ICManager", "\346\267\273\345\212\240/\346\233\264\346\226\260", 0));
        btn_delliftmap->setText(QApplication::translate("ICManager", "\345\210\240\351\231\244", 0));
        btn_elevt_ok->setText(QApplication::translate("ICManager", "\347\241\256\350\256\244", 0));
        btn_elevt_cancel->setText(QApplication::translate("ICManager", "\345\217\226\346\266\210", 0));
    } // retranslateUi

};

namespace Ui {
    class ICManager: public Ui_ICManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ICMANAGER_H
