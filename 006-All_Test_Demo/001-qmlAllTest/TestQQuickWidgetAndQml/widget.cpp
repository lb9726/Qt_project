#include "widget.h"
//#include "ui_widget.h"
#include <QUrl>
Widget::Widget(QWidget *parent) :
    QWidget(parent)/*,*/
//    ui(new Ui::Widget)
{
//    ui->setupUi(this);
}

Widget::~Widget()
{
//    delete ui;
}

QQuickWidget *Widget::setQmlIntoWidget()
{
    m_quickWidget = new QQuickWidget(this);
    QUrl source("qrc:/qml/MultiMedia.qml");
//    QUrl source("/home/libo/Desktop/Repository/Qt_project/000-All_Test/TestQQuickWidgetAndQml/MultiMeida.qml");
    m_quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView );
    m_quickWidget->setSource(source);
    m_quickWidget->move(0, 0);
//    m_quickWidget->resize(300, 500);
    return m_quickWidget;
}

