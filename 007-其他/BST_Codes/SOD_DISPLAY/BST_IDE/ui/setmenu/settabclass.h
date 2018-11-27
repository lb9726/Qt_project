#ifndef SETTABCLASS_H
#define SETTABCLASS_H

#include "global.h"
#include <QGroupBox>

class EditedBox : public QGroupBox
{
    Q_OBJECT
public:
    EditedBox(QWidget* parent=0);
    EditedBox(const QString &title, QWidget* parent=0);

    virtual bool SetEditable(bool pFlag);
    virtual void Next();
    virtual void Previous();

public:
    bool  mEditable;
};

class SrnRotBox : public EditedBox
{
    Q_OBJECT
public:
    SrnRotBox(QWidget* parent=0);
    SrnRotBox(const QString &title, QWidget* parent=0);

    bool SetEditable(bool pFlag);

    int GetFocus();
    void SetFocus(int pIndex);

    void Next();
    void Previous();

public:
    QList<QPushButton*>     mBtnList;
};

class TimeSetBox : public EditedBox
{
    Q_OBJECT
public:
    TimeSetBox(QWidget* parent=0);
    TimeSetBox(const QString &title, QWidget* parent=0);

    bool SetEditable(bool pFlag);
    void Next();
    void Previous();
};

class BLSetBox : public EditedBox
{
    Q_OBJECT
public:
    BLSetBox(QWidget* parent=0);
    BLSetBox(const QString &title, QWidget* parent=0);

    bool SetEditable(bool pFlag);
    void Next();
    void Previous();

public:
    QPointer<QSlider>    mSlider;
};

class VolSetBox : public EditedBox
{
    Q_OBJECT
public:
    VolSetBox(QWidget* parent=0);
    VolSetBox(const QString &title, QWidget* parent=0);

    bool SetEditable(bool pFlag);
    void Next();
    void Previous();

public:
    QPointer<QSlider>    mSlider;
};



#endif // SETTABCLASS_H

