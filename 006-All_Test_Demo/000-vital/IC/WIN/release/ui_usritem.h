/********************************************************************************
** Form generated from reading UI file 'usritem.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USRITEM_H
#define UI_USRITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UsrItem
{
public:
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *btn_destory;

    void setupUi(QWidget *UsrItem)
    {
        if (UsrItem->objectName().isEmpty())
            UsrItem->setObjectName(QStringLiteral("UsrItem"));
        UsrItem->resize(543, 46);
        UsrItem->setStyleSheet(QLatin1String("QPushButton{\n"
"						background:qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(128, 181, 235, 255), stop:0.495 										rgba(119, 169, 231, 255), stop:0.505 rgba(110,157, 226, 255), stop:1 rgba(109, 155, 0225, 255));\n"
"						color:white;\n"
"						border:1px solid rgb(106,118,132);\n"
"						border-radius: 3px;\n"
"}\n"
"\n"
"QPushButton::pressed{\n"
"						background:qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(143, 167, 197, 255), stop:0.495 										rgba(103, 133, 171, 255), stop:0.505 rgba(93,124,165, 255), stop:1 rgba(65, 99,146, 255));\n"
"						color:white;\n"
"						border:1px solid rgb(106,118,132);\n"
"						border-radius: 3px;\n"
"}"));
        horizontalLayout = new QHBoxLayout(UsrItem);
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(2, 2, 2, 2);
        pushButton = new QPushButton(UsrItem);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(UsrItem);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        sizePolicy.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButton_2);

        btn_destory = new QPushButton(UsrItem);
        btn_destory->setObjectName(QStringLiteral("btn_destory"));
        sizePolicy.setHeightForWidth(btn_destory->sizePolicy().hasHeightForWidth());
        btn_destory->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(btn_destory);


        retranslateUi(UsrItem);

        QMetaObject::connectSlotsByName(UsrItem);
    } // setupUi

    void retranslateUi(QWidget *UsrItem)
    {
        UsrItem->setWindowTitle(QApplication::translate("UsrItem", "Form", 0));
        pushButton->setText(QApplication::translate("UsrItem", "\346\237\245\347\234\213\345\215\241", 0));
        pushButton_2->setText(QApplication::translate("UsrItem", "\345\217\221\345\215\241", 0));
        btn_destory->setText(QApplication::translate("UsrItem", "\346\263\250\351\224\200", 0));
    } // retranslateUi

};

namespace Ui {
    class UsrItem: public Ui_UsrItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USRITEM_H
