/********************************************************************************
** Form generated from reading UI file 'devicesetting.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEVICESETTING_H
#define UI_DEVICESETTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "bstui.h"

QT_BEGIN_NAMESPACE

class Ui_DeviceManageForm
{
public:
    QVBoxLayout *verticalLayout;
    QStackedWidget *MainStackedWidget;
    QWidget *page1_language;
    QLabel *label_1_title;
    QPushButton *lang_ned_banner;
    QPushButton *lang_en_banner;
    QPushButton *lang_deu_banner;
    QPushButton *lang_fra_banner;
    QLabel *label_logo;
    QLabel *label_version;
    QPushButton *btn_1_1_local;
    QPushButton *btn_1_1_new;
    QPushButton *btn_download;
    QWidget *page2_mainpage;
    QLabel *label_2_title;
    QLabel *img_2_preview;
    QLabel *label_2_preview;
    QPushButton *btn_modify;
    QPushButton *btn_remove;
    QPushButton *btn_exit;
    QPushButton *btn_export;
    QPushButton *btn_close;
    QWidget *widget_2_info;
    QLabel *label_info_name_1;
    QLabel *label_info_name_2;
    QLabel *label_info_appver_2;
    QLabel *label_info_appver_1;
    QLabel *label_info_sysver_2;
    QLabel *label_info_sysver_1;
    QLabel *label_info_devtype_2;
    QLabel *label_info_devtype_1;
    QPushButton *btn_test;
    QWidget *page2_1_createpro;
    QLabel *label_2_1_title;
    QLabel *label_2_1_name;
    QLineEdit *edit_2_1_name;
    QPushButton *btn_2_1_return;
    QPushButton *btn_2_1_create;
    QLabel *label_2_1_devtype;
    QRadioButton *radio_canopen;
    QRadioButton *radio_saf;
    QWidget *page2_2_downloadpro;
    QLabel *label_2_2_title;
    QPushButton *btn_wifi;
    QPushButton *btn_usb;
    QPushButton *btn_2_2_return;
    QWidget *page2_4_simplepro;
    QLabel *label_2_4_title;
    QCheckBox *check_application;
    QCheckBox *check_conffile;
    QCheckBox *check_sysfile;
    QCheckBox *check_style;
    QPushButton *btn_2_4_ok;
    QCheckBox *check_audio;
    QCheckBox *check_all;
    QPushButton *btn_2_4_return;
    QPushButton *btn_2_4_ok_md;
    QWidget *page2_21_fromrwifi;
    QListWidget *list_device;
    QLabel *label_2_21_title;
    QPushButton *btn_scandev;
    QLineEdit *edit_wifipasswd;
    QPushButton *btn_2_21_return;
    QLabel *label_wifipasswd;
    QPushButton *btn_logindev;
    QLineEdit *edit_wifiip;
    QWidget *page2_3_importpro;
    QLabel *label_2_3_title;
    QLabel *label_2_3_name;
    QLineEdit *edit_2_3_name;
    QLabel *label_2_3_path;
    QLineEdit *edit_2_3_path;
    QPushButton *btn_2_3_selectfolder;
    QPushButton *btn_2_3_import;
    QPushButton *btn_2_3_return;
    QWidget *page3_modify;
    QLabel *label_3_title;
    QPushButton *btn_style;
    QPushButton *btn_setting;
    QPushButton *btn_3_return;
    QPushButton *btn_language;
    QPushButton *btn_layout;
    QLabel *label_3_preview;
    QLabel *img_3_preview;
    QPushButton *btn_explorer;
    QStackedWidget *stacked_arrowspeed;
    QWidget *page31_saf;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QComboBox *combo_arrowspeed;
    QWidget *page31_com;
    QPushButton *btn_application;
    QWidget *page31_layout;
    QLabel *label_31_title;
    QLabel *label_31_preview;
    QLabel *img_31_preview;
    QRadioButton *radio_srn_arrow;
    QRadioButton *radio_srn_floor;
    QPushButton *btn_31_return;
    QRadioButton *radio_srn_4_3;
    QRadioButton *radio_srn_4;
    QRadioButton *radio_srn_180;
    QRadioButton *radio_srn_90;
    QRadioButton *radio_srn_270;
    QWidget *page32_style;
    QLabel *label_32_preview;
    QLabel *img_32_preview;
    QLabel *label_32_title;
    QPushButton *btn_32_import;
    QPushButton *btn_32_return;
    QListWidget *list_32_style;
    QPushButton *btn_32_export;
    QPushButton *btn_32_remove;
    QPushButton *btn_32_text;
    QWidget *page32_1_import;
    QLabel *label_32_1_title;
    QLineEdit *edit_32_1_name;
    QLabel *label_32_1_name;
    QPushButton *btn_32_1_import;
    QLabel *label_32_1_path;
    QLineEdit *edit_32_1_path;
    QPushButton *btn_32_1_selectfolder;
    QPushButton *btn_32_1_return;
    QPushButton *btn_32_1_selectzip;
    QPushButton *btn_32_1_import_project;
    QLineEdit *edit_32_1_path_pro;
    QLabel *label_32_1_path_pro;
    QPushButton *btn_32_1_selectfolder_pro;
    QWidget *page32_2_create;
    QLabel *label_32_2_title;
    QLineEdit *edit_32_2_name;
    QLabel *label_32_2_name;
    QPushButton *btn_32_2_return;
    QPushButton *btn_32_2_next;
    QPushButton *btn_32_2_save;
    QWidget *page321_floor;
    QLabel *img_321_preview;
    QPushButton *btn_321_next;
    QLabel *label_321_title;
    QLabel *label_321_preview;
    QPushButton *btn_321_return;
    QPushButton *btn_321_import;
    QPushButton *btn_321_test;
    QListWidget *list_321_floor;
    QLineEdit *edit_321_cur;
    QWidget *page322_arrow;
    QLabel *label_322_preview;
    QPushButton *btn_322_import;
    QPushButton *btn_322_next;
    QPushButton *btn_322_return;
    QPushButton *btn_322_test;
    QLabel *img_322_preview;
    QLabel *label_322_title;
    QListWidget *list_322_arrows;
    QLineEdit *edit_322_cur;
    QWidget *page323_logo;
    QLabel *label_323_title;
    QPushButton *btn_323_return;
    QLineEdit *edit_323_text1;
    QLabel *label_323_text1;
    QLabel *label_323_text2;
    QLineEdit *edit_323_text2;
    QListWidget *list_323_logo;
    QWidget *page324_background;
    QLabel *label_324_preview;
    QPushButton *btn_324_import;
    QPushButton *btn_324_return;
    QLabel *img_324_preview;
    QLabel *label_324_title;
    QListWidget *list_324_background;
    QPushButton *btn_324_test;
    QLineEdit *edit_324_cur;
    QWidget *page33_language;
    QPushButton *btn_33_import;
    QPushButton *btn_33_return;
    QLabel *label_33_title;
    QListWidget *list_33_language;
    QListWidget *list_33_audio;
    QPushButton *btn_33_test;
    QPushButton *btn_33_addfile;
    QPushButton *btn_33_delfile;
    QPushButton *btn_33_delete;
    QLabel *label_33_langlist;
    QLabel *label_33_audiolist;
    QStackedWidget *stacked_openmsg;
    QWidget *page_33_saf_openmsg;
    QCheckBox *check_openmsg;
    QWidget *page_33_com_openmsg;
    QWidget *page33_1_import;
    QLineEdit *edit_33_1_path;
    QPushButton *btn_33_1_selectfolder;
    QLabel *label_33_1_path;
    QLineEdit *edit_33_1_name;
    QLabel *label_33_1_title;
    QLabel *label_33_1_name;
    QPushButton *btn_33_1_return;
    QPushButton *btn_33_1_import;
    QPushButton *btn_33_1_selectzip;
    QWidget *page33_2_new;
    QLineEdit *edit_33_2_name;
    QPushButton *btn_33_2_return;
    QPushButton *btn_33_2_next;
    QLabel *label_33_2_title;
    QLabel *label_33_2_name;
    QLineEdit *edit_33_2_path;
    QPushButton *btn_33_2_selectzip;
    QPushButton *btn_33_2_selectfolder;
    QPushButton *btn_33_2_import;
    QLabel *label_33_2_path;
    QWidget *page34_setting;
    QStackedWidget *SettingStacked;
    QWidget *page341_volume;
    QLabel *label_msg;
    QLabel *label_gong;
    QLabel *label_music;
    QLabel *label_msg_icon1;
    MySlider *slider_message_volume;
    QLabel *label_msg_icon2;
    QLabel *label_gong_icon2;
    MySlider *slider_gong_volume;
    QLabel *label_gong_icon1;
    QLabel *label_music_icon2;
    QLabel *label_music_icon1;
    MySlider *slider_bgmusic_volume;
    QLabel *label_msg_value;
    QLabel *label_gong_value;
    QLabel *label_music_value;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_36;
    QLabel *label_volume_enable;
    QCheckBox *check_volume_enable;
    QWidget *page342_floors;
    QStackedWidget *stacked_flrmap;
    QWidget *page;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *table_flrmap;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_flrmap;
    QWidget *page343_backlight;
    QLabel *label_standby_min;
    QLabel *label_normal;
    QLabel *label_normal_max;
    MySlider *slider_backlight;
    QLabel *label_standby_max;
    QLabel *label_delay_max;
    QLabel *label_standby;
    MySlider *slider_standby;
    QLabel *label_delay;
    QLabel *label_delay_min;
    QLabel *label_normal_min;
    MySlider *slider_delay;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_35;
    QLabel *label_backlight_enable;
    QCheckBox *check_backlight_enable;
    QWidget *page344_logo;
    QPushButton *btn_344_select;
    QWidget *wid_logo;
    QLabel *label_344_logo;
    QWidget *page345_wifi;
    QLineEdit *edit_passwd;
    QLabel *label_3_passwd;
    QLineEdit *edit_ssid;
    QLabel *label_3_ssid;
    QPushButton *btn_34_volume;
    QPushButton *btn_34_floors;
    QPushButton *btn_34_backlight;
    QPushButton *btn_34_logo;
    QPushButton *btn_34_return;
    QPushButton *btn_34_wifi;

    void setupUi(QWidget *DeviceManageForm)
    {
        if (DeviceManageForm->objectName().isEmpty())
            DeviceManageForm->setObjectName(QStringLiteral("DeviceManageForm"));
        DeviceManageForm->resize(982, 550);
        DeviceManageForm->setMinimumSize(QSize(982, 550));
        DeviceManageForm->setMaximumSize(QSize(982, 550));
        DeviceManageForm->setStyleSheet(QLatin1String("QWidget#page1_language { border-image: url(:/1_language/rc/1_language/bg.png);}\n"
"QWidget#page1_1_selectpro { border-image: url(:/2_mainpage/rc/2_mainpage/bg.png); }\n"
"QWidget#page2_mainpage { border-image: url(:/2_mainpage/rc/2_mainpage/bg.png); }\n"
"QWidget#page2_1_createpro { border-image:url(:/2_mainpage/rc/2_mainpage/bg.png); }\n"
"QWidget#page2_2_downloadpro { border-image:url(:/2_mainpage/rc/2_mainpage/bg.png); }\n"
"QWidget#page2_4_simplepro { border-image:url(:/2_mainpage/rc/2_mainpage/bg.png); }\n"
"QWidget#page2_21_fromrwifi { border-image:url(:/2_mainpage/rc/2_mainpage/bg.png); }\n"
"QWidget#page2_3_importpro { border-image:url(:/2_mainpage/rc/2_mainpage/bg.png); }\n"
"QWidget#page3_modify { border-image:url(:/2_mainpage/rc/2_mainpage/bg.png); }\n"
"QWidget#page31_layout { border-image:url(:/2_mainpage/rc/2_mainpage/bg.png); }\n"
"QWidget#page32_style { border-image:url(:/2_mainpage/rc/2_mainpage/bg.png); }\n"
"QWidget#page32_1_import { border-image:url(:/2_mainpage/rc/2_mainpage/bg.png); }\n"
""
                        "QWidget#page32_2_create { border-image:url(:/2_mainpage/rc/2_mainpage/bg.png); }\n"
"QWidget#page321_floor { border-image:url(:/2_mainpage/rc/2_mainpage/bg.png); }\n"
"QWidget#page322_arrow { border-image:url(:/2_mainpage/rc/2_mainpage/bg.png); }\n"
"QWidget#page323_logo { border-image:url(:/2_mainpage/rc/2_mainpage/bg.png); }\n"
"QWidget#page324_background { border-image:url(:/2_mainpage/rc/2_mainpage/bg.png); }\n"
"QWidget#page33_language { border-image:url(:/2_mainpage/rc/2_mainpage/bg.png); }\n"
"QWidget#page33_1_import { border-image:url(:/2_mainpage/rc/2_mainpage/bg.png); }\n"
"QWidget#page33_2_new { border-image:url(:/2_mainpage/rc/2_mainpage/bg.png); }\n"
"QWidget#page34_setting { border-image:url(:/2_mainpage/rc/2_mainpage/bg.png); }\n"
"\n"
"\n"
"QWidget#page31_saf, QWidget#page31_com, QWidget#page_33_saf_openmsg, QWidget#page_33_com_openmsg { background-color: rgba(255, 255, 255, 0); }\n"
"\n"
"QTableWidget#table_flrmap{ background-color: rgba(255, 255, 255, 0); color: rgba(255, 143, 16, 255);  icon"
                        "-size: 90px 90px; space : 40px; font: bold large \"Arial\"; font-size: 25pt;}\n"
"QTableWidget::item{  /*padding-top: 4px;padding-bottom: 4px;border-bottom: 1px solid #343434;*/}\n"
"QTableWidget::item:selected{background-color:rgba(254, 158, 37, 255); border: none;}\n"
"QTableWidget::item:selected:active{border: none;}\n"
"QTableWidget::item:selected:!active{border: none;}\n"
"\n"
"QWidget#contents_map, QWidget#page341_volume, QWidget#page342_floors, QWidget#page343_backlight, QWidget#page344_logo, QWidget#page345_wifi{ background-color:rgba(195, 195, 195, 0); }\n"
"\n"
"QScrollBar:vertical { background: rgba(0, 0, 0, 0);  width: 9px;  margin: 0px 0 0px 0; }\n"
"QScrollBar::handle:vertical { background: rgba(195, 195, 195, 255); min-height: 20px;  margin: 2px 0px 1px 0px;   border-radius: 3px;  border: 0px dashed rgba(180,130,100,255); }\n"
"QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }\n"
"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {  background: none; }\n"
""
                        "\n"
"QScrollBar:horizontal  { background: rgba(0, 0, 0, 0);  height: 9px;  margin: 0px 0 0px 0; }\n"
"QScrollBar::handle:horizontal  { background: rgba(195, 195, 195, 255);  min-width: 20px;  margin: 2px 0px 1px 0px;   border-radius: 3px;  border: 0px dashed rgba(180,130,100,255); }\n"
"QScrollBar::add-line:horizontal , QScrollBar::sub-line:horizontal  { height: 0px; }\n"
"QScrollBar::add-page:horizontal , QScrollBar::sub-page:horizontal  {  background: none; }\n"
"\n"
"QSlider::groove:horizontal {  border: 1px rgba(254, 158, 37, 255);  height: 2px;  margin: 0px 0; left: 22px; right: 22px;  }\n"
"QSlider::handle:horizontal {   border-image: url(:/rc/12_SetVolume/sliderdot.png);  width: 40px; height : 40px;  margin: -20px -20px -20px -20px; }\n"
"QSlider::sub-page:horizontal{  background: rgba(254, 158, 37, 255); }\n"
"QSlider::add-page:horizontal{ background: rgba(100, 100, 100, 255); }\n"
"QSlider::indicator { right : 22px}\n"
"\n"
"/*Style for all non specific QListWidget*/\n"
"QListWidget{ icon-size: 40px 4"
                        "0px; background-color: rgba(255, 255, 255, 0); alternate-background-color: #1b1b1b;  selection-\n"
"background-color: rgba(254, 158, 37, 255);  color: rgba(254, 254, 254, 255);  gridline-color:#343434;   border: 1px solid #29292a;  border-radius: 6px;  padding: 0px;  font: bold large \"Arial\"; font-size: 30pt; }\n"
"QListWidget::item{  /*padding-top: 4px;padding-bottom: 4px;border-bottom: 1px solid #343434;*/}\n"
"QListWidget::item:selected{background-color:rgba(254, 158, 37, 255); border: none;}\n"
"QListWidget::item:selected:active{border: none;}\n"
"QListWidget::item:selected:!active{border: none;}\n"
"\n"
"QListWidget#list_32_style{ icon-size: 60px 60px; background-color: rgba(255, 255, 255, 0); alternate-background-color: #1b1b1b;  selection-\n"
"background-color: rgba(254, 158, 37, 255);  color: rgba(254, 254, 254, 255);  gridline-color:#343434;   border: 1px solid #29292a;  border-radius: 6px;  padding: 0px;  font: bold large \"Arial\"; font-size: 20pt; }\n"
"\n"
"QComboBox#combo_arrowspeed {  font: bo"
                        "ld large \"Arial\"; font-size: 16pt; }\n"
"\n"
"QPushButton#btn_enter{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 25pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_enter::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 25pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#lang_en_banner{ border-image: url(:/1_language/rc/1_language/english.png);}\n"
"QPushButton#lang_en_banner::checked{ border-image: url(:/1_language/rc/1_language/english_en.png); }\n"
"\n"
"QPushButton#lang_fra_banner{ border-image: url(:/1_language/rc/1_language/french.png); }\n"
"QPushButton#lang_fra_banner::checked{ border-image: url(:/1_language/rc/1_language/french_en.png); }\n"
"\n"
"QPushButton#lang_deu_banner{ border-image: url(:/1_language/rc/1_language/germany.png);  }\n"
"QPushButton#lang_deu_banner::checked{ border-image: url(:/1_language/rc/1_language/germany_en.png);  }\n"
"\n"
"QPushButton#lang_ned_banner{ border-image:"
                        " url(:/1_language/rc/1_language/holland.png);}\n"
"QPushButton#lang_ned_banner::checked{ border-image: url(:/1_language/rc/1_language/holland_en.png);  }\n"
"\n"
"QLabel#label_logo{ border-image: url(:/1_language/rc/1_language/logo.png);}\n"
"\n"
"QPushButton#btn_explorer{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_explorer::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_download{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_download::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_modify{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; "
                        "color: rgba(198,210,225,250); }\n"
"QPushButton#btn_modify::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_close{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_close::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_new{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_new::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_open{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_open::pr"
                        "essed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_export{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_export::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_import{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_import::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_remove{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_remove::pressed{ border-image: url(:/0_common/rc/0_common/bt"
                        "nlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_test{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_test::checked{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_exit{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_exit::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_autotest{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_autotest::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; c"
                        "olor: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_save{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_save::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_layout{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_layout::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_style{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_style::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_"
                        "application{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_application::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_language{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_language::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_setting{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_setting::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_2_4_ok{ border-image: url(:/0_comm"
                        "on/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_2_4_ok::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_1_1_import{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_1_1_import::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_1_1_new{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_1_1_new::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_1_1_local{ border-image: url(:/0_common/rc/0_common/btndark.png); font: la"
                        "rge \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_1_1_local::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_3_return{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_3_return::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_autotest{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_autotest::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_2_1_create{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; col"
                        "or: rgba(198,210,225,250); }\n"
"QPushButton#btn_2_1_create::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_2_1_return{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_2_1_return::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_2_2_return{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_2_2_return::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_2_21_return{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250)"
                        "; }\n"
"QPushButton#btn_2_21_return::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_32_remove{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_32_remove::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_32_text{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_32_text::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_wifi{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_wifi::pressed{ "
                        "border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_usb{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_usb::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_logindev{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_logindev::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_scandev{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_scandev::pressed{ border-image: url(:/0_common/rc/0_common/btnlight."
                        "png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_testdev{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_testdev::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_downloaddev{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_downloaddev::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_2_3_import{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_2_3_import::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; f"
                        "ont-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_2_3_return{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_2_3_return::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_2_3_selectfolder{ border-image: url(:/0_common/rc/0_common/btngo.png); }\n"
"QPushButton#btn_2_3_selectfolder::pressed{ border-image: url(:/0_common/rc/0_common/btngolight.png); }\n"
"\n"
"QPushButton#btn_screensize{ border-image: url(:/0_common/rc/0_common/check1.png); }\n"
"QPushButton#btn_screensize::checked{ border-image: url(:/0_common/rc/0_common/check2.png);  }\n"
"\n"
"QLabel#label_screenbig, QLabel#label_screensmall { font: large \"Arial\"; font-size: 25pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_31_return{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; fo"
                        "nt-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_31_return::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_32_create{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_32_create::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_32_import{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_32_import::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_32_return{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,2"
                        "10,225,250); }\n"
"QPushButton#btn_32_return::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_32_save{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_32_save::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_33_apply{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_33_apply::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_323_save{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_323_"
                        "save::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_32_1_return{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_32_1_return::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_32_1_import{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_32_1_import::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_32_1_selectfolder{ border-image: url(:/0_common/rc/0_common/btngo.png); }\n"
"QPushButton#btn_32_1_selectfolder::pressed{ border-image: url(:/0_common/rc/0_common/btngolight.png); }\n"
"\n"
""
                        "QPushButton#btn_32_1_selectfolder_pro{ border-image: url(:/0_common/rc/0_common/btngo.png); }\n"
"QPushButton#btn_32_1_selectfolder_pro::pressed{ border-image: url(:/0_common/rc/0_common/btngolight.png); }\n"
"\n"
"QPushButton#btn_32_1_selectzip{ border-image: url(:/0_common/rc/0_common/zip.png); }\n"
"QPushButton#btn_32_1_selectzip::pressed{ border-image: url(:/0_common/rc/0_common/ziplight.png); }\n"
"\n"
"QPushButton#btn_33_1_selectzip{ border-image: url(:/0_common/rc/0_common/zip.png); }\n"
"QPushButton#btn_33_1_selectzip::pressed{ border-image: url(:/0_common/rc/0_common/ziplight.png); }\n"
"\n"
"QPushButton#btn_32_1_import_project{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_32_1_import_project::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_33_2_selectzip{ border-image: url(:/0_common/rc/0"
                        "_common/zip.png); }\n"
"QPushButton#btn_33_2_selectzip::pressed{ border-image: url(:/0_common/rc/0_common/ziplight.png); }\n"
"\n"
"QPushButton#btn_32_2_return{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_32_2_return::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_32_2_next{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_32_2_next::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_32_2_save{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_32_2_save::pressed{ border-image: url(:/0_common/rc/0"
                        "_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_32_export{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_32_export::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_321_import{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_321_import::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_321_next{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_321_next::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: lar"
                        "ge \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_321_remove{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_321_remove::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_321_return{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_321_return::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_321_test{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_321_test::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt;"
                        " color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_322_import{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_322_import::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_322_next{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_322_next::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_322_remove{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_322_remove::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250);"
                        " }\n"
"\n"
"QPushButton#btn_322_return{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_322_return::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_322_test{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_322_test::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_323_import{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_323_import::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_32"
                        "3_next{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_323_next::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_323_remove{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_323_remove::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_323_return{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_323_return::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_323_test{ border-image: url(:/0_"
                        "common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_323_test::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_324_remove{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_324_remove::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_324_test{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_324_test::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_324_import{ border-image: url(:/0_common/rc/0_common/btndark.png);"
                        " font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_324_import::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_324_return{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_324_return::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_324_save{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_324_save::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_33_import{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-si"
                        "ze: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_33_import::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_33_test{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_33_test::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_33_addfile{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_33_addfile::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_33_delfile{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225"
                        ",250); }\n"
"QPushButton#btn_33_delfile::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_33_new{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_33_new::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_33_delete{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_33_delete::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_33_save{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_33_save::p"
                        "ressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_33_return{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_33_return::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_33_1_return{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_33_1_return::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_33_1_import{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_33_1_import::pressed{ border-image: ur"
                        "l(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_33_1_selectfolder{ border-image: url(:/0_common/rc/0_common/btngo.png); }\n"
"QPushButton#btn_33_1_selectfolder::pressed{ border-image: url(:/0_common/rc/0_common/btngolight.png); }\n"
"\n"
"QPushButton#btn_33_2_return{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_33_2_return::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_33_2_next{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_33_2_next::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_33_2_import{ bo"
                        "rder-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_33_2_import::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_33_2_selectfolder{ border-image: url(:/0_common/rc/0_common/btngo.png); }\n"
"QPushButton#btn_33_2_selectfolder::pressed{ border-image: url(:/0_common/rc/0_common/btngolight.png); }\n"
"\n"
"\n"
"QPushButton#btn_2_4_return{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_2_4_return::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_342_ok{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_342_o"
                        "k::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QLabel#label_3_ssid, QLabel#label_3_passwd { font: large \"Arial\"; font-size: 14pt; color: rgba(255,255,225,250); }\n"
"\n"
"QLineEdit#edit_2_1_name, QLineEdit#edit_2_3_name, QLineEdit#edit_2_3_path, QLineEdit#edit_32_1_name, QLineEdit#edit_32_1_path, QLineEdit#edit_32_1_path_pro, QLineEdit#edit_32_2_name, QLineEdit#edit_323_text1, QLineEdit#edit_323_text2, QLineEdit#edit_33_1_name, QLineEdit#edit_33_1_path, QLineEdit#edit_33_2_path, QLineEdit#edit_33_2_name, QLineEdit#edit_wifipasswd, QLineEdit#edit_wifiip, QLineEdit#edit_321_cur, QLineEdit#edit_322_cur, QLineEdit#edit_323_cur, QLineEdit#edit_324_cur, QLineEdit#edit_ssid, QLineEdit#edit_passwd { font: large \"Arial\"; font-size: 25pt; color: rgba(255,255,225,250); border-image: url(:/0_common/rc/0_common/input.png); }\n"
"\n"
"QLabel#label_1_title,QLabel#label_1_1_title, QLabel#label_2_title, QLabel#label_3_titl"
                        "e, QLabel#label_2_1_title, QLabel#label_2_2_title, QLabel#label_2_3_title, QLabel#label_2_4_title , QLabel#label_31_title, QLabel#label_32_title, QLabel#label_32_1_title,  QLabel#label_32_1_title_pro, QLabel#label_32_2_title, QLabel#label_321_title, QLabel#label_322_title, QLabel#label_323_title, QLabel#label_324_title, QLabel#label_33_title, QLabel#label_33_1_title, QLabel#label_33_2_title, QLabel#label_2_21_title { font: bold large \"Arial\"; font-size: 38pt; color: rgba(198,210,225,250); }\n"
"\n"
"QLabel#label_2_preview, QLabel#label_3_preview, QLabel#label_31_preview, QLabel#label_32_preview, QLabel#label_321_preview, QLabel#label_322_preview, QLabel#label_323_preview, QLabel#label_324_preview { font: large \"Arial\"; font-size: 25pt; color: rgba(198,210,225,250); }\n"
"\n"
"QLabel#label_info_name_1, QLabel#label_info_appver_1, QLabel#label_info_sysver_1, QLabel#label_info_devtype_1 { font: large \"Arial\"; font-size: 20pt; color: rgba(200,200,200,250); }\n"
"QLabel#label_info_name_2, QLabel#label_info_ap"
                        "pver_2, QLabel#label_info_sysver_2, QLabel#label_info_devtype_2 { font: large \"Arial\"; font-size: 14pt; color: rgba(255,255,225,250); }\n"
"\n"
"QLabel#label_2_1_name, QLabel#label_2_3_name, QLabel#label_2_3_path, QLabel#label_32_1_name, QLabel#label_32_1_path,  QLabel#label_32_1_path_pro, QLabel#label_32_2_name, QLabel#label_33_1_name, QLabel#label_33_1_path,QLabel#label_33_2_name, QLabel#label_33_2_path, QLabel#label_wifipasswd { font: large \"Arial\"; font-size: 25pt; color: rgba(255,255,225,250); }\n"
"\n"
"QLabel#label_323_text1, QLabel#label_323_text2 { font: large \"Arial\"; font-size: 20pt; color: rgba(255,255,225,250); }\n"
"\n"
"QLabel#label_version{ font: large \"Arial\"; font-size: 20pt; color: rgba(255,255,225,250); }\n"
"\n"
"QLabel#label_backlight_enable, QLabel#label_volume_enable { font: large \"Arial\"; font-size: 20pt; color: rgba(255,255,225,250); }\n"
"\n"
"QRadioButton { font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QRadioButton::indicator { width: 30px; he"
                        "ight: 30px;  }\n"
"QRadioButton::indicator::unchecked { image: url(:/0_common/rc/0_common/radio.png); }\n"
"QRadioButton::indicator::checked { image: url(:/0_common/rc/0_common/radiolight.png); }\n"
"\n"
"QCheckBox { font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QCheckBox::indicator { width: 60px; height: 60px;  }\n"
"QCheckBox::indicator::unchecked { image: url(:/0_common/rc/0_common/checkoff.png); }\n"
"QCheckBox::indicator::checked { image: url(:/0_common/rc/0_common/checkon.png); }\n"
"\n"
"QPushButton#btn_34_backlight{ border-image: url(:/0_common/rc/0_common/btndark1.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_34_backlight::checked{ border-image: url(:/0_common/rc/0_common/btnlight1.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_34_floors{ border-image: url(:/0_common/rc/0_common/btndark1.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
""
                        "QPushButton#btn_34_floors::checked{ border-image: url(:/0_common/rc/0_common/btnlight1.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_34_logo{ border-image: url(:/0_common/rc/0_common/btndark1.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_34_logo::checked{ border-image: url(:/0_common/rc/0_common/btnlight1.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_34_volume{ border-image: url(:/0_common/rc/0_common/btndark1.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_34_volume::checked{ border-image: url(:/0_common/rc/0_common/btnlight1.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_34_wifi{ border-image: url(:/0_common/rc/0_common/btndark1.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_34_wifi::checked"
                        "{ border-image: url(:/0_common/rc/0_common/btnlight1.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_34_save{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_34_save::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_34_return{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_34_return::pressed{ border-image: url(:/0_common/rc/0_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QPushButton#btn_2_4_ok_md{ border-image: url(:/0_common/rc/0_common/btndark.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"QPushButton#btn_2_4_ok_md::pressed{ border-image: url(:/0_common/rc/0"
                        "_common/btnlight.png); font: large \"Arial\"; font-size: 20pt; color: rgba(198,210,225,250); }\n"
"\n"
"QSlider::groove:horizontal {  border: 1px rgba(254, 158, 37, 255);  height: 2px;  margin: 0px 0; left: 22px; right: 22px;  }\n"
"QSlider::handle:horizontal {   border-image: url(:/0_common/rc/0_common/sliderdot.png);  width: 40px; height : 40px;  margin: -20px -20px -20px -20px; }\n"
"QSlider::sub-page:horizontal{  background: rgba(254, 158, 37, 255); }\n"
"QSlider::add-page:horizontal{ background: rgba(100, 100, 100, 255); }\n"
"QSlider::indicator { right : 22px}\n"
"\n"
"QStackedWidget#SettingStacked { border: 1px solid rgba(198,210,225, 150); }\n"
"\n"
"QPushButton#btn_341_confirm{ border-image: url(:/0_common/rc/0_common/checkoff.png); }\n"
"QPushButton#btn_341_confirm::checked{ border-image: url(:/0_common/rc/0_common/checkon.png); }\n"
"\n"
"QPushButton#btn_344_select{ border:none; font: large \"Arial\"; font-size: 25pt; color: rgba(198,210,225,255); }\n"
"QPushButton#btn_344_select::checked{ border:no"
                        "ne; font: large \"Arial\"; font-size: 25pt; color: rgba(255,255,225,255); }\n"
"\n"
"QLabel#label_341_title, QLabel#label_344_title { font: large \"Arial\"; font-size: 25pt; color: rgba(255,255,225,250); }\n"
"QLabel#label_msg, QLabel#label_gong, QLabel#label_music { font: large \"Arial\"; font-size: 22pt; color: rgba(255,255,225,250); }\n"
"QLabel#label_msg_icon1, QLabel#label_gong_icon1, QLabel#label_music_icon1 { border-image: url(:/0_common/rc/0_common/voldel.png); }\n"
"QLabel#label_msg_icon2, QLabel#label_gong_icon2, QLabel#label_music_icon2 { border-image: url(:/0_common/rc/0_common/voladd.png); }\n"
"\n"
"QLabel#label_normal, QLabel#label_standby, QLabel#label_delay { font: large \"Arial\"; font-size: 22pt; color: rgba(255,255,225,250); }\n"
"QLabel#label_normal_max, QLabel#label_normal_min, QLabel#label_standby_max, QLabel#label_standby_min, QLabel#label_delay_max, QLabel#label_delay_min, QLabel#label_standby_unit { font: large \"Arial\"; font-size: 17pt; color: rgba(255,255,225,250); }\n"
"\n"
"QLabe"
                        "l#label_msg_value, QLabel#label_gong_value, QLabel#label_music_value, QLabel#label_arrowspeed { font: large \"Arial\"; font-size: 17pt; color: rgba(255,255,225,250); }\n"
"\n"
"QLabel#label_2_1_devtype{ font: large \"Arial\"; font-size: 25pt; color: rgba(255,255,225,250); }\n"
"\n"
"QLabel#label_flrmap{ font: large \"Arial\"; font-size: 25pt; color: rgba(255,255,225,250); }\n"
"\n"
"\n"
"QLabel#label_33_langlist, QLabel#label_33_audiolist{ font: large \"Arial\"; font-size: 20pt; color: rgba(255,255,225,250); }\n"
"\n"
"QGroupBox {font: large \"Arial\"; font-size: 20pt; color: rgba(255,255,225,250);}\n"
""));
        verticalLayout = new QVBoxLayout(DeviceManageForm);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        MainStackedWidget = new QStackedWidget(DeviceManageForm);
        MainStackedWidget->setObjectName(QStringLiteral("MainStackedWidget"));
        page1_language = new QWidget();
        page1_language->setObjectName(QStringLiteral("page1_language"));
        label_1_title = new QLabel(page1_language);
        label_1_title->setObjectName(QStringLiteral("label_1_title"));
        label_1_title->setGeometry(QRect(120, 110, 811, 71));
        lang_ned_banner = new QPushButton(page1_language);
        lang_ned_banner->setObjectName(QStringLiteral("lang_ned_banner"));
        lang_ned_banner->setGeometry(QRect(609, 377, 111, 77));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lang_ned_banner->sizePolicy().hasHeightForWidth());
        lang_ned_banner->setSizePolicy(sizePolicy);
        lang_ned_banner->setCheckable(true);
        lang_en_banner = new QPushButton(page1_language);
        lang_en_banner->setObjectName(QStringLiteral("lang_en_banner"));
        lang_en_banner->setGeometry(QRect(280, 377, 111, 77));
        sizePolicy.setHeightForWidth(lang_en_banner->sizePolicy().hasHeightForWidth());
        lang_en_banner->setSizePolicy(sizePolicy);
        lang_en_banner->setCheckable(true);
        lang_deu_banner = new QPushButton(page1_language);
        lang_deu_banner->setObjectName(QStringLiteral("lang_deu_banner"));
        lang_deu_banner->setGeometry(QRect(500, 377, 111, 77));
        sizePolicy.setHeightForWidth(lang_deu_banner->sizePolicy().hasHeightForWidth());
        lang_deu_banner->setSizePolicy(sizePolicy);
        lang_deu_banner->setCheckable(true);
        lang_fra_banner = new QPushButton(page1_language);
        lang_fra_banner->setObjectName(QStringLiteral("lang_fra_banner"));
        lang_fra_banner->setGeometry(QRect(390, 377, 111, 77));
        sizePolicy.setHeightForWidth(lang_fra_banner->sizePolicy().hasHeightForWidth());
        lang_fra_banner->setSizePolicy(sizePolicy);
        lang_fra_banner->setCheckable(true);
        label_logo = new QLabel(page1_language);
        label_logo->setObjectName(QStringLiteral("label_logo"));
        label_logo->setGeometry(QRect(409, 473, 165, 31));
        label_version = new QLabel(page1_language);
        label_version->setObjectName(QStringLiteral("label_version"));
        label_version->setGeometry(QRect(700, 450, 241, 31));
        label_version->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        btn_1_1_local = new QPushButton(page1_language);
        btn_1_1_local->setObjectName(QStringLiteral("btn_1_1_local"));
        btn_1_1_local->setGeometry(QRect(40, 250, 271, 91));
        btn_1_1_new = new QPushButton(page1_language);
        btn_1_1_new->setObjectName(QStringLiteral("btn_1_1_new"));
        btn_1_1_new->setGeometry(QRect(310, 250, 271, 91));
        btn_download = new QPushButton(page1_language);
        btn_download->setObjectName(QStringLiteral("btn_download"));
        btn_download->setGeometry(QRect(580, 250, 378, 91));
        MainStackedWidget->addWidget(page1_language);
        page2_mainpage = new QWidget();
        page2_mainpage->setObjectName(QStringLiteral("page2_mainpage"));
        label_2_title = new QLabel(page2_mainpage);
        label_2_title->setObjectName(QStringLiteral("label_2_title"));
        label_2_title->setGeometry(QRect(30, 20, 501, 76));
        img_2_preview = new QLabel(page2_mainpage);
        img_2_preview->setObjectName(QStringLiteral("img_2_preview"));
        img_2_preview->setGeometry(QRect(30, 168, 480, 272));
        img_2_preview->setMinimumSize(QSize(480, 272));
        img_2_preview->setMaximumSize(QSize(480, 272));
        label_2_preview = new QLabel(page2_mainpage);
        label_2_preview->setObjectName(QStringLiteral("label_2_preview"));
        label_2_preview->setGeometry(QRect(86, 110, 331, 51));
        label_2_preview->setAlignment(Qt::AlignCenter);
        btn_modify = new QPushButton(page2_mainpage);
        btn_modify->setObjectName(QStringLiteral("btn_modify"));
        btn_modify->setGeometry(QRect(320, 470, 146, 77));
        btn_remove = new QPushButton(page2_mainpage);
        btn_remove->setObjectName(QStringLiteral("btn_remove"));
        btn_remove->setGeometry(QRect(472, 470, 147, 76));
        btn_exit = new QPushButton(page2_mainpage);
        btn_exit->setObjectName(QStringLiteral("btn_exit"));
        btn_exit->setGeometry(QRect(20, 470, 131, 76));
        btn_export = new QPushButton(page2_mainpage);
        btn_export->setObjectName(QStringLiteral("btn_export"));
        btn_export->setGeometry(QRect(625, 470, 147, 76));
        btn_close = new QPushButton(page2_mainpage);
        btn_close->setObjectName(QStringLiteral("btn_close"));
        btn_close->setGeometry(QRect(780, 470, 191, 76));
        widget_2_info = new QWidget(page2_mainpage);
        widget_2_info->setObjectName(QStringLiteral("widget_2_info"));
        widget_2_info->setGeometry(QRect(590, 100, 381, 221));
        label_info_name_1 = new QLabel(widget_2_info);
        label_info_name_1->setObjectName(QStringLiteral("label_info_name_1"));
        label_info_name_1->setGeometry(QRect(0, 10, 120, 31));
        label_info_name_1->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_info_name_2 = new QLabel(widget_2_info);
        label_info_name_2->setObjectName(QStringLiteral("label_info_name_2"));
        label_info_name_2->setGeometry(QRect(130, 10, 241, 31));
        label_info_appver_2 = new QLabel(widget_2_info);
        label_info_appver_2->setObjectName(QStringLiteral("label_info_appver_2"));
        label_info_appver_2->setGeometry(QRect(130, 70, 241, 31));
        label_info_appver_1 = new QLabel(widget_2_info);
        label_info_appver_1->setObjectName(QStringLiteral("label_info_appver_1"));
        label_info_appver_1->setGeometry(QRect(0, 70, 120, 31));
        label_info_appver_1->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_info_sysver_2 = new QLabel(widget_2_info);
        label_info_sysver_2->setObjectName(QStringLiteral("label_info_sysver_2"));
        label_info_sysver_2->setGeometry(QRect(130, 130, 241, 31));
        label_info_sysver_1 = new QLabel(widget_2_info);
        label_info_sysver_1->setObjectName(QStringLiteral("label_info_sysver_1"));
        label_info_sysver_1->setGeometry(QRect(0, 130, 120, 31));
        label_info_sysver_1->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_info_devtype_2 = new QLabel(widget_2_info);
        label_info_devtype_2->setObjectName(QStringLiteral("label_info_devtype_2"));
        label_info_devtype_2->setGeometry(QRect(130, 190, 241, 31));
        label_info_devtype_1 = new QLabel(widget_2_info);
        label_info_devtype_1->setObjectName(QStringLiteral("label_info_devtype_1"));
        label_info_devtype_1->setGeometry(QRect(0, 190, 120, 31));
        label_info_devtype_1->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        btn_test = new QPushButton(page2_mainpage);
        btn_test->setObjectName(QStringLiteral("btn_test"));
        btn_test->setGeometry(QRect(620, 370, 251, 77));
        btn_test->setCheckable(true);
        MainStackedWidget->addWidget(page2_mainpage);
        page2_1_createpro = new QWidget();
        page2_1_createpro->setObjectName(QStringLiteral("page2_1_createpro"));
        label_2_1_title = new QLabel(page2_1_createpro);
        label_2_1_title->setObjectName(QStringLiteral("label_2_1_title"));
        label_2_1_title->setGeometry(QRect(30, 20, 791, 76));
        label_2_1_name = new QLabel(page2_1_createpro);
        label_2_1_name->setObjectName(QStringLiteral("label_2_1_name"));
        label_2_1_name->setGeometry(QRect(40, 250, 301, 41));
        label_2_1_name->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        edit_2_1_name = new QLineEdit(page2_1_createpro);
        edit_2_1_name->setObjectName(QStringLiteral("edit_2_1_name"));
        edit_2_1_name->setGeometry(QRect(350, 240, 411, 61));
        btn_2_1_return = new QPushButton(page2_1_createpro);
        btn_2_1_return->setObjectName(QStringLiteral("btn_2_1_return"));
        btn_2_1_return->setGeometry(QRect(750, 470, 191, 71));
        btn_2_1_create = new QPushButton(page2_1_createpro);
        btn_2_1_create->setObjectName(QStringLiteral("btn_2_1_create"));
        btn_2_1_create->setGeometry(QRect(550, 470, 191, 71));
        label_2_1_devtype = new QLabel(page2_1_createpro);
        label_2_1_devtype->setObjectName(QStringLiteral("label_2_1_devtype"));
        label_2_1_devtype->setGeometry(QRect(40, 360, 301, 41));
        label_2_1_devtype->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        radio_canopen = new QRadioButton(page2_1_createpro);
        radio_canopen->setObjectName(QStringLiteral("radio_canopen"));
        radio_canopen->setGeometry(QRect(500, 360, 191, 41));
        radio_saf = new QRadioButton(page2_1_createpro);
        radio_saf->setObjectName(QStringLiteral("radio_saf"));
        radio_saf->setGeometry(QRect(360, 360, 111, 41));
        radio_saf->setChecked(true);
        MainStackedWidget->addWidget(page2_1_createpro);
        page2_2_downloadpro = new QWidget();
        page2_2_downloadpro->setObjectName(QStringLiteral("page2_2_downloadpro"));
        label_2_2_title = new QLabel(page2_2_downloadpro);
        label_2_2_title->setObjectName(QStringLiteral("label_2_2_title"));
        label_2_2_title->setGeometry(QRect(20, 20, 821, 76));
        btn_wifi = new QPushButton(page2_2_downloadpro);
        btn_wifi->setObjectName(QStringLiteral("btn_wifi"));
        btn_wifi->setGeometry(QRect(400, 190, 191, 81));
        btn_usb = new QPushButton(page2_2_downloadpro);
        btn_usb->setObjectName(QStringLiteral("btn_usb"));
        btn_usb->setGeometry(QRect(400, 293, 191, 78));
        btn_2_2_return = new QPushButton(page2_2_downloadpro);
        btn_2_2_return->setObjectName(QStringLiteral("btn_2_2_return"));
        btn_2_2_return->setGeometry(QRect(770, 470, 191, 71));
        MainStackedWidget->addWidget(page2_2_downloadpro);
        page2_4_simplepro = new QWidget();
        page2_4_simplepro->setObjectName(QStringLiteral("page2_4_simplepro"));
        label_2_4_title = new QLabel(page2_4_simplepro);
        label_2_4_title->setObjectName(QStringLiteral("label_2_4_title"));
        label_2_4_title->setGeometry(QRect(20, 30, 781, 76));
        check_application = new QCheckBox(page2_4_simplepro);
        check_application->setObjectName(QStringLiteral("check_application"));
        check_application->setGeometry(QRect(130, 120, 400, 61));
        check_conffile = new QCheckBox(page2_4_simplepro);
        check_conffile->setObjectName(QStringLiteral("check_conffile"));
        check_conffile->setGeometry(QRect(130, 190, 400, 61));
        check_sysfile = new QCheckBox(page2_4_simplepro);
        check_sysfile->setObjectName(QStringLiteral("check_sysfile"));
        check_sysfile->setGeometry(QRect(130, 260, 400, 61));
        check_style = new QCheckBox(page2_4_simplepro);
        check_style->setObjectName(QStringLiteral("check_style"));
        check_style->setGeometry(QRect(130, 330, 400, 61));
        btn_2_4_ok = new QPushButton(page2_4_simplepro);
        btn_2_4_ok->setObjectName(QStringLiteral("btn_2_4_ok"));
        btn_2_4_ok->setGeometry(QRect(510, 470, 231, 71));
        check_audio = new QCheckBox(page2_4_simplepro);
        check_audio->setObjectName(QStringLiteral("check_audio"));
        check_audio->setGeometry(QRect(130, 400, 400, 61));
        check_all = new QCheckBox(page2_4_simplepro);
        check_all->setObjectName(QStringLiteral("check_all"));
        check_all->setGeometry(QRect(540, 120, 400, 61));
        btn_2_4_return = new QPushButton(page2_4_simplepro);
        btn_2_4_return->setObjectName(QStringLiteral("btn_2_4_return"));
        btn_2_4_return->setGeometry(QRect(760, 470, 191, 71));
        btn_2_4_ok_md = new QPushButton(page2_4_simplepro);
        btn_2_4_ok_md->setObjectName(QStringLiteral("btn_2_4_ok_md"));
        btn_2_4_ok_md->setGeometry(QRect(290, 470, 201, 71));
        MainStackedWidget->addWidget(page2_4_simplepro);
        page2_21_fromrwifi = new QWidget();
        page2_21_fromrwifi->setObjectName(QStringLiteral("page2_21_fromrwifi"));
        list_device = new QListWidget(page2_21_fromrwifi);
        list_device->setObjectName(QStringLiteral("list_device"));
        list_device->setGeometry(QRect(60, 100, 501, 411));
        label_2_21_title = new QLabel(page2_21_fromrwifi);
        label_2_21_title->setObjectName(QStringLiteral("label_2_21_title"));
        label_2_21_title->setGeometry(QRect(30, 20, 701, 76));
        btn_scandev = new QPushButton(page2_21_fromrwifi);
        btn_scandev->setObjectName(QStringLiteral("btn_scandev"));
        btn_scandev->setGeometry(QRect(600, 70, 269, 81));
        edit_wifipasswd = new QLineEdit(page2_21_fromrwifi);
        edit_wifipasswd->setObjectName(QStringLiteral("edit_wifipasswd"));
        edit_wifipasswd->setGeometry(QRect(640, 220, 231, 51));
        btn_2_21_return = new QPushButton(page2_21_fromrwifi);
        btn_2_21_return->setObjectName(QStringLiteral("btn_2_21_return"));
        btn_2_21_return->setGeometry(QRect(700, 450, 266, 91));
        label_wifipasswd = new QLabel(page2_21_fromrwifi);
        label_wifipasswd->setObjectName(QStringLiteral("label_wifipasswd"));
        label_wifipasswd->setGeometry(QRect(600, 160, 191, 51));
        btn_logindev = new QPushButton(page2_21_fromrwifi);
        btn_logindev->setObjectName(QStringLiteral("btn_logindev"));
        btn_logindev->setGeometry(QRect(610, 350, 269, 81));
        edit_wifiip = new QLineEdit(page2_21_fromrwifi);
        edit_wifiip->setObjectName(QStringLiteral("edit_wifiip"));
        edit_wifiip->setGeometry(QRect(640, 290, 231, 51));
        MainStackedWidget->addWidget(page2_21_fromrwifi);
        page2_3_importpro = new QWidget();
        page2_3_importpro->setObjectName(QStringLiteral("page2_3_importpro"));
        label_2_3_title = new QLabel(page2_3_importpro);
        label_2_3_title->setObjectName(QStringLiteral("label_2_3_title"));
        label_2_3_title->setGeometry(QRect(20, 20, 791, 76));
        label_2_3_name = new QLabel(page2_3_importpro);
        label_2_3_name->setObjectName(QStringLiteral("label_2_3_name"));
        label_2_3_name->setGeometry(QRect(150, 170, 131, 41));
        edit_2_3_name = new QLineEdit(page2_3_importpro);
        edit_2_3_name->setObjectName(QStringLiteral("edit_2_3_name"));
        edit_2_3_name->setGeometry(QRect(285, 160, 409, 63));
        label_2_3_path = new QLabel(page2_3_importpro);
        label_2_3_path->setObjectName(QStringLiteral("label_2_3_path"));
        label_2_3_path->setGeometry(QRect(50, 250, 231, 41));
        edit_2_3_path = new QLineEdit(page2_3_importpro);
        edit_2_3_path->setObjectName(QStringLiteral("edit_2_3_path"));
        edit_2_3_path->setGeometry(QRect(285, 240, 406, 61));
        btn_2_3_selectfolder = new QPushButton(page2_3_importpro);
        btn_2_3_selectfolder->setObjectName(QStringLiteral("btn_2_3_selectfolder"));
        btn_2_3_selectfolder->setGeometry(QRect(692, 239, 58, 60));
        btn_2_3_import = new QPushButton(page2_3_importpro);
        btn_2_3_import->setObjectName(QStringLiteral("btn_2_3_import"));
        btn_2_3_import->setGeometry(QRect(770, 240, 191, 71));
        btn_2_3_return = new QPushButton(page2_3_importpro);
        btn_2_3_return->setObjectName(QStringLiteral("btn_2_3_return"));
        btn_2_3_return->setGeometry(QRect(773, 470, 193, 74));
        MainStackedWidget->addWidget(page2_3_importpro);
        page3_modify = new QWidget();
        page3_modify->setObjectName(QStringLiteral("page3_modify"));
        label_3_title = new QLabel(page3_modify);
        label_3_title->setObjectName(QStringLiteral("label_3_title"));
        label_3_title->setGeometry(QRect(30, 10, 731, 76));
        btn_style = new QPushButton(page3_modify);
        btn_style->setObjectName(QStringLiteral("btn_style"));
        btn_style->setGeometry(QRect(205, 468, 194, 76));
        sizePolicy.setHeightForWidth(btn_style->sizePolicy().hasHeightForWidth());
        btn_style->setSizePolicy(sizePolicy);
        btn_setting = new QPushButton(page3_modify);
        btn_setting->setObjectName(QStringLiteral("btn_setting"));
        btn_setting->setGeometry(QRect(592, 468, 192, 76));
        sizePolicy.setHeightForWidth(btn_setting->sizePolicy().hasHeightForWidth());
        btn_setting->setSizePolicy(sizePolicy);
        btn_3_return = new QPushButton(page3_modify);
        btn_3_return->setObjectName(QStringLiteral("btn_3_return"));
        btn_3_return->setGeometry(QRect(784, 468, 193, 77));
        sizePolicy.setHeightForWidth(btn_3_return->sizePolicy().hasHeightForWidth());
        btn_3_return->setSizePolicy(sizePolicy);
        btn_language = new QPushButton(page3_modify);
        btn_language->setObjectName(QStringLiteral("btn_language"));
        btn_language->setGeometry(QRect(398, 468, 193, 77));
        sizePolicy.setHeightForWidth(btn_language->sizePolicy().hasHeightForWidth());
        btn_language->setSizePolicy(sizePolicy);
        btn_layout = new QPushButton(page3_modify);
        btn_layout->setObjectName(QStringLiteral("btn_layout"));
        btn_layout->setGeometry(QRect(10, 468, 194, 76));
        sizePolicy.setHeightForWidth(btn_layout->sizePolicy().hasHeightForWidth());
        btn_layout->setSizePolicy(sizePolicy);
        label_3_preview = new QLabel(page3_modify);
        label_3_preview->setObjectName(QStringLiteral("label_3_preview"));
        label_3_preview->setGeometry(QRect(550, 90, 300, 51));
        img_3_preview = new QLabel(page3_modify);
        img_3_preview->setObjectName(QStringLiteral("img_3_preview"));
        img_3_preview->setGeometry(QRect(370, 150, 480, 272));
        img_3_preview->setMinimumSize(QSize(480, 272));
        img_3_preview->setMaximumSize(QSize(480, 272));
        btn_explorer = new QPushButton(page3_modify);
        btn_explorer->setObjectName(QStringLiteral("btn_explorer"));
        btn_explorer->setGeometry(QRect(-200, 380, 191, 78));
        sizePolicy.setHeightForWidth(btn_explorer->sizePolicy().hasHeightForWidth());
        btn_explorer->setSizePolicy(sizePolicy);
        btn_explorer->setMinimumSize(QSize(0, 0));
        btn_explorer->setMaximumSize(QSize(20000, 20000));
        stacked_arrowspeed = new QStackedWidget(page3_modify);
        stacked_arrowspeed->setObjectName(QStringLiteral("stacked_arrowspeed"));
        stacked_arrowspeed->setGeometry(QRect(30, 250, 321, 131));
        page31_saf = new QWidget();
        page31_saf->setObjectName(QStringLiteral("page31_saf"));
        groupBox = new QGroupBox(page31_saf);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(0, 10, 311, 101));
        groupBox->setFlat(true);
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setSpacing(9);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(9, 9, 9, 9);
        combo_arrowspeed = new QComboBox(groupBox);
        combo_arrowspeed->setObjectName(QStringLiteral("combo_arrowspeed"));
        combo_arrowspeed->setMinimumSize(QSize(0, 35));
        combo_arrowspeed->setMaximumSize(QSize(16777215, 35));

        horizontalLayout->addWidget(combo_arrowspeed);

        stacked_arrowspeed->addWidget(page31_saf);
        page31_com = new QWidget();
        page31_com->setObjectName(QStringLiteral("page31_com"));
        stacked_arrowspeed->addWidget(page31_com);
        btn_application = new QPushButton(page3_modify);
        btn_application->setObjectName(QStringLiteral("btn_application"));
        btn_application->setGeometry(QRect(30, 150, 321, 76));
        sizePolicy.setHeightForWidth(btn_application->sizePolicy().hasHeightForWidth());
        btn_application->setSizePolicy(sizePolicy);
        MainStackedWidget->addWidget(page3_modify);
        page31_layout = new QWidget();
        page31_layout->setObjectName(QStringLiteral("page31_layout"));
        label_31_title = new QLabel(page31_layout);
        label_31_title->setObjectName(QStringLiteral("label_31_title"));
        label_31_title->setGeometry(QRect(30, 10, 721, 76));
        label_31_preview = new QLabel(page31_layout);
        label_31_preview->setObjectName(QStringLiteral("label_31_preview"));
        label_31_preview->setGeometry(QRect(200, 110, 300, 51));
        img_31_preview = new QLabel(page31_layout);
        img_31_preview->setObjectName(QStringLiteral("img_31_preview"));
        img_31_preview->setGeometry(QRect(30, 170, 480, 272));
        img_31_preview->setMinimumSize(QSize(480, 272));
        img_31_preview->setMaximumSize(QSize(480, 272));
        radio_srn_arrow = new QRadioButton(page31_layout);
        radio_srn_arrow->setObjectName(QStringLiteral("radio_srn_arrow"));
        radio_srn_arrow->setGeometry(QRect(560, 277, 321, 41));
        radio_srn_arrow->setLayoutDirection(Qt::RightToLeft);
        radio_srn_arrow->setChecked(false);
        radio_srn_floor = new QRadioButton(page31_layout);
        radio_srn_floor->setObjectName(QStringLiteral("radio_srn_floor"));
        radio_srn_floor->setGeometry(QRect(560, 330, 321, 41));
        radio_srn_floor->setLayoutDirection(Qt::RightToLeft);
        radio_srn_floor->setChecked(false);
        btn_31_return = new QPushButton(page31_layout);
        btn_31_return->setObjectName(QStringLiteral("btn_31_return"));
        btn_31_return->setGeometry(QRect(784, 468, 192, 76));
        sizePolicy.setHeightForWidth(btn_31_return->sizePolicy().hasHeightForWidth());
        btn_31_return->setSizePolicy(sizePolicy);
        radio_srn_4_3 = new QRadioButton(page31_layout);
        radio_srn_4_3->setObjectName(QStringLiteral("radio_srn_4_3"));
        radio_srn_4_3->setGeometry(QRect(550, 210, 151, 41));
        radio_srn_4_3->setLayoutDirection(Qt::RightToLeft);
        radio_srn_4_3->setChecked(false);
        radio_srn_4 = new QRadioButton(page31_layout);
        radio_srn_4->setObjectName(QStringLiteral("radio_srn_4"));
        radio_srn_4->setGeometry(QRect(730, 210, 151, 41));
        radio_srn_4->setLayoutDirection(Qt::RightToLeft);
        radio_srn_4->setChecked(false);
        radio_srn_180 = new QRadioButton(page31_layout);
        radio_srn_180->setObjectName(QStringLiteral("radio_srn_180"));
        radio_srn_180->setGeometry(QRect(660, 140, 101, 41));
        radio_srn_180->setLayoutDirection(Qt::RightToLeft);
        radio_srn_180->setChecked(false);
        radio_srn_90 = new QRadioButton(page31_layout);
        radio_srn_90->setObjectName(QStringLiteral("radio_srn_90"));
        radio_srn_90->setGeometry(QRect(550, 140, 91, 41));
        radio_srn_90->setLayoutDirection(Qt::RightToLeft);
        radio_srn_90->setChecked(false);
        radio_srn_270 = new QRadioButton(page31_layout);
        radio_srn_270->setObjectName(QStringLiteral("radio_srn_270"));
        radio_srn_270->setGeometry(QRect(780, 140, 101, 41));
        radio_srn_270->setLayoutDirection(Qt::RightToLeft);
        radio_srn_270->setChecked(true);
        MainStackedWidget->addWidget(page31_layout);
        page32_style = new QWidget();
        page32_style->setObjectName(QStringLiteral("page32_style"));
        label_32_preview = new QLabel(page32_style);
        label_32_preview->setObjectName(QStringLiteral("label_32_preview"));
        label_32_preview->setGeometry(QRect(200, 110, 300, 51));
        img_32_preview = new QLabel(page32_style);
        img_32_preview->setObjectName(QStringLiteral("img_32_preview"));
        img_32_preview->setGeometry(QRect(30, 170, 480, 272));
        img_32_preview->setMinimumSize(QSize(480, 272));
        img_32_preview->setMaximumSize(QSize(480, 272));
        label_32_title = new QLabel(page32_style);
        label_32_title->setObjectName(QStringLiteral("label_32_title"));
        label_32_title->setGeometry(QRect(30, 10, 681, 76));
        btn_32_import = new QPushButton(page32_style);
        btn_32_import->setObjectName(QStringLiteral("btn_32_import"));
        btn_32_import->setGeometry(QRect(580, 470, 191, 71));
        btn_32_return = new QPushButton(page32_style);
        btn_32_return->setObjectName(QStringLiteral("btn_32_return"));
        btn_32_return->setGeometry(QRect(784, 470, 191, 71));
        list_32_style = new QListWidget(page32_style);
        list_32_style->setObjectName(QStringLiteral("list_32_style"));
        list_32_style->setGeometry(QRect(560, 60, 401, 381));
        btn_32_export = new QPushButton(page32_style);
        btn_32_export->setObjectName(QStringLiteral("btn_32_export"));
        btn_32_export->setGeometry(QRect(400, 470, 171, 71));
        btn_32_remove = new QPushButton(page32_style);
        btn_32_remove->setObjectName(QStringLiteral("btn_32_remove"));
        btn_32_remove->setGeometry(QRect(10, 470, 181, 71));
        btn_32_text = new QPushButton(page32_style);
        btn_32_text->setObjectName(QStringLiteral("btn_32_text"));
        btn_32_text->setGeometry(QRect(200, 470, 191, 71));
        MainStackedWidget->addWidget(page32_style);
        page32_1_import = new QWidget();
        page32_1_import->setObjectName(QStringLiteral("page32_1_import"));
        label_32_1_title = new QLabel(page32_1_import);
        label_32_1_title->setObjectName(QStringLiteral("label_32_1_title"));
        label_32_1_title->setGeometry(QRect(20, 10, 731, 76));
        edit_32_1_name = new QLineEdit(page32_1_import);
        edit_32_1_name->setObjectName(QStringLiteral("edit_32_1_name"));
        edit_32_1_name->setGeometry(QRect(287, 160, 341, 61));
        label_32_1_name = new QLabel(page32_1_import);
        label_32_1_name->setObjectName(QStringLiteral("label_32_1_name"));
        label_32_1_name->setGeometry(QRect(50, 170, 231, 41));
        label_32_1_name->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        btn_32_1_import = new QPushButton(page32_1_import);
        btn_32_1_import->setObjectName(QStringLiteral("btn_32_1_import"));
        btn_32_1_import->setGeometry(QRect(770, 240, 191, 71));
        label_32_1_path = new QLabel(page32_1_import);
        label_32_1_path->setObjectName(QStringLiteral("label_32_1_path"));
        label_32_1_path->setGeometry(QRect(50, 250, 231, 41));
        label_32_1_path->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        edit_32_1_path = new QLineEdit(page32_1_import);
        edit_32_1_path->setObjectName(QStringLiteral("edit_32_1_path"));
        edit_32_1_path->setGeometry(QRect(286, 240, 341, 61));
        btn_32_1_selectfolder = new QPushButton(page32_1_import);
        btn_32_1_selectfolder->setObjectName(QStringLiteral("btn_32_1_selectfolder"));
        btn_32_1_selectfolder->setGeometry(QRect(691, 240, 61, 59));
        btn_32_1_return = new QPushButton(page32_1_import);
        btn_32_1_return->setObjectName(QStringLiteral("btn_32_1_return"));
        btn_32_1_return->setGeometry(QRect(770, 470, 201, 71));
        btn_32_1_selectzip = new QPushButton(page32_1_import);
        btn_32_1_selectzip->setObjectName(QStringLiteral("btn_32_1_selectzip"));
        btn_32_1_selectzip->setGeometry(QRect(630, 240, 61, 59));
        btn_32_1_import_project = new QPushButton(page32_1_import);
        btn_32_1_import_project->setObjectName(QStringLiteral("btn_32_1_import_project"));
        btn_32_1_import_project->setGeometry(QRect(770, 320, 191, 71));
        edit_32_1_path_pro = new QLineEdit(page32_1_import);
        edit_32_1_path_pro->setObjectName(QStringLiteral("edit_32_1_path_pro"));
        edit_32_1_path_pro->setGeometry(QRect(286, 320, 391, 61));
        label_32_1_path_pro = new QLabel(page32_1_import);
        label_32_1_path_pro->setObjectName(QStringLiteral("label_32_1_path_pro"));
        label_32_1_path_pro->setGeometry(QRect(52, 330, 231, 41));
        label_32_1_path_pro->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        btn_32_1_selectfolder_pro = new QPushButton(page32_1_import);
        btn_32_1_selectfolder_pro->setObjectName(QStringLiteral("btn_32_1_selectfolder_pro"));
        btn_32_1_selectfolder_pro->setGeometry(QRect(690, 320, 61, 59));
        MainStackedWidget->addWidget(page32_1_import);
        page32_2_create = new QWidget();
        page32_2_create->setObjectName(QStringLiteral("page32_2_create"));
        label_32_2_title = new QLabel(page32_2_create);
        label_32_2_title->setObjectName(QStringLiteral("label_32_2_title"));
        label_32_2_title->setGeometry(QRect(20, 10, 791, 71));
        edit_32_2_name = new QLineEdit(page32_2_create);
        edit_32_2_name->setObjectName(QStringLiteral("edit_32_2_name"));
        edit_32_2_name->setGeometry(QRect(350, 240, 411, 61));
        label_32_2_name = new QLabel(page32_2_create);
        label_32_2_name->setObjectName(QStringLiteral("label_32_2_name"));
        label_32_2_name->setGeometry(QRect(110, 250, 231, 41));
        label_32_2_name->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        btn_32_2_return = new QPushButton(page32_2_create);
        btn_32_2_return->setObjectName(QStringLiteral("btn_32_2_return"));
        btn_32_2_return->setGeometry(QRect(390, 470, 191, 71));
        btn_32_2_next = new QPushButton(page32_2_create);
        btn_32_2_next->setObjectName(QStringLiteral("btn_32_2_next"));
        btn_32_2_next->setGeometry(QRect(776, 470, 191, 71));
        btn_32_2_save = new QPushButton(page32_2_create);
        btn_32_2_save->setObjectName(QStringLiteral("btn_32_2_save"));
        btn_32_2_save->setGeometry(QRect(580, 470, 191, 71));
        MainStackedWidget->addWidget(page32_2_create);
        page321_floor = new QWidget();
        page321_floor->setObjectName(QStringLiteral("page321_floor"));
        img_321_preview = new QLabel(page321_floor);
        img_321_preview->setObjectName(QStringLiteral("img_321_preview"));
        img_321_preview->setGeometry(QRect(30, 170, 480, 272));
        img_321_preview->setMinimumSize(QSize(480, 272));
        img_321_preview->setMaximumSize(QSize(480, 272));
        btn_321_next = new QPushButton(page321_floor);
        btn_321_next->setObjectName(QStringLiteral("btn_321_next"));
        btn_321_next->setGeometry(QRect(780, 470, 193, 77));
        sizePolicy.setHeightForWidth(btn_321_next->sizePolicy().hasHeightForWidth());
        btn_321_next->setSizePolicy(sizePolicy);
        label_321_title = new QLabel(page321_floor);
        label_321_title->setObjectName(QStringLiteral("label_321_title"));
        label_321_title->setGeometry(QRect(30, 10, 811, 71));
        label_321_preview = new QLabel(page321_floor);
        label_321_preview->setObjectName(QStringLiteral("label_321_preview"));
        label_321_preview->setGeometry(QRect(200, 110, 300, 51));
        btn_321_return = new QPushButton(page321_floor);
        btn_321_return->setObjectName(QStringLiteral("btn_321_return"));
        btn_321_return->setGeometry(QRect(590, 470, 193, 77));
        sizePolicy.setHeightForWidth(btn_321_return->sizePolicy().hasHeightForWidth());
        btn_321_return->setSizePolicy(sizePolicy);
        btn_321_import = new QPushButton(page321_floor);
        btn_321_import->setObjectName(QStringLiteral("btn_321_import"));
        btn_321_import->setGeometry(QRect(400, 470, 193, 77));
        sizePolicy.setHeightForWidth(btn_321_import->sizePolicy().hasHeightForWidth());
        btn_321_import->setSizePolicy(sizePolicy);
        btn_321_test = new QPushButton(page321_floor);
        btn_321_test->setObjectName(QStringLiteral("btn_321_test"));
        btn_321_test->setGeometry(QRect(10, 470, 193, 77));
        sizePolicy.setHeightForWidth(btn_321_test->sizePolicy().hasHeightForWidth());
        btn_321_test->setSizePolicy(sizePolicy);
        list_321_floor = new QListWidget(page321_floor);
        list_321_floor->setObjectName(QStringLiteral("list_321_floor"));
        list_321_floor->setGeometry(QRect(560, 110, 401, 251));
        edit_321_cur = new QLineEdit(page321_floor);
        edit_321_cur->setObjectName(QStringLiteral("edit_321_cur"));
        edit_321_cur->setGeometry(QRect(560, 380, 401, 61));
        edit_321_cur->setReadOnly(true);
        MainStackedWidget->addWidget(page321_floor);
        page322_arrow = new QWidget();
        page322_arrow->setObjectName(QStringLiteral("page322_arrow"));
        label_322_preview = new QLabel(page322_arrow);
        label_322_preview->setObjectName(QStringLiteral("label_322_preview"));
        label_322_preview->setGeometry(QRect(200, 110, 281, 51));
        btn_322_import = new QPushButton(page322_arrow);
        btn_322_import->setObjectName(QStringLiteral("btn_322_import"));
        btn_322_import->setGeometry(QRect(400, 470, 193, 77));
        sizePolicy.setHeightForWidth(btn_322_import->sizePolicy().hasHeightForWidth());
        btn_322_import->setSizePolicy(sizePolicy);
        btn_322_next = new QPushButton(page322_arrow);
        btn_322_next->setObjectName(QStringLiteral("btn_322_next"));
        btn_322_next->setGeometry(QRect(784, 470, 193, 77));
        sizePolicy.setHeightForWidth(btn_322_next->sizePolicy().hasHeightForWidth());
        btn_322_next->setSizePolicy(sizePolicy);
        btn_322_return = new QPushButton(page322_arrow);
        btn_322_return->setObjectName(QStringLiteral("btn_322_return"));
        btn_322_return->setGeometry(QRect(590, 470, 193, 77));
        sizePolicy.setHeightForWidth(btn_322_return->sizePolicy().hasHeightForWidth());
        btn_322_return->setSizePolicy(sizePolicy);
        btn_322_test = new QPushButton(page322_arrow);
        btn_322_test->setObjectName(QStringLiteral("btn_322_test"));
        btn_322_test->setGeometry(QRect(10, 470, 193, 77));
        sizePolicy.setHeightForWidth(btn_322_test->sizePolicy().hasHeightForWidth());
        btn_322_test->setSizePolicy(sizePolicy);
        img_322_preview = new QLabel(page322_arrow);
        img_322_preview->setObjectName(QStringLiteral("img_322_preview"));
        img_322_preview->setGeometry(QRect(30, 170, 480, 272));
        img_322_preview->setMinimumSize(QSize(480, 272));
        img_322_preview->setMaximumSize(QSize(480, 272));
        label_322_title = new QLabel(page322_arrow);
        label_322_title->setObjectName(QStringLiteral("label_322_title"));
        label_322_title->setGeometry(QRect(30, 10, 641, 71));
        list_322_arrows = new QListWidget(page322_arrow);
        list_322_arrows->setObjectName(QStringLiteral("list_322_arrows"));
        list_322_arrows->setGeometry(QRect(540, 60, 401, 301));
        edit_322_cur = new QLineEdit(page322_arrow);
        edit_322_cur->setObjectName(QStringLiteral("edit_322_cur"));
        edit_322_cur->setGeometry(QRect(540, 380, 401, 61));
        edit_322_cur->setReadOnly(true);
        MainStackedWidget->addWidget(page322_arrow);
        page323_logo = new QWidget();
        page323_logo->setObjectName(QStringLiteral("page323_logo"));
        label_323_title = new QLabel(page323_logo);
        label_323_title->setObjectName(QStringLiteral("label_323_title"));
        label_323_title->setGeometry(QRect(40, 10, 641, 71));
        btn_323_return = new QPushButton(page323_logo);
        btn_323_return->setObjectName(QStringLiteral("btn_323_return"));
        btn_323_return->setGeometry(QRect(740, 470, 193, 77));
        sizePolicy.setHeightForWidth(btn_323_return->sizePolicy().hasHeightForWidth());
        btn_323_return->setSizePolicy(sizePolicy);
        edit_323_text1 = new QLineEdit(page323_logo);
        edit_323_text1->setObjectName(QStringLiteral("edit_323_text1"));
        edit_323_text1->setGeometry(QRect(530, 200, 401, 41));
        label_323_text1 = new QLabel(page323_logo);
        label_323_text1->setObjectName(QStringLiteral("label_323_text1"));
        label_323_text1->setGeometry(QRect(480, 150, 281, 41));
        label_323_text2 = new QLabel(page323_logo);
        label_323_text2->setObjectName(QStringLiteral("label_323_text2"));
        label_323_text2->setGeometry(QRect(480, 260, 261, 41));
        edit_323_text2 = new QLineEdit(page323_logo);
        edit_323_text2->setObjectName(QStringLiteral("edit_323_text2"));
        edit_323_text2->setGeometry(QRect(530, 310, 401, 41));
        list_323_logo = new QListWidget(page323_logo);
        list_323_logo->setObjectName(QStringLiteral("list_323_logo"));
        list_323_logo->setGeometry(QRect(40, 100, 401, 331));
        MainStackedWidget->addWidget(page323_logo);
        page324_background = new QWidget();
        page324_background->setObjectName(QStringLiteral("page324_background"));
        label_324_preview = new QLabel(page324_background);
        label_324_preview->setObjectName(QStringLiteral("label_324_preview"));
        label_324_preview->setGeometry(QRect(200, 110, 301, 51));
        btn_324_import = new QPushButton(page324_background);
        btn_324_import->setObjectName(QStringLiteral("btn_324_import"));
        btn_324_import->setGeometry(QRect(580, 470, 193, 77));
        sizePolicy.setHeightForWidth(btn_324_import->sizePolicy().hasHeightForWidth());
        btn_324_import->setSizePolicy(sizePolicy);
        btn_324_return = new QPushButton(page324_background);
        btn_324_return->setObjectName(QStringLiteral("btn_324_return"));
        btn_324_return->setGeometry(QRect(770, 470, 193, 77));
        sizePolicy.setHeightForWidth(btn_324_return->sizePolicy().hasHeightForWidth());
        btn_324_return->setSizePolicy(sizePolicy);
        img_324_preview = new QLabel(page324_background);
        img_324_preview->setObjectName(QStringLiteral("img_324_preview"));
        img_324_preview->setGeometry(QRect(30, 170, 480, 272));
        img_324_preview->setMinimumSize(QSize(480, 272));
        img_324_preview->setMaximumSize(QSize(480, 272));
        label_324_title = new QLabel(page324_background);
        label_324_title->setObjectName(QStringLiteral("label_324_title"));
        label_324_title->setGeometry(QRect(30, 10, 701, 71));
        list_324_background = new QListWidget(page324_background);
        list_324_background->setObjectName(QStringLiteral("list_324_background"));
        list_324_background->setGeometry(QRect(560, 70, 401, 301));
        btn_324_test = new QPushButton(page324_background);
        btn_324_test->setObjectName(QStringLiteral("btn_324_test"));
        btn_324_test->setGeometry(QRect(20, 470, 193, 77));
        sizePolicy.setHeightForWidth(btn_324_test->sizePolicy().hasHeightForWidth());
        btn_324_test->setSizePolicy(sizePolicy);
        edit_324_cur = new QLineEdit(page324_background);
        edit_324_cur->setObjectName(QStringLiteral("edit_324_cur"));
        edit_324_cur->setGeometry(QRect(560, 390, 401, 61));
        edit_324_cur->setReadOnly(true);
        MainStackedWidget->addWidget(page324_background);
        page33_language = new QWidget();
        page33_language->setObjectName(QStringLiteral("page33_language"));
        btn_33_import = new QPushButton(page33_language);
        btn_33_import->setObjectName(QStringLiteral("btn_33_import"));
        btn_33_import->setGeometry(QRect(230, 460, 201, 81));
        sizePolicy.setHeightForWidth(btn_33_import->sizePolicy().hasHeightForWidth());
        btn_33_import->setSizePolicy(sizePolicy);
        btn_33_return = new QPushButton(page33_language);
        btn_33_return->setObjectName(QStringLiteral("btn_33_return"));
        btn_33_return->setGeometry(QRect(752, 460, 221, 81));
        sizePolicy.setHeightForWidth(btn_33_return->sizePolicy().hasHeightForWidth());
        btn_33_return->setSizePolicy(sizePolicy);
        label_33_title = new QLabel(page33_language);
        label_33_title->setObjectName(QStringLiteral("label_33_title"));
        label_33_title->setGeometry(QRect(20, 10, 511, 71));
        list_33_language = new QListWidget(page33_language);
        list_33_language->setObjectName(QStringLiteral("list_33_language"));
        list_33_language->setGeometry(QRect(20, 140, 361, 291));
        list_33_audio = new QListWidget(page33_language);
        list_33_audio->setObjectName(QStringLiteral("list_33_audio"));
        list_33_audio->setGeometry(QRect(400, 140, 361, 291));
        btn_33_test = new QPushButton(page33_language);
        btn_33_test->setObjectName(QStringLiteral("btn_33_test"));
        btn_33_test->setGeometry(QRect(780, 110, 191, 81));
        sizePolicy.setHeightForWidth(btn_33_test->sizePolicy().hasHeightForWidth());
        btn_33_test->setSizePolicy(sizePolicy);
        btn_33_addfile = new QPushButton(page33_language);
        btn_33_addfile->setObjectName(QStringLiteral("btn_33_addfile"));
        btn_33_addfile->setGeometry(QRect(780, 200, 191, 81));
        sizePolicy.setHeightForWidth(btn_33_addfile->sizePolicy().hasHeightForWidth());
        btn_33_addfile->setSizePolicy(sizePolicy);
        btn_33_delfile = new QPushButton(page33_language);
        btn_33_delfile->setObjectName(QStringLiteral("btn_33_delfile"));
        btn_33_delfile->setGeometry(QRect(780, 300, 191, 81));
        sizePolicy.setHeightForWidth(btn_33_delfile->sizePolicy().hasHeightForWidth());
        btn_33_delfile->setSizePolicy(sizePolicy);
        btn_33_delete = new QPushButton(page33_language);
        btn_33_delete->setObjectName(QStringLiteral("btn_33_delete"));
        btn_33_delete->setGeometry(QRect(20, 460, 191, 81));
        label_33_langlist = new QLabel(page33_language);
        label_33_langlist->setObjectName(QStringLiteral("label_33_langlist"));
        label_33_langlist->setGeometry(QRect(20, 90, 361, 41));
        label_33_audiolist = new QLabel(page33_language);
        label_33_audiolist->setObjectName(QStringLiteral("label_33_audiolist"));
        label_33_audiolist->setGeometry(QRect(400, 90, 361, 41));
        stacked_openmsg = new QStackedWidget(page33_language);
        stacked_openmsg->setObjectName(QStringLiteral("stacked_openmsg"));
        stacked_openmsg->setGeometry(QRect(550, 10, 411, 80));
        page_33_saf_openmsg = new QWidget();
        page_33_saf_openmsg->setObjectName(QStringLiteral("page_33_saf_openmsg"));
        check_openmsg = new QCheckBox(page_33_saf_openmsg);
        check_openmsg->setObjectName(QStringLiteral("check_openmsg"));
        check_openmsg->setGeometry(QRect(10, 20, 401, 51));
        check_openmsg->setLayoutDirection(Qt::RightToLeft);
        stacked_openmsg->addWidget(page_33_saf_openmsg);
        page_33_com_openmsg = new QWidget();
        page_33_com_openmsg->setObjectName(QStringLiteral("page_33_com_openmsg"));
        stacked_openmsg->addWidget(page_33_com_openmsg);
        MainStackedWidget->addWidget(page33_language);
        page33_1_import = new QWidget();
        page33_1_import->setObjectName(QStringLiteral("page33_1_import"));
        edit_33_1_path = new QLineEdit(page33_1_import);
        edit_33_1_path->setObjectName(QStringLiteral("edit_33_1_path"));
        edit_33_1_path->setGeometry(QRect(286, 240, 341, 61));
        btn_33_1_selectfolder = new QPushButton(page33_1_import);
        btn_33_1_selectfolder->setObjectName(QStringLiteral("btn_33_1_selectfolder"));
        btn_33_1_selectfolder->setGeometry(QRect(692, 239, 63, 61));
        label_33_1_path = new QLabel(page33_1_import);
        label_33_1_path->setObjectName(QStringLiteral("label_33_1_path"));
        label_33_1_path->setGeometry(QRect(90, 250, 191, 41));
        label_33_1_path->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        edit_33_1_name = new QLineEdit(page33_1_import);
        edit_33_1_name->setObjectName(QStringLiteral("edit_33_1_name"));
        edit_33_1_name->setGeometry(QRect(287, 160, 341, 61));
        label_33_1_title = new QLabel(page33_1_import);
        label_33_1_title->setObjectName(QStringLiteral("label_33_1_title"));
        label_33_1_title->setGeometry(QRect(20, 10, 811, 71));
        label_33_1_name = new QLabel(page33_1_import);
        label_33_1_name->setObjectName(QStringLiteral("label_33_1_name"));
        label_33_1_name->setGeometry(QRect(90, 170, 191, 41));
        label_33_1_name->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        btn_33_1_return = new QPushButton(page33_1_import);
        btn_33_1_return->setObjectName(QStringLiteral("btn_33_1_return"));
        btn_33_1_return->setGeometry(QRect(770, 470, 196, 71));
        btn_33_1_import = new QPushButton(page33_1_import);
        btn_33_1_import->setObjectName(QStringLiteral("btn_33_1_import"));
        btn_33_1_import->setGeometry(QRect(760, 240, 191, 71));
        btn_33_1_selectzip = new QPushButton(page33_1_import);
        btn_33_1_selectzip->setObjectName(QStringLiteral("btn_33_1_selectzip"));
        btn_33_1_selectzip->setGeometry(QRect(630, 240, 61, 59));
        MainStackedWidget->addWidget(page33_1_import);
        page33_2_new = new QWidget();
        page33_2_new->setObjectName(QStringLiteral("page33_2_new"));
        edit_33_2_name = new QLineEdit(page33_2_new);
        edit_33_2_name->setObjectName(QStringLiteral("edit_33_2_name"));
        edit_33_2_name->setGeometry(QRect(280, 170, 411, 61));
        btn_33_2_return = new QPushButton(page33_2_new);
        btn_33_2_return->setObjectName(QStringLiteral("btn_33_2_return"));
        btn_33_2_return->setGeometry(QRect(550, 460, 191, 71));
        btn_33_2_next = new QPushButton(page33_2_new);
        btn_33_2_next->setObjectName(QStringLiteral("btn_33_2_next"));
        btn_33_2_next->setGeometry(QRect(756, 460, 191, 71));
        label_33_2_title = new QLabel(page33_2_new);
        label_33_2_title->setObjectName(QStringLiteral("label_33_2_title"));
        label_33_2_title->setGeometry(QRect(20, 10, 751, 71));
        label_33_2_name = new QLabel(page33_2_new);
        label_33_2_name->setObjectName(QStringLiteral("label_33_2_name"));
        label_33_2_name->setGeometry(QRect(60, 180, 211, 41));
        label_33_2_name->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        edit_33_2_path = new QLineEdit(page33_2_new);
        edit_33_2_path->setObjectName(QStringLiteral("edit_33_2_path"));
        edit_33_2_path->setGeometry(QRect(276, 260, 341, 61));
        btn_33_2_selectzip = new QPushButton(page33_2_new);
        btn_33_2_selectzip->setObjectName(QStringLiteral("btn_33_2_selectzip"));
        btn_33_2_selectzip->setGeometry(QRect(620, 260, 61, 59));
        btn_33_2_selectfolder = new QPushButton(page33_2_new);
        btn_33_2_selectfolder->setObjectName(QStringLiteral("btn_33_2_selectfolder"));
        btn_33_2_selectfolder->setGeometry(QRect(682, 259, 63, 61));
        btn_33_2_import = new QPushButton(page33_2_new);
        btn_33_2_import->setObjectName(QStringLiteral("btn_33_2_import"));
        btn_33_2_import->setGeometry(QRect(750, 260, 191, 71));
        label_33_2_path = new QLabel(page33_2_new);
        label_33_2_path->setObjectName(QStringLiteral("label_33_2_path"));
        label_33_2_path->setGeometry(QRect(80, 270, 191, 41));
        label_33_2_path->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        MainStackedWidget->addWidget(page33_2_new);
        page34_setting = new QWidget();
        page34_setting->setObjectName(QStringLiteral("page34_setting"));
        page34_setting->setStyleSheet(QLatin1String("QSlider::groove:horizontal {  border: 1px rgba(254, 158, 37, 255);  height: 2px;  margin: 0px 0; left: 22px; right: 22px;  }\n"
"QSlider::handle:horizontal {   border-image: url(:/0_common/rc/0_common/sliderdot.png);  width: 40px; height : 40px;  margin: -20px -20px -20px -20px; }\n"
"QSlider::sub-page:horizontal{  background: rgba(254, 158, 37, 255); }\n"
"QSlider::add-page:horizontal{ background: rgba(100, 100, 100, 255); }\n"
"QSlider::indicator { right : 22px}\n"
"QSlider::hover{  border: 2px rgba(254, 158, 37, 255); }"));
        SettingStacked = new QStackedWidget(page34_setting);
        SettingStacked->setObjectName(QStringLiteral("SettingStacked"));
        SettingStacked->setGeometry(QRect(344, 94, 614, 359));
        page341_volume = new QWidget();
        page341_volume->setObjectName(QStringLiteral("page341_volume"));
        label_msg = new QLabel(page341_volume);
        label_msg->setObjectName(QStringLiteral("label_msg"));
        label_msg->setGeometry(QRect(61, 58, 131, 41));
        label_gong = new QLabel(page341_volume);
        label_gong->setObjectName(QStringLiteral("label_gong"));
        label_gong->setGeometry(QRect(61, 148, 131, 51));
        label_music = new QLabel(page341_volume);
        label_music->setObjectName(QStringLiteral("label_music"));
        label_music->setGeometry(QRect(60, 250, 131, 51));
        label_msg_icon1 = new QLabel(page341_volume);
        label_msg_icon1->setObjectName(QStringLiteral("label_msg_icon1"));
        label_msg_icon1->setGeometry(QRect(61, 111, 39, 31));
        slider_message_volume = new MySlider(page341_volume);
        slider_message_volume->setObjectName(QStringLiteral("slider_message_volume"));
        slider_message_volume->setGeometry(QRect(110, 100, 340, 51));
        slider_message_volume->setMaximum(100);
        slider_message_volume->setPageStep(5);
        slider_message_volume->setOrientation(Qt::Horizontal);
        label_msg_icon2 = new QLabel(page341_volume);
        label_msg_icon2->setObjectName(QStringLiteral("label_msg_icon2"));
        label_msg_icon2->setGeometry(QRect(454, 110, 36, 32));
        label_gong_icon2 = new QLabel(page341_volume);
        label_gong_icon2->setObjectName(QStringLiteral("label_gong_icon2"));
        label_gong_icon2->setGeometry(QRect(454, 208, 36, 32));
        slider_gong_volume = new MySlider(page341_volume);
        slider_gong_volume->setObjectName(QStringLiteral("slider_gong_volume"));
        slider_gong_volume->setGeometry(QRect(110, 200, 340, 51));
        slider_gong_volume->setMaximum(100);
        slider_gong_volume->setPageStep(5);
        slider_gong_volume->setOrientation(Qt::Horizontal);
        label_gong_icon1 = new QLabel(page341_volume);
        label_gong_icon1->setObjectName(QStringLiteral("label_gong_icon1"));
        label_gong_icon1->setGeometry(QRect(61, 208, 39, 31));
        label_music_icon2 = new QLabel(page341_volume);
        label_music_icon2->setObjectName(QStringLiteral("label_music_icon2"));
        label_music_icon2->setGeometry(QRect(454, 310, 36, 32));
        label_music_icon1 = new QLabel(page341_volume);
        label_music_icon1->setObjectName(QStringLiteral("label_music_icon1"));
        label_music_icon1->setGeometry(QRect(60, 310, 39, 31));
        slider_bgmusic_volume = new MySlider(page341_volume);
        slider_bgmusic_volume->setObjectName(QStringLiteral("slider_bgmusic_volume"));
        slider_bgmusic_volume->setGeometry(QRect(110, 300, 340, 51));
        slider_bgmusic_volume->setMaximum(100);
        slider_bgmusic_volume->setPageStep(5);
        slider_bgmusic_volume->setOrientation(Qt::Horizontal);
        label_msg_value = new QLabel(page341_volume);
        label_msg_value->setObjectName(QStringLiteral("label_msg_value"));
        label_msg_value->setGeometry(QRect(501, 110, 71, 31));
        label_gong_value = new QLabel(page341_volume);
        label_gong_value->setObjectName(QStringLiteral("label_gong_value"));
        label_gong_value->setGeometry(QRect(501, 208, 71, 31));
        label_music_value = new QLabel(page341_volume);
        label_music_value->setObjectName(QStringLiteral("label_music_value"));
        label_music_value->setGeometry(QRect(500, 310, 71, 31));
        layoutWidget_2 = new QWidget(page341_volume);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(60, 10, 481, 42));
        horizontalLayout_36 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_36->setSpacing(0);
        horizontalLayout_36->setObjectName(QStringLiteral("horizontalLayout_36"));
        horizontalLayout_36->setContentsMargins(0, 0, 0, 0);
        label_volume_enable = new QLabel(layoutWidget_2);
        label_volume_enable->setObjectName(QStringLiteral("label_volume_enable"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_volume_enable->sizePolicy().hasHeightForWidth());
        label_volume_enable->setSizePolicy(sizePolicy1);
        label_volume_enable->setMinimumSize(QSize(0, 40));
        label_volume_enable->setMaximumSize(QSize(16777215, 40));

        horizontalLayout_36->addWidget(label_volume_enable);

        check_volume_enable = new QCheckBox(layoutWidget_2);
        check_volume_enable->setObjectName(QStringLiteral("check_volume_enable"));
        check_volume_enable->setMinimumSize(QSize(0, 30));
        check_volume_enable->setMaximumSize(QSize(16777215, 30));
        check_volume_enable->setLayoutDirection(Qt::RightToLeft);
        check_volume_enable->setChecked(true);

        horizontalLayout_36->addWidget(check_volume_enable);

        SettingStacked->addWidget(page341_volume);
        page342_floors = new QWidget();
        page342_floors->setObjectName(QStringLiteral("page342_floors"));
        stacked_flrmap = new QStackedWidget(page342_floors);
        stacked_flrmap->setObjectName(QStringLiteral("stacked_flrmap"));
        stacked_flrmap->setGeometry(QRect(0, 0, 611, 351));
        stacked_flrmap->setStyleSheet(QStringLiteral("QWidget { background-color: rgba(255, 255, 255, 0); }"));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        verticalLayout_2 = new QVBoxLayout(page);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        table_flrmap = new QTableWidget(page);
        if (table_flrmap->columnCount() < 1)
            table_flrmap->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        table_flrmap->setHorizontalHeaderItem(0, __qtablewidgetitem);
        table_flrmap->setObjectName(QStringLiteral("table_flrmap"));
        table_flrmap->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        table_flrmap->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        table_flrmap->setSelectionMode(QAbstractItemView::SingleSelection);
        table_flrmap->setSelectionBehavior(QAbstractItemView::SelectRows);
        table_flrmap->setShowGrid(false);
        table_flrmap->setGridStyle(Qt::NoPen);
        table_flrmap->horizontalHeader()->setVisible(false);
        table_flrmap->horizontalHeader()->setCascadingSectionResizes(false);
        table_flrmap->horizontalHeader()->setDefaultSectionSize(748);
        table_flrmap->horizontalHeader()->setHighlightSections(true);
        table_flrmap->verticalHeader()->setVisible(false);
        table_flrmap->verticalHeader()->setCascadingSectionResizes(false);
        table_flrmap->verticalHeader()->setDefaultSectionSize(71);

        verticalLayout_2->addWidget(table_flrmap);

        stacked_flrmap->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        verticalLayout_3 = new QVBoxLayout(page_2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_flrmap = new QLabel(page_2);
        label_flrmap->setObjectName(QStringLiteral("label_flrmap"));
        label_flrmap->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_flrmap);

        stacked_flrmap->addWidget(page_2);
        SettingStacked->addWidget(page342_floors);
        page343_backlight = new QWidget();
        page343_backlight->setObjectName(QStringLiteral("page343_backlight"));
        label_standby_min = new QLabel(page343_backlight);
        label_standby_min->setObjectName(QStringLiteral("label_standby_min"));
        label_standby_min->setGeometry(QRect(67, 210, 39, 31));
        label_standby_min->setAlignment(Qt::AlignCenter);
        label_normal = new QLabel(page343_backlight);
        label_normal->setObjectName(QStringLiteral("label_normal"));
        label_normal->setGeometry(QRect(61, 68, 131, 31));
        label_normal_max = new QLabel(page343_backlight);
        label_normal_max->setObjectName(QStringLiteral("label_normal_max"));
        label_normal_max->setGeometry(QRect(424, 108, 161, 32));
        label_normal_max->setAlignment(Qt::AlignCenter);
        slider_backlight = new MySlider(page343_backlight);
        slider_backlight->setObjectName(QStringLiteral("slider_backlight"));
        slider_backlight->setGeometry(QRect(110, 100, 340, 51));
        slider_backlight->setMaximum(100);
        slider_backlight->setPageStep(5);
        slider_backlight->setOrientation(Qt::Horizontal);
        label_standby_max = new QLabel(page343_backlight);
        label_standby_max->setObjectName(QStringLiteral("label_standby_max"));
        label_standby_max->setGeometry(QRect(424, 210, 161, 32));
        label_standby_max->setAlignment(Qt::AlignCenter);
        label_delay_max = new QLabel(page343_backlight);
        label_delay_max->setObjectName(QStringLiteral("label_delay_max"));
        label_delay_max->setGeometry(QRect(425, 310, 161, 32));
        label_delay_max->setAlignment(Qt::AlignCenter);
        label_standby = new QLabel(page343_backlight);
        label_standby->setObjectName(QStringLiteral("label_standby"));
        label_standby->setGeometry(QRect(61, 163, 131, 31));
        slider_standby = new MySlider(page343_backlight);
        slider_standby->setObjectName(QStringLiteral("slider_standby"));
        slider_standby->setGeometry(QRect(110, 200, 340, 51));
        slider_standby->setMaximum(50);
        slider_standby->setPageStep(5);
        slider_standby->setOrientation(Qt::Horizontal);
        label_delay = new QLabel(page343_backlight);
        label_delay->setObjectName(QStringLiteral("label_delay"));
        label_delay->setGeometry(QRect(61, 263, 321, 31));
        label_delay_min = new QLabel(page343_backlight);
        label_delay_min->setObjectName(QStringLiteral("label_delay_min"));
        label_delay_min->setGeometry(QRect(67, 310, 39, 31));
        label_delay_min->setAlignment(Qt::AlignCenter);
        label_normal_min = new QLabel(page343_backlight);
        label_normal_min->setObjectName(QStringLiteral("label_normal_min"));
        label_normal_min->setGeometry(QRect(67, 109, 39, 31));
        label_normal_min->setAlignment(Qt::AlignCenter);
        slider_delay = new MySlider(page343_backlight);
        slider_delay->setObjectName(QStringLiteral("slider_delay"));
        slider_delay->setGeometry(QRect(110, 300, 340, 51));
        slider_delay->setMaximum(50);
        slider_delay->setPageStep(5);
        slider_delay->setOrientation(Qt::Horizontal);
        layoutWidget = new QWidget(page343_backlight);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(60, 10, 471, 42));
        horizontalLayout_35 = new QHBoxLayout(layoutWidget);
        horizontalLayout_35->setSpacing(0);
        horizontalLayout_35->setObjectName(QStringLiteral("horizontalLayout_35"));
        horizontalLayout_35->setContentsMargins(0, 0, 0, 0);
        label_backlight_enable = new QLabel(layoutWidget);
        label_backlight_enable->setObjectName(QStringLiteral("label_backlight_enable"));
        sizePolicy1.setHeightForWidth(label_backlight_enable->sizePolicy().hasHeightForWidth());
        label_backlight_enable->setSizePolicy(sizePolicy1);
        label_backlight_enable->setMinimumSize(QSize(0, 40));
        label_backlight_enable->setMaximumSize(QSize(16777215, 40));

        horizontalLayout_35->addWidget(label_backlight_enable);

        check_backlight_enable = new QCheckBox(layoutWidget);
        check_backlight_enable->setObjectName(QStringLiteral("check_backlight_enable"));
        check_backlight_enable->setMinimumSize(QSize(0, 30));
        check_backlight_enable->setMaximumSize(QSize(16777215, 30));
        check_backlight_enable->setLayoutDirection(Qt::RightToLeft);
        check_backlight_enable->setChecked(true);

        horizontalLayout_35->addWidget(check_backlight_enable);

        SettingStacked->addWidget(page343_backlight);
        page344_logo = new QWidget();
        page344_logo->setObjectName(QStringLiteral("page344_logo"));
        btn_344_select = new QPushButton(page344_logo);
        btn_344_select->setObjectName(QStringLiteral("btn_344_select"));
        btn_344_select->setGeometry(QRect(420, 290, 185, 53));
        wid_logo = new QWidget(page344_logo);
        wid_logo->setObjectName(QStringLiteral("wid_logo"));
        wid_logo->setGeometry(QRect(20, 30, 571, 261));
        label_344_logo = new QLabel(wid_logo);
        label_344_logo->setObjectName(QStringLiteral("label_344_logo"));
        label_344_logo->setGeometry(QRect(10, 20, 361, 171));
        label_344_logo->setScaledContents(true);
        SettingStacked->addWidget(page344_logo);
        page345_wifi = new QWidget();
        page345_wifi->setObjectName(QStringLiteral("page345_wifi"));
        edit_passwd = new QLineEdit(page345_wifi);
        edit_passwd->setObjectName(QStringLiteral("edit_passwd"));
        edit_passwd->setGeometry(QRect(70, 210, 481, 51));
        label_3_passwd = new QLabel(page345_wifi);
        label_3_passwd->setObjectName(QStringLiteral("label_3_passwd"));
        label_3_passwd->setGeometry(QRect(20, 160, 181, 31));
        label_3_passwd->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        edit_ssid = new QLineEdit(page345_wifi);
        edit_ssid->setObjectName(QStringLiteral("edit_ssid"));
        edit_ssid->setGeometry(QRect(70, 90, 481, 51));
        label_3_ssid = new QLabel(page345_wifi);
        label_3_ssid->setObjectName(QStringLiteral("label_3_ssid"));
        label_3_ssid->setGeometry(QRect(20, 50, 191, 31));
        label_3_ssid->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        SettingStacked->addWidget(page345_wifi);
        btn_34_volume = new QPushButton(page34_setting);
        btn_34_volume->setObjectName(QStringLiteral("btn_34_volume"));
        btn_34_volume->setGeometry(QRect(100, 120, 185, 53));
        btn_34_volume->setCheckable(true);
        btn_34_volume->setChecked(true);
        btn_34_floors = new QPushButton(page34_setting);
        btn_34_floors->setObjectName(QStringLiteral("btn_34_floors"));
        btn_34_floors->setGeometry(QRect(100, 210, 185, 53));
        btn_34_floors->setCheckable(true);
        btn_34_backlight = new QPushButton(page34_setting);
        btn_34_backlight->setObjectName(QStringLiteral("btn_34_backlight"));
        btn_34_backlight->setGeometry(QRect(100, 300, 185, 53));
        btn_34_backlight->setCheckable(true);
        btn_34_logo = new QPushButton(page34_setting);
        btn_34_logo->setObjectName(QStringLiteral("btn_34_logo"));
        btn_34_logo->setEnabled(false);
        btn_34_logo->setGeometry(QRect(10000, 320, 185, 53));
        btn_34_logo->setCheckable(true);
        btn_34_return = new QPushButton(page34_setting);
        btn_34_return->setObjectName(QStringLiteral("btn_34_return"));
        btn_34_return->setGeometry(QRect(770, 470, 196, 71));
        btn_34_wifi = new QPushButton(page34_setting);
        btn_34_wifi->setObjectName(QStringLiteral("btn_34_wifi"));
        btn_34_wifi->setGeometry(QRect(100, 390, 185, 53));
        btn_34_wifi->setCheckable(true);
        MainStackedWidget->addWidget(page34_setting);

        verticalLayout->addWidget(MainStackedWidget);


        retranslateUi(DeviceManageForm);

        stacked_openmsg->setCurrentIndex(0);
        SettingStacked->setCurrentIndex(2);
        stacked_flrmap->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(DeviceManageForm);
    } // setupUi

    void retranslateUi(QWidget *DeviceManageForm)
    {
        DeviceManageForm->setWindowTitle(QApplication::translate("DeviceManageForm", "Form", 0));
        label_1_title->setText(QApplication::translate("DeviceManageForm", "Lift Color Display Designer", 0));
        lang_ned_banner->setText(QString());
        lang_en_banner->setText(QString());
        lang_deu_banner->setText(QString());
        lang_fra_banner->setText(QString());
        label_logo->setText(QString());
        label_version->setText(QApplication::translate("DeviceManageForm", "V1.0", 0));
        btn_1_1_local->setText(QApplication::translate("DeviceManageForm", "OPEN A PROJECT", 0));
        btn_1_1_new->setText(QApplication::translate("DeviceManageForm", "NEW PROJECT", 0));
        btn_download->setText(QApplication::translate("DeviceManageForm", "UPLOAD FROM DISPLAY", 0));
        label_2_title->setText(QApplication::translate("DeviceManageForm", "Project ", 0));
        img_2_preview->setText(QString());
        label_2_preview->setText(QApplication::translate("DeviceManageForm", "Preview", 0));
        btn_modify->setText(QApplication::translate("DeviceManageForm", "MODIFY", 0));
        btn_remove->setText(QApplication::translate("DeviceManageForm", "DELETE", 0));
        btn_exit->setText(QApplication::translate("DeviceManageForm", "EXIT", 0));
        btn_export->setText(QApplication::translate("DeviceManageForm", "EXPORT", 0));
        btn_close->setText(QApplication::translate("DeviceManageForm", "RETURN", 0));
        label_info_name_1->setText(QApplication::translate("DeviceManageForm", "NAME:", 0));
        label_info_name_2->setText(QApplication::translate("DeviceManageForm", "TextLabel", 0));
        label_info_appver_2->setText(QApplication::translate("DeviceManageForm", "TextLabel", 0));
        label_info_appver_1->setText(QApplication::translate("DeviceManageForm", "APP:", 0));
        label_info_sysver_2->setText(QApplication::translate("DeviceManageForm", "TextLabel", 0));
        label_info_sysver_1->setText(QApplication::translate("DeviceManageForm", "SYS:", 0));
        label_info_devtype_2->setText(QApplication::translate("DeviceManageForm", "TextLabel", 0));
        label_info_devtype_1->setText(QApplication::translate("DeviceManageForm", "TYPE:", 0));
        btn_test->setText(QApplication::translate("DeviceManageForm", "TEST", 0));
        label_2_1_title->setText(QApplication::translate("DeviceManageForm", "Create a new project", 0));
        label_2_1_name->setText(QApplication::translate("DeviceManageForm", "Project Name  :", 0));
        edit_2_1_name->setText(QString());
        btn_2_1_return->setText(QApplication::translate("DeviceManageForm", "RETURN", 0));
        btn_2_1_create->setText(QApplication::translate("DeviceManageForm", "CREATE", 0));
        label_2_1_devtype->setText(QApplication::translate("DeviceManageForm", "Device Type  :", 0));
        radio_canopen->setText(QApplication::translate("DeviceManageForm", "CANOPEN", 0));
        radio_saf->setText(QApplication::translate("DeviceManageForm", "SAF", 0));
        label_2_2_title->setText(QApplication::translate("DeviceManageForm", "Download a project", 0));
        btn_wifi->setText(QApplication::translate("DeviceManageForm", "WIFI", 0));
        btn_usb->setText(QApplication::translate("DeviceManageForm", "USB", 0));
        btn_2_2_return->setText(QApplication::translate("DeviceManageForm", "RETURN", 0));
        label_2_4_title->setText(QApplication::translate("DeviceManageForm", "Select components:", 0));
        check_application->setText(QApplication::translate("DeviceManageForm", "Application", 0));
        check_conffile->setText(QApplication::translate("DeviceManageForm", "Configure Files", 0));
        check_sysfile->setText(QApplication::translate("DeviceManageForm", "System Files", 0));
        check_style->setText(QApplication::translate("DeviceManageForm", "Style", 0));
        btn_2_4_ok->setText(QApplication::translate("DeviceManageForm", "To LocalFolder", 0));
        check_audio->setText(QApplication::translate("DeviceManageForm", "Audio", 0));
        check_all->setText(QApplication::translate("DeviceManageForm", "   All", 0));
        btn_2_4_return->setText(QApplication::translate("DeviceManageForm", "RETURN", 0));
        btn_2_4_ok_md->setText(QApplication::translate("DeviceManageForm", "To MobileDisk", 0));
        label_2_21_title->setText(QApplication::translate("DeviceManageForm", "Select Device", 0));
        btn_scandev->setText(QApplication::translate("DeviceManageForm", "Scan Device", 0));
        btn_2_21_return->setText(QApplication::translate("DeviceManageForm", "RETURN", 0));
        label_wifipasswd->setText(QApplication::translate("DeviceManageForm", "Password:", 0));
        btn_logindev->setText(QApplication::translate("DeviceManageForm", "DownLoad", 0));
        edit_wifiip->setText(QApplication::translate("DeviceManageForm", "192.168.1.10", 0));
        label_2_3_title->setText(QApplication::translate("DeviceManageForm", "Import a project", 0));
        label_2_3_name->setText(QApplication::translate("DeviceManageForm", "Name  :", 0));
        label_2_3_path->setText(QApplication::translate("DeviceManageForm", "Select folder :", 0));
        btn_2_3_selectfolder->setText(QString());
        btn_2_3_import->setText(QApplication::translate("DeviceManageForm", "IMPORT", 0));
        btn_2_3_return->setText(QApplication::translate("DeviceManageForm", "RETURN", 0));
        label_3_title->setText(QApplication::translate("DeviceManageForm", "Modify project", 0));
        btn_style->setText(QApplication::translate("DeviceManageForm", "STYLE", 0));
        btn_setting->setText(QApplication::translate("DeviceManageForm", "SETTING", 0));
        btn_3_return->setText(QApplication::translate("DeviceManageForm", "RETURN", 0));
        btn_language->setText(QApplication::translate("DeviceManageForm", "LANGUAGE", 0));
        btn_layout->setText(QApplication::translate("DeviceManageForm", "LAYOUT", 0));
        label_3_preview->setText(QApplication::translate("DeviceManageForm", "Preview", 0));
        img_3_preview->setText(QString());
        btn_explorer->setText(QApplication::translate("DeviceManageForm", "EXPLORER", 0));
        groupBox->setTitle(QApplication::translate("DeviceManageForm", "Arrow Speed", 0));
        combo_arrowspeed->clear();
        combo_arrowspeed->insertItems(0, QStringList()
         << QApplication::translate("DeviceManageForm", "Lower speed", 0)
         << QApplication::translate("DeviceManageForm", "Normal speed", 0)
         << QApplication::translate("DeviceManageForm", "Faster speed", 0)
         << QApplication::translate("DeviceManageForm", "Highest speed", 0)
        );
        btn_application->setText(QApplication::translate("DeviceManageForm", "APPLICATION", 0));
        label_31_title->setText(QApplication::translate("DeviceManageForm", "Modify layout", 0));
        label_31_preview->setText(QApplication::translate("DeviceManageForm", "Preview", 0));
        img_31_preview->setText(QString());
        radio_srn_arrow->setText(QApplication::translate("DeviceManageForm", "arrow at the front", 0));
        radio_srn_floor->setText(QApplication::translate("DeviceManageForm", "arrow at the behind", 0));
        btn_31_return->setText(QApplication::translate("DeviceManageForm", "RETURN", 0));
        radio_srn_4_3->setText(QApplication::translate("DeviceManageForm", "4.3 inch", 0));
        radio_srn_4->setText(QApplication::translate("DeviceManageForm", "4 inch", 0));
        radio_srn_180->setText(QApplication::translate("DeviceManageForm", "180", 0));
        radio_srn_90->setText(QApplication::translate("DeviceManageForm", "90", 0));
        radio_srn_270->setText(QApplication::translate("DeviceManageForm", "270", 0));
        label_32_preview->setText(QApplication::translate("DeviceManageForm", "Preview", 0));
        img_32_preview->setText(QString());
        label_32_title->setText(QApplication::translate("DeviceManageForm", "Modify style", 0));
        btn_32_import->setText(QApplication::translate("DeviceManageForm", "IMPORT", 0));
        btn_32_return->setText(QApplication::translate("DeviceManageForm", "RETURN", 0));
        btn_32_export->setText(QApplication::translate("DeviceManageForm", "EXPORT", 0));
        btn_32_remove->setText(QApplication::translate("DeviceManageForm", "DELETE", 0));
        btn_32_text->setText(QApplication::translate("DeviceManageForm", "TEXT", 0));
        label_32_1_title->setText(QApplication::translate("DeviceManageForm", "Import a style", 0));
        label_32_1_name->setText(QApplication::translate("DeviceManageForm", "Name  :", 0));
        btn_32_1_import->setText(QApplication::translate("DeviceManageForm", "IMPORT", 0));
        label_32_1_path->setText(QApplication::translate("DeviceManageForm", "Select style :", 0));
        btn_32_1_selectfolder->setText(QString());
        btn_32_1_return->setText(QApplication::translate("DeviceManageForm", "RETURN", 0));
        btn_32_1_selectzip->setText(QString());
        btn_32_1_import_project->setText(QApplication::translate("DeviceManageForm", "IMPORT", 0));
        label_32_1_path_pro->setText(QApplication::translate("DeviceManageForm", "Select project :", 0));
        btn_32_1_selectfolder_pro->setText(QString());
        label_32_2_title->setText(QApplication::translate("DeviceManageForm", "Create a new style", 0));
        label_32_2_name->setText(QApplication::translate("DeviceManageForm", "Name  :", 0));
        btn_32_2_return->setText(QApplication::translate("DeviceManageForm", "RETURN", 0));
        btn_32_2_next->setText(QApplication::translate("DeviceManageForm", "NEXT", 0));
        btn_32_2_save->setText(QApplication::translate("DeviceManageForm", "SAVE", 0));
        img_321_preview->setText(QString());
        btn_321_next->setText(QApplication::translate("DeviceManageForm", "NEXT", 0));
        label_321_title->setText(QApplication::translate("DeviceManageForm", "Modify floor display", 0));
        label_321_preview->setText(QApplication::translate("DeviceManageForm", "Preview", 0));
        btn_321_return->setText(QApplication::translate("DeviceManageForm", "RETURN", 0));
        btn_321_import->setText(QApplication::translate("DeviceManageForm", "IMPORT", 0));
        btn_321_test->setText(QString());
        label_322_preview->setText(QApplication::translate("DeviceManageForm", "Preview", 0));
        btn_322_import->setText(QApplication::translate("DeviceManageForm", "IMPORT", 0));
        btn_322_next->setText(QApplication::translate("DeviceManageForm", "NEXT", 0));
        btn_322_return->setText(QApplication::translate("DeviceManageForm", "RETURN", 0));
        btn_322_test->setText(QString());
        img_322_preview->setText(QString());
        label_322_title->setText(QApplication::translate("DeviceManageForm", "Modify arrows", 0));
        label_323_title->setText(QApplication::translate("DeviceManageForm", "Select logo", 0));
        btn_323_return->setText(QApplication::translate("DeviceManageForm", "RETURN", 0));
        label_323_text1->setText(QApplication::translate("DeviceManageForm", "Text line 1", 0));
        label_323_text2->setText(QApplication::translate("DeviceManageForm", "Text line 2", 0));
        label_324_preview->setText(QApplication::translate("DeviceManageForm", "Preview", 0));
        btn_324_import->setText(QApplication::translate("DeviceManageForm", "IMPORT", 0));
        btn_324_return->setText(QApplication::translate("DeviceManageForm", "RETURN", 0));
        img_324_preview->setText(QString());
        label_324_title->setText(QApplication::translate("DeviceManageForm", "Select background", 0));
        btn_324_test->setText(QString());
        btn_33_import->setText(QApplication::translate("DeviceManageForm", "IMPORT", 0));
        btn_33_return->setText(QApplication::translate("DeviceManageForm", "RETURN", 0));
        label_33_title->setText(QApplication::translate("DeviceManageForm", "Select language", 0));
        btn_33_test->setText(QApplication::translate("DeviceManageForm", "TEST", 0));
        btn_33_addfile->setText(QApplication::translate("DeviceManageForm", "Add File", 0));
        btn_33_delfile->setText(QApplication::translate("DeviceManageForm", "Delete File", 0));
        btn_33_delete->setText(QApplication::translate("DeviceManageForm", "DELETE", 0));
        label_33_langlist->setText(QApplication::translate("DeviceManageForm", "Language List:", 0));
        label_33_audiolist->setText(QApplication::translate("DeviceManageForm", "Current Audios List:", 0));
        check_openmsg->setText(QApplication::translate("DeviceManageForm", "OpenMessage Enable", 0));
        btn_33_1_selectfolder->setText(QString());
        label_33_1_path->setText(QApplication::translate("DeviceManageForm", "Path  :", 0));
        label_33_1_title->setText(QApplication::translate("DeviceManageForm", "Import a language library", 0));
        label_33_1_name->setText(QApplication::translate("DeviceManageForm", "Name  :", 0));
        btn_33_1_return->setText(QApplication::translate("DeviceManageForm", "RETURN", 0));
        btn_33_1_import->setText(QApplication::translate("DeviceManageForm", "IMPORT", 0));
        btn_33_1_selectzip->setText(QString());
        btn_33_2_return->setText(QApplication::translate("DeviceManageForm", "RETURN", 0));
        btn_33_2_next->setText(QApplication::translate("DeviceManageForm", "NEXT", 0));
        label_33_2_title->setText(QApplication::translate("DeviceManageForm", "Create a new Language", 0));
        label_33_2_name->setText(QApplication::translate("DeviceManageForm", "Name  :", 0));
        btn_33_2_selectzip->setText(QString());
        btn_33_2_selectfolder->setText(QString());
        btn_33_2_import->setText(QApplication::translate("DeviceManageForm", "IMPORT", 0));
        label_33_2_path->setText(QApplication::translate("DeviceManageForm", "Path  :", 0));
        label_msg->setText(QApplication::translate("DeviceManageForm", "Message", 0));
        label_gong->setText(QApplication::translate("DeviceManageForm", "Gong", 0));
        label_music->setText(QApplication::translate("DeviceManageForm", "Music", 0));
        label_msg_icon1->setText(QString());
        label_msg_icon2->setText(QString());
        label_gong_icon2->setText(QString());
        label_gong_icon1->setText(QString());
        label_music_icon2->setText(QString());
        label_music_icon1->setText(QString());
        label_msg_value->setText(QApplication::translate("DeviceManageForm", "100", 0));
        label_gong_value->setText(QApplication::translate("DeviceManageForm", "100", 0));
        label_music_value->setText(QApplication::translate("DeviceManageForm", "100", 0));
        label_volume_enable->setText(QApplication::translate("DeviceManageForm", "Volume Enable", 0));
        check_volume_enable->setText(QString());
        QTableWidgetItem *___qtablewidgetitem = table_flrmap->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("DeviceManageForm", "\346\226\260\345\273\272\345\210\227", 0));
        label_flrmap->setText(QApplication::translate("DeviceManageForm", "Only for SAF!", 0));
        label_standby_min->setText(QApplication::translate("DeviceManageForm", "0", 0));
        label_normal->setText(QApplication::translate("DeviceManageForm", "Normal", 0));
        label_normal_max->setText(QApplication::translate("DeviceManageForm", "100", 0));
        label_standby_max->setText(QApplication::translate("DeviceManageForm", "50", 0));
        label_delay_max->setText(QApplication::translate("DeviceManageForm", "100", 0));
        label_standby->setText(QApplication::translate("DeviceManageForm", "Standby", 0));
        label_delay->setText(QApplication::translate("DeviceManageForm", "Delay before standby", 0));
        label_delay_min->setText(QApplication::translate("DeviceManageForm", "0", 0));
        label_normal_min->setText(QApplication::translate("DeviceManageForm", "0", 0));
        label_backlight_enable->setText(QApplication::translate("DeviceManageForm", "Standby Enable", 0));
        check_backlight_enable->setText(QString());
        btn_344_select->setText(QApplication::translate("DeviceManageForm", "SELECT >", 0));
        label_344_logo->setText(QString());
        label_3_passwd->setText(QApplication::translate("DeviceManageForm", "Password:", 0));
        label_3_ssid->setText(QApplication::translate("DeviceManageForm", "SSID:", 0));
        btn_34_volume->setText(QApplication::translate("DeviceManageForm", "Volume", 0));
        btn_34_floors->setText(QApplication::translate("DeviceManageForm", "Floors", 0));
        btn_34_backlight->setText(QApplication::translate("DeviceManageForm", "Backlight", 0));
        btn_34_logo->setText(QApplication::translate("DeviceManageForm", "Customer logo", 0));
        btn_34_return->setText(QApplication::translate("DeviceManageForm", "RETURN", 0));
        btn_34_wifi->setText(QApplication::translate("DeviceManageForm", "WIFI", 0));
    } // retranslateUi

};

namespace Ui {
    class DeviceManageForm: public Ui_DeviceManageForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEVICESETTING_H
