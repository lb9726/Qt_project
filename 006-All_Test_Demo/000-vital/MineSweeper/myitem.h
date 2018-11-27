#ifndef MYITEM_H
#define MYITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QPixmap>
#include <QGraphicsSceneMouseEvent>
#include <QSound>

#include <QDebug>

class myItem : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
//    myItem();
    myItem(int t_x,int t_y,const QPixmap & pixmap,QObject *parent = 0);//构造函数

    int  m_X;             //X坐标,行数
    int  m_Y;             //Y坐标,列数
    bool m_isMine;        //该方块是否为雷
    bool m_isOpened;      //该方块是否打开
    int  m_aroundMineNum; //该方块上的数字
    int  m_rMouseKeyNum;  //该方块鼠标右键点击的次数

    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);   

signals:
    void signal_openAllItems();//打开所有方块（信号）
    void signal_expendWater(myItem *t_item);//点中空白位置自动扩散（信号）
    void signal_openAroundItems(myItem *t_item);//打开周围八块（信号）
    void signal_beginGame();   //开始游戏（信号）
    void signal_endGame(int);  //处理游戏（信号）
    void signal_setMineNum(int t_signedMineNum);//设置标记（信号）
};

#endif // MYITEM_H
