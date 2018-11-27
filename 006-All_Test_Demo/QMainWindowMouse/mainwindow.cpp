#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMouseTracking(true);
    ui->centralWidget->setMouseTracking(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::eventFilter(QObject *o, QEvent *e)
{
    if ( (e->type() == QEvent::MouseButtonPress))
    {
        QString str1 = QString::number(e->x());
        QString str2 = QString::number(e->y());
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<str1<<str2;
    }
}
