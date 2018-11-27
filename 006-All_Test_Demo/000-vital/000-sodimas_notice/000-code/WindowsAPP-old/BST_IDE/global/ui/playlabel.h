#ifndef PLAYLABEL_H
#define PLAYLABEL_H

#include "global.h"
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QLabel>

class PlayLabel : public QLabel
{
    Q_OBJECT
public:
    explicit PlayLabel(QWidget *parent = 0);
    ~PlayLabel();
    void SetEvnRcver(QWidget *pRcvOB);
public:
    QWidget*        mQesp;
protected:
    void mouseDoubleClickEvent(QMouseEvent* ev);
    void keyPressEvent(QKeyEvent *ev);
signals:
    
public slots:  
};

#endif // PLAYLABEL_H
