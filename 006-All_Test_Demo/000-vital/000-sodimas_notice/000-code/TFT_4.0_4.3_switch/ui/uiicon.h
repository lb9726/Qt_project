#ifndef UIICON_H
#define UIICON_H

#include <QtWidgets>
#include <QDomElement>

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

public:
    QLabel          *mIcon;
    QLabel          *mTextLine1;
    QLabel          *mTextLine2;

    QLabel          *mBigTextLine1;
    QLabel          *mBigTextLine2;
    QLabel          *mBigTextLine3;
    QLabel          *mBigTextLine4;

    QHash<int, QString>   mRcHash;  //  rcÐòºÅ->rcÂ·¾¶

    int             mIconNum;

    QDomElement    mDomElement;
};


#endif // UIICON_H
