/********************************************************************************
** Form generated from reading UI file 'stackedmenu_h600.ui'
**
** Created: Fri Nov 3 16:02:10 2017
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STACKEDMENU_H600_H
#define UI_STACKEDMENU_H600_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "ilistview.h"

QT_BEGIN_NAMESPACE

class Ui_StackedMenu
{
public:
    QWidget *page_menu;
    QVBoxLayout *verticalLayout;
    ItemScrollArea *scroll_menu;
    ItemScrollContent *scroll_menu_contents;
    QWidget *line;
    QPushButton *btn_saveexit;
    QPushButton *btn_exit;
    QWidget *page_para;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_title;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_back;
    QLabel *label_title;
    QPushButton *btn_back_2;
    QWidget *line_2;
    QStackedWidget *stacked_menu_paras;
    QWidget *page_menu_time;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    ItemScrollArea *scroll_year;
    ItemScrollContent *scrollArea_year;
    ItemScrollArea *scroll_month;
    ItemScrollContent *scrollArea_month;
    ItemScrollArea *scroll_day;
    ItemScrollContent *scrollArea_day;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    ItemScrollArea *scroll_hour;
    ItemScrollContent *scrollArea_hour;
    ItemScrollArea *scroll_minute;
    ItemScrollContent *scrollArea_minute;
    QSpacerItem *horizontalSpacer_4;
    QWidget *page_menu_volume;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_4;
    QWidget *wid_video;
    QVBoxLayout *verticalLayout_8;
    QLabel *label_video;
    ItemScrollArea *scroll_volume_video;
    ItemScrollContent *scrollArea_volume_2;
    QWidget *wid_audio;
    QVBoxLayout *verticalLayout_9;
    QLabel *label_audio;
    ItemScrollArea *scroll_volume_audio;
    ItemScrollContent *scrollArea_volume;
    QWidget *page_menu_backlight;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_5;
    ItemScrollArea *scroll_backlight;
    ItemScrollContent *scrollArea_backlight;
    QWidget *page_menu_theme;
    QWidget *page_menu_protocol;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_7;
    ItemScrollArea *scroll_protocol_type;
    ItemScrollContent *scrollArea_protocol_type;
    QSpacerItem *horizontalSpacer_8;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_5;
    ItemScrollArea *scroll_protocol_buad;
    ItemScrollContent *scrollArea_protocol_buad;
    ItemScrollArea *scroll_protocol_id;
    ItemScrollContent *scrollArea_protocol_id;
    QSpacerItem *horizontalSpacer_6;

    void setupUi(QStackedWidget *StackedMenu)
    {
        if (StackedMenu->objectName().isEmpty())
            StackedMenu->setObjectName(QString::fromUtf8("StackedMenu"));
        StackedMenu->resize(450, 600);
        StackedMenu->setStyleSheet(QString::fromUtf8("QWidget { background-color: rgba(90,90,90,250); border:none; }\n"
"\n"
"QLabel#label_title {font: bold 34pt  \"Walkway Expand UltraBold\"; color:rgb(255,255,255);}\n"
"\n"
"QPushButton#btn_exit { background-color: rgba(250,100,100,250); font: bold 18pt  \"Walkway Expand UltraBold\"; color:rgb(255,255,255);}\n"
"QPushButton#btn_exit:pressed { background-color: rgba(100,100,250,250); font: bold 18pt  \"Walkway Expand UltraBold\"; color:rgb(255,255,255);}\n"
"\n"
"QPushButton#btn_saveexit { background-color: rgba(100,255,100,250); font: bold 18pt  \"Walkway Expand UltraBold\"; color:rgb(255,255,255);}\n"
"QPushButton#btn_saveexit:pressed { background-color: rgba(100,100,250,250); font: bold 18pt  \"Walkway Expand UltraBold\"; color:rgb(255,255,255);}\n"
"\n"
"QPushButton#btn_back { border-image: url(:/ui/back.png); font: bold 12pt  \"Walkway Expand UltraBold\"; color:rgb(255,255,255); }\n"
"QPushButton#btn_back:pressed { border-image: url(:/ui/backlight.png); }\n"
"\n"
"QWidget#line_2 { background-color: rgba(44,"
                        "44,44,250); border:none; }\n"
"\n"
"QLabel#label_video, QLabel#label_audio {font: bold 22pt  \"Walkway Expand UltraBold\"; color:rgb(255,255,255);}\n"
"\n"
"\n"
"\n"
"\n"
""));
        page_menu = new QWidget();
        page_menu->setObjectName(QString::fromUtf8("page_menu"));
        verticalLayout = new QVBoxLayout(page_menu);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        scroll_menu = new ItemScrollArea(page_menu);
        scroll_menu->setObjectName(QString::fromUtf8("scroll_menu"));
        scroll_menu->setWidgetResizable(true);
        scroll_menu_contents = new ItemScrollContent();
        scroll_menu_contents->setObjectName(QString::fromUtf8("scroll_menu_contents"));
        scroll_menu_contents->setGeometry(QRect(0, 0, 450, 450));
        scroll_menu->setWidget(scroll_menu_contents);

        verticalLayout->addWidget(scroll_menu);

        line = new QWidget(page_menu);
        line->setObjectName(QString::fromUtf8("line"));
        line->setMinimumSize(QSize(0, 2));
        line->setMaximumSize(QSize(16777215, 2));

        verticalLayout->addWidget(line);

        btn_saveexit = new QPushButton(page_menu);
        btn_saveexit->setObjectName(QString::fromUtf8("btn_saveexit"));
        btn_saveexit->setMinimumSize(QSize(0, 70));
        btn_saveexit->setMaximumSize(QSize(16777215, 70));

        verticalLayout->addWidget(btn_saveexit);

        btn_exit = new QPushButton(page_menu);
        btn_exit->setObjectName(QString::fromUtf8("btn_exit"));
        btn_exit->setMinimumSize(QSize(0, 70));
        btn_exit->setMaximumSize(QSize(16777215, 70));

        verticalLayout->addWidget(btn_exit);

        StackedMenu->addWidget(page_menu);
        page_para = new QWidget();
        page_para->setObjectName(QString::fromUtf8("page_para"));
        verticalLayout_2 = new QVBoxLayout(page_para);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        widget_title = new QWidget(page_para);
        widget_title->setObjectName(QString::fromUtf8("widget_title"));
        widget_title->setMinimumSize(QSize(0, 60));
        widget_title->setMaximumSize(QSize(16777215, 60));
        widget_title->setStyleSheet(QString::fromUtf8("QWidget { background-color: qlineargradient(spread:reflect, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(50, 50, 50, 255), stop:0.505682 rgba(120, 120, 120, 255), stop:0.982955 rgba(90, 90, 90, 255)); border:none; }\n"
"\n"
"QPushButton#btn_back_2 { background-color: rgba(82, 64, 209, 0); border:none; }\n"
""));
        horizontalLayout = new QHBoxLayout(widget_title);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btn_back = new QPushButton(widget_title);
        btn_back->setObjectName(QString::fromUtf8("btn_back"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btn_back->sizePolicy().hasHeightForWidth());
        btn_back->setSizePolicy(sizePolicy);
        btn_back->setMinimumSize(QSize(100, 52));
        btn_back->setMaximumSize(QSize(100, 52));

        horizontalLayout->addWidget(btn_back);

        label_title = new QLabel(widget_title);
        label_title->setObjectName(QString::fromUtf8("label_title"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_title->sizePolicy().hasHeightForWidth());
        label_title->setSizePolicy(sizePolicy1);
        label_title->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_title);

        btn_back_2 = new QPushButton(widget_title);
        btn_back_2->setObjectName(QString::fromUtf8("btn_back_2"));
        sizePolicy.setHeightForWidth(btn_back_2->sizePolicy().hasHeightForWidth());
        btn_back_2->setSizePolicy(sizePolicy);
        btn_back_2->setMinimumSize(QSize(100, 52));
        btn_back_2->setMaximumSize(QSize(100, 52));

        horizontalLayout->addWidget(btn_back_2);


        verticalLayout_2->addWidget(widget_title);

        line_2 = new QWidget(page_para);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setMinimumSize(QSize(0, 2));
        line_2->setMaximumSize(QSize(16777215, 2));

        verticalLayout_2->addWidget(line_2);

        stacked_menu_paras = new QStackedWidget(page_para);
        stacked_menu_paras->setObjectName(QString::fromUtf8("stacked_menu_paras"));
        sizePolicy1.setHeightForWidth(stacked_menu_paras->sizePolicy().hasHeightForWidth());
        stacked_menu_paras->setSizePolicy(sizePolicy1);
        page_menu_time = new QWidget();
        page_menu_time->setObjectName(QString::fromUtf8("page_menu_time"));
        page_menu_time->setStyleSheet(QString::fromUtf8("/*ItemScrollArea {border-image: url(:/ui/timebg.png);}*/"));
        verticalLayout_3 = new QVBoxLayout(page_menu_time);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(16);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        scroll_year = new ItemScrollArea(page_menu_time);
        scroll_year->setObjectName(QString::fromUtf8("scroll_year"));
        scroll_year->setMinimumSize(QSize(120, 180));
        scroll_year->setMaximumSize(QSize(120, 180));
        scroll_year->setWidgetResizable(true);
        scrollArea_year = new ItemScrollContent();
        scrollArea_year->setObjectName(QString::fromUtf8("scrollArea_year"));
        scrollArea_year->setGeometry(QRect(0, 0, 120, 180));
        scroll_year->setWidget(scrollArea_year);

        horizontalLayout_2->addWidget(scroll_year);

        scroll_month = new ItemScrollArea(page_menu_time);
        scroll_month->setObjectName(QString::fromUtf8("scroll_month"));
        scroll_month->setMinimumSize(QSize(80, 180));
        scroll_month->setMaximumSize(QSize(80, 180));
        scroll_month->setWidgetResizable(true);
        scrollArea_month = new ItemScrollContent();
        scrollArea_month->setObjectName(QString::fromUtf8("scrollArea_month"));
        scrollArea_month->setGeometry(QRect(0, 0, 80, 180));
        scroll_month->setWidget(scrollArea_month);

        horizontalLayout_2->addWidget(scroll_month);

        scroll_day = new ItemScrollArea(page_menu_time);
        scroll_day->setObjectName(QString::fromUtf8("scroll_day"));
        scroll_day->setMinimumSize(QSize(80, 180));
        scroll_day->setMaximumSize(QSize(80, 180));
        scroll_day->setWidgetResizable(true);
        scrollArea_day = new ItemScrollContent();
        scrollArea_day->setObjectName(QString::fromUtf8("scrollArea_day"));
        scrollArea_day->setGeometry(QRect(0, 0, 80, 180));
        scroll_day->setWidget(scrollArea_day);

        horizontalLayout_2->addWidget(scroll_day);

        horizontalSpacer_2 = new QSpacerItem(38, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(16);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        scroll_hour = new ItemScrollArea(page_menu_time);
        scroll_hour->setObjectName(QString::fromUtf8("scroll_hour"));
        scroll_hour->setMinimumSize(QSize(80, 180));
        scroll_hour->setMaximumSize(QSize(80, 180));
        scroll_hour->setWidgetResizable(true);
        scrollArea_hour = new ItemScrollContent();
        scrollArea_hour->setObjectName(QString::fromUtf8("scrollArea_hour"));
        scrollArea_hour->setGeometry(QRect(0, 0, 80, 180));
        scroll_hour->setWidget(scrollArea_hour);

        horizontalLayout_3->addWidget(scroll_hour);

        scroll_minute = new ItemScrollArea(page_menu_time);
        scroll_minute->setObjectName(QString::fromUtf8("scroll_minute"));
        scroll_minute->setMinimumSize(QSize(80, 180));
        scroll_minute->setMaximumSize(QSize(80, 180));
        scroll_minute->setWidgetResizable(true);
        scrollArea_minute = new ItemScrollContent();
        scrollArea_minute->setObjectName(QString::fromUtf8("scrollArea_minute"));
        scrollArea_minute->setGeometry(QRect(0, 0, 80, 180));
        scroll_minute->setWidget(scrollArea_minute);

        horizontalLayout_3->addWidget(scroll_minute);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);


        verticalLayout_3->addLayout(horizontalLayout_3);

        stacked_menu_paras->addWidget(page_menu_time);
        page_menu_volume = new QWidget();
        page_menu_volume->setObjectName(QString::fromUtf8("page_menu_volume"));
        verticalLayout_4 = new QVBoxLayout(page_menu_volume);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        wid_video = new QWidget(page_menu_volume);
        wid_video->setObjectName(QString::fromUtf8("wid_video"));
        wid_video->setMinimumSize(QSize(120, 214));
        wid_video->setMaximumSize(QSize(120, 214));
        verticalLayout_8 = new QVBoxLayout(wid_video);
        verticalLayout_8->setSpacing(4);
        verticalLayout_8->setContentsMargins(0, 0, 0, 0);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        label_video = new QLabel(wid_video);
        label_video->setObjectName(QString::fromUtf8("label_video"));

        verticalLayout_8->addWidget(label_video);

        scroll_volume_video = new ItemScrollArea(wid_video);
        scroll_volume_video->setObjectName(QString::fromUtf8("scroll_volume_video"));
        scroll_volume_video->setMinimumSize(QSize(120, 180));
        scroll_volume_video->setMaximumSize(QSize(120, 180));
        scroll_volume_video->setWidgetResizable(true);
        scrollArea_volume_2 = new ItemScrollContent();
        scrollArea_volume_2->setObjectName(QString::fromUtf8("scrollArea_volume_2"));
        scrollArea_volume_2->setGeometry(QRect(0, 0, 120, 180));
        scroll_volume_video->setWidget(scrollArea_volume_2);

        verticalLayout_8->addWidget(scroll_volume_video);


        horizontalLayout_4->addWidget(wid_video);

        wid_audio = new QWidget(page_menu_volume);
        wid_audio->setObjectName(QString::fromUtf8("wid_audio"));
        wid_audio->setMinimumSize(QSize(120, 214));
        wid_audio->setMaximumSize(QSize(120, 214));
        verticalLayout_9 = new QVBoxLayout(wid_audio);
        verticalLayout_9->setSpacing(4);
        verticalLayout_9->setContentsMargins(0, 0, 0, 0);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        label_audio = new QLabel(wid_audio);
        label_audio->setObjectName(QString::fromUtf8("label_audio"));

        verticalLayout_9->addWidget(label_audio);

        scroll_volume_audio = new ItemScrollArea(wid_audio);
        scroll_volume_audio->setObjectName(QString::fromUtf8("scroll_volume_audio"));
        scroll_volume_audio->setMinimumSize(QSize(120, 180));
        scroll_volume_audio->setMaximumSize(QSize(120, 180));
        scroll_volume_audio->setWidgetResizable(true);
        scrollArea_volume = new ItemScrollContent();
        scrollArea_volume->setObjectName(QString::fromUtf8("scrollArea_volume"));
        scrollArea_volume->setGeometry(QRect(0, 0, 120, 180));
        scroll_volume_audio->setWidget(scrollArea_volume);

        verticalLayout_9->addWidget(scroll_volume_audio);


        horizontalLayout_4->addWidget(wid_audio);


        verticalLayout_4->addLayout(horizontalLayout_4);

        stacked_menu_paras->addWidget(page_menu_volume);
        page_menu_backlight = new QWidget();
        page_menu_backlight->setObjectName(QString::fromUtf8("page_menu_backlight"));
        verticalLayout_5 = new QVBoxLayout(page_menu_backlight);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        scroll_backlight = new ItemScrollArea(page_menu_backlight);
        scroll_backlight->setObjectName(QString::fromUtf8("scroll_backlight"));
        scroll_backlight->setMinimumSize(QSize(150, 180));
        scroll_backlight->setMaximumSize(QSize(150, 180));
        scroll_backlight->setWidgetResizable(true);
        scrollArea_backlight = new ItemScrollContent();
        scrollArea_backlight->setObjectName(QString::fromUtf8("scrollArea_backlight"));
        scrollArea_backlight->setGeometry(QRect(0, 0, 150, 180));
        scroll_backlight->setWidget(scrollArea_backlight);

        horizontalLayout_5->addWidget(scroll_backlight);


        verticalLayout_5->addLayout(horizontalLayout_5);

        stacked_menu_paras->addWidget(page_menu_backlight);
        page_menu_theme = new QWidget();
        page_menu_theme->setObjectName(QString::fromUtf8("page_menu_theme"));
        stacked_menu_paras->addWidget(page_menu_theme);
        page_menu_protocol = new QWidget();
        page_menu_protocol->setObjectName(QString::fromUtf8("page_menu_protocol"));
        verticalLayout_6 = new QVBoxLayout(page_menu_protocol);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(16);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_7);

        scroll_protocol_type = new ItemScrollArea(page_menu_protocol);
        scroll_protocol_type->setObjectName(QString::fromUtf8("scroll_protocol_type"));
        scroll_protocol_type->setMinimumSize(QSize(150, 180));
        scroll_protocol_type->setMaximumSize(QSize(150, 180));
        scroll_protocol_type->setWidgetResizable(true);
        scrollArea_protocol_type = new ItemScrollContent();
        scrollArea_protocol_type->setObjectName(QString::fromUtf8("scrollArea_protocol_type"));
        scrollArea_protocol_type->setGeometry(QRect(0, 0, 150, 180));
        scroll_protocol_type->setWidget(scrollArea_protocol_type);

        horizontalLayout_7->addWidget(scroll_protocol_type);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_8);


        verticalLayout_6->addLayout(horizontalLayout_7);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(16);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);

        scroll_protocol_buad = new ItemScrollArea(page_menu_protocol);
        scroll_protocol_buad->setObjectName(QString::fromUtf8("scroll_protocol_buad"));
        scroll_protocol_buad->setMinimumSize(QSize(120, 180));
        scroll_protocol_buad->setMaximumSize(QSize(120, 180));
        scroll_protocol_buad->setWidgetResizable(true);
        scrollArea_protocol_buad = new ItemScrollContent();
        scrollArea_protocol_buad->setObjectName(QString::fromUtf8("scrollArea_protocol_buad"));
        scrollArea_protocol_buad->setGeometry(QRect(0, 0, 120, 180));
        scroll_protocol_buad->setWidget(scrollArea_protocol_buad);

        horizontalLayout_6->addWidget(scroll_protocol_buad);

        scroll_protocol_id = new ItemScrollArea(page_menu_protocol);
        scroll_protocol_id->setObjectName(QString::fromUtf8("scroll_protocol_id"));
        scroll_protocol_id->setMinimumSize(QSize(120, 180));
        scroll_protocol_id->setMaximumSize(QSize(120, 180));
        scroll_protocol_id->setWidgetResizable(true);
        scrollArea_protocol_id = new ItemScrollContent();
        scrollArea_protocol_id->setObjectName(QString::fromUtf8("scrollArea_protocol_id"));
        scrollArea_protocol_id->setGeometry(QRect(0, 0, 120, 180));
        scroll_protocol_id->setWidget(scrollArea_protocol_id);

        horizontalLayout_6->addWidget(scroll_protocol_id);

        horizontalSpacer_6 = new QSpacerItem(38, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_6);


        verticalLayout_6->addLayout(horizontalLayout_6);

        stacked_menu_paras->addWidget(page_menu_protocol);

        verticalLayout_2->addWidget(stacked_menu_paras);

        StackedMenu->addWidget(page_para);

        retranslateUi(StackedMenu);

        StackedMenu->setCurrentIndex(1);
        stacked_menu_paras->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(StackedMenu);
    } // setupUi

    void retranslateUi(QStackedWidget *StackedMenu)
    {
        StackedMenu->setWindowTitle(QApplication::translate("StackedMenu", "StackedWidget", 0, QApplication::UnicodeUTF8));
        btn_saveexit->setText(QApplication::translate("StackedMenu", "Save and Exit", 0, QApplication::UnicodeUTF8));
        btn_exit->setText(QApplication::translate("StackedMenu", "Exit", 0, QApplication::UnicodeUTF8));
        btn_back->setText(QApplication::translate("StackedMenu", " Back", 0, QApplication::UnicodeUTF8));
        label_title->setText(QApplication::translate("StackedMenu", "Menu", 0, QApplication::UnicodeUTF8));
        btn_back_2->setText(QString());
        label_video->setText(QApplication::translate("StackedMenu", "Video", 0, QApplication::UnicodeUTF8));
        label_audio->setText(QApplication::translate("StackedMenu", "Audio", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class StackedMenu: public Ui_StackedMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STACKEDMENU_H600_H
