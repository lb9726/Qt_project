#include "messagebox.h"
#include "ui_messagebox.h"

MsgBox::MsgBox(QString pMsg, bool pCancel, MSG_LEVEL pLevel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageBox)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    if(!pCancel)
        ui->btn_cancel->setDisabled(true);
    else
        ui->btn_cancel->setEnabled(true);
    if(pLevel == LEVEL_SUCCESS)
        ui->icon_warn->setStyleSheet("QWidget#icon_warn { border-image: url(:/Message/rc/Message/success.png); }");
    else if(pLevel == LEVEL_QUESTION)
        ui->icon_warn->setStyleSheet("QWidget#icon_warn { border-image: url(:/Message/rc/Message/question.png); }");
    ui->label_warn->setText(pMsg);
    connect(ui->btn_cancel, SIGNAL(clicked()), this, SLOT(slot_Reject()));
    connect(ui->btn_ok, SIGNAL(clicked()), this, SLOT(slot_Accept()));
}

MsgBox::~MsgBox()
{
    delete ui;
}

void MsgBox::SetText(QString pCancel, QString pOK)
{
    ui->btn_ok->setText(pOK);
    ui->btn_cancel->setText(pCancel);
}

void MsgBox::slot_Reject()
{
    QDialog::reject();
}

void MsgBox::slot_Accept()
{
    QDialog::accept();
}

