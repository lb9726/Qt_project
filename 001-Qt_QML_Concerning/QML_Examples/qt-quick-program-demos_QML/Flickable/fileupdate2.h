#ifndef FILEUPDATE_H
#define FILEUPDATE_H

#include <QObject>
#include <QTimer>
#include <QDebug>
class FileUpdate2 : public QObject
{
    Q_OBJECT
public:
    explicit FileUpdate2(QObject *parent = 0);

signals:
    void sendPercent(int val);
    void sendStartAniDown(bool animstardown);
    void sendStartAniUp(bool animstarup);
public slots:
    void ChangePercent();
    void StartAnimDown();
    void StartAnimUp();
};

#endif // FILEUPDATE_H
