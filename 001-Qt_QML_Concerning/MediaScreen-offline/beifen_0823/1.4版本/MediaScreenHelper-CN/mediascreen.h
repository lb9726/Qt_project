#ifndef MEDIASCREEN_H
#define MEDIASCREEN_H

#include <QObject>

class MediaScreen : public QObject
{
    Q_OBJECT
public:
    explicit MediaScreen(QObject *parent = 0);

signals:

public slots:
};

#endif // MEDIASCREEN_H