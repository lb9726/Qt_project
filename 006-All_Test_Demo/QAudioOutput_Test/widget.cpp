#include "widget.h"
#include "ui_widget.h"
#include <QAudioOutput>
#include <QFile>
#include <QAudioFormat>

const QString MediaPath = QString("/usr/bst/usrfs/Theme/sodimas/media/");

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    init();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init()
{
    QFile inputFile;
    inputFile.setFileName(MediaPath + "M43.wav");
    inputFile.open(QIODevice::ReadOnly);

    //设置采样格式
    QAudioFormat audioFormat;
    //设置采样率
    audioFormat.setSampleRate(44100);
    //设置通道数
    audioFormat.setChannelCount(2);
    //设置采样大小，一般为8位或16位
    audioFormat.setSampleSize(16);
    //设置编码方式
    audioFormat.setCodec("audio/pcm");
    //设置字节序
    audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    //设置样本数据类型
    audioFormat.setSampleType(QAudioFormat::UnSignedInt);

    QAudioOutput *audio = new QAudioOutput( audioFormat, 0);
    audio->start(&inputFile);
}
