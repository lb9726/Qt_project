#include "widget.h"
#include "ui_widget.h"
#include <QPixmap>
#include <QCoreApplication>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    runPath(QCoreApplication::applicationDirPath())

{
    ui->setupUi(this);
    QPixmap tmp1(runPath + "/Theme/sodimas/desktop/0.png");
    ui->label->setPixmap(tmp1);

    QPixmap tmp2(runPath + "/Theme/sodimas/desktop/bootBg.png");
    ui->label_2->setPixmap(tmp2);

    QPixmap tmp3(runPath + "/Theme/sodimas/desktop/logo.png");
    ui->label_3->setPixmap(tmp3);

    QPixmap tmp4(runPath + "/Theme/sodimas/desktop/T45.png");
    ui->label_4->setPixmap(tmp4);

    connect(&mScreenSaver, SIGNAL(sigSetBlackPicture(int)), this, SLOT(ChangeWidget(int)));
    mScreenSaver.setScreenSaverTime(10000);
    mScreenSaver.setChangeBgTime(5000);

    connect(&timer, SIGNAL(timeout()), this, SLOT(testChangeWidget()));
    timer.start(240000);
    mScreenSaver.enterScreenSaver(100);
    mScreenSaver.setPreWidgetIndex(ui->stackedWidget->currentIndex());
}

Widget::~Widget()
{
    delete ui;
}

void Widget::testChangeWidget()
{
    mScreenSaver.exitScreenSaver();
}

void Widget::ChangeWidget(int index)
{
    qDebug()<<__FUNCTION__<<" index = "<< index;
    ui->stackedWidget->setCurrentIndex(index);
}
