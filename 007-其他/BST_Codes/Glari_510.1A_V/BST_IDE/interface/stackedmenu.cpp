#include "stackedmenu.h"
#ifdef SIZE_800x480
    #include "ui_stackedmenu_h400.h"
    #define D_MENU_ITEM_HEIGHT  70
    #define D_NUM_ITEM_HEIGHT  40
#endif
#ifdef SIZE_800x600
    #include "ui_stackedmenu_h400.h"
    #define D_MENU_ITEM_HEIGHT  70
    #define D_NUM_ITEM_HEIGHT  40
#endif
#ifdef SIZE_1024x768
    #include "ui_stackedmenu_h600.h"
    #define D_MENU_ITEM_HEIGHT  85
    #define D_NUM_ITEM_HEIGHT  60
#endif
#ifdef SIZE_1280x800
    #include "ui_stackedmenu_h600.h"
    #define D_MENU_ITEM_HEIGHT  85
    #define D_NUM_ITEM_HEIGHT  60
#endif
#ifdef SIZE_1366x768
    #include "ui_stackedmenu_h600.h"
    #define D_MENU_ITEM_HEIGHT  85
    #define D_NUM_ITEM_HEIGHT  60
#endif
#ifdef SIZE_1920x540
    #include "ui_stackedmenu_h400.h"
    #define D_MENU_ITEM_HEIGHT  70
    #define D_NUM_ITEM_HEIGHT  40
#endif

#include "pluginwidget.h"

StackedMenu::StackedMenu(QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::StackedMenu)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_TranslucentBackground);

    QList<QPushButton*>tmpList = findChildren<QPushButton*>();
    QPushButton *tmpWid = 0;
    foreach(tmpWid, tmpList)
    {
        tmpWid->setFocusPolicy(Qt::NoFocus); //>@同时设置所有widget都没有焦点
    }

    mParent = VAR_CAST<PluginWidget*>(parent->parent());
    setCurrentIndex(0);

    connect(ui->scroll_menu, SIGNAL(sItemClicked(int)), this, SLOT(slot_MenuItemClicked(int)));
    ui->scroll_menu->Init();
    ui->scroll_menu->SetFlag(ItemScrollArea::FLAG_ITEMHEIGHT, D_MENU_ITEM_HEIGHT);
    ui->scroll_menu->SetFlag(ItemScrollArea::FLAG_FOCUS_FONTSIZE, 22);
    ui->scroll_menu->SetFlag(ItemScrollArea::FLAG_UNFOCUS_FONTSIZE, 22);
    ui->scroll_menu->SetFlag(ItemScrollArea::FLAG_ITEMBUTTON, true);
    ui->scroll_menu->SetFlag(ItemScrollArea::FLAG_SELECTOR, false);
    mMenuTextList   << tr("Time")
                    << tr("Volume")
                    << tr("Lumince")
                    << tr("Theme")
                    << tr("Protocol");
    ui->scroll_menu->Init(mMenuTextList, 0);

    mOriginDateTime = QDateTime(QDate(2016, 1, 1), QTime(0, 0));

    ui->scroll_year->Init();
    ui->scroll_year->SetFlag(ItemScrollArea::FLAG_ITEMHEIGHT, D_NUM_ITEM_HEIGHT);
    ui->scroll_year->SetFlag(ItemScrollArea::FLAG_FOCUS_FONTSIZE, 18);
    ui->scroll_year->SetFlag(ItemScrollArea::FLAG_UNFOCUS_FONTSIZE, 16);
    ui->scroll_year->SetFlag(ItemScrollArea::FLAG_ITEMBUTTON, false);
    ui->scroll_year->SetFlag(ItemScrollArea::FLAG_SELECTOR, true);
    QStringList tmpYearList;
    tmpYearList.append(" ");
    for(int i=2015;i<=2030;i++)
        tmpYearList.append(QString::number(i));
    tmpYearList.append(" ");
    ui->scroll_year->Init(tmpYearList, D_YEAR_NUM(mOriginDateTime.date().year()));

    ui->scroll_month->Init();
    ui->scroll_month->SetFlag(ItemScrollArea::FLAG_ITEMHEIGHT, D_NUM_ITEM_HEIGHT);
    ui->scroll_month->SetFlag(ItemScrollArea::FLAG_FOCUS_FONTSIZE, 18);
    ui->scroll_month->SetFlag(ItemScrollArea::FLAG_UNFOCUS_FONTSIZE, 16);
    ui->scroll_month->SetFlag(ItemScrollArea::FLAG_ITEMBUTTON, false);
    ui->scroll_month->SetFlag(ItemScrollArea::FLAG_SELECTOR, true);
    QStringList tmpMonthList;
    tmpMonthList.append(" ");
    for(int i=1;i<=12;i++)
        tmpMonthList.append(QString::number(i));
    tmpMonthList.append(" ");
    ui->scroll_month->Init(tmpMonthList, D_MONTH_NUM(mOriginDateTime.date().month()));

    ui->scroll_day->Init();
    ui->scroll_day->SetFlag(ItemScrollArea::FLAG_ITEMHEIGHT, D_NUM_ITEM_HEIGHT);
    ui->scroll_day->SetFlag(ItemScrollArea::FLAG_FOCUS_FONTSIZE, 18);
    ui->scroll_day->SetFlag(ItemScrollArea::FLAG_UNFOCUS_FONTSIZE, 16);
    ui->scroll_day->SetFlag(ItemScrollArea::FLAG_ITEMBUTTON, false);
    ui->scroll_day->SetFlag(ItemScrollArea::FLAG_SELECTOR, true);
    QStringList tmpDayList;
    tmpDayList.append(" ");
    for(int i=1;i<=31;i++)
        tmpDayList.append(QString::number(i));
    tmpDayList.append(" ");
    ui->scroll_day->Init(tmpDayList, D_DAY_NUM(mOriginDateTime.date().day()));

    ui->scroll_hour->Init();
    ui->scroll_hour->SetFlag(ItemScrollArea::FLAG_ITEMHEIGHT, D_NUM_ITEM_HEIGHT);
    ui->scroll_hour->SetFlag(ItemScrollArea::FLAG_FOCUS_FONTSIZE, 18);
    ui->scroll_hour->SetFlag(ItemScrollArea::FLAG_UNFOCUS_FONTSIZE, 16);
    ui->scroll_hour->SetFlag(ItemScrollArea::FLAG_ITEMBUTTON, false);
    ui->scroll_hour->SetFlag(ItemScrollArea::FLAG_SELECTOR, true);
    QStringList tmpHourList;
    tmpHourList.append(" ");
    for(int i=0;i<24;i++)
        tmpHourList.append(QString::number(i));
    tmpHourList.append(" ");
    ui->scroll_hour->Init(tmpHourList, D_HOUR_NUM(mOriginDateTime.time().hour()));

    ui->scroll_minute->Init();
    ui->scroll_minute->SetFlag(ItemScrollArea::FLAG_ITEMHEIGHT, D_NUM_ITEM_HEIGHT);
    ui->scroll_minute->SetFlag(ItemScrollArea::FLAG_FOCUS_FONTSIZE, 18);
    ui->scroll_minute->SetFlag(ItemScrollArea::FLAG_UNFOCUS_FONTSIZE, 16);
    ui->scroll_minute->SetFlag(ItemScrollArea::FLAG_ITEMBUTTON, false);
    ui->scroll_minute->SetFlag(ItemScrollArea::FLAG_SELECTOR, true);
    QStringList tmpMinuteList;
    tmpMinuteList.append(" ");
    for(int i=0;i<60;i++)
        tmpMinuteList.append(QString::number(i));
    tmpMinuteList.append(" ");
    ui->scroll_minute->Init(tmpMinuteList, D_MINUTE_NUM(mOriginDateTime.time().hour()));

    mOriginVolAudio = 100;
    ui->scroll_volume_audio->Init();
    ui->scroll_volume_audio->SetFlag(ItemScrollArea::FLAG_ITEMHEIGHT, D_NUM_ITEM_HEIGHT);
    ui->scroll_volume_audio->SetFlag(ItemScrollArea::FLAG_FOCUS_FONTSIZE, 25);
    ui->scroll_volume_audio->SetFlag(ItemScrollArea::FLAG_UNFOCUS_FONTSIZE, 23);
    ui->scroll_volume_audio->SetFlag(ItemScrollArea::FLAG_ITEMBUTTON, false);
    ui->scroll_volume_audio->SetFlag(ItemScrollArea::FLAG_SELECTOR, true);
    QStringList tmpVolumeList;
    tmpVolumeList.append(" ");
    for(int i=0;i<=100;i++)
        tmpVolumeList.append(QString::number(i));
    tmpVolumeList.append(" ");
    ui->scroll_volume_audio->Init(tmpVolumeList, D_VOLUME_NUM(mOriginVolAudio));

    mOriginVolVideo = 100;
    ui->scroll_volume_video->Init();
    ui->scroll_volume_video->SetFlag(ItemScrollArea::FLAG_ITEMHEIGHT, D_NUM_ITEM_HEIGHT);
    ui->scroll_volume_video->SetFlag(ItemScrollArea::FLAG_FOCUS_FONTSIZE, 25);
    ui->scroll_volume_video->SetFlag(ItemScrollArea::FLAG_UNFOCUS_FONTSIZE, 23);
    ui->scroll_volume_video->SetFlag(ItemScrollArea::FLAG_ITEMBUTTON, false);
    ui->scroll_volume_video->SetFlag(ItemScrollArea::FLAG_SELECTOR, true);
    tmpVolumeList.clear();
    tmpVolumeList.append(" ");
    for(int i=0;i<=100;i++)
        tmpVolumeList.append(QString::number(i));
    tmpVolumeList.append(" ");
    ui->scroll_volume_video->Init(tmpVolumeList, D_VOLUME_NUM(mOriginVolVideo));

    mOriginLuminance = 100;
    ui->scroll_backlight->Init();
    ui->scroll_backlight->SetFlag(ItemScrollArea::FLAG_ITEMHEIGHT, D_NUM_ITEM_HEIGHT);
    ui->scroll_backlight->SetFlag(ItemScrollArea::FLAG_FOCUS_FONTSIZE, 25);
    ui->scroll_backlight->SetFlag(ItemScrollArea::FLAG_UNFOCUS_FONTSIZE, 23);
    ui->scroll_backlight->SetFlag(ItemScrollArea::FLAG_ITEMBUTTON, false);
    ui->scroll_backlight->SetFlag(ItemScrollArea::FLAG_SELECTOR, true);
    QStringList tmpBlList;
    tmpBlList.append(" ");
    for(int i=0;i<=100;i++)
        tmpBlList.append(QString::number(i));
    tmpBlList.append(" ");
    ui->scroll_backlight->Init(tmpBlList, D_BACKLIGHT_NUM(mOriginLuminance));

    mOriginType = D_ELE_TYPE_CAN;
    ui->scroll_protocol_type->Init();
    ui->scroll_protocol_type->SetFlag(ItemScrollArea::FLAG_ITEMHEIGHT, D_NUM_ITEM_HEIGHT);
    ui->scroll_protocol_type->SetFlag(ItemScrollArea::FLAG_FOCUS_FONTSIZE, 20);
    ui->scroll_protocol_type->SetFlag(ItemScrollArea::FLAG_UNFOCUS_FONTSIZE, 18);
    ui->scroll_protocol_type->SetFlag(ItemScrollArea::FLAG_ITEMBUTTON, false);
    ui->scroll_protocol_type->SetFlag(ItemScrollArea::FLAG_SELECTOR, true);
    QStringList tmpProtocolList;
    tmpProtocolList.append(" ");
    tmpProtocolList.append("CAN");
    tmpProtocolList.append("485");
    tmpProtocolList.append(" ");
    ui->scroll_protocol_type->Init(tmpProtocolList, D_PROTYPE_NUM(mOriginType));

    mOriginBuad = 50;
    ui->scroll_protocol_buad->Init();
    ui->scroll_protocol_buad->SetFlag(ItemScrollArea::FLAG_ITEMHEIGHT, D_NUM_ITEM_HEIGHT);
    ui->scroll_protocol_buad->SetFlag(ItemScrollArea::FLAG_FOCUS_FONTSIZE, 18);
    ui->scroll_protocol_buad->SetFlag(ItemScrollArea::FLAG_UNFOCUS_FONTSIZE, 16);
    ui->scroll_protocol_buad->SetFlag(ItemScrollArea::FLAG_ITEMBUTTON, false);
    ui->scroll_protocol_buad->SetFlag(ItemScrollArea::FLAG_SELECTOR, true);
    mBuadList.clear();
    mBuadList.append(" ");
    mBuadList << "20"
            << "50"
            << "100"
            << "200";
    mBuadList.append(" ");
    ui->scroll_protocol_buad->Init(mBuadList, D_PROBUAD_NUM(mOriginBuad));

    mOriginID = 1;
    ui->scroll_protocol_id->Init();
    ui->scroll_protocol_id->SetFlag(ItemScrollArea::FLAG_ITEMHEIGHT, D_NUM_ITEM_HEIGHT);
    ui->scroll_protocol_id->SetFlag(ItemScrollArea::FLAG_FOCUS_FONTSIZE, 18);
    ui->scroll_protocol_id->SetFlag(ItemScrollArea::FLAG_UNFOCUS_FONTSIZE, 16);
    ui->scroll_protocol_id->SetFlag(ItemScrollArea::FLAG_ITEMBUTTON, false);
    ui->scroll_protocol_id->SetFlag(ItemScrollArea::FLAG_SELECTOR, true);
    QStringList tmpProtocolIDList;
    tmpProtocolIDList.append(" ");
    for(int i=1;i<=42;i++)
        tmpProtocolIDList.append(QString::number(i));
    tmpProtocolIDList.append(" ");
    ui->scroll_protocol_id->Init(tmpProtocolIDList, D_PROID_NUM(mOriginID));
}

StackedMenu::~StackedMenu()
{
    delete ui;
}

void StackedMenu::SyncDateTime(QDateTime pDateTime)
{
    if(mOriginDateTime != pDateTime)
    {
        mOriginDateTime = pDateTime;

        ui->scroll_year->SetCurrentIndex(D_YEAR_NUM(pDateTime.date().year()));
        ui->scroll_month->SetCurrentIndex(D_MONTH_NUM(pDateTime.date().month()));
        ui->scroll_day->SetCurrentIndex(D_DAY_NUM(pDateTime.date().day()));

        ui->scroll_hour->SetCurrentIndex(D_HOUR_NUM(pDateTime.time().hour()));
        ui->scroll_minute->SetCurrentIndex(D_MINUTE_NUM(pDateTime.time().minute()));
    }
}

void StackedMenu::SyncLuminance(int pValue)
{
    if(mOriginLuminance != pValue)
    {
        mOriginLuminance = pValue;
        ui->scroll_backlight->SetCurrentIndex(D_BACKLIGHT_NUM(pValue));
    }
}

void StackedMenu::SyncVolume(int pAudio, int pVideo)
{
    if(mOriginVolAudio != pAudio)
    {
        mOriginVolAudio = pAudio;
        ui->scroll_volume_audio->SetCurrentIndex(D_VOLUME_NUM(pAudio));
    }
    if(mOriginVolVideo != pVideo)
    {
        mOriginVolVideo = pVideo;
        ui->scroll_volume_video->SetCurrentIndex(D_VOLUME_NUM(pVideo));
    }
}

void StackedMenu::SyncProtocol(int pType, int pBuad, int pID)
{
    if(mOriginType != pType)
    {
        mOriginType = pType;
        ui->scroll_protocol_type->SetCurrentIndex(D_PROTYPE_NUM(pType));
    }
    if(mOriginBuad != pBuad)
    {
        mOriginBuad = pBuad;
        ui->scroll_protocol_buad->SetCurrentIndex(D_PROBUAD_NUM(pBuad));
    }
    if(mOriginID != pID)
    {
        mOriginID = pID;
        ui->scroll_protocol_id->SetCurrentIndex(D_PROID_NUM(pID));
    }
}

void StackedMenu::slot_MenuItemClicked(int id)
{
    setCurrentIndex(1);
    ui->stacked_menu_paras->setCurrentIndex(id);
    ui->label_title->setText(mMenuTextList.at(id));
}

void StackedMenu::SetGeometry(QRect pRect)
{
    setGeometry(pRect);
}

void StackedMenu::on_btn_back_clicked()
{
    setCurrentIndex(0);
}

void StackedMenu::on_btn_saveexit_clicked()
{
    if(mParent)
    {
        int year = ui->scroll_year->GetCurrentText().toInt();
        int month = ui->scroll_month->GetCurrentText().toInt();
        int day = ui->scroll_day->GetCurrentText().toInt();
        int hour = ui->scroll_hour->GetCurrentText().toInt();
        int minute = ui->scroll_minute->GetCurrentText().toInt();
        QDateTime tmpDateTime = QDateTime(QDate(year,month,day),QTime(hour,minute));
        if(mOriginDateTime != tmpDateTime)
            mParent->SyncTime(tmpDateTime);
        IDE_DEBUG(tmpDateTime.toString("yyyy-MM-dd hh:mm:ss"));

        int backlight = ui->scroll_backlight->GetCurrentText().toInt();
        if(backlight != mOriginLuminance)
            mParent->SyncLuminance(backlight);
        IDE_TRACE_INT(backlight);

        int volume_audio = ui->scroll_volume_audio->GetCurrentText().toInt();
        int volume_video = ui->scroll_volume_video->GetCurrentText().toInt();
        if(volume_audio != mOriginVolAudio || volume_video != mOriginVolVideo)
            mParent->SyncVolume(volume_audio, volume_video);
        IDE_TRACE_INT(volume_audio);
        IDE_TRACE_INT(volume_video);

        QString tmpType = ui->scroll_protocol_type->GetCurrentText();
        int type;
        if(!tmpType.compare("CAN", Qt::CaseInsensitive))
            type = D_ELE_TYPE_CAN;
        else
            type = D_ELE_TYPE_485;
        int buad = ui->scroll_protocol_buad->GetCurrentText().toInt();
        int id = ui->scroll_protocol_id->GetCurrentText().toInt();
        if(type != mOriginType || buad != mOriginBuad || id != mOriginID)
        {
            mParent->SyncProtocol(type, buad, id);
        }
        IDE_TRACE();

        if(mParent->mMdpParser)
            mParent->mMdpParser->SaveMDP();
    }
    emit sHide();
}

void StackedMenu::on_btn_exit_clicked()
{
    emit sHide();
}
