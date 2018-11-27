/********************************************************************************
** Form generated from reading UI file 'menuitem.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MENUITEM_H
#define UI_MENUITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MenuItem
{
public:
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_name;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_left;
    QLabel *label_value;
    QPushButton *btn_right;

    void setupUi(QWidget *MenuItem)
    {
        if (MenuItem->objectName().isEmpty())
            MenuItem->setObjectName(QStringLiteral("MenuItem"));
        MenuItem->resize(607, 83);
        MenuItem->setStyleSheet(QLatin1String("QWidget#MenuItem{border-image:url(:/SetMenu/rc/SetMenu/setitem.png);}\n"
"QLabel{font:bold larger\"Arial\";font-size: 25px; color:white;}\n"
"\n"
"QPushButton#btn_left{border-image:url(:/SetMenu/rc/SetMenu/left_off.png);}\n"
"QPushButton#btn_left::pressed{border-image:url(:/SetMenu/rc/SetMenu/left_on.png);}\n"
"\n"
"QPushButton#btn_right{border-image:url(:/SetMenu/rc/SetMenu/right_off.png);}\n"
"QPushButton#btn_right::pressed{border-image:url(:/SetMenu/rc/SetMenu/right_on.png);}"));
        horizontalLayout_2 = new QHBoxLayout(MenuItem);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(9, 9, 9, 9);
        label_name = new QLabel(MenuItem);
        label_name->setObjectName(QStringLiteral("label_name"));
        label_name->setMinimumSize(QSize(130, 0));
        label_name->setMaximumSize(QSize(130, 16777215));

        horizontalLayout_2->addWidget(label_name);

        horizontalSpacer = new QSpacerItem(250, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        btn_left = new QPushButton(MenuItem);
        btn_left->setObjectName(QStringLiteral("btn_left"));
        btn_left->setMinimumSize(QSize(90, 50));
        btn_left->setMaximumSize(QSize(90, 50));

        horizontalLayout->addWidget(btn_left);

        label_value = new QLabel(MenuItem);
        label_value->setObjectName(QStringLiteral("label_value"));
        label_value->setMinimumSize(QSize(80, 50));
        label_value->setMaximumSize(QSize(80, 50));
        label_value->setScaledContents(false);
        label_value->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_value);

        btn_right = new QPushButton(MenuItem);
        btn_right->setObjectName(QStringLiteral("btn_right"));
        btn_right->setMinimumSize(QSize(90, 50));
        btn_right->setMaximumSize(QSize(90, 50));

        horizontalLayout->addWidget(btn_right);


        horizontalLayout_2->addLayout(horizontalLayout);


        retranslateUi(MenuItem);

        QMetaObject::connectSlotsByName(MenuItem);
    } // setupUi

    void retranslateUi(QWidget *MenuItem)
    {
        MenuItem->setWindowTitle(QApplication::translate("MenuItem", "Form", Q_NULLPTR));
        label_name->setText(QString());
        btn_left->setText(QString());
        label_value->setText(QString());
        btn_right->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MenuItem: public Ui_MenuItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MENUITEM_H
