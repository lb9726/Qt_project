#include "fileupdate.h"
#include "ui_fileupdate.h"
#include <QPalette>
#include <QPixmap>
static int enterflag;
FileUpdate::FileUpdate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileUpdate)
{
    qDebug()<< __PRETTY_FUNCTION__;
    ui->setupUi(this);
    ui->progressBar->hide();
    ui->label_text->hide();
    SetWidgetBackGround(":/new/prefix1/images/bground.png");
    Protocoldeal *pro = Protocoldeal::GetInstance();
    connect(pro, SIGNAL(SendPercent(int)), this, SLOT(ChangeValue(int)));
    enterflag = 0;
}

FileUpdate::~FileUpdate()
{
    qDebug()<< __PRETTY_FUNCTION__;
    delete ui;
}

void FileUpdate::ChangeValue(int val)
{
    qDebug()<< __PRETTY_FUNCTION__<<"val = "<< val;
    if (0 == enterflag)
    {
        ui->progressBar->show();
        ui->label_text->show();
        enterflag ++;
    }

    ui->progressBar->setValue(val);
}

//  设置图片作为widget的背景
void FileUpdate::SetWidgetBackGround(QString path)
{
    qDebug()<<__PRETTY_FUNCTION__ << "start";
    QPalette pal;
    QPixmap pixmap(path);
    pal.setBrush(QPalette::Window, QBrush(pixmap));
    setPalette(pal);
    qDebug()<<__PRETTY_FUNCTION__ << "end";
}

//void FileUpdate::OnDeStroyScreen(FileUpdate *f)
//{
//    f->hide();
//    delete f;
//}
