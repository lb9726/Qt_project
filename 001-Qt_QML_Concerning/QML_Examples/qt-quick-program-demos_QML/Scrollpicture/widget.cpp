#include "widget.h"

//Widget::Widget(QWidget *parent)
//    : QWidget(parent)
//{
//}

Widget::~Widget()
{

}

Widget::Widget(QStringList picList, QSize picSize, int picCount,
                         int speedTime, int space, QWidget *parent) : QWidget(parent),
    m_picList(picList),m_picSize(picSize),m_picCount(picCount),
    m_speedTime(speedTime),m_space(space)
{
    this->m_itemSize = QSize(this->m_space + this->m_picSize.width(),this->m_picSize.height());
    this->m_group = new QParallelAnimationGroup;
    this->m_itemCount = this->m_picCount;
    this->m_picWindowCount = 0;
    this->initItem();
    this->initCoord();
    this->initStates();
    this->initAnimationGroup();
}
void Widget::initItem()
{
    this->m_itemList.clear();
    for(int i = 0;i < m_picCount; i++)
    {
        this->m_item = new QLabel(this);
        this->m_item->setPixmap(this->m_picList.at(i));
        this->m_item->resize(m_picSize);
        this->m_itemList.append(m_item);
    }
}

void Widget::initCoord()
{
    QPoint startPoint(0 - this->m_itemSize.width(),0);
    for(int i = 0; i < this->m_picCount; i++)
    {
        this->m_picPosList.append(QPoint(startPoint.x() + i * this->m_itemSize.width(),startPoint.y()));
    }
}

void Widget::initStates()
{
    this->m_state = 0;
    for(int i = 0; i < this->m_picCount; i++)
    {
        this->m_state = new QState();
        this->m_stateList.append(this->m_state);
    }
}

void Widget::initAnimationGroup()
{
    for ( int i = 0; i < this->m_itemList.size(); ++i ) {
        this->m_anim = new QPropertyAnimation(this->m_itemList.at(i), "pos",this);
        this->m_anim->setDuration( this->m_speedTime );
        this->m_group->addAnimation( this->m_anim );
    }
    connect(this->m_group,SIGNAL(finished()),this,SLOT(changeItemPic()));
    this->m_transStart = this->m_stateList.at(0)->addTransition( this,SIGNAL(AnimationStart()),this->m_stateList.at(1) );
    this->m_transStart->addAnimation( this->m_group );
    for ( int i = 0; i < this->m_picCount; i++ )
    {
        for ( int j = 0; j < this->m_picCount; j++ )
        {
            int posIndex = j - i;
            if (posIndex < 0)
            {
                posIndex += this->m_picCount;
            }
            this->m_stateList.at(i)->assignProperty(this->m_itemList.at(j), "pos",this->m_picPosList.at(posIndex));
            if ( posIndex == ( this->m_picCount-1 ) )
            {
                this->m_stateList.at(i)->assignProperty(this->m_itemList.at(j),"visible",false);
            } else
            {
                this->m_stateList.at(i)->assignProperty(this->m_itemList.at(j),"visible",true);
            }
        }
        this->m_scrollMachine.addState( this->m_stateList.at(i));
        if ( i != this->m_picCount - 1 )
        {
            this->m_trans = this->m_stateList.at(i)->addTransition(this, SIGNAL(nextState()),this->m_stateList.at( i+1 ));
            this->m_trans->addAnimation(this->m_group);
        } else
        {
            this->m_transEnd = this->m_stateList.at(i)->addTransition(this,SIGNAL(nextState()),this->m_stateList.at(0));
            this->m_transEnd->addAnimation(this->m_group);
        }
    }
    this->m_scrollMachine.setInitialState(this->m_stateList.at(0));
    this->m_scrollMachine.start();
}

void Widget::changeItemPic()
{
    if(this->m_picWindowCount % this->m_picCount == 0)
    {
        this->m_picWindowCount = 0;
    }
    if(this->m_itemCount >= this->m_picList.size())
    {
        this->m_itemCount -= this->m_picList.size();
    }
    this->m_itemList.at(this->m_picWindowCount)->setPixmap(this->m_picList.at(this->m_itemCount));
    this->m_picWindowCount++;
    this->m_itemCount++;
    emit this->nextState();
}

void Widget::start()
{
    emit this->AnimationStart();
}

void Widget::resume()
{
    if(this->m_picList.at(0) != "NULL")
    {
        if(this->m_group->state() == QAbstractAnimation::Stopped)
        {
            this->start();
        }else if(this->m_group->state() == QAbstractAnimation::Paused)
        {
            this->m_group->setPaused(false);
        }
    }else
    {
        qDebug() << "this ApicAnimationstate has stop";
    }
}

void Widget::pause()
{
    if(this->m_picList.at(0)!="NULL")
    {
        this->m_group->setPaused(true);
    }else
    {
        qDebug() << "this Apicanimationstate has stop!";
    }
}
