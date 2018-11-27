/********************************************************************************
** Form generated from reading UI file 'query.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QUERY_H
#define UI_QUERY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_query
{
public:
    QVBoxLayout *verticalLayout;
    QTableView *tableView;

    void setupUi(QDialog *query)
    {
        if (query->objectName().isEmpty())
            query->setObjectName(QStringLiteral("query"));
        query->resize(530, 300);
        query->setMinimumSize(QSize(530, 0));
        query->setMaximumSize(QSize(530, 16777215));
        verticalLayout = new QVBoxLayout(query);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tableView = new QTableView(query);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setMinimumSize(QSize(520, 0));
        tableView->setMaximumSize(QSize(530, 16777215));

        verticalLayout->addWidget(tableView);


        retranslateUi(query);

        QMetaObject::connectSlotsByName(query);
    } // setupUi

    void retranslateUi(QDialog *query)
    {
        query->setWindowTitle(QApplication::translate("query", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class query: public Ui_query {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QUERY_H
