#ifndef BACKLIGHTCTRL_H
#define BACKLIGHTCTRL_H

#include <QObject>
#define     BRIGHTNESS_CTR  "/sys/class/backlight/backlight/brightness"
#define     BESTGRADE       7
class BacklightCtrl : public QObject
{
    Q_OBJECT
public:
    explicit BacklightCtrl(QObject *parent = 0);
    int getLightGrade();
signals:

public slots:
    bool setBacklightValue(int brightness);
};

#endif // BACKLIGHTCTRL_H
