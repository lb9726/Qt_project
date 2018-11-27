/********************************************************************************
** Form generated from reading UI file 'textinput.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEXTINPUT_H
#define UI_TEXTINPUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <bstui.h>

QT_BEGIN_NAMESPACE

class Ui_TextInput
{
public:
    QVBoxLayout *verticalLayout;
    LineEdit *edit_text;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_cancel;
    QPushButton *btn_ok;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *TextInput)
    {
        if (TextInput->objectName().isEmpty())
            TextInput->setObjectName(QStringLiteral("TextInput"));
        TextInput->resize(266, 125);
        TextInput->setStyleSheet(QLatin1String("QWidget#FlrmapBar { background-color: rgba(255, 255, 255, 0); }\n"
"\n"
"QLabel#label_tip { font: bold \"Arial\"; font-size:16pt; color: rgba(0,160,233,255); }\n"
"\n"
"QLineEdit{ border-image: url(:/rc/15_Flrmap/edit_num.png); background-color: rgba(255, 255, 255, 0); font: large \"Arial\"; font-size:22pt; color: rgba(255,255,255,250); border:0px solid rgb(10,10,10);/*border-radius:15px;*/}\n"
"\n"
"QPushButton#btn_ok{ border-image: url(:/0_common/rc/0_common/btnadd.png);  font: bold \"Arial\"; font-size:18pt; color: rgba(255,255,255,255);}\n"
"QPushButton#btn_ok::pressed{ border-image: url(:/0_common/rc/0_common/btnlight2.png); }\n"
"\n"
"QPushButton#btn_cancel{ border-image: url(:/0_common/rc/0_common/btndel.png);  font: bold \"Arial\"; font-size:18pt; color: rgba(255,255,255,255);}\n"
"QPushButton#btn_cancel::pressed{ border-image: url(:/0_common/rc/0_common/btnlight2.png); }"));
        verticalLayout = new QVBoxLayout(TextInput);
        verticalLayout->setSpacing(12);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(-1, -1, -1, 4);
        edit_text = new LineEdit(TextInput);
        edit_text->setObjectName(QStringLiteral("edit_text"));
        edit_text->setMinimumSize(QSize(0, 41));
        edit_text->setMaximumSize(QSize(16777215, 41));
        edit_text->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(edit_text);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        btn_cancel = new QPushButton(TextInput);
        btn_cancel->setObjectName(QStringLiteral("btn_cancel"));
        btn_cancel->setMinimumSize(QSize(120, 45));
        btn_cancel->setMaximumSize(QSize(120, 45));

        horizontalLayout->addWidget(btn_cancel);

        btn_ok = new QPushButton(TextInput);
        btn_ok->setObjectName(QStringLiteral("btn_ok"));
        btn_ok->setMinimumSize(QSize(120, 45));
        btn_ok->setMaximumSize(QSize(120, 45));

        horizontalLayout->addWidget(btn_ok);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(TextInput);

        QMetaObject::connectSlotsByName(TextInput);
    } // setupUi

    void retranslateUi(QDialog *TextInput)
    {
        TextInput->setWindowTitle(QApplication::translate("TextInput", "Dialog", 0));
        edit_text->setInputMask(QString());
        edit_text->setText(QString());
        btn_cancel->setText(QApplication::translate("TextInput", "CANCEL", 0));
        btn_ok->setText(QApplication::translate("TextInput", "OK", 0));
    } // retranslateUi

};

namespace Ui {
    class TextInput: public Ui_TextInput {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEXTINPUT_H
