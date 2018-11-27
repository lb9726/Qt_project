#include "customslider.h"

void CustomSlider::mousePressEvent(QMouseEvent *ev)
{
    //注意应先调用父类的鼠标点击处理事件，这样可以不影响拖动的情况
    QSlider::mousePressEvent(ev);
    //获取鼠标的位置，这里并不能直接从ev中取值（因为如果是拖动的话，鼠标开始点击的位置没有意义了）
    double pos;
    if(this->orientation() == Qt::Horizontal )
    {   //水平Slider
        pos = ev->pos().x() / (double)width();
        setValue(pos * (maximum() - minimum()) + minimum());
    }
    else if(this->orientation() == Qt::Vertical )
    {   //垂直Slider
        pos = ev->pos().y() / (double)height();
        setValue(maximum() - pos * (maximum() - minimum()));
    }

    //发送自定义的鼠标单击信号
    emit costomSliderClicked();
}
