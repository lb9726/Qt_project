#include "mymsgbox.h"
#include "ui_mymsgbox.h"

MyMsgBox::MyMsgBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyMsgBox)
{
    ui->setupUi(this);
    mContentText = QString();
    mTitleName = QString();
    this->setWindowFlags(Qt::Dialog| Qt::FramelessWindowHint);
    initMsgDailog();
}

MyMsgBox::~MyMsgBox()
{
    delete ui;
}

void MyMsgBox::initMsgDailog()
{
    connect(ui->btnYes, &QPushButton::clicked, this, &MyMsgBox::sltBtnClicked);
    connect(ui->btnNo, &QPushButton::clicked, this, &MyMsgBox::sltBtnClicked);
}

void MyMsgBox::setTitleName(QString pTitleName)
{
    this->setWindowTitle(pTitleName);
}

void MyMsgBox::setTextContent(QString pText)
{
    ui->labelContent->setText(pText);
}

void MyMsgBox::sltBtnClicked()
{
    QPushButton *tmpBtn = nullptr;
    tmpBtn = static_cast<QPushButton *> (sender());
    if (tmpBtn)
    {
        if (QString("btnYes") == tmpBtn->objectName())
        {
            emit sigBtnSelect(QString("yes"));
        }
        else if (QString("btnNo") == tmpBtn->objectName())
        {
            emit sigBtnSelect(QString("no"));
        }
    }
}
