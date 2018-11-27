#ifndef STACKEDMENU_H
#define STACKEDMENU_H

#include "global.h"
#include <QStackedWidget>

namespace Ui {
class StackedMenu;
}

#define D_ELE_TYPE_CAN     0
#define D_ELE_TYPE_485     1

#define D_YEAR_NUM(x)       (x-2015+1)
#define D_MONTH_NUM(x)      (x)
#define D_DAY_NUM(x)        (x)

#define D_HOUR_NUM(x)       (x+1)
#define D_MINUTE_NUM(x)     (x+1)

#define D_VOLUME_NUM(x)     (x+1)
#define D_BACKLIGHT_NUM(x)  (x+1)

#define D_PROTYPE_NUM(x)    (x+1)
#define D_PROBUAD_NUM(x)    (mBuadList.indexOf(QString::number(x))+1)
#define D_PROID_NUM(x)      (x)

class PluginWidget;

class StackedMenu : public QStackedWidget
{
    Q_OBJECT
    
public:
    explicit StackedMenu(QWidget *parent = 0);
    ~StackedMenu();

    void SetGeometry(QRect pRect);

    void SyncDateTime(QDateTime pDateTime);
    void SyncLuminance(int pValue);
    void SyncVolume(int pAudio, int pVideo);
    void SyncProtocol(int pType, int pBuad, int pID);
    
signals:
    void sHide();

public slots:
    void slot_MenuItemClicked(int id);

public:
    Ui::StackedMenu *ui;
    QStringList      mMenuTextList;
    PluginWidget    *mParent;

    QStringList      mBuadList;

    QDateTime        mOriginDateTime;
    int              mOriginLuminance;
    int              mOriginVolAudio, mOriginVolVideo;
    int              mOriginType, mOriginBuad, mOriginID;

private slots:
    void on_btn_back_clicked();
    void on_btn_saveexit_clicked();
    void on_btn_exit_clicked();
};

#endif // STACKEDMENU_H
