#ifndef MESSAGERBAR_H
#define MESSAGERBAR_H

#include <QWidget>

namespace Ui {
class MessagerBar;
}

class MessagerBar : public QWidget
{
    Q_OBJECT

public:
    explicit MessagerBar(QWidget *parent = 0);
    ~MessagerBar();

private:
    Ui::MessagerBar *ui;
};

#endif // MESSAGERBAR_H
