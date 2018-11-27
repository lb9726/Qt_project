#include "managerdialog.h"
#include "ui_managerdialog.h"
#include "icmanager.h"

ManagerDialog::ManagerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManagerDialog)
{
    ui->setupUi(this);

    setWindowTitle(GBKToUTF8("后台设置菜单"));
    //setWindowFlags(Qt::FramelessWindowHint);

    mParent = (ICManager*)(parent);

    ui->lineEdit_passwd->setText(mParent->mPasswd);
}

ManagerDialog::~ManagerDialog()
{
    delete ui;
}

void ManagerDialog::on_btn_ok_clicked()
{
    mParent->SetPasswd(ui->lineEdit_passwd->text());
    accept();
}

void ManagerDialog::on_btn_cancel_clicked()
{
    reject();
}
