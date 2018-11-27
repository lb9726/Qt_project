#ifndef MYGRAPHICVIEW_H
#define MYGRAPHICVIEW_H

#include <QObject>

class MyGraphicView : public QObject
{
    Q_OBJECT
public:
    explicit MyGraphicView(QObject *parent = 0);

signals:

public slots:
};

#endif // MYGRAPHICVIEW_H