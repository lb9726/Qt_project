#ifndef NEWWINER_H
#define NEWWINER_H

#include <QDialog>
#include <QString>

namespace Ui {
class newwiner;
}

class newwiner : public QDialog
{
    Q_OBJECT

public:
    explicit newwiner(QWidget *parent = 0);
    ~newwiner();
    void setwintext(QString a,QString b,int c);

public slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_ok_clicked();

    void on_quit_clicked();

private:
    Ui::newwiner *ui;
    QString time;
    QString mine;
    int mine_num;
};

#endif // NEWWINER_H
