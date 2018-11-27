/********************************************************************************
** Form generated from reading UI file 'imageselectdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGESELECTDIALOG_H
#define UI_IMAGESELECTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ImageSelectDialog
{
public:
    QVBoxLayout *verticalLayout;
    QListWidget *listWidget;
    QFrame *line;
    QHBoxLayout *horizontalLayout;
    QLabel *label_preview;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *line_imagepath;
    QPushButton *btn_imageselect;
    QFrame *line_2;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ImageSelectDialog)
    {
        if (ImageSelectDialog->objectName().isEmpty())
            ImageSelectDialog->setObjectName(QStringLiteral("ImageSelectDialog"));
        ImageSelectDialog->resize(411, 325);
        ImageSelectDialog->setStyleSheet(QString::fromUtf8("QWidget#ImageSelectDialog{\n"
"        border-image: url(:/dialog/rc/dialog/dialogbg.png);\n"
"}\n"
"\n"
"QLabel{\n"
"        font:800 15px/25px \"\351\273\221\344\275\223\";\n"
"        color: rgba(255,255,255,250);\n"
"        border: 2px dashed rgba(180,130,100,0);\n"
"        border-radius: 15px;\n"
"    	padding: 2px 4px;\n"
"}\n"
"\n"
"QListWidget{\n"
"        font-weight: 800;\n"
"        color: rgba(199,199,14,250);\n"
"        border: 2px dashed rgba(180,130,100,0);\n"
"        border-image: url(:/Setting/rc/Setting/rclistbg.png);\n"
"        border-radius: 10px;\n"
"    	padding: 2px 4px;\n"
"}\n"
"QListWidget::item:selected{ border:none; padding:0px; margin:0px; }\n"
"QListWidget::item:selected::!active{ border-width: 0px ; }\n"
"QListWidget::item:selected::active{ border-width: 3px; }"));
        verticalLayout = new QVBoxLayout(ImageSelectDialog);
#ifndef Q_OS_MAC
        verticalLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        verticalLayout->setContentsMargins(9, 9, 9, 9);
#endif
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        listWidget = new QListWidget(ImageSelectDialog);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setMinimumSize(QSize(0, 120));
        listWidget->setMaximumSize(QSize(16777215, 120));
        listWidget->setStyleSheet(QString::fromUtf8("/*\n"
" \346\260\264\345\271\263\346\273\232\345\212\250\346\235\241\n"
"*/\n"
"QScrollBar:horizontal {\n"
"    background: rgba(0, 0, 0, 0);\n"
"    height: 9px;\n"
"    margin: 0px 0 0px 0;\n"
"}\n"
"QScrollBar::handle:horizontal {\n"
"    background: rgba(195, 195, 195, 255);\n"
"    min-width: 20px;\n"
"    margin: 2px 0px 1px 0px;\n"
"    border-radius: 3px;\n"
"    border: 0px dashed rgba(180,130,100,0);\n"
"}\n"
"QScrollBar::add-line:horizontal {\n"
"    border: 0px solid rgba(180,130,100,0);\n"
"    background: rgba(180,130,100,0);\n"
"    width: 0px;\n"
"    subcontrol-position: right;\n"
"    subcontrol-origin: margin;\n"
"}\n"
"\n"
"QScrollBar::sub-line:horizontal {\n"
"    border: 0px solid rgba(180,130,100,0);\n"
"    background: rgba(180,130,100,0);\n"
"    width: 0px;\n"
"    subcontrol-position: left;\n"
"    subcontrol-origin: margin;\n"
"}\n"
"\n"
"QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {\n"
"    background: rgba(180,130,100,0);\n"
"}\n"
""));
        listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        verticalLayout->addWidget(listWidget);

        line = new QFrame(ImageSelectDialog);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_preview = new QLabel(ImageSelectDialog);
        label_preview->setObjectName(QStringLiteral("label_preview"));
        label_preview->setMinimumSize(QSize(120, 120));
        label_preview->setMaximumSize(QSize(120, 120));
        label_preview->setPixmap(QPixmap(QString::fromUtf8(":/icon/rc/icon/busy.gif")));
        label_preview->setScaledContents(true);

        horizontalLayout->addWidget(label_preview);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        line_imagepath = new QLineEdit(ImageSelectDialog);
        line_imagepath->setObjectName(QStringLiteral("line_imagepath"));

        horizontalLayout_2->addWidget(line_imagepath);

        btn_imageselect = new QPushButton(ImageSelectDialog);
        btn_imageselect->setObjectName(QStringLiteral("btn_imageselect"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icon/rc/icon/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_imageselect->setIcon(icon);
        btn_imageselect->setIconSize(QSize(22, 22));
        btn_imageselect->setFlat(true);

        horizontalLayout_2->addWidget(btn_imageselect);


        verticalLayout_2->addLayout(horizontalLayout_2);


        horizontalLayout->addLayout(verticalLayout_2);


        verticalLayout->addLayout(horizontalLayout);

        line_2 = new QFrame(ImageSelectDialog);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_2);

        buttonBox = new QDialogButtonBox(ImageSelectDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(ImageSelectDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ImageSelectDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ImageSelectDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ImageSelectDialog);
    } // setupUi

    void retranslateUi(QDialog *ImageSelectDialog)
    {
        ImageSelectDialog->setWindowTitle(QApplication::translate("ImageSelectDialog", "Dialog", 0));
        label_preview->setText(QString());
        btn_imageselect->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ImageSelectDialog: public Ui_ImageSelectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGESELECTDIALOG_H
