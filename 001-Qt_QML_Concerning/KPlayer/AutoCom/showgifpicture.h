#ifndef SHOWGIFPICTURE_H
#define SHOWGIFPICTURE_H

#include <QObject>
#include <QVariant>
class showGifPicture : public QObject
{
    Q_OBJECT
public:
    explicit showGifPicture(QObject *parent = 0);

signals:

public slots:
    QVariant isShowGif(QString filename);
};

#endif // SHOWGIFPICTURE_H
