/********************************************************************************
** Form generated from reading UI file 'srnsaveform.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SRNSAVEFORM_H
#define UI_SRNSAVEFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SrnSaveForm
{
public:
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label_logo;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *SrnSaveForm)
    {
        if (SrnSaveForm->objectName().isEmpty())
            SrnSaveForm->setObjectName(QStringLiteral("SrnSaveForm"));
        SrnSaveForm->resize(451, 371);
        SrnSaveForm->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));
        verticalLayout = new QVBoxLayout(SrnSaveForm);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 157, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label_logo = new QLabel(SrnSaveForm);
        label_logo->setObjectName(QStringLiteral("label_logo"));
        label_logo->setScaledContents(true);

        horizontalLayout->addWidget(label_logo);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_2 = new QSpacerItem(20, 156, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        retranslateUi(SrnSaveForm);

        QMetaObject::connectSlotsByName(SrnSaveForm);
    } // setupUi

    void retranslateUi(QWidget *SrnSaveForm)
    {
        SrnSaveForm->setWindowTitle(QApplication::translate("SrnSaveForm", "Form", 0));
        label_logo->setText(QApplication::translate("SrnSaveForm", "Sodimas", 0));
    } // retranslateUi

};

namespace Ui {
    class SrnSaveForm: public Ui_SrnSaveForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SRNSAVEFORM_H
