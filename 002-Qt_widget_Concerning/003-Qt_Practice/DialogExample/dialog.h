#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QColorDialog>
#include "inputdlg.h"
class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();
private:
    QPushButton *fileBtn;
    QLineEdit *fileLineEdit;
    QGridLayout *mainLayout;
    QPushButton *colorBtn;
    QFrame *colorFrame;
    QPushButton *fontBtn;
    QLineEdit *fontLineEdit;
    QPushButton *inputBtn;
    InputDlg *inputDig;

private slots:
    void showFile();
    void showColor();
    void showFont();
    void showInputDlg();

};

#endif // DIALOG_H
