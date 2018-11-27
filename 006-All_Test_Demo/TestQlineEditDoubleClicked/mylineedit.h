#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QObject>
#include <QLineEdit>
#include <QWidget>

class MyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit MyLineEdit(QLineEdit *parent = 0);
    virtual void mouseDoubleClickEvent(QMouseEvent *e);

signals:
    void doubleClicked();
public slots:
};

#endif // MYLINEEDIT_H
