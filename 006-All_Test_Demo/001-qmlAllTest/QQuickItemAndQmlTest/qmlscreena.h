#ifndef QMLSCREENA_H
#define QMLSCREENA_H

#include <QQuickItem>
#include <QColor>
#include <QTimer>
/*
 * Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
 *
*/
class QmlScreenA : public QQuickItem
{
    Q_OBJECT

    Q_ENUMS(GenerateAlgorithm)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor timeColor READ timeColor)

public:
    enum GenerateAlgorithm {
        RandomRGB,
        RandomRed,
        RandomGreen,
        RandomBlue,
        LinearIncrease
    };

public:
    QmlScreenA();
    ~QmlScreenA();

    QColor color() const;
    void setColor(const QColor &color);
    QColor timeColor()const;

    Q_INVOKABLE GenerateAlgorithm algorithm() const;
    Q_INVOKABLE void setAlgorithm(GenerateAlgorithm algorithm);

signals:
    void colorChanged(const QColor &color);
    void currentTime(const QString &strTime);

public slots:
    void changeColor();
    void slotColorChanged();

private:
    GenerateAlgorithm m_algorithm;
    QColor m_currentColor;
    int m_nColorTimer;
    QTimer mtimer;
};

#endif // QMLSCREENA_H
