#ifndef QJSONTEST_H
#define QJSONTEST_H

#include <QObject>
#include <QByteArray>

class QjsonTest : public QObject
{
    Q_OBJECT
public:
    explicit QjsonTest(QObject *parent = 0);
    QByteArray generateJsonData();
signals:

public slots:
};

#endif // QJSONTEST_H
