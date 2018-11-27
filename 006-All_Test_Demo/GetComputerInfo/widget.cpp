#include "widget.h"
#include "getcomputerinfos.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    GetComputerInfos gci;
    gci.getIp();
    gci.getMachineIp();
}

Widget::~Widget()
{

}
