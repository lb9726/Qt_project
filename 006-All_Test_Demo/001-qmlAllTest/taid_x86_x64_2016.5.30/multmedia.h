#ifndef MULTMEDIA_H
#define MULTMEDIA_H

#include <QObject>

class MultMedia : public QObject
{
    Q_OBJECT
public:
    explicit MultMedia(QObject *parent = 0);

signals:

public slots:

private:
    bool m_supportAd;//是否支持广告图片
    bool m_supportVideo;//是否支持多媒体
    bool m_supportMusic;//是否支持背景音乐

//    QList<QRect> m_adPos;//位置信息
//    QList<QRect> m_videoPos;//
//    QList<QRect> m_musicPos;//


};

#endif // MULTMEDIA_H
