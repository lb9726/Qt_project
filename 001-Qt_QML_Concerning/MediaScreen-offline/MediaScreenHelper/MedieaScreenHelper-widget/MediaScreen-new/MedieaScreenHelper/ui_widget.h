
#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGroupBox *groupBox;
    QTextEdit *textEdit;
    QLineEdit *lineEdit;
    QLabel *label;
    QLabel *label_2;
    QRadioButton *radioButton_video;
    QRadioButton *radioButton_picture;
    QLineEdit *lineEdit_2;
    QPushButton *pushButton_selectvideo;
    QListView *listView;
    QPushButton *pushbutton_selectpicture;
    QLabel *label_prompt;
    QGroupBox *groupBox_2;
    QCheckBox *checkBox_hideTime;
    QCheckBox *checkBox_hideTitle;
    QCheckBox *checkBox_hideScroText;
    QCheckBox *checkBox_fullscreen;
    QSlider *horizontalSlider_volume;
    QSlider *horizontalSlider_bright;
    QSpinBox *spinBox_volume;
    QSpinBox *spinBox_bright;
    QSlider *horizontalSlider_1_bright;
    QSpinBox *spinBox_1_bright;
    QSlider *horizontalSlider_2_bright;
    QSpinBox *spinBox_2_bright;
    QSpinBox *spinBox_1_time;
    QSpinBox *spinBox_2_time;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QRadioButton *radioButton_12;
    QRadioButton *radioButton_24;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *radioButton_year;
    QRadioButton *radioButton_mouth;
    QGroupBox *groupBox_3;
    QComboBox *comboBox_USB;
    QPushButton *pushButton_makeudisk;
    QPlainTextEdit *plainTextEdit;
    QPushButton *pushButton_copy;
    QCheckBox *checkBox_Reset;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(873, 462);
        QIcon icon;
        icon.addFile(QStringLiteral(":/new/prefix1/images/MediaImage.png"), QSize(), QIcon::Normal, QIcon::Off);
        Widget->setWindowIcon(icon);
        groupBox = new QGroupBox(Widget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(20, 20, 321, 431));
        groupBox->setAlignment(Qt::AlignCenter);
        textEdit = new QTextEdit(groupBox);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(10, 330, 291, 81));
        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(10, 260, 291, 31));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 240, 54, 12));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 310, 91, 16));
        radioButton_video = new QRadioButton(groupBox);
        radioButton_video->setObjectName(QStringLiteral("radioButton_video"));
        radioButton_video->setGeometry(QRect(70, 40, 89, 16));
        radioButton_video->setChecked(true);
        radioButton_picture = new QRadioButton(groupBox);
        radioButton_picture->setObjectName(QStringLiteral("radioButton_picture"));
        radioButton_picture->setGeometry(QRect(190, 40, 89, 16));
        lineEdit_2 = new QLineEdit(groupBox);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(10, 100, 291, 20));
        pushButton_selectvideo = new QPushButton(groupBox);
        pushButton_selectvideo->setObjectName(QStringLiteral("pushButton_selectvideo"));
        pushButton_selectvideo->setGeometry(QRect(100, 200, 81, 23));
        listView = new QListView(groupBox);
        listView->setObjectName(QStringLiteral("listView"));
        listView->setGeometry(QRect(10, 90, 291, 101));
        pushbutton_selectpicture = new QPushButton(groupBox);
        pushbutton_selectpicture->setObjectName(QStringLiteral("pushbutton_selectpicture"));
        pushbutton_selectpicture->setGeometry(QRect(90, 200, 101, 23));
        label_prompt = new QLabel(groupBox);
        label_prompt->setObjectName(QStringLiteral("label_prompt"));
        label_prompt->setGeometry(QRect(13, 70, 291, 20));
        label_prompt->setTextFormat(Qt::RichText);
        label_prompt->setAlignment(Qt::AlignCenter);
        groupBox_2 = new QGroupBox(Widget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(360, 20, 301, 431));
        groupBox_2->setAlignment(Qt::AlignCenter);
        checkBox_hideTime = new QCheckBox(groupBox_2);
        checkBox_hideTime->setObjectName(QStringLiteral("checkBox_hideTime"));
        checkBox_hideTime->setGeometry(QRect(20, 168, 171, 16));
        checkBox_hideTitle = new QCheckBox(groupBox_2);
        checkBox_hideTitle->setObjectName(QStringLiteral("checkBox_hideTitle"));
        checkBox_hideTitle->setGeometry(QRect(20, 190, 151, 16));
        checkBox_hideScroText = new QCheckBox(groupBox_2);
        checkBox_hideScroText->setObjectName(QStringLiteral("checkBox_hideScroText"));
        checkBox_hideScroText->setGeometry(QRect(20, 146, 151, 16));
        checkBox_fullscreen = new QCheckBox(groupBox_2);
        checkBox_fullscreen->setObjectName(QStringLiteral("checkBox_fullscreen"));
        checkBox_fullscreen->setGeometry(QRect(20, 122, 151, 16));
        horizontalSlider_volume = new QSlider(groupBox_2);
        horizontalSlider_volume->setObjectName(QStringLiteral("horizontalSlider_volume"));
        horizontalSlider_volume->setGeometry(QRect(110, 40, 111, 22));
        horizontalSlider_volume->setMaximum(100);
        horizontalSlider_volume->setOrientation(Qt::Horizontal);
        horizontalSlider_bright = new QSlider(groupBox_2);
        horizontalSlider_bright->setObjectName(QStringLiteral("horizontalSlider_bright"));
        horizontalSlider_bright->setGeometry(QRect(110, 70, 111, 22));
        horizontalSlider_bright->setMaximum(100);
        horizontalSlider_bright->setOrientation(Qt::Horizontal);
        spinBox_volume = new QSpinBox(groupBox_2);
        spinBox_volume->setObjectName(QStringLiteral("spinBox_volume"));
        spinBox_volume->setGeometry(QRect(240, 40, 42, 22));
        spinBox_volume->setMaximum(100);
        spinBox_bright = new QSpinBox(groupBox_2);
        spinBox_bright->setObjectName(QStringLiteral("spinBox_bright"));
        spinBox_bright->setGeometry(QRect(240, 70, 42, 22));
        spinBox_bright->setMaximum(100);
        horizontalSlider_1_bright = new QSlider(groupBox_2);
        horizontalSlider_1_bright->setObjectName(QStringLiteral("horizontalSlider_1_bright"));
        horizontalSlider_1_bright->setGeometry(QRect(110, 330, 111, 22));
        horizontalSlider_1_bright->setMaximum(100);
        horizontalSlider_1_bright->setOrientation(Qt::Horizontal);
        spinBox_1_bright = new QSpinBox(groupBox_2);
        spinBox_1_bright->setObjectName(QStringLiteral("spinBox_1_bright"));
        spinBox_1_bright->setGeometry(QRect(240, 330, 42, 22));
        spinBox_1_bright->setMaximum(100);
        horizontalSlider_2_bright = new QSlider(groupBox_2);
        horizontalSlider_2_bright->setObjectName(QStringLiteral("horizontalSlider_2_bright"));
        horizontalSlider_2_bright->setGeometry(QRect(110, 400, 111, 22));
        horizontalSlider_2_bright->setMaximum(100);
        horizontalSlider_2_bright->setOrientation(Qt::Horizontal);
        spinBox_2_bright = new QSpinBox(groupBox_2);
        spinBox_2_bright->setObjectName(QStringLiteral("spinBox_2_bright"));
        spinBox_2_bright->setGeometry(QRect(240, 400, 42, 22));
        spinBox_2_bright->setMaximum(100);
        spinBox_1_time = new QSpinBox(groupBox_2);
        spinBox_1_time->setObjectName(QStringLiteral("spinBox_1_time"));
        spinBox_1_time->setGeometry(QRect(110, 300, 111, 22));
        spinBox_1_time->setMaximum(6000);
        spinBox_2_time = new QSpinBox(groupBox_2);
        spinBox_2_time->setObjectName(QStringLiteral("spinBox_2_time"));
        spinBox_2_time->setGeometry(QRect(110, 360, 111, 22));
        spinBox_2_time->setMaximum(6000);
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 40, 51, 20));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 70, 54, 12));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(20, 300, 81, 16));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 330, 81, 16));
        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(20, 400, 81, 16));
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(20, 360, 71, 16));
        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(40, 215, 54, 21));
        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(220, 209, 61, 31));
        label_10->setAlignment(Qt::AlignCenter);
        layoutWidget = new QWidget(groupBox_2);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 240, 91, 40));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        radioButton_12 = new QRadioButton(layoutWidget);
        radioButton_12->setObjectName(QStringLiteral("radioButton_12"));
        radioButton_12->setChecked(true);

        verticalLayout->addWidget(radioButton_12);

        radioButton_24 = new QRadioButton(layoutWidget);
        radioButton_24->setObjectName(QStringLiteral("radioButton_24"));

        verticalLayout->addWidget(radioButton_24);

        layoutWidget1 = new QWidget(groupBox_2);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(220, 240, 61, 40));
        verticalLayout_2 = new QVBoxLayout(layoutWidget1);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        radioButton_year = new QRadioButton(layoutWidget1);
        radioButton_year->setObjectName(QStringLiteral("radioButton_year"));
        radioButton_year->setChecked(true);

        verticalLayout_2->addWidget(radioButton_year);

        radioButton_mouth = new QRadioButton(layoutWidget1);
        radioButton_mouth->setObjectName(QStringLiteral("radioButton_mouth"));

        verticalLayout_2->addWidget(radioButton_mouth);

        layoutWidget->raise();
        layoutWidget->raise();
        checkBox_hideTime->raise();
        checkBox_hideTitle->raise();
        checkBox_hideScroText->raise();
        checkBox_fullscreen->raise();
        horizontalSlider_volume->raise();
        horizontalSlider_bright->raise();
        spinBox_volume->raise();
        spinBox_bright->raise();
        horizontalSlider_1_bright->raise();
        spinBox_1_bright->raise();
        horizontalSlider_2_bright->raise();
        spinBox_2_bright->raise();
        spinBox_1_time->raise();
        spinBox_2_time->raise();
        label_3->raise();
        label_4->raise();
        label_5->raise();
        label_6->raise();
        label_7->raise();
        label_8->raise();
        label_9->raise();
        label_10->raise();
        groupBox_3 = new QGroupBox(Widget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(680, 60, 171, 171));
        groupBox_3->setAlignment(Qt::AlignCenter);
        comboBox_USB = new QComboBox(groupBox_3);
        comboBox_USB->setObjectName(QStringLiteral("comboBox_USB"));
        comboBox_USB->setGeometry(QRect(40, 60, 91, 31));
        pushButton_makeudisk = new QPushButton(groupBox_3);
        pushButton_makeudisk->setObjectName(QStringLiteral("pushButton_makeudisk"));
        pushButton_makeudisk->setGeometry(QRect(40, 120, 91, 31));
        plainTextEdit = new QPlainTextEdit(Widget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(680, 310, 171, 141));
        pushButton_copy = new QPushButton(Widget);
        pushButton_copy->setObjectName(QStringLiteral("pushButton_copy"));
        pushButton_copy->setGeometry(QRect(720, 420, 91, 31));
        checkBox_Reset = new QCheckBox(Widget);
        checkBox_Reset->setObjectName(QStringLiteral("checkBox_Reset"));
        checkBox_Reset->setGeometry(QRect(740, 20, 81, 16));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0));
        groupBox->setTitle(QApplication::translate("Widget", " Resource Import", 0));
        label->setText(QApplication::translate("Widget", "Title", 0));
        label_2->setText(QApplication::translate("Widget", "Scroll Text", 0));
        radioButton_video->setText(QApplication::translate("Widget", "   video", 0));
        radioButton_picture->setText(QApplication::translate("Widget", " picture", 0));
        lineEdit_2->setPlaceholderText(QApplication::translate("Widget", "\350\257\267\347\202\271\345\207\273 Select video \346\214\211\351\222\256\351\200\211\346\213\251\351\234\200\350\246\201\345\257\274\345\205\245\347\232\204\345\215\225\344\270\252\350\247\206\351\242\221", 0));
        pushButton_selectvideo->setText(QApplication::translate("Widget", "Select Video", 0));
        pushbutton_selectpicture->setText(QApplication::translate("Widget", "Select Picture", 0));
        label_prompt->setText(QApplication::translate("Widget", "   \350\257\267\347\202\271\345\207\273Select Picture \346\214\211\351\222\256\357\274\214\351\200\211\346\213\251\351\234\200\350\246\201\345\257\274\345\205\245\347\232\204\345\233\276\347\211\207", 0));
        groupBox_2->setTitle(QApplication::translate("Widget", " Parameter Configure", 0));
        checkBox_hideTime->setText(QApplication::translate("Widget", "Hidden Time and Date Area", 0));
        checkBox_hideTitle->setText(QApplication::translate("Widget", "Hidden Title Area", 0));
        checkBox_hideScroText->setText(QApplication::translate("Widget", "Hidden ScrollText Area", 0));
        checkBox_fullscreen->setText(QApplication::translate("Widget", "Show Full Screen", 0));
        label_3->setText(QApplication::translate("Widget", "volume", 0));
        label_4->setText(QApplication::translate("Widget", "bright ", 0));
        label_5->setText(QApplication::translate("Widget", "Stage1-time", 0));
        label_6->setText(QApplication::translate("Widget", "Stage1-bright", 0));
        label_7->setText(QApplication::translate("Widget", "Stage2-bright", 0));
        label_8->setText(QApplication::translate("Widget", "Stage2-time", 0));
        label_9->setText(QApplication::translate("Widget", "\346\227\266\351\227\264\346\240\274\345\274\217", 0));
        label_10->setText(QApplication::translate("Widget", "\346\227\245\346\234\237\346\240\274\345\274\217", 0));
        radioButton_12->setText(QApplication::translate("Widget", "12\345\260\217\346\227\266\346\240\274\345\274\217", 0));
        radioButton_24->setText(QApplication::translate("Widget", "24\345\260\217\346\227\266\346\240\274\345\274\217", 0));
        radioButton_year->setText(QApplication::translate("Widget", "\345\271\264\346\234\210\346\227\245", 0));
        radioButton_mouth->setText(QApplication::translate("Widget", "\346\234\210\346\227\245\345\271\264", 0));
        groupBox_3->setTitle(QApplication::translate("Widget", " Make Tools", 0));
        pushButton_makeudisk->setText(QApplication::translate("Widget", "Make UDisk", 0));
        pushButton_copy->setText(QApplication::translate("Widget", "Copy File", 0));
        checkBox_Reset->setText(QApplication::translate("Widget", " Reset", 0));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
