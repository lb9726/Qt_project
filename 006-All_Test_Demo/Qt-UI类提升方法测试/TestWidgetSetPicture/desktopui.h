#ifndef DESKTOPUI_H
#define DESKTOPUI_H

#include <QWidget>
#include <QPixmap>
#include <QString>

class DesktopUi : public QWidget
{
    Q_OBJECT
public:
    explicit DesktopUi(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    void init();

signals:

public slots:
private:
    QPixmap  mHorizontalPixmap;
    QPixmap  mVerticalPixmap;
    QString mRunPath;
};

#endif // DESKTOPUI_H
