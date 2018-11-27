#include "widget.h"
#include "ui_widget.h"
#include<QPainter>
#include<qcoreevent.h>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    offset = 0;
    myTimerId = 0;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setText(const QString& newText)
{
    myText = newText;
    update();
    updateGeometry();
}

QSize Widget::sizeHint()const
{
    return fontMetrics().size(0,text());
}

void Widget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    int textWidth = fontMetrics().width(text());
    if(textWidth < 1)
        return;
    int x =- offset;
    while(x < width())
    {
        painter.drawText(x, 0, textWidth, height(),
                         Qt::AlignLeft|Qt::AlignVCenter, text());
        x += textWidth;
    }
}

void Widget::timerEvent(QTimerEvent* event)
{
    if(event->timerId()==myTimerId){
        ++offset;
        if(offset>=fontMetrics().width(text()))
            offset=0;
        scroll(-1, 0);
    }else{
        QWidget::timerEvent(event);
    }
}

void Widget::showEvent(QShowEvent* event)
{
    myTimerId = startTimer(40);
}

void Widget::hideEvent(QHideEvent* event)
{
    killTimer(myTimerId);
    myTimerId = 0;
}
