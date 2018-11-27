/********************************************************************************
** Form generated from reading UI file 'listbox.ui'
**
** Created: Tue Aug 16 10:05:27 2016
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LISTBOX_H
#define UI_LISTBOX_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "ilistview.h"

QT_BEGIN_NAMESPACE

class Ui_ListBox
{
public:
    QVBoxLayout *verticalLayout_2;
    QWidget *wid_list;
    QVBoxLayout *verticalLayout;
    QLabel *label_title;
    QWidget *line;
    ItemScrollArea *scroll_wid;
    ItemScrollContent *scroll_area;

    void setupUi(QWidget *ListBox)
    {
        if (ListBox->objectName().isEmpty())
            ListBox->setObjectName(QString::fromUtf8("ListBox"));
        ListBox->resize(133, 210);
        ListBox->setStyleSheet(QString::fromUtf8("QWidget { background-color: rgba(144,144,144,0); border:none; }\n"
"\n"
"QLabel {font: bold 16pt  \"Arial\"; color:rgb(240,240,240);}\n"
"\n"
"QWidget#wid_list { border-width: 0px; border-style: solid; border-radius: 8px; background-color: rgba(44,44,44,250); }\n"
"\n"
"QWidget#line { border-width: 0px; border-style: solid; border-radius: 8px; background-color: rgba(44,100,200,250); }\n"
"\n"
"\n"
"\n"
"QPushButton { border-width: 0px; border-radius: 8px; background-color: rgba(255, 170, 0, 200); font: bold 18pt  \"Arial\"; color:rgb(255,255,255);}\n"
"QPushButton:pressed { background-color: rgba(255, 85, 0,250); font: bold 18pt  \"Arial\"; color:rgb(255,255,255);}"));
        verticalLayout_2 = new QVBoxLayout(ListBox);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        wid_list = new QWidget(ListBox);
        wid_list->setObjectName(QString::fromUtf8("wid_list"));
        verticalLayout = new QVBoxLayout(wid_list);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_title = new QLabel(wid_list);
        label_title->setObjectName(QString::fromUtf8("label_title"));
        label_title->setMinimumSize(QSize(0, 30));
        label_title->setMaximumSize(QSize(16777215, 30));
        label_title->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_title);

        line = new QWidget(wid_list);
        line->setObjectName(QString::fromUtf8("line"));
        line->setMinimumSize(QSize(0, 3));
        line->setMaximumSize(QSize(16777215, 3));

        verticalLayout->addWidget(line);

        scroll_wid = new ItemScrollArea(wid_list);
        scroll_wid->setObjectName(QString::fromUtf8("scroll_wid"));
        scroll_wid->setMinimumSize(QSize(0, 170));
        scroll_wid->setMaximumSize(QSize(16777215, 170));
        scroll_wid->setWidgetResizable(true);
        scroll_area = new ItemScrollContent();
        scroll_area->setObjectName(QString::fromUtf8("scroll_area"));
        scroll_area->setGeometry(QRect(0, 0, 133, 170));
        scroll_wid->setWidget(scroll_area);

        verticalLayout->addWidget(scroll_wid);


        verticalLayout_2->addWidget(wid_list);


        retranslateUi(ListBox);

        QMetaObject::connectSlotsByName(ListBox);
    } // setupUi

    void retranslateUi(QWidget *ListBox)
    {
        ListBox->setWindowTitle(QApplication::translate("ListBox", "Form", 0, QApplication::UnicodeUTF8));
        label_title->setText(QApplication::translate("ListBox", "Title", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ListBox: public Ui_ListBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LISTBOX_H
