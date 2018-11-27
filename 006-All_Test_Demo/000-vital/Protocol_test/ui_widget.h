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
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGraphicsView *graphicsView_scroll;
    QLabel *label_date;
    QGraphicsView *graphicsView_logo;
    QLabel *label_time;
    QGraphicsView *graphicsView_Arrow;
    QGraphicsView *graphicsView_floor1;
    QGraphicsView *graphicsView_floor2;
    QGraphicsView *graphicsView_emergency;
    QLabel *label_floor;
    QLabel *label_Arrow;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(1024, 768);
        QFont font;
        font.setFamily(QStringLiteral("FZCuQian-M17S"));
        Widget->setFont(font);
        graphicsView_scroll = new QGraphicsView(Widget);
        graphicsView_scroll->setObjectName(QStringLiteral("graphicsView_scroll"));
        graphicsView_scroll->setGeometry(QRect(549, 696, 324, 36));
        graphicsView_scroll->setFont(font);
        label_date = new QLabel(Widget);
        label_date->setObjectName(QStringLiteral("label_date"));
        label_date->setGeometry(QRect(790, 40, 201, 31));
        label_date->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        graphicsView_logo = new QGraphicsView(Widget);
        graphicsView_logo->setObjectName(QStringLiteral("graphicsView_logo"));
        graphicsView_logo->setGeometry(QRect(294, 40, 128, 54));
        label_time = new QLabel(Widget);
        label_time->setObjectName(QStringLiteral("label_time"));
        label_time->setGeometry(QRect(850, 0, 111, 31));
        label_time->setAlignment(Qt::AlignCenter);
        graphicsView_Arrow = new QGraphicsView(Widget);
        graphicsView_Arrow->setObjectName(QStringLiteral("graphicsView_Arrow"));
        graphicsView_Arrow->setGeometry(QRect(50, 83, 191, 220));
        graphicsView_floor1 = new QGraphicsView(Widget);
        graphicsView_floor1->setObjectName(QStringLiteral("graphicsView_floor1"));
        graphicsView_floor1->setGeometry(QRect(0, 452, 130, 157));
        graphicsView_floor2 = new QGraphicsView(Widget);
        graphicsView_floor2->setObjectName(QStringLiteral("graphicsView_floor2"));
        graphicsView_floor2->setGeometry(QRect(90, 452, 130, 156));
        graphicsView_emergency = new QGraphicsView(Widget);
        graphicsView_emergency->setObjectName(QStringLiteral("graphicsView_emergency"));
        graphicsView_emergency->setGeometry(QRect(73, 695, 171, 54));
        label_floor = new QLabel(Widget);
        label_floor->setObjectName(QStringLiteral("label_floor"));
        label_floor->setGeometry(QRect(20, 452, 200, 156));
        label_floor->setAlignment(Qt::AlignCenter);
        label_Arrow = new QLabel(Widget);
        label_Arrow->setObjectName(QStringLiteral("label_Arrow"));
        label_Arrow->setGeometry(QRect(50, 113, 191, 171));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0));
        label_date->setText(QApplication::translate("Widget", "date", 0));
        label_time->setText(QApplication::translate("Widget", "time", 0));
        label_floor->setText(QString());
        label_Arrow->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
