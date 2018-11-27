#ifndef MYSCENCE_H
#define MYSCENCE_H

#include <QObject>
#include <QGraphicsScene>
#include "myitem.h"
#include <QTimer>

class myScence : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit myScence(QObject *parent = 0);
    ~myScence();

    void welcome();
    //获取新建雷区的长和宽  雷数
    void set_mine(int a,int b, int c){m = a; n = b;  number = c;}

    void new_minegame();
    //新建
    void newmine();    //雷区
    void decoratemine();  //布雷
    void spreadmine();   //计算周围雷数

    //扩散
    void clearablock(int i,int j);
    //获得被标记的数量    数量没有上限
    int getflag_num();
    //游戏结束
    void show_mine();   //显示雷的位置并设置图元按钮不可点击
    void setend(int a){end = a;}
    int getend(){return end;}   //单次游戏结束标识

    void promptmine();  //提示

private:
    int m,n,number;          //雷区的长和宽  雷数
   myItem *item[50][50];  //雷区的最大范围
   int mine[52][52];
   QTimer *ptimer;      //
   int end;   //单次游戏结束标识

   myItem *m_ptem;
private slots:
   void updatemine();     //扩散条件的判定
   void win_or_lose();      //输赢条件的判定


signals:

public slots:
};

#endif // MYSCENCE_H
