#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMediaPlayer>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void init();

public slots:
    void positionChanged(qint64 pos);

private:
    Ui::Widget *ui;
    QMediaPlayer *mPlayer;
};

#endif // WIDGET_H
