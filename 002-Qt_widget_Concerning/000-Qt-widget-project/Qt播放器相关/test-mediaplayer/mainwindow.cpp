#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *widget = new QWidget;
    widget->resize(400, 300);   //

    QVBoxLayout *layout = new QVBoxLayout;
    QMediaPlayer* player = new QMediaPlayer;
    QVideoWidget* vw = new QVideoWidget;

    layout->addWidget(vw);
    widget->setLayout(layout);
    player->setVideoOutput(vw);

    player->setMedia(QUrl::fromLocalFile("/home/devin/Desktop/Test1.mp4"));
    player->play();
    widget->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
