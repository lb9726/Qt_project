#ifndef HERODIALOG_H
#define HERODIALOG_H

#include "common.h"
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>

class heroDialog : public QDialog
{
    Q_OBJECT
public:
    explicit heroDialog(QWidget *parent = 0);//构造函数

    QGridLayout *glayout;
    QLabel *label_low;
    QLabel *label_low_time;
    QLabel *label_low_name;
    QLabel *label_middle;
    QLabel *label_middle_time;
    QLabel *label_middle_name;
    QLabel *label_high;
    QLabel *label_high_time;
    QLabel *label_high_name;
    QPushButton *pushButton;
    QPushButton *pushButton_OK;

signals:
    
public slots:
    void setHeroDialog(); //设置英雄榜对话框的内容   
};

#endif // HERODIALOG_H
