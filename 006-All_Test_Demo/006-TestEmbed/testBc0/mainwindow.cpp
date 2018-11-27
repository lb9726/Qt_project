#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QThread>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<<"bts will start in specialbtn set priority and remove specialbtn thread"<<__FUNCTION__;
//    bts.start();
    sbt.start();
    t1.start();
    t2.start();
    t3.start();
    t4.start();
    t5.start();
    connect(&timers, &QTimer::timeout, &gps, &Gpios::startCheckBtnStatus, Qt::QueuedConnection);
    timers.start(10);
}

MainWindow::~MainWindow()
{
    delete ui;
}
