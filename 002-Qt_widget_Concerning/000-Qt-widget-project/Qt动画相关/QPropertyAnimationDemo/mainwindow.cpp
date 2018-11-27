#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
    QMovie* movie=new QMovie(":/circle/gif/animated.gif");
    ui->label->setMovie(movie);
    movie->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_left_clicked()
{
    QLabel *label = new QLabel(this);
    label->resize(this->centralWidget()->size());
    label->setPixmap(this->centralWidget()->grab());
    label->show();
    QPropertyAnimation *animation= new QPropertyAnimation(label,"geometry");
    animation->setDuration(1000);
    animation->setStartValue(this->centralWidget()->geometry());
    animation->setEndValue(QRect(-this->centralWidget()->width(), 0, this->centralWidget()->width(), this->centralWidget()->height()));
    //animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start();
    connect(animation,SIGNAL(finished()), animation, SLOT(deleteLater()));
    connect(animation,SIGNAL(finished()), label, SLOT(deleteLater()));
    qDebug()<<"left";
}

void MainWindow::on_btn_right_clicked()
{
    QLabel *label = new QLabel(this);
    label->resize(this->centralWidget()->size());
    label->setPixmap(this->centralWidget()->grab());
    label->show();

    QPropertyAnimation *animation= new QPropertyAnimation(label,"geometry");
    animation->setDuration(1000);
    animation->setStartValue(this->centralWidget()->geometry());
    animation->setEndValue(QRect(this->centralWidget()->width(), 0, this->centralWidget()->width(), this->centralWidget()->height()));

    QPropertyAnimation *animation1= new QPropertyAnimation(this->centralWidget(),"geometry");
    animation1->setDuration(1000);
    animation1->setStartValue(QRect(-this->centralWidget()->width(), 0, this->centralWidget()->width(), this->centralWidget()->height()));
    animation1->setEndValue(this->centralWidget()->geometry());

    QParallelAnimationGroup *group = new QParallelAnimationGroup;
    group->addAnimation(animation);
    group->addAnimation(animation1);
    group->start();
    connect(group,SIGNAL(finished()), group, SLOT(deleteLater()));
    connect(animation,SIGNAL(finished()), animation, SLOT(deleteLater()));
    connect(animation1,SIGNAL(finished()), animation1, SLOT(deleteLater()));
    qDebug()<<"right";
}

void MainWindow::on_btn_up_clicked()
{
    QLabel *label = new QLabel(this);
    label->resize(this->centralWidget()->size());
    label->setPixmap(this->centralWidget()->grab());
    label->show();
    QPropertyAnimation *animation= new QPropertyAnimation(label,"geometry");
    animation->setDuration(800);
    animation->setStartValue(this->centralWidget()->geometry());
    animation->setEndValue(QRect(0, -this->centralWidget()->height(), this->centralWidget()->width(), this->centralWidget()->height()));
    animation->setEasingCurve(QEasingCurve::OutBack);
    animation->start();
    connect(animation,SIGNAL(finished()), animation, SLOT(deleteLater()));
    qDebug()<<"up";
}

void MainWindow::on_btn_down_clicked()
{
    QLabel *label = new QLabel(this);
    label->resize(this->centralWidget()->size());
    label->setPixmap(this->centralWidget()->grab());
    label->show();
    QPropertyAnimation *animation= new QPropertyAnimation(label,"geometry");
    animation->setDuration(800);
    animation->setStartValue(this->centralWidget()->geometry());
    animation->setEndValue(QRect(0, this->centralWidget()->height(), this->centralWidget()->width(), this->centralWidget()->height()));
    animation->start();
    connect(animation,SIGNAL(finished()), animation, SLOT(deleteLater()));
    qDebug()<<"down";
}

void MainWindow::on_pushButton_clicked()
{
    this->close();
}
