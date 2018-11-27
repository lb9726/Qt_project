#include "myscence.h"
#include <time.h>
extern int flag_num;
myScence::myScence(QObject *parent) : QGraphicsScene(parent)
{
    //新建
    end = 0;
}

myScence::~myScence()
{
    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j< n; j++)
        {
            delete item[i][j];
        }
    }
}
//
void myScence::new_minegame()
{
    newmine();
    decoratemine();
    spreadmine();

    ptimer = new QTimer();
    connect(ptimer,SIGNAL(timeout()),this,SLOT(updatemine()));
    connect(ptimer,SIGNAL(timeout()),this,SLOT(win_or_lose()));
    ptimer->start(100);
}
//==============================================
void myScence::newmine()        //雷区
{
    int i,j;                    //int m,n,number; //雷区的长和宽  雷数
    for(i = 0; i < m; i++)     //显示雷区大小
    {
        for(j = 0; j < n; j++)
        {
            item[i][j] = new myItem();
            item[i][j]->setPos(i*35,j*35);
            this->addItem(item[i][j]);
        }
    }
    for(int i=0;i<m+2;i++)   //设置记录雷的数组
    {
        for(int j=0;j<n+2;j++)
        {
            mine[i][j] = 0;
        }
    }
}

void myScence::decoratemine()//布雷
{
    int i = 0;
    int x,y;
    qsrand((unsigned)time(NULL));
    do
    {
        x = qrand()%m;
        y = qrand()%n;
        if(item[x][y]->getm_flag() == 1 || item[x][y]->getbutton_num() == 2)
            i--;
        else{
        item[x][y]->setm_flag(1);
        mine[x+1][y+1] = 1;
        }
        i++;
    }while(i<number);
}

void myScence::spreadmine()//计算周围雷数
{
   int num = 0;  //设置存储雷数空间
   for(int i = 1; i<m+1;i++)
   {
       for(int j = 1; j<n+1; j++)
       {
          num =mine[i][j-1]+mine[i][j+1]+
                               mine[i-1][j-1]+mine[i-1][j]+mine[i-1][j+1]+
                               mine[i+1][j-1]+mine[i+1][j]+mine[i+1][j+1];
           item[i-1][j-1]->setmine_num(num); //将计算得到的雷数传给对应的图元按钮
       }
   }
}
//=========================================
void myScence::updatemine()//扩散条件的判定
{
    for(int i = 0;i<m;i++)
    {
        for(int j=0; j<n; j++)
        {
            //该图元按钮已经被按下且本身不是雷，周围没有雷
            if(item[i][j]->getbutton_num() == 2 && item[i][j]->getm_flag() == 0 && item[i][j]->getmine_num() == 0)
                clearablock(i,j);
        }
    }
}

void myScence::clearablock(int i, int j)
{
    int k,p;
    for(k = i-1; k<=i+1;k++)
    {
        for(p = j-1; p <= j+1;p++)
        {
            if(k>=0&&k<m&&p>=0&&p<n)
            {
                if(item[k][p]->getbutton_num() != 1)   //该按钮图元没有被标识
                {
                    item[k][p]->setbutton_num(2); //标识该图元按钮已经被按下
                    item[k][p]->showmine(); //被动显示该图元按钮周围的雷数
                }
            }
        }
    }
}

//=======================================================
void myScence::win_or_lose()     //输赢条件的判定
{
    int i,j;
    int num = 0;   //正确点击不是雷的数量
    int k = 0;    //被正确标记是雷的数量
    for(i = 0;i<m;i++)
    {
        for(j = 0; j< n;j++)
        {
            //被点击的图元按钮是雷           游戏失败结束
            if(item[i][j]->getbutton_num() == 2 && item[i][j]->getm_flag() == 1)
            {
                ptimer->disconnect(this);      //停止槽函数
                end = 1;
                show_mine();      //显示雷的位置并设置图元按钮不可点击
                return;
            }
            //被点击的图元按钮不是雷
            if(item[i][j]->getbutton_num() == 2 && item[i][j]->getm_flag() == 0)
            {
                num++;    //正确点击不是雷的数量 + 1
            }
            //正确点击不是雷的数量 = 雷区不是雷的数量        游戏赢了  结束
            if(num == (m*n - number))
            {
                ptimer->disconnect(this);       //停止槽函数
                end = 2;
                show_mine();    //显示雷的位置并设置图元按钮不可点击
                return;
            }
            //被标记的图元按钮是雷
            if(item[i][j]->getbutton_num() == 1 && item[i][j]->getm_flag() == 1)
            {
                k++;    //被正确标记是雷的数量+1
            }
            //被正确标记是雷的数量 =  雷区雷的总数       游戏赢了  结束
            if(k == 10)
            {
                ptimer->disconnect(this);       //停止槽函数
                end = 3;
                show_mine();        //显示雷的位置并设置图元按钮不可点击
                return;
            }
        } //end for(n)
    }  //end for(m)
}

void myScence::show_mine()  //显示雷的位置并设置图元按钮不可点击
{
    for(int i = 0;i<m;i++)
    {
        for(int j = 0;j<n;j++)
        {
            if(1 == end)   //游戏失败
            {
                item[i][j]->endshow();
            }
            if(2 == end || 3 == end)  //游戏赢了
            {
                if(item[i][j]->getm_flag() == 1)
                {
                    item[i][j]->setimage(":/mine.png");    //显示雷
                }
            }
            item[i][j]->setbutton_num(2);   //设置不可点击
        }
    }
}
//===================================
//获得被标记的数量    数量没有上限
int myScence::getflag_num()
{
    flag_num = 0;
    for(int i=0;i<m; i++)
    {
        for(int j=0;j<n;j++)
        {
            if(item[i][j]->getbutton_num() == 1)
                flag_num++;
        }
    }
    return flag_num;
}
//=================================
void myScence::welcome()
{
    m_ptem = new myItem;
    m_ptem->setimage(":/welcome1.png");
   m_ptem->setPos(m_ptem->boundingRect().width(),m_ptem->boundingRect().height());
    this->addItem(m_ptem);

}
//
void myScence::promptmine()
{
    for(int i = 0; i<m;i++)
    {
        for(int j = 0; j<n;j++)
        {
            if(item[i][j]->getbutton_num() == 0 && item[i][j]->getm_flag() == 1)
            {
                item[i][j]->setbutton_num(1);
                item[i][j]->setimage(":/flag.png");
                return;
            }
        }
    }
}
