/********************************************************************************
** Form generated from reading UI file 'mainform.ui'
**
** Created: Fri Nov 3 16:02:10 2017
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINFORM_H
#define UI_MAINFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <widgetarrow.h>
#include <widgetbase.h>
#include <widgetfloor.h>
#include "widgetbutton.h"
#include "widgetmedia.h"

QT_BEGIN_NAMESPACE

class Ui_MainForm
{
public:
    WidgetBase *MainWidget;
    WidgetFloor *Floor;
    WidgetBase *single;
    WidgetBase *ten;
    QLabel *time;
    QLabel *date;
    WidgetButton *Button_1;
    WidgetMedia *Media;
    WidgetButton *Button_close;
    WidgetBase *Function;
    WidgetButton *Button_open;
    WidgetArrow *Arrow;
    WidgetButton *Button_2;

    void setupUi(QMainWindow *MainForm)
    {
        if (MainForm->objectName().isEmpty())
            MainForm->setObjectName(QString::fromUtf8("MainForm"));
        MainForm->setEnabled(true);
        MainForm->resize(800, 1280);
        MainForm->setMinimumSize(QSize(800, 1280));
        MainForm->setMaximumSize(QSize(800, 1280));
        MainForm->setStyleSheet(QString::fromUtf8(""));
        MainWidget = new WidgetBase(MainForm);
        MainWidget->setObjectName(QString::fromUtf8("MainWidget"));
        MainWidget->setMinimumSize(QSize(800, 1280));
        MainWidget->setMaximumSize(QSize(800, 1280));
        MainWidget->setSizeIncrement(QSize(0, 0));
        MainWidget->setStyleSheet(QString::fromUtf8("QWidget{background-color:rgba(255, 255, 255,0);}\n"
"QWidget#MainWidget {border-image:url(D:/mls/BVT5/518.5/Glari_510.1A_V/BST_PLUGIN_JULI_101_1280x800_0420V2/usrfs/THEME/PLUGIN/Theme.png);}\n"
"\n"
"\n"
"QLabel#date {font: 9pt \"\345\256\213\344\275\223\"; font-size:40pt;  color:rgb(255, 255, 255);} \n"
"\n"
"QLabel#time {font: 9pt \"\345\256\213\344\275\223\"; font-size:40pt;  color:rgb(255, 255, 255);}\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
""));
        Floor = new WidgetFloor(MainWidget);
        Floor->setObjectName(QString::fromUtf8("Floor"));
        Floor->setGeometry(QRect(460, 610, 252, 169));
        single = new WidgetBase(Floor);
        single->setObjectName(QString::fromUtf8("single"));
        single->setGeometry(QRect(126, 0, 126, 169));
        ten = new WidgetBase(Floor);
        ten->setObjectName(QString::fromUtf8("ten"));
        ten->setGeometry(QRect(0, 0, 126, 169));
        time = new QLabel(MainWidget);
        time->setObjectName(QString::fromUtf8("time"));
        time->setGeometry(QRect(470, 80, 0, 0));
        date = new QLabel(MainWidget);
        date->setObjectName(QString::fromUtf8("date"));
        date->setGeometry(QRect(140, 77, 0, 0));
        Button_1 = new WidgetButton(MainWidget);
        Button_1->setObjectName(QString::fromUtf8("Button_1"));
        Button_1->setGeometry(QRect(120, 816, 205, 203));
        Media = new WidgetMedia(MainWidget);
        Media->setObjectName(QString::fromUtf8("Media"));
        Media->setGeometry(QRect(20, 106, 760, 448));
        Button_close = new WidgetButton(MainWidget);
        Button_close->setObjectName(QString::fromUtf8("Button_close"));
        Button_close->setGeometry(QRect(480, 1040, 205, 203));
        Function = new WidgetBase(MainWidget);
        Function->setObjectName(QString::fromUtf8("Function"));
        Function->setGeometry(QRect(290, 570, 200, 50));
        Button_open = new WidgetButton(MainWidget);
        Button_open->setObjectName(QString::fromUtf8("Button_open"));
        Button_open->setGeometry(QRect(120, 1040, 205, 203));
        Arrow = new WidgetArrow(MainWidget);
        Arrow->setObjectName(QString::fromUtf8("Arrow"));
        Arrow->setGeometry(QRect(146, 610, 147, 173));
        Button_2 = new WidgetButton(MainWidget);
        Button_2->setObjectName(QString::fromUtf8("Button_2"));
        Button_2->setGeometry(QRect(480, 816, 205, 203));
        MainForm->setCentralWidget(MainWidget);

        retranslateUi(MainForm);

        QMetaObject::connectSlotsByName(MainForm);
    } // setupUi

    void retranslateUi(QMainWindow *MainForm)
    {
        MainForm->setWindowTitle(QApplication::translate("MainForm", "MainWindow", 0, QApplication::UnicodeUTF8));
        time->setText(QApplication::translate("MainForm", "11:00", 0, QApplication::UnicodeUTF8));
        date->setText(QApplication::translate("MainForm", "2016-07-27", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainForm: public Ui_MainForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINFORM_H
