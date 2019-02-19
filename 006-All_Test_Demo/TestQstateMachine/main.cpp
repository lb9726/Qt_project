//#include "widget.h"
//#include <QApplication>

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    Widget w;
//    w.show();

//    return a.exec();
//}

#include <QApplication>
#include <QPushButton>
#include <QState>
#include <QStateMachine>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QPushButton button("state machine");

    QStateMachine machine;
    //新建三个状态机
    QState* s1 = new QState(&machine);
    QState* s2 = new QState(&machine);
    QState* s3 = new QState(&machine);
    //设定三个状态分别控制button在不同的位置显示
    s1->assignProperty(&button, "geometry", QRect(100, 100, 100, 50));
    s2->assignProperty(&button, "geometry", QRect(300, 100, 100, 50));
    s3->assignProperty(&button, "geometry", QRect(200, 200, 100, 50));
    //关联三个状态间的切换
    s1->addTransition(&button, SIGNAL(clicked()), s2);
    s2->addTransition(&button, SIGNAL(clicked()), s3);
    s3->addTransition(&button, SIGNAL(clicked()), s1);
    // entred为进入 exited离开 还可以有finished propertiesAssigned 	destroyed
    //QObject::connect(s3, SIGNAL(entered()), &button, SLOT(showMinimized()));
    //设置起始状态
    machine.setInitialState(s1);
    machine.start();
    button.show();
    return app.exec();
}
