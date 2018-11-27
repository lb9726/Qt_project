#ifndef BOOTSCREEN_H
#define BOOTSCREEN_H

#include <QObject>
#include <QTimer>
#include <QLabel>
class BootScreen : public QObject
{
    Q_OBJECT
public:
    explicit BootScreen(QObject *parent = 0);
    QTimer qtime;
    QLabel * qlabel;

signals:

public slots:
    void hideScreen();
};

#endif // BOOTSCREEN_H
