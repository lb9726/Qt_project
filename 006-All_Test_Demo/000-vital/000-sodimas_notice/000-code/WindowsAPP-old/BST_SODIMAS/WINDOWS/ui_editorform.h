/********************************************************************************
** Form generated from reading UI file 'editorform.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITORFORM_H
#define UI_EDITORFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_editorForm
{
public:
    QHBoxLayout *horizontalLayout;
    QTextEdit *CodeEdit;

    void setupUi(QWidget *editorForm)
    {
        if (editorForm->objectName().isEmpty())
            editorForm->setObjectName(QStringLiteral("editorForm"));
        editorForm->resize(400, 300);
        horizontalLayout = new QHBoxLayout(editorForm);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        CodeEdit = new QTextEdit(editorForm);
        CodeEdit->setObjectName(QStringLiteral("CodeEdit"));

        horizontalLayout->addWidget(CodeEdit);


        retranslateUi(editorForm);

        QMetaObject::connectSlotsByName(editorForm);
    } // setupUi

    void retranslateUi(QWidget *editorForm)
    {
        editorForm->setWindowTitle(QApplication::translate("editorForm", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class editorForm: public Ui_editorForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITORFORM_H
