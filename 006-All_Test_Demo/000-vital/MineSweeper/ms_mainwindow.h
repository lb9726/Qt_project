#ifndef MS_MAINWINDOW_H
#define MS_MAINWINDOW_H

#include "myscene.h"
#include "herodialog.h"
#include "setdialog.h"
#include "playerdialog.h"
#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QActionGroup>
#include <QLCDNumber>
#include <QLabel>
#include <QString>
#include <QIcon>
#include <QPixmap>
#include <QPushButton>
#include <QWidget>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QTime>
#include <time.h>
#include <QSettings>
#include <QMessageBox>

#include <QDebug>

class MS_MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MS_MainWindow(QWidget *parent = 0);
    ~MS_MainWindow();

    QMenu *m_gameMenu;     //游戏菜单
    QMenu *m_helpMenu;     //帮助菜单
    QAction *m_newGameAct;    //开局
    QAction *m_lowLevelAct;   //初级
    QAction *m_middleLevelAct;//中级
    QAction *m_highLevelAct;  //高级
    QAction *m_gameSetAct;    //自定义
    QAction *m_soundAct;      //声音
    QAction *m_heroRollAct;   //扫雷英雄榜
    QAction *m_exitAct;       //退出
    QAction *m_aboutAct;      //关于
    QLabel *m_mineLab;        //雷
    QLabel *m_timeLab;        //时间
    QLCDNumber *m_mineLCD;    //显示雷数
    QLCDNumber *m_timeLCD;    //显示时间
    QPushButton *m_smileBtn;  //笑脸按钮
    myScene *m_scene;         //舞台
    QGraphicsView *m_view;    //镜头
    QHBoxLayout *hboxlayout;  //水平boxlayout
    QVBoxLayout *vboxlayout;  //竖直boxlayout
    QWidget *m_widget;        //中心窗体
    QMessageBox *m_messageDialog;//消息对话框
    setDialog *m_setDialog;      //游戏自定义对话框
    heroDialog *m_heroDialog;    //扫雷英雄榜对话框
    playerDialog *m_playerDialog; //刷新记录后对话框
    QSound *sound_1;              //声音：开始游戏
    QSound *sound_2;             //声音：鼠标左键单击
    int m_mineNum;              //记录舞台上的雷数
    int m_time;                 //记录舞台上的时间
    int m_value[3];     //自定义游戏对应的值（行、列、雷）
    bool m_isCustom;    //是否自定义
    bool m_isgameBegin; //游戏是否开始
    bool m_isgameEnd;		//游戏是否结束
    bool m_isUpdate;    //是否有刷新纪录
    bool m_isrecountHeroRollScore;//是否重新计分
    bool m_soundOpen;       //开启声音的开关true: 打开声音 false: 关闭声音
    QTimer *m_timer;            //计时的定时器
    gameRecodeInfo m_gameRecode[3];//存放游戏记录信息
    gameLevel   m_gameLevel;    //当前游戏的级别
    int     m_playerTime;   //刷新游戏记录的时间

    virtual void closeEvent(QCloseEvent *event);//关闭事件
    void readSettings();   //读注册表
    void writeSettings();  //写注册表
    void createMenu();     //创建菜单
    void createGame();     //创建游戏

public slots:
    void newGame();           //新游戏
    void beginGame();           //开始游戏
    void endGame(int);           //游戏结束
    void lowLevelGame();      //初级游戏
    void middleLevelGame();   //中级游戏
    void highLevelGame();     //高级游戏
    void gameSet(); //自定义游戏
    void setValue(int t_rowNum,int t_colNum,int t_mineNum);//设置自定义的int t_rowNum,int t_colNum,int t_mineNum
    void displayHeroRoll();//显示扫雷英雄榜
    void soundAction();       //是否开启声音
    void displayTime();//显示时间
    void displayMineNum(int t_signedMineNum); //显示雷数
    void recountHeroRollScore();//重新计算英雄榜的分数
    void recvPlayerName(QString t_playerName);//接收刷新记录的玩家名
    void about();

};

#endif // MS_MAINWINDOW_H
