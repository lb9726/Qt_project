#include "widget.h"
#include "ui_widget.h"
#include "qdevicewatcher_p.h"
#include "qdevicewatcher.h"
#include <QDebug>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    qDebug()<< __PRETTY_FUNCTION__;
    ui->setupUi(this);
//    usbmanager *u = new usbmanager;
    q = new QDeviceWatcher;
    q->appendEventReceiver(this);
//    QDeviceWatcher q;
    connect(q, SIGNAL(deviceAdded(QString)), this, SLOT(addusb(QString)));
    connect(q, SIGNAL(deviceRemoved(QString)), this, SLOT(rmusb(QString)));
    q->start();
    ui->label->setText("just test");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::addusb(QString str)
{
    qDebug()<< __PRETTY_FUNCTION__;
    ui->label->setText("add usb");
}

void Widget::rmusb(QString str)
{
    qDebug()<< __PRETTY_FUNCTION__;
    ui->label->setText("remove usb");
}
