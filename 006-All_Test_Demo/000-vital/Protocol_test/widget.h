#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "protocoldeal.h"
#include <QString>
#include <QImage>
#include <QGraphicsView>
namespace Ui {
class Widget;
}

class Protocoldeal;

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void UiInit();
    void SetWidgetBackGround(QString path);
    void HideGraphicViewBorder();
    void SetPicture(QString path, QGraphicsView *graphic);
    void AnimationForPicture();
    void PosAnimationTextScroll();
    QString GainFloorNumber(unsigned char *str);
    void ShowArrowStatus(unsigned char str);
    void SetTimerArrowUp();
    void SetTimerArrowDn();
    void ShiningArrowUp();
    void ShiningArrowDn();

public slots:
    void DealNewData(unsigned char s);
    void ShowTime();
    void ShowDate();
    void ChangePositionUp();
    void ChangePositionDn();
    void SetFloorNumber(QString s);
    void OnShiningArrowUp();
    void OnShiningArrowDn();
    void CopyData();

signals:
    void DateChangesSignal();
private:
    Ui::Widget *ui;
    Protocoldeal *pro;
    QImage *image;
    QTimer *timer;
//    QTimer *writetimer;
};

#endif // WIDGET_H
