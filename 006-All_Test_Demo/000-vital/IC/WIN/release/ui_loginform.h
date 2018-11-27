/********************************************************************************
** Form generated from reading UI file 'loginform.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINFORM_H
#define UI_LOGINFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "uiclass.h"

QT_BEGIN_NAMESPACE

class Ui_LoginForm
{
public:
    QVBoxLayout *verticalLayout;
    ImageForm *wid_image;
    QWidget *horizontalLayoutWidget_5;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_min;
    QPushButton *btn_close;
    QWidget *widget;
    QVBoxLayout *verticalLayout_2;
    QStackedWidget *stackedWidget;
    QWidget *page_login;
    LineEditForm *lineedit_usrname;
    LineEditForm *lineedit_passwd;
    QCheckBox *check_remberpasswd;
    QPushButton *btn_login;
    LinkButton *btn_forgetpasswd;
    QLabel *label_barcode;
    QLabel *label_barcode_info;
    QLabel *label_logintips;
    QWidget *page_register;
    QPushButton *btn_reg_confirm;
    QPushButton *btn_reg_back;
    LineEditForm *lineedit_reg_phone;
    LineEditForm *lineedit_reg_passwd;
    LineEditForm *lineedit_reg_verify;
    QLabel *label_getpasswd_info;

    void setupUi(QDialog *LoginForm)
    {
        if (LoginForm->objectName().isEmpty())
            LoginForm->setObjectName(QStringLiteral("LoginForm"));
        LoginForm->resize(494, 422);
        LoginForm->setMinimumSize(QSize(494, 422));
        LoginForm->setMaximumSize(QSize(494, 422));
        LoginForm->setStyleSheet(QLatin1String("QWidget#widget{background-color: rgb(255, 255, 255);}\n"
"QWidget#wid_image {background-color: rgb(0, 85, 255);}\n"
"\n"
"QLabel#label_title { color:white; }\n"
"\n"
"QLabel#label_logintips, QLabel#label_reg_passwd_tip, QLabel#label_reg_phone_tip, QLabel#label_reg_verify_tip { color:red; }\n"
"\n"
"QCheckBox::indicator {\n"
"    width: 15px;\n"
"    height: 15px;\n"
"}\n"
"\n"
"QCheckBox::indicator:unchecked {\n"
"    image: url(:/icon/rc/check_off.png);\n"
"}\n"
"\n"
"QCheckBox::indicator:checked {\n"
"    image: url(:/icon/rc/check_on.png);\n"
"}\n"
"\n"
"QLabel#label_barcode {border-image: url(:/image/rc/barcode.png);}\n"
"\n"
"QPushButton#btn_login { border-radius: 4px;background-color: rgb(51, 123, 216); min-width: 80px; color:white; }\n"
"QPushButton#btn_login:pressed { border: 1px solid rgb(255, 170, 127); background-color:rgb(46, 0, 140); }\n"
"\n"
"QPushButton#btn_reg_confirm { border-radius: 4px;background-color: rgb(51, 123, 216); min-width: 80px; color:white; }\n"
"QPushButton#btn_reg_confirm:press"
                        "ed { border: 1px solid rgb(255, 170, 127); background-color: rgb(46, 0, 140); }\n"
"\n"
"QPushButton#btn_verify_getcode { border-radius: 4px;background-color: rgb(51, 123, 216); min-width: 80px; color:white; }\n"
"QPushButton#btn_verify_getcode:pressed { border: 1px solid rgb(255, 170, 127); background-color: rgb(46, 0, 140); }\n"
"\n"
"QPushButton#btn_reg_back { border-radius: 4px;background-color: lightgray; min-width: 80px; color:black; }\n"
"QPushButton#btn_reg_back:pressed { border: 1px solid rgb(255, 170, 127); background-color: darkgray; }\n"
"\n"
"QPushButton#btn_close { border-image: url(:/icon/rc/close.png); }\n"
"QPushButton#btn_min { border-image: url(:/icon/rc/minium.png); }"));
        verticalLayout = new QVBoxLayout(LoginForm);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(8, 8, 8, 8);
        wid_image = new ImageForm(LoginForm);
        wid_image->setObjectName(QStringLiteral("wid_image"));
        wid_image->setMinimumSize(QSize(0, 240));
        wid_image->setMaximumSize(QSize(16777215, 240));
        horizontalLayoutWidget_5 = new QWidget(wid_image);
        horizontalLayoutWidget_5->setObjectName(QStringLiteral("horizontalLayoutWidget_5"));
        horizontalLayoutWidget_5->setGeometry(QRect(0, 0, 471, 31));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_5);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        btn_min = new QPushButton(horizontalLayoutWidget_5);
        btn_min->setObjectName(QStringLiteral("btn_min"));
        btn_min->setMinimumSize(QSize(16, 16));
        btn_min->setMaximumSize(QSize(16, 16));
        btn_min->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_2->addWidget(btn_min);

        btn_close = new QPushButton(horizontalLayoutWidget_5);
        btn_close->setObjectName(QStringLiteral("btn_close"));
        btn_close->setMinimumSize(QSize(16, 16));
        btn_close->setMaximumSize(QSize(16, 16));
        btn_close->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_2->addWidget(btn_close);


        verticalLayout->addWidget(wid_image);

        widget = new QWidget(LoginForm);
        widget->setObjectName(QStringLiteral("widget"));
        verticalLayout_2 = new QVBoxLayout(widget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        stackedWidget = new QStackedWidget(widget);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        page_login = new QWidget();
        page_login->setObjectName(QStringLiteral("page_login"));
        lineedit_usrname = new LineEditForm(page_login);
        lineedit_usrname->setObjectName(QStringLiteral("lineedit_usrname"));
        lineedit_usrname->setGeometry(QRect(20, 20, 201, 30));
        lineedit_usrname->setMinimumSize(QSize(0, 30));
        lineedit_usrname->setMaximumSize(QSize(16777215, 30));
        lineedit_usrname->setFocusPolicy(Qt::StrongFocus);
        lineedit_passwd = new LineEditForm(page_login);
        lineedit_passwd->setObjectName(QStringLiteral("lineedit_passwd"));
        lineedit_passwd->setGeometry(QRect(20, 60, 201, 30));
        lineedit_passwd->setMinimumSize(QSize(0, 30));
        lineedit_passwd->setMaximumSize(QSize(16777215, 30));
        lineedit_passwd->setFocusPolicy(Qt::StrongFocus);
        check_remberpasswd = new QCheckBox(page_login);
        check_remberpasswd->setObjectName(QStringLiteral("check_remberpasswd"));
        check_remberpasswd->setGeometry(QRect(30, 107, 71, 16));
        btn_login = new QPushButton(page_login);
        btn_login->setObjectName(QStringLiteral("btn_login"));
        btn_login->setGeometry(QRect(130, 100, 84, 28));
        btn_forgetpasswd = new LinkButton(page_login);
        btn_forgetpasswd->setObjectName(QStringLiteral("btn_forgetpasswd"));
        btn_forgetpasswd->setGeometry(QRect(240, 64, 84, 28));
        label_barcode = new QLabel(page_login);
        label_barcode->setObjectName(QStringLiteral("label_barcode"));
        label_barcode->setGeometry(QRect(360, 20, 85, 85));
        label_barcode->setMinimumSize(QSize(85, 85));
        label_barcode->setMaximumSize(QSize(85, 85));
        label_barcode_info = new QLabel(page_login);
        label_barcode_info->setObjectName(QStringLiteral("label_barcode_info"));
        label_barcode_info->setGeometry(QRect(356, 110, 101, 16));
        label_logintips = new QLabel(page_login);
        label_logintips->setObjectName(QStringLiteral("label_logintips"));
        label_logintips->setGeometry(QRect(30, 130, 311, 31));
        stackedWidget->addWidget(page_login);
        page_register = new QWidget();
        page_register->setObjectName(QStringLiteral("page_register"));
        btn_reg_confirm = new QPushButton(page_register);
        btn_reg_confirm->setObjectName(QStringLiteral("btn_reg_confirm"));
        btn_reg_confirm->setGeometry(QRect(150, 130, 91, 28));
        btn_reg_confirm->setMinimumSize(QSize(80, 28));
        btn_reg_confirm->setMaximumSize(QSize(16777215, 28));
        btn_reg_back = new QPushButton(page_register);
        btn_reg_back->setObjectName(QStringLiteral("btn_reg_back"));
        btn_reg_back->setGeometry(QRect(260, 130, 91, 28));
        btn_reg_back->setMinimumSize(QSize(80, 28));
        btn_reg_back->setMaximumSize(QSize(16777215, 28));
        lineedit_reg_phone = new LineEditForm(page_register);
        lineedit_reg_phone->setObjectName(QStringLiteral("lineedit_reg_phone"));
        lineedit_reg_phone->setGeometry(QRect(140, 10, 210, 30));
        lineedit_reg_phone->setMinimumSize(QSize(210, 30));
        lineedit_reg_phone->setMaximumSize(QSize(210, 30));
        lineedit_reg_passwd = new LineEditForm(page_register);
        lineedit_reg_passwd->setObjectName(QStringLiteral("lineedit_reg_passwd"));
        lineedit_reg_passwd->setGeometry(QRect(140, 47, 210, 30));
        lineedit_reg_passwd->setMinimumSize(QSize(210, 30));
        lineedit_reg_passwd->setMaximumSize(QSize(210, 30));
        lineedit_reg_verify = new LineEditForm(page_register);
        lineedit_reg_verify->setObjectName(QStringLiteral("lineedit_reg_verify"));
        lineedit_reg_verify->setGeometry(QRect(140, 87, 211, 30));
        lineedit_reg_verify->setMinimumSize(QSize(0, 30));
        lineedit_reg_verify->setMaximumSize(QSize(16777215, 30));
        label_getpasswd_info = new QLabel(page_register);
        label_getpasswd_info->setObjectName(QStringLiteral("label_getpasswd_info"));
        label_getpasswd_info->setGeometry(QRect(360, 10, 111, 31));
        stackedWidget->addWidget(page_register);

        verticalLayout_2->addWidget(stackedWidget);


        verticalLayout->addWidget(widget);


        retranslateUi(LoginForm);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(LoginForm);
    } // setupUi

    void retranslateUi(QDialog *LoginForm)
    {
        LoginForm->setWindowTitle(QApplication::translate("LoginForm", "Form", 0));
        btn_min->setText(QString());
        btn_close->setText(QString());
        check_remberpasswd->setText(QApplication::translate("LoginForm", "\350\256\260\344\275\217\345\257\206\347\240\201", 0));
        btn_login->setText(QApplication::translate("LoginForm", "\347\231\273\351\231\206", 0));
        btn_forgetpasswd->setText(QApplication::translate("LoginForm", "\345\277\230\350\256\260\345\257\206\347\240\201", 0));
        label_barcode->setText(QString());
        label_barcode_info->setText(QApplication::translate("LoginForm", "\346\211\253\344\270\200\346\211\253\344\270\213\350\275\275\346\211\213\346\234\272\347\253\257", 0));
        label_logintips->setText(QString());
        btn_reg_confirm->setText(QApplication::translate("LoginForm", "\346\217\220\344\272\244\350\216\267\345\217\226", 0));
        btn_reg_back->setText(QApplication::translate("LoginForm", "\350\277\224\345\233\236\347\231\273\351\231\206", 0));
        label_getpasswd_info->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class LoginForm: public Ui_LoginForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINFORM_H
