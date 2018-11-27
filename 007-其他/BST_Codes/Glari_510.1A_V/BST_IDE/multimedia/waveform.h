#ifndef WAVEFORM_H
#define WAVEFORM_H

#include "baseaudiocodec.h"
#include <QTimer>
#include <QQueue>
#include <QPainter>
#include <QPalette>
#include <QGradient>
#include <QWidget>

namespace Ui {
class WaveForm;
}

class WaveForm : public QWidget
{
    Q_OBJECT   
public:
    WaveForm(QWidget *parent = 0);
    ~WaveForm();
    void InitCodec();
    void ReleaseCodec();

    void PlayFile(QString pSrc);
    void RecordToMem(quint32 pRecordSeconds);
    void CreatToMem(quint32 pCnt, quint32 pTimeMs);
    void Play();
    void MemSave(QString pDst);

    void PaintTFD();
public:
    baseAudiocodec          *m_Audiocodec;
    QVector<QPointF>        m_PointFs;
    QVector<QRectF>         m_RectFs;
    QImage                  mImage;
public slots:
    void slot_Level(qreal pRms, qreal pPeak, qint64 pTimeline);
    void slot_FD(double *pFDRms, quint32 pGrade, quint32 pFrq);
protected:
    void paintEvent(QPaintEvent *event);
private:
    Ui::WaveForm *ui;
};

#endif // WAVEFORM_H
