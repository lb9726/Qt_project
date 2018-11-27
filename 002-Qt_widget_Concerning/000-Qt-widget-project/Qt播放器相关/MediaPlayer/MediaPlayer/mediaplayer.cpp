#include "mediaplayer.h"
#include "ui_mediaplayer.h"
#include <QMediaPlayer>
#include <QVBoxLayout>
#include <QVideoWidget>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTimer>

#include "customslider.h"

//播放视频的全局变量
QVBoxLayout* layout_video;
QMediaPlayer* player;
QVideoWidget* vWidget;

//播放状态，true为播放，false为暂停
bool play_state;

//与Slider有关的播放控制变量
QTimer * timer;
int maxValue = 1000;//设置进度条的最大值

//音量控制slider
CustomSlider * slider_volume;


MediaPlayer::MediaPlayer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MediaPlayer)
{
    ui->setupUi(this);
    ui->pushButton_play_and_pause->setEnabled(false);
    ui->pushButton_volume->setEnabled(false);

    //禁用slider_progress，连接信号槽
    ui->slider_progress->setEnabled(false);
    connect(ui->slider_progress,&CustomSlider::costomSliderClicked,this,&MediaPlayer::slider_progress_clicked);
    connect(ui->slider_progress,&CustomSlider::sliderMoved,this,&MediaPlayer::slider_progress_moved);
    connect(ui->slider_progress,&CustomSlider::sliderReleased,this,&MediaPlayer::slider_progress_released);
    
    //手动设置slider_volume 包括初始化 方向 禁用，以及槽函数
    slider_volume = new CustomSlider(this);
    slider_volume->setOrientation(Qt::Vertical);
    slider_volume->setEnabled(false);
    slider_volume->hide();
    //由于不涉及到slider值的刷新，因此只需对move和自定义click两个信号进行处理，并且可以共用一个槽函数
    connect(slider_volume,&CustomSlider::costomSliderClicked,this,&MediaPlayer::slider_volume_changed);
    connect(slider_volume,&CustomSlider::sliderMoved,this,&MediaPlayer::slider_volume_changed);

}
//播放进度控制Slider的槽函数
void MediaPlayer::slider_progress_clicked()
{
    player->setPosition(ui->slider_progress->value()*player->duration()/maxValue);
}

void MediaPlayer::slider_progress_moved()
{
    //暂时停止计时器，在用户拖动过程中不修改slider的值
    timer->stop();
    player->setPosition(ui->slider_progress->value()*player->duration()/maxValue);
}

void MediaPlayer::slider_progress_released()
{
    //用户释放滑块后，重启定时器
    timer->start();
}

//音量控制Slider的槽函数
void MediaPlayer::slider_volume_changed()
{
    player->setVolume(slider_volume->value());
}


MediaPlayer::~MediaPlayer()
{
    delete ui;
}

void MediaPlayer::on_pushButton_open_file_clicked()
{
    //实例化三个全局变量
    layout_video = new QVBoxLayout;
    layout_video->setMargin(1);
    player = new QMediaPlayer;
    vWidget = new QVideoWidget;
    //根据label_player尺寸设置播放区域
    vWidget->resize(ui->label_player->size());
    layout_video->addWidget(vWidget);
    ui->label_player->setLayout(layout_video);
    player->setVideoOutput(vWidget);
qDebug()<< "use on_pushButton_open-libo";
    //选择视频文件
    QString filename = QFileDialog::getOpenFileName(this,tr("选择视频文件"),".",tr("视频格式(*.avi *.mp4 *.flv *.mkv)"));
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(NULL, "Title", "Content", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    //设置播放器
    player->setMedia(QUrl::fromLocalFile(filename));
    //play_state为true表示播放，false表示暂停
    play_state = true;
    //启用播放/暂停按钮，并将其文本设置为“暂停”
    ui->pushButton_play_and_pause->setEnabled(true);
    ui->pushButton_play_and_pause->setText("暂停");
    //播放器开启
    player->play();

    //启用slider并设置范围
    ui->slider_progress->setEnabled(true);
    ui->slider_progress->setRange(0,maxValue);
    timer = new QTimer();
    timer->setInterval(1000);
    timer->start();
    //将timer连接至onTimerOut槽函数
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));

    //启用音量控制slider
    slider_volume->setEnabled(true);
    slider_volume->setRange(0, 100);
    //启用音量按钮
    ui->pushButton_volume->setEnabled(true);
}

void MediaPlayer::on_pushButton_play_and_pause_clicked()
{
    //反转播放状态
    if(play_state)
    {
        player->pause();
        ui->pushButton_play_and_pause->setText("播放");
    }
    else
    {
        player->play();
        ui->pushButton_play_and_pause->setText("暂停");
    }

    play_state = !play_state;
}

void MediaPlayer::onTimerOut()
{
    ui->slider_progress->setValue(player->position()*maxValue/player->duration());
}


bool state_slider_volume = false;
void MediaPlayer::on_pushButton_volume_clicked()
{
    //通过hide()和show()方法，实现音量控制Slider的唤出和收起
    if(state_slider_volume)
    {
        slider_volume->hide();
    }
    else
    {
        slider_volume->setValue(player->volume());
        //计算位置，使其位于音量控制按钮的上方
        slider_volume->setGeometry(QRect(ui->pushButton_volume->pos().rx()+0.5*ui->pushButton_volume->width()-15, ui->pushButton_volume->y()-100 , 30, 102));
        slider_volume->show();
    }
    state_slider_volume = !state_slider_volume;
}
