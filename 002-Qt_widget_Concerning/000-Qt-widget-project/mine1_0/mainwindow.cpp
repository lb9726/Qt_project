#include "mainwindow.h"
#include "ui_mainwindow.h"


int m,n,number;  //雷区的长和宽  雷数
int flag_num;       //被标记的数量
//extern bool button_flag;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timeflag = 0;
    gameflag = 0;
    ui->pushButton->setIconSize(QSize(30,30));
    ui->pushButton->setFixedSize(40,40);
    ui->pushButton->setIcon(QIcon(tr(":/kai.png")));

    m = n =9,number =10;
    sc = new myScence();
    sc->welcome();
    sc->setend(0);
    ui->graphicsView->setScene(sc);
    
    qtimer = new QTimer;
    connect(qtimer,SIGNAL(timeout()),this,SLOT(uptime()));
    connect(qtimer,SIGNAL(timeout()),this,SLOT(gamevoer()));
    qtimer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}
//====================================================
void MainWindow::uptime()
{
    int n = 0;
    if(gameflag == 1)
    {
    QTime ctime(0,0,0,0);
    QTime stime;
    stime = ctime.addSecs(timeflag);
    QString sctime = stime.toString("mm:ss");
    ui->ltime->setText(sctime);
    timeflag++;

    //获得被标记的数量     设置右键1状态的上限
    n = number - sc->getflag_num();
    QString str = QString::number(n);
    ui->lminenum->setText(str);
    }
}
//游戏结束
void MainWindow::gamevoer()
{
    if(sc->getend() == 1)   //游戏失败
    {
        ui->pushButton->setIcon(QIcon(tr(":/shibai.png")));
        qtimer->stop();
        QMessageBox::information(this,"确认失败","排雷失败,你输了","ok");
        return;
    }
    else if(sc->getend() >1)   //游戏胜利
    {
        ui->pushButton->setIcon(QIcon(tr(":/haha.png")));
        qtimer->stop();
        QString winstr = QString::number(m)+"*"+QString::number(n);
        QString wintime = ui->ltime->text();
        newwiner  w;
        w.setwintext(wintime,winstr,number);
        w.exec();
    }
}

void MainWindow::newgame()   //新游戏
{
    timeflag = 1;     //初始化计时
    gameflag = 1;   //新游戏 标识
   // button_flag = false;
    ui->pushButton->setIcon(QIcon(tr(":/kai.png")));
    //注意内存泄露
    qtimer->start(1000);
    sc = new myScence();
    sc->set_mine(m,n,number);
    ui->graphicsView->setScene(sc);
    sc->new_minegame();
}
//=====================================================
void MainWindow::on_pushButton_clicked()     //笑脸 按钮
{
    m = n =9,number =10;
    newgame();   //笑脸 默认新建9X9 10个雷
}

void MainWindow::on_action_triggered()   //新游戏    菜单按钮
{
    newgame();  //默认新建规格和上次相同的
}
//退出游戏
//========================================================
void MainWindow::on_action_2_triggered()  //退出游戏
{
    int ret =
    QMessageBox::information(this,"确认退出","您将退出本游戏。确认要继续吗？",QMessageBox::Yes,QMessageBox::No);
    if(ret == QMessageBox::Yes)
        this->close();
    else if(ret == QMessageBox::No){}
    return;
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    int ret =
    QMessageBox::information(this,"确认退出","您将退出本游戏。确认要继续吗？",QMessageBox::Yes,QMessageBox::No);
    if(ret == QMessageBox::Yes)
    {
        this->close();
    }else{
        event->ignore();
    }
}
//难度
//=============================================================
void MainWindow::on_action9_9_10_triggered()   //9*9 10个雷   菜单按钮
{
   m = n =9,number =10;   
   newgame();    //新建9X9 10个雷
}

void MainWindow::on_action16_16_40_triggered()  //16*16 40个雷     菜单按钮
{
    m = n =16,number =40;
    newgame();   //新建16X16 40个雷
}

void MainWindow::on_action_3_triggered()       //自定义     菜单按钮
{
    mygame mynewgame;
    mynewgame.exec();
    m = mynewgame.getlength();
    n = mynewgame.getwidth();
    number = mynewgame.getmine();
    if(m>50){ m = 50;}
    if(m<9){ m =9; }
    if(n>50){ n = 50;}
    if(n<9){ n =9; }
    if(number < 10){ number = 10;}
    if(number > (m*n-10)){ number = (m*n -10);}
    newgame();
}
//=================================================================
//
void MainWindow::on_action_4_triggered()    //游戏规则  菜单按钮
{
    introduce in;
    in.setWindowTitle("游戏规则及其介绍");
    in.exec();
}

void MainWindow::on_action_5_triggered()    //关于开发者 菜单按钮
{

}
//==========================================
void MainWindow::on_action_6_triggered()     //胜利者留爪   菜单按钮
{
    query q;
    q.exec();
}
//==================================================
void MainWindow::on_action_7_triggered()    //提示
{
    sc->promptmine();
    timeflag += 15;
}
