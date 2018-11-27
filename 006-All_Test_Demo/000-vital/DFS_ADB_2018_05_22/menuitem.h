#ifndef MENUITEM_H
#define MENUITEM_H

#include <QWidget>

namespace Ui {
class MenuItem;
}

class MenuItem : public QWidget
{
    Q_OBJECT

public:
    explicit MenuItem(QWidget *parent = 0);
    ~MenuItem();

    void SetPressed(bool pFlag);

    void SetName(QString pName);

    void InitList(int pCur, int pMin, int pMax);
    void InitList(int pCur, QStringList pImgList);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

signals:
    void sSelected();
    void sDataChanged(QString pText);

public slots:

public:
    QPixmap mPixmap;

    int  mType;  //0为数字调整，1为图片选择

    int mCur, mMin, mMax;

    int         mImgIndex;
    QStringList mImgList;

private slots:
    void on_btn_left_clicked();

    void on_btn_right_clicked();

private:
    Ui::MenuItem *ui;
};

#endif // MENUITEM_H
