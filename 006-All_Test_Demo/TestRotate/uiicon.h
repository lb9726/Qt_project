#ifndef UIICON_H
#define UIICON_H

#include <QtWidgets>
#include <QDomElement>
#include <QListView>
#include <QProgressBar>
//#include "common/qdevicewatcher/usbaction.h"
#include <QStringListModel>

class UiIcon : public QWidget
{
    Q_OBJECT
public:
    UiIcon(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    void Init(QDomElement pElement);

    void setIcon(int pIcon);
    void updateIcon();

    void setText(QString pText1, QString pText2, QString pText3, QString pText4);
    void setBigSrnMode(bool pFlag);
    int getStackWidgetIndex();
    void setHaveCopyFlag(bool flag);
//    void setCurStackWidgetIndex(int index);
    void setTimeAndDest(QString time, QString dest);

public slots:
    void setBlackGround(int index);
    void setUsbConnected();
//    void showCopyScreen();
    void updateProgress(int num);
    void setUsbRemoveFlag();


public:
    QLabel          *mIcon;
    QLabel          *mTextLine1;
    QLabel          *mTextLine2;

    QLabel                  *mBigTextLine1;
    QLabel                  *mBigTextLine2;
    QLabel                  *mBigTextLine3;
    QLabel                  *mBigTextLine4;
    QLabel                  *mBlackBack;

    QLabel                  *mTimeText;
    QLabel                  *mDestText;
    QLabel                  *mUsbPicture;
    QLabel                  *mLabelWaring;
    QListView               *mTextView;
    QProgressBar            *mCopyProgress;
    QHash<int, QString>     mRcHash;  //  rcÐòºÅ->rcÂ·¾¶

    int                     mIconNum;

    QDomElement             mDomElement;

    int                     curStackWidgetIndex;
    int                     preStackWidgetIndex;
//    UsbAction               mUsbAct;
    QStringListModel        model;
    bool                    mHaveCopy;

};


#endif // UIICON_H
