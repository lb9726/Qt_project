#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "buttons.h"
#include "gpios.h"
#include <QTimer>
#include <testthread.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ButtonS bts;
    Gpios gps;
    QTimer timers;
    SpecialBtns sbt;
    TestThread  t1;
    TestThread2 t2;
    TestThread3 t3;
    TestThread4 t4;
    TestThread5 t5;
};

#endif // MAINWINDOW_H
