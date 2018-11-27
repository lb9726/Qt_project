/********************************************************************************
** Form generated from reading UI file 'carditem.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CARDITEM_H
#define UI_CARDITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CardItem
{
public:
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_log;
    QPushButton *btn_act;
    QPushButton *btn_LogOut;

    void setupUi(QWidget *CardItem)
    {
        if (CardItem->objectName().isEmpty())
            CardItem->setObjectName(QStringLiteral("CardItem"));
        CardItem->resize(269, 43);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CardItem->sizePolicy().hasHeightForWidth());
        CardItem->setSizePolicy(sizePolicy);
        CardItem->setStyleSheet(QLatin1String("QPushButton#btn_act{\n"
"						background:qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(128, 181, 235, 255), stop:0.495 										rgba(119, 169, 231, 255), stop:0.505 rgba(110,157, 226, 255), stop:1 rgba(109, 155, 0225, 255));\n"
"						color:white;\n"
"						border:1px solid rgb(86,89,93);\n"
"						border-radius: 3px;			\n"
"}\n"
"QPushButton#btn_log{\n"
"						background:qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(128, 181, 235, 255), stop:0.495 										rgba(119, 169, 231, 255), stop:0.505 rgba(110,157, 226, 255), stop:1 rgba(109, 155, 0225, 255));\n"
"						color:white;\n"
"						border:1px solid rgb(86,89,93);\n"
"						border-radius: 3px;			\n"
"}\n"
"\n"
"QPushButton#btn_act::pressed{\n"
"						background:qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(143, 167, 197, 255), stop:0.495 										rgba(103, 133, 171, 255), stop:0.505 rgba(93,124,165, 255), stop:1 rgba(65, 99,146, 255));\n"
"						color:white;\n"
"						border:1px solid rgb(106,118,132)"
                        ";\n"
"						border-radius: 3px;			\n"
"}\n"
"QPushButton#btn_log::pressed{\n"
"						background:qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(143, 167, 197, 255), stop:0.495 										rgba(103, 133, 171, 255), stop:0.505 rgba(93,124,165, 255), stop:1 rgba(65, 99,146, 255));\n"
"						color:white;\n"
"						border:1px solid rgb(106,118,132);\n"
"						border-radius: 3px;\n"
"}\n"
"\n"
"QPushButton#btn_LogOut{\n"
"						background:qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(128, 181, 235, 255), stop:0.495 										rgba(119, 169, 231, 255), stop:0.505 rgba(110,157, 226, 255), stop:1 rgba(109, 155, 0225, 255));\n"
"						color:white;\n"
"						border:1px solid rgb(86,89,93);\n"
"						border-radius: 3px;			\n"
"}\n"
"\n"
"QPushButton#btn_LogOut::pressed{\n"
"						background:qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(143, 167, 197, 255), stop:0.495 										rgba(103, 133, 171, 255), stop:0.505 rgba(93,124,165, 255), stop:1 rgba(65, 99,146, 255));\n"
"			"
                        "			color:white;\n"
"						border:1px solid rgb(106,118,132);\n"
"						border-radius: 3px;			\n"
"}\n"
""));
        horizontalLayout = new QHBoxLayout(CardItem);
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(2, 2, 2, 2);
        btn_log = new QPushButton(CardItem);
        btn_log->setObjectName(QStringLiteral("btn_log"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(btn_log->sizePolicy().hasHeightForWidth());
        btn_log->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(btn_log);

        btn_act = new QPushButton(CardItem);
        btn_act->setObjectName(QStringLiteral("btn_act"));
        sizePolicy1.setHeightForWidth(btn_act->sizePolicy().hasHeightForWidth());
        btn_act->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(btn_act);

        btn_LogOut = new QPushButton(CardItem);
        btn_LogOut->setObjectName(QStringLiteral("btn_LogOut"));
        sizePolicy1.setHeightForWidth(btn_LogOut->sizePolicy().hasHeightForWidth());
        btn_LogOut->setSizePolicy(sizePolicy1);
        btn_LogOut->setMinimumSize(QSize(0, 0));

        horizontalLayout->addWidget(btn_LogOut);


        retranslateUi(CardItem);

        QMetaObject::connectSlotsByName(CardItem);
    } // setupUi

    void retranslateUi(QWidget *CardItem)
    {
        CardItem->setWindowTitle(QApplication::translate("CardItem", "Form", 0));
        btn_log->setText(QApplication::translate("CardItem", "\345\215\241\350\256\260\345\275\225", 0));
        btn_act->setText(QApplication::translate("CardItem", "\350\241\245\345\215\241", 0));
        btn_LogOut->setText(QApplication::translate("CardItem", "\346\263\250\351\224\200", 0));
    } // retranslateUi

};

namespace Ui {
    class CardItem: public Ui_CardItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CARDITEM_H
