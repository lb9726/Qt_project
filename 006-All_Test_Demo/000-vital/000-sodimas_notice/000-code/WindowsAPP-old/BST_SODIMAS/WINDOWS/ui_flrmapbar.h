/********************************************************************************
** Form generated from reading UI file 'flrmapbar.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FLRMAPBAR_H
#define UI_FLRMAPBAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>
#include <bstui.h>

QT_BEGIN_NAMESPACE

class Ui_FlrmapBar
{
public:
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_level;
    LineEdit *edit_level;
    QPushButton *btn_mapping;
    QStackedWidget *stacked_mapping;
    QWidget *page_char;
    QHBoxLayout *horizontalLayout_2;
    LineEdit *edit_mapping;
    QWidget *page_img;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_mapping;
    QPushButton *btn_switch;
    QWidget *preview;
    QLabel *label_hundred;
    QLabel *label_ten;
    QLabel *label_single;
    QPushButton *btn_audio;
    LineEdit *edit_audio;
    QPushButton *btn_audtest;
    QPushButton *btn_del;

    void setupUi(QWidget *FlrmapBar)
    {
        if (FlrmapBar->objectName().isEmpty())
            FlrmapBar->setObjectName(QStringLiteral("FlrmapBar"));
        FlrmapBar->resize(730, 50);
        FlrmapBar->setStyleSheet(QLatin1String("QWidget#FlrmapBar { background-color: rgba(255, 255, 255, 0); }\n"
"\n"
"QWidget#preview { border-image: url(:/rc/15_Flrmap/edit_num.png);}\n"
"\n"
"QPushButton#btn_del{ font: bold \"Arial\"; font-size: 24pt; color: rgba(255,255,255,255); }\n"
"\n"
"QPushButton#btn_level, QPushButton#btn_mapping, QPushButton#btn_audio{ font: bold \"Arial\"; font-size: 24pt; color: rgba(0,160,233,255); }\n"
"\n"
"QPushButton#btn_level{ border-image: url(:/rc/15_Flrmap/framedark.png); }\n"
"QPushButton#btn_level::pressed{ border-image: url(:/rc/15_Flrmap/framelight.png);  background-color: rgb(255, 85, 0); }\n"
"\n"
"QPushButton#btn_mapping{ border-image: url(:/rc/15_Flrmap/framedark.png); }\n"
"QPushButton#btn_mapping::pressed{ border-image: url(:/rc/15_Flrmap/framelight.png);  background-color: rgb(255, 85, 0); }\n"
"\n"
"QPushButton#btn_audtest{ border-image: url(:/rc/15_Flrmap/btn_audio.png); }\n"
"QPushButton#btn_audtest::pressed{ border-image: url(:/rc/15_Flrmap/btn_audio.png);  background-color: rgb(255, 85, 0); }\n"
"\n"
""
                        "QPushButton#btn_audio{ border-image: url(:/rc/15_Flrmap/framedark.png); }\n"
"QPushButton#btn_audio::pressed{ border-image: url(:/rc/15_Flrmap/framelight.png);  background-color: rgb(255, 85, 0); }\n"
"\n"
"QPushButton#btn_del{ border-image: url(:/rc/15_Flrmap/btndel.png); }\n"
"QPushButton#btn_del::pressed{ border-image: url(:/rc/15_Flrmap/btnlight.png);  }\n"
"\n"
"QPushButton#btn_switch{ border-image: url(:/rc/15_Flrmap/undo.png); }\n"
"QPushButton#btn_switch::pressed{ border-image: url(:/rc/15_Flrmap/undo.png); background-color: rgb(255, 85, 0);  }\n"
"\n"
"QLineEdit{ border-image: url(:/rc/15_Flrmap/edit_num.png); background-color: rgba(255, 255, 255, 0); font: large \"Arial\"; font-size: 26pt; color: rgba(255,255,255,250); border: 0px solid rgb(10,10,10);/*border-radius: 15px;*/}\n"
"\n"
"QLabel#label_mapping { border-image: url(:/rc/15_Flrmap/edit_num.png); }\n"
""));
        horizontalLayout = new QHBoxLayout(FlrmapBar);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        btn_level = new QPushButton(FlrmapBar);
        btn_level->setObjectName(QStringLiteral("btn_level"));
        btn_level->setEnabled(false);

        horizontalLayout->addWidget(btn_level);

        edit_level = new LineEdit(FlrmapBar);
        edit_level->setObjectName(QStringLiteral("edit_level"));
        edit_level->setMinimumSize(QSize(0, 41));
        edit_level->setMaximumSize(QSize(16777215, 41));
        edit_level->setAlignment(Qt::AlignCenter);
        edit_level->setReadOnly(true);

        horizontalLayout->addWidget(edit_level);

        btn_mapping = new QPushButton(FlrmapBar);
        btn_mapping->setObjectName(QStringLiteral("btn_mapping"));

        horizontalLayout->addWidget(btn_mapping);

        stacked_mapping = new QStackedWidget(FlrmapBar);
        stacked_mapping->setObjectName(QStringLiteral("stacked_mapping"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(stacked_mapping->sizePolicy().hasHeightForWidth());
        stacked_mapping->setSizePolicy(sizePolicy);
        page_char = new QWidget();
        page_char->setObjectName(QStringLiteral("page_char"));
        horizontalLayout_2 = new QHBoxLayout(page_char);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        edit_mapping = new LineEdit(page_char);
        edit_mapping->setObjectName(QStringLiteral("edit_mapping"));
        edit_mapping->setMinimumSize(QSize(0, 41));
        edit_mapping->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(edit_mapping);

        stacked_mapping->addWidget(page_char);
        page_img = new QWidget();
        page_img->setObjectName(QStringLiteral("page_img"));
        horizontalLayout_3 = new QHBoxLayout(page_img);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_mapping = new QLabel(page_img);
        label_mapping->setObjectName(QStringLiteral("label_mapping"));
        label_mapping->setMinimumSize(QSize(0, 41));
        label_mapping->setMaximumSize(QSize(16777215, 41));
        label_mapping->setScaledContents(true);

        horizontalLayout_3->addWidget(label_mapping);

        btn_switch = new QPushButton(page_img);
        btn_switch->setObjectName(QStringLiteral("btn_switch"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(btn_switch->sizePolicy().hasHeightForWidth());
        btn_switch->setSizePolicy(sizePolicy1);
        btn_switch->setMinimumSize(QSize(30, 41));
        btn_switch->setMaximumSize(QSize(30, 41));
        btn_switch->setStyleSheet(QStringLiteral("QPushButton#btn_switch::pressed { border:none; background-color: qlineargradient(spread:pad, x1:0.04, y1:0, x2:0, y2:1, stop:0 rgba(255, 85, 0, 255), stop:1 rgba(255, 170, 0, 255));}"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/0_common/rc/0_common/undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_switch->setIcon(icon);
        btn_switch->setFlat(true);

        horizontalLayout_3->addWidget(btn_switch);

        stacked_mapping->addWidget(page_img);

        horizontalLayout->addWidget(stacked_mapping);

        preview = new QWidget(FlrmapBar);
        preview->setObjectName(QStringLiteral("preview"));
        preview->setMinimumSize(QSize(71, 41));
        preview->setMaximumSize(QSize(71, 41));
        label_hundred = new QLabel(preview);
        label_hundred->setObjectName(QStringLiteral("label_hundred"));
        label_hundred->setGeometry(QRect(2, 2, 16, 41));
        label_hundred->setMinimumSize(QSize(0, 0));
        label_hundred->setScaledContents(true);
        label_ten = new QLabel(preview);
        label_ten->setObjectName(QStringLiteral("label_ten"));
        label_ten->setGeometry(QRect(24, 2, 16, 41));
        label_ten->setMinimumSize(QSize(0, 0));
        label_ten->setScaledContents(true);
        label_single = new QLabel(preview);
        label_single->setObjectName(QStringLiteral("label_single"));
        label_single->setGeometry(QRect(47, 2, 16, 41));
        label_single->setMinimumSize(QSize(0, 0));
        label_single->setScaledContents(true);

        horizontalLayout->addWidget(preview);

        btn_audio = new QPushButton(FlrmapBar);
        btn_audio->setObjectName(QStringLiteral("btn_audio"));

        horizontalLayout->addWidget(btn_audio);

        edit_audio = new LineEdit(FlrmapBar);
        edit_audio->setObjectName(QStringLiteral("edit_audio"));
        edit_audio->setMinimumSize(QSize(0, 41));
        edit_audio->setMaximumSize(QSize(16777215, 41));
        edit_audio->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(edit_audio);

        btn_audtest = new QPushButton(FlrmapBar);
        btn_audtest->setObjectName(QStringLiteral("btn_audtest"));
        btn_audtest->setMinimumSize(QSize(38, 35));

        horizontalLayout->addWidget(btn_audtest);

        btn_del = new QPushButton(FlrmapBar);
        btn_del->setObjectName(QStringLiteral("btn_del"));
        btn_del->setMinimumSize(QSize(80, 50));

        horizontalLayout->addWidget(btn_del);

        btn_level->raise();
        btn_mapping->raise();
        edit_level->raise();
        preview->raise();
        btn_del->raise();
        edit_audio->raise();
        btn_audio->raise();
        btn_audtest->raise();
        stacked_mapping->raise();

        retranslateUi(FlrmapBar);

        stacked_mapping->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(FlrmapBar);
    } // setupUi

    void retranslateUi(QWidget *FlrmapBar)
    {
        FlrmapBar->setWindowTitle(QApplication::translate("FlrmapBar", "Form", 0));
        btn_level->setText(QApplication::translate("FlrmapBar", "Level", 0));
        edit_level->setText(QString());
        btn_mapping->setText(QApplication::translate("FlrmapBar", "Text", 0));
        edit_mapping->setInputMask(QString());
        edit_mapping->setText(QString());
        label_mapping->setText(QString());
        btn_switch->setText(QString());
        label_hundred->setText(QString());
        label_ten->setText(QString());
        label_single->setText(QString());
        btn_audio->setText(QApplication::translate("FlrmapBar", "Audio", 0));
        edit_audio->setInputMask(QString());
        edit_audio->setText(QString());
        btn_audtest->setText(QString());
        btn_del->setText(QApplication::translate("FlrmapBar", "Del", 0));
    } // retranslateUi

};

namespace Ui {
    class FlrmapBar: public Ui_FlrmapBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FLRMAPBAR_H
