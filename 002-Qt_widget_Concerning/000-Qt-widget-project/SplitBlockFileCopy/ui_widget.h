/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QProgressBar *progressBar;
    QListView *listView;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_selectFile;
    QPushButton *btn_delete;
    QPushButton *btn_clear;
    QPushButton *btn_copyFile;
    QLabel *label_copyname;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(520, 476);
        progressBar = new QProgressBar(Widget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(140, 340, 251, 23));
        progressBar->setValue(0);
        listView = new QListView(Widget);
        listView->setObjectName(QStringLiteral("listView"));
        listView->setGeometry(QRect(10, 20, 501, 231));
        layoutWidget = new QWidget(Widget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(30, 280, 461, 32));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        btn_selectFile = new QPushButton(layoutWidget);
        btn_selectFile->setObjectName(QStringLiteral("btn_selectFile"));

        horizontalLayout->addWidget(btn_selectFile);

        btn_delete = new QPushButton(layoutWidget);
        btn_delete->setObjectName(QStringLiteral("btn_delete"));

        horizontalLayout->addWidget(btn_delete);

        btn_clear = new QPushButton(layoutWidget);
        btn_clear->setObjectName(QStringLiteral("btn_clear"));

        horizontalLayout->addWidget(btn_clear);

        btn_copyFile = new QPushButton(layoutWidget);
        btn_copyFile->setObjectName(QStringLiteral("btn_copyFile"));

        horizontalLayout->addWidget(btn_copyFile);

        label_copyname = new QLabel(Widget);
        label_copyname->setObjectName(QStringLiteral("label_copyname"));
        label_copyname->setGeometry(QRect(36, 379, 451, 31));
        label_copyname->setAlignment(Qt::AlignCenter);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0));
        btn_selectFile->setText(QApplication::translate("Widget", "\351\200\211\346\213\251\346\226\207\344\273\266", 0));
        btn_delete->setText(QApplication::translate("Widget", "\345\210\240\351\231\244 ", 0));
        btn_clear->setText(QApplication::translate("Widget", "\346\270\205\347\251\272", 0));
        btn_copyFile->setText(QApplication::translate("Widget", "\345\244\215\345\210\266\346\226\207\344\273\266", 0));
        label_copyname->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
