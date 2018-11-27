#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void setText(const QString& newText);
    QString text()const{return myText;}
    QSize sizeHint()const;
protected:
    void paintEvent(QPaintEvent*event);
    void timerEvent(QTimerEvent*event);
    void showEvent(QShowEvent*event);
    void hideEvent(QHideEvent*event);
private:
    QString myText;
    int offset;
    int myTimerId;
    Ui::Widget *ui;
};

#endif // WIDGET_H
