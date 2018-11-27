#ifndef UPDATE_H
#define UPDATE_H

#include <QWidget>

namespace Ui {
class Form;
}

class update : public QWidget
{
    Q_OBJECT
public:
    explicit update(QWidget *parent = 0);
    ~update();
signals:

public slots:
private:
    Ui::Form *up;
};

#endif // UPDATE_H
