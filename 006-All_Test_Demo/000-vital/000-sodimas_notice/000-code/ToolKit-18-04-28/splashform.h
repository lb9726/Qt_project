#ifndef SPLASHFORM_H
#define SPLASHFORM_H

#include <QWidget>

namespace Ui {
class SplashForm;
}

class SplashForm : public QWidget
{
    Q_OBJECT

public:
    explicit SplashForm(QWidget *parent = 0);
    ~SplashForm();

private:
    Ui::SplashForm *ui;
};

#endif // SPLASHFORM_H
