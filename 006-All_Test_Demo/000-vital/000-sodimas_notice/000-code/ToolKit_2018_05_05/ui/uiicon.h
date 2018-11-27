#ifndef UIICON_H
#define UIICON_H

#include <QtWidgets>
#include <QDomElement>
#include <QListView>
#include <QProgressBar>
#include "common/qdevicewatcher/usbaction.h"
#include <QStringListModel>

class UiIcon : public QWidget
{
    Q_OBJECT
public:
    UiIcon(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    void Init(QDomElement pElement);
    void setIcon(int pIcon);
    void setText(QString pText1, QString pText2, QString pText3, QString pText4);
    void setBigSrnMode(bool pFlag);
    void setHaveCopyFlag(bool flag);
    bool getHaveCopyFlag();
    void setUpdateCompleteFlag(bool flag);
    int getStackWidgetIndex();
    void setChangedGround(int index);

private:
    void updateIcon();    
    void setUsbUI(bool flag, QString str);

private slots:
    void setUsbConnected();
    void showCopyScreen();
    void updateProgress(int num);
    void promptUpdateComplete();

public slots:
    void showBackUpScreen();
    void setUsbRemoveFlag();

private:
    void setLabelStyleSheet();
    void setLabelGeometry();

public:
    QLabel                  *mIcon;
    QLabel                  *mTextLine1;
    QLabel                  *mTextLine2;

    QLabel                  *mBigTextLine1;
    QLabel                  *mBigTextLine2;
    QLabel                  *mBigTextLine3;
    QLabel                  *mBigTextLine4;
    QLabel                  *mBlackBack;

    QLabel                  *mUsbPicture;
    QLabel                  *mLabelWaring;
    QListView               *mTextView;
    QProgressBar            *mCopyProgress;
    QHash<int, QString>     mRcHash;  //  rc序号->rc路径

    int                     mIconNum;

    QDomElement             mDomElement;

    UsbAction               mUsbAct;
    QStringListModel        model;
    bool                    mHaveCopy;
    bool                    mBackUp;
    bool                    mUpdateComplete;
};


#endif // UIICON_H
