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
    void initAudio(QDomElement pElement);//floor map
    void setFloor(int pGe,int pShi,int pBai, int frameCount = 0);
    void updateFloor();
    void setTimeAndDest(QString dest, QString time);
    QString setMapMedia(int key);//floor map
    int getGeWei();
    int getShiWei();
    int getBaiWei();
    void setEmptyFloor();
    void setSmallTextEmpty();

private:
    int getFloorHashKey(int frame);
    void updateMap(QString path);

public:
    QLabel*     mHundLabel;
    QLabel*     mTenLabel;
    QLabel*     mSingleLabel;
    QLabel*     mTimeText;
    QLabel*     mDestText;
    QLabel*     mFloorMapLabel;
    int         mHundred;
    int         mTen;
    int         mSingle;

    QHash<int, QString>   mRcHash;  //  rcÐòºÅ->rcÂ·¾¶
    QHash<int, QString>   mFloorHash; // floor map path
    QHash<int, QString>   mAudioHash;//floor map
};

#endif // UIFLOOR_H
