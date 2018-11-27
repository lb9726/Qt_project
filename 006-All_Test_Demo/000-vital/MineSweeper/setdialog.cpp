#include "setdialog.h"

/***********************************************************************
函数名字：setDialog(QWidget *parent)
函数功能：setDialog构造函数
函数流程：
    初始化成员
************************************************************************/
setDialog::setDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle("自定义");
    gridLayout = new QGridLayout(this);
    label_1 = new QLabel("行数(9-24)：");
    label_2 = new QLabel("列数(9-30)：");;
    label_3 = new QLabel("雷数(10-668)：");;

    spinBox_row = new QSpinBox;
    spinBox_row->setRange(9,24);
    spinBox_col = new QSpinBox;
    spinBox_col->setRange(9,30);
    spinBox_mine = new QSpinBox;
    spinBox_mine->setRange(0,668);

    button_1 = new QPushButton("确定");
    this->connect(button_1,SIGNAL(clicked()),this,SLOT(dealWithSetInfo()));
    button_2 = new QPushButton("取消");
    this->connect(button_2,SIGNAL(clicked()),this,SLOT(close()));

    gridLayout->addWidget(label_1,0,0);
    gridLayout->addWidget(label_2,1,0);
    gridLayout->addWidget(label_3,2,0);
    gridLayout->addWidget(spinBox_row,0,1);
    gridLayout->addWidget(spinBox_col,1,1);
    gridLayout->addWidget(spinBox_mine,2,1);

    gridLayout->addWidget(button_1,0,2);
    gridLayout->addWidget(button_2,2,2);
}

/***********************************************************************
函数名字：dealWithSetInfo()
函数功能：接受信息被设置函数(槽)
函数流程：
    从对话框接收输入的值
    发送这些值
    关闭对话框
************************************************************************/
void setDialog::dealWithSetInfo()	//接受信息被设置函数(槽)
{
    int row_str = this->spinBox_row->value();
    int col_str = this->spinBox_col->value();
    int mine_str = this->spinBox_mine->value();
    emit sendSetInfo(row_str,col_str,mine_str);
    this->close();
}
