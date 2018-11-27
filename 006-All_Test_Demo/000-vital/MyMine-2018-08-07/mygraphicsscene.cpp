#include "mygraphicsscene.h"
#include <QDebug>
#include <stdlib.h>
#include "logcat.h"

MyGraphicsScene::MyGraphicsScene(QGraphicsScene *parent) : QGraphicsScene(parent)
{
    mGameOver = 0;
    mItemsArray = nullptr;
    mMineCountRecordArray = nullptr;
    mRows = 0;
    mColumns = 0;
}

MyGraphicsScene::~MyGraphicsScene()
{
    if (nullptr != mItemsArray)
    {
        for (int i = 0; i < mColumns; ++i)
        {
            delete[] mItemsArray[i];
            mItemsArray[i] = nullptr;
        }
        delete mItemsArray;
        mItemsArray = nullptr;
    }
    if (nullptr != mMineCountRecordArray)
    {
        for (int i = 0; i < mColumns + 2; ++i)
        {
            delete[] mMineCountRecordArray[i];
            mMineCountRecordArray[i] = nullptr;
        }
        delete mMineCountRecordArray;
        mMineCountRecordArray = nullptr;
    }
}

int MyGraphicsScene::getGameOverFlag() const
{
    return mGameOver;
}

void MyGraphicsScene::setGameOverFlag(int gameOver)
{
    mGameOver = gameOver;
}

void MyGraphicsScene::setMineScene(int pRows, int pColumns, int pMineCount)
{
    mRows = pRows;
    mColumns = pColumns;
    mMineCounts = pMineCount;
}

int MyGraphicsScene::getMineCounts() const
{
    return mMineCounts;
}

/*调用前需要先设置行列和雷数 setMineScene(int pRows, int pColumns, int pMineCount)*/
void MyGraphicsScene::initMineScenery()
{
    if (mRows > 0 && mColumns > 0)
    {
        mItemsArray = new MyGraphicsPixmapItem *[mRows];            // 生成行指针
        for (int i = 0; i < mRows; ++i)
        {
            mItemsArray[i] = new MyGraphicsPixmapItem[mColumns];    // 为每一个行指针分配空间
        }

        mMineCountRecordArray = new int *[mRows + 2];
        for (int j = 0; j < mRows + 2; ++j)
        {
            mMineCountRecordArray[j] = new int[mColumns + 2];
        }

        for (int i = 0; i < mRows + 2; ++i)
        {
            for (int j = 0; j < mColumns + 2; ++j)
            {
                mMineCountRecordArray[i][j] = 0;
//                qDebug("mMineCountRecordArray[%d][%d] = %d", i, j, mMineCountRecordArray[i][j]);
//                if (j == mColumns + 1)
//                {
//                    printf("\n");
//                }
                if (i < mRows && j < mColumns)
                {
//                    qDebug("mMineCountRecordArray[%d][%d] = %d", i, j, mMineCountRecordArray[i][j]);
                    mItemsArray[i][j].setPos(j*32, i*32);  // 注意需要横排放置每一个图元
                    // 根据图片的像素来的，图片是30×30的，稍微大一点即可
                    this->addItem(&mItemsArray[i][j]);
                    mItemsArray[i][j].setRowAndColPos(i, j);
                    connect(&mItemsArray[i][j], &MyGraphicsPixmapItem::sigLeftBtnClicked,
                            this, &MyGraphicsScene::spreadAlgorithm);
                }
            }
        }
    }
}

// 布雷
void MyGraphicsScene::layMines()
{
    int i = 0;
    int x;
    int y;
    qsrand((unsigned)time(NULL));
    do
    {
        x = qrand() % mRows;
        y = qrand() % mColumns;
//        MYDebug("x = %d, y = %d", x, y);
        // 只有产生的随机数x，y是没有被设置成雷的图元，才能生效
        if (0 == mItemsArray[x][y].getIsMineflag())
        {
            i++;
            mItemsArray[x][y].setIsMineflag(1);
            mMineCountRecordArray[x+1][y+1] = 1;
            // 计数的二维数组比真实的图元多了一行一列，所以对应的index值应该行列各加上1
        }
    }while (i < mMineCounts);
}

/*
如果是雷时，在计算雷数的时候可以被置为相应的雷数，
只是在显示时优先判断是否为雷的标志位，是雷则显示雷的图标
*/
void MyGraphicsScene::setItemMineCounts()
{
    int num = 0; // 设置存储雷数空间
    for (int i = 1; i < mRows + 1; ++i)
    {
        for (int j = 1; j < mColumns + 1; ++j)
        {
//            MYDebug("i-1 = %d, i = %d, i+1 = %d, j-1 = %d, j = %d, j+1 = %d", i-1, i, i+1, j-1, j, j+1);
            num = mMineCountRecordArray[i-1][j-1] + mMineCountRecordArray[i-1][j] + mMineCountRecordArray[i-1][j+1]
                    + mMineCountRecordArray[i][j-1] + mMineCountRecordArray[i][j+1]
                    + mMineCountRecordArray[i+1][j-1] + mMineCountRecordArray[i+1][j] + mMineCountRecordArray[i+1][j+1];
            mItemsArray[i-1][j-1].setMinesNumber(num);
        }
    }
}

/*
    扩算算法 Algorithm 英 [ˈælgərɪðəm]
*/
void MyGraphicsScene::spreadAlgorithm(int x, int y)
{
    for (int i = -1; i < 2; ++i)
    {
        for (int j = -1; j < 2; ++j)
        {
            /*排除越界问题*/
            if (x+i >= mRows || x+i < 0
                || y+j >= mColumns || y+j < 0 || (0 == i && 0 == j))
            {
                continue;
            }
            /* 已经点开过的，不能再点开，否则会进入死循环，这些情况都要排除掉 */
            if (1 == mItemsArray[x+i][y+j].getIsMineflag()
                || 1 == mItemsArray[x+i][y+j].getAlreadyOpenFlag())
            {
                continue;
            }
            mItemsArray[x+i][y+j].setAlreadyOpenFlag(1);
            if (0 == mItemsArray[x+i][y+j].getMinesNumber()) // 空白
            {
                mItemsArray[x+i][y+j].showMine();
                spreadAlgorithm(x+i, y+j);
//                MYDebug("x = %d, y = %d, x+i = %d, y+j = %d", x, y, x+i, y+j);
            }
            else
            {
                if (0 == mItemsArray[x+i][y+j].getIsMineflag()) // 不是雷的显示出来
                {
                    mItemsArray[x+i][y+j].showMine();
                }
            }
        }
    }
}

void MyGraphicsScene::printMinePos()
{
    for (int i = 0; i < mRows; ++i)
    {
        for (int j = 0; j < mColumns; ++j)
        {
            printf("%d ", mItemsArray[i][j].getIsMineflag());
            fflush(stdout);
            if (mColumns - 1 == j)
            {
                printf("\n");
            }
        }
    }
    printf("\n");
    fflush(stdout);
}

void MyGraphicsScene::prinMineNumber()
{
    for (int i = 0; i < mRows; ++i)
    {
        for (int j = 0; j < mColumns; ++j)
        {
            printf("%d ", mItemsArray[i][j].getMinesNumber());
            fflush(stdout);
            if (mColumns - 1 == j)
            {
                printf("\n");
            }
        }
    }
    printf("\n");
    fflush(stdout);
}










































