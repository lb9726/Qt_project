#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDebug>
#define FILENAME  "/home/devin/Desktop/Repository/Qt_project/Qt_project/Qt-widget-project/testFilelength/a.txt"
namespace Ui {
class MainWindow;
}
typedef struct quanzhi {
    int quan;
}Quanzhi;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void CountFileSize(const char* filename, unsigned char *str, int count);
    void IntToUnsignedChar(int length, int count, unsigned char *str);
    int GetNewLength(int *i, int length, int array[], unsigned char *str, int *pos);
    int SplitNumber(int length, int array[], int *i);
    int Micifang(int zhishu, int dishu);
private:
    Ui::MainWindow *ui;
    bool ConvertFlag;
};

#endif // MAINWINDOW_H
