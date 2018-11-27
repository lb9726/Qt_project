#include "myscene.h"

/***********************************************************************
函数名字：myScene(QObject *parent)
函数功能：myScene构造函数
函数流程：
    初始化成员
************************************************************************/
myScene::myScene(QObject *parent) :
    QGraphicsScene(parent)
{

    t_vec = new QVector<myItem *>;
    m_isGameOver = false;	//游戏是否结束
    m_remainNoMines = 0; 	//记录舞台上剩余非雷的个数
    m_soundOpen = false;    //开启声音的开关true: 打开声音 false: 关闭声音
    sound_1 = new QSound(":/sounds/bong.wav");//雷
    sound_2 = new QSound(":/sounds/critical.wav");//扩散
}

int myScene::m_rowNum = 0;
int myScene::m_colNum = 0;
int myScene::m_mineNum = 0;

/***********************************************************************
函数名字：countAroundMines(myItem *t_item)
函数功能：给为雷的周围八块的雷数加1
函数流程：
    判断雷周围的块是否存在
    存在则雷数加1
************************************************************************/
void myScene::countAroundMines(myItem *t_item) //给为雷的周围八块的雷数加1
{
    myItem *temp_item = NULL;
    for(int i = 0; i < 8; i++)
    {
        if((t_item->m_X + t_around[i].x()) < 0 || (t_item->m_X + t_around[i].x()) >= m_rowNum\
                || (t_item->m_Y + t_around[i].y()) < 0 ||  (t_item->m_Y + t_around[i].y()) >= m_colNum)
            continue;
        temp_item = t_vec->at((t_item->m_X + t_around[i].x())*m_colNum + t_item->m_Y + t_around[i].y());
        temp_item->m_aroundMineNum++;
    }
}


/***********************************************************************
函数名字：openAllItems()
函数功能：打开所有的方块
函数流程：
    打开所有的方块
************************************************************************/
void myScene::openAllItems()    //打开所有的方块
{
    m_isGameOver = true;
    myItem *temp_item = NULL;
    for(int i = 0; i < m_rowNum; i++)
    {
        for(int j = 0; j < m_colNum; j++)
        {
            temp_item = t_vec->at(i*m_colNum + j);
            temp_item->m_isOpened = true;
            if(temp_item->m_isMine)
            {
                temp_item->setPixmap(QPixmap(":/images/end.png"));
                if(this->m_soundOpen)
                    this->sound_1->play();
            }
            else
                switch (temp_item->m_aroundMineNum)
                {
                    case 0: temp_item->setPixmap(QPixmap(":/images/n0.png"));
                            break;
                    case 1: temp_item->setPixmap(QPixmap(":/images/n1.png"));
                            break;
                    case 2: temp_item->setPixmap(QPixmap(":/images/n2.png"));
                            break;
                    case 3: temp_item->setPixmap(QPixmap(":/images/n3.png"));
                            break;
                    case 4: temp_item->setPixmap(QPixmap(":/images/n4.png"));
                            break;
                    case 5: temp_item->setPixmap(QPixmap(":/images/n5.png"));
                            break;
                    case 6: temp_item->setPixmap(QPixmap(":/images/n6.png"));
                            break;
                    case 7: temp_item->setPixmap(QPixmap(":/images/n7.png"));
                            break;
                    case 8: temp_item->setPixmap(QPixmap(":/images/n8.png"));
                            break;
                    default:
                            break;
                }
        }
    }
}

/***********************************************************************
函数名字：expendWater(myItem *t_item)
函数功能：扩散函数
函数流程：
    递归扩散空白区域
************************************************************************/
void myScene::expendWater(myItem *t_item)   //扩散函数
{
    t_item->m_isOpened = true;
    if(this->m_soundOpen)
        sound_2->play();
    t_item->setPixmap(QPixmap(":/images/n0.png"));

    myItem *temp_item = NULL;
    for(int i = 0; i < 8; i++)
    {
        if((t_item->m_X + t_around[i].x()) < 0 || (t_item->m_X + t_around[i].x()) >= m_rowNum\
                || (t_item->m_Y + t_around[i].y()) < 0 ||  (t_item->m_Y + t_around[i].y()) >= m_colNum)
            continue;

        temp_item = t_vec->at((t_item->m_X + t_around[i].x())*m_colNum + t_item->m_Y + t_around[i].y());

        if(temp_item->m_isMine == true || temp_item->m_isOpened == true)
            continue;
        this->m_remainNoMines++;
        temp_item->m_isOpened = true;
        switch (temp_item->m_aroundMineNum)
        {
            case 0: temp_item->setPixmap(QPixmap(":/images/n0.png"));
                    expendWater(temp_item);
                    break;
            case 1: temp_item->setPixmap(QPixmap(":/images/n1.png"));
                    break;
            case 2: temp_item->setPixmap(QPixmap(":/images/n2.png"));
                    break;
            case 3: temp_item->setPixmap(QPixmap(":/images/n3.png"));
                    break;
            case 4: temp_item->setPixmap(QPixmap(":/images/n4.png"));
                    break;
            case 5: temp_item->setPixmap(QPixmap(":/images/n5.png"));
                    break;
            case 6: temp_item->setPixmap(QPixmap(":/images/n6.png"));
                    break;
            case 7: temp_item->setPixmap(QPixmap(":/images/n7.png"));
                    break;
            case 8: temp_item->setPixmap(QPixmap(":/images/n8.png"));
                    break;
            default:
                break;
        }
    }
}

/***********************************************************************
函数名字：openAroundItems(myItem *t_item)
函数功能：翻开周围8点的方块
函数流程：
    翻开周围8点的方块
************************************************************************/
void myScene::openAroundItems(myItem *t_item)//翻开周围8点的方块
{
    if(this->m_soundOpen)
        sound_2->play();
    myItem *temp_item = NULL;

    if(t_item->m_isOpened && t_item->m_aroundMineNum == countFlagNum(t_item))
        for(int i = 0; i < 8; i++)
        {
            if((t_item->m_X + t_around[i].x()) < 0 || (t_item->m_X + t_around[i].x()) >= m_rowNum\
                    || (t_item->m_Y + t_around[i].y()) < 0 ||  (t_item->m_Y + t_around[i].y()) >= m_colNum)
                continue;

            temp_item = t_vec->at((t_item->m_X + t_around[i].x())*m_colNum + t_item->m_Y + t_around[i].y());

            if(temp_item->m_isOpened == true || temp_item->m_rMouseKeyNum == 1)
                continue;

            this->m_remainNoMines++;
            temp_item->m_isOpened = true;
            if(temp_item->m_isMine)
            {
                temp_item->setPixmap(QPixmap(":/images/end.png"));
                m_isGameOver = true;
            }

            switch (temp_item->m_aroundMineNum)
            {
                case 0: temp_item->setPixmap(QPixmap(":/images/n0.png"));
                        break;
                case 1: temp_item->setPixmap(QPixmap(":/images/n1.png"));
                        break;
                case 2: temp_item->setPixmap(QPixmap(":/images/n2.png"));
                        break;
                case 3: temp_item->setPixmap(QPixmap(":/images/n3.png"));
                        break;
                case 4: temp_item->setPixmap(QPixmap(":/images/n4.png"));
                        break;
                case 5: temp_item->setPixmap(QPixmap(":/images/n5.png"));
                        break;
                case 6: temp_item->setPixmap(QPixmap(":/images/n6.png"));
                        break;
                case 7: temp_item->setPixmap(QPixmap(":/images/n7.png"));
                        break;
                case 8: temp_item->setPixmap(QPixmap(":/images/n8.png"));
                        break;
                default:
                    break;
            }
        }
}

/***********************************************************************
函数名字：countFlagNum(myItem *t_item)
函数功能：统计周围八点插旗的数量
函数流程：
    统计周围八点插旗的数量
    返回旗的数量
************************************************************************/
int myScene::countFlagNum(myItem *t_item)//统计周围八点插旗的数量
{
    int m_aroundFlagNum = 0;
    for(int i = 0; i < 8; i++)
    {
        if((t_item->m_X + t_around[i].x()) < 0 || (t_item->m_X + t_around[i].x()) >= m_rowNum\
                || (t_item->m_Y + t_around[i].y()) < 0 ||  (t_item->m_Y + t_around[i].y()) >= m_colNum)
            continue;
        if(t_vec->at((t_item->m_X + t_around[i].x())*m_colNum + t_item->m_Y + t_around[i].y())->m_rMouseKeyNum == 1)
                m_aroundFlagNum++;
    }
    return m_aroundFlagNum;
}
