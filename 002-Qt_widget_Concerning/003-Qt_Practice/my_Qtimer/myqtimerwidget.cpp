#include "myqtimerwidget.h"
#include <QGridLayout>
#include <QTimer>
#include <QDateTime>
myQtimerWidget::myQtimerWidget(QWidget *parent)
    : QWidget(parent)
{
    lineEdit = new QLineEdit(this);
    start = new QPushButton(this);
    QTimer *time_clock = new QTimer(this);
    lcd = new QLCDNumber(this);
    start->setText(tr("kaishi"));
    end = new QPushButton(this);
    end->setText("close");
    QGridLayout *mainLayout = new QGridLayout(this);
    setMinimumSize(500,500);
    setMaximumSize(900,900);
    mainLayout->addWidget(lineEdit,0,0);
    mainLayout->addWidget(start,1,1);
    mainLayout->addWidget(end,7,1);
    mainLayout->addWidget(lcd,3,0);
//    lcd->setDigitCount(8);
    time_clock->start(2000);
    connect(end,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(start,SIGNAL(clicked(bool)),this,SLOT(starttimer()));
    connect(time_clock,SIGNAL(timeout()),this,SLOT(sQtime()));
}
void myQtimerWidget::closewindows()
{
    this->close();
}

void myQtimerWidget::starttimer()
{
    QDateTime *datatime = new QDateTime(QDateTime::currentDateTime());
    lineEdit->setText(datatime->date().toString());
    lineEdit->show();
}

void myQtimerWidget::sQtime()
{
//    QDateTime *datatime = new QDateTime(QDateTime::currentDateTime());
//    lineEdit->setText(datatime->date().toString("hh:mm:ss"));
//    lineEdit->show();

    QTime time = QTime::currentTime();
    lcd->setDigitCount(8);
    lcd->display(time.toString("hh:mm:ss"));

}

myQtimerWidget::~myQtimerWidget()
{

}

