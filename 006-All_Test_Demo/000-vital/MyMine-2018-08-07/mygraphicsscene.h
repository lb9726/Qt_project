#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

#include <QGraphicsScene>
#include "mygraphicspixmapitem.h"

class MyGraphicsPixmapItem;

class MyGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MyGraphicsScene(QGraphicsScene *parent = 0);
    ~MyGraphicsScene();
    int getGameOverFlag() const;
    void setGameOverFlag(int gameOverFlag);
    void setMineScene(int pRows, int pColumns, int pMineCount);
    int getMineCounts() const;
    void initMineScenery();

    void layMines();            // 布雷
    void setItemMineCounts();   // 为每一个图元设置雷的数目，根据周围8个的雷的情况来设置的
    void spreadAlgorithm(int x, int y);     // 扩散算法
    void printMinePos();
    void prinMineNumber();
signals:

public slots:

private:
    int mRows;
    int mColumns;
    int mMineCounts;
    int mGameOver;
    MyGraphicsPixmapItem **mItemsArray;
    int **mMineCountRecordArray;
};

#endif // MYGRAPHICSSCENE_H
