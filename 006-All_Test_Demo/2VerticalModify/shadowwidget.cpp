#include <QtGui/QtGui>
#include <QtCore/QtCore>
#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QGraphicsDropShadowEffect>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>

class ShadowWidget : public QWidget
{
    Q_OBJECT
public:
    ShadowWidget(QWidget *parent =0)
    {
        //setWindowFlags(Qt::FramelessWindowHint);
        QGraphicsDropShadowEffect *fEffect =  new QGraphicsDropShadowEffect;
        //fEffect->setColor(QColor(255,0,0));
        QVBoxLayout *layout = new QVBoxLayout(this);
        QGraphicsScene *scene = new QGraphicsScene;
        scene->setSceneRect(QRect(0,0,900,600));
        fPixmap= new QGraphicsPixmapItem;
        fPixmap->setPos(200,200);
        QPixmap *pix = new QPixmap("1.jpg");
        fPixmap->setPixmap(*pix);
        //fPixmap->setGraphicsEffect(fEffect);
        QTransform *tran = new QTransform;
        tran->rotate(80,Qt::YAxis);
        //fPixmap->setTransform(*tran);
        //fPixmap->setTransformOriginPoint(QPointF(pix->size().width()/2,pix->size().height()/2));
        //fPixmap->setTransformOriginPoint(100,100);
        scene->addItem(fPixmap);
        QGraphicsView *view = new QGraphicsView(scene);
        layout->addWidget(view);
        QPushButton * animButton = new QPushButton(tr("Start"));
        layout->addWidget(animButton);
        connect(animButton,SIGNAL(clicked()),this,SLOT(performAnimation()));
        //setGraphicsEffect(fEffect);
        resize(900,600);
        fTimeLine = NULL;
    }

private slots:
    void performAnimation()
    {
        if(!fTimeLine)
        {
            fTimeLine = new QTimeLine;
            fTimeLine->setDuration(1000);
            fTimeLine->setFrameRange(1,10);
            connect(fTimeLine,SIGNAL(frameChanged(int)),this,SLOT(performRotateAnimation(int )));
        }

        fTimeLine->start();
    }

    void performRotateAnimation(int frame)
    {
        qreal angle = 90*(frame)/10.0;
        QTransform tran;
        tran.translate(150,0);
        //fPixmap->setTransformOriginPoint(0,0);
        tran.rotate(angle,Qt::YAxis);
        fPixmap->setTransform(tran);
        QTransform t;
        t.translate(-150,0);
        fPixmap->setTransform(t,true);
    }

private:
    QGraphicsPixmapItem *fPixmap;
    QTimeLine *fTimeLine ;
};
