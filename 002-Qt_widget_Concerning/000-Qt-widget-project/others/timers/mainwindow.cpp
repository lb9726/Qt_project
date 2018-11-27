#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTimer>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(slotPrint()));
    timer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotPrint()
{
    qDebug()<<"1";
}
