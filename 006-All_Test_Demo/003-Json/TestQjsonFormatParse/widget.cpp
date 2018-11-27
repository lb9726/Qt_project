#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
//    mjson.readJson();
//    mjson.parserJsonFormat();
//    mjson.readHeartJson();

}

Widget::~Widget()
{
    delete ui;
}
