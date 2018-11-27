#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QDebug>
#include <QMovie>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btn_left_clicked();

    void on_btn_right_clicked();

    void on_btn_up_clicked();

    void on_pushButton_clicked();

    void on_btn_down_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
