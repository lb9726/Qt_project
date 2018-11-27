#include "dialog.h"
#include <QGridLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QColorDialog>
#include <QFontDialog>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("各种标准对话框的实例"));
    fileBtn = new QPushButton;                    // 各个控件对象的初始化
    fileBtn -> setText(tr("文件标准对话框实例"));
    fileLineEdit = new QLineEdit;                 // 用来显示选择的文件名
    mainLayout = new QGridLayout(this);           // 布局设计
    mainLayout -> addWidget(fileBtn,0,0);
    mainLayout -> addWidget(fileLineEdit,0,1);
    connect(fileBtn, SIGNAL(clicked()), this, SLOT(showFile()));  // 事件关联

    colorBtn = new QPushButton;                   // 创建各个空间的对象
    colorBtn -> setText(tr("颜色标准对话框实例"));
    colorFrame = new QFrame;
    colorFrame -> setFrameShape(QFrame::Box);
    colorFrame -> setAutoFillBackground(true);
    mainLayout -> addWidget(colorBtn, 1, 0);
    mainLayout -> addWidget(colorFrame, 1, 1);
    connect(colorBtn, SIGNAL(clicked()), this, SLOT(showColor())); // 事件关联

    fontBtn = new QPushButton;                  // 创建控件对象
    fontBtn -> setText(tr("字体标准对话框实例"));
    fontLineEdit = new QLineEdit;               //  显示更改的字符串
    fontLineEdit -> setText(tr("Welcome!"));    //
    mainLayout -> addWidget(fontBtn, 2, 0);
    mainLayout -> addWidget(fontLineEdit, 2, 1);
    connect(fontBtn, SIGNAL(clicked()), this, SLOT(showFont()));   // 事件关联

    inputBtn = new QPushButton;
    inputBtn -> setText(tr("标准输入对话框实例"));
    mainLayout -> addWidget(inputBtn, 3, 0);
    connect(inputBtn, SIGNAL(clicked()), this, SLOT(showInputDlg()));
}
void Dialog::showFile()
{
    QString s = QFileDialog::getOpenFileName(this, "open this dialog", "/", "c++ file(*.cpp)::C files(*.c)::Head files(*.h)");
    fileLineEdit -> setText(s);
}

void Dialog::showColor()
{
    QColor c = QColorDialog::getColor(Qt::blue);
    if (c.isValid()) {
        colorFrame -> setPalette(QPalette(c));
    }
}

void Dialog::showFont()
{
    bool ok;
    QFont f = QFontDialog::getFont(&ok);
    if (ok) {
        fontLineEdit -> setFont(f);
    }
}

void Dialog::showInputDlg()
{
    InputDlg = new InputDlg(this);
    InputDlg -> show();
}

Dialog::~Dialog()
{

}
