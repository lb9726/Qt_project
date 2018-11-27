#include "widgetbutton.h"

WidgetButton::WidgetButton(QWidget *parent) :
    WidgetBase(parent)
{
    m_Checked = false;

    mOperate = OPERATE_NONE;
    mValidFlag = true;

    mIsPressed = false;
    mStatues = 0;

    m_BtnType = 0;

    mPressDelay = 100;
    connect(&mPressDelayTimer, SIGNAL(timeout()), this, SLOT(slot_PressDelay()));
    mPressDelayTimer.setSingleShot(true);
    mPressDelayTimer.setInterval(mPressDelay);
}

WidgetButton::~WidgetButton()
{

}

void WidgetButton::SetValidRect(QRect pRect)
{
    mValidRect = pRect;

}

void WidgetButton::InitButton(QString pDark, QString pLight)
{
    load(PIC_DARK, pDark);
    load(PIC_LIGHT, pLight);

    m_ImgPointer.mLastPointer = m_ImgPointer.mCurPointer;
    m_ImgPointer.mCurPointer = GetPixmap(PIC_DARK);
    StartRender();
}

void WidgetButton::mousePressEvent(QMouseEvent *e)
{
    if (e && e->button() == Qt::LeftButton)
    {
        mValidFlag = true;
        if(!mValidRect.isEmpty())
        {
            if(!mValidRect.contains(e->pos()))
                mValidFlag = false;
        }
        if(mValidFlag)
        {
            mIsPressed = true;
            if(m_BtnType == 0)
            {
                //mPressDelayTimer.start();
                slot_ExecOperate(OPERATE_KEYPRESS, QVariant());
            }
            emit sPressed();
        }
    }
    QWidget::mousePressEvent(e);
}

void WidgetButton::slot_PressDelay()
{
    mIsPressed = true;
    slot_ExecOperate(OPERATE_KEYPRESS, QVariant());
    emit sPressed();
}

void WidgetButton::ReleaseButton()
{
    if(m_Checked)
    {
        m_Checked = false;
        slot_ExecOperate(OPERATE_KEYRELEASE, QVariant());
        emit sReleased();
    }
}

void WidgetButton::mouseMoveEvent(QMouseEvent *e)
{
    QWidget::mouseMoveEvent(e);
}

void WidgetButton::mouseReleaseEvent(QMouseEvent *e)
{
    if (e && e->button() == Qt::LeftButton)
    {
        if(mValidFlag)
        {
            mIsPressed = false;
            if(m_BtnType == 0)
            {
                slot_ExecOperate(OPERATE_KEYRELEASE, QVariant());
            }
            emit sReleased();
        }
        else
        {
            mValidFlag = true;
        }
    }
    QWidget::mouseReleaseEvent(e);
}

void WidgetButton::slot_ExecOperate(AREA_OPERATE pOperate, QVariant pPara)
{
    m_EffectRender = m_EffectRenders.value(pOperate);
    switch(pOperate)
    {
        case OPERATE_KEYPRESS:
        {
            if(mOperate == pOperate)
                return;
            mOperate = pOperate;
            m_ImgPointer.mLastPointer = m_ImgPointer.mCurPointer;
            m_ImgPointer.mCurPointer = GetPixmap(PIC_LIGHT);
            StartRender();
            break;
        }
        case OPERATE_KEYLIGHT:
        {
            if(mOperate == pOperate)
                return;
            mOperate = pOperate;
            m_ImgPointer.mLastPointer = m_ImgPointer.mCurPointer = GetPixmap(PIC_LIGHT);
            StartRender();
            break;
        }
        case OPERATE_KEYCANCEL:
        {
            if(!mIsPressed)
            {
                if(mOperate == pOperate)
                    return;
                mOperate = pOperate;
                m_ImgPointer.mLastPointer = m_ImgPointer.mCurPointer = GetPixmap(PIC_DARK);
                StartRender();
            }
            break;
        }
        case OPERATE_KEYRELEASE:
        {
            if(mOperate == pOperate)
                return;
            if(mOperate != OPERATE_KEYLIGHT)
            {
                m_ImgPointer.mLastPointer = m_ImgPointer.mCurPointer;
                m_ImgPointer.mCurPointer = GetPixmap(PIC_DARK);
                StartRender();
            }
            mOperate = pOperate;
            break;
        }
        default:
            break;
    }
}


