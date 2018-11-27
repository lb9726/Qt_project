#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUiLoader>
#include "uiarrow.h"
#include "uifloor.h"
#include "uidesktop.h"
#include "uiicon.h"
#include "uitext.h"
#include "uivideo.h"
#include "uiclock.h"
#include "themeparser.h"

bool CopyFileContent(QString srcFile, QString dstFile);

class Uiloader;
extern Uiloader *gUiLoader;

class Uiloader : public QUiLoader
{
    Q_OBJECT

public:
    Uiloader(QObject *parent=0);
    QWidget* createWidget(const QString &className, QWidget *parent, const QString &name);

    void InitComponent(COM_TYPE pType, QDomElement pElement);
    void SetLayout(int pLayout);
    void SetRotate(int pRotate);

    QWidget* Init(QString pUiPath);

public slots:
    void slot_AutoTest();
    void slot_ArrowAction(bool pArrows_FLM, bool pArrows_FLD, bool pArrows_SCROL);
    void slot_FloorArrive(int pGe, int pShi, int pBai, int pSoundNumber);
    void slot_ShowFuncLogo(int pFuncLogoIndex, int pSoundNumber);
    void slot_Orient(int pOrientation); //sdo旋转横竖屏
public:
    QTimer                      mAutoTestTimer;
    int                         mAutoTestCount;
    QGraphicsView               *mView;
    ThemeParser                 *m_themeParser;
    int                         m_themeLayout;
    DIRECTION                   m_themeDirection;
    int                         m_themeRotate;
    UiDesktop                  *mDesktop;
    UiArrow                    *mArrow;
    UiFloor                    *mFloor;
    UiIcon                     *mIcon;

public:
    int             mScrollSpeed;
    bool            mIsShowLogo;

};

#endif // MAINWINDOW_H
