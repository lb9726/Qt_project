/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action;
    QAction *action_2;
    QAction *action9_9_10;
    QAction *action16_16_40;
    QAction *action_3;
    QAction *action_4;
    QAction *action_5;
    QAction *action_6;
    QAction *action_7;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *lminenum;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_5;
    QLabel *ltime;
    QLabel *label_3;
    QGraphicsView *graphicsView;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(490, 478);
        action = new QAction(MainWindow);
        action->setObjectName(QStringLiteral("action"));
        action_2 = new QAction(MainWindow);
        action_2->setObjectName(QStringLiteral("action_2"));
        action9_9_10 = new QAction(MainWindow);
        action9_9_10->setObjectName(QStringLiteral("action9_9_10"));
        action16_16_40 = new QAction(MainWindow);
        action16_16_40->setObjectName(QStringLiteral("action16_16_40"));
        action_3 = new QAction(MainWindow);
        action_3->setObjectName(QStringLiteral("action_3"));
        action_4 = new QAction(MainWindow);
        action_4->setObjectName(QStringLiteral("action_4"));
        action_5 = new QAction(MainWindow);
        action_5->setObjectName(QStringLiteral("action_5"));
        action_6 = new QAction(MainWindow);
        action_6->setObjectName(QStringLiteral("action_6"));
        action_7 = new QAction(MainWindow);
        action_7->setObjectName(QStringLiteral("action_7"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        lminenum = new QLabel(centralWidget);
        lminenum->setObjectName(QStringLiteral("lminenum"));
        lminenum->setMinimumSize(QSize(40, 0));
        lminenum->setMaximumSize(QSize(16777215, 27));
        lminenum->setStyleSheet(QLatin1String("background-color: rgb(36, 29, 29);\n"
"color: rgb(230, 21, 21);\n"
"font: 13pt \"Ubuntu\";"));
        lminenum->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(lminenum);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout->addWidget(label_5);

        ltime = new QLabel(centralWidget);
        ltime->setObjectName(QStringLiteral("ltime"));
        ltime->setMaximumSize(QSize(16777215, 27));
        ltime->setStyleSheet(QLatin1String("background-color: rgb(23, 9, 9);\n"
"color: rgb(226, 20, 20);\n"
"font: 13pt \"Ubuntu\";"));
        ltime->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(ltime);

        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMinimumSize(QSize(0, 8));
        label_3->setStyleSheet(QStringLiteral("font: 13pt \"Ubuntu\";"));

        horizontalLayout->addWidget(label_3);


        verticalLayout->addLayout(horizontalLayout);

        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setMinimumSize(QSize(0, 360));

        verticalLayout->addWidget(graphicsView);


        horizontalLayout_2->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 490, 24));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QStringLiteral("menu_3"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menu->addAction(action);
        menu->addAction(action_2);
        menu_2->addAction(action9_9_10);
        menu_2->addAction(action16_16_40);
        menu_2->addAction(action_3);
        menu_3->addAction(action_4);
        menu_3->addAction(action_7);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        action->setText(QApplication::translate("MainWindow", "\346\226\260\346\270\270\346\210\217", 0));
        action_2->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272", 0));
        action9_9_10->setText(QApplication::translate("MainWindow", "9*9 10\344\270\252\351\233\267", 0));
        action16_16_40->setText(QApplication::translate("MainWindow", "16*16 40\344\270\252\351\233\267 ", 0));
        action_3->setText(QApplication::translate("MainWindow", "\350\207\252\345\256\232\344\271\211", 0));
        action_4->setText(QApplication::translate("MainWindow", "\346\270\270\346\210\217\350\247\204\345\210\231  ", 0));
        action_5->setText(QApplication::translate("MainWindow", "\345\205\263\344\272\216\345\274\200\345\217\221\350\200\205", 0));
        action_6->setText(QApplication::translate("MainWindow", "\350\203\234\345\210\251\350\200\205\347\225\231\347\210\252", 0));
        action_7->setText(QApplication::translate("MainWindow", "\346\217\220\347\244\272", 0));
        label->setText(QApplication::translate("MainWindow", "\351\233\267\346\225\260:", 0));
        lminenum->setText(QApplication::translate("MainWindow", "0", 0));
        pushButton->setText(QString());
        label_5->setText(QApplication::translate("MainWindow", "\346\227\266\351\227\264:", 0));
        ltime->setText(QApplication::translate("MainWindow", "00:00", 0));
        label_3->setText(QApplication::translate("MainWindow", "s", 0));
        menu->setTitle(QApplication::translate("MainWindow", "\346\270\270\346\210\217 ", 0));
        menu_2->setTitle(QApplication::translate("MainWindow", "\351\232\276\345\272\246", 0));
        menu_3->setTitle(QApplication::translate("MainWindow", "\345\270\256\345\212\251", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
