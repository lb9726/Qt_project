#include "settabclass.h"

EditedBox::EditedBox(QWidget* parent)
    : QGroupBox(parent)
{
    mEditable = false;
}

EditedBox::EditedBox(const QString &title, QWidget* parent)
    : QGroupBox(title, parent)
{
    mEditable = false;
}

bool EditedBox::SetEditable(bool pFlag)
{
    mEditable = pFlag;
}

void EditedBox::Next()
{
    if(!mEditable)
        return;
}

void EditedBox::Previous()
{
    if(!mEditable)
        return;
}

SrnRotBox::SrnRotBox(QWidget* parent)
    : EditedBox(parent)
{
    mEditable = false;
}

SrnRotBox::SrnRotBox(const QString &title, QWidget* parent)
    : EditedBox(title, parent)
{
    mEditable = false;
}

bool SrnRotBox::SetEditable(bool pFlag)
{
    mEditable = pFlag;
    if(mEditable)
    {
        mBtnList = findChildren<QPushButton*>();
    }
    else
    {
        mBtnList.clear();
    }
}

int SrnRotBox::GetFocus()
{
    for(int i=0;i<mBtnList.count();i++)
    {
        if(mBtnList.at(i)->isChecked())
            return i;
    }
    return -1;
}

void SrnRotBox::SetFocus(int pIndex)
{
    if(pIndex >= mBtnList.count())
        return;
    for(int i=0;i<mBtnList.count();i++)
    {
        if(pIndex == i)
            mBtnList.at(i)->setChecked(true);
        else
            mBtnList.at(i)->setChecked(false);
    }
}

void SrnRotBox::Next()
{
    if(!mEditable)
        return;
    int index = GetFocus();
    if(index < mBtnList.count()-1)
        SetFocus(index+1);
}

void SrnRotBox::Previous()
{
    if(!mEditable)
        return;
    int index = GetFocus();
    if(index > 0)
        SetFocus(index-1);
}

TimeSetBox::TimeSetBox(QWidget* parent)
    : EditedBox(parent)
{
    mEditable = false;
}

TimeSetBox::TimeSetBox(const QString &title, QWidget* parent)
    : EditedBox(title, parent)
{
    mEditable = false;
}

bool TimeSetBox::SetEditable(bool pFlag)
{
    mEditable = pFlag;
}

void TimeSetBox::Next()
{
    if(!mEditable)
        return;
}

void TimeSetBox::Previous()
{
    if(!mEditable)
        return;
}

BLSetBox::BLSetBox(QWidget* parent)
    : EditedBox(parent)
{
    mEditable = false;
}

BLSetBox::BLSetBox(const QString &title, QWidget* parent)
    : EditedBox(title, parent)
{
    mEditable = false;
}

bool BLSetBox::SetEditable(bool pFlag)
{
    mEditable = pFlag;
    if(mEditable)
    {
        mSlider = findChild<QSlider*>();
    }
}

void BLSetBox::Next()
{
    if(!mEditable)
        return;
    mSlider.data()->setValue(mSlider.data()->value()+mSlider.data()->singleStep());
}

void BLSetBox::Previous()
{
    if(!mEditable)
        return;
    mSlider.data()->setValue(mSlider.data()->value()-mSlider.data()->singleStep());
}

VolSetBox::VolSetBox(QWidget* parent)
    : EditedBox(parent)
{
    mEditable = false;
}

VolSetBox::VolSetBox(const QString &title, QWidget* parent)
    : EditedBox(title, parent)
{
    mEditable = false;
}

bool VolSetBox::SetEditable(bool pFlag)
{
    mEditable = pFlag;
    if(mEditable)
    {
        mSlider = findChild<QSlider*>();
    }
}

void VolSetBox::Next()
{
    if(!mEditable)
        return;
    mSlider.data()->setValue(mSlider.data()->value()+mSlider.data()->singleStep());
}

void VolSetBox::Previous()
{
    if(!mEditable)
        return;
    mSlider.data()->setValue(mSlider.data()->value()-mSlider.data()->singleStep());
}

