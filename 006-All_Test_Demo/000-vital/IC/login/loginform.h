#ifndef LOGINFORM_H
#define LOGINFORM_H

#include "uiclass.h"
#include "sqlparser.h"

namespace Ui {
class LoginForm;
}

#define D_SHADOW_LEN  10
#define D_SHADOW_RADIUS  4

class LoginForm : public QDialog
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = 0);
    ~LoginForm();

    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

    void ClearLoginError();
    void ClearRegisterError();

private slots:
    void on_btn_reg_back_clicked();
    void on_btn_close_clicked();
    void on_btn_min_clicked();
    void on_btn_login_clicked();

    void on_check_remberpasswd_clicked(bool checked);

    void slot_LoginFoucsIn();
    void slot_LoginFoucsOut();
    void slot_RegisterFoucsIn();
    void slot_RegisterFoucsOut();

    void slot_InitImage();

    void on_btn_forgetpasswd_clicked();
    void on_btn_reg_confirm_clicked();

public:
    Ui::LoginForm *ui;

    SQL_PARSER         *mSqlParser;

    QString             mUsrName;

    QPixmap        mIconPixmap;
    bool           m_bIsPressed;
    QPoint         m_windowsPoint, m_startPoint;
};

#endif // LOGINFORM_H
