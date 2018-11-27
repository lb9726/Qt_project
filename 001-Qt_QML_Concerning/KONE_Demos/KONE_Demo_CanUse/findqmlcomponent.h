#ifndef FINDQMLCOMPONENT_H
#define FINDQMLCOMPONENT_H

#include <QObject>
#include <QtQml>
#include <QQuickWindow>
#include <showfloor.h>
class FindQmlComponent : public QObject
{
    Q_OBJECT
public:
    explicit FindQmlComponent(QObject *parent = 0);

signals:

public slots:
};

#endif // FINDQMLCOMPONENT_H
