/********************************************************************************
** Form generated from reading UI file 'mygame.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYGAME_H
#define UI_MYGAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_mygame
{
public:
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *length;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *width;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *mine;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *quit;
    QSpacerItem *horizontalSpacer;
    QPushButton *mynewgame;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *mygame)
    {
        if (mygame->objectName().isEmpty())
            mygame->setObjectName(QStringLiteral("mygame"));
        mygame->setEnabled(true);
        mygame->resize(455, 347);
        verticalLayout_3 = new QVBoxLayout(mygame);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_4 = new QLabel(mygame);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setMinimumSize(QSize(431, 71));
        label_4->setStyleSheet(QLatin1String("background-color: rgb(50, 45, 210);\n"
"color: rgb(95, 28, 28);\n"
"font: 14pt \"Ubuntu\";\n"
"color: qconicalgradient(cx:0.5, cy:0.5, angle:0, stop:0 rgba(255, 255, 255, 255), stop:0.373979 rgba(255, 255, 255, 255), stop:0.373991 rgba(33, 30, 255, 255), stop:0.624018 rgba(33, 30, 255, 255), stop:0.624043 rgba(255, 0, 0, 255), stop:1 rgba(255, 0, 0, 255));"));
        label_4->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(mygame);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label);

        length = new QLineEdit(mygame);
        length->setObjectName(QStringLiteral("length"));
        length->setEnabled(true);

        horizontalLayout->addWidget(length);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(mygame);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(label_2);

        width = new QLineEdit(mygame);
        width->setObjectName(QStringLiteral("width"));

        horizontalLayout_2->addWidget(width);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_3 = new QLabel(mygame);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(label_3);

        mine = new QLineEdit(mygame);
        mine->setObjectName(QStringLiteral("mine"));

        horizontalLayout_3->addWidget(mine);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        quit = new QPushButton(mygame);
        quit->setObjectName(QStringLiteral("quit"));

        horizontalLayout_4->addWidget(quit);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        mynewgame = new QPushButton(mygame);
        mynewgame->setObjectName(QStringLiteral("mynewgame"));
        mynewgame->setEnabled(false);

        horizontalLayout_4->addWidget(mynewgame);


        verticalLayout->addLayout(horizontalLayout_4);


        horizontalLayout_5->addLayout(verticalLayout);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout_5);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        verticalLayout_3->addLayout(verticalLayout_2);

        label->raise();
        label_2->raise();
        length->raise();
        width->raise();
        label_3->raise();
        mine->raise();
        quit->raise();
        mynewgame->raise();
        length->raise();
        label_4->raise();

        retranslateUi(mygame);

        QMetaObject::connectSlotsByName(mygame);
    } // setupUi

    void retranslateUi(QDialog *mygame)
    {
        mygame->setWindowTitle(QApplication::translate("mygame", "Dialog", 0));
        label_4->setText(QApplication::translate("mygame", "\350\207\252\345\256\232\344\271\211", 0));
        label->setText(QApplication::translate("mygame", "\351\225\277\345\272\246:", 0));
        length->setPlaceholderText(QApplication::translate("mygame", "\351\225\277\345\272\2469~50", 0));
        label_2->setText(QApplication::translate("mygame", "\345\256\275\345\272\246:", 0));
        width->setPlaceholderText(QApplication::translate("mygame", "\345\256\275\345\272\2469~50", 0));
        label_3->setText(QApplication::translate("mygame", "\351\233\267\346\225\260:", 0));
        mine->setPlaceholderText(QApplication::translate("mygame", "\351\233\267\346\225\26010\357\275\236l*w-10", 0));
        quit->setText(QApplication::translate("mygame", "\345\217\226\346\266\210", 0));
        mynewgame->setText(QApplication::translate("mygame", "\346\226\260\346\270\270\346\210\217", 0));
    } // retranslateUi

};

namespace Ui {
    class mygame: public Ui_mygame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYGAME_H
