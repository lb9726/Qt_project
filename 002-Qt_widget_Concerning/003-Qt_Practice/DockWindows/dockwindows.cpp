#include "dockwindows.h"
#include <QTextEdit>
#include <QDockWidget>

DockWindows::DockWindows(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("DockWindows")); // 设置主窗口的标题栏文字
    QTextEdit *te = new QTextEdit(this); // 定义一个QTextEdit 对象作为主窗口
    te->setText(tr("Main Window"));
    te->setAlignment(Qt::AlignCenter);
    setCentralWidget(te); // 将此编辑框设为主窗口的中央窗口
    // 停靠窗口 1
    QDockWidget *dock = new QDockWidget(tr("DockWindow1"), this);
    // 可移动
    dock->setFeatures(QDockWidget::DockWidgetMovable);        // (a)
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea); // (b)
    QTextEdit *te1 = new QTextEdit();
    te1->setText(tr("Window1, The dock widget can be moved between docks by the user"));
    dock->setWidget(te1);
    addDockWidget(Qt::RightDockWidgetArea,dock);
    // 停靠窗口 2
    dock = new QDockWidget(tr("DockWindow2"), this);
    dock->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetFloatable);
    QTextEdit *te2 = new QTextEdit();
    te2->setText(tr("Window2, the dock widget can be detached from the main window,"
                    "and floated as an independent window, and can be closed"));
    dock->setWidget(te2);
    addDockWidget(Qt::RightDockWidgetArea,dock);
    // 停靠窗口 3
    dock = new QDockWidget(tr("DockWindow3"), this);
    dock->setFeatures(QDockWidget::AllDockWidgetFeatures);// 全部特性
    QTextEdit *te3 = new QTextEdit();
    te3->setText(tr("window3, the dock widget can be closed ,moved ,an floated"));
    dock->setWidget(te3);
    addDockWidget(Qt::RightDockWidgetArea,dock);

}

DockWindows::~DockWindows()
{

}
