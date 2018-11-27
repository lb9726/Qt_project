#include "myitem.h"

/***********************************************************************
函数名字：myItem(int t_x,int t_y,const QPixmap & pixmap,QObject *parent)
函数功能：myItem构造函数
函数流程：
    初始化成员
************************************************************************/
myItem::myItem(int t_x,int t_y,const QPixmap & pixmap,QObject *parent) : //构造函数
    QObject(parent)
{
    this->m_X = t_x;
    this->m_Y = t_y;
    this->setPixmap(pixmap);
    this->m_isMine = false;
    this->m_isOpened = false;
    this->m_aroundMineNum = 0;
    this->m_rMouseKeyNum = 0;
}

/***********************************************************************
函数名字：mousePressEvent(QGraphicsSceneMouseEvent * event)
函数功能：鼠标按下
函数流程：
    判断item是否打开
    是左键按下发送信号
    设置图片
************************************************************************/
void myItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    if(this->m_isOpened)
        return;
    if(event->button() == Qt::LeftButton)
        emit signal_endGame(3);
    this->setPixmap(QPixmap(":/images/enter_in.png"));
}

/***********************************************************************
函数名字：mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
函数功能：鼠标放开
函数流程：
    判断鼠标按键
    进行对应的动作
************************************************************************/
void myItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(this->m_isOpened || this->m_rMouseKeyNum == 1)
            return;
        emit signal_beginGame();
        if(this->m_isMine)
        {
            emit signal_openAllItems();
            emit signal_endGame(0);
            this->setPixmap(QPixmap(":/images/end.png"));
        }
        else
        {
            switch (this->m_aroundMineNum)
            {
            case 0: this->setPixmap(QPixmap(":/images/n0.png"));
                emit signal_expendWater(this);
                emit signal_endGame(4);
                break;
            case 1: this->setPixmap(QPixmap(":/images/n1.png"));
                emit signal_endGame(1);
                break;
            case 2: this->setPixmap(QPixmap(":/images/n2.png"));
                emit signal_endGame(1);
                break;
            case 3: this->setPixmap(QPixmap(":/images/n3.png"));
                emit signal_endGame(1);
                break;
            case 4: this->setPixmap(QPixmap(":/images/n4.png"));
                emit signal_endGame(1);
                break;
            case 5: this->setPixmap(QPixmap(":/images/n5.png"));
                emit signal_endGame(1);
                break;
            case 6: this->setPixmap(QPixmap(":/images/n6.png"));
                emit signal_endGame(1);
                break;
            case 7: this->setPixmap(QPixmap(":/images/n7.png"));
                emit signal_endGame(1);
                break;
            case 8: this->setPixmap(QPixmap(":/images/n8.png"));
                emit signal_endGame(1);
                break;
            default:
                break;
            }
        }
        this->m_isOpened = true;
    }
    else if(event->button() == Qt::RightButton)
    {
        if(this->m_isOpened)
            return;
        this->m_rMouseKeyNum++;
        switch (this->m_rMouseKeyNum)
        {
            case 1: this->setPixmap(QPixmap(":/images/flag_h.png"));
                    emit signal_setMineNum(1);
                    break;
            case 2: this->setPixmap(QPixmap(":/images/ques.png"));
                    emit signal_setMineNum(-1);
                    break;
            case 3: this->setPixmap(QPixmap(":/images/enter.png"));
                    this->m_rMouseKeyNum = 0;
                    break;
            default:
                break;
        }
    }
}

/***********************************************************************
函数名字：mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
函数功能：鼠标双击
函数流程：
    发送信号（扩散和游戏处理）
************************************************************************/
void myItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
    emit signal_openAroundItems(this);
    emit signal_endGame(2);
}
