#ifndef MANAGERDIALOG_H
#define MANAGERDIALOG_H

#include <QDialog>

namespace Ui {
class ManagerDialog;
}

class ICManager;

class ManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ManagerDialog(QWidget *parent = 0);
    ~ManagerDialog();

private slots:
    void on_btn_ok_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::ManagerDialog *ui;

    ICManager          *mParent;
};

#endif // MANAGERDIALOG_H
