#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    tmp = 100;
    connect(&qtime, SIGNAL(timeout()), this, SLOT(setBackLight()));
    qtime.start(2000);
}

MainWindow::~MainWindow()
{

}

void MainWindow::setBackLight()
{
    bac.getLightGrade();
    tmp = tmp - 10;
    if (tmp < 0)
    {
        tmp = 100;
    }
    bac.setBacklightValue(tmp);
}
