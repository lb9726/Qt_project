/********************************************************************************
** Form generated from reading UI file 'managerdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MANAGERDIALOG_H
#define UI_MANAGERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <flickcharm.h>

QT_BEGIN_NAMESPACE

class Ui_ManagerDialog
{
public:
    QVBoxLayout *verticalLayout;
    ScrollArea *scrollArea;
    QWidget *scrollContents;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QLineEdit *lineEdit_passwd;
    QTableView *tableView;
    QSpacerItem *verticalSpacer;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_ok;
    QPushButton *btn_cancel;

    void setupUi(QDialog *ManagerDialog)
    {
        if (ManagerDialog->objectName().isEmpty())
            ManagerDialog->setObjectName(QStringLiteral("ManagerDialog"));
        ManagerDialog->resize(507, 465);
        ManagerDialog->setStyleSheet(QLatin1String("/*QWidget { background: rgba(0, 0, 0, 200); }*/\n"
"\n"
"QScrollArea#scrollArea{border:none;}\n"
"\n"
"QScrollBar:vertical { background: rgba(0, 0, 0, 0);  width: 9px;  margin: 0px 0 0px 0; }\n"
"QScrollBar::handle:vertical { background: rgba(195, 195, 195, 255); min-height: 20px;  margin: 2px 0px 1px 0px;   border-radius: 3px;  border: 0px dashed rgba(180,130,100,255); }\n"
"QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }\n"
"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {  background: none; }\n"
"\n"
"QScrollBar:horizontal  { background: rgba(0, 0, 0, 0);  height: 9px;  margin: 0px 0 0px 0; }\n"
"QScrollBar::handle:horizontal  { background: rgba(195, 195, 195, 255);  min-width: 20px;  margin: 2px 0px 1px 0px;   border-radius: 3px;  border: 0px dashed rgba(180,130,100,255); }\n"
"QScrollBar::add-line:horizontal , QScrollBar::sub-line:horizontal  { height: 0px; }\n"
"QScrollBar::add-page:horizontal , QScrollBar::sub-page:horizontal  {  background: none; }"));
        verticalLayout = new QVBoxLayout(ManagerDialog);
        verticalLayout->setSpacing(4);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea = new ScrollArea(ManagerDialog);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        scrollContents = new QWidget();
        scrollContents->setObjectName(QStringLiteral("scrollContents"));
        scrollContents->setGeometry(QRect(0, 0, 498, 431));
        verticalLayout_3 = new QVBoxLayout(scrollContents);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        groupBox = new QGroupBox(scrollContents);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(0, 30));

        verticalLayout_2->addWidget(label);

        lineEdit_passwd = new QLineEdit(groupBox);
        lineEdit_passwd->setObjectName(QStringLiteral("lineEdit_passwd"));
        lineEdit_passwd->setMinimumSize(QSize(0, 30));
        lineEdit_passwd->setMaximumSize(QSize(16777215, 30));

        verticalLayout_2->addWidget(lineEdit_passwd);

        tableView = new QTableView(groupBox);
        tableView->setObjectName(QStringLiteral("tableView"));

        verticalLayout_2->addWidget(tableView);


        verticalLayout_3->addWidget(groupBox);

        verticalSpacer = new QSpacerItem(20, 318, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        scrollArea->setWidget(scrollContents);

        verticalLayout->addWidget(scrollArea);

        widget = new QWidget(ManagerDialog);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(0, 30));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(342, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btn_ok = new QPushButton(widget);
        btn_ok->setObjectName(QStringLiteral("btn_ok"));
        btn_ok->setMinimumSize(QSize(0, 30));
        btn_ok->setMaximumSize(QSize(16777215, 30));

        horizontalLayout->addWidget(btn_ok);

        btn_cancel = new QPushButton(widget);
        btn_cancel->setObjectName(QStringLiteral("btn_cancel"));
        btn_cancel->setMinimumSize(QSize(0, 30));
        btn_cancel->setMaximumSize(QSize(16777215, 30));

        horizontalLayout->addWidget(btn_cancel);


        verticalLayout->addWidget(widget);


        retranslateUi(ManagerDialog);

        QMetaObject::connectSlotsByName(ManagerDialog);
    } // setupUi

    void retranslateUi(QDialog *ManagerDialog)
    {
        ManagerDialog->setWindowTitle(QApplication::translate("ManagerDialog", "Dialog", 0));
        groupBox->setTitle(QString());
        label->setText(QApplication::translate("ManagerDialog", "\345\215\241\345\257\206\347\240\201\350\256\276\345\256\232", 0));
        btn_ok->setText(QApplication::translate("ManagerDialog", "\347\241\256\350\256\244", 0));
        btn_cancel->setText(QApplication::translate("ManagerDialog", "\345\217\226\346\266\210", 0));
    } // retranslateUi

};

namespace Ui {
    class ManagerDialog: public Ui_ManagerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MANAGERDIALOG_H
