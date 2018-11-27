#include "glabel.h"
#include <QPen>
#include <QDebug>

GLabel::GLabel(QWidget *parent, GLabel::PEOPLE f) :
    QLabel(parent)
{
      people = f;
      parent_maxwidth = ((QWidget*)this->parent())->width();
      init();
}

void GLabel::init()
{
    label_text  = new QLabel(this);
    label_text->setWordWrap(true);
    label_text->setGeometry(20+30, 20, this->width()-40-30, this->height()-40);
    label_text->setStyleSheet("QLabel{background:rgb(173, 216, 67)}");//rgb(173, 216, 67)
    label_text->show();

    label_people = new QLabel(this);
    if(people == GLabel::She)
    {
        label_people->setGeometry(0, 18, 30, 30);
        label_text->setStyleSheet("QLabel{background:rgb(173, 216, 67)}");
        label_people->setPixmap(QPixmap(":/image/she.png"));
    }
    else
    {
       label_people->setGeometry(this->width()-30, 18, 30, 30);
       label_text->setStyleSheet("QLabel{background:rgb(240, 240, 240)}");
       label_people->setPixmap(QPixmap(":/image/he.png"));
    }
}

void GLabel::initUI(GLabel::PEOPLE f)
{

}

void GLabel::setSize(QString &paramstr)
{
//    int all_word_width  = this->fontMetrics().width(paramstr);
//    int one_line_height = this->fontMetrics().lineSpacing();
    int max_width = parent_maxwidth-50;//-label_people->width();
    int all_word_width;

    label_text->setWordWrap(false);
    label_text->setText(paramstr);
    label_text->adjustSize();
    all_word_width = label_text->width();
    label_text->setWordWrap(true);

    float line_count;
    int line;

    if(all_word_width > max_width)
    {
//        line_count = (float)all_word_width/max_width;
//        line = (int)line_count;
//        if(line < line_count)line=line+2;
        label_text->setMaximumWidth(max_width-20-30);
        label_text->setText(paramstr);
        label_text->adjustSize();
        qDebug()<<"the size of text"<<label_text->size()<<((QWidget*)this->parent())->width()-15;

        m_height = label_text->height()/*line*one_line_height*/;
        m_width  = parent_maxwidth-15;
       // qDebug()<<"linr_count"<<line_count<<one_line_height<<max_width<<all_word_width;
        this->resize(m_width, m_height+40);
    }
    else
    {
        label_text->setWordWrap(false);
        m_height = 66;

        label_text->setText(paramstr);
        label_text->adjustSize();
        m_width = label_text->width()+40+30;
        //label_text->setMinimumWidth(all_word_width);
        this->resize(m_width+10, m_height);
    }

    if(people == GLabel::She)
        label_text->setGeometry(20+40, 20, this->width()-40-30, this->height()-40);
    else
        label_text->setGeometry(20, 20, this->width()-40-30, this->height()-40);

    update();
}

void GLabel::setOurText(QString paramstr)
{
    setSize(paramstr);
    qDebug()<<"setSize"<<this->width()<<this->height();
    //label_text->setWordWrap(true);
   // label_text->setText(paramstr);
}

void GLabel::mousePressEvent(QMouseEvent *e)
{
    qDebug()<<"this pos is"<<e->pos();
}

void GLabel::setHeadpic(QString dir)
{
    label_people->setPixmap(QPixmap(dir));
}

void GLabel::paintEvent(QPaintEvent *e)
{
    //QPainter painter(this);
    if(people == GLabel::She)
    {
        QRectF rectangle(50.0, 10.0, this->width()-20-40, this->height()-20);

        QPainter painter(this);
        painter.setRenderHint( QPainter::Antialiasing, true );
        painter.setBrush(QBrush(QColor(173, 216, 67)));
        painter.drawRoundedRect(rectangle, 10, 10);

        QPointF points[3] = {
           QPointF(40, 33),
           QPointF(51, 28),
           QPointF(51, 38),
       };

       QPen pen;
       pen.setColor(QColor(173, 216, 67));
       painter.setPen(pen);
       painter.drawPolygon(points, 3);

       QPen pen1;
       pen.setColor(Qt::black);
       painter.setPen(pen1);
       painter.drawLine(points[0], points[1]);
       painter.drawLine(points[0], points[2]);
    }
    else if(people == GLabel::Me)
    {
        QRectF rectangle(10.0, 10.0, this->width()-20-40, this->height()-20);

        QPainter painter(this);
        painter.setRenderHint( QPainter::Antialiasing, true );
        painter.setBrush(QBrush(QColor(240, 240, 240)));
        painter.drawRoundedRect(rectangle, 10, 10);

        QPointF points[3] = {
           QPointF(this->width()-40, 33),
           QPointF(this->width()-51, 28),
           QPointF(this->width()-51, 38),
       };

       QPen pen;
       pen.setColor(QColor(240, 240, 240));
       painter.setPen(pen);
       painter.drawPolygon(points, 3);

       QPen pen1;
       pen.setColor(Qt::black);
       painter.setPen(pen1);
       painter.drawLine(points[0], points[1]);
       painter.drawLine(points[0], points[2]);

      // label_text->setGeometry(20, 20, this->width()-40-30, this->height()-40);
       label_people->setGeometry(this->width()-30, 18, 30, 30);
       this->setGeometry(((QWidget*)this->parent())->width()-15-this->width(), this->y(), this->width(), this->height());
    }
   // qDebug()<<"this size of pix"<<label_people->size();
   // drawShe();
}
