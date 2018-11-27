#ifndef MYGAME_H
#define MYGAME_H

#include <QDialog>

namespace Ui {
class mygame;
}

class mygame : public QDialog
{
    Q_OBJECT

public:
    explicit mygame(QWidget *parent = 0);
    ~mygame();
    int getlength(){return length;}
    int getwidth(){return width;}
    int getmine(){return mine;}

private slots:
    void on_length_textChanged(const QString &arg1);

    void on_width_textChanged(const QString &arg1);

    void on_mine_textChanged(const QString &arg1);

    void on_mynewgame_clicked();

private:
    Ui::mygame *ui;
    int length;
    int width;
    int mine;
    int length_flag;
    int width_flag;
    int mine_flag;
    void setbutton();
};

#endif // MYGAME_H
