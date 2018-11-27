#include "ilistview.h"

#include <QAbstractScrollArea>
#include <QApplication>
#include <QBasicTimer>
#include <QEvent>
#include <QHash>
#include <QList>
#include <QMouseEvent>
#include <QScrollBar>
#include <QTime>

const int fingerAccuracyThreshold = 3;

struct FlickData {
    typedef enum {
        Steady, // Interaction without scrolling
        ManualScroll, // Scrolling manually with the finger on the screen
        AutoScroll, // Scrolling automatically
        AutoScrollAcceleration // Scrolling automatically but a finger is on the screen
    } State;
    State state;
    QWidget *widget;
    QPoint pressPos;
    QPoint lastPos;
    QPoint speed;
    QTime speedTimer;
    QList<QEvent*> ignored;
    QTime accelerationTimer;
    bool lastPosValid:1;
    bool waitingAcceleration:1;
    int rate;

    FlickData()
        : lastPosValid(false)
        , waitingAcceleration(false)
        , rate(1)
    {}


    void resetSpeed()
    {
        speed = QPoint();
        lastPosValid = false;
    }

    void updateSpeed(const QPoint &newPosition)
    {
        if (lastPosValid) {
            const int timeElapsed = speedTimer.elapsed();
            if (timeElapsed) {
                const QPoint newPixelDiff = (newPosition - lastPos);
                const QPoint pixelsPerSecond = newPixelDiff * (1000 / timeElapsed);
                // fingers are inacurates, we ignore small changes to avoid stopping the autoscroll because
                // of a small horizontal offset when scrolling vertically
                const int newSpeedY = (qAbs(pixelsPerSecond.y()) > fingerAccuracyThreshold) ? pixelsPerSecond.y() : 0;
                const int newSpeedX = (qAbs(pixelsPerSecond.x()) > fingerAccuracyThreshold) ? pixelsPerSecond.x() : 0;
                if (state == AutoScrollAcceleration) {
                    const int max = 4000; //4000; // px by seconds
                    const int oldSpeedY = speed.y();
                    const int oldSpeedX = speed.x();
                    if ((oldSpeedY <= 0 && newSpeedY <= 0) ||  (oldSpeedY >= 0 && newSpeedY >= 0)
                        && (oldSpeedX <= 0 && newSpeedX <= 0) ||  (oldSpeedX >= 0 && newSpeedX >= 0)) {
                        speed.setY(qBound(-max, (oldSpeedY + (newSpeedY / 4)), max));
                        speed.setX(qBound(-max, (oldSpeedX + (newSpeedX / 4)), max));
                    } else {
                        speed = QPoint();
                    }
                } else {
                    const int max =  2000; //2000; // px by seconds
                    // we average the speed to avoid strange effects with the last delta
                    if (!speed.isNull()) {
                        speed.setX(qBound(-max, (speed.x() / 4) + (newSpeedX * 3 / 4), max));
                        speed.setY(qBound(-max, (speed.y() / 4) + (newSpeedY * 3 / 4), max));
                    } else {
                        speed = QPoint(newSpeedX, newSpeedY);
                    }
                }
            }
        } else {
            lastPosValid = true;
        }
        speedTimer.start();
        lastPos = newPosition;
    }

    // scroll by dx, dy
    // return true if the widget was scrolled
    bool scrollWidget(const int dx, const int dy)
    {
        QAbstractScrollArea *scrollArea = qobject_cast<QAbstractScrollArea*>(widget);
        if (scrollArea) {
            const int x = scrollArea->horizontalScrollBar()->value();
            const int y = scrollArea->verticalScrollBar()->value();
            scrollArea->horizontalScrollBar()->setValue(x);
            if(rate == 0)
            {
                rate = 1;
            }
            scrollArea->horizontalScrollBar()->setValue(x - dx / rate);
            scrollArea->verticalScrollBar()->setValue(y - dy / rate);
            return (scrollArea->horizontalScrollBar()->value() != x
                    || scrollArea->verticalScrollBar()->value() != y);
        }

        return false;
    }

    bool scrollTo(const QPoint &newPosition)
    {
        const QPoint delta = newPosition - lastPos;
        updateSpeed(newPosition);
        return scrollWidget(delta.x(), delta.y());
    }
};

class FlickCharmPrivate
{
public:
    QHash<QWidget*, FlickData*> flickData;
    QBasicTimer ticker;
    QTime timeCounter;
    void startTicker(QObject *object)
    {
        if (!ticker.isActive())
            ticker.start(15, object);
        timeCounter.start();
    }
};

FlickCharm::FlickCharm(QObject *parent): QObject(parent)
{
    d = new FlickCharmPrivate;
}

FlickCharm::~FlickCharm()
{
    delete d;
}

void FlickCharm::activateOn(QWidget *widget, int rate)
{
    QAbstractScrollArea *scrollArea = qobject_cast<QAbstractScrollArea*>(widget);
    if (scrollArea) {
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        QWidget *viewport = scrollArea->viewport();

        viewport->installEventFilter(this);
        scrollArea->installEventFilter(this);

        d->flickData.remove(viewport);
        d->flickData[viewport] = new FlickData;
        d->flickData[viewport]->widget = widget;
        d->flickData[viewport]->state = FlickData::Steady;
        d->flickData[viewport]->rate = rate;

        return;
    }

//    qWarning() << "FlickCharm only works on QAbstractScrollArea (and derived classes)";
//    qWarning() << "or QWebView (and derived classes)";
}

void FlickCharm::deactivateFrom(QWidget *widget)
{
    QAbstractScrollArea *scrollArea = qobject_cast<QAbstractScrollArea*>(widget);
    if (scrollArea) {
        QWidget *viewport = scrollArea->viewport();

        viewport->removeEventFilter(this);
        scrollArea->removeEventFilter(this);

        delete d->flickData[viewport];
        d->flickData.remove(viewport);

        return;
    }
}

static QPoint deaccelerate(const QPoint &speed, const int deltatime)
{
    const int deltaSpeed = deltatime;

    int x = speed.x();
    int y = speed.y();
    x = (x == 0) ? x : (x > 0) ? qMax(0, x - deltaSpeed) : qMin(0, x + deltaSpeed);
    y = (y == 0) ? y : (y > 0) ? qMax(0, y - deltaSpeed) : qMin(0, y + deltaSpeed);
    return QPoint(x, y);
}

bool FlickCharm::eventFilter(QObject *object, QEvent *event)
{
    if (!object->isWidgetType())
        return false;

    const QEvent::Type type = event->type();

    QWidget *viewport = qobject_cast<QWidget*>(object);
    FlickData *data = d->flickData.value(viewport);

    switch (type) {
    case QEvent::MouseButtonPress:
    case QEvent::MouseMove:
        {
            if(data)
            {
                QAbstractScrollArea *scrollArea = (QAbstractScrollArea *)(data->widget);
                if (scrollArea)
                {
                    //scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
                    //scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
                }
            }
            break;
        }
    case QEvent::MouseButtonRelease:
        {
            if(data)
            {
                QAbstractScrollArea *scrollArea = (QAbstractScrollArea *)(data->widget);
                if (scrollArea)
                {
                    //scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                    //scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
                }
            }
            //qDebug() << "release";
            break;
        }
    case QEvent::MouseButtonDblClick: // skip double click
        return true;
    default:
        return false;
    }

    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
    if (type == QEvent::MouseMove && mouseEvent->buttons() != Qt::LeftButton)
        return false;

    if (mouseEvent->modifiers() != Qt::NoModifier)
        return false;

    if (!viewport || !data || data->ignored.removeAll(event))
        return false;

    const QPoint mousePos = mouseEvent->pos();
    bool consumed = false;
    switch (data->state)
    {
        case FlickData::Steady:
        {
            if (type == QEvent::MouseButtonPress)
            {
                //qDebug() << "mouse press";
                consumed = true;
                data->pressPos = mousePos;
            }
            else if (type == QEvent::MouseButtonRelease)
            {
                //qDebug() << "mouse release";
                consumed = true;
                QMouseEvent *event1 = new QMouseEvent(QEvent::MouseButtonPress,
                                                      data->pressPos, Qt::LeftButton,
                                                      Qt::LeftButton, Qt::NoModifier);
                QMouseEvent *event2 = new QMouseEvent(QEvent::MouseButtonRelease,
                                                      data->pressPos, Qt::LeftButton,
                                                      Qt::LeftButton, Qt::NoModifier);

                data->ignored << event1;
                data->ignored << event2;
                QApplication::postEvent(object, event1);
                QApplication::postEvent(object, event2);

                emit sFinished();
            }
            else if (type == QEvent::MouseMove)
            {
                //qDebug() << "mouse move";
                if((abs(mousePos.y() - data->pressPos.y()) < 20) && (abs(mousePos.x() - data->pressPos.x()) < 20))
                {
                    break;
                }
                consumed = true;
                data->scrollTo(mousePos);

                const QPoint delta = mousePos - data->pressPos;
                if (delta.x() > fingerAccuracyThreshold || delta.y() > fingerAccuracyThreshold)
                    data->state = FlickData::ManualScroll;
            }
            break;
        }
        case FlickData::ManualScroll:
        {
            if (type == QEvent::MouseMove)
            {
                //qDebug() << "manual mouse move";
                consumed = true;
                data->scrollTo(mousePos);
            }
            else if (type == QEvent::MouseButtonRelease)
            {
                //qDebug() << "manual mouse release";
                consumed = true;
                data->state = FlickData::AutoScroll;
                data->lastPosValid = false;
                d->startTicker(this);
            }
            break;
        }
        case FlickData::AutoScroll:
        {
            if (type == QEvent::MouseButtonPress)
            {
                //qDebug() << "auto mouse press";
                consumed = true;
                data->state = FlickData::AutoScrollAcceleration;
                data->waitingAcceleration = true;
                data->accelerationTimer.start();
                data->updateSpeed(mousePos);
                data->pressPos = mousePos;
            }
            else if (type == QEvent::MouseButtonRelease)
            {
                //qDebug() << "auto mouse release";
                consumed = true;
                data->state = FlickData::Steady;
                data->resetSpeed();
            }
            break;
        }
        case FlickData::AutoScrollAcceleration:
        {
            if (type == QEvent::MouseMove)
            {
                //qDebug() << "auto acce mouse move";
                consumed = true;
                data->updateSpeed(mousePos);
                data->accelerationTimer.start();
                if (data->speed.isNull())
                    data->state = FlickData::ManualScroll;
            }
            else if (type == QEvent::MouseButtonRelease)
            {
                //qDebug() << "auto acce mouse release";
                consumed = true;
                data->state = FlickData::AutoScroll;
                data->waitingAcceleration = false;
                data->lastPosValid = false;
            }
            break;
        }
        default:
        {
            //qDebug() << "FlickData:NONE";
            break;
        }
    }
    data->lastPos = mousePos;
    if(type == QEvent::MouseButtonPress)
    {
        return false;
    }
    return true;
}

void FlickCharm::timerEvent(QTimerEvent *event)
{
    int count = 0;
    QHashIterator<QWidget*, FlickData*> item(d->flickData);
    while (item.hasNext())
    {
        item.next();
        FlickData *data = item.value();
        if (data->state == FlickData::AutoScrollAcceleration
            && data->waitingAcceleration
            && data->accelerationTimer.elapsed() > 40) {
            data->state = FlickData::ManualScroll;
            data->resetSpeed();
        }
        if (data->state == FlickData::AutoScroll || data->state == FlickData::AutoScrollAcceleration)
        {
            const int timeElapsed = d->timeCounter.elapsed();
            const QPoint delta = (data->speed) * timeElapsed / 1000;
            bool hasScrolled = data->scrollWidget(delta.x(), delta.y());

            if (data->speed.isNull() || !hasScrolled)
                data->state = FlickData::Steady;
            else
                count++;
            data->speed = deaccelerate(data->speed, timeElapsed);
        }
    }

    if (!count)
    {
        d->ticker.stop();
        emit sFinished();
    }
    else
    {
        d->timeCounter.start();
    }

    QObject::timerEvent(event);
}

ItemScrollContent::ItemScrollContent(QWidget *parent) : QWidget(parent)
{
    mButtonMode = true;
    mFont[0] = QFont(QString("Arial"), 12, QFont::Bold, false);
    mFont[1] = QFont(QString("Arial"), 9, QFont::Normal, false);
    mColor[0] = Qt::white;
    mColor[1] = QColor(220,220,220,250);
    mFrameColor = QColor(22,24,23,250);
    mBgColor[0] = QColor(22,24,23,250);
    mBgColor[1] = QColor(44,44,44,250);
}

ItemScrollContent::~ItemScrollContent()
{

}

void ItemScrollContent::Highlight(QPainter *p, QString pString, QRect pRect, bool pEnable)
{
    if(!p->isActive())
        return;
    if(pEnable)
    {
        if(mButtonMode)
        {
            p->save();
            if(!mImage[0].isNull())
            {
                p->drawPixmap(pRect, mImage[0]);
            }
            else
            {
                if(!mBgImg[0].isNull())
                    p->drawPixmap(pRect, mBgImg[0]);
                else
                {
                    p->setPen(mFrameColor);
                    p->setBrush(mBgColor[0]);
                    p->drawRect(pRect.adjusted(-D_ITEM_LINE_LEN,-D_ITEM_LINE_LEN,D_ITEM_LINE_LEN,0));
                }
                p->setFont(mFont[0]);
                p->setPen(mColor[0]);
                p->drawText(pRect, Qt::AlignCenter, pString);
            }
            p->restore();
        }
        else  //非Button模式，不会使用高亮色
        {
            p->save();
            if(!mImage[1].isNull())
            {
                p->drawPixmap(pRect, mImage[1]);
            }
            else
            {
                if(!mBgImg[1].isNull())
                    p->drawPixmap(pRect, mBgImg[1]);
                else
                {
                    p->setPen(mFrameColor);
                    p->setBrush(mBgColor[1]);
                    p->drawRect(pRect.adjusted(-D_ITEM_LINE_LEN,-D_ITEM_LINE_LEN,D_ITEM_LINE_LEN,0));
                }
                p->setFont(mFont[0]);
                p->setPen(mColor[0]);
                p->drawText(pRect, Qt::AlignCenter, pString);
            }
            p->restore();
        }
    }
    else
    {
        p->save();
        if(!mImage[1].isNull())
        {
            p->drawPixmap(pRect, mImage[1]);
        }
        else
        {
            if(!mBgImg[1].isNull())
                p->drawPixmap(pRect, mBgImg[1]);
            else
            {
                p->setPen(mFrameColor);
                p->setBrush(mBgColor[1]);
                p->drawRect(pRect.adjusted(-D_ITEM_LINE_LEN,-D_ITEM_LINE_LEN,D_ITEM_LINE_LEN,0));
            }
            p->setFont(mFont[1]);
            p->setPen(mColor[1]);
            p->drawText(pRect, Qt::AlignCenter, pString);
        }
        p->restore();
    }
}

void ItemScrollContent::slot_Clicked(int pIndex)
{
    SetCurrentIndex(pIndex);
}

void ItemScrollContent::SetCurrentIndex(int pIndex, bool pSync)
{
    if(pIndex >= 0 && pIndex < mStringList.count())
    {
        if(pSync)
            emit sClicked(pIndex);
        if(pIndex == mIndex)
            return;
        QPainter p(&mBuffer);
        Highlight(&p, mStringList.at(mIndex), mRectList.at(mIndex), false);
        Highlight(&p, mStringList.at(pIndex), mRectList.at(pIndex), true);
        mIndex = pIndex;
        p.end();
        mBuffer2 = QPixmap::fromImage(mBuffer);
        update();
    }
}

void ItemScrollContent::mousePressEvent(QMouseEvent *e)
{
    if(mButtonMode)
        mPressPos = e->pos();
    QWidget::mousePressEvent(e);
}

void ItemScrollContent::mouseMoveEvent(QMouseEvent *e)
{
    QWidget::mouseMoveEvent(e);
}

void ItemScrollContent::mouseReleaseEvent(QMouseEvent *e)
{
    if(mButtonMode)
    {
        mReleasePos = e->pos();
        QLine tmpLine(mPressPos, mReleasePos);
        if(qAbs(tmpLine.dy()) < 10)  //点击，而非滑动
        {
            int index = mReleasePos.y() / mItemHeight;
            SetCurrentIndex(index);
        }
    }
    QWidget::mouseReleaseEvent(e);
}

void ItemScrollContent::Init(QStringList pList, int pIndex)
{
    mIndex = pIndex;
    mStringList = pList;
    int totalHeight = mItemHeight*mStringList.count();
    setFixedHeight(totalHeight);
    //>@创建一级缓冲
    mRectList.clear();
    QSize tmpSize = QSize(width(), totalHeight);
    mBuffer = TransparentImage(tmpSize);
    QPainter p(&mBuffer);
    for(int i=0; i<mStringList.count(); i++)
    {
        QString tmpString = mStringList.at(i);
        QRect tmpRect = QRect(QPoint(0,i*mItemHeight), QSize(width(), mItemHeight));
        mRectList.append(tmpRect);
        if(mIndex == i)
            Highlight(&p, tmpString, tmpRect, true);
        else
            Highlight(&p, tmpString, tmpRect, false);
    }
    p.end();
    mBuffer2 = QPixmap::fromImage(mBuffer);
    update();
}

void ItemScrollContent::Resize(QSize pSize)
{
    if(!mBuffer.isNull())
    {
        QSize tmpSize = mBuffer.size();
        if(pSize == tmpSize)
            return;
    }
    //>@创建一级缓冲
    mRectList.clear();
    QSize tmpSize = pSize;
    mBuffer = TransparentImage(tmpSize);
    QPainter p(&mBuffer);
    for(int i=0; i<mStringList.count(); i++)
    {
        QString tmpString = mStringList.at(i);
        QRect tmpRect = QRect(QPoint(0,i*mItemHeight), QSize(tmpSize.width(), mItemHeight));
        mRectList.append(tmpRect);
        if(mIndex == i)
            Highlight(&p, tmpString, tmpRect, true);
        else
            Highlight(&p, tmpString, tmpRect, false);
    }
    p.end();
    mBuffer2 = QPixmap::fromImage(mBuffer);
    update();
}

void ItemScrollContent::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    p.drawPixmap(QPoint(0,0), mBuffer2);
}

ItemScrollArea::ItemScrollArea(QWidget *parent) : QScrollArea(parent)
{
    mWidget = 0;
    mFlickCharm.activateOn(this);

    mSelectorMode = false;
}

ItemScrollArea::~ItemScrollArea()
{

}

void ItemScrollArea::Init()
{
    if(widget())
    {
        mWidget = static_cast<ItemScrollContent*>(widget());
        if(!mWidget)
        {
            QWidget *tmpWidget = takeWidget();
            tmpWidget->deleteLater();
        }
    }
    if(!mWidget)
    {
        mWidget = new ItemScrollContent(this);
        setWidget(mWidget);
    }
    connect(this, SIGNAL(sClicked(int)), mWidget, SLOT(slot_Clicked(int)));
}

void ItemScrollArea::Init(QStringList pList, int pIndex)
{
    mWidget->Init(pList, pIndex);
    verticalScrollBar()->setRange(0, (pList.count()-1) * mWidget->mItemHeight);
    if(mSelectorMode)
    {
        int tmpValue = (pIndex-1) * mWidget->mItemHeight;
        verticalScrollBar()->setValue(tmpValue);
    }
}

void ItemScrollArea::resizeEvent(QResizeEvent *e)
{
    if(mWidget)
    {
        if(!verticalScrollBar()->isVisible())
        {
            mWidget->setFixedWidth(e->size().width());
            mWidget->Resize(QSize(e->size().width(), mWidget->height()));
        }
    }
    QScrollArea::resizeEvent(e);
}

void ItemScrollArea::slot_ScrollFinished()
{
    if(mSelectorMode)
    {
        int y = verticalScrollBar()->value();
        int diff = y % mWidget->mItemHeight;
        int index = y / mWidget->mItemHeight;
        verticalScrollBar()->setValue(y - diff);
        if(diff)
        {
            emit sClicked(index+1);
        }
        else
        {
            emit sClicked(index+1);
        }
    }
}

void ItemScrollArea::slot_Clicked(int pIndex)
{
    emit sItemClicked(pIndex);
}

QString ItemScrollArea::GetCurrentText()
{
    return mWidget->mStringList.at(mWidget->mIndex);
}

int ItemScrollArea::GetCurrentIndex()
{
    return mWidget->mIndex;
}

void ItemScrollArea::SetCurrentIndex(int pIndex)
{
    mWidget->SetCurrentIndex(pIndex, false);
    if(mSelectorMode)
    {
        verticalScrollBar()->setValue((pIndex-1) * mWidget->mItemHeight);
    }
}

void ItemScrollArea::SetFlag(quint32 pFlag, QVariant pPara)
{
    if(pFlag == FLAG_SELECTOR)
    {
        mSelectorMode = pPara.toBool();
        if(mSelectorMode)
        {
            //限制View大小为三倍Item大小
            setFixedHeight(mWidget->mItemHeight*3);
            connect(&mFlickCharm, SIGNAL(sFinished()), this, SLOT(slot_ScrollFinished()));
        }
        else
        {
            disconnect(&mFlickCharm, SIGNAL(sFinished()), this, SLOT(slot_ScrollFinished()));
        }
    }
    else if(pFlag == FLAG_ITEMBUTTON)
    {
        mWidget->mButtonMode = pPara.toBool();
        if(mWidget->mButtonMode)
        {
            connect(mWidget, SIGNAL(sClicked(int)), this, SLOT(slot_Clicked(int)));
        }
        else
        {
            disconnect(mWidget, SIGNAL(sClicked(int)), this, SLOT(slot_Clicked(int)));
        }
    }
    else if(pFlag == FLAG_ITEMHEIGHT)
    {
        mWidget->mItemHeight = pPara.toInt();
    }
    else if(pFlag == FLAG_FOCUS_FONTTYPE)
    {
        mWidget->mFont[0].setFamily(pPara.toString());
    }
    else if(pFlag == FLAG_UNFOCUS_FONTTYPE)
    {
        mWidget->mFont[1].setFamily(pPara.toString());
    }
    else if(pFlag == FLAG_FOCUS_FONTSIZE)
    {
        mWidget->mFont[0].setPointSize(pPara.toInt());
    }
    else if(pFlag == FLAG_UNFOCUS_FONTSIZE)
    {
        mWidget->mFont[1].setPointSize(pPara.toInt());
    }
    else if(pFlag == FLAG_RECT_COLOR)
    {
        mWidget->mFrameColor = (Qt::GlobalColor)(pPara.toInt());
    }
    else if(pFlag == FLAG_FOCUS_COLOR)
    {
        mWidget->mColor[0] = (Qt::GlobalColor)(pPara.toInt());
    }
    else if(pFlag == FLAG_UNFOCUS_COLOR)
    {
        mWidget->mColor[1] = (Qt::GlobalColor)(pPara.toInt());
    }
    else if(pFlag == FLAG_FOCUS_BGCOLOR)
    {
        mWidget->mBgColor[0] = (Qt::GlobalColor)(pPara.toInt());
    }
    else if(pFlag == FLAG_UNFOCUS_BGCOLOR)
    {
        mWidget->mBgColor[1] = (Qt::GlobalColor)(pPara.toInt());
    }
    else if(pFlag == FLAG_FOCUS_BGIMG)
    {
        mWidget->mBgImg[0] = pPara.value<QPixmap>();
    }
    else if(pFlag == FLAG_UNFOCUS_BGIMG)
    {
        mWidget->mBgImg[1] = pPara.value<QPixmap>();
    }
}

TextTerminal::TextTerminal(QWidget *parent) :
    QWidget(parent), UiClass()
{
    mFont = QFont(QString("Arial"), 12, QFont::Bold);

    mColor[ML_INFO] = Qt::white;
    mColor[ML_WARN] = Qt::green;
    mColor[ML_ERROR] = Qt::red;

    mMaxCount = 100;
    mMaxRows = 3;

    m_ShowTitle = true;
    m_Title = QString("Title");
}

TextTerminal::~TextTerminal()
{
    foreach(TEXT_INFO *tmpTextInfo, mTextList)
    {
        if(tmpTextInfo)
            free(tmpTextInfo);
    }
    mTextList.clear();
}

void TextTerminal::UpdatePara(QSize pSize)
{
    //背景更新
    m_BgRect = QRect(QPoint(0,0), pSize);

    QLinearGradient tmpBgGradient(m_BgRect.topLeft(), m_BgRect.bottomLeft());
    tmpBgGradient.setColorAt(0, QColor(54,183,251,180));
    tmpBgGradient.setColorAt(0.4, QColor(17,116,192,180));
    tmpBgGradient.setColorAt(0.5, QColor(5, 50, 171, 240));
    tmpBgGradient.setColorAt(0.6, QColor(17,116,192, 180));
    tmpBgGradient.setColorAt(1, QColor(54,183,251,180));//设置渐变的颜色和路径比例
    m_BgBrush = QBrush(tmpBgGradient);
    m_BgPenColor = QColor(54,183,251,180);

    m_FgRect = m_BgRect.adjusted(4,4,-4,-4);
    m_FgBrush = QBrush(QColor(1, 12, 24, 255));
    m_FgPenColor = QColor(13,37,68,255);

    int x = m_FgRect.x();
    int y = m_FgRect.y();
    int w = m_FgRect.width();
    int h = m_FgRect.height();
    int tmpW = w*6/5.0;
    m_BGlowRect = QRect(x-w/10.0, y+h-w/5.0, tmpW, tmpW);
    QRadialGradient tmpBGlowGradient(m_BGlowRect.x()+m_BGlowRect.width()/2,
                                   m_BGlowRect.y()+m_BGlowRect.height()/2,
                                   m_BGlowRect.width()/2);
    tmpBGlowGradient.setColorAt(0,QColor(240, 238, 250, 80));
    tmpBGlowGradient.setColorAt(1.0,QColor(245,245,255,0));//设置渐变的颜色和路径比例
    m_BGlowBrush = QBrush(tmpBGlowGradient);

    m_TGlassRect = QRect(x, y+25, w, 35);
    QLinearGradient tmpTGlassGradient(m_TGlassRect.topLeft(), m_TGlassRect.bottomLeft());
    tmpTGlassGradient.setColorAt(0, QColor(79, 120, 165, 200));
    tmpTGlassGradient.setColorAt(0.45, QColor(55, 92, 136, 200));
    tmpTGlassGradient.setColorAt(0.46, QColor(20, 62, 117, 200));
    tmpTGlassGradient.setColorAt(1, QColor(13,91,173,200));//设置渐变的颜色和路径比例
    m_TGlassBrush = QBrush(tmpTGlassGradient);

    QLinearGradient tmpTGlassGradient2(m_TGlassRect.topLeft(), m_TGlassRect.topRight());
    tmpTGlassGradient2.setColorAt(0, QColor(1, 12, 24, 255));
    tmpTGlassGradient2.setColorAt(0.4, QColor(1, 12, 24, 40));
    tmpTGlassGradient2.setColorAt(0.5, QColor(1, 12, 24, 0));
    tmpTGlassGradient2.setColorAt(0.6, QColor(1, 12, 24, 40));
    tmpTGlassGradient2.setColorAt(1, QColor(1, 12, 24, 255));//设置渐变的颜色和路径比例
    m_TGlassBrush2 = QBrush(tmpTGlassGradient2);

    m_LightRect = QRect(x, y+260, w, 80);
    m_LightBrush = QBrush(QColor(255,199,0,120));

    m_TitleFont = QFont("Arial", 15, QFont::DemiBold);
    m_TitleColor = QColor(221, 222, 223, 255);

    if(m_ShowTitle)
        mTextRect = m_FgRect.adjusted(20, 70, -20, -10);
    else
        mTextRect = m_FgRect.adjusted(20, 20, -20, -10);

    //计算行数
    QFontMetricsF fm(mFont);
    int tmpHeight = fm.height();
    mDiffHeight = tmpHeight / 4;
    mItemHeight = mDiffHeight * 6;
    int tmpTotalHeight = mTextRect.height();
    mMaxRows = (tmpTotalHeight+mDiffHeight) / mItemHeight;
}

void TextTerminal::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    UpdatePara(e->size());
    Refresh();
}

void TextTerminal::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.drawPixmap(0,0,QPixmap::fromImage(mBuffer));
}

void TextTerminal::Refresh()
{
    if(mTextList.isEmpty())
        return;

    if(mImage.isNull() || mImage.size() != size())
    {
        mImage = TransparentImage(size());

        QPainter p(&mImage);
        p.setRenderHint(QPainter::SmoothPixmapTransform, true);
        p.setRenderHint(QPainter::Antialiasing);

        p.setBrush(m_BgBrush);
        p.setPen(m_BgPenColor);
        int round1 = 15;
        int round2 = 15;
        p.drawRoundedRect(m_BgRect, round1, round2, Qt::AbsoluteSize);

        p.setBrush(m_FgBrush);
        p.setPen(m_FgPenColor);
        round1 = 12;
        round2 = 12;
        p.drawRoundedRect(m_FgRect, round1, round2, Qt::AbsoluteSize);

        if(m_ShowTitle)
        {
            //>@绘制玻璃效果
            p.setPen(Qt::NoPen);
            p.setBrush(m_TGlassBrush);
            p.drawRect(m_TGlassRect);
            p.setBrush(m_TGlassBrush2);
            p.drawRect(m_TGlassRect);

            //>@绘制文字
            p.setFont(m_TitleFont);
            p.setBrush(m_TitleColor);
            p.setPen(m_TitleColor);
            p.drawText(m_TGlassRect, Qt::AlignHCenter|Qt::AlignVCenter, m_Title);
        }

        //>@绘制底部光晕
        p.setBrush(m_BGlowBrush);
        p.setPen(Qt::NoPen);
        p.drawEllipse(m_BGlowRect);//在相应的坐标画出来

        p.end();
    }

    mBuffer = TransparentImage(size());

    QPainter p(&mBuffer);
    p.setRenderHint(QPainter::SmoothPixmapTransform, true);
    p.setRenderHint(QPainter::Antialiasing);

    p.drawImage(rect(), mImage);
    int tmpStart = 0;
    if(mTextList.count() > mMaxRows)
        tmpStart = mTextList.count() - mMaxRows;
    p.setFont(mFont);
    for(int i=tmpStart; i<mTextList.count(); i++)
    {
        TEXT_INFO *tmpInfo = mTextList.at(i);
        p.save();
        p.setPen(mColor[tmpInfo->mLevel]);
        if(!tmpInfo->mText.isEmpty())
        {
            QString tmpMsg;
            if(tmpInfo->mCount > 1)
                tmpMsg = QString("%1--[%])").arg(tmpInfo->mText).arg(tmpInfo->mCount);
            else
                tmpMsg = tmpInfo->mText;
            p.drawText(QRect(mTextRect.left(), mTextRect.top() + mItemHeight*(i-tmpStart), mTextRect.width(), mItemHeight), tmpMsg);
        }
        p.restore();
    }
    p.end();

    update();
}

void TextTerminal::SetFlag(quint32 pFlag, QVariant pPara)
{
    if(pFlag == FLAG_TITLE_EN)
    {
        bool tmpEnable = pPara.toBool();
        if(m_ShowTitle != tmpEnable)
        {
            m_ShowTitle = tmpEnable;
            UpdatePara(size());
            Refresh();
        }
    }
    else if(pFlag == FLAG_FONT_SIZE)
    {
        int tmpSize = pPara.toInt();
        if(tmpSize != mFont.pointSize())
        {
            mFont.setPointSize(tmpSize);
            //计算行数
            QFontMetricsF fm(mFont);
            int tmpHeight = fm.height();
            mDiffHeight = tmpHeight / 4;
            mItemHeight = mDiffHeight * 6;
            int tmpTotalHeight = height();
            mMaxRows = tmpTotalHeight / mItemHeight;
            Refresh();
        }
    }
    else if(pFlag == FLAG_FONT_FAMILY)
    {
        QString tmpFamily = pPara.toString();
        if(tmpFamily.compare(mFont.family(), Qt::CaseInsensitive))
        {
            mFont.setFamily(tmpFamily);
            Refresh();
        }
    }
    else if(pFlag == FLAG_FONT_WEIGHT)
    {
        int tmpSize = pPara.toInt();
        if(tmpSize != mFont.weight())
        {
            mFont.setWeight(tmpSize);
            Refresh();
        }
    }
    else if(pFlag == FLAG_INFO_COLOR)
    {
        int tmpSize = pPara.toInt();
        if(mColor[ML_INFO] != tmpSize)
        {
            mColor[ML_INFO] = (Qt::GlobalColor)tmpSize;
            Refresh();
        }
    }
    else if(pFlag == FLAG_WARN_COLOR)
    {
        int tmpSize = pPara.toInt();
        if(mColor[ML_WARN] != tmpSize)
        {
            mColor[ML_WARN] = (Qt::GlobalColor)tmpSize;
            Refresh();
        }
    }
    else if(pFlag == FLAG_ERROR_COLOR)
    {
        int tmpSize = pPara.toInt();
        if(mColor[ML_ERROR] != tmpSize)
        {
            mColor[ML_ERROR] = (Qt::GlobalColor)tmpSize;
            Refresh();
        }
    }
}

void TextTerminal::Append(E_MSG_LEVEL pLevel, QString pText)
{
    TEXT_INFO *tmpText = 0;
    if(!mTextList.isEmpty())
    {
        tmpText = mTextList.last();
        if(tmpText->mLevel == pLevel && !tmpText->mText.compare(pText))
        {
            tmpText->mCount++;
            return;
        }
        if(mTextList.count() >= mMaxCount)
        {
            tmpText = mTextList.takeFirst();
            delete tmpText;
            tmpText = 0;
        }
    }
    tmpText = new TEXT_INFO;
    tmpText->mLevel = pLevel;
    tmpText->mText = pText;
    tmpText->mCount = 1;
    mTextList.append(tmpText);
    Refresh();
}

void TextTerminal::Replace(int pIndex, QString pText)
{
    if(mTextList.isEmpty())
        return;
    TEXT_INFO *tmpText = mTextList.last();
    if(pIndex > tmpText->mText.count())
        return;
    QString tmpLeftText = tmpText->mText.left(pIndex);
    tmpText->mText = tmpLeftText + pText;
    Refresh();
}


