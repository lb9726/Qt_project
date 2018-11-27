#include "stackdlg.h"
#include <QHBoxLayout>

StackDlg::StackDlg(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Stackedwidget"));
    list = new QListWidget(this); // 新建一个QListWidget 控件对象
    // 在新建的QListWidget 控件中插入三个条目，作为选择项
    list->insertItem(0, tr("Window1"));
    list->insertItem(1, tr("Window2"));
    list->insertItem(2,tr("Window3"));

    // 创建三个QLabel标签控件对象，作为堆栈窗口需要显示的三层窗口
    label1 = new QLabel(tr("WindowTest1"));
    label2 = new QLabel(tr("WindowTest2"));
    label3 = new QLabel(tr("WindowTest3"));
    stack = new QStackedWidget(this); // 新建一个QStackedWidget堆栈窗体对象

    // 将创建的三个QLabel 标签控件依次插入堆栈窗体中
    stack->addWidget(label1);
    stack->addWidget(label2);
    stack->addWidget(label3);
    QHBoxLayout *mainLayout = new QHBoxLayout(this); // 对整个对话框进行布局

    mainLayout->setMargin(5); // 设定对话框（或窗体）的边距为5
    mainLayout->setSpacing(5);// 设定各个控件之间的间距为5
    mainLayout->addWidget(list);
    mainLayout->addWidget(stack,0,Qt::AlignCenter);
    mainLayout->setStretchFactor(list,1);
    mainLayout->setStretchFactor(stack,3);
    connect(list,SIGNAL(currentRowChanged(int)),stack, SLOT(setCurrentIndex(int)));

}

StackDlg::~StackDlg()
{

}
