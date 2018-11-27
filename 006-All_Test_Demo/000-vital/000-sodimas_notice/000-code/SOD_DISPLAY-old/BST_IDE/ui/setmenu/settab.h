#ifndef SETTAB_H
#define SETTAB_H

#include "settabclass.h"
#include <QButtonGroup>
#include "physicalplayer.h"
#include "devicemanager.h"
#include "bstui.h"

#ifdef SUPPORT_SCENE
#include "uiscene.h"
class UiScene;
#elif SUPPORT_WIDGET
#include "uiwidget.h"
class UiWidget;
#endif

namespace Ui {
class SetTabBar;
}

class SetTabBar : public QDialog
{
    Q_OBJECT
public:
#ifdef SUPPORT_SCENE
    explicit SetTabBar(UiScene *pContainer, QWidget *parent = 0);
#elif SUPPORT_WIDGET
    explicit SetTabBar(UiWidget *pContainer, QWidget *parent = 0);
#endif
    ~SetTabBar();

    void ShowBusy(bool pFlag, quint64 pTimeout=0);
    void InitTime();
    bool eventFilter(QObject *target, QEvent *event);
    void KeyProcess(KEY_OPERATE pKey);

    int GetFocusTab();
    void SetFocusTab(int pTab);
    void SetGroupChecked(QList<EditedBox*> pList, int pIndex);
    void SetGroupChecked(QList<EditedBox*> pList, EditedBox *pItem);

    void Exit(bool pFlag);

public slots:
    void slot_CloseBusy();
    void slot_Init();
    void slot_TabClicked(int id);

public:
#ifdef SUPPORT_SCENE
    QPointer<UiScene>   m_UiContainer;
#elif SUPPORT_WIDGET
    QPointer<UiWidget>  m_UiContainer;
#endif

    QButtonGroup        m_TabGroup;

    //>@标识符
    MANUAL_STATE        m_ManualState;
    bool                m_NeedReboot;

    QTimer              mBusyTimeout;
    gifplayer          *mBusyFlagIcon;

    bool                mInited;

private:
    Ui::SetTabBar *ui;
};

#endif // SETTAB_H

