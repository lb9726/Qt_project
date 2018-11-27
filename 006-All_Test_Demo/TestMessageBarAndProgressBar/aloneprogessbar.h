#ifndef ALONEPROGESSBAR_H
#define ALONEPROGESSBAR_H

#include <QWidget>

namespace Ui {
class AloneProgessBar;
}

class AloneProgessBar : public QWidget
{
    Q_OBJECT

public:
    explicit AloneProgessBar(QWidget *parent = 0);
    ~AloneProgessBar();

private:
    Ui::AloneProgessBar *ui;
};

#endif // ALONEPROGESSBAR_H
