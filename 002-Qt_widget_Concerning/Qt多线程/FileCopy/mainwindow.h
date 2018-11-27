#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStringList>
#include "copythread.h"


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
    void copyStation(int flag,QString qsText);

    void on_selectSrcBtn_clicked();

    void on_selectDesBtn_clicked();

    void on_startBtn_clicked();

private:
    Ui::MainWindow *ui;
    CopyThread *copyThread;
};

#endif // MAINWINDOW_H
