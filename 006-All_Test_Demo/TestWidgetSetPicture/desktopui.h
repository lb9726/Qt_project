#ifndef DESKTOPUI_H
#define DESKTOPUI_H

#include <QWidget>
#include <QPixmap>
#include <QString>
#include <QLabel>

class DesktopUi : public QWidget
{
    Q_OBJECT
public:
    explicit DesktopUi(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    void init();
    void getPointer();
signals:

public slots:
private:

private:
    QPixmap  mHorizontalPixmap;
    QPixmap  mVerticalPixmap;
    QString mRunPath;

    QLabel* mArrow;
    QLabel* mFloor;
};

#endif // DESKTOPUI_H
