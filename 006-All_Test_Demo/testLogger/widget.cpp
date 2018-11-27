#include "widget.h"
#include <QString>
//#include <qloggingcategory.h>
#include <QLoggingCategory>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    Init();
}

Widget::~Widget()
{

}

void Widget::Init()
{
    IDE_TRACE_STR(QString("123"));
    IDE_PRINTF("%s","344");
    QString lb = "fjeoifaf";
    qDebug()<<"123234";
//    qCritical()<< lb;
    QLoggingCategory category("driver.usb");
    qCWarning(category) << "a warning message";

}
