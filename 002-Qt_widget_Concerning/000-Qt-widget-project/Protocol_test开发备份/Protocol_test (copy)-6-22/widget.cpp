#include "widget.h"
#include "ui_widget.h"
#include <QTimer>
#include <QObject>
#include <stdio.h>
#include <QGraphicsScene>
#include <QPixmap>
#include <QDateTime>
#include <QFont>
#include <QPalette>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include "usbdetect.h"

using namespace std;
static unsigned char sstr[1024];
static unsigned char requestBuf[1000];
QTimer *mtime1;
QTimer *mtime2;
QTimer *mtime3;
QTimer *mtime4;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    qDebug()<<__PRETTY_FUNCTION__ << "start";
    ui->setupUi(this);
    timer = new QTimer(this);
    bool flagtime = connect(timer, SIGNAL(timeout()), this, SLOT(ShowTime()));
    connect(this, SIGNAL(DateChangesSignal()), this, SLOT(ShowDate()));
    timer->start(1000);
    qDebug() << "flagtime = "<< flagtime;

//    writetimer = new QTimer(this);
//    connect(writetimer, SIGNAL(timeout()), this, SLOT(CopyData()));
//    writetimer->start(2000);
    pro = Protocoldeal::GetInstance();
    connect(pro, SIGNAL(AcceptDataFormBottom(unsigned char)), this, SLOT(DealNewData(unsigned char)));

    UiInit();
    qDebug()<<__PRETTY_FUNCTION__ << "end";
}

Widget::~Widget()
{
    qDebug()<<__PRETTY_FUNCTION__ << "start";
    delete pro;
    timer->stop();
    delete timer;
//    writetimer->stop();
//    delete writetimer;
    delete ui;
    qDebug() << __PRETTY_FUNCTION__ << "delete all pointers";
}

void Widget::UiInit()
{
    qDebug()<<__PRETTY_FUNCTION__ << "start";
    ShowDate();
    ShowTime();
    SetWidgetBackGround(":/new/prefix1/images/background.png");     // 设置图片作为widget的背景
    HideGraphicViewBorder();   // 将QGraphicview控件边框设置为透明的
    PosAnimationTextScroll();
//    SetPicture(":/new/prefix1/images/ArrowUp.png", ui->graphicsView_Arrow);
    SetPicture(":/new/prefix1/images/logo.png", ui->graphicsView_logo);
    qDebug()<<__PRETTY_FUNCTION__ << "end";
}

//
void Widget::AnimationForPicture()
{
    qDebug()<<__PRETTY_FUNCTION__ << "start";
    QPixmap image; //定义一张图片
    image.load(":/new/prefix1/images/ArrowUp.png");//加载
    ui->label_Arrow->clear();//清空
    ui->label_Arrow->setPixmap(image);//加载到Label标签
    ui->label_Arrow->show();//显示

//    QLabel *mylabel = new QLabel(this);
//    mylabel->clear();
//    mylabel->setPixmap(image);
    QPropertyAnimation *panimation = new QPropertyAnimation(ui->label_Arrow, "pos");
    panimation->setDuration(10000);
    panimation->setStartValue(QPoint(136, 250));
    panimation->setEndValue(QPoint(136, 115));
    panimation->setLoopCount(-1);
    qDebug() << __PRETTY_FUNCTION__;
    //pAnimation->setEasingCurve(QEasingCurve::OutBounce);  // 缓和曲线风格
    qDebug()<<__PRETTY_FUNCTION__ << "end";
}

// 滚动字幕的动画设置
void Widget::PosAnimationTextScroll()
{
    qDebug()<<__PRETTY_FUNCTION__ << "start";
    QLabel *mylabel = new QLabel(this);
    QString s = "欢迎光临上海贝思特";
    QFont ft;                    // 设置字号
    ft.setPointSize(20);
    mylabel->setFont(ft);

    QPalette pe;                // 设置颜色
    pe.setColor(QPalette::WindowText, Qt::white);
    mylabel->setPalette(pe);

    mylabel->setText(s);
    QPropertyAnimation *panimation = new QPropertyAnimation(mylabel, "pos");
    panimation->setDuration(10000);
    panimation->setStartValue(QPoint(290, 711));
//    panimation->setEndValue(QPoint(this->width() - this->fontMetrics().width(s), 711));
    panimation->setEndValue(QPoint(1024, 711));
    panimation->setLoopCount(-1);
    panimation->start();
    qDebug()<<__PRETTY_FUNCTION__ << "end";
}

void Widget::SetTimerArrowUp()
{
    QPixmap image; //定义一张图片
    image.load(":/new/prefix1/images/ArrowUp.png");//加载
    ui->label_Arrow->clear();//清空
    ui->label_Arrow->setPixmap(image);//加载到Label标签
    ui->label_Arrow->show();//显示

    QTimer *mytime = new QTimer(this);
    mtime1 = mytime;
    connect(mytime, SIGNAL(timeout()), this, SLOT(ChangePositionUp()));
    mytime->start(250);
}

void Widget::SetTimerArrowDn()
{
    QPixmap image; //定义一张图片
    image.load(":/new/prefix1/images/ArrowDn.png");//加载
    ui->label_Arrow->clear();//清空
    ui->label_Arrow->setPixmap(image);//加载到Label标签
    ui->label_Arrow->show();//显示

    QTimer *mytime = new QTimer(this);
    mtime2 = mytime;
    connect(mytime, SIGNAL(timeout()), this, SLOT(ChangePositionDn()));
    mytime->start(1000);
}

// 上箭头移动
void Widget::ChangePositionUp()
{
    static int x = 250;
    static int count = 0;
    if (x - 10 > 115)
    {
        x = x - 10;
        ui->label_Arrow->move(40, x);
    }
    else
    {
        ui->label_Arrow->move(40, 250);
        x = 250;
        count ++;
        if (2 == count)
        {
            count = 0;
            mtime1->stop();
            qDebug() << "stop timer";
            ui->label_Arrow->clear();
        }
    }
}

// 下箭头移动
void Widget::ChangePositionDn()
{
    static int y = 115;
    static int count = 0;
    if (y + 10 < 250)
    {
        y = y + 10;
        ui->label_Arrow->move(40, y);
    }
    else
    {
        ui->label_Arrow->move(40, 115);
        y = 115;
        count ++;
        if (2 == count)
        {
            count = 0;
            mtime2->stop();
            qDebug() <<__PRETTY_FUNCTION__<< "stop timer";
            ui->label_Arrow->clear();
        }
    }
}

// 上箭头闪烁
void Widget::ShiningArrowUp()
{
    QPixmap image; //定义一张图片
    image.load(":/new/prefix1/images/ArrowUp.png");//加载
    ui->label_Arrow->clear();//清空
    ui->label_Arrow->setPixmap(image);//加载到Label标签
    ui->label_Arrow->show();//显示

    QTimer *mytime = new QTimer(this);
    mtime3 = mytime;
    connect(mytime, SIGNAL(timeout()), this, SLOT(OnShiningArrowUp()));
    mytime->start(200);
}

// 下箭头闪烁
void Widget::ShiningArrowDn()
{
    QPixmap image; //定义一张图片
    image.load(":/new/prefix1/images/ArrowDn.png");//加载
    ui->label_Arrow->clear();//清空
    ui->label_Arrow->setPixmap(image);//加载到Label标签
    ui->label_Arrow->show();//显示

    QTimer *mytime = new QTimer(this);
    mtime4 = mytime;
    connect(mytime, SIGNAL(timeout()), this, SLOT(OnShiningArrowDn()));
    mytime->start(200);
}

// 上箭头闪烁的槽函数
void Widget::OnShiningArrowUp()
{
    static float opacity = 0.0;
    static int count = 0;
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
    effect->setOpacity(opacity);
    if (opacity >= 1.0)
    {
        opacity = 0.0;
        effect->setOpacity(opacity);
        ui->label_Arrow->setGraphicsEffect(effect);
    }
    else
    {
        opacity = 1.0;
        effect->setOpacity(opacity);
        ui->label_Arrow->setGraphicsEffect(effect);
    }
    count ++;
    if (3 == count)
    {
        mtime3->stop();
        count = 0;
        qDebug()<< "delete mtime3";
        effect->setOpacity(0.0);
        ui->label_Arrow->setGraphicsEffect(effect);
        delete mtime3;
    }
}

// 下箭头闪烁的槽函数
void Widget::OnShiningArrowDn()
{
    static float opacity = 0.0;
    static int count = 0;
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
    effect->setOpacity(opacity);
    if (opacity >= 1.0)
    {
        opacity = 0.0;
        effect->setOpacity(opacity);
        ui->label_Arrow->setGraphicsEffect(effect);
    }
    else
    {
        opacity = 1.0;
        effect->setOpacity(opacity);
        ui->label_Arrow->setGraphicsEffect(effect);
    }
    count ++;
    if (3 == count)
    {
        mtime4->stop();
        count = 0;
        qDebug()<< "delete mtime4";
        effect->setOpacity(0.0);
        ui->label_Arrow->setGraphicsEffect(effect);
        delete mtime4;
    }
}

//  设置图片作为widget的背景
void Widget::SetWidgetBackGround(QString path)
{
    qDebug()<<__PRETTY_FUNCTION__ << "start";
    QPalette pal;
    QPixmap pixmap(path);
    pal.setBrush(QPalette::Window, QBrush(pixmap));
    setPalette(pal);
    qDebug()<<__PRETTY_FUNCTION__ << "end";
}

// 将QGraphicview控件边框设置为透明的
void Widget::HideGraphicViewBorder()
{
    qDebug()<<__PRETTY_FUNCTION__ << "start";
    ui->graphicsView_Arrow->setStyleSheet("background: transparent;border:0px");
    ui->graphicsView_emergency->setStyleSheet("background: transparent;border:0px");
    ui->graphicsView_floor1->setStyleSheet("background: transparent;border:0px");
    ui->graphicsView_floor2->setStyleSheet("background: transparent;border:0px");
    ui->graphicsView_logo->setStyleSheet("background: transparent;border:0px");
    ui->graphicsView_scroll->setStyleSheet("background: transparent;border:0px");
    qDebug()<<__PRETTY_FUNCTION__ << "end";
}

// 显示图片，将图片显示在指定的QGraphicview 的控件中
void Widget::SetPicture(QString path, QGraphicsView *graphic)
{
    qDebug()<<__PRETTY_FUNCTION__ << "start";
//    this->image = new QImage();
    QImage image;
//    bool flag = image->load(str);
    bool flag = image.load(path);
    cout << "flag = "<< flag << endl;
    cout << "noting"<< endl;
    QGraphicsScene *scene = new QGraphicsScene(this);
//    scene->addPixmap(QPixmap::fromImage(*image));
    scene->addPixmap(QPixmap::fromImage(image));
    graphic->setScene(scene);
    graphic->show();
    qDebug()<<__PRETTY_FUNCTION__ << "end";
}

// 显示日期
void Widget::ShowDate()
{
    qDebug()<<__PRETTY_FUNCTION__ << "start";
    QDateTime Time = QDateTime::currentDateTime();
    QString tim = Time.toString("MM-dd-yyyy");
    qDebug()<<"tim = "<< tim;

    QFont ft;      // 设置字号
    ft.setPointSize(20);
    ui->label_date->setFont(ft);

    QPalette pe;   // 设置颜色
    pe.setColor(QPalette::WindowText,Qt::white);
    ui->label_date->setPalette(pe);
    ui->label_date->setText(tim);
    qDebug()<<__PRETTY_FUNCTION__ << "end";
}

// 显示时间，每秒刷新一次
//QString Widget::GetTime()
//{
//    QDateTime Time = QDateTime::currentDateTime();
////    QString tim = Time.toString("h:mm ap"); // 12小时制
//    QString tim = Time.toString("hh:mm"); // 24小时制
//    qDebug()<<"tim = "<< tim<<'\n';
//    return tim;
//}

// 显示时间，每秒刷新一次
void Widget::ShowTime()
{
    qDebug()<<__PRETTY_FUNCTION__ << "start";
    QTime time = QTime::currentTime();
    QString text = time.toString("hh : mm");
    if ((time.second() % 2) == 0)
        text[3] = ' ';
    if ('0' == text[0] && '0' == text[1]
           && '0' == text[5] && text[6] == '0')
    {
        qDebug()<< "日期变更通知";
        emit DateChangesSignal();
    }

    QFont ft;                    // 设置字号
    ft.setPointSize(20);
    ui->label_time->setFont(ft);

    QPalette pe;                // 设置颜色
    pe.setColor(QPalette::WindowText,Qt::white);
    ui->label_time->setPalette(pe);
    ui->label_time->setText(text);
    qDebug()<< "text = "<< text;
    qDebug()<<__PRETTY_FUNCTION__ << "end";
}

// 解析楼层数据，返回QString的值用于显示楼层
QString Widget::GainFloorNumber(unsigned char* str)
{
    qDebug()<<__PRETTY_FUNCTION__ << "start";
    QString s;
    QString sstr;
    int i;
    s.clear();
    sstr.clear();
    unsigned char *p = str;
    for (i = 4; i >= 1; i--)
    {
        switch (*(p + i)) {
        case 0x30: s.append("0"); break;
        case 0x31: s.append("1"); break;
        case 0x32: s.append("2"); break;
        case 0x33: s.append("3"); break;
        case 0x34: s.append("4"); break;
        case 0x35: s.append("5"); break;
        case 0x36: s.append("6"); break;
        case 0x37: s.append("7"); break;
        case 0x38: s.append("8"); break;
        case 0x39: s.append("9"); break;
        case 0x41: s.append("A"); break;
        case 0x42: s.append("B"); break;
        default:
            break;
        }
    }
    qDebug()<<"s = "<< s;
    if ('0' != s[0])   // 千位数为0
    {
        sstr = s;
    }
    else if ('0' == s[0] && '0' != s[1])  // 千位数为0 百位数不为0
    {
        for (i = 1; i < 4; i++)
            sstr.append(s[i]);
    }
    else if ('0' == s[0] && '0' == s[1] && '0' != s[2])
    {
        for (i = 2; i < 4; i++)
            sstr.append(s[i]);
    }
    else if ('0' == s[0] && '0' == s[1] && '0' == s[2])
    {
        for (i = 3; i < 4; i++)
            sstr.append(s[i]);
    }
    qDebug()<<"sstr = "<< sstr;
    qDebug()<<__PRETTY_FUNCTION__ << "end";
    return sstr;
}

//设置楼层号的槽函数
void Widget::SetFloorNumber(QString s)
{
    qDebug()<<__PRETTY_FUNCTION__ << "start";
    QFont ft;                    // 设置字号
    if (4 == s.length())         // 有千位数时，将字体调小，否则可能显示不下
    {
        ft.setPointSize(70);
    }
    else if (3 == s.length())
    {
        ft.setPointSize(80);
    }
    else if ( 2 == s.length() || 1 == s.length())
    {
        ft.setPointSize(90);
    }
    ui->label_floor->setFont(ft);

    QPalette pe;                // 设置颜色
    pe.setColor(QPalette::WindowText, Qt::white);
    ui->label_floor->setPalette(pe);

    ui->label_floor->setText(s);
    qDebug()<<__PRETTY_FUNCTION__ << "end";
}

// 处理新发过来的数据
void Widget::DealNewData(unsigned char str)
{
    qDebug()<<__PRETTY_FUNCTION__<< "the slots is running !";
    int len;
    if (NULL != pro )
    {
        printf("发过来的数据 str = %X\n", str);
        cout << "调用拷贝字符串数据\n";
        pro->CopyStringFromProtocol(str, sstr);
        cout << "sstr字符串数据\n";
        len = pro->GetDataLength();
        pro->PrintString(sstr, len);
        switch (str) {
        case 0x00:case 0x01:
            SetFloorNumber(GainFloorNumber(sstr));
            ShowArrowStatus(sstr[5]);
            break;
        default:
            break;
        }
        memset(sstr, 0 , len);
    }    
    qDebug()<<__PRETTY_FUNCTION__ << "end";
}

void Widget::ShowArrowStatus(unsigned char str)
{
    switch (str) {
//    case 0x30: ui->graphicsView_Arrow->hide(); break;
//    case 0x31: SetPicture(":/new/prefix1/images/ArrowUp.png", ui->graphicsView_Arrow); break;
//    case 0x32: SetPicture(":/new/prefix1/images/ArrowDn.png", ui->graphicsView_Arrow); break;
//    case 0x33: SetTimerArrowUp(); break;
//    case 0x34: SetTimerArrowDn(); break;
//    case 0x35: ShiningArrowUp(); break;
//    case 0x36: ShiningArrowDn(); break;
    case 0x00: SetTimerArrowUp(); break;
    default:
        break;
    }
}

void Widget::CopyData()
{
    requestBuf[0] = 0x01;
    requestBuf[1] = 0x2A;
    requestBuf[2] = 0x55;
    requestBuf[3] = 0x30;
    requestBuf[4] = 0x30;
    requestBuf[5] = 0x30;
    requestBuf[6] = 0x30;
    qDebug()<<__PRETTY_FUNCTION__;
    if (NULL != pro)
    {
        pro->CopyStringFromUi(requestBuf[0], requestBuf);
    }
}

