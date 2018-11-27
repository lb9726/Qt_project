#include "mylineedit.h"
#include <QMouseEvent>
#include <QDebug>
#include <QAction>

MyLineEdit::MyLineEdit(QLineEdit *parent) : QLineEdit(parent)
{
}

void MyLineEdit::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (QEvent::MouseButtonDblClick == e->type())
    {
        qDebug()<<__PRETTY_FUNCTION__<<"is double clicked";
        emit doubleClicked();
    }
}
