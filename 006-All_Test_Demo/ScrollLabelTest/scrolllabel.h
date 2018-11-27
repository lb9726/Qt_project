#ifndef SCROLLLABEL_H
#define SCROLLLABEL_H

#include <QLabel>
#include <QTimer>
#include <QString>

class ScrollLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ScrollLabel(QLabel *parent = 0);
    void setScrollText(QString str);
    void setInterval(int pInterval);

signals:

public slots:
    void scrollCaption();

private:
    QTimer      mTimer;
    QString     mScrollText;
    QString     mTmpText;
    int         mInterval;
};

#endif // SCROLLLABEL_H
