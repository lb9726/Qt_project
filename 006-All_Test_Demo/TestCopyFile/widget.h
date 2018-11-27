#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void onClicked();
    bool copyFile(QString const& srcfile, QString const& destfile);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
