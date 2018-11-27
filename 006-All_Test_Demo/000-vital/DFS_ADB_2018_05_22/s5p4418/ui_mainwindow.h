/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>
#include "menuitem.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *Desktop;
    QHBoxLayout *horizontalLayout;
    QStackedWidget *stackedWidget;
    QWidget *page_elevator;
    QStackedWidget *stackedWidget_tips;
    QWidget *page_indicator;
    QLabel *label_direction;
    QLabel *label_elevator;
    QWidget *page_echo;
    QLabel *label_echo;
    QWidget *page_message;
    QLabel *label_message;
    QWidget *page_inputDF;
    QLabel *label_inputDF;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QPushButton *btn_5;
    QPushButton *btn_0;
    QPushButton *btn_enter;
    QPushButton *btn_1;
    QPushButton *btn_8;
    QPushButton *btn_9;
    QPushButton *btn_6;
    QPushButton *btn_4;
    QPushButton *btn_del;
    QPushButton *btn_7;
    QPushButton *btn_2;
    QPushButton *btn_3;
    QWidget *page_passwd;
    QWidget *widget_passwd;
    QPushButton *btn_pass7;
    QPushButton *btn_pass4;
    QPushButton *btn_pass1;
    QPushButton *btn_pass8;
    QPushButton *btn_pass9;
    QPushButton *btn_pass5;
    QPushButton *btn_pass0;
    QPushButton *btn_pass2;
    QPushButton *btn_pass6;
    QPushButton *btn_pass3;
    QLabel *label_passwd;
    QWidget *widget;
    QPushButton *btn_passEnter;
    QPushButton *btn_passdel;
    QWidget *page_setMenu;
    QStackedWidget *stacked_MenuPage;
    QWidget *page_setArrMapping;
    QLabel *label_setmenu;
    MenuItem *widget_arrow_A;
    MenuItem *widget_arrow_B;
    MenuItem *widget_arrow_D;
    MenuItem *widget_arrow_C;
    QWidget *page_setFlrMapping;
    QLabel *label_setmapping;
    MenuItem *widget_key_enter;
    MenuItem *widget_key_del;
    MenuItem *widget_key_4;
    QWidget *page_version;
    QLabel *label_version;
    QPushButton *btn_flrmapping;
    QPushButton *btn_arrmapping;
    QPushButton *btn_back;
    QLabel *label;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 1280);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setStyleSheet(QLatin1String("QLabel#label_2,QLabel#label_3{font:bold large \"Arial\";font-size: 20px; color:white;}\n"
"\n"
"QWidget{background-color:rgba(255,255,255,0);}\n"
"QWidget#widget_passwd{background-color:rgba(255,255,255,50);border-radius:20px;}\n"
"\n"
"QLabel#label_logo{border-image:url(:/rc/logo.png);}\n"
"QLabel#label_echo{font:bold larger\"Arial\";font-size: 150px; color:white;}\n"
"QLabel#label_message,QLabel#label_inputDF{font:bold large \"Arial\";font-size: 55px; color:white;}\n"
"QLabel#label_elevator{ font: bold large \"Arial\"; font-size:100pt; color: rgb(255, 255, 255); }\n"
"QLabel#label_passwd{font:bold large \"Arial\";font-size: 51px; color:white;background-color: rgba(100, 100, 100, 50);border-radius:10;}\n"
"QWidget#widget{background-color:rgba(255,255,255,200);}\n"
"\n"
"QScrollArea{border:none;background-color:rgba(0,0,0,0);}\n"
"\n"
"QScrollBar:vertical { background: rgba(0, 0, 0, 0);  width: 9px;  margin: 0px 0 0px 0; }\n"
"QScrollBar::handle:vertical { background: rgba(195, 195, 195, 255); min-height: 20px"
                        ";  margin: 2px 0px 1px 0px;   border-radius: 3px;  border: 0px dashed rgba(180,130,100,255); }\n"
"QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }\n"
"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {  background: none; }\n"
"\n"
"QScrollBar:horizontal  { background: rgba(0, 0, 0, 0);  height: 9px;  margin: 0px 0 0px 0; }\n"
"QScrollBar::handle:horizontal  { background: rgba(195, 195, 195, 255);  min-width: 20px;  margin: 2px 0px 1px 0px;   border-radius: 3px;  border: 0px dashed rgba(180,130,100,255); }\n"
"QScrollBar::add-line:horizontal , QScrollBar::sub-line:horizontal  { height: 0px; }\n"
"QScrollBar::add-page:horizontal , QScrollBar::sub-page:horizontal {  background: none; }\n"
"\n"
"QWidget#page_elevator{border-image:url(:/rc/bg1.png);}\n"
"QWidget#page_setMenu{border-image:url(:/SetMenu/rc/SetMenu/bg_menu.png);}\n"
"QWidget#page_passwd{border-image:url(:/SetMenu/rc/SetMenu/bg_menu.png);}\n"
"\n"
"QPushButton#btn_0{border-image:url(:/rc/btn_0.png);}\n"
"QPushB"
                        "utton#btn_0::pressed{border-image:url(:/rc/btn_0_light.png);background-color: rgba(200, 200, 200, 50);border-radius:10;}\n"
"\n"
"QPushButton#btn_1{border-image:url(:/rc/btn_1.png);}\n"
"QPushButton#btn_1::pressed{border-image:url(:/rc/btn_1_light.png);background-color: rgba(200, 200, 200, 50);border-radius:10;}\n"
"\n"
"QPushButton#btn_2{border-image:url(:/rc/btn_2.png);}\n"
"QPushButton#btn_2::pressed{border-image:url(:/rc/btn_2_light.png);background-color: rgba(200, 200, 200, 50);border-radius:10;}\n"
"\n"
"QPushButton#btn_3{border-image:url(:/rc/btn_3.png);}\n"
"QPushButton#btn_3::pressed{border-image:url(:/rc/btn_3_light.png);background-color: rgba(200, 200, 200, 50);border-radius:10;}\n"
"\n"
"QPushButton#btn_4{border-image:url(:/rc/btn_4.png);}\n"
"QPushButton#btn_4::pressed{border-image:url(:/rc/btn_4_light.png);background-color: rgba(200, 200, 200, 50);border-radius:10;}\n"
"\n"
"QPushButton#btn_5{border-image:url(:/rc/btn_5.png);}\n"
"QPushButton#btn_5::pressed{border-image:url(:/rc/btn_5_light.png);"
                        "background-color: rgba(200, 200, 200, 50);border-radius:10;}\n"
"\n"
"QPushButton#btn_6{border-image:url(:/rc/btn_6.png);}\n"
"QPushButton#btn_6::pressed{border-image:url(:/rc/btn_6_light.png);background-color: rgba(200, 200, 200, 50);border-radius:10;}\n"
"\n"
"QPushButton#btn_7{border-image:url(:/rc/btn_7.png);}\n"
"QPushButton#btn_7::pressed{border-image:url(:/rc/btn_7_light.png);background-color: rgba(200, 200, 200, 50);border-radius:10;}\n"
"\n"
"QPushButton#btn_8{border-image:url(:/rc/btn_8.png);}\n"
"QPushButton#btn_8::pressed{border-image:url(:/rc/btn_8_light.png);background-color: rgba(200, 200, 200, 50);border-radius:10;}\n"
"\n"
"QPushButton#btn_9{border-image:url(:/rc/btn_9.png);}\n"
"QPushButton#btn_9::pressed{border-image:url(:/rc/btn_9_light.png);background-color: rgba(200, 200, 200, 50);border-radius:10;}\n"
"\n"
"QPushButton#btn_enter{border-image:url(:/rc/btn_enter.png);}\n"
"QPushButton#btn_enter::pressed{border-image:url(:/rc/btn_enter_light.png);background-color: rgba(200, 200, 200, 50);bo"
                        "rder-radius:10;}\n"
"\n"
"QPushButton#btn_del{border-image:url(:/rc/btn_delete.png);}\n"
"QPushButton#btn_del::pressed{border-image:url(:/rc/btn_delete_light.png);background-color: rgba(200, 200, 200, 50);border-radius:10;}\n"
"\n"
"\n"
"\n"
"QPushButton#btn_pass0{border-image:url(:/rc/btn_0.png);background-color: rgba(200, 200, 200, 50);border-radius:10;}\n"
"QPushButton#btn_pass0::pressed{border-image:url(:/rc/btn_0_light.png);background-color:rgba(200, 200, 200, 150);border-radius:10;}\n"
"\n"
"QPushButton#btn_pass1{border-image:url(:/rc/btn_1.png);background-color: rgba(200, 200, 200, 50);border-radius:10;}\n"
"QPushButton#btn_pass1::pressed{border-image:url(:/rc/btn_1_light.png);background-color: rgba(200, 200, 200, 150);border-radius:10;}\n"
"\n"
"QPushButton#btn_pass2{border-image:url(:/rc/btn_2.png);background-color: rgba(200, 200, 200, 50);border-radius:10;}\n"
"QPushButton#btn_pass2::pressed{border-image:url(:/rc/btn_2_light.png);background-color: rgba(200, 200, 200, 150);border-radius:10;}\n"
"\n"
""
                        "QPushButton#btn_pass3{border-image:url(:/rc/btn_3.png);background-color: rgba(200, 200, 200, 50);border-radius:10;}\n"
"QPushButton#btn_pass3::pressed{border-image:url(:/rc/btn_3_light.png);background-color: rgba(200, 200, 200, 150);border-radius:10;}\n"
"\n"
"QPushButton#btn_pass4{border-image:url(:/rc/btn_4.png);background-color: rgba(200, 200, 200, 50);border-radius:10;}\n"
"QPushButton#btn_pass4::pressed{border-image:url(:/rc/btn_4_light.png);background-color: rgba(200, 200, 200, 150);border-radius:10;}\n"
"\n"
"QPushButton#btn_pass5{border-image:url(:/rc/btn_5.png);background-color: rgba(200, 200, 200, 50);border-radius:10;}\n"
"QPushButton#btn_pass5::pressed{border-image:url(:/rc/btn_5_light.png);background-color: rgba(200, 200, 200, 150);border-radius:10;}\n"
"\n"
"QPushButton#btn_pass6{border-image:url(:/rc/btn_6.png);background-color: rgba(200, 200, 200, 50);border-radius:10;}\n"
"QPushButton#btn_pass6::pressed{border-image:url(:/rc/btn_6_light.png);background-color: rgba(200, 200, 200, 150);border-ra"
                        "dius:10;}\n"
"\n"
"QPushButton#btn_pass7{border-image:url(:/rc/btn_7.png);background-color: rgba(200, 200, 200, 50);border-radius:10;}\n"
"QPushButton#btn_pass7::pressed{border-image:url(:/rc/btn_7_light.png);background-color: rgba(200, 200, 200, 150);border-radius:10;}\n"
"\n"
"QPushButton#btn_pass8{border-image:url(:/rc/btn_8.png);background-color: rgba(200, 200, 200, 50);border-radius:10;}\n"
"QPushButton#btn_pass8::pressed{border-image:url(:/rc/btn_8_light.png);background-color: rgba(200, 200, 200, 150);border-radius:10;}\n"
"\n"
"QPushButton#btn_pass9{border-image:url(:/rc/btn_9.png);background-color: rgba(200, 200, 200, 50);border-radius:10;}\n"
"QPushButton#btn_pass9::pressed{border-image:url(:/rc/btn_9_light.png);background-color: rgba(200, 200, 200, 150);border-radius:10;}\n"
"\n"
"QPushButton#btn_passEnter{border-image:url(:/rc/btn_enter.png);background-color: rgba(200, 200, 200, 50);border-radius:10;}\n"
"QPushButton#btn_passEnter::pressed{border-image:url(:/rc/btn_enter_light.png);background-color:"
                        " rgba(200, 200, 200, 150);border-radius:10;}\n"
"\n"
"QPushButton#btn_passdel{border-image:url(:/rc/btn_delete.png);background-color: rgba(200, 200, 200, 50);border-radius:10;}\n"
"QPushButton#btn_passdel::pressed{border-image:url(:/rc/btn_delete_light.png);background-color: rgba(200, 200, 200, 150);border-radius:10;}\n"
"\n"
"\n"
"QWidget#page_setFlrMapping,QWidget#page_setArrMapping{border-image: url(:/SetMenu/rc/SetMenu/setmenu.png);}\n"
"QLabel#label_setmenu,QWidget#page_setmapping,QLabel#label_setmapping,QLabel#label_version{font:bold larger\"Arial\";font-size: 30px; color:white;}\n"
"\n"
"\n"
"QLabel#label_elevtAmapping,QLabel#label_elevtBmapping,QLabel#label_elevtCmapping,QLabel#label_elevtDmapping{font:bold larger\"Arial\";font-size: 26px; color:white;}\n"
"\n"
"QPushButton#btn_menu{border-image:url(:/SetMenu/rc/btn_menu.png);font:bold larger\"Arial\";font-size: 22px; color:white;}\n"
"QPushButton#btn_menu::pressed{border-image:url(:/SetMenu/rc/btn_menu_light.png);font:bold larger\"Arial\";font-size: 2"
                        "2px; color:white;}\n"
"QPushButton#btn_mapping{border-image:url(:/SetMenu/rc/btn_menu.png);font:bold larger\"Arial\";font-size: 22px; color:white;}\n"
"QPushButton#btn_mapping::pressed{border-image:url(:/SetMenu/rc/btn_menu_light.png);font:bold larger\"Arial\";font-size: 22px; color:white;}\n"
"QPushButton#btn_version{border-image:url(:/SetMenu/rc/btn_menu.png);font:bold larger\"Arial\";font-size: 22px; color:white;}\n"
"QPushButton#btn_version::pressed{border-image:url(:/SetMenu/rc/btn_menu_light.png);font:bold larger\"Arial\";font-size: 22px; color:white;}\n"
"\n"
"QWidget#widget_item1{border-image:url(:/SetMenu/rc/SetMenu/setitem.png);}\n"
"QLabel#label_light_name,QLabel#label_light_value,QLabel#label_volume_name,QLabel#label_volume_value{font:bold larger\"Arial\";font-size: 25px; color:white;}\n"
"\n"
"\n"
"\n"
"QPushButton#btn_left_light,QPushButton#btn_left_volume{border-image:url(:/SetMenu/rc/SetMenu/left_off.png);}\n"
"\n"
"QPushButton#btn_left_light::pressed,QPushButton#btn_left_volume::pressed{border"
                        "-image:url(:/SetMenu/rc/SetMenu/left_on.png);}\n"
"\n"
"QPushButton#btn_right_light,QPushButton#btn_right_volume{border-image:url(:/SetMenu/rc/SetMenu/right_off.png);}\n"
"QPushButton#btn_right_light::pressed,QPushButton#btn_right_volume::pressed{border-image:url(:/SetMenu/rc/SetMenu/right_on.png);}\n"
"\n"
"QLabel#label_Aleft,QLabel#label_Bleft,QLabel#label_Cleft,QLabel#label_Dleft{border-image:url(:/rc/dir_left.png);}\n"
"QLabel#label_Aright,QLabel#label_Bright,QLabel#label_Cright,QLabel#label_Dright{border-image:url(:/rc/dir_right.png);}\n"
"\n"
"/*QLabel#label_direction{border-image:url(:/rc/dir_left.png);}*/\n"
"\n"
"QPushButton#btn_back{border-image:url(:/SetMenu/rc/SetMenu/btn_back.png);}\n"
"QPushButton#btn_back::pressed{border-image:url(:/SetMenu/rc/SetMenu/btn_back_light.png);}\n"
"QPushButton#btn_flrmapping{border-image:url(:/SetMenu/rc/SetMenu/btn_floormap.png);}\n"
"QPushButton#btn_flrmapping::checked{border-image:url(:/SetMenu/rc/SetMenu/btn_floormap_light.png);}\n"
"QPushButton#btn_arrmapping{bo"
                        "rder-image:url(:/SetMenu/rc/SetMenu/btn_arrowmap.png);}\n"
"QPushButton#btn_arrmapping::checked{border-image:url(:/SetMenu/rc/SetMenu/btn_arrowmap_light.png);}"));
        Desktop = new QWidget(MainWindow);
        Desktop->setObjectName(QStringLiteral("Desktop"));
        sizePolicy.setHeightForWidth(Desktop->sizePolicy().hasHeightForWidth());
        Desktop->setSizePolicy(sizePolicy);
        Desktop->setMinimumSize(QSize(800, 1280));
        Desktop->setMaximumSize(QSize(800, 1280));
        horizontalLayout = new QHBoxLayout(Desktop);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        stackedWidget = new QStackedWidget(Desktop);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        stackedWidget->setStyleSheet(QStringLiteral(""));
        page_elevator = new QWidget();
        page_elevator->setObjectName(QStringLiteral("page_elevator"));
        stackedWidget_tips = new QStackedWidget(page_elevator);
        stackedWidget_tips->setObjectName(QStringLiteral("stackedWidget_tips"));
        stackedWidget_tips->setGeometry(QRect(50, 60, 711, 301));
        page_indicator = new QWidget();
        page_indicator->setObjectName(QStringLiteral("page_indicator"));
        label_direction = new QLabel(page_indicator);
        label_direction->setObjectName(QStringLiteral("label_direction"));
        label_direction->setGeometry(QRect(423, 81, 129, 126));
        label_elevator = new QLabel(page_indicator);
        label_elevator->setObjectName(QStringLiteral("label_elevator"));
        label_elevator->setGeometry(QRect(173, 70, 146, 149));
        stackedWidget_tips->addWidget(page_indicator);
        page_echo = new QWidget();
        page_echo->setObjectName(QStringLiteral("page_echo"));
        label_echo = new QLabel(page_echo);
        label_echo->setObjectName(QStringLiteral("label_echo"));
        label_echo->setGeometry(QRect(90, 70, 531, 171));
        label_echo->setAlignment(Qt::AlignCenter);
        stackedWidget_tips->addWidget(page_echo);
        page_message = new QWidget();
        page_message->setObjectName(QStringLiteral("page_message"));
        label_message = new QLabel(page_message);
        label_message->setObjectName(QStringLiteral("label_message"));
        label_message->setGeometry(QRect(20, 70, 671, 161));
        label_message->setAlignment(Qt::AlignCenter);
        stackedWidget_tips->addWidget(page_message);
        page_inputDF = new QWidget();
        page_inputDF->setObjectName(QStringLiteral("page_inputDF"));
        label_inputDF = new QLabel(page_inputDF);
        label_inputDF->setObjectName(QStringLiteral("label_inputDF"));
        label_inputDF->setGeometry(QRect(10, 70, 691, 161));
        label_inputDF->setAlignment(Qt::AlignCenter);
        stackedWidget_tips->addWidget(page_inputDF);
        scrollArea = new QScrollArea(page_elevator);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setGeometry(QRect(50, 500, 711, 691));
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 711, 691));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(scrollAreaWidgetContents->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents->setSizePolicy(sizePolicy1);
        btn_5 = new QPushButton(scrollAreaWidgetContents);
        btn_5->setObjectName(QStringLiteral("btn_5"));
        btn_5->setGeometry(QRect(242, 199, 203, 126));
        btn_0 = new QPushButton(scrollAreaWidgetContents);
        btn_0->setObjectName(QStringLiteral("btn_0"));
        btn_0->setGeometry(QRect(242, 540, 203, 126));
        btn_enter = new QPushButton(scrollAreaWidgetContents);
        btn_enter->setObjectName(QStringLiteral("btn_enter"));
        btn_enter->setGeometry(QRect(470, 540, 203, 126));
        btn_1 = new QPushButton(scrollAreaWidgetContents);
        btn_1->setObjectName(QStringLiteral("btn_1"));
        btn_1->setGeometry(QRect(25, 24, 203, 126));
        btn_8 = new QPushButton(scrollAreaWidgetContents);
        btn_8->setObjectName(QStringLiteral("btn_8"));
        btn_8->setGeometry(QRect(242, 378, 203, 126));
        btn_9 = new QPushButton(scrollAreaWidgetContents);
        btn_9->setObjectName(QStringLiteral("btn_9"));
        btn_9->setGeometry(QRect(470, 378, 203, 126));
        btn_6 = new QPushButton(scrollAreaWidgetContents);
        btn_6->setObjectName(QStringLiteral("btn_6"));
        btn_6->setGeometry(QRect(470, 199, 203, 126));
        btn_4 = new QPushButton(scrollAreaWidgetContents);
        btn_4->setObjectName(QStringLiteral("btn_4"));
        btn_4->setGeometry(QRect(25, 199, 203, 126));
        btn_del = new QPushButton(scrollAreaWidgetContents);
        btn_del->setObjectName(QStringLiteral("btn_del"));
        btn_del->setGeometry(QRect(25, 539, 203, 126));
        btn_7 = new QPushButton(scrollAreaWidgetContents);
        btn_7->setObjectName(QStringLiteral("btn_7"));
        btn_7->setGeometry(QRect(25, 378, 203, 126));
        btn_2 = new QPushButton(scrollAreaWidgetContents);
        btn_2->setObjectName(QStringLiteral("btn_2"));
        btn_2->setGeometry(QRect(242, 24, 203, 126));
        btn_3 = new QPushButton(scrollAreaWidgetContents);
        btn_3->setObjectName(QStringLiteral("btn_3"));
        btn_3->setGeometry(QRect(470, 24, 203, 126));
        scrollArea->setWidget(scrollAreaWidgetContents);
        stackedWidget->addWidget(page_elevator);
        page_passwd = new QWidget();
        page_passwd->setObjectName(QStringLiteral("page_passwd"));
        widget_passwd = new QWidget(page_passwd);
        widget_passwd->setObjectName(QStringLiteral("widget_passwd"));
        widget_passwd->setGeometry(QRect(40, 230, 731, 841));
        btn_pass7 = new QPushButton(widget_passwd);
        btn_pass7->setObjectName(QStringLiteral("btn_pass7"));
        btn_pass7->setGeometry(QRect(64, 550, 180, 100));
        btn_pass4 = new QPushButton(widget_passwd);
        btn_pass4->setObjectName(QStringLiteral("btn_pass4"));
        btn_pass4->setGeometry(QRect(64, 410, 180, 100));
        btn_pass1 = new QPushButton(widget_passwd);
        btn_pass1->setObjectName(QStringLiteral("btn_pass1"));
        btn_pass1->setGeometry(QRect(64, 270, 180, 100));
        btn_pass8 = new QPushButton(widget_passwd);
        btn_pass8->setObjectName(QStringLiteral("btn_pass8"));
        btn_pass8->setGeometry(QRect(280, 550, 180, 100));
        btn_pass9 = new QPushButton(widget_passwd);
        btn_pass9->setObjectName(QStringLiteral("btn_pass9"));
        btn_pass9->setGeometry(QRect(495, 550, 180, 100));
        btn_pass5 = new QPushButton(widget_passwd);
        btn_pass5->setObjectName(QStringLiteral("btn_pass5"));
        btn_pass5->setGeometry(QRect(280, 410, 180, 100));
        btn_pass0 = new QPushButton(widget_passwd);
        btn_pass0->setObjectName(QStringLiteral("btn_pass0"));
        btn_pass0->setGeometry(QRect(280, 693, 180, 100));
        btn_pass2 = new QPushButton(widget_passwd);
        btn_pass2->setObjectName(QStringLiteral("btn_pass2"));
        btn_pass2->setGeometry(QRect(280, 270, 180, 100));
        btn_pass6 = new QPushButton(widget_passwd);
        btn_pass6->setObjectName(QStringLiteral("btn_pass6"));
        btn_pass6->setGeometry(QRect(495, 410, 180, 100));
        btn_pass3 = new QPushButton(widget_passwd);
        btn_pass3->setObjectName(QStringLiteral("btn_pass3"));
        btn_pass3->setGeometry(QRect(495, 270, 180, 100));
        label_passwd = new QLabel(widget_passwd);
        label_passwd->setObjectName(QStringLiteral("label_passwd"));
        label_passwd->setGeometry(QRect(90, 60, 551, 121));
        label_passwd->setAlignment(Qt::AlignCenter);
        widget = new QWidget(widget_passwd);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(64, 225, 611, 3));
        btn_passEnter = new QPushButton(widget_passwd);
        btn_passEnter->setObjectName(QStringLiteral("btn_passEnter"));
        btn_passEnter->setGeometry(QRect(495, 693, 180, 100));
        btn_passdel = new QPushButton(widget_passwd);
        btn_passdel->setObjectName(QStringLiteral("btn_passdel"));
        btn_passdel->setGeometry(QRect(64, 693, 180, 100));
        stackedWidget->addWidget(page_passwd);
        page_setMenu = new QWidget();
        page_setMenu->setObjectName(QStringLiteral("page_setMenu"));
        stacked_MenuPage = new QStackedWidget(page_setMenu);
        stacked_MenuPage->setObjectName(QStringLiteral("stacked_MenuPage"));
        stacked_MenuPage->setGeometry(QRect(170, 490, 471, 621));
        page_setArrMapping = new QWidget();
        page_setArrMapping->setObjectName(QStringLiteral("page_setArrMapping"));
        label_setmenu = new QLabel(page_setArrMapping);
        label_setmenu->setObjectName(QStringLiteral("label_setmenu"));
        label_setmenu->setGeometry(QRect(50, 20, 371, 66));
        label_setmenu->setAlignment(Qt::AlignCenter);
        widget_arrow_A = new MenuItem(page_setArrMapping);
        widget_arrow_A->setObjectName(QStringLiteral("widget_arrow_A"));
        widget_arrow_A->setGeometry(QRect(10, 125, 452, 82));
        widget_arrow_B = new MenuItem(page_setArrMapping);
        widget_arrow_B->setObjectName(QStringLiteral("widget_arrow_B"));
        widget_arrow_B->setGeometry(QRect(10, 206, 452, 82));
        widget_arrow_D = new MenuItem(page_setArrMapping);
        widget_arrow_D->setObjectName(QStringLiteral("widget_arrow_D"));
        widget_arrow_D->setGeometry(QRect(10, 369, 452, 82));
        widget_arrow_C = new MenuItem(page_setArrMapping);
        widget_arrow_C->setObjectName(QStringLiteral("widget_arrow_C"));
        widget_arrow_C->setGeometry(QRect(10, 288, 452, 82));
        stacked_MenuPage->addWidget(page_setArrMapping);
        page_setFlrMapping = new QWidget();
        page_setFlrMapping->setObjectName(QStringLiteral("page_setFlrMapping"));
        label_setmapping = new QLabel(page_setFlrMapping);
        label_setmapping->setObjectName(QStringLiteral("label_setmapping"));
        label_setmapping->setGeometry(QRect(50, 20, 371, 66));
        label_setmapping->setAlignment(Qt::AlignCenter);
        widget_key_enter = new MenuItem(page_setFlrMapping);
        widget_key_enter->setObjectName(QStringLiteral("widget_key_enter"));
        widget_key_enter->setGeometry(QRect(10, 125, 452, 82));
        widget_key_del = new MenuItem(page_setFlrMapping);
        widget_key_del->setObjectName(QStringLiteral("widget_key_del"));
        widget_key_del->setGeometry(QRect(10, 206, 452, 82));
        widget_key_4 = new MenuItem(page_setFlrMapping);
        widget_key_4->setObjectName(QStringLiteral("widget_key_4"));
        widget_key_4->setGeometry(QRect(10, 288, 451, 80));
        stacked_MenuPage->addWidget(page_setFlrMapping);
        page_version = new QWidget();
        page_version->setObjectName(QStringLiteral("page_version"));
        label_version = new QLabel(page_version);
        label_version->setObjectName(QStringLiteral("label_version"));
        label_version->setGeometry(QRect(160, 60, 191, 91));
        stacked_MenuPage->addWidget(page_version);
        btn_flrmapping = new QPushButton(page_setMenu);
        btn_flrmapping->setObjectName(QStringLiteral("btn_flrmapping"));
        btn_flrmapping->setGeometry(QRect(270, 301, 281, 131));
        btn_flrmapping->setCheckable(true);
        btn_flrmapping->setChecked(false);
        btn_arrmapping = new QPushButton(page_setMenu);
        btn_arrmapping->setObjectName(QStringLiteral("btn_arrmapping"));
        btn_arrmapping->setGeometry(QRect(270, 160, 281, 141));
        btn_arrmapping->setCheckable(true);
        btn_arrmapping->setChecked(false);
        btn_back = new QPushButton(page_setMenu);
        btn_back->setObjectName(QStringLiteral("btn_back"));
        btn_back->setGeometry(QRect(353, 245, 116, 110));
        label = new QLabel(page_setMenu);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(620, 60, 54, 12));
        stackedWidget->addWidget(page_setMenu);

        horizontalLayout->addWidget(stackedWidget);

        MainWindow->setCentralWidget(Desktop);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);
        stackedWidget_tips->setCurrentIndex(2);
        stacked_MenuPage->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        label_direction->setText(QString());
        label_elevator->setText(QString());
        label_echo->setText(QString());
        label_message->setText(QString());
        label_inputDF->setText(QString());
        btn_5->setText(QString());
        btn_0->setText(QString());
        btn_enter->setText(QString());
        btn_1->setText(QString());
        btn_8->setText(QString());
        btn_9->setText(QString());
        btn_6->setText(QString());
        btn_4->setText(QString());
        btn_del->setText(QString());
        btn_7->setText(QString());
        btn_2->setText(QString());
        btn_3->setText(QString());
        btn_pass7->setText(QString());
        btn_pass4->setText(QString());
        btn_pass1->setText(QString());
        btn_pass8->setText(QString());
        btn_pass9->setText(QString());
        btn_pass5->setText(QString());
        btn_pass0->setText(QString());
        btn_pass2->setText(QString());
        btn_pass6->setText(QString());
        btn_pass3->setText(QString());
        label_passwd->setText(QString());
        btn_passEnter->setText(QString());
        btn_passdel->setText(QString());
        label_setmenu->setText(QApplication::translate("MainWindow", "Arrow Map", Q_NULLPTR));
        label_setmapping->setText(QApplication::translate("MainWindow", "Floor Map", Q_NULLPTR));
        label_version->setText(QApplication::translate("MainWindow", "version1.0", Q_NULLPTR));
        btn_flrmapping->setText(QString());
        btn_arrmapping->setText(QString());
        btn_back->setText(QString());
        label->setText(QApplication::translate("MainWindow", "v1.0", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
