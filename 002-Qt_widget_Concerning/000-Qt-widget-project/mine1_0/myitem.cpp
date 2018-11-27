#include "myitem.h"
extern int flag_num;
extern int number;


myItem::myItem()//QObject *parent) : QGraphicsPixmapItem(parent)
{
    m_flag = 0;  //标识是否为雷
    button_num = 0;  //标识按钮状态 0未按  1按下右键  2 按下左键
    mine_num = 0;   //标识周围雷数
    setimage(":/blank1.png");
}

//设置图片
void myItem::setimage(QString szPath)
{
    setPixmap(QPixmap(szPath));
    return ;
}
//被动显示雷数
void myItem::showmine()
{
    if(button_num == 2)
    {
        QString szPath =QString(":/%1.png").arg(mine_num);
        setimage(szPath);
    }
}

//鼠标点击事件
void myItem::mousePressEvent(QGraphicsSceneMouseEvent *event )
{
    if(event->button() == Qt::RightButton)
    {
        if(0 == button_num && flag_num < number)
        {
            //右键点击一次
            setimage(":/flag.png");
            button_num = 1;
            return;
        }
        if(1 == button_num)
        {
            //右键点击两次
            setimage(":/0.png");
            button_num = 0;
            return;
        }
    }

    if(event->button() == Qt::LeftButton)
    {
        if(m_flag == 1 && button_num == 0)
        {
              setimage(":/foundMine.png");  //点击的是雷
        }
        else if(m_flag == 0&&button_num == 0)
        {
            QString szPath =QString(":/%1.png").arg(mine_num);
            setimage(szPath); //不是雷 显示数字
        }

        button_num = 2;//鼠标点击事件设置
        return;
    }
}
//显示雷的位置并设置图元按钮不可点击
void myItem::endshow()
{
    //该图元按钮是雷 且没标记
    if(m_flag == 1 && button_num == 0)
    {
        setimage(":/mine.png");   //显示爆炸的雷
    }
     //该图元按钮是雷 且标记
    if(m_flag == 1 && button_num == 1)
    {
        setimage(":/mine.png");    //显示雷
    }
    button_num = 2;   //设置不可点击
}

