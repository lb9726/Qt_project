#include "cmysplashscreen.h"
#include <QTime>
#include <QTimer>
#include <QPalette>
#include <QFont>
#include <QCoreApplication>
#include <QDebug>
#include <QRect>
#include <QFontMetrics>

CMySplashScreen::CMySplashScreen(int time) :
    elapseTime(time),
    runPath(QCoreApplication::applicationDirPath())
{
    bootLogo = new QLabel(this);
    ProgressBar = new QProgressBar(this);
    sysInfo = new QLabel(this);
    appInfo = new QLabel(this);

    this->setBackGround(runPath + "/Theme/sodimas/desktop/bootBg.png");
    this->setLogoPosition(runPath + "/Theme/sodimas/desktop/logo.png");
    this->setProgressBarPositon();
    this->setTextSysInfo("SYS:0.1.k.180211");
    this->setTextAppInfo("APP:0.9.1.180211");
    this->setSysAndAppPosition();
}

CMySplashScreen::~CMySplashScreen()
{
    qDebug()<<"is call "<<__FUNCTION__;
    delete ProgressBar;
    delete sysInfo;
    delete appInfo;
}

void CMySplashScreen::setBackGround(QString path)
{
    this->setGeometry(0, 0, 480, 272);
    qDebug()<<__FUNCTION__ <<" path = "<< path;
    QPixmap tmpixbg(path);
    this->setPixmap(tmpixbg);
}

void CMySplashScreen::setLogoPosition(QString path)
{
    qDebug()<<__FUNCTION__ <<" path = "<< path;
    QPixmap tmpix(path);
    if (NULL != bootLogo)
    {
        bootLogo->setGeometry((480 - tmpix.width())/2, 50 , tmpix.width(), tmpix.height());
        bootLogo->setPixmap(tmpix);
    }
}

void CMySplashScreen::setProgressBarPositon()
{
    if (NULL != ProgressBar)
    {
        ProgressBar->setGeometry((480-252)/2, 130, 252, 30); //设置进度条的位置
        //设置进度条的样式
        ProgressBar->setStyleSheet("QProgressBar { border: 5px solid grey;border-radius: 10px; }QProgressBar {color:grey;font:15px;text-align:center; } QProgressBar::chunk {background-color: rgb(202, 165, 14);}");
        //设置进度条的范围
        ProgressBar->setRange(0, 100); //设置进度条的当前进
        ProgressBar->setValue(0);
        generateAscendRandomNumber();
        setProgress();
    }
}

void CMySplashScreen::setTextSysInfo(QString pSys)
{
    if (NULL != sysInfo)
    {
        sysInfo->setText(pSys);
        sysInfo->setStyleSheet("QLabel {color: white}");
    }
}

void CMySplashScreen::setTextAppInfo(QString pApp)
{
    if (NULL != appInfo)
    {
        appInfo->setText(pApp);
        appInfo->setStyleSheet("QLabel {color: white}");
    }
}

void CMySplashScreen::setSysAndAppPosition()
{
    QFont ft;                    // 设置字号
    ft.setPointSize(12);
    if ( NULL != sysInfo)
    {
        sysInfo->setFont(ft);
    }
    if (NULL != appInfo)
    {
        appInfo->setFont(ft);
    }

    QFontMetrics fm(ft);
    QRect rec = fm.boundingRect(sysInfo->text());
    int sysLength = rec.width() + 10;//这个就获得了字符串所占的像素宽度

    QRect rec2 = fm.boundingRect(appInfo->text());
    int appLength = rec2.width()+ 10;//这个就获得了字符串所占的像素宽度

    sysInfo->setGeometry((480 - sysLength)/2, 200, sysLength, 45);
    appInfo->setGeometry((480 - appLength)/2, 218, appLength, 45);
}

void CMySplashScreen::setProgress()
{
    int tempTime = elapseTime/100;
    for(int i = 0; i < 100; ++i)
    {
       QTimer::singleShot(i*tempTime, this, SLOT(slotUpdateProgress()));
    }
    QTimer::singleShot(elapseTime, this, SLOT(close()));
}

void CMySplashScreen::slotUpdateProgress()
{
    static int num = 0;
    if (num < 96)
    {
        ProgressBar->setValue(numbersList[num]);
    }
    else
    {
        ProgressBar->setValue(100);
    }
    ++num;
}

void CMySplashScreen::generateAscendRandomNumber()
{
    int i;
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    for(i = 0; i < 100; ++i) //生成100个大小在[0, 100]之间的随机数
    {
        numbersList.append(qrand()%101);
    }
    //递增排序
    std::stable_sort(numbersList.begin(), numbersList.end());
}
