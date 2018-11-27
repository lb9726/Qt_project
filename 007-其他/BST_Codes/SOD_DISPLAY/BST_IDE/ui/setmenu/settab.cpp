#include "settab.h"
#include "ui_settab.h"

#ifdef SUPPORT_SCENE
SetTabBar::SetTabBar(UiScene *pContainer, QWidget *parent) :
#elif SUPPORT_WIDGET
SetTabBar::SetTabBar(UiWidget *pContainer, QWidget *parent) :
#endif
    QDialog(parent
#ifndef DEBUG
            , Qt::FramelessWindowHint
#endif
            ),
    ui(new Ui::SetTabBar)
{
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
    ui->setupUi(this);

    installEventFilter(this);

    m_NeedReboot = false;
    mInited = false;
    m_ManualState = STATE_ITEMUI;
    setFocus();

    m_UiContainer = pContainer;

    mBusyFlagIcon = 0;
    mBusyTimeout.setSingleShot(true);
    connect(&mBusyTimeout, SIGNAL(timeout()), this, SLOT(slot_CloseBusy()));

    connect(&m_TabGroup, SIGNAL(buttonClicked(int)), this, SLOT(slot_TabClicked(int)));
    m_TabGroup.addButton(ui->btn_bar_basic, 0);
    m_TabGroup.addButton(ui->btn_bar_physical, 1);
    m_TabGroup.addButton(ui->btn_bar_logic, 2);
    m_TabGroup.addButton(ui->btn_bar_theme, 3);

    slot_TabClicked(0);

    QTimer::singleShot(100, this, SLOT(slot_Init()));
}

SetTabBar::~SetTabBar()
{
    delete ui;
}

void SetTabBar::slot_TabClicked(int id)
{
    if(id == 0)
    {
        ui->MainStacked->setCurrentIndex(0);
    }
    else if(id == 1)
    {
        ui->MainStacked->setCurrentIndex(1);
    }
    else if(id == 2)
    {
        ui->MainStacked->setCurrentIndex(2);
    }
    else if(id == 3)
    {
        ui->MainStacked->setCurrentIndex(3);
    }
}

void SetTabBar::slot_Init()
{
    if(mInited)
        return;
    mInited = true;
    ShowBusy(true);
    if(!m_UiContainer.isNull())
    {
        //>@Basic组件
        InitTime();

        //>@Physical组件
        QList<PH_Type> tmpPHList = m_UiContainer.data()->m_LogicThemeManager->m_DevManager->m_PHDevList.keys();
        PH_Type tmpPH;
        int tmpMaxHeight = 0;
        foreach(tmpPH, tmpPHList)
        {
            PhysicalDevice *tmpPHDev = m_UiContainer.data()->m_LogicThemeManager->m_DevManager->m_PHDevList.value(tmpPH);
            if(tmpPHDev && tmpPHDev->m_SettingGroup)
            {
                ui->contents_physical->layout()->addWidget(tmpPHDev->m_SettingGroup);
                tmpMaxHeight += tmpPHDev->m_SettingGroup->height() + 20;
            }
        }
        ui->contents_physical->layout()->addItem(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
        ui->contents_physical->setFixedHeight(tmpMaxHeight);
        //>@Logic组件
        QList<LG_Type> tmpLGList = m_UiContainer.data()->m_LogicThemeManager->m_DevManager->m_LGDevList.keys();
        LG_Type tmpLG;
        tmpMaxHeight = 0;
        foreach(tmpLG, tmpLGList)
        {
            LogicDevice *tmpLGDev = m_UiContainer.data()->m_LogicThemeManager->m_DevManager->m_LGDevList.value(tmpLG);
            if(tmpLGDev && tmpLGDev->m_SettingGroup)
            {
                ui->contents_logic->layout()->addWidget(tmpLGDev->m_SettingGroup);
                tmpMaxHeight += tmpLGDev->m_SettingGroup->height() + 20;
            }
        }
        ui->contents_logic->layout()->addItem(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
        ui->contents_logic->setFixedHeight(tmpMaxHeight);
        //>@Theme组件
        m_UiContainer.data()->m_ComDiagramGroup;
    }
    ui->scroll_basic->InitMovePara(Vertical);
    ui->scroll_physical->InitMovePara(Vertical);
    ui->scroll_logic->InitMovePara(Vertical);
    ui->scroll_theme->InitMovePara(Vertical);
    ShowBusy(false);
}

void SetTabBar::InitTime()
{
    PhysicalRTC* tmpRTC = VAR_CAST<PhysicalRTC*>(m_UiContainer.data()->m_LogicThemeManager->m_DevManager->m_PHDevList.value(P_RTC));
    if(tmpRTC)
    {
        ui->TimeBox->setEnabled(true);
        QDateTime tmpDateTime = tmpRTC->GetTime();
        ui->list_time_year->clear();
        ui->list_time_month->clear();
        ui->list_time_day->clear();
        ui->list_time_hour->clear();
        ui->list_time_minute->clear();
        for(int i=0;i<50;i++)
        {
            int year = 2000+i;
            QListWidgetItem *tmpItem = new QListWidgetItem(QString("%1").arg(year));
            tmpItem->setTextAlignment(Qt::AlignHCenter);
            ui->list_time_year->addItem(tmpItem);
        }
        SetListCurRow(ui->list_time_year, tmpDateTime.date().year()-2000);
        for(int i=1;i<=12;i++)
        {
            QListWidgetItem *tmpItem = new QListWidgetItem(QString("%1").arg(i));
            tmpItem->setTextAlignment(Qt::AlignHCenter);
            ui->list_time_month->addItem(tmpItem);
        }
        SetListCurRow(ui->list_time_month, tmpDateTime.date().month()-1); //>@1~12
        int daycount = tmpDateTime.date().daysInMonth();
        for(int i=1;i<=daycount;i++)
        {
            QListWidgetItem *tmpItem = new QListWidgetItem(QString("%1").arg(i));
            tmpItem->setTextAlignment(Qt::AlignHCenter);
            ui->list_time_day->addItem(tmpItem);
        }
        SetListCurRow(ui->list_time_day, tmpDateTime.date().day()-1);  //>@1~31
        for(int i=0;i<24;i++)
        {
            QListWidgetItem *tmpItem = new QListWidgetItem(QString("%1").arg(i));
            tmpItem->setTextAlignment(Qt::AlignHCenter);
            ui->list_time_hour->addItem(tmpItem);
        }
        SetListCurRow(ui->list_time_hour, tmpDateTime.time().hour());  //>@0~23
        for(int i=0;i<59;i++)
        {
            QListWidgetItem *tmpItem = new QListWidgetItem(QString("%1").arg(i));
            tmpItem->setTextAlignment(Qt::AlignHCenter);
            ui->list_time_minute->addItem(tmpItem);
        }
        SetListCurRow(ui->list_time_minute, tmpDateTime.time().minute()); //>@0~59

        ui->list_time_year->SetAlignMode(true);
        ui->list_time_year->InitMovePara(Vertical, QSizeF(1, 3), QSizeF(1, 50));
        ui->list_time_month->SetAlignMode(true);
        ui->list_time_month->InitMovePara(Vertical, QSizeF(1, 3), QSizeF(1, 12));
        ui->list_time_day->SetAlignMode(true);
        ui->list_time_day->InitMovePara(Vertical, QSizeF(1, 3), QSizeF(1, 31));
        ui->list_time_hour->SetAlignMode(true);
        ui->list_time_hour->InitMovePara(Vertical, QSizeF(1, 3), QSizeF(1, 24));
        ui->list_time_minute->SetAlignMode(true);
        ui->list_time_minute->InitMovePara(Vertical, QSizeF(1, 3), QSizeF(1, 60));
    }
    else
    {
        ui->TimeBox->setDisabled(true);
    }
}

void SetTabBar::ShowBusy(bool pFlag, quint64 pTimeout)
{
    if(!mBusyFlagIcon)
    {
        mBusyFlagIcon = new gifplayer(":/0_common/rc/0_common/busy.gif", true, this);
        mBusyFlagIcon->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
        mBusyFlagIcon->setAttribute(Qt::WA_TranslucentBackground);
    }

    mBusyTimeout.stop();
    if(pFlag)
    {
        setDisabled(true);
        mBusyFlagIcon->setFixedSize(100, 100);
        mBusyFlagIcon->move(QPoint(size().width()/2 - mBusyFlagIcon->size().width()/2.0,
                                   size().height()/2 - mBusyFlagIcon->size().height()/2.0));
        mBusyFlagIcon->show();
        if(pTimeout > 0)
        {
            mBusyTimeout.setInterval(pTimeout);
            mBusyTimeout.start();
        }
    }
    else
    {
        setEnabled(true);
        mBusyFlagIcon->hide();
    }
}

void SetTabBar::slot_CloseBusy()
{
    setEnabled(true);
    mBusyFlagIcon->hide();
}

bool SetTabBar::eventFilter(QObject *target, QEvent *event)
{
    if(target == this)
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent *k = static_cast<QKeyEvent *>(event);
            switch(k->key())
            {
                case Qt::Key_F1: //KEY_SET
                {
                    KeyProcess(KEY_SET);
                    break;
                }
                case Qt::Key_F2: //KEY_+
                {
                    KeyProcess(KEY_UP);
                    break;
                }
                case Qt::Key_F3: //KEY_-
                {
                    KeyProcess(KEY_DN);
                    break;
                }
                case Qt::Key_F4: //KEY_BACK
                {
                    KeyProcess(KEY_BACK);
                    break;
                }
                default:
                    break;
            }
        }
    }
    return QWidget::eventFilter(target,event);
}

void SetTabBar::Exit(bool pFlag)
{
    m_ManualState = STATE_ITEMUI;
    ui->TabBar->setFocus();
    if(pFlag)
    {
        if(m_UiContainer.data() && m_UiContainer.data()->m_LogicThemeManager &&
           m_UiContainer.data()->m_LogicThemeManager->m_DevManager &&
           m_UiContainer.data()->m_LogicThemeManager->m_DevManager->mMdpManager)
        {
            m_UiContainer.data()->m_LogicThemeManager->m_DevManager->mMdpManager->SaveMDP();
        }
        if(m_UiContainer.data() && m_UiContainer.data()->m_UiManager)
        {
            m_UiContainer.data()->m_UiManager->SaveUI();
        }
        QDialog::accept();
    }
    else
    {
        QDialog::reject();
    }
}

int SetTabBar::GetFocusTab()
{
    bool tmpBasicTab = ui->btn_bar_basic->isChecked();
    if(tmpBasicTab)
        return 0;
    bool tmpPhysicalTab = ui->btn_bar_physical->isChecked();
    if(tmpPhysicalTab)
        return 1;
    bool tmpLogicTab = ui->btn_bar_logic->isChecked();
    if(tmpLogicTab)
        return 2;
    bool tmpThemeTab = ui->btn_bar_theme->isChecked();
    if(tmpThemeTab)
        return 3;
    return -1;
}

void SetTabBar::SetFocusTab(int pTab)
{
    switch(pTab)
    {
    case 0:
        ui->btn_bar_basic->setChecked(true);
        break;
    case 1:
        ui->btn_bar_physical->setChecked(true);
        break;
    case 2:
        ui->btn_bar_logic->setChecked(true);
        break;
    case 3:
        ui->btn_bar_theme->setChecked(true);
        break;
    default:
        break;
    }
}

void SetTabBar::SetGroupChecked(QList<EditedBox*> pList, EditedBox *pItem)
{
    int index = pList.indexOf(pItem);
    if(index < 0)
        return;
    SetGroupChecked(pList, index);
}

void SetTabBar::SetGroupChecked(QList<EditedBox*> pList, int pIndex)
{
    for(int i=0;i<pList.count();i++)
    {
        if(i == pIndex)
        {
            pList.at(i)->setChecked(true);
        }
        else
        {
            pList.at(i)->setChecked(false);
        }
    }
}

void SetTabBar::KeyProcess(KEY_OPERATE pKey)
{
    static QList<EditedBox*> sCurGroupList;
    static EditedBox *sCurGroup = 0;
    switch(m_ManualState)
    {
        case STATE_NONE:        //>@点击SET按钮进入一级菜单
        {
            if(pKey == KEY_SET)
            {
                m_ManualState = STATE_ITEMUI;
                ui->TabBar->setFocus();
            }
            break;
        }
        case STATE_ITEMUI:      //>@点击左右按键左右滑动，点击SET按钮设置窗口获得输入焦点
        {
            if(pKey == KEY_BACK)
            {
                Exit(true);
                if(m_NeedReboot)
                {
                    Reboot();
#ifdef LINUX
                    while(1)
                    {
                        QtSleep(1000);
                    }
#endif
                }
            }
            else if(pKey == KEY_SET)
            {
                QWidget *tmpWidget = ui->MainStacked->currentWidget();
                if(tmpWidget)
                {
                    sCurGroupList = tmpWidget->findChildren<EditedBox *>();
                    if(sCurGroupList.isEmpty())
                        return;
                    ui->MainStacked->setFocus();
                    sCurGroup = sCurGroupList.first();
                    SetGroupChecked(sCurGroupList, 0);
                    m_ManualState = STATE_CTRLUI;
                }
            }
            else if(pKey == KEY_UP)
            {
                int tmpTab = GetFocusTab();
                if(tmpTab > 0)
                    SetFocusTab(tmpTab-1);
            }
            else if(pKey == KEY_DN)
            {
                int tmpTab = GetFocusTab();
                if(tmpTab < 3)
                    SetFocusTab(tmpTab+1);
            }
            break;
        }
        case STATE_CTRLUI:
        {
            if(pKey == KEY_BACK)
            {
                sCurGroupList.clear();
                sCurGroup = 0;
                m_ManualState = STATE_ITEMUI;
                ui->TabBar->setFocus();
            }
            else if(pKey == KEY_SET)
            {
                m_ManualState = STATE_FOCUSUI;
                sCurGroup->SetEditable(true);
            }
            else if(pKey == KEY_UP)
            {
                int index = sCurGroupList.indexOf(sCurGroup);
                if(index > 0)
                {
                    sCurGroup = sCurGroupList.at(index-1);
                    SetGroupChecked(sCurGroupList, index-1);
                    QWidget *tmpPar = VAR_CAST<QWidget*>(sCurGroup->parent());
                    QScrollArea *tmpScrollArea = VAR_CAST<QScrollArea*>(tmpPar->parent()->parent());
                    if(tmpScrollArea)
                    {
                        QScrollBar *tmpBar = tmpScrollArea->verticalScrollBar();
                        int h1 = sCurGroup->geometry().y();
                        int h2 = tmpPar->height();
                        int h4 = tmpBar->maximum();
                        int h3 = h1*h4/h2;
                        tmpBar->setValue(h3);
                    }
                }
            }
            else if(pKey == KEY_DN)
            {
                int index = sCurGroupList.indexOf(sCurGroup);
                if(index < sCurGroupList.count()-1)
                {
                    sCurGroup = sCurGroupList.at(index+1);
                    SetGroupChecked(sCurGroupList, index+1);
                    QWidget *tmpPar = VAR_CAST<QWidget*>(sCurGroup->parent());
                    QScrollArea *tmpScrollArea = VAR_CAST<QScrollArea*>(tmpPar->parent()->parent());
                    if(tmpScrollArea)
                    {
                        QScrollBar *tmpBar = tmpScrollArea->verticalScrollBar();
                        int h1 = sCurGroup->geometry().y();
                        int h2 = tmpPar->height();
                        int h4 = tmpBar->maximum();
                        int h3 = h1*h4/h2;
                        tmpBar->setValue(h3);
                    }
                }
            }
            break;
        }
        case STATE_FOCUSUI:      //>@点击SET按钮设置当前输入框修改区域,点击左右按键左右移动焦点
        {
            if(pKey == KEY_BACK)
            {
                m_ManualState = STATE_CTRLUI;
                sCurGroup->SetEditable(false);
            }
            else if(pKey == KEY_UP)
            {
                sCurGroup->Previous();
            }
            else if(pKey == KEY_DN)
            {
                sCurGroup->Next();
            }
            break;
        }
    }
}



