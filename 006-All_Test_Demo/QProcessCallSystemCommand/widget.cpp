#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_unZip.startUnZip("sodimas_canopen.zip");

}

Widget::~Widget()
{
    delete ui;
}
