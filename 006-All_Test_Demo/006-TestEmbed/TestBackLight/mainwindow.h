#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "backlightctrl.h"
#include <QTimer>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTimer qtime;
    BacklightCtrl bac;
    int tmp;
public slots:
    void setBackLight();
};

#endif // MAINWINDOW_H
