#ifndef UIDESKTOP_H
#define UIDESKTOP_H
#include <QWidget>
#include <QDomElement>

class UiDesktop : public QWidget
{
    Q_OBJECT
public:
    UiDesktop(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    void Init(QDomElement pElement);

    void paintEvent(QPaintEvent *event);

public:
    QPixmap  mHorizontalPixmap;
    QPixmap  mVerticalPixmap;
};

#endif // UIDESKTOP_H
