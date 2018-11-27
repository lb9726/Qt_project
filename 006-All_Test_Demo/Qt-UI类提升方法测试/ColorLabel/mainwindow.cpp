#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_SelectColorButton_clicked()
{
    QColor color =  QColorDialog::getColor(Qt::white);
    if(color.isValid())
    {
        ui->colorLab->setColor(color);
        ui->colorLab->update();
    }
}
