#ifndef MYQTIMERWIDGET_H
#define MYQTIMERWIDGET_H
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <QLCDNumber>
class myQtimerWidget : public QWidget
{
    Q_OBJECT

public:
    myQtimerWidget(QWidget *parent = 0);
    ~myQtimerWidget();


private:
    QLineEdit *lineEdit;
    QPushButton *start,*end;
    QLCDNumber *lcd;
private slots:
    void closewindows();
    void starttimer();
    void sQtime();
};

#endif // MYQTIMERWIDGET_H
