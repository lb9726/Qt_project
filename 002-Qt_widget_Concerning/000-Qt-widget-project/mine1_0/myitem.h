#ifndef MYITEM_H
#define MYITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QString>
#include <QMessageBox>

class myItem : public QGraphicsPixmapItem
{
   // Q_OBJECT
    int m_flag;  //标识是否为雷
    int button_num;  //标识按钮状态 0未按  1按下右键  2 按下左键
    int mine_num;   //标识周围雷数
public:
 //   explicit myItem(QObject *parent = 0);
    myItem();
    void setmine_num(int num){mine_num = num;}
    int getmine_num(){return mine_num;}
    void setbutton_num(int i){button_num = i;}
    int getbutton_num(){return button_num;}
    void setm_flag(int flag){m_flag = flag;}
    int getm_flag(){return m_flag;}

    void setimage(QString); //设置图片
    void showmine();  //被动显示雷数
    void endshow();    //显示雷的位置并设置图元按钮不可点击

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event ); //鼠标点击事件
signals:

public slots:
};

#endif // MYITEM_H
