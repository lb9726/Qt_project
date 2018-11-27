#include "herodialog.h"

/**************************************
函数名字：heroDialog(QWidget *parent)
函数功能：英雄榜对话框构造函数
函数流程：
    初始化成员
    添加对话框成员
***************************************/
heroDialog::heroDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle("英雄榜");
    glayout = new QGridLayout(this);

    label_low = new QLabel("初    级：");
    label_low_time = new QLabel("999");
    label_low_name = new QLabel("匿名");

    label_middle = new QLabel("中    级：");
    label_middle_time = new QLabel("999");
    label_middle_name = new QLabel("匿名");

    label_high = new QLabel("高    级：");
    label_high_time = new QLabel("999");
    label_high_name = new QLabel("匿名");

    glayout->addWidget(label_low,0,0);
    glayout->addWidget(label_low_time,0,1);
    glayout->addWidget(label_low_name,0,2);

    glayout->addWidget(label_middle,1,0);
    glayout->addWidget(label_middle_time,1,1);
    glayout->addWidget(label_middle_name,1,2);

    glayout->addWidget(label_high,2,0);
    glayout->addWidget(label_high_time,2,1);
    glayout->addWidget(label_high_name,2,2);

    pushButton = new QPushButton("重新计分",this);
    this->connect(pushButton,SIGNAL(clicked()),this,SLOT(setHeroDialog()));

    pushButton_OK = new QPushButton("确    定",this);
    this->connect(pushButton_OK,SIGNAL(clicked()),this,SLOT(close()));
    pushButton_OK->setFocus();  //设置确定键默认选中

    glayout->addWidget(pushButton,3,0);
    glayout->addWidget(pushButton_OK,3,2);
}

/**************************************
函数名字：setHeroDialog()
函数功能：设置英雄榜对话框
函数流程：
    设置英雄榜对话框的内容
    显示
***************************************/
void heroDialog::setHeroDialog() //设置英雄榜对话框的内容
{
    this->label_low_time->setNum(999);
    this->label_low_name->setText("匿名");
    this->label_middle_time->setNum(999);

    this->label_middle_name->setText("匿名");
    this->label_high_time->setNum(999);
    this->label_high_name->setText("匿名");

    this->show();
}
