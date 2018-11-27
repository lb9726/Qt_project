#ifndef MYSCENE_H
#define MYSCENE_H

#include "myitem.h"
#include "common.h"
#include <QGraphicsScene>
#include <QVector>

class myScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit myScene(QObject *parent = 0);

    QVector<myItem *> *t_vec;
    static int m_rowNum;    //方块的行数
    static int m_colNum;    //方块的列数
    static int m_mineNum;   //方块的雷数
    bool m_isGameOver;		//游戏是否结束
    int m_remainNoMines; 	//记录舞台上被打开的item个数
    bool m_soundOpen;       //开启声音的开关true: 打开声音 false: 关闭声音
    QSound *sound_1;        //声音：扩散
    QSound *sound_2;        //声音：爆炸
    //原点周围的八点
    QPoint t_around[8] = {
        QPoint(-1,-1),
        QPoint(-1,0),
        QPoint(-1,1),
        QPoint(0,-1),
        QPoint(0,1),
        QPoint(1,-1),
        QPoint(1,0),
        QPoint(1,1)
    };

    void countAroundMines(myItem *t_item); //统计该方块周围的雷数
    int countFlagNum(myItem *t_item); //统计周围八点插旗的数量

public slots:
    void openAllItems();    //打开所有的方块
    void expendWater(myItem *t_item);   //扩散函数
    void openAroundItems(myItem *t_item);//翻开周围8点的方块
};

#endif // MYSCENE_H
