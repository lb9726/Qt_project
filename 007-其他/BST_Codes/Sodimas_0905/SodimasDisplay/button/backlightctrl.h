#ifndef BACKLIGHTCTRL_H
#define BACKLIGHTCTRL_H

#include <QObject>

class BacklightCtrl : public QObject
{
    Q_OBJECT
public:
    explicit BacklightCtrl(QObject *parent = 0);
    static bool setBacklightValue(quint8 brightness);
signals:

public slots:

};

#endif // BACKLIGHTCTRL_H
