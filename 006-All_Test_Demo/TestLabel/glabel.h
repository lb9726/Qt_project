#ifndef GLABEL_H
#define GLABEL_H

#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

class GLabel : public QLabel
{
    Q_OBJECT
public:
    enum PEOPLE{She,Me};
    Q_PROPERTY(QString str_context READ getStrContext WRITE setContext)
    explicit GLabel(QWidget *parent = 0,GLabel::PEOPLE f=GLabel::She);
    void setContext(const QString s){str_context = s;}
    QString getStrContext()const{return str_context;}

    QString str_context;
    QLabel *label_text;
    QLabel *label_people;
    PEOPLE people;
public:
    int m_width;
    int m_height;
    int parent_maxwidth;

    void init();
    void initUI(GLabel::PEOPLE f);
    void setSize(QString&);
    void setOurText(QString);
    void setHeadpic(QString);
    void  paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
//signals:

//public slots:

};
#endif // GLABEL_H
