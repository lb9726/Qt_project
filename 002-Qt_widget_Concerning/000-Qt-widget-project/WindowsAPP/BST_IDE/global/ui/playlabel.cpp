#include "playlabel.h"

PlayLabel::PlayLabel(QWidget *parent) :
    QLabel(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setPixmap(QPixmap(":/Page/rc/Page/VideoScreen.png"));
}

PlayLabel::~PlayLabel()
{

}

void PlayLabel::SetEvnRcver(QWidget *pEvnRcver)
{
    mQesp = pEvnRcver;
}

void PlayLabel::mouseDoubleClickEvent(QMouseEvent* ev)
{
//    QLabel::mouseDoubleClickEvent(ev);
    QMouseEvent *DBClickEvn = new QMouseEvent
            (QEvent::MouseButtonDblClick,
             QPoint(10,10),
             Qt::LeftButton,
             Qt::LeftButton,
             Qt::NoModifier);
    QApplication::sendEvent(mQesp, DBClickEvn);
}

void PlayLabel::keyPressEvent(QKeyEvent *ev)
{
    switch(ev->key())
    {
        case Qt::Key_Escape:
            QMouseEvent *DBClickEvn = new QMouseEvent
                    (QEvent::MouseButtonDblClick,
                     QPoint(10,10),
                     Qt::LeftButton,
                     Qt::LeftButton,
                     Qt::NoModifier);
            QApplication::sendEvent(mQesp, DBClickEvn);
            break;
    }
}
