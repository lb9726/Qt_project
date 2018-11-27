/********************************************************************************
** Form generated from reading UI file 'messagebox.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESSAGEBOX_H
#define UI_MESSAGEBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MessageBox
{
public:
    QLabel *label_warn;
    QPushButton *btn_cancel;
    QPushButton *btn_ok;
    QWidget *icon_warn;

    void setupUi(QDialog *MessageBox)
    {
        if (MessageBox->objectName().isEmpty())
            MessageBox->setObjectName(QStringLiteral("MessageBox"));
        MessageBox->setEnabled(true);
        MessageBox->resize(590, 199);
        MessageBox->setStyleSheet(QLatin1String("QDialog#MessageBox { border-image: url(:/Message/rc/Message/messagebox.png);}\n"
"\n"
"QLabel#label_warn{ font: bold \"Arial\"; font-size:13pt; color: rgba(255,255,255,250); }\n"
"\n"
"QWidget#icon_warn { border-image: url(:/Message/rc/Message/error.png); }\n"
"\n"
"QPushButton#btn_cancel, QPushButton#btn_ok{ font: bold large \"Adobe Arabic\"; font-size:30pt; color: rgba(255,255,255,250); }\n"
"\n"
"QPushButton#btn_cancel{ border-image: url(:/Message/rc/Message/btndel.png); }\n"
"QPushButton#btn_cancel::pressed{ border-image: url(:/Message/rc/Message/btnlight.png); }\n"
"QPushButton#btn_cancel::disabled {border-image: url(:/Message/rc/Message/btndisable.png);}\n"
"\n"
"QPushButton#btn_ok{ border-image: url(:/Message/rc/Message/btnadd.png); }\n"
"QPushButton#btn_ok::pressed{ border-image: url(:/Message/rc/Message/btnlight.png); }\n"
""));
        label_warn = new QLabel(MessageBox);
        label_warn->setObjectName(QStringLiteral("label_warn"));
        label_warn->setGeometry(QRect(130, 30, 431, 81));
        btn_cancel = new QPushButton(MessageBox);
        btn_cancel->setObjectName(QStringLiteral("btn_cancel"));
        btn_cancel->setEnabled(true);
        btn_cancel->setGeometry(QRect(189, 123, 133, 70));
        btn_ok = new QPushButton(MessageBox);
        btn_ok->setObjectName(QStringLiteral("btn_ok"));
        btn_ok->setGeometry(QRect(320, 123, 135, 70));
        icon_warn = new QWidget(MessageBox);
        icon_warn->setObjectName(QStringLiteral("icon_warn"));
        icon_warn->setGeometry(QRect(28, 22, 86, 86));

        retranslateUi(MessageBox);

        QMetaObject::connectSlotsByName(MessageBox);
    } // setupUi

    void retranslateUi(QDialog *MessageBox)
    {
        MessageBox->setWindowTitle(QApplication::translate("MessageBox", "Dialog", 0));
        label_warn->setText(QString());
        btn_cancel->setText(QApplication::translate("MessageBox", "Cancel", 0));
        btn_ok->setText(QApplication::translate("MessageBox", "OK", 0));
    } // retranslateUi

};

namespace Ui {
    class MessageBox: public Ui_MessageBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESSAGEBOX_H
