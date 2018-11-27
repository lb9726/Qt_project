#include "widget.h"
#include "ui_widget.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->label->setText("just test usb");
    p = new QDeviceWatcher;
    p->appendEventReceiver(this);
    connect(p, SIGNAL(deviceAdded(QString)), this, SLOT(addusb(QString)));
    connect(p, SIGNAL(deviceRemoved(QString)), this, SLOT(rmusb(QString)));
    p->start();
}

Widget::~Widget()
{
    delete ui;
    delete p;
}

void Widget::addusb(QString str)
{
    ui->label->setText("add usb");
    ui->label->setText(str);
}

void Widget::rmusb(QString str)
{
    ui->label->setText("remove usb");
    ui->label->setText(str);
}
