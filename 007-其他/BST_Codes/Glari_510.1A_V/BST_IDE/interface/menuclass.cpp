#include "menuclass.h"

MenuItem::MenuItem(QWidget *parent) :
    QWidget(parent)
{
    mIsChanged = false;
    mType = -1;
    mFocus = false;
    mBgBrush.setColor(QColor(44, 44, 44));
    mLineBrush.setColor(QColor(53, 54, 53));
}

MenuItem::~MenuItem()
{
}

void MenuItem::SetComponent(int pType, QWidget *pWidget)
{
    mComponents.insert(pType, pWidget);
}

void MenuItem::SetFocus(bool pFlag)
{
    if(mFocus != pFlag)
    {
        mFocus = pFlag;
        update();
    }
}

void MenuItem::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.setPen(Qt::NoPen);

    if(mFocus)
        p.setBrush(QColor(22,24,23));
    else
        p.setBrush(QColor(44, 44, 44));
    QRect tmpRect1 = QRect(0, 0, width(), height()-D_ITEM_LINE_LEN);
    p.drawRect(tmpRect1);

    p.setBrush(QColor(22,24,23));
    QRect tmpRect2 = QRect(0, height()-D_ITEM_LINE_LEN, width(), D_ITEM_LINE_LEN);
    p.drawRect(tmpRect2);
}

bool MenuItem::IsChanged()
{
    return mIsChanged;
}

QVariant MenuItem::GetValue()
{

}

bool MenuItem::SetValue(QVariant pValue)
{

}

MenuWidget::MenuWidget(QWidget *parent) :
    QWidget(parent)
{
    installEventFilter(this);
    mValidPressed = false;
    mIndex = -1;
    QTimer::singleShot(10, this, SLOT(slot_Init()));
}

MenuWidget::~MenuWidget()
{
}

void MenuWidget::slot_Init()
{
    Initialize();
}

void MenuWidget::Initialize()
{

}

bool MenuWidget::AddItem(MenuItem *pItem)
{
    if(!pItem)
        return false;
    if(mItemWidgets.contains(pItem))
        return false;
    //pItem->installEventFilter(this);
    mItemWidgets.append(pItem);
    return true;
}

bool MenuWidget::IsChanged()
{
    int tmpItemType = -1;
    return IsChanged(tmpItemType);
}

bool MenuWidget::IsChanged(int &pType)
{
    for(int i=0;i<mItemWidgets.count();i++)
    {
        MenuItem *tmpItem = mItemWidgets.at(i);
        if(!tmpItem)
            continue;
        if(tmpItem->IsChanged())
        {
            pType = i;
            return true;
        }
    }
    return false;
}

QSize MenuWidget::ItemSize(int pIndex)
{
    if(pIndex >= 0 && pIndex < mItemWidgets.count())
    {
        return mItemWidgets.at(pIndex)->size();
    }
    return QSize();
}

int MenuWidget::ItemCount()
{
    return mItemWidgets.count();
}

MenuItem* MenuWidget::CurrentItem()
{
    if(mIndex >= 0 && mIndex < mItemWidgets.count())
    {
        return mItemWidgets.at(mIndex);
    }
    return (MenuItem*)0;
}

QVariant MenuWidget::CurrentValue()
{
    MenuItem *tmpItem = CurrentItem();
    if(tmpItem)
    {
        return tmpItem->GetValue();
    }
    return QVariant();
}

int MenuWidget::CurrentIndex()
{
    return mIndex;
}

bool MenuWidget::eventFilter(QObject *object, QEvent *event)
{
    const QEvent::Type type = event->type();
    if(type == QEvent::MouseButtonPress)
    {
        mValidPressed = true;
    }
    else if(type == QEvent::MouseMove)
    {
        mValidPressed = false;
    }
    else if(type == QEvent::MouseButtonRelease && mValidPressed)
    {
        mValidPressed = false;

        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        const QPoint mousePos = mouseEvent->pos();
        bool tmpChange = true;
        if(mIndex >= 0 && mIndex < mItemWidgets.count())  //判断是否点击的是同一个Item
        {
            if(mItemWidgets.at(mIndex)->geometry().contains(mousePos))
                tmpChange = false;
        }
        if(tmpChange)
        {
            for(int i=0;i<mItemWidgets.count();i++)
            {
                MenuItem *tmpItem = mItemWidgets.at(i);
                if(tmpItem)
                {
                    if(tmpItem->geometry().contains(mousePos))
                    {
                        tmpItem->SetFocus(true);
                        mIndex = i;
                    }
                    else
                    {
                        tmpItem->SetFocus(false);
                    }
                }
            }
        }
    }
    return QWidget::eventFilter(object, event);
}

MenuScroll::MenuScroll(QWidget *parent) : QScrollArea(parent)
{
    mFlickCharm.activateOn(this);
}

MenuScroll::~MenuScroll()
{

}

void MenuScroll::resizeEvent(QResizeEvent *e)
{
    if(e)
    {
        MenuWidget* tmpItem = Widget();
        if(tmpItem)
        {
            tmpItem->setFixedWidth(e->size().width());
        }
    }
    QScrollArea::resizeEvent(e);
}

MenuWidget* MenuScroll::Widget()
{
    return VAR_CAST<MenuWidget*>(widget());
}

void MenuScroll::MoveDown(int pDiff)
{
    if(pDiff)
        horizontalScrollBar()->setValue(horizontalScrollBar()->value()+pDiff);
    else
        horizontalScrollBar()->setValue(horizontalScrollBar()->value()+10);
}

void MenuScroll::MoveUp(int pDiff)
{
    if(pDiff)
        horizontalScrollBar()->setValue(horizontalScrollBar()->value()-pDiff);
    else
        horizontalScrollBar()->setValue(horizontalScrollBar()->value()-10);
}
