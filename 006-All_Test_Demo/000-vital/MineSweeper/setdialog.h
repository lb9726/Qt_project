#ifndef SETDIALOG_H
#define SETDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>

class setDialog : public QDialog
{
    Q_OBJECT
public:
    explicit setDialog(QWidget *parent = 0);//构造函数

    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *label_1;
    QLabel *label_2;
    QLabel *label_3;
    QSpinBox *spinBox_row;
    QSpinBox *spinBox_col;
    QSpinBox *spinBox_mine;
    QPushButton *button_1;
    QPushButton *button_2;

signals:
    void sendSetInfo(int t_rowNum,int t_colNum,int t_mineNum);//游戏设置信息被设置信号(信号)
    
public slots:
    void dealWithSetInfo();	//接受信息被设置函数(槽)
    
};

#endif // SETDIALOG_H
