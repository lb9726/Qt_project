/********************************************************************************
** Form generated from reading UI file 'lineeditform.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LINEEDITFORM_H
#define UI_LINEEDITFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include "uiclass.h"

QT_BEGIN_NAMESPACE

class Ui_LineEditForm
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    LineEdit *lineEdit;

    void setupUi(QWidget *LineEditForm)
    {
        if (LineEditForm->objectName().isEmpty())
            LineEditForm->setObjectName(QStringLiteral("LineEditForm"));
        LineEditForm->resize(216, 34);
        LineEditForm->setStyleSheet(QStringLiteral(""));
        horizontalLayout = new QHBoxLayout(LineEditForm);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(6, 2, 6, 2);
        label = new QLabel(LineEditForm);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(30, 30));
        label->setMaximumSize(QSize(30, 30));
        label->setFocusPolicy(Qt::NoFocus);
        label->setScaledContents(true);

        horizontalLayout->addWidget(label);

        lineEdit = new LineEdit(LineEditForm);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());
        lineEdit->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(lineEdit);


        retranslateUi(LineEditForm);

        QMetaObject::connectSlotsByName(LineEditForm);
    } // setupUi

    void retranslateUi(QWidget *LineEditForm)
    {
        LineEditForm->setWindowTitle(QApplication::translate("LineEditForm", "Form", 0));
        label->setText(QString());
        lineEdit->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class LineEditForm: public Ui_LineEditForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LINEEDITFORM_H
