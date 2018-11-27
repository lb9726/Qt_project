#ifndef GAMEWINNER_H
#define GAMEWINNER_H

#include <QWidget>

namespace Ui {
class GameWinner;
}

class GameWinner : public QWidget
{
    Q_OBJECT

public:
    explicit GameWinner(QWidget *parent = 0);
    ~GameWinner();

private:
    Ui::GameWinner *ui;
};

#endif // GAMEWINNER_H
