#include "waveform.h"
#include "ui_waveform.h"

WaveForm::WaveForm(QWidget *parent)
    :   QWidget(parent)
    ,   ui(new Ui::WaveForm)
    ,   m_Audiocodec(0)
{
    ui->setupUi(this);
    InitCodec();

    m_PointFs.clear();
}

WaveForm::~WaveForm()
{
    IDE_TRACE_STR("~WaveForm");
    delete ui;
    ReleaseCodec();
}

void WaveForm::InitCodec()
{
    m_Audiocodec = new baseAudiocodec(this);

    connect(m_Audiocodec, SIGNAL(sLevel(qreal, qreal, qint64)), this, SLOT(slot_Level(qreal, qreal, qint64)));
    connect(m_Audiocodec, SIGNAL(sFD(double*, quint32, quint32)), this, SLOT(slot_FD(double*, quint32, quint32)));
}

void WaveForm::ReleaseCodec()
{
    if(m_Audiocodec != 0)
    {
        disconnect(m_Audiocodec, SIGNAL(sLevel(qreal, qreal, qint64)), 0, 0);
        disconnect(m_Audiocodec, SIGNAL(sFD(double*, quint32, quint32)), 0, 0);
        delete m_Audiocodec;
        m_Audiocodec = 0;
    }
}

void WaveForm::PlayFile(QString pSrc)
{
    if(m_Audiocodec != 0)
    {
        m_Audiocodec->PlayFile(pSrc);
    }
}

void WaveForm::RecordToMem(quint32 pRecordSeconds)
{
    if(m_Audiocodec != 0)
    {
        m_Audiocodec->RecordToMem(pRecordSeconds);
    }
}

void WaveForm::CreatToMem(quint32 pCnt, quint32 pTimeMs)
{
    if(m_Audiocodec != 0)
    {
        m_Audiocodec->CreatToMem(pCnt, pTimeMs);
    }
}

void WaveForm::Play()
{
    if(m_Audiocodec != 0)
    {
        m_Audiocodec->Play();
    }
}

void WaveForm::MemSave(QString pDst)
{
    if(m_Audiocodec != 0)
    {
        m_Audiocodec->MemSave(pDst);
    }
}

void WaveForm::PaintTFD()
{
    mImage = QImage(this->rect().width(),this->rect().height(),QImage::Format_ARGB32_Premultiplied);
    if(mImage.isNull())
        return;
    mImage.fill(Qt::transparent); //>@创建透明图层
    QPainter p(&(mImage));
    p.setRenderHint(QPainter::Antialiasing, true);

    if(m_PointFs.isEmpty())
        return;
    p.save();
    p.setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::SquareCap));
    //>@用于将时域图整体上移20，这样效果更好看
    p.translate(0, -20);
    p.drawPolyline(m_PointFs);
    p.restore();

    p.save();
    p.setPen(QPen(Qt::yellow, 5, Qt::DashLine, Qt::SquareCap));
    p.drawLine(QPointF(0,(this->rect().height()/2)),QPointF((this->rect().width()),(this->rect().height()/2)));
    p.restore();

    if(m_RectFs.isEmpty())
        return;
    QTransform transform;
    p.save();
    static int i = 0;
    transform.rotate(180, Qt::XAxis);
    transform.translate(0, -(this->rect().height()*3/2));
    p.setTransform(transform);
    QLinearGradient linearGradient(0, m_RectFs.at(0).top(), 0, this->rect().bottom());
    linearGradient.setSpread(QLinearGradient::RepeatSpread);
    linearGradient.setColorAt(0.0, Qt::darkBlue);
    linearGradient.setColorAt(0.125, Qt::blue);
    linearGradient.setColorAt(0.25, Qt::darkGreen);
    linearGradient.setColorAt(0.375, Qt::green);
    linearGradient.setColorAt(0.5, Qt::darkYellow);
    linearGradient.setColorAt(0.625, Qt::yellow);
    linearGradient.setColorAt(0.75, Qt::darkRed);
    linearGradient.setColorAt(0.875, Qt::red);
    p.setBrush(QBrush(linearGradient));
//    p.setPen(QPen(Qt::darkBlue, 3, Qt::SolidLine, Qt::SquareCap));
    p.setPen(QPen(QBrush(linearGradient), 3));
    p.drawRects(m_RectFs);
    p.restore();
    update();
}

void WaveForm::paintEvent( QPaintEvent * event)
{ 
    QPainter p(this);
    p.fillRect(this->rect(), Qt::black);
    if(mImage.isNull())
        return;
    QPixmap pixmap = QPixmap::fromImage(mImage.scaled(size(), Qt::KeepAspectRatio) );
    p.drawPixmap(this->rect(), pixmap, this->rect());
}

void WaveForm::slot_Level(qreal pRms, qreal pPeak, qint64 pTimeline)
{
    static QList<qreal> tmpYList;
    qreal yRms = (1.0 - pRms) * this->rect().height() / 2;
    tmpYList.append(yRms);
    int cnt=tmpYList.count();
    if(cnt>=this->rect().width())
    {
        tmpYList.removeFirst();
        cnt -= 1;
    }
    m_PointFs.clear();
    for(int i=0;i<cnt-1;i++)
    {
        QPointF pointF = QPointF((qreal)i, tmpYList.at(cnt-1-i));
        m_PointFs.append(pointF);
    }
    static int i = 0;
    i++;
    if(i%3 == 0)
        PaintTFD();
}

void WaveForm::slot_FD(double *pFDRms, quint32 pGrade, quint32 pFrq)
{
    qreal yScale = qreal(this->rect().height())/1.0;
    qreal xGrade = qreal(this->rect().width())/pGrade;
    m_RectFs.clear();
    const quint8 columnNum = pGrade / 2 + 1;
    const qreal scale = (qreal)pGrade / (qreal)columnNum;
    for(int i=0;i<columnNum;i++)
    {
        qreal left = xGrade*i*scale + 5;
        qreal top = yScale/2.0;
        qreal width = xGrade*scale - 10;
        qreal height = qMin((qreal)pFDRms[i]*yScale/3.0,yScale/2.0);
        QRectF tmpRectF = QRectF(left,top,width,height);
        m_RectFs.append(tmpRectF);
    }
}

