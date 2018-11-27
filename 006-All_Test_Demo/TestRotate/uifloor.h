#ifndef UIFLOOR_H
#define UIFLOOR_H
#include <QWidget>
#include <QLabel>
#include <QDomElement>

class UiFloor : public QWidget
{
    Q_OBJECT
public:
    UiFloor(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

public:
    void Init(QDomElement pElement);
    void setFloor(int pGe,int pShi,int pBai);
    void updateFloor();
    void setTimeAndDest(QString time, QString dest);

public:
    QLabel*     mHundLabel;
    QLabel*     mTenLabel;
    QLabel*     mSingleLabel;
    QLabel*     mTimeText;
    QLabel*     mDestText;

    int         mHundred;
    int         mTen;
    int         mSingle;

    QHash<int, QString>   mRcHash;  //  rcÐòºÅ->rcÂ·¾¶
};

#endif // UIFLOOR_H
