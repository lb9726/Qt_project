#ifndef INTRODUCEMINE_H
#define INTRODUCEMINE_H

#include <QWidget>

namespace Ui {
class IntroduceMine;
}

class IntroduceMine : public QWidget
{
    Q_OBJECT

public:
    explicit IntroduceMine(QWidget *parent = 0);
    ~IntroduceMine();

private:
    Ui::IntroduceMine *ui;
};

#endif // INTRODUCEMINE_H
