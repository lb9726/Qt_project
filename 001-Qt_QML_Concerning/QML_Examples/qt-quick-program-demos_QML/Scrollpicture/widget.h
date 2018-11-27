#ifndef WIDGET_H
#define WIDGET_H
#define ROLLPICTURE_H
#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QState>
#include <QStateMachine>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QSignalTransition>
#include <QDebug>

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QStringList picList,QSize picSize,int picCount,
    int speedTime,int space = 0,QWidget *parent = 0);
    void resume();//恢复
    void pause();//暂停
    signals:
    void AnimationStart();
    void nextState();
    public slots:
    void start();
    void changeItemPic();
    private:
    void initItem();
    void initStates();
    void initCoord();
    void initAnimationGroup();
    private:
    QStringList m_picList;
    QSize m_picSize;
    int m_picCount;
    int m_speedTime;
    int m_space;
    QSize m_itemSize;
    int m_itemCount;
    int m_picWindowCount;
    QState *m_state; //??
    QLabel *m_item;
    QParallelAnimationGroup *m_group; //
    QPropertyAnimation *m_anim; //
    QStateMachine m_scrollMachine; //
    QSignalTransition *m_transStart; //
    QSignalTransition *m_trans; //
    QSignalTransition *m_transEnd; //
    QList <QState *> m_stateList; //
    QList <QLabel *> m_itemList;
    QList <QPoint> m_picPosList;
public:
    ~Widget();
};

#endif // WIDGET_H
