/********************************************************************************
** Form generated from reading UI file 'adminitem.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADMINITEM_H
#define UI_ADMINITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AdminItem
{
public:
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QPushButton *pushButton_3;
    QPushButton *btn_destory;

    void setupUi(QWidget *AdminItem)
    {
        if (AdminItem->objectName().isEmpty())
            AdminItem->setObjectName(QStringLiteral("AdminItem"));
        AdminItem->resize(252, 36);
        AdminItem->setStyleSheet(QLatin1String("QPushButton{\n"
"						background:qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(128, 181, 235, 255), stop:0.495 rgba(119, 169, 231, 255), stop:0.505 rgba(110,157, 226, 255), stop:1 rgba(109, 155, 225, 255));\n"
"						color:white;\n"
"						border:1px solid rgb(86,89,93);\n"
"						border-radius: 3px;						\n"
"}\n"
"\n"
"QPushButton::pressed{\n"
"						background:qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(143, 167, 197, 255), stop:0.495 rgba(103, 133, 171, 255), stop:0.505 rgba(93,124,165, 255), stop:1 rgba(65, 99,146, 255));\n"
"						color:white;\n"
"						border:1px solid rgb(106,118,132);\n"
"}\n"
""));
        horizontalLayout = new QHBoxLayout(AdminItem);
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(2, 2, 2, 2);
        pushButton_2 = new QPushButton(AdminItem);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButton_2);

        pushButton = new QPushButton(AdminItem);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButton);

        pushButton_3 = new QPushButton(AdminItem);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        sizePolicy.setHeightForWidth(pushButton_3->sizePolicy().hasHeightForWidth());
        pushButton_3->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButton_3);

        btn_destory = new QPushButton(AdminItem);
        btn_destory->setObjectName(QStringLiteral("btn_destory"));
        sizePolicy.setHeightForWidth(btn_destory->sizePolicy().hasHeightForWidth());
        btn_destory->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(btn_destory);


        retranslateUi(AdminItem);

        QMetaObject::connectSlotsByName(AdminItem);
    } // setupUi

    void retranslateUi(QWidget *AdminItem)
    {
        AdminItem->setWindowTitle(QApplication::translate("AdminItem", "Form", 0));
        pushButton_2->setText(QApplication::translate("AdminItem", "\345\217\221\345\215\241", 0));
        pushButton->setText(QApplication::translate("AdminItem", "\346\237\245\347\234\213\345\267\262\345\217\221\345\215\241", 0));
        pushButton_3->setText(QApplication::translate("AdminItem", "\351\207\215\350\256\276\345\257\206\347\240\201", 0));
        btn_destory->setText(QApplication::translate("AdminItem", "\346\263\250\351\224\200", 0));
    } // retranslateUi

};

namespace Ui {
    class AdminItem: public Ui_AdminItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMINITEM_H
