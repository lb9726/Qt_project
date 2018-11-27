/********************************************************************************
** Form generated from reading UI file 'newwiner.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWWINER_H
#define UI_NEWWINER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_newwiner
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *winlable;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *ok;
    QSpacerItem *horizontalSpacer;
    QPushButton *quit;

    void setupUi(QDialog *newwiner)
    {
        if (newwiner->objectName().isEmpty())
            newwiner->setObjectName(QStringLiteral("newwiner"));
        newwiner->resize(345, 300);
        layoutWidget = new QWidget(newwiner);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(40, 20, 275, 211));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        winlable = new QLabel(layoutWidget);
        winlable->setObjectName(QStringLiteral("winlable"));
        winlable->setMinimumSize(QSize(71, 0));

        verticalLayout->addWidget(winlable);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        ok = new QPushButton(layoutWidget);
        ok->setObjectName(QStringLiteral("ok"));
        ok->setEnabled(false);

        horizontalLayout_2->addWidget(ok);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        quit = new QPushButton(layoutWidget);
        quit->setObjectName(QStringLiteral("quit"));

        horizontalLayout_2->addWidget(quit);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(newwiner);

        QMetaObject::connectSlotsByName(newwiner);
    } // setupUi

    void retranslateUi(QDialog *newwiner)
    {
        newwiner->setWindowTitle(QApplication::translate("newwiner", "Dialog", 0));
        winlable->setText(QApplication::translate("newwiner", "TextLabel", 0));
        ok->setText(QApplication::translate("newwiner", "ok", 0));
        quit->setText(QApplication::translate("newwiner", "quit", 0));
    } // retranslateUi

};

namespace Ui {
    class newwiner: public Ui_newwiner {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWWINER_H
