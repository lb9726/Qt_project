#include "icmanager.h"
#include "ui_icmanager.h"
#include "usritem.h"
#include "adminitem.h"
#include "carditem.h"

ICManager::ICManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ICManager)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);

    mSqlParser = 0;
    resetPassword = false;

    connect(&mTickTimer, SIGNAL(timeout()), this, SLOT(slot_Ticks()));
    mTickTimer.setInterval(1000);
    mTickTimer.start();

    connect(&mAutoHideMsgPrg, SIGNAL(timeout()), this, SLOT(slot_AutoHideMsgPrg()));
    mAutoHideMsgPrg.setInterval(1000);
    mAutoHideMsgPrg.setSingleShot(true);

    slot_AutoHideMsgPrg();

    Init_user_table();
    Init_admin_table();
    Init_cardlist_table();
    Init_cardlog_table();
    Init_cellInfo_table();
    Init_elevator_table();
    Init_map_table();

    mMainMenuBtns.addButton(ui->btn_user, 0);
    mMainMenuBtns.addButton(ui->btn_admin, 1);
    mMainMenuBtns.addButton(ui->btn_cardlist, 2);
    mMainMenuBtns.addButton(ui->btn_cardlog, 3);
    mMainMenuBtns.addButton(ui->btn_makeownercard, 4);
    mMainMenuBtns.addButton(ui->btn_makeadmincard,5);
    mMainMenuBtns.addButton(ui->btn_maketoolcard,6);
    mMainMenuBtns.addButton(ui->btn_losscard,7);
    mMainMenuBtns.addButton(ui->btn_remakecard,8);
    mMainMenuBtns.addButton(ui->btn_cellinfo, 9);
    mMainMenuBtns.addButton(ui->btn_elevatorinfo, 10);
    connect(&mMainMenuBtns, SIGNAL(buttonClicked(int)), this, SLOT(slot_MainMenuClicked(int)));

/*    mUsrOperateBtns.addButton(ui->btn_user_add, 0);
    mUsrOperateBtns.addButton(ui->btn_user_del, 1);
    connect(&mUsrOperateBtns, SIGNAL(buttonClicked(int)), this, SLOT(slot_UsrOperateClicked(int)));

    mAdminOperateBtns.addButton(ui->btn_admin_edit, 0);
    mAdminOperateBtns.addButton(ui->btn_admin_logoff, 1);
    connect(&mAdminOperateBtns, SIGNAL(buttonClicked(int)), this, SLOT(slot_AdminOperateClicked(int)));
*/
    mMaketoolcardBtns.addButton(ui->btn_maketoolcard_syntime, 0);
    mMaketoolcardBtns.addButton(ui->btn_maketoolcard_readcard, 1);
    mMaketoolcardBtns.addButton(ui->btn_maketoolcard_maskroom, 2);
    mMaketoolcardBtns.addButton(ui->btn_maketoolcard_openfloor, 3);
    mMaketoolcardBtns.addButton(ui->btn_maketoolcard_elevtimeallow, 4);
    mMaketoolcardBtns.addButton(ui->btn_maketoolcard_acticatecard,5);
    mMaketoolcardBtns.addButton(ui->btn_maketoolcard_elevNum,6);
    mMaketoolcardBtns.addButton(ui->btn_maketoolcard_lostCart,7);
    connect(&mMaketoolcardBtns, SIGNAL(buttonClicked(int)),this, SLOT(slot_MaketoolcardClicked(int)));

    InitFromDB();

    GotoPage(E_PAGE_CARDLIST);

    ui->combo_cardfee_type->setCurrentIndex(1);
    ui->checkBox_available_time->setChecked(true);
    ui->checkBox_admin_cardValidtime->setChecked(true);
    ui->checkBox_admin_availabletime->setChecked(true);

    ui->checkBox_time1->setChecked(true);
    ui->checkBox_time2->setChecked(true);
    ui->checkBox_time3->setChecked(true);

    ui->table_elevator->sortItems(0, Qt::AscendingOrder);
    qSort(mElevatorList.begin(), mElevatorList.end());
    memset(&usrOpenFloors,0,sizeof(UsrCard_OpenFloor));
}

ICManager::~ICManager()
{
    delete ui;
}

void ICManager::on_btn_back_clicked()
{
    if(mPageList.count() > 1)
    {
        mPageList.takeLast();
        GotoPage(mPageList.last(), false);
    }
    else
    {
        GotoPage(mPageList.last(), false);
    }
}

void ICManager::slot_AutoHideMsgPrg()
{
    ui->wid_prgmsg->hide();
}

void ICManager::ShowMsgPrg(QString pMsg, int pPrg)
{
    if(mAutoHideMsgPrg.isActive())
        mAutoHideMsgPrg.stop();
    if(!ui->wid_prgmsg->isVisible())
        ui->wid_prgmsg->show();
    ui->label_msg->setText(pMsg);
    ui->progress->setValue(pPrg);
    if(pPrg >= 100)
        mAutoHideMsgPrg.start();
}

void ICManager::GotoPage(int pIndex, bool pNew)
{
    switch(pIndex)
    {
        case E_PAGE_USER:
            ui->stacked_pages->setCurrentWidget(ui->page_user);
            ui->btn_user->setChecked(true);
           // ui->stacked_useredit->setCurrentWidget(ui->page_user_add);
            break;
        case E_PAGE_ADMIN:
            ui->stacked_pages->setCurrentWidget(ui->page_admin);
            ui->btn_admin->setChecked(true);
            break;
        case E_PAGE_CARDLIST:
            ui->stacked_pages->setCurrentWidget(ui->page_cardlist);
            ui->btn_cardlist->setChecked(true);
            break;
        case E_PAGE_CARDLOG:
            ui->stacked_pages->setCurrentWidget(ui->page_cardlog);
            ui->btn_cardlog->setChecked(true);
            break;
        case E_PAGE_MAKEOWERCARD:
            ui->stacked_pages->setCurrentWidget(ui->page_makeownercard);
            ui->btn_makeownercard->setChecked(true);
            break;
        case E_PAGE_MAKEADMINCARD:
            ui->stacked_pages->setCurrentWidget(ui->page_makeadmincard);
            ui->btn_makeadmincard->setChecked(true);
            break;
        case E_PAGE_MAKETOOLCARD:
            ui->stacked_pages->setCurrentWidget(ui->page_maketoolcard);
            ui->btn_maketoolcard->setChecked(true);
            break;
        case E_PAGE_LOSSCARD:
            ui->stacked_pages->setCurrentWidget(ui->page_losscard);
            ui->btn_losscard->setChecked(true);
            break;
        case E_PAGE_REMAKECARD:
            ui->stacked_pages->setCurrentWidget(ui->page_remakecard);
            ui->btn_remakecard->setChecked(true);
            break;
        case E_PAGE_CELLINFO:
            ui->stacked_pages->setCurrentWidget(ui->page_cellinfo);
            ui->btn_cellinfo->setChecked(true);
            break;
        case E_PAGE_ELEVATOR:
            ui->stacked_pages->setCurrentWidget(ui->page_elevator);
            ui->btn_elevatorinfo->setChecked(true);
            break;
        default:
            return;
    }
    if(pNew)
    {
        if(!mPageList.isEmpty())
        {
            if(mPageList.last() == pIndex)
                return;
        }
        mPageList.append(pIndex);
    }
}

void ICManager::SetPasswd(QString pText)
{
    QList<QVariant> tmpFieldVaules;
    tmpFieldVaules << "passwd" << pText;
    if(!mSqlParser->ModifyRow("information", "type", "passwd", tmpFieldVaules))
    {
        QMessageBox::critical(0, GBKToUTF8("修改密码错误"),
                              GBKToUTF8("无法修改密码，请检查填写是否正确"));
        return;
    }
    mPasswd = pText;
    ui->line_business_password->setText(mPasswd);
    ui->line_makeadmin_password->setText(mPasswd);
    ui->line_maketoolcard_password->setText(mPasswd);
    ui->line_cardloss_password->setText(mPasswd);
}

void ICManager::InitFlrMap(QString pUnitNum)//初始化楼层映射，根据一个单元号来查询出该单元下的楼层及楼层映射，并插入哈希表，下次可以直接使用，不必每次到数据库中查询
{
    if(mFlrMapUnitNum.compare(pUnitNum) == 0) //如果某一单元号被查询过，就不需要重复查
        return;
    mFlrMaps.clear();
    mFlrMapUnitNum = pUnitNum;

    QSqlQuery tmpCellLayersQuery = mSqlParser->Exec(QString("select * from addr_layers where unit_num=\"%1\"").arg(pUnitNum));
    while(tmpCellLayersQuery.next())
    {
        QSqlRecord tmpCellLayersRecord = tmpCellLayersQuery.record();
        QString tmpLayersNum = tmpCellLayersRecord.value("layer_num").toString();
        QString tmpLayersMap = tmpCellLayersRecord.value("layer_map").toString();
        if(tmpLayersMap.isEmpty())
            continue;
        mFlrMaps.insert(tmpLayersNum, tmpLayersMap);
    }
}

QString ICManager::GetFlrMap(QString pFlrNum)
{
    QString tmpMap = mFlrMaps.value(pFlrNum);
    if(tmpMap.isEmpty())
        tmpMap = pFlrNum;
    return tmpMap;
}

QString ICManager::GetFlrNum(QString pFlrMap)
{
    QString tmpNum = mFlrMaps.key(pFlrMap);
    if(tmpNum.isEmpty())
        tmpNum = pFlrMap;
    return tmpNum;
}

void ICManager::InitFromDB()
{
    mSqlParser = new SQL_PARSER(this);
    if(!mSqlParser->open(qApp->applicationDirPath(), QString("/Resource/IC_DB.db")))
    {
        QMessageBox::information(0, GBKToUTF8("数据库操作"), GBKToUTF8("打开数据库失败"));
        qApp->exit();
        return;
    }

    QSqlQuery tmpPasswdQuery = mSqlParser->Exec(QString("select value from information where type=\"passwd\""));
    if(tmpPasswdQuery.next())
    {
        QSqlRecord tmpRecord = tmpPasswdQuery.record();
        mPasswd = tmpRecord.value("value").toString();
    }

    QSqlQuery tmpUserQuery = mSqlParser->Exec("select * from person_user");
    while(tmpUserQuery.next())
    {
        QSqlRecord tmpRecord = tmpUserQuery.record();
        QString tmpName = tmpRecord.value("name").toString();
        QString tmpPhone = tmpRecord.value("phone").toString();
        QString tmpFamilyNum = tmpRecord.value("family_num").toString();
        QString tmpUnitNum = tmpRecord.value("unit_num").toString();

        InitFlrMap(tmpUnitNum);
        QString tmpLayerNum = tmpRecord.value("layer_num").toString();
        tmpLayerNum = GetFlrMap(tmpLayerNum);

        QString tmpAddress = QString(GBKToUTF8("%1单元%2层%3室")).arg(tmpUnitNum).arg(tmpLayerNum).arg(tmpFamilyNum);

        int tmpCardNum = 0;
        QSqlQuery tmpNumQuery = mSqlParser->Exec(QString("select card_num from card_list where name=\"%1\"").arg(tmpName));
        while(tmpNumQuery.next())
            tmpCardNum++;

        QString tmpDetail = tmpRecord.value("details").toString();

        AddUser(tmpName,tmpAddress,tmpPhone,tmpCardNum,tmpDetail);  //添加业主的记录
    }

    connect(ui->table_user, SIGNAL(cellClicked(int, int)), this, SLOT(slot_TableUser_Click(int, int)));


    QSqlQuery tmpAdminQuery = mSqlParser->Exec("select * from person_admin");//查询数据库中所有记录
    while(tmpAdminQuery.next())
    {
        QSqlRecord tmpRecord = tmpAdminQuery.record();        //当前第一条记录
        QString tmpName = tmpRecord.value("name").toString();  //分别取出这条记录的每一个字段内容
        QString tmpPhone = tmpRecord.value("phone").toString();
        QString tmpAddress = tmpRecord.value("address").toString();

        int tmpCardNum = 0;
        QSqlQuery tmpNumQuery = mSqlParser->Exec(QString("select card_num from card_list where maker=\"%1\"").arg(tmpName));
        while(tmpNumQuery.next())
            tmpCardNum++;

        QString tmpDetail = tmpRecord.value("details").toString();

        AddAdmin(tmpName,tmpAddress,tmpPhone,tmpCardNum,tmpDetail);  //添加管理员的记录
    }

    connect(ui->table_admin, SIGNAL(cellClicked(int,int)),this, SLOT(slot_TableAdmin_Click(int,int)));


    QSqlQuery tmpCardListQuery = mSqlParser->Exec("select * from card_list");
    while(tmpCardListQuery.next())
    {
        QSqlRecord tmpCardListRecord = tmpCardListQuery.record();
        QString tmpNum = tmpCardListRecord.value("card_num").toString();

        QString tmpType = tmpCardListRecord.value("card_type").toString();
        QSqlQuery tmpCardTypeQuery = mSqlParser->Exec(QString("select * from card_type where type=\"%1\"").arg(tmpType));
        QString tmpDescription;
        if(tmpCardTypeQuery.next())
        {
            QSqlRecord tmpCardTypeRecord = tmpCardTypeQuery.record();
            tmpDescription = tmpCardTypeRecord.value("description").toString();
        }

        QString tmpName = tmpCardListRecord.value("name").toString();
        QString tmpLimits = tmpCardListRecord.value("limits").toString();
        QString tmpState = tmpCardListRecord.value("state").toString();
        QString tmpMaker = tmpCardListRecord.value("maker").toString();

        AddCardlist(tmpNum,tmpDescription,tmpName,tmpLimits,tmpState,tmpMaker);//添加卡列表的记录
    }

   // connect(ui->table_cardlist, SIGNAL(cellClicked(int,int)), this, SLOT(slot_TableCardList_Click(int,int)));

    QSqlQuery tmpCardLogQuery = mSqlParser->Exec("select * from card_logs");
    while(tmpCardLogQuery.next())
    {
        QSqlRecord tmpCardLogRecord = tmpCardLogQuery.record();
        QString tmpType = tmpCardLogRecord.value("type").toString();
        QSqlQuery tmpCardTypeQuery = mSqlParser->Exec(QString("select * from card_type where type=\"%1\"").arg(tmpType));
        QString tmpDescription;
        if(tmpCardTypeQuery.next())
        {
            QSqlRecord tmpCardTypeRecord = tmpCardTypeQuery.record();
            tmpDescription = tmpCardTypeRecord.value("description").toString();
        }

        QString tmpNum = tmpCardLogRecord.value("card_num").toString();
        QSqlQuery tmpNameQuery = mSqlParser->Exec(QString("select name from card_list where card_num=\"%1\"").arg(tmpNum));
        QString tmpName;
        if(tmpNameQuery.next())
        {
            QSqlRecord tmpNameRecord = tmpNameQuery.record();
            tmpName = tmpNameRecord.value("name").toString();
        }
        QDateTime tmpTime = tmpCardLogRecord.value("time").toDateTime();
        QString tmpMode = tmpCardLogRecord.value("contents").toString();

        AddCardLog(tmpDescription,tmpNum,tmpName,tmpTime,tmpMode);//添加卡日志记录
    }


    QString tmpLayersNum;
    QString tmpFamilysNum;
    QString tmpUnitsNum;
    QSqlQuery tmpCellUnitsQuery = mSqlParser->Exec("select * from addr_units");
    while(tmpCellUnitsQuery.next())
    {
        QSqlRecord tmpCellUnitsRecord = tmpCellUnitsQuery.record();
        tmpUnitsNum = tmpCellUnitsRecord.value("unit_num").toString();
        ui->combo_unitnum->addItem(tmpUnitsNum);     //查询到的单元号依次放入combox中
        ui->comboBox_toolcard_units->addItem(tmpUnitsNum);
        ui->combox_user_unit_num->addItem(tmpUnitsNum);
        mCellUnitList.append(tmpUnitsNum);

        InitFlrMap(tmpUnitsNum);

        QSqlQuery tmpCellLayersQuery = mSqlParser->Exec(QString("select * from addr_layers where unit_num=\"%1\"").arg(tmpUnitsNum));
        while(tmpCellLayersQuery.next())
        {
            QSqlRecord tmpCellLayersRecord = tmpCellLayersQuery.record();
            tmpLayersNum = tmpCellLayersRecord.value("layer_num").toString();
            QSqlQuery tmpCellFamilysQuery = mSqlParser->Exec(QString("select * from addr_familys where layer_num=\"%1\" and unit_num=\"%2\"").arg(tmpLayersNum).arg(tmpUnitsNum));

            //做一下楼层映射
            tmpLayersNum = GetFlrMap(tmpLayersNum);

            while(tmpCellFamilysQuery.next())
            {
                QSqlRecord tmpCellFamilysRecord = tmpCellFamilysQuery.record();
                tmpFamilysNum = tmpCellFamilysRecord.value("family_num").toString();

                AddCellInfo(tmpUnitsNum,tmpLayersNum,tmpFamilysNum);
            }
        }
    }
    connect(ui->table_cellinfo,SIGNAL(cellClicked(int,int)),this,SLOT(slot_TableCellInfo_click(int,int)));


    QSqlQuery tmpElevatorQuery = mSqlParser->Exec("select * from elevator");
    while(tmpElevatorQuery.next())
    {
        QSqlRecord tmpElevatorRecord = tmpElevatorQuery.record();
        QString tmpElevNum = tmpElevatorRecord.value("elev_num").toString();
        QString tmpUnitNum = tmpElevatorRecord.value("unit_num").toString();
        QString tmpCtrLayerCnt = tmpElevatorRecord.value("ctrlayer_cnt").toString();
        AddElevator(tmpElevNum,tmpUnitNum,tmpCtrLayerCnt);
    }
    connect(ui->table_elevator,SIGNAL(cellClicked(int,int)),this,SLOT(slot_TableElevator_click(int,int)));
}

void ICManager::SetComboxText(QComboBox *pBox, QString pText)
{
#if (QT_VERSION > QT_VERSION_CHECK(4, 9, 9))
    pBox->setCurrentText(pText);
#else
    int index = pBox->findText(pText);
    pBox->setCurrentIndex(index);
#endif
}

void ICManager::slot_Ticks()
{
    bool tmpReaderOpen = true;
    if(!mIcReader.IsOpen())
    {
        tmpReaderOpen = mIcReader.Open(QIODevice::ReadWrite);
        if(tmpReaderOpen)
            mIcReader.SetLedBeep(D_LED_RED, 0, 0, 0);  //红灯一直亮
        else
            mIcReader.SetLedBeep(0, 0, 0, 0); //一直不亮
    }
    if(tmpReaderOpen)
    {
        int tmpDetectRet = mIcReader.DetectCard();
        if(tmpDetectRet == D_RET_NEWCARD)     //卡刚放上
        {
            mIcReader.SetLedBeep(D_BEEP|D_LED_GREEN, 50, 50, 1); //蜂鸣器叫、绿灯亮，500ms亮接着500ms灭，循环一次

            //进入卡列表界面
            GotoPage(E_PAGE_CARDLIST);

            char tmpBuffer[48];
            //读取卡片数据，并初始化界面
            QByteArray tmpRet = mIcReader.ReadCard(1, 48);
            memcpy(tmpBuffer, tmpRet.constData(), 48);

            QString tmpCardNum = mIcReader.GetCardNum();

            ui->line_business_cardnum->setText(tmpCardNum);
            ui->line_business_password->setText(mPasswd);
            ui->line_makeadmin_cardnum->setText(tmpCardNum);
            ui->line_makeadmin_password->setText(mPasswd);
            ui->line_maketoolcard_cardnum->setText(tmpCardNum);
            ui->line_maketoolcard_password->setText(mPasswd);
            ui->line_cardloss_cardnum->setText(tmpCardNum);
            ui->line_cardloss_password->setText(mPasswd);

            QString tmpName;
            QSqlQuery tmpQuery = mSqlParser->Exec(QString("select name from card_list where card_num=\"%1\"")
                                                .arg(tmpCardNum));
            if(tmpQuery.next())
            {
                QSqlRecord tmpRecord = tmpQuery.record();
                tmpName = tmpRecord.value("name").toString();
            }

            unsigned char tmpType = tmpBuffer[4];  //卡类型为第四个字节

            if(tmpType < 100)
            {
                if(tmpType == 1)  //业主卡
                {
                    GotoPage(E_PAGE_MAKEOWERCARD);

                    UsrCard* tmpData = (UsrCard*)tmpBuffer;

                    CardPasswd *tmpPw = (CardPasswd *) tmpData->mPasswd; //密码
                    QString tmpPasswd = QString("%1%2%3%4%5%6")
                                                .arg(tmpPw->mPwList.mPw1)
                                                .arg(tmpPw->mPwList.mPw2)
                                                .arg(tmpPw->mPwList.mPw3)
                                                .arg(tmpPw->mPwList.mPw4)
                                                .arg(tmpPw->mPwList.mPw5)
                                                .arg(tmpPw->mPwList.mPw6);

                    ui->line_business_password->setText(tmpPasswd);

                    ui->line_business_name->setText(tmpName);//根据卡号得姓名

                    CardFreeFee *tmpFreeFee = (CardFreeFee *) tmpData->mMoney; //卡内余额
                    ui->lineEdit_freefee->setText(QString("%1").arg(tmpFreeFee->mShortFee));

                    ui->combo_cardfee_type->setCurrentIndex((int)tmpData->mChargeType); //收费类型
                    ui->lineEdit_chargestandard->setText(QString("%1").arg((int)tmpData->mPrice));

//                    SetComboxText(ui->combo_validelevt1, (QString("%1").arg((int)tmpData->mValidEles[0])));
//                    SetComboxText(ui->combo_validelevt2, (QString("%1").arg((int)tmpData->mValidEles[1])));
//                    SetComboxText(ui->combo_validelevt3, (QString("%1").arg((int)tmpData->mValidEles[2])));
//                    SetComboxText(ui->combo_validelevt4, (QString("%1").arg((int)tmpData->mValidEles[3])));
//                    SetComboxText(ui->combo_validelevt5, (QString("%1").arg((int)tmpData->mValidEles[4])));
//                    SetComboxText(ui->combo_validelevt6, (QString("%1").arg((int)tmpData->mValidEles[5])));
//                    SetComboxText(ui->combo_validelevt7, (QString("%1").arg((int)tmpData->mValidEles[6])));
//                    SetComboxText(ui->combo_validelevt8, (QString("%1").arg((int)tmpData->mValidEles[7])));

                    DateTime *tmpDateTime = (DateTime *)tmpData->mValidStartDate;   //卡片有效日期起始
                    int tmpYear = tmpDateTime->mDateList.mYear + 2000;
                    int tmpMonth = tmpDateTime->mDateList.mMonth;
                    int tmpDay = tmpDateTime->mDateList.mDay;
                    QDate tmpNewDate;
                    tmpNewDate.setDate(tmpYear,tmpMonth,tmpDay);
                    ui->date_card_start->setDate(tmpNewDate);

                    tmpDateTime = (DateTime *)tmpData->mValidEndDate;  //卡片有效日期结束
                    tmpYear = tmpDateTime->mDateList.mYear + 2000;
                    tmpMonth = tmpDateTime->mDateList.mMonth;
                    tmpDay = tmpDateTime->mDateList.mDay;
                    tmpNewDate.setDate(tmpYear,tmpMonth,tmpDay);
                    ui->date_card_end->setDate(tmpNewDate);

                    ui->comboBox_calltype->setCurrentIndex(tmpData->mCallEleType);   //呼梯方式
                    //SetComboxText(ui->comboBox_response_time, (QString("%1").arg((int)tmpData->mResponseTime)));
                    ui->comboBox_response_time->setCurrentText((QString("%1").arg((int)tmpData->mResponseTime)));//响应时间

                    if(tmpData->mAvailableTime)  //卡可用时间段使能为1
                    {
                        ui->checkBox_available_time->setChecked(true);
                        ui->widget_available_time->setDisabled(false);

                        tmpDateTime = (DateTime *)tmpData->mValidStartTime;  //可用时间段起始
                        unsigned char tmpHour = tmpDateTime->mTimeList.mHour;
                        unsigned char tmpMinute = tmpDateTime->mTimeList.mMinute;
                        unsigned char tmpSecond = tmpDateTime->mTimeList.mSecond;
                        QTime tmpTime;
                        tmpTime.setHMS(tmpHour,tmpMinute,tmpSecond);
                        ui->time_card_start->setTime(tmpTime);

                        tmpDateTime = (DateTime *)tmpData->mValidEndTime;   //可用时间段结束
                        tmpHour = tmpDateTime->mTimeList.mHour;
                        tmpMinute = tmpDateTime->mTimeList.mMinute;
                        tmpSecond =  tmpDateTime->mTimeList.mSecond;
                        tmpTime.setHMS(tmpHour,tmpMinute,tmpSecond);
                        ui->time_card_end->setTime(tmpTime);

                        CardByte *tmpCardByte =(CardByte *)(&(tmpData->mValidWeek));   //周设置
                        ui->check_card_week_1->setChecked((bool)tmpCardByte->mBits.mBit0);
                        ui->check_card_week_2->setChecked((bool)tmpCardByte->mBits.mBit1);
                        ui->check_card_week_3->setChecked((bool)tmpCardByte->mBits.mBit2);
                        ui->check_card_week_4->setChecked((bool)tmpCardByte->mBits.mBit3);
                        ui->check_card_week_5->setChecked((bool)tmpCardByte->mBits.mBit4);
                        ui->check_card_week_6->setChecked((bool)tmpCardByte->mBits.mBit5);
                        ui->check_card_week_7->setChecked((bool)tmpCardByte->mBits.mBit6);
                    }
                    else
                    {
                        ui->checkBox_available_time->setChecked(false);
                        ui->widget_available_time->setDisabled(true);
                    }
                    ui->combox_floor_num->clear();
                    QList<int> itemFloor;
                    for(int i=0;i<8;i++)
                    {
                        QString tmpFloor = QString::number(tmpData->mValidEles[i]);
                        int floorNum = tmpFloor.toInt();
                        if(floorNum != 0)
                        {
                            itemFloor.append(floorNum);
                        }
                    }
                    for(int i=0;i<itemFloor.count();i++)
                    {
                        QString floorNum = QString::number(itemFloor.at(i));
                        ui->combox_floor_num->addItem(floorNum);
                    }

                    char tmpValidFlrBuffer[48];

                    QByteArray tmpRet = mIcReader.ReadCard(2, 48);     //读出第二个扇区的48个字节
                    memcpy(tmpValidFlrBuffer, tmpRet.constData(), 48); //拷贝到48个字节的数组中

                    SetCorrespdFlrs(0, tmpValidFlrBuffer);  //
                    SetCorrespdFlrs(1, tmpValidFlrBuffer+16);
                    SetCorrespdFlrs(2, tmpValidFlrBuffer+32);

                    tmpRet = mIcReader.ReadCard(3, 48);
                    memcpy(tmpValidFlrBuffer, tmpRet.constData(), 48);
                    SetCorrespdFlrs(3, tmpValidFlrBuffer);
                    SetCorrespdFlrs(4, tmpValidFlrBuffer+16);
                    SetCorrespdFlrs(5, tmpValidFlrBuffer+32);

                    tmpRet = mIcReader.ReadCard(4, 48);
                    memcpy(tmpValidFlrBuffer, tmpRet.constData(), 48);
                    SetCorrespdFlrs(6, tmpValidFlrBuffer);
                    SetCorrespdFlrs(7, tmpValidFlrBuffer+16);
                }
                else if(tmpType == 2)
                {
                    GotoPage(E_PAGE_MAKEADMINCARD);    //管理员卡

                    AdminCard *tmpData = (AdminCard *)tmpBuffer;

                    CardPasswd *tmpPw = (CardPasswd *) tmpData->mPasswd;
                    QString tmpPasswd = QString("%1%2%3%4%5%6")
                                                .arg(tmpPw->mPwList.mPw1)
                                                .arg(tmpPw->mPwList.mPw2)
                                                .arg(tmpPw->mPwList.mPw3)
                                                .arg(tmpPw->mPwList.mPw4)
                                                .arg(tmpPw->mPwList.mPw5)
                                                .arg(tmpPw->mPwList.mPw6);

                    ui->line_makeadmin_password->setText(tmpPasswd);

                    ui->line_makeadmin_name->setText(tmpName);

                    if(tmpData->mValidDateEnable == 1)
                    {
                        ui->checkBox_admin_cardValidtime->setChecked(true);
                        ui->widget_admin_cardvalidtime->setDisabled(false);

                        DateTime *tmpDateTime = (DateTime *)tmpData->mValidStartDate;
                        int tmpYear = tmpDateTime->mDateList.mYear + 2000;
                        int tmpMonth = tmpDateTime->mDateList.mMonth;
                        int tmpDay = tmpDateTime->mDateList.mDay;
                        QDate tmpNewDate;
                        tmpNewDate.setDate(tmpYear,tmpMonth,tmpDay);
                        ui->date_admincard_start->setDate(tmpNewDate);

                        tmpDateTime = (DateTime *)tmpData->mValidEndDate;
                        tmpYear = tmpDateTime->mDateList.mYear + 2000;
                        tmpMonth = tmpDateTime->mDateList.mMonth;
                        tmpDay = tmpDateTime->mDateList.mDay;
                        tmpNewDate.setDate(tmpYear,tmpMonth,tmpDay);
                        ui->date_admincard_end->setDate(tmpNewDate);
                    }
                    else
                    {
                        ui->checkBox_admin_cardValidtime->setChecked(false);
                        ui->widget_admin_cardvalidtime->setDisabled(true);
                    }

                    if(tmpData->mAvailableTime == 1)
                    {
                        ui->checkBox_admin_availabletime->setChecked(true);
                        ui->widget_admin_availabletime->setDisabled(false);

                        DateTime *tmpDateTime = (DateTime *)tmpData->mValidStartTime;
                        unsigned char tmpHour = tmpDateTime->mTimeList.mHour;
                        unsigned char tmpMinute = tmpDateTime->mTimeList.mMinute;
                        unsigned char tmpSecond = tmpDateTime->mTimeList.mSecond;
                        QTime tmpTime;
                        tmpTime.setHMS(tmpHour,tmpMinute,tmpSecond);
                        ui->time_admincard_start->setTime(tmpTime);

                        tmpDateTime = (DateTime *)tmpData->mValidEndTime;
                        tmpHour = tmpDateTime->mTimeList.mHour;
                        tmpMinute = tmpDateTime->mTimeList.mMinute;
                        tmpSecond =  tmpDateTime->mTimeList.mSecond;
                        tmpTime.setHMS(tmpHour,tmpMinute,tmpSecond);
                        ui->time_admincard_end->setTime(tmpTime);

                        CardByte *tmpCardByte =(CardByte *)(&(tmpData->mValidWeek));
                        ui->check_admincard_week_1->setChecked((bool)tmpCardByte->mBits.mBit0);
                        ui->check_admincard_week_2->setChecked((bool)tmpCardByte->mBits.mBit1);
                        ui->check_admincard_week_3->setChecked((bool)tmpCardByte->mBits.mBit2);
                        ui->check_admincard_week_4->setChecked((bool)tmpCardByte->mBits.mBit3);
                        ui->check_admincard_week_5->setChecked((bool)tmpCardByte->mBits.mBit4);
                        ui->check_admincard_week_6->setChecked((bool)tmpCardByte->mBits.mBit5);
                        ui->check_admincard_week_7->setChecked((bool)tmpCardByte->mBits.mBit6);
                    }
                    else
                    {
                        ui->checkBox_admin_availabletime->setChecked(false);
                        ui->widget_admin_availabletime->setDisabled(true);
                    }

                }
            }
            else if(tmpType > 100)
            {
                GotoPage(E_PAGE_MAKETOOLCARD);

                ToolCard_Header *tmpHeader = (ToolCard_Header *)tmpBuffer;

                ui->comboBox_toolcard_units->setCurrentText(QString("%1").arg((int)tmpHeader->mUnit));
                //同步电梯
                int tmpIndex = 0;
                QList<QCheckBox*> tmpChdList = ui->scroll_unitElevt->findChildren<QCheckBox*>();
                QList<int> tmpList;
                for(int i=0;i<tmpChdList.count();i++)
                {
                    QCheckBox *tmpCheBox = tmpChdList.at(i);
                    if(tmpCheBox)
                    {
                        tmpCheBox->setChecked(false);
                        QString tmpText = tmpCheBox->text();//电梯001
                        QString tmpEle = tmpText.mid(2); //从第2个开始
                        int tmpEleNum = tmpEle.toInt(); //转换为int类型
                        tmpList.append(tmpEleNum); //001等数字编号list
                    }
                }
                while(tmpHeader->mElevator[tmpIndex])
                {
                    int tmpCkbIndex = tmpList.indexOf(tmpHeader->mElevator[tmpIndex]);
                    if(tmpCkbIndex >= 0)
                    {
                        QCheckBox *tmpCheBox = tmpChdList.at(tmpCkbIndex);
                        if(tmpCheBox)
                            tmpCheBox->setChecked(true);
                    }
                    tmpIndex++;
                }

                CardPasswd *tmpPw = (CardPasswd *) tmpHeader->mPasswd;
                QString tmpPasswd = QString("%1%2%3%4%5%6").arg((int)tmpPw->mPwList.mPw1)
                                                           .arg((int)tmpPw->mPwList.mPw2)
                                                           .arg((int)tmpPw->mPwList.mPw3)
                                                           .arg((int)tmpPw->mPwList.mPw4)
                                                           .arg((int)tmpPw->mPwList.mPw5)
                                                           .arg((int)tmpPw->mPwList.mPw6);

                ui->line_maketoolcard_password->setText(tmpPasswd);

                if(tmpType == 101)  //时间同步卡
                {
                    ToolCard_DateTimeSyn *tmpDTSyn = (ToolCard_DateTimeSyn *)tmpBuffer;

                    ui->stacked_maketoolcard->setCurrentWidget(ui->page_maketoolcard_syntime);

                    DateTime* tmpDataTime = (DateTime*)tmpDTSyn->mDateTimeSyn;
                    int tmpYear = tmpDataTime->mDateList.mYear+2000;
                    int tmpMonth = tmpDataTime->mDateList.mMonth;
                    int tmpDay = tmpDataTime->mDateList.mDay;
                    QDate tmpD;
                    tmpD.setDate(tmpYear,tmpMonth,tmpDay);

                    tmpDataTime = (DateTime*)(tmpDTSyn->mDateTimeSyn+3);
                    int tmpHour = tmpDataTime->mTimeList.mHour;
                    int tmpMinute = tmpDataTime->mTimeList.mMinute;
                    int tmpSecond = tmpDataTime->mTimeList.mSecond;
                    QTime tmpT;
                    tmpT.setHMS(tmpHour,tmpMinute,tmpSecond);

                    ui->dateEdit_syntime->setDate(tmpD);
                    ui->timeEdit_syntime->setTime(tmpT);
                }
                else if(tmpType == 102)  //卡记录同步
                {
                    ui->stacked_maketoolcard->setCurrentWidget(ui->page_maketoolcard_readcard);
                    ToolCard_CardRecord* tmpCardRecord = (ToolCard_CardRecord*)tmpBuffer;

                    //SetComboxText(ui->combo_readcard_selectfloor, (QString("%1").arg(tmpCardRecord->mFloor)));
                    ui->combo_readcard_selectfloor->setCurrentText(QString("%1").arg(tmpCardRecord->mFloor));

                    QString tmpName =QString::fromLocal8Bit(QByteArray((char*)tmpCardRecord->mName,8));
                    //SetComboxText(ui->combo_readcard_selectowner, (tmpString));
                    ui->line_readcard_selectowner->setText(tmpName);

                    DateTime* tmpDT =(DateTime*)(tmpCardRecord->mDateTimeStart);
                    int tmpYear = tmpDT->mDateList.mYear + 2000;
                    int tmpMonth = tmpDT->mDateList.mMonth;
                    int tmpDay = tmpDT->mDateList.mDay;
                    QDate tmpDate;
                    tmpDate.setDate(tmpYear,tmpMonth,tmpDay);
                    ui->dateEdit_readcard_start->setDate(tmpDate);

                    tmpDT =(DateTime*)(tmpCardRecord->mDateTimeStart+3);
                    int tmpHour = tmpDT->mTimeList.mHour;
                    int tmpMinute = tmpDT->mTimeList.mMinute;
                    QTime tmpTime;
                    tmpTime.setHMS(tmpHour,tmpMinute,0);
                    ui->timeEdit_readcard_start->setTime(tmpTime);

                    tmpDT =(DateTime*)(tmpCardRecord->mDateTimeEnd);
                    tmpYear = tmpDT->mDateList.mYear + 2000;
                    tmpMonth = tmpDT->mDateList.mMonth;
                    tmpDay = tmpDT->mDateList.mDay;
                    tmpDate.setDate(tmpYear,tmpMonth,tmpDay);
                    ui->dateEdit_readcard_end->setDate(tmpDate);

                    tmpDT =(DateTime*)(tmpCardRecord->mDateTimeEnd+3);
                    tmpHour = tmpDT->mTimeList.mHour;
                    tmpMinute = tmpDT->mTimeList.mMinute;
                    tmpTime.setHMS(tmpHour,tmpMinute,0);
                    ui->timeEdit_readcard_end->setTime(tmpTime);
                }
                else if(tmpType == 103)  //屏蔽房间卡
                {
                    ui->stacked_maketoolcard->setCurrentWidget(ui->page_maketoolcard_maskroom);
                    ToolCard_MaskRoom *tmpMaskRoom = (ToolCard_MaskRoom*)tmpBuffer;

                    //同步房间
                    QHash<int, QCheckBox*> tmpFindHash;
                    for(int i=0;i<ui->tabWidget_rooms->count();i++)  //tab页总数
                    {
                        QString tmpText = ui->tabWidget_rooms->tabText(i); //取出每个tab上的文本
                        QWidget *tmpWidget = ui->tabWidget_rooms->widget(i); //取出每个tab控件的widget
                        QList<QCheckBox*> tmpChdList = tmpWidget->findChildren<QCheckBox*>();//取出每个widget上所有checkbox
                        QString tmpLayer = tmpText.mid(0, tmpText.count()-1); //从第0个开始，取tmpText.count()-1长度的数据:几（层）

                        tmpLayer = GetFlrNum(tmpLayer);  //反映射

                        int tmpLayerNum = tmpLayer.toInt(); //转换为int类型
                        for(int j=0;j<tmpChdList.count();j++)//所有的checkbox
                        {
                            QCheckBox* tmpBtn = tmpChdList.at(j); //所有的checkbox一个一个取出来
                            QString tmpRoomText = tmpBtn->text();//获取checkbox上的文本
                            QString tmpRoom = tmpRoomText.mid(0, tmpRoomText.count()-1);//取文本几（室）
                            int tmpRoomNum = tmpRoom.toInt();
                            tmpFindHash.insert(((tmpLayerNum<<8)+tmpRoomNum), tmpBtn);
                        }
                    }
                    tmpIndex = 0;
                    while(tmpMaskRoom->mRoom[tmpIndex][1])
                    {
                        int tmpRoomNum = (tmpMaskRoom->mRoom[tmpIndex][0] << 8) + tmpMaskRoom->mRoom[tmpIndex][1];
                        QCheckBox* tmpBtn = tmpFindHash.take(tmpRoomNum);
                        if(tmpBtn)
                            tmpBtn->setChecked(true);
                        tmpIndex++;
                    }
                    foreach(QCheckBox* tmpBtn, tmpFindHash)
                    {
                        if(tmpBtn)
                            tmpBtn->setChecked(false);
                    }
                }
                else if(tmpType == 104)  //开放楼层卡
                {
                    ui->stacked_maketoolcard->setCurrentWidget(ui->page_maketoolcard_openfloor);
                    ToolCard_Openfloor *tmpOpenFlr = (ToolCard_Openfloor*)tmpBuffer;

                    QList<QCheckBox*> tmpChdList = ui->scrollContents_openfloor->findChildren<QCheckBox*>();//取出每个widget上所有checkbox
                    for(int j=0;j<tmpChdList.count();j++)//所有的checkbox
                    {
                        QCheckBox* tmpBtn = tmpChdList.at(j); //所有的checkbox一个一个取出来
                        QString tmpText = tmpBtn->text();//获取checkbox上的文本
                        QString tmpLayer = tmpText.mid(0, tmpText.count()-1);//取文本几（层）

                        tmpLayer = GetFlrNum(tmpLayer);  //反映射

                        int tmpLayerNum = tmpLayer.toInt();
                        int tmpValue = tmpLayerNum -1;
                        int column = tmpValue / 8;
                        int row = tmpValue % 8;
                        if(tmpOpenFlr->mOpenFloor[column] & (1<<row))
                        {
                            tmpBtn->setChecked(true);
                        }
                        else
                        {
                            tmpBtn->setChecked(false);
                        }
                    }
                }
                else if(tmpType == 105)
                {
                    ui->stacked_maketoolcard->setCurrentWidget(ui->page_maketoolcard_elevtimeallow);
                    ToolCard_Elevtimeallow* tmpElevtimeallow = (ToolCard_Elevtimeallow*)tmpBuffer;

                    if(tmpElevtimeallow->mTimeEnable1 == 1)
                    {
                        ui->checkBox_time1->setChecked(true);
                        ui->widget_time1->setDisabled(false);
                        DateTime* tmpDateTime = (DateTime *)tmpElevtimeallow->mTimeallowStart1;
                        int tmpHour = tmpDateTime->mTimeList.mHour;
                        int tmpMinute = tmpDateTime->mTimeList.mMinute;
                        int tmpSecond =  tmpDateTime->mTimeList.mSecond;
                        QTime tmpTime;
                        tmpTime.setHMS(tmpHour,tmpMinute,tmpSecond);
                        ui->timeEdit_timeallowstart1->setTime(tmpTime);

                        tmpDateTime = (DateTime *)tmpElevtimeallow->mTimeallowEnd1;
                        tmpHour = tmpDateTime->mTimeList.mHour;
                        tmpMinute = tmpDateTime->mTimeList.mMinute;
                        tmpSecond =  tmpDateTime->mTimeList.mSecond;
                        tmpTime.setHMS(tmpHour,tmpMinute,tmpSecond);
                        ui->timeEdit_timeallowend1->setTime(tmpTime);
                    }
                    else
                    {
                        ui->checkBox_time1->setChecked(false);
                        ui->widget_time1->setDisabled(true);
                    }

                    if(tmpElevtimeallow->mTimeEnable2 == 1)
                    {
                        ui->checkBox_time2->setChecked(true);
                        ui->widget_time2->setDisabled(false);
                        DateTime* tmpDateTime = (DateTime *)tmpElevtimeallow->mTimeallowStart2;
                        int tmpHour = tmpDateTime->mTimeList.mHour;
                        int tmpMinute = tmpDateTime->mTimeList.mMinute;
                        int tmpSecond =  tmpDateTime->mTimeList.mSecond;
                        QTime tmpTime;
                        tmpTime.setHMS(tmpHour,tmpMinute,tmpSecond);
                        ui->timeEdit_timeallowstart2->setTime(tmpTime);

                        tmpDateTime = (DateTime *)tmpElevtimeallow->mTimeallowEnd2;
                        tmpHour = tmpDateTime->mTimeList.mHour;
                        tmpMinute = tmpDateTime->mTimeList.mMinute;
                        tmpSecond =  tmpDateTime->mTimeList.mSecond;
                        tmpTime.setHMS(tmpHour,tmpMinute,tmpSecond);
                        ui->timeEdit_timeallowend2->setTime(tmpTime);
                    }
                    else
                    {
                        ui->checkBox_time2->setChecked(false);
                        ui->widget_time2->setDisabled(true);
                    }

                    if(tmpElevtimeallow->mTimeEnable3 == 1)
                    {
                        ui->checkBox_time3->setChecked(true);
                        ui->widget_time3->setDisabled(false);
                        DateTime* tmpDateTime = (DateTime *)tmpElevtimeallow->mTimeallowStart3;
                        int tmpHour = tmpDateTime->mTimeList.mHour;
                        int tmpMinute = tmpDateTime->mTimeList.mMinute;
                        int tmpSecond =  tmpDateTime->mTimeList.mSecond;
                        QTime tmpTime;
                        tmpTime.setHMS(tmpHour,tmpMinute,tmpSecond);
                        ui->timeEdit_timeallowstart3->setTime(tmpTime);

                        tmpDateTime = (DateTime *)tmpElevtimeallow->mTimeallowEnd3;
                        tmpHour = tmpDateTime->mTimeList.mHour;
                        tmpMinute = tmpDateTime->mTimeList.mMinute;
                        tmpSecond =  tmpDateTime->mTimeList.mSecond;
                        tmpTime.setHMS(tmpHour,tmpMinute,tmpSecond);
                        ui->timeEdit_timeallowend3->setTime(tmpTime);
                    }
                    else
                    {
                        ui->checkBox_time3->setChecked(false);
                        ui->widget_time3->setDisabled(true);
                    }

                    CardByte* tmpByte = (CardByte*)(&(tmpElevtimeallow->mWeek));
                    ui->check_week_1->setChecked((tmpByte->mBits.mBit0)?true:false);
                    ui->check_week_2->setChecked((tmpByte->mBits.mBit1)?true:false);
                    ui->check_week_3->setChecked((tmpByte->mBits.mBit2)?true:false);
                    ui->check_week_4->setChecked((tmpByte->mBits.mBit3)?true:false);
                    ui->check_week_5->setChecked((tmpByte->mBits.mBit4)?true:false);
                    ui->check_week_6->setChecked((tmpByte->mBits.mBit5)?true:false);
                    ui->check_week_7->setChecked((tmpByte->mBits.mBit6)?true:false);
                }
                else if(tmpType == 107)
                {
                    ui->stacked_maketoolcard->setCurrentWidget(ui->page_maketoolcard_activatevCard);
                }
                else if(tmpType == 108)
                {
                      ui->stacked_maketoolcard->setCurrentWidget(ui->page_maketoolcard_elevatorNnum);
                }
                else if(tmpType == 106)
                {
                    GotoPage(E_PAGE_LOSSCARD);
                    LossCard* tmpData = (LossCard*)tmpBuffer;

                    CardPasswd *tmpPw = (CardPasswd *) tmpData->mPasswd; //密码
                    QString tmpPasswd = QString("%1%2%3%4%5%6")
                                                .arg(tmpPw->mPwList.mPw1)
                                                .arg(tmpPw->mPwList.mPw2)
                                                .arg(tmpPw->mPwList.mPw3)
                                                .arg(tmpPw->mPwList.mPw4)
                                                .arg(tmpPw->mPwList.mPw5)
                                                .arg(tmpPw->mPwList.mPw6);

                    ui->line_maketoolcard_password->setText(tmpPasswd);
                    ui->lineEdit_Elevator_number->setText(QString("%1").arg(tmpData->mElevator));

                    int i = 0;
                    QString tmpNum;
                    while(i < 4 )
                    {
                        tmpNum.append(QString::number(tmpData->mUserId[i], 16));
                        tmpNum.append(QString(" "));
                        i++;
                    }
//                    QString cardId = QString("%1%2%3%4")
//                                                .arg(tmpData->mUserId[0])
//                                                .arg(tmpData->mUserId[1])
//                                                .arg(tmpData->mUserId[2])
//                                                .arg(tmpData->mUserId[3]);
                    ui->lineEdit_card_number->setText(tmpNum);
                    ui->line_LostCardNum->setText(tmpNum);

                }
            }
        }
        else if(tmpDetectRet == D_RET_NOCARD)   //卡刚拿走
        {
            mIcReader.SetLedBeep(D_BEEP|D_LED_RED, 50, 50, 1);//蜂鸣器叫、红灯亮一次

        }
        else      //读卡器上的卡一直在，或一直不在
        {
            if(!mIcReader.HasCard())
                mIcReader.SetLedBeep(D_LED_RED, 0, 0, 0);
            else
                mIcReader.SetLedBeep(D_LED_GREEN, 0, 0, 0);
        }
    }
}

/*void ICManager::slot_TableCardList_Click(int pRow, int pColumn)
{
    QTableWidgetItem *tmpItem = ui->table_cardlist->item(pRow, 2);
    ui->line_business_name->setText(tmpItem->text());

    tmpItem = ui->table_cardlist->item(pRow, 0);
    ui->line_business_cardnum->setText(tmpItem->text());

    QString tmpPassword;
    QSqlQuery tmpCardPaswdQuery = mSqlParser->Exec(QString("select password from card_list where card_num=\"%1\"").arg(tmpItem->text()));
    if(tmpCardPaswdQuery.next())
    {
        QSqlRecord tmpCardPaswdRecord = tmpCardPaswdQuery.record();
        tmpPassword = tmpCardPaswdRecord.value("password").toString();
    }
    ui->line_business_password->setText(tmpPassword);

}*/

void ICManager::slot_TableCellInfo_click(int pRow,int pColumn)
{
    QTableWidgetItem *tmpItem = ui->table_cellinfo->item(pRow, 0);
    ui->line_cellunit->setText(tmpItem->text());

    tmpItem = ui->table_cellinfo->item(pRow, 1);
    ui->line_celllayer->setText(tmpItem->text());

    tmpItem = ui->table_cellinfo->item(pRow, 2);
    ui->line_cellfamily->setText(tmpItem->text());
}

void ICManager::slot_TableAdmin_Click(int pRow, int pColumn)  //管理员页面
{
    QTableWidgetItem *tmpItem = ui->table_admin->item(pRow, 0);
    ui->line_admin_name->setText(tmpItem->text());
    QString tmpName = ui->line_admin_name->text();

    tmpItem = ui->table_admin->item(pRow, 1);
    ui->line_admin_address->setText(tmpItem->text());

    tmpItem = ui->table_admin->item(pRow, 2);
    ui->line_admin_phone->setText(tmpItem->text());

    QSqlQuery tmpPowerQuery = mSqlParser->Exec(QString("select power from person_admin where name=\"%1\"").arg(tmpName));
    while(tmpPowerQuery.next())
    {
        QSqlRecord tmpPowerRecord = tmpPowerQuery.record();
        int tmpPower = tmpPowerRecord.value("power").toInt();

        if(tmpPower & (0x01<<0))
        {
            ui->checkBox_adminPower_makelevatimeallow->setChecked(true);
        }
        else
        {
            ui->checkBox_adminPower_makelevatimeallow->setChecked(false);
        }

        if(tmpPower & (0x01<<1))
        {
            ui->checkBox_adminPower_makeopenfloorcard->setChecked(true);
        }
        else
        {
            ui->checkBox_adminPower_makeopenfloorcard->setChecked(false);
        }

        if(tmpPower & (0x01<<2))
        {
            ui->checkBox_adminPower_makemaskroomcard->setChecked(true);
        }
        else
        {
            ui->checkBox_adminPower_makemaskroomcard->setChecked(false);
        }
        if(tmpPower & (0x01<<3))
        {
            ui->checkBox_adminPower_makereadcard->setChecked(true);
        }
        else
        {
            ui->checkBox_adminPower_makereadcard->setChecked(false);
        }
        if(tmpPower & (0x01<<4))
        {
            ui->checkBox_adminPower_makesyntimecard->setChecked(true);
        }
        else
        {
            ui->checkBox_adminPower_makesyntimecard->setChecked(false);
        }
        if(tmpPower & (0x01<<5))
        {
            ui->checkBox_adminPower_makeadmincard->setChecked(true);
        }
        else
        {
            ui->checkBox_adminPower_makeadmincard->setChecked(false);
        }
        if(tmpPower & (0x01<<6))
        {
            ui->checkBox_adminPower_makeownercard->setChecked(true);
        }
        else
        {
            ui->checkBox_adminPower_makeownercard->setChecked(false);
        }
    }

    QSqlQuery tmpTopUpQuery = mSqlParser->Exec(QString("select topup from person_admin where name=\"%1\"").arg(tmpName));
    while(tmpTopUpQuery.next())
    {
        QSqlRecord tmpTopUpRecord = tmpTopUpQuery.record();
        int tmpTopUp = tmpTopUpRecord.value("topup").toInt();
        ui->comboBox_topup->setCurrentText(QString("%1").arg(tmpTopUp));
        QString tmpT = ui->comboBox_topup->currentText();
    }

}

void ICManager::slot_TableElevator_click(int pRow, int pColumn)
{
    QTableWidgetItem *tmpItem = ui->table_elevator->item(pRow,0);
    ui->line_elevnum->setText(tmpItem->text());

    QString tmp = ui->table_elevator->item(pRow,1)->text();
    //SetComboxText(ui->combo_unitnum, (tmpItem->text()));
    ui->combo_unitnum->setCurrentText(tmp);

    ui->line_ctrl_layernum->setText(ui->table_elevator->item(pRow,2)->text());
}

void ICManager::slot_TableUser_Click(int pRow, int pColumn)
{
    QTableWidgetItem *tmpItem = ui->table_user->item(pRow, 0);
    ui->line_user_name->setText(tmpItem->text());

    tmpItem = ui->table_user->item(pRow, 2);
    ui->line_user_phone->setText(tmpItem->text());

    tmpItem = ui->table_user->item(pRow, 1);
    QString tmpAddress = tmpItem->text();

    QString tmpDetail = ui->table_user->item(pRow, 5)->text();

    int tmpStartPos = 0;
    int index1 = tmpAddress.indexOf(GBKToUTF8("单元"), tmpStartPos);
    int index2 = 0;
    int index3 = 0;

    QString tmpUnitNum, tmpLayerNum, tmpFamilyNum;
    if(index1 >= 0)
    {
        tmpUnitNum = tmpAddress.mid(0, index1);
        SetComboxText(ui->combox_user_unit_num, (tmpUnitNum));
        tmpStartPos = index1 + QString(GBKToUTF8("单元")).count();
        index2 = tmpAddress.indexOf(GBKToUTF8("层"), tmpStartPos);
        if(index2 >= 0)
        {
            tmpLayerNum = tmpAddress.mid(tmpStartPos, index2 - tmpStartPos);
            SetComboxText(ui->combox_user_layer_num, (tmpLayerNum));
            tmpStartPos = index2 + QString(GBKToUTF8("层")).count();
            index3 = tmpAddress.indexOf(GBKToUTF8("室"), tmpStartPos);
            if(index3 >= 0)
            {
                tmpFamilyNum = tmpAddress.mid(tmpStartPos, index3 - tmpStartPos);
                SetComboxText(ui->combox_user_family_num, (tmpFamilyNum));
            }
        }
    }
    QStringList tmpSubDetails = tmpDetail.split(";");
    if(tmpSubDetails.count() == 2)
    {
        QString tmpValidLift = tmpSubDetails.at(0);
//        QString tmpValidLay = tmpSubDetails.at(1);
        QStringList tmpValidLifts = tmpValidLift.split(",");
        QStringList tmpValidFloors;
        mUserUnitElevt.clear();
        ui->combox_floor_num->clear();
        QList<QCheckBox*> tmpList = ui->wid_valideles->findChildren<QCheckBox*>();
        for(int i=0;i<tmpList.count();i++)
        {

            QCheckBox* tmpBox = tmpList.at(i);
            if(!tmpBox)
                continue;
            QString tmpKey = tmpBox->text().mid(QString(GBKToUTF8("电梯")).count());

            if(tmpValidLifts.contains(tmpKey))
            {
                if(!tmpValidFloors.contains(tmpKey))
                {
                    ui->combox_floor_num->addItem(tmpKey);
                    mUserUnitElevt.insert(tmpKey,tmpBox);
                }
                tmpBox->setChecked(true);
                tmpValidFloors.append(tmpKey);
            }
            else
                tmpBox->setChecked(false);
        }

//        mUserOpenFlrs.clear();

//        QStringList tmpValidLayers = tmpValidLay.split(",");  //1,3
//        tmpList = ui->wid_validlayers->findChildren<QCheckBox*>();
//        for(int i=0;i<tmpList.count();i++)
//        {
//            QCheckBox* tmpBox = tmpList.at(i);
//            if(!tmpBox)
//                continue;
//            QString tmpText = tmpBox->text();
//            int index = tmpText.indexOf(QString(GBKToUTF8("层")));
//            if(index)
//            {
//                QString tmpMap = tmpText.mid(0, index);   //3F或1
//                //获取此映射对应的实际楼层
//                QString tmpLayersNum =GetFlrNum(tmpMap);
//                if(tmpValidLayers.contains(tmpLayersNum))
//                {
//                    tmpBox->setChecked(true);
//                    mUserOpenFlrs.insert(tmpLayersNum,tmpBox);
//                }
//                else
//                    tmpBox->setChecked(false);
//            }
//        }
    }
    return;
}

void ICManager::slot_MainMenuClicked(int index)
{
    GotoPage(index);
    if(index == 10)
    {
        QList<QString> comboxList;
        QString tmpUnitNum;
        QSqlQuery tmpCellUintQuery = mSqlParser->Exec("select * from addr_units");
        while(tmpCellUintQuery.next())
        {
            QSqlRecord tmpCellUintRecord = tmpCellUintQuery.record();
            tmpUnitNum = tmpCellUintRecord.value("unit_num").toString();
            for(int i=0;i<ui->combo_unitnum->count();i++)
            {
                QString item = ui->combo_unitnum->itemText(i);
                comboxList.append(item);
            }
        }
        if(!comboxList.contains(tmpUnitNum))
        {
            ui->combo_unitnum->addItem(tmpUnitNum);
            ui->comboBox_toolcard_units->addItem(tmpUnitNum);
            ui->combox_user_unit_num->addItem(tmpUnitNum);
        }
    }
}

/*void ICManager::slot_UsrOperateClicked(int index)
{
    if(index < ui->stacked_useredit->count())
        ui->stacked_useredit->setCurrentIndex(index);
}

void ICManager::slot_AdminOperateClicked(int index)
{
    if(index < ui->stacked_admin->count())
        ui->stacked_admin->setCurrentIndex(index);
}
*/
void ICManager::slot_MaketoolcardClicked(int index)
{
    if(index < ui->stacked_maketoolcard->count())
        ui->stacked_maketoolcard->setCurrentIndex(index);
}

void ICManager::Init_cardlist_table()
{
   ui->table_cardlist->setSelectionBehavior(QAbstractItemView::SelectRows);
   ui->table_admin->horizontalHeader()->setStretchLastSection(true);
   ui->table_admin->horizontalHeader()->setVisible(true);
   ui->table_cardlist->setSelectionMode(QAbstractItemView::SingleSelection);
   ui->table_cardlist->setColumnWidth(0,160);
   ui->table_cardlist->setColumnWidth(1,100);
   ui->table_cardlist->setColumnWidth(2,100);
   ui->table_cardlist->setColumnWidth(3,100);
   ui->table_cardlist->setColumnWidth(4,100);
   ui->table_cardlist->setColumnWidth(5,100);

}

void ICManager::Init_admin_table()
{
    ui->table_admin->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选择行为，以行为单位
    ui->table_admin->horizontalHeader()->setStretchLastSection(true);
    ui->table_admin->horizontalHeader()->setVisible(true);
    ui->table_admin->setColumnWidth(0,150);
    ui->table_admin->setColumnWidth(1,160);
    ui->table_admin->setColumnWidth(2,150);
    ui->table_admin->setColumnWidth(3,60);
    ui->table_admin->setColumnWidth(4,200);
    ui->table_admin->setSelectionMode(QAbstractItemView::SingleSelection);
}

void ICManager::Init_user_table()
{
    ui->table_user->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    ui->table_user->horizontalHeader()->setStretchLastSection(true); //就是这个地方
    ui->table_user->horizontalHeader()->setVisible(true);

    ui->table_user->setColumnWidth(0, 120);
    ui->table_user->setColumnWidth(1, 160);
    ui->table_user->setColumnWidth(2, 120);
    ui->table_user->setColumnWidth(3, 60);
    ui->table_user->setColumnWidth(4, 160);
   // ui->table_user->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->table_user->setSelectionMode(QAbstractItemView::SingleSelection);
}

void ICManager::Init_map_table()
{
    ui->table_map->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    ui->table_map->horizontalHeader()->setStretchLastSection(true); //就是这个地方
    ui->table_map->horizontalHeader()->setVisible(true);

   // ui->table_user->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->table_map->setSelectionMode(QAbstractItemView::SingleSelection);

}

void ICManager::Init_cellInfo_table()
{
    ui->table_cellinfo->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_cellinfo->setColumnWidth(0,100);
    ui->table_cellinfo->setColumnWidth(1,100);
    ui->table_cellinfo->setColumnWidth(2,100);
    ui->table_cellinfo->setSelectionMode(QAbstractItemView::SingleSelection);
}

void ICManager::Init_cardlog_table()
{
    ui->table_cardlog->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_cardlog->setColumnWidth(0,100);
    ui->table_cardlog->setColumnWidth(1,160);
    ui->table_cardlog->setColumnWidth(2,100);
    ui->table_cardlog->setColumnWidth(3,210);
    ui->table_cardlog->setColumnWidth(4,210);
    ui->table_cardlog->setSelectionMode(QAbstractItemView::SingleSelection);
}

void ICManager::Init_elevator_table()
{
    ui->table_elevator->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_elevator->horizontalHeader()->setStretchLastSection(true); //就是这个地方
    ui->table_elevator->horizontalHeader()->setVisible(true);
    ui->table_elevator->setColumnWidth(0,100);
    ui->table_elevator->setColumnWidth(1,100);
    ui->table_elevator->setColumnWidth(2,100);
    ui->table_elevator->setSelectionMode(QAbstractItemView::SingleSelection);
}


void ICManager::AddElevator(QString pElevNum,QString pUnitNum,QString pCtrLayerCnt)
{
    int row = ui->table_elevator->rowCount();
    ui->table_elevator->insertRow(row);

    QTableWidgetItem *tmpItem0 = new QTableWidgetItem;
    tmpItem0->setText(pElevNum);
    tmpItem0->setFlags(tmpItem0->flags() & ~(Qt::ItemIsEditable));
    tmpItem0->setTextAlignment(Qt::AlignCenter);
    ui->table_elevator->setItem(row,0,tmpItem0);

    QTableWidgetItem *tmpItem1 = new QTableWidgetItem;
    tmpItem1->setText(pUnitNum);
    tmpItem1->setFlags(tmpItem1->flags() & ~(Qt::ItemIsEditable));
    tmpItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_elevator->setItem(row,1,tmpItem1);

    QTableWidgetItem *tmpItem2 = new QTableWidgetItem;
    tmpItem2->setText(pCtrLayerCnt);
    tmpItem2->setFlags(tmpItem2->flags() & ~(Qt::ItemIsEditable));
    tmpItem2->setTextAlignment(Qt::AlignCenter);
    ui->table_elevator->setItem(row,2,tmpItem2);

    QString tmpKey = QString("%1&%2").arg(pElevNum).arg(pUnitNum);
    mElevatorList.append(tmpKey);
}

void ICManager::AddCardLog(QString pType,QString pCardNum,QString pName,QDateTime pTime,QString pMode)
{
    int row = ui->table_cardlog->rowCount();
    ui->table_cardlog->insertRow(row);

    QTableWidgetItem *tmpItem0 = new QTableWidgetItem;
    tmpItem0->setText(pType);
    tmpItem0->setFlags(tmpItem0->flags() & ~(Qt::ItemIsEditable));
    tmpItem0->setTextAlignment(Qt::AlignCenter);
    ui->table_cardlog->setItem(row,0,tmpItem0);

    QTableWidgetItem *tmpItem1 = new QTableWidgetItem;
    tmpItem1->setText(pCardNum);
    tmpItem1->setFlags(tmpItem1->flags() & ~(Qt::ItemIsEditable));
    tmpItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_cardlog->setItem(row,1,tmpItem1);

    QTableWidgetItem *tmpItem2 = new QTableWidgetItem;
    tmpItem2->setText(pName);
    tmpItem2->setFlags(tmpItem2->flags() & ~(Qt::ItemIsEditable));
    tmpItem2->setTextAlignment(Qt::AlignCenter);
    ui->table_cardlog->setItem(row,2,tmpItem2);

    QTableWidgetItem *tmpItem3 = new QTableWidgetItem;
    tmpItem3->setText(pTime.toString("yyyy-MM-dd  hh:mm"));
    tmpItem3->setFlags(tmpItem3->flags() & ~(Qt::ItemIsEditable));
    tmpItem3->setTextAlignment(Qt::AlignCenter);
    ui->table_cardlog->setItem(row,3,tmpItem3);

    QTableWidgetItem *tmpItem4 = new QTableWidgetItem;
    tmpItem4->setText(pMode);
    tmpItem4->setFlags(tmpItem4->flags() & ~(Qt::ItemIsEditable));
    tmpItem4->setTextAlignment(Qt::AlignCenter);
    ui->table_cardlog->setItem(row,4,tmpItem4);

}

void ICManager::AddCellInfo(QString pUnitsNum,QString pLayersNum,QString pFamilysNum)
{
    int row = ui->table_cellinfo->rowCount();
    ui->table_cellinfo->insertRow(row);

    QTableWidgetItem *tmpItem0 = new QTableWidgetItem;
    tmpItem0->setText(pUnitsNum);
    tmpItem0->setIcon(QIcon(":/icon/icon/6.png"));
    tmpItem0->setFlags(tmpItem0->flags() & ~(Qt::ItemIsEditable));
    tmpItem0->setTextAlignment(Qt::AlignCenter);
    ui->table_cellinfo->setItem(row,0,tmpItem0);

    QTableWidgetItem *tmpItem1 = new QTableWidgetItem;
    tmpItem1->setText(pLayersNum);
    tmpItem1->setFlags(tmpItem1->flags() & ~(Qt::ItemIsEditable));
    tmpItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_cellinfo->setItem(row,1,tmpItem1);

    QTableWidgetItem *tmpItem2 = new QTableWidgetItem;
    tmpItem2->setText(pFamilysNum);
    tmpItem2->setFlags(tmpItem2->flags() & ~(Qt::ItemIsEditable));
    tmpItem2->setTextAlignment(Qt::AlignCenter);
    ui->table_cellinfo->setItem(row,2,tmpItem2);
}

void ICManager::AddCardlist(QString pCardNum,QString pType,QString pName,QString pPermission,QString pState,QString pMaker)
{
    mCardList.clear();
    int row = ui->table_cardlist->rowCount();
    ui->table_cardlist->insertRow(row);    //在table_admin中添加一行

    QTableWidgetItem *tmpItem0 = new QTableWidgetItem;
    tmpItem0->setText(pCardNum);
    tmpItem0->setFlags(tmpItem0->flags() & ~(Qt::ItemIsEditable));//设置第0个item不可编辑
    tmpItem0->setIcon(QIcon(":/icon/icon/1.png"));
    ui->table_cardlist->setItem(row,0,tmpItem0);   //在table_admin的这一行中添加第0列的QTableWidgetItem

    QTableWidgetItem *tmpItem1 = new QTableWidgetItem;
    tmpItem1->setText(pType);
    tmpItem1->setFlags(tmpItem1->flags() & ~(Qt::ItemIsEditable));
    tmpItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_cardlist->setItem(row,1,tmpItem1);

    QTableWidgetItem *tmpItem2 = new QTableWidgetItem;
    tmpItem2->setText(pName);
    tmpItem2->setFlags(tmpItem2->flags() & ~(Qt::ItemIsEditable));
    tmpItem2->setTextAlignment(Qt::AlignCenter);
    ui->table_cardlist->setItem(row,2,tmpItem2);

    QTableWidgetItem *tmpItem3 = new QTableWidgetItem;
    tmpItem3->setText(pPermission);
    tmpItem3->setFlags(tmpItem3->flags() & ~(Qt::ItemIsEditable));
    tmpItem3->setTextAlignment(Qt::AlignCenter);
    ui->table_cardlist->setItem(row,3,tmpItem3);

    QTableWidgetItem *tmpItem4 = new QTableWidgetItem;
    tmpItem4->setText(pState);
    tmpItem4->setFlags(tmpItem4->flags() & ~(Qt::ItemIsEditable));
    tmpItem4->setTextAlignment(Qt::AlignCenter);
    ui->table_cardlist->setItem(row,4,tmpItem4);

    QTableWidgetItem *tmpItem5 = new QTableWidgetItem;
    tmpItem5->setText(pMaker);
    tmpItem5->setFlags(tmpItem5->flags() & ~(Qt::ItemIsEditable));
    tmpItem5->setTextAlignment(Qt::AlignCenter);
    ui->table_cardlist->setItem(row,5,tmpItem5);

    CardItem *tmpItem6 = new CardItem;
    tmpItem6->SetCardState(pState);
    ui->table_cardlist->setCellWidget(row,6,tmpItem6);
    connect(tmpItem6,SIGNAL(sCardLog()),this,SLOT(slot_CardLog()));
    connect(tmpItem6,SIGNAL(sCardRepair()),this,SLOT(slot_CardRepair()));
    connect(tmpItem6,SIGNAL(sCardBackup()),this,SLOT(slot_CardLoss()));
    connect(tmpItem6,SIGNAL(sCardLogOut()),this,SLOT(slot_CardLogRemove()));

    mCardList.append(pCardNum);
}

void ICManager::AddAdmin(QString pName,QString pAddress,QString pPhone,int pCardCount,QString pInfo)
{
    mAdminList.clear();
    int row = ui->table_admin->rowCount();
    ui->table_admin->insertRow(row);    //在table_admin中添加一行

    QTableWidgetItem *tmpItem0 = new QTableWidgetItem;
    tmpItem0->setText(pName);
    tmpItem0->setFlags(tmpItem0->flags() & ~(Qt::ItemIsEditable));//设置第0个item不可编辑
    tmpItem0->setIcon(QIcon(":/icon/icon/1.png"));
    ui->table_admin->setItem(row,0,tmpItem0);   //在table_admin的这一行中添加第0列的QTableWidgetItem

    QTableWidgetItem *tmpItem1 = new QTableWidgetItem;
    tmpItem1->setText(pAddress);
    tmpItem1->setFlags(tmpItem1->flags() & ~(Qt::ItemIsEditable));
    ui->table_admin->setItem(row,1,tmpItem1);

    QTableWidgetItem *tmpItem2 = new QTableWidgetItem;
    tmpItem2->setText(pPhone);
    tmpItem2->setFlags(tmpItem2->flags() & ~(Qt::ItemIsEditable));
    tmpItem2->setTextAlignment(Qt::AlignCenter);
    ui->table_admin->setItem(row,2,tmpItem2);

    QTableWidgetItem *tmpItem3 = new QTableWidgetItem;
    tmpItem3->setText(QString("%1").arg(pCardCount));
    tmpItem3->setFlags(tmpItem3->flags() & ~(Qt::ItemIsEditable));
    tmpItem3->setTextAlignment(Qt::AlignCenter);
    ui->table_admin->setItem(row,3,tmpItem3);

    AdminItem *tmpItem4 = new AdminItem;
    ui->table_admin->setCellWidget(row,4,tmpItem4);
    connect(tmpItem4, SIGNAL(sMadeCardsNum()),this, SLOT(slot_MadeCardsNum()));
    connect(tmpItem4, SIGNAL(sMakeAdminCard()), this, SLOT(slot_MakeAdminCard()));
    connect(tmpItem4, SIGNAL(sDestoryCard()), this, SLOT(slot_DestoryAdminCard()));
    connect(tmpItem4, SIGNAL(setPassword()), this, SLOT(slot_SetAdminPassword()));

    QTableWidgetItem *tmpItem5 = new QTableWidgetItem;
    tmpItem5->setText(pInfo);
    tmpItem5->setFlags(tmpItem5->flags() & ~(Qt::ItemIsEditable));
    ui->table_admin->setItem(row,5,tmpItem5);

    mAdminList.append(pName);
}

void ICManager::AddUserToDB(QString pName, QString pUnitNum, QString pLayerNum, QString pFamilyNum, QString pPhone, QString pDetail)
{
    int index = mUserList.indexOf(pName);  //在原名字列表中查找此名字的位置(第几行)，看此名是否被编辑，被编辑过的都存入数据库
    if(index >= 0)  //找到，原用户没有被编辑改变，询问是否要编辑此业主
    {
        QMessageBox::StandardButton tmpret = QMessageBox::question(0, GBKToUTF8("更新询问"),
                                                                      GBKToUTF8("是否更改业主信息"),
                                                                   QMessageBox::Ok | QMessageBox::Cancel);
        if(tmpret != QMessageBox::Ok)
        {
            return;
        }
    }

    QList<QVariant> tmpFieldVaules;
    tmpFieldVaules << pName << pPhone << pUnitNum << pLayerNum << pFamilyNum << pDetail;
    if(!mSqlParser->ModifyRow("person_user", "name", pName, tmpFieldVaules))
    {
        QMessageBox::critical(0, GBKToUTF8("添加业主错误"),
                              GBKToUTF8("无法添加此业主，请检查填写是否正确"));
        return;
    }

    QString tmpAddress = QString(GBKToUTF8("%1单元%2层%3室")).arg(pUnitNum).arg(pLayerNum).arg(pFamilyNum);
    if(index >= 0)   //名字没改变，不需要添加新卡，只要修改老记录，把这条记录相应的其它字段（编辑或没编辑）改变
    {
        QTableWidgetItem *tmpItem = ui->table_user->item(index, 1);
        tmpItem->setText(tmpAddress);

        tmpItem = ui->table_user->item(index, 2);
        tmpItem->setText(pPhone);

        tmpItem = ui->table_user->item(index, 5);
        tmpItem->setText(pDetail);
    }
    else   //没有找到，名字改变就需要重新添加新卡，加入到table列表中
    {
        AddUser(pName, tmpAddress, pPhone, 0, pDetail);
    }
}

void ICManager::DelUserFromDB(QString pName)
{
    int index = mUserList.indexOf(pName);  //在原名字列表中查找此名字的位置(第几行)，看此名是否被编辑，被编辑过的都存入数据库
    if(index >= 0)  //找到，原用户没有被编辑改变，询问是否要编辑此业主
    {
        QMessageBox::StandardButton tmpret = QMessageBox::question(0, GBKToUTF8("删除询问"),
                                                                      GBKToUTF8("是否删除业主信息"),
                                                                   QMessageBox::Ok | QMessageBox::Cancel);
        if(tmpret != QMessageBox::Ok)
        {
            return;
        }
    }
    else
    {
        return;
    }

    if(!mSqlParser->DelRow("person_user", "name", pName))
    {
        QMessageBox::critical(0, GBKToUTF8("删除业主错误"),
                              GBKToUTF8("无法删除此业主，请检查填写是否正确"));
        return;
    }
    ui->table_user->removeRow(index);
}

void ICManager::DelCardFromDB(QString CardNum,int index)
{
    ui->table_cardlist->removeRow(index);
    if(!mSqlParser->DelRow("card_list","card_num",CardNum))
    {
        QMessageBox::critical(0, GBKToUTF8("删除业主错误"),
                              GBKToUTF8("无法删除此业主，请检查填写是否正确"));
        return;
    }
}

void ICManager::DelAdminFromDB(QString pName)
{
    QList <QTableWidgetItem *> item = ui->table_admin->findItems(pName,Qt::MatchContains);
    if(!item.isEmpty())
    {
        for(int i=0;i<item.count();i++)
        {
            index = item.at(i)->row();
        }
    }
//    int index = mAdminList.indexOf(pName);
    if(index >= 0)
    {
        QMessageBox::StandardButton tmpret = QMessageBox::question(0,GBKToUTF8("删除询问"),
                                                                   GBKToUTF8("是否删除管理员信息"),
                                                                   QMessageBox::Ok|QMessageBox::Cancel);
        if(tmpret != QMessageBox::Ok)
        {
            return;
        }
    }
    else
    {
        return;
    }
    ui->table_admin->removeRow(index);
    if(!mSqlParser->DelRow("person_admin","name", pName))
    {
        QMessageBox::critical(0, GBKToUTF8("删除管理员错误"),
                              GBKToUTF8("无法删除此管理员，请检查填写是否正确"));
        return;
    }

}

void ICManager::AddAdminToDB(QString pName, QString pPhone, QString pAddress, int pPower, QString pTopUp, QString pPassword)
{
    QList <QTableWidgetItem *> item = ui->table_admin->findItems(pName,Qt::MatchContains);
    if(!item.isEmpty())
    {
        for(int i=0;i<item.count();i++)
        {
            index = item.at(i)->row();
        }
    }
    else
    {
        index = -1;
    }
//    int index = mAdminList.indexOf(pName);
    if(index >= 0)
    {
        QMessageBox::StandardButton tmpret = QMessageBox::question(0,GBKToUTF8("更新询问"),
                                                                   GBKToUTF8("是否更改管理员信息"),
                                                                   QMessageBox::Ok|QMessageBox::Cancel);
        if(tmpret != QMessageBox::Ok)
        {
            return;
        }
    }

    QList<QVariant> tmpFieldVaules;
    if(index >=0 )
    {
        tmpFieldVaules << pName << pPhone << pAddress << QString("%1").arg(pPower) << pTopUp << "";
    }
    else
    {
        tmpFieldVaules << pName << pPhone << pAddress << QString("%1").arg(pPower) << pTopUp << "" << pPassword;
    }
    if(resetPassword)
    {
        tmpFieldVaules.clear();
        tmpFieldVaules << pName << pPhone << pAddress << QString("%1").arg(pPower) << pTopUp << "" << pPassword;
    }

    if(!mSqlParser->ModifyRow("person_admin","name",pName,tmpFieldVaules))
    {
        QMessageBox::critical(0,GBKToUTF8("添加管理员错误"),
                              GBKToUTF8("无法添加此管理员，请检查填写是否正确"));
        return;
    }

    if(index >= 0)
    {
        QTableWidgetItem *tmpItem = ui->table_admin->item(index, 1);
        tmpItem->setText(pAddress);

        tmpItem = ui->table_admin->item(index, 2);
        tmpItem->setText(pPhone);      
    }
    else
    {
        AddAdmin(pName, pAddress, pPhone, 0, QString());
    }

}

void ICManager::AddMakeAdminCardToDB(QString pName,QString pCardNum,QString pPassword)
{
    int index = mCardList.indexOf(pCardNum);
    if(1)
    {
        QMessageBox::StandardButton tmpret = QMessageBox::question(0,GBKToUTF8("更新询问"),
                                                                   GBKToUTF8("是否更改卡信息"),
                                                                   QMessageBox::Ok|QMessageBox::Cancel);
        if(tmpret != QMessageBox::Ok)
        {
            return;
        }
    }

    QList<QVariant> tmpFieldVaules;
    tmpFieldVaules.clear();
    tmpFieldVaules << pCardNum << 2 << pName << "" <<GBKToUTF8("正常") << "" << pPassword;  //界面参数在更改情况下，此条才会加到数据库中，内容改为此条信息（界面中的参数）
    if(!mSqlParser->ModifyRow("card_list","card_num",pCardNum,tmpFieldVaules))
    {
        QMessageBox::critical(0,GBKToUTF8("添加卡信息错误"),
                              GBKToUTF8("无法添加此卡，请检查填写是否正确"));
        return;
    }

    ShowMsgPrg(QString(), 10);

    if(index >= 0)  //更改旧卡参数到table列表
    {
        QTableWidgetItem *tmpItem = ui->table_cardlist->item(index, 2);
        if(tmpItem)
            tmpItem->setText(pName);
    }
    else   //添加新卡到界面table列表
    {
         QString tmpAdminCard = QString(GBKToUTF8("管理员卡"));
         QString tmpCardAct = QString(GBKToUTF8("正常"));
         AddCardlist(pCardNum,tmpAdminCard,pName,QString(),tmpCardAct,QString());
    }

    ShowMsgPrg(QString(), 50);

    //写入卡
    AdminCard tmpData;
    memset(&tmpData,0,sizeof(AdminCard));

    //密码为123456
    CardPasswd *tmpPw = (CardPasswd *) (&tmpData);
    tmpPw->mPwList.mPw1 = 1;
    tmpPw->mPwList.mPw2 = 2;
    tmpPw->mPwList.mPw3 = 3;
    tmpPw->mPwList.mPw4 = 4;
    tmpPw->mPwList.mPw5 = 5;
    tmpPw->mPwList.mPw6 = 6;

    tmpData.mState = 0;

    tmpData.mType = 2;

    ShowMsgPrg(QString(), 60);

    if(ui->checkBox_admin_cardValidtime->isChecked())
    {
        tmpData.mValidDateEnable = 1;
        DateTime *tmpDateTime = (DateTime *)tmpData.mValidStartDate;
        QDate tmpNewDate = ui->date_admincard_start->date();
        tmpDateTime->mDateList.mYear = tmpNewDate.year()-2000;
        tmpDateTime->mDateList.mMonth = tmpNewDate.month();
        tmpDateTime->mDateList.mDay = tmpNewDate.day();

        tmpDateTime = (DateTime *)tmpData.mValidEndDate;
        tmpNewDate = ui->date_admincard_end->date();
        tmpDateTime->mDateList.mYear = tmpNewDate.year()-2000;
        tmpDateTime->mDateList.mMonth = tmpNewDate.month();
        tmpDateTime->mDateList.mDay = tmpNewDate.day();
    }
    else
    {
        tmpData.mValidDateEnable = 0;
    }

    if(ui->checkBox_admin_availabletime->isChecked())
    {
        tmpData.mAvailableTime = 1;
        DateTime *tmpDateTime = (DateTime *)tmpData.mValidStartTime;
        QTime tmpNewTime = ui->time_admincard_start->time();
        tmpDateTime->mTimeList.mHour = tmpNewTime.hour();
        tmpDateTime->mTimeList.mMinute = tmpNewTime.minute();
        tmpDateTime->mTimeList.mSecond = tmpNewTime.second();

        tmpDateTime = (DateTime *)tmpData.mValidEndTime;
        tmpNewTime = ui->time_admincard_end->time();
        tmpDateTime->mTimeList.mHour = tmpNewTime.hour();
        tmpDateTime->mTimeList.mMinute = tmpNewTime.minute();
        tmpDateTime->mTimeList.mSecond = tmpNewTime.second();

        CardByte *tmpWeek = (CardByte*)(&(tmpData.mValidWeek));
        tmpWeek->mBits.mBit0 = (int)(ui->check_admincard_week_1->isChecked());
        tmpWeek->mBits.mBit1 = (int)(ui->check_admincard_week_2->isChecked());
        tmpWeek->mBits.mBit2 = (int)(ui->check_admincard_week_3->isChecked());
        tmpWeek->mBits.mBit3 = (int)(ui->check_admincard_week_4->isChecked());
        tmpWeek->mBits.mBit4 = (int)(ui->check_admincard_week_5->isChecked());
        tmpWeek->mBits.mBit5 = (int)(ui->check_admincard_week_6->isChecked());
        tmpWeek->mBits.mBit6 = (int)(ui->check_admincard_week_7->isChecked());
    }
    else
    {
        tmpData.mAvailableTime = 0;
    }

    QByteArray tmpBuffer1((char*)(&tmpData), 48);
    mIcReader.WriteCard(1, tmpBuffer1);

    ShowMsgPrg(QString(), 100);
}

void ICManager::on_btn_saveFloor_clicked()
{
    int index = ui->combox_floor_num->currentIndex();
    AddValidFloorsToDB(index);
}
void ICManager::AddValidFloorsToDB(int index)
{
    QByteArray tmpCorrespdFlrs;
    if(index == 0)
    {
        tmpCorrespdFlrs = GetUserOpenFlrs();
        memcpy(usrOpenFloors.usrOpenFloorOne, tmpCorrespdFlrs.constData(), 12);
    }
    else if(index == 1)
    {
        tmpCorrespdFlrs = GetUserOpenFlrs();
        memcpy(usrOpenFloors.usrOpenFloorTow, tmpCorrespdFlrs.constData(), 12);
    }
    else if(index == 2)
    {
        tmpCorrespdFlrs = GetUserOpenFlrs();
        memcpy(usrOpenFloors.usrOpenFloorThree, tmpCorrespdFlrs.constData(), 12);
    }

    else if(index == 3)
    {
        tmpCorrespdFlrs = GetUserOpenFlrs();
        memcpy(usrOpenFloors.usrOpenFloorFour, tmpCorrespdFlrs.constData(), 12);
    }
    else if(index == 4)
    {
        tmpCorrespdFlrs = GetUserOpenFlrs();
        memcpy(usrOpenFloors.usrOpenFloorFives, tmpCorrespdFlrs.constData(), 12);
    }
    else if(index == 5)
    {
        tmpCorrespdFlrs = GetUserOpenFlrs();
        memcpy(usrOpenFloors.usrOpenFloorSix, tmpCorrespdFlrs.constData(), 12);
    }
    else if(index == 6)
    {
        tmpCorrespdFlrs = GetUserOpenFlrs();
        memcpy(usrOpenFloors.usrOpenFloorSeven, tmpCorrespdFlrs.constData(), 12);
    }
    else if(index == 7)
    {
        tmpCorrespdFlrs = GetUserOpenFlrs();
        memcpy(usrOpenFloors.usrOpenFloorEight, tmpCorrespdFlrs.constData(), 12);
    }

}

void ICManager::AddMakeCardToDB(QString pName,QString pCardNum,QString pPassword)
{
    int index = mCardList.indexOf(pCardNum);
    if(1)
    {
        QMessageBox::StandardButton tmpret = QMessageBox::question(0,GBKToUTF8("更新询问"),
                                                                   GBKToUTF8("是否更改卡信息"),
                                                                   QMessageBox::Ok|QMessageBox::Cancel);
        if(tmpret != QMessageBox::Ok)
        {
            return;
        }
    }

    QList<QVariant> tmpFieldVaules;
    tmpFieldVaules.clear();
    tmpFieldVaules << pCardNum << 1 << pName << "" <<GBKToUTF8("正常") << "" << pPassword;  //界面参数在更改情况下，此条才会加到数据库中，内容改为此条信息（界面中的参数）
    if(!mSqlParser->ModifyRow("card_list","card_num",pCardNum,tmpFieldVaules))
    {
        QMessageBox::critical(0,GBKToUTF8("添加卡信息错误"),
                              GBKToUTF8("无法添加此卡，请检查填写是否正确"));
        return;
    }

    ShowMsgPrg(QString(), 10);

    if(index >= 0)  //更改旧卡参数到table列表
    {
        QTableWidgetItem *tmpItem = ui->table_cardlist->item(index, 2);
        tmpItem->setText(pName);
    }
    else   //添加新卡到界面table列表
    {
         QString tmpUserCard = QString(GBKToUTF8("用户卡"));
         QString tmpCardAct = QString(GBKToUTF8("正常"));
         AddCardlist(pCardNum,tmpUserCard,pName,QString(),tmpCardAct,QString());
    }

    ShowMsgPrg(QString(), 50);

    //写入卡
    UsrCard tmpData;
    memset(&tmpData,0,sizeof(UsrCard));

    //密码为123456
    CardPasswd *tmpPw = (CardPasswd *) (&tmpData);
    tmpPw->mPwList.mPw1 = 1;
    tmpPw->mPwList.mPw2 = 2;
    tmpPw->mPwList.mPw3 = 3;
    tmpPw->mPwList.mPw4 = 4;
    tmpPw->mPwList.mPw5 = 5;
    tmpPw->mPwList.mPw6 = 6;

    tmpData.mState = 0;

//        tmpData.mValidEles[0]= ui->combo_validelevt1->currentText().toInt();
//        tmpData.mValidEles[1]= ui->combo_validelevt2->currentText().toInt();
//        tmpData.mValidEles[2]= ui->combo_validelevt3->currentText().toInt();
//        tmpData.mValidEles[3]= ui->combo_validelevt4->currentText().toInt();
//        tmpData.mValidEles[4]= ui->combo_validelevt5->currentText().toInt();
//        tmpData.mValidEles[5]= ui->combo_validelevt6->currentText().toInt();
//        tmpData.mValidEles[6]= ui->combo_validelevt7->currentText().toInt();
//        tmpData.mValidEles[7]= ui->combo_validelevt8->currentText().toInt();

    tmpData.mType = 1;

    ShowMsgPrg(QString(), 60);

    CardFreeFee *tmpFreeFee = (CardFreeFee *)(tmpData.mMoney);
    tmpFreeFee->mShortFee = ui->lineEdit_freefee->text().toInt();

    tmpData.mPrice = ui->lineEdit_chargestandard->text().toInt();
    tmpData.mChargeType = ui->combo_cardfee_type->currentIndex();

    QList<int> tmpFloorList;
    QList<QString> tmpUserEleList = mUserUnitElevt.keys();
    int ElevIndex = 0;
    for(int i=0; i<tmpUserEleList.count();i++)
    {
        QString tmpElevtKey = tmpUserEleList.at(i);
        QCheckBox* tmpCheck = mUserUnitElevt.value(tmpElevtKey);
        if(tmpCheck->isChecked())
        {
            int tmpNum = tmpElevtKey.toInt();
            tmpFloorList.append(tmpNum);
        }
    }

    qSort(tmpFloorList.begin(),tmpFloorList.end());
    for(int i=0;i<tmpFloorList.count();i++)
    {
       tmpData.mValidEles[ElevIndex++] = tmpFloorList.at(i);
    }


    DateTime *tmpDateTime = (DateTime *)tmpData.mValidStartDate;
    QDate tmpNewDate = ui->date_card_start->date();
    tmpDateTime->mDateList.mYear = tmpNewDate.year()-2000;
    tmpDateTime->mDateList.mMonth = tmpNewDate.month();
    tmpDateTime->mDateList.mDay = tmpNewDate.day();

    tmpDateTime = (DateTime *)tmpData.mValidEndDate;
    tmpNewDate = ui->date_card_end->date();
    tmpDateTime->mDateList.mYear = tmpNewDate.year()-2000;
    tmpDateTime->mDateList.mMonth = tmpNewDate.month();
    tmpDateTime->mDateList.mDay = tmpNewDate.day();

    tmpData.mCallEleType = ui->comboBox_calltype->currentIndex();

    tmpData.mResponseTime = ui->comboBox_response_time->currentText().toInt();

    if(ui->checkBox_available_time->isChecked())
    {
        tmpData.mAvailableTime = 1;
        tmpDateTime = (DateTime *)tmpData.mValidStartTime;
        QTime tmpNewTime = ui->time_card_start->time();
        tmpDateTime->mTimeList.mHour = tmpNewTime.hour();
        tmpDateTime->mTimeList.mMinute = tmpNewTime.minute();
        tmpDateTime->mTimeList.mSecond = tmpNewTime.second();

        tmpDateTime = (DateTime *)tmpData.mValidEndTime;
        tmpNewTime = ui->time_card_end->time();
        tmpDateTime->mTimeList.mHour = tmpNewTime.hour();
        tmpDateTime->mTimeList.mMinute = tmpNewTime.minute();
        tmpDateTime->mTimeList.mSecond = tmpNewTime.second();

        CardByte *tmpWeek = (CardByte*)(&(tmpData.mValidWeek));
        tmpWeek->mBits.mBit0 = (int)(ui->check_card_week_1->isChecked());
        tmpWeek->mBits.mBit1 = (int)(ui->check_card_week_2->isChecked());
        tmpWeek->mBits.mBit2 = (int)(ui->check_card_week_3->isChecked());
        tmpWeek->mBits.mBit3 = (int)(ui->check_card_week_4->isChecked());
        tmpWeek->mBits.mBit4 = (int)(ui->check_card_week_5->isChecked());
        tmpWeek->mBits.mBit5 = (int)(ui->check_card_week_6->isChecked());
        tmpWeek->mBits.mBit6 = (int)(ui->check_card_week_7->isChecked());
    }
    else
    {
        tmpData.mAvailableTime = 0;
    }

    QByteArray tmpBuffer1((char*)(&tmpData), 48);
    mIcReader.WriteCard(1, tmpBuffer1);

    ShowMsgPrg(QString(), 80);

    char tmpFlrBuffer[48] = {0};

//    QByteArray tmpCorrespdFlrs = GetCorrespdFlrs(0);
//    memcpy(tmpFlrBuffer, tmpCorrespdFlrs.constData(), 12);

    memcpy(tmpFlrBuffer, usrOpenFloors.usrOpenFloorOne, 12);
    memcpy(tmpFlrBuffer+16, usrOpenFloors.usrOpenFloorTow, 12);
    memcpy(tmpFlrBuffer+32, usrOpenFloors.usrOpenFloorThree, 12);

    QByteArray tmpBuffer2(tmpFlrBuffer, 48);
    mIcReader.WriteCard(2, tmpBuffer2);

    tmpFlrBuffer[48] = {0};
    memcpy(tmpFlrBuffer, usrOpenFloors.usrOpenFloorFour, 12);
    memcpy(tmpFlrBuffer+16, usrOpenFloors.usrOpenFloorFives, 12);
    memcpy(tmpFlrBuffer+32, usrOpenFloors.usrOpenFloorSix, 12);

    QByteArray tmpBuffer3(tmpFlrBuffer, 48);
    mIcReader.WriteCard(3,tmpBuffer3);

    tmpFlrBuffer[48] = {0};
    memcpy(tmpFlrBuffer, usrOpenFloors.usrOpenFloorSeven, 12);
    memcpy(tmpFlrBuffer+16, usrOpenFloors.usrOpenFloorEight, 12);

    QByteArray tmpBuffer4(tmpFlrBuffer, 32);
    mIcReader.WriteCard(4,tmpBuffer4);
    memset(&usrOpenFloors,0,sizeof(UsrCard_OpenFloor));

    ShowMsgPrg(QString(), 100);
}

void ICManager::AddToolCardToDB(QString pName,QString pCardNum,QString pPassword)
{
//    int index = mCardList.indexOf(pCardNum);
    if(1)
    {
       QMessageBox::StandardButton tmpBtn = QMessageBox::question(0,GBKToUTF8("更新询问"),
                                                                  GBKToUTF8("确认更改工具卡的信息"),
                                                                  QMessageBox::Ok|QMessageBox::Cancel);
       if(tmpBtn!=QMessageBox::Ok)
       {
           return;
       }

    }

//    QList<QVariant> tmpFieldVaules;
//    tmpFieldVaules << pCardNum << 102 << pName << "" << GBKToUTF8("正常") << ""<<pPassword;
//    if(!mSqlParser->ModifyRow("card_list","card_num",pCardNum,tmpFieldVaules))
//    {
//        QMessageBox::critical(0,GBKToUTF8("添加时间同步卡错误"),
//                               GBKToUTF8("无法添加时间同步卡，请检查填写是否正确"));
//        return;
//    }

//    ShowMsgPrg(QString(), 10);

//    if(index>=0)
//    {
//        QTableWidgetItem *tmpItem = ui->table_cardlist->item(index,2);
//        tmpItem->setText(pName);
//    }
//    else
//    {
//        AddCardlist(pCardNum,"102",pName,"",GBKToUTF8("正常"),"");
//    }

//    ShowMsgPrg(QString(), 50);

    unsigned char tmpBuffer[48] = {0};

    //密码为123456
    CardPasswd *tmpPw = (CardPasswd *) (tmpBuffer);
    tmpPw->mPwList.mPw1 = 1;
    tmpPw->mPwList.mPw2 = 2;
    tmpPw->mPwList.mPw3 = 3;
    tmpPw->mPwList.mPw4 = 4;
    tmpPw->mPwList.mPw5 = 5;
    tmpPw->mPwList.mPw6 = 6;

    ToolCard_Header *tmpHeader = (ToolCard_Header *)tmpBuffer;

    tmpHeader->mUnit = ui->comboBox_toolcard_units->currentText().toInt();//单元号

    QList<QString> tmpEleList = mUnitElevt.keys();//选择单元号所在的所有电梯编号
    int tmpIndex = 0;
    for(int i=0; i< tmpEleList.count();i++)
    {
        QString tmpElevtKey = tmpEleList.at(i);
        QCheckBox* tmpCheck = mUnitElevt.value(tmpElevtKey);//每一个电梯编号对应的check是否打钩
        if(tmpCheck->isChecked())
        {
            tmpHeader->mElevator[tmpIndex++] = tmpElevtKey.toInt();
        }
    }

//    ShowMsgPrg(QString(), 60);

    if(ui->stacked_maketoolcard->currentWidget() == ui->page_maketoolcard_syntime)
    {
        ToolCard_DateTimeSyn *tmpDTSyn = (ToolCard_DateTimeSyn *)tmpBuffer;
        tmpDTSyn->mToolHeader.mState = 0;
        tmpDTSyn->mToolHeader.mType = 101;

        //int tmpunit = ui->comboBox_toolcard_units->currentText().toInt();
        DateTime* tmpDataTime = (DateTime*)tmpDTSyn->mDateTimeSyn;
        QDate tmpD = ui->dateEdit_syntime->date();
        tmpDataTime->mDateList.mYear = tmpD.year()-2000;
        tmpDataTime->mDateList.mMonth = tmpD.month();
        tmpDataTime->mDateList.mDay = tmpD.day();

        tmpDataTime = (DateTime*)(tmpDTSyn->mDateTimeSyn+3);
        QTime tmpT = ui->timeEdit_syntime->time();
        tmpDataTime->mTimeList.mHour = tmpT.hour();
        tmpDataTime->mTimeList.mMinute = tmpT.minute();
        tmpDataTime->mTimeList.mSecond = tmpT.second();
    }
    else if(ui->stacked_maketoolcard->currentWidget() == ui->page_maketoolcard_readcard)
    {
        ToolCard_CardRecord *tmpCardRecord = (ToolCard_CardRecord *)tmpBuffer;
        tmpCardRecord->mToolHeader.mState = 0;
        tmpCardRecord->mToolHeader.mType = 102;

        DateTime* tmpDateTime = (DateTime*)tmpCardRecord->mDateTimeStart;
        QDate tmpD = ui->dateEdit_readcard_start->date();
        tmpDateTime->mDateList.mYear = tmpD.year()-2000;
        tmpDateTime->mDateList.mMonth = tmpD.month();
        tmpDateTime->mDateList.mDay = tmpD.day();

        tmpDateTime = (DateTime*)(tmpCardRecord->mDateTimeStart+3);
        QTime tmpT = ui->timeEdit_readcard_start->time();
        tmpDateTime->mTimeList.mHour =tmpT.hour();
        tmpDateTime->mTimeList.mMinute = tmpT.minute();

        tmpDateTime = (DateTime*)tmpCardRecord->mDateTimeEnd;
        tmpD = ui->dateEdit_readcard_end->date();
        tmpDateTime->mDateList.mYear = tmpD.year()-2000;
        tmpDateTime->mDateList.mMonth = tmpD.month();
        tmpDateTime->mDateList.mDay = tmpD.day();

        tmpDateTime = (DateTime*)(tmpCardRecord->mDateTimeEnd+3);
        tmpT = ui->timeEdit_readcard_end->time();
        tmpDateTime->mTimeList.mHour =tmpT.hour();
        tmpDateTime->mTimeList.mMinute = tmpT.minute();

        tmpCardRecord->mFloor = ui->combo_readcard_selectfloor->currentText().toInt();

        QString tmpName = ui->line_readcard_selectowner->text();
        QByteArray tmpNameArray = tmpName.toLocal8Bit();
        memcpy(tmpCardRecord->mName,tmpNameArray.constData(),8);

    }
    else if(ui->stacked_maketoolcard->currentWidget() == ui->page_maketoolcard_maskroom)
    {
        ToolCard_MaskRoom* tmpMaskRoom = (ToolCard_MaskRoom*)(tmpBuffer);
        tmpMaskRoom->mToolHeader.mState = 0;
        tmpMaskRoom->mToolHeader.mType = 103;

        int tmpCount = 0;
        for(int i=0;i<ui->tabWidget_rooms->count();i++)  //Widget总页数
        {
            QString tmpText = ui->tabWidget_rooms->tabText(i); //取出每个tab上的文本
            QWidget *tmpWidget = ui->tabWidget_rooms->widget(i); //取出每个tab控件的widget
            QList<QCheckBox*> tmpChdList = tmpWidget->findChildren<QCheckBox*>();//取出每个widget上所有checkbox
            QString tmpLayer = tmpText.mid(0, tmpText.count()-1); //从第0个开始，取tmpText.count()-1长度的数据:几（层）
            QString mtmpLayerNum = GetFlrNum(tmpLayer);
            int tmpLayerNum = mtmpLayerNum.toInt(); //转换为int类型
            for(int j=0;j<tmpChdList.count();j++)//所有的checkbox
            {
                QCheckBox* tmpBtn = tmpChdList.at(j); //所有的checkbox一个一个取出来
                if(tmpBtn->isChecked()) //查看每个checkbox是否被打勾
                {
                    QString tmpRoomText = tmpBtn->text();//获取checkbox上的文本
                    QString tmpRoom = tmpRoomText.mid(0, tmpRoomText.count()-1);//取文本几（室）
                    int tmpRoomNum = tmpRoom.toInt();
                    tmpMaskRoom->mRoom[tmpCount][0] = tmpLayerNum;//
                    tmpMaskRoom->mRoom[tmpCount][1] = tmpRoomNum;
                    tmpCount++;
                    if(tmpCount >= 16)
                        break;
                }
                if(tmpCount >= 16)
                    break;
            }
        }
    }
    else if(ui->stacked_maketoolcard->currentWidget() == ui->page_maketoolcard_openfloor)
    {
        ToolCard_Openfloor* tmpOpenFloor = ( ToolCard_Openfloor*)(tmpBuffer);
        tmpOpenFloor->mToolHeader.mState = 0;
        tmpOpenFloor->mToolHeader.mType = 104;

        QByteArray tmpOpFloor = GetOpenFlrs();
        memcpy(tmpOpenFloor->mOpenFloor,tmpOpFloor.constData(),12);
    }
    else if(ui->stacked_maketoolcard->currentWidget() == ui->page_maketoolcard_elevtimeallow)
    {
        ToolCard_Elevtimeallow* tmpElevtimeallow = (ToolCard_Elevtimeallow*)tmpBuffer;
        tmpElevtimeallow->mToolHeader.mState = 0;
        tmpElevtimeallow->mToolHeader.mType = 105;

        if(ui->checkBox_time1->isChecked())
        {
            tmpElevtimeallow->mTimeEnable1 = 1;

            QTime tmpT = ui->timeEdit_timeallowstart1->time();
            DateTime* tmpDT = (DateTime*)(tmpElevtimeallow->mTimeallowStart1);
            tmpDT->mTimeList.mHour = tmpT.hour();
            tmpDT->mTimeList.mMinute = tmpT.minute();
            tmpDT->mTimeList.mSecond =  tmpT.second();

            tmpT = ui->timeEdit_timeallowend1->time();
            tmpDT = (DateTime*)(tmpElevtimeallow->mTimeallowEnd1);
            tmpDT->mTimeList.mHour = tmpT.hour();
            tmpDT->mTimeList.mMinute = tmpT.minute();
            tmpDT->mTimeList.mSecond = tmpT.second();
        }
        else
        {
            tmpElevtimeallow->mTimeEnable1 = 0;
        }
        if(ui->checkBox_time2->isChecked())
        {
            tmpElevtimeallow->mTimeEnable2 = 1;

            QTime tmpT = ui->timeEdit_timeallowstart2->time();
            DateTime* tmpDT = (DateTime*)(tmpElevtimeallow->mTimeallowStart2);
            tmpDT->mTimeList.mHour = tmpT.hour();
            tmpDT->mTimeList.mMinute = tmpT.minute();
            tmpDT->mTimeList.mSecond = tmpT.second();

            tmpT = ui->timeEdit_timeallowend2->time();
            tmpDT = (DateTime*)(tmpElevtimeallow->mTimeallowEnd2);
            tmpDT->mTimeList.mHour = tmpT.hour();
            tmpDT->mTimeList.mMinute = tmpT.minute();
            tmpDT->mTimeList.mSecond = tmpT.second();
        }
        else
        {
            tmpElevtimeallow->mTimeEnable2 = 0;
        }
        if(ui->checkBox_time3->isChecked())
        {
            tmpElevtimeallow->mTimeEnable3 = 1;

            QTime tmpT = ui->timeEdit_timeallowstart3->time();
            DateTime* tmpDT = (DateTime*)(tmpElevtimeallow->mTimeallowStart3);
            tmpDT->mTimeList.mHour = tmpT.hour();
            tmpDT->mTimeList.mMinute = tmpT.minute();
            tmpDT->mTimeList.mSecond = tmpT.second();

            tmpT = ui->timeEdit_timeallowend3->time();
            tmpDT = (DateTime*)(tmpElevtimeallow->mTimeallowEnd3);
            tmpDT->mTimeList.mHour = tmpT.hour();
            tmpDT->mTimeList.mMinute = tmpT.minute();
            tmpDT->mTimeList.mSecond = tmpT.second();
        }
        else
        {
            tmpElevtimeallow->mTimeEnable3 = 0;
        }

        CardByte* tmpByte = (CardByte*)(&(tmpElevtimeallow->mWeek));
        tmpByte->mBits.mBit0 = (int)ui->check_week_1->isChecked();
        tmpByte->mBits.mBit1 = (int)ui->check_week_2->isChecked();
        tmpByte->mBits.mBit2 = (int)ui->check_week_3->isChecked();
        tmpByte->mBits.mBit3 = (int)ui->check_week_4->isChecked();
        tmpByte->mBits.mBit4 = (int)ui->check_week_5->isChecked();
        tmpByte->mBits.mBit5 = (int)ui->check_week_6->isChecked();
        tmpByte->mBits.mBit6 = (int)ui->check_week_7->isChecked();
    }
    else if(ui->stacked_maketoolcard->currentWidget() == ui->page_maketoolcard_activatevCard)
    {
        tmpHeader->mState = 0;
        tmpHeader->mType = 107;
    }
    else if(ui->stacked_maketoolcard->currentWidget() == ui->page_maketoolcard_elevatorNnum)
    {
        tmpHeader->mState = 0;
        tmpHeader->mType = 108;
    }
    else if(ui->stacked_maketoolcard->currentWidget() == ui->page_maketoolcard_lostCard)
    {
        ToolCard_LostCart* tmpLostCartData = (ToolCard_LostCart*)tmpBuffer;
        tmpLostCartData->mToolHeader.mState = 0;
        tmpLostCartData->mToolHeader.mType = 106;
        QString cardNumber = ui->line_LostCardNum->text();
        QStringList tmpCardNum = cardNumber.split(" ");

        for(int i=0;i<4;i++)
        {
            bool ok;
            QString tmpNumStr = tmpCardNum.at(i);
            int tmpNum = tmpNumStr.toInt(&ok,16);
            tmpLostCartData->userID[i] = tmpNum;
        }

    }

//    ShowMsgPrg(QString(), 80);

    QByteArray tmpArray((char*)tmpBuffer, 48);
    int count = mIcReader.WriteCard(1, tmpArray);
    if(count != -1 )
    {
        QMessageBox::information(0,GBKToUTF8("写卡成功"),
                              GBKToUTF8("数据已成功写入"));

    }
//    ShowMsgPrg(QString(), 100);
}

void ICManager::AddCellInfoToDB(QString pUnit,QString pLayer,QString pFamily)
{
    int index1 = mCellUnitList.indexOf(pUnit);
    if(index1 < 0)//没找到，添加新单元
    {
        mCellUnitList.append(pUnit);
        QList<QVariant> tmpFieldUnitValues;
        tmpFieldUnitValues << pUnit;
        if(!mSqlParser->AddRow("addr_units",tmpFieldUnitValues))
        {
            QMessageBox::critical(0,GBKToUTF8("添加单元信息错误"),
                                    GBKToUTF8("无法添加单元信息，请检查填写是否正确"),
                                    QMessageBox::Ok);
            return;
        }
    }

    QSqlQuery tmpQuery = mSqlParser->Exec(QString("select * from addr_layers where layer_num=\"%1\" and unit_num=\"%2\"")
                                        .arg(pLayer).arg(pUnit));
    if(!tmpQuery.next())
    {
        QList<QVariant> tmpFieldLayerValues;
        tmpFieldLayerValues << pUnit << pLayer << pLayer;
        if(!mSqlParser->AddRow("addr_layers",tmpFieldLayerValues))
        {
            QMessageBox::critical(0,GBKToUTF8("添加楼层信息错误"),
                                    GBKToUTF8("无法添加楼层信息，请检查填写是否正确"),
                                    QMessageBox::Ok);
            return;
        }
    }

    tmpQuery = mSqlParser->Exec(QString("select * from addr_familys where family_num=\"%1\" and layer_num=\"%2\" and unit_num=\"%3\"")
                                        .arg(pFamily).arg(pLayer).arg(pUnit));
    if(tmpQuery.next()) //此单元、层、楼层在数据库中存在
    {
        QMessageBox::critical(0,GBKToUTF8("添加小区信息错误"),
                                GBKToUTF8("此信息已存在，无法添加此信息"),
                                QMessageBox::Ok);
        return;
    }
    else //在数据库中不存在
    {
        QList<QVariant> tmpFieldFamilyValues;
        tmpFieldFamilyValues << pUnit << pLayer << pFamily;
        if(!mSqlParser->AddRow("addr_familys",tmpFieldFamilyValues))
        {
            QMessageBox::critical(0,GBKToUTF8("添加室错误"),
                                    GBKToUTF8("无法添加室信息，请检查填写是否正确"),
                                    QMessageBox::Ok);
            return;
        }

        AddCellInfo(pUnit, pLayer, pFamily);
    }
}


void ICManager::AddElevatorToDB(QString pElevNum,QString pUnitNum,QString pCtrlLayerNum)
{
    QString tmpKey = QString("%1&%2").arg(pElevNum).arg(pUnitNum);

    int index = mElevatorList.indexOf(tmpKey);
    if(index >= 0)
    {
        QMessageBox::StandardButton tmpret = QMessageBox::question(0,GBKToUTF8("更新询问"),
                                                                   GBKToUTF8("是否更改电梯信息"),
                                                                   QMessageBox::Ok|QMessageBox::Cancel);
        if(tmpret != QMessageBox::Ok)
        {
            return;
        }
    }

    QList<QVariant> tmpFieldVaules;
    tmpFieldVaules << pElevNum << pUnitNum << pCtrlLayerNum;

    QString tmpFilter = QString("elev_num='%1' and unit_num='%2'").arg(pElevNum).arg(pUnitNum);
    if(!mSqlParser->ModifyRow("elevator",tmpFilter,tmpFieldVaules))
    {
        QMessageBox::critical(0,GBKToUTF8("添加电梯错误"),
                              GBKToUTF8("无法添加此电梯，请检查填写是否正确"));
        return;
    }

    if(index >= 0)
    {
        ui->table_elevator->item(index, 0)->setText(pElevNum);
        ui->table_elevator->item(index, 1)->setText(pUnitNum);
        ui->table_elevator->item(index, 2)->setText(pCtrlLayerNum);
    }
    else
    {
        AddElevator(pElevNum, pUnitNum, pCtrlLayerNum);
        QMessageBox::information(0,GBKToUTF8("新增电梯"),GBKToUTF8("新增电梯成功"));
    }

}

void ICManager::AddLostCardToDB(QString pCardNum,QString pPassword)
{
    QMessageBox::StandardButton tmpret = QMessageBox::question(0,GBKToUTF8("更新询问"),
                                                                       GBKToUTF8("是否更改卡信息"),
                                                                       QMessageBox::Ok|QMessageBox::Cancel);
    if(tmpret != QMessageBox::Ok)
    {
        return;
    }

    //写入卡
    LossCard tmpData;
    memset(&tmpData,0,sizeof(LossCard));

    //密码为123456
    CardPasswd *tmpPw = (CardPasswd *) (&tmpData);
    tmpPw->mPwList.mPw1 = 1;
    tmpPw->mPwList.mPw2 = 2;
    tmpPw->mPwList.mPw3 = 3;
    tmpPw->mPwList.mPw4 = 4;
    tmpPw->mPwList.mPw5 = 5;
    tmpPw->mPwList.mPw6 = 6;

    tmpData.mState = 1;
    tmpData.mType = 106;
    tmpData.mElevator = ui->lineEdit_Elevator_number->text().toInt();
    QString cardNumber = ui->lineEdit_card_number->text();
    QStringList tmpCardNum = cardNumber.split(" ");

    for(int i=0;i<4;i++)
    {
        bool ok;
        QString tmpNumStr = tmpCardNum.at(i);
        int tmpNum = tmpNumStr.toInt(&ok,16);
        tmpData.mUserId[i] = tmpNum;
    }

    QByteArray tmpBuffer((char *)(&tmpData),48);
    mIcReader.WriteCard(1,tmpBuffer);

}


void ICManager::AddUser(QString pName,QString pAddress,QString pPhone,int pCardCount,QString pInfo)
{
    int row = ui->table_user->rowCount();
    ui->table_user->insertRow(row);

    QTableWidgetItem *tmpItem0 = new QTableWidgetItem;
    tmpItem0->setText(pName);
    tmpItem0->setFlags(tmpItem0->flags() & ~(Qt::ItemIsEditable));
    tmpItem0->setIcon(QIcon(":/icon/icon/1.png"));
    ui->table_user->setItem(row, 0, tmpItem0);

    QTableWidgetItem *tmpItem1 = new QTableWidgetItem;
    tmpItem1->setText(pAddress);
    tmpItem1->setFlags(tmpItem1->flags() & ~(Qt::ItemIsEditable));
    ui->table_user->setItem(row, 1, tmpItem1);

    QTableWidgetItem *tmpItem2 = new QTableWidgetItem;
    tmpItem2->setText(pPhone);
    tmpItem2->setTextAlignment(Qt::AlignCenter);
    tmpItem2->setFlags(tmpItem2->flags() & ~(Qt::ItemIsEditable));
    ui->table_user->setItem(row, 2, tmpItem2);

    QTableWidgetItem *tmpItem3 = new QTableWidgetItem;
    tmpItem3->setText(QString("%1").arg(pCardCount));
    tmpItem3->setTextAlignment(Qt::AlignCenter);
    tmpItem3->setFlags(tmpItem3->flags() & ~(Qt::ItemIsEditable));
    ui->table_user->setItem(row, 3, tmpItem3);

    UsrItem *tmpItem4 = new UsrItem;
    ui->table_user->setCellWidget(row, 4, tmpItem4);
    connect(tmpItem4, SIGNAL(sCardRead()), this, SLOT(slot_CardRead()));
    connect(tmpItem4, SIGNAL(sMakeUsrCard()), this, SLOT(slot_MakeUsrCard()));
    connect(tmpItem4, SIGNAL(sDestoryCard()), this, SLOT(slot_DestoryUsrCard()));

    QTableWidgetItem *tmpItem5 = new QTableWidgetItem;
    tmpItem5->setText(pInfo);
    tmpItem5->setFlags(tmpItem5->flags() & ~(Qt::ItemIsEditable));
    ui->table_user->setItem(row, 5, tmpItem5);

    mUserList.append(pName);
}

void ICManager::slot_CardRead()
{
    GotoPage(E_PAGE_CARDLIST);
}

void ICManager::slot_ReDoCard()
{
    GotoPage(E_PAGE_REMAKECARD);
}

void ICManager::slot_CardRepair()
{
    QMessageBox::StandardButton tmpBtn = QMessageBox::question(0,GBKToUTF8("卡片询问"),
                                                               GBKToUTF8("是否已经放入新卡片"),
                                                               QMessageBox::Ok|QMessageBox::Cancel);
    if(tmpBtn != QMessageBox::Ok)
    {
        return;
    }

    //将次卡片保存的内容写入新卡

    QWidget *tmpWid = (QWidget*)(sender());
    if(tmpWid ==0)
        return;
    for(int i=0;i<ui->table_cardlist->rowCount();i++)
    {
        QWidget *tmpItem = ui->table_cardlist->cellWidget(i, 6);
        if(tmpItem == tmpWid)
        {
            QString tmpNum = ui->table_cardlist->item(i, 0)->text();

            return;
        }
    }
}

void ICManager::slot_CardLoss()
{
    QMessageBox::StandardButton tmpBtn = QMessageBox::question(0,GBKToUTF8("询问"),
                                                               GBKToUTF8("是否要注销此卡片"),
                                                               QMessageBox::Ok|QMessageBox::Cancel);
    if(tmpBtn != QMessageBox::Cancel)
    {
        return;
    }

    QWidget *tmpWid = (QWidget*)(sender());
    if(tmpWid ==0)
        return;
    for(int i=0;i<ui->table_cardlist->rowCount();i++)
    {
        QWidget *tmpItem = ui->table_cardlist->cellWidget(i, 6);
        if(tmpItem == tmpWid)
        {
            QString tmpNum = ui->table_cardlist->item(i, 0)->text();

            return;
        }
    }

}
void ICManager::slot_CardLogRemove()
{
    QWidget *tmpWid = (QWidget*)(sender());
    if(tmpWid ==0)
        return;
    for(int i=0;i<ui->table_cardlist->rowCount();i++)
    {
        if(ui->table_cardlist->cellWidget(i, 6) == tmpWid)   //放按钮的table单元格取cellWidget，放文字的取item
        {
            QString CardNum = ui->table_cardlist->item(i, 0)->text();
            DelCardFromDB(CardNum,i);
        }
    }
}

void ICManager::slot_CardLog()
{
    GotoPage(E_PAGE_CARDLOG);
}

void ICManager::slot_DestoryUsrCard()
{
    UsrItem *tmpItem = (UsrItem *)sender();
    for(int i=0;i<ui->table_user->rowCount();i++)
    {
        if(ui->table_user->cellWidget(i, 4) == tmpItem)   //放按钮的table单元格取cellWidget，放文字的取item
        {
            QString tmpName = ui->table_user->item(i, 0)->text();
            DelUserFromDB(tmpName);
            break;
        }
    }
}

void ICManager::slot_DestoryAdminCard()
{
    AdminItem *tmpItem = (AdminItem *)sender();
    for(int i=0;i<ui->table_admin->rowCount();i++)
    {
        if(ui->table_admin->cellWidget(i, 4) == tmpItem)
        {
            QString tmpName = ui->table_admin->item(i, 0)->text();
            DelAdminFromDB(tmpName);
            break;
        }
    }
}
void ICManager::slot_SetAdminPassword()
{
    AdminItem *tmpItem = (AdminItem *)sender();
    for(int i=0;i<ui->table_admin->rowCount();i++)
    {
        if(ui->table_admin->cellWidget(i,4) == tmpItem)
        {
            QString tmpPassword;
            QString tmpName = ui->table_admin->item(i,0)->text();
            QSqlQuery tmpPasswordQuery = mSqlParser->Exec(QString("select passwd from person_admin where name=\"%1\"").arg(tmpName));
            if(tmpPasswordQuery.next())
            {
                QSqlRecord tmpPasswordRecord = tmpPasswordQuery.record();
                tmpPassword = tmpPasswordRecord.value("passwd").toString();
                ui->line_admin_password->setText(tmpPassword);
            }

        }
    }
    resetPassword = true;
}

void ICManager::slot_MakeUsrCard()
{
    GotoPage(E_PAGE_MAKEOWERCARD);
    UsrItem *tmpItem = (UsrItem *)sender();
    for(int i=0;i<ui->table_user->rowCount();i++)
    {
        if(ui->table_user->cellWidget(i, 4) == tmpItem)
        {
            ui->line_business_name->setText(ui->table_user->item(i, 0)->text());
            break;
        }
    }
}

void ICManager::slot_MakeAdminCard()
{
    GotoPage(E_PAGE_MAKEADMINCARD);
    UsrItem *tmpItem = (UsrItem *)sender();
    for(int i=0;i<ui->table_admin->rowCount();i++)
    {
        if(ui->table_admin->cellWidget(i, 4) == tmpItem)
        {
            ui->line_makeadmin_name->setText(ui->table_admin->item(i, 0)->text());
            break;
        }
    }
}

void ICManager::slot_MadeCardsNum()
{
    GotoPage(E_PAGE_CARDLIST);
}

void ICManager::on_btn_user_edit_ok_clicked()
{
    QString tmpName = ui->line_user_name->text();
    QString tmpPhone = ui->line_user_phone->text();
    QString tmpUnitNum = ui->combox_user_unit_num->currentText();
    QString tmpLayerNum = ui->combox_user_layer_num->currentText();
    QString tmpFamilyNum = ui->combox_user_family_num->currentText();

    tmpLayerNum = GetFlrNum(tmpLayerNum);  //反映射

    //if(ui->stacked_useredit->currentWidget() == ui->page_user_add)  //编辑页面

        //Detail
        QStringList tmpEleList;
        QList<QCheckBox*> tmpList = ui->wid_valideles->findChildren<QCheckBox*>();
        foreach(QCheckBox* tmpBox, tmpList)
        {
            if(tmpBox->isChecked())
            {
                tmpEleList.append(tmpBox->text().remove(QString(GBKToUTF8("电梯"))));
            }
            else
                continue;
        }
//        QStringList tmpLayerList;
//        tmpList = ui->wid_validlayers->findChildren<QCheckBox*>();
//        for(int i=0;i<tmpList.count();i++)
//        {
//            QCheckBox* tmpBox = tmpList.at(i);
//            if(!tmpBox)
//                continue;
//            if(tmpBox->isChecked())
//            {
//                QString tmpText = tmpBox->text();
//                int index = tmpText.indexOf(QString(GBKToUTF8("层")));
//                if(index)
//                {
//                    QString tmpLayer = tmpText.mid(0, index);
                    //获取此映射对应的实际楼层
    //                QSqlQuery tmpQuery = mSqlParser->Exec(QString("select layer_num from addr_layers where unit_num=\"%1\" and layer_map=\"%2\"").arg(tmpUnitNum).arg(tmpKey));
    //                if(tmpQuery.next())
    //                {
    //                    QSqlRecord tmpRecord = tmpQuery.record();
    //                    QString tmpLayersNum = tmpRecord.value("layer_num").toString();
    //                    tmpLayerList.append(tmpLayersNum);
    //                }
//                    QString tmpLayersNum = GetFlrNum(tmpLayer);
//                    tmpLayerList.append(tmpLayersNum);
//                }
//            }
//        }
//        QString tmpDetail  = QString("%1;%2").arg(tmpEleList.join(",")).arg(tmpLayerList.join(","));
        QString tmpDetail  = QString("%1;").arg(tmpEleList.join(","));

        AddUserToDB(tmpName, tmpUnitNum, tmpLayerNum, tmpFamilyNum, tmpPhone, tmpDetail);
    /*else if(ui->stacked_useredit->currentWidget() == ui->page_user_del)  //删除页面
    {
        DelUserFromDB(tmpName);
    }*/
}

void ICManager::SetCorrespdFlrs(int pNum, char *pBuffer)
{
    for(int column=0;column<12;column++)
    {
        for(int row=0;row<8;row++)
        {
            int tmpFlr = column *8 + row + 1;     //一个个遍历所有的楼层
            QCheckBox *tmpCheck = mValidFlrs[pNum].value(tmpFlr);   //得出楼层对应的的CheckBox
            if(tmpCheck)     //如果checkbox存在
            {
                if(pBuffer[column] & (1<<row))   //判断“卡数据结构中可用楼层”一列所对应的8行数据是否为1
                    tmpCheck->setChecked(true);    //为1就设置打钩
                else
                    tmpCheck->setChecked(false);
            }
        }
    }
}

QByteArray ICManager::GetCorrespdFlrs(int pNum)
{
    char tmpBuffer[12] = {0};
    QList<int> tmpFlrKeyList = mValidFlrs[pNum].keys();
    for(int i=0;i<tmpFlrKeyList.count();i++)
    {
        int tmpFlrKey = tmpFlrKeyList.at(i);
        QCheckBox *tmpBtn = mValidFlrs[pNum].value(tmpFlrKey);
        int tmpFlrByteIndex = 0, tmpFlrBitIndex = 0;
        tmpFlrByteIndex = (tmpFlrKey-1) / 8;
        tmpFlrBitIndex = (tmpFlrKey-1) % 8;
        if(tmpBtn->isChecked())
            tmpBuffer[tmpFlrByteIndex] |= (1<<tmpFlrBitIndex);
        else
            tmpBuffer[tmpFlrByteIndex] &= ~(1<<tmpFlrBitIndex);
    }
    return QByteArray(tmpBuffer, 12);
}


QByteArray ICManager::GetOpenFlrs()
{
    char tmpBuffer[12] = {0};
    QList<QString> tmpFlrKeyList = mOpenFlrs.keys();
    for(int i=0;i<tmpFlrKeyList.count();i++)
    {
        QString tmpFlrKey = tmpFlrKeyList.at(i);
        QCheckBox *tmpBtn = mOpenFlrs.value(tmpFlrKey);
        int tmpFlrByteIndex = 0, tmpFlrBitIndex = 0;
        int tmpFlrNumber = tmpFlrKey.toInt();
        tmpFlrByteIndex = (tmpFlrNumber-1) / 8;
        tmpFlrBitIndex = (tmpFlrNumber-1) % 8;
        if(tmpBtn->isChecked())
            tmpBuffer[tmpFlrByteIndex] |= (1<<tmpFlrBitIndex);
        else
            tmpBuffer[tmpFlrByteIndex] &= ~(1<<tmpFlrBitIndex);
    }
    return QByteArray(tmpBuffer, 12);
}

QByteArray ICManager::GetUserOpenFlrs()
{
    char tmpBuffer[12] = {0};
    if(mUserOpenFlrs.isEmpty())
    {
        return QByteArray(tmpBuffer, 12);
    }
    QList<QString> tmpFlrKeyList = mUserOpenFlrs.keys();
    for(int i=0;i<tmpFlrKeyList.count();i++)
    {
        QString tmpFlrKey = tmpFlrKeyList.at(i);
        QString tmpFlrNum = GetFlrNum(tmpFlrKey);
        QCheckBox *tmpBtn = mUserOpenFlrs.value(tmpFlrKey);
        int tmpFlrByteIndex = 0, tmpFlrBitIndex = 0;
        int tmpFlrNumber = tmpFlrNum.toInt();
        tmpFlrByteIndex = (tmpFlrNumber-1) / 8;
        tmpFlrBitIndex = (tmpFlrNumber-1) % 8;
        if(tmpBtn->isChecked())
            tmpBuffer[tmpFlrByteIndex] |= (1<<tmpFlrBitIndex);
        else
            tmpBuffer[tmpFlrByteIndex] &= ~(1<<tmpFlrBitIndex);
    }
    return QByteArray(tmpBuffer, 12);
}

void ICManager::on_comboBox_toolcard_units_currentIndexChanged(const QString &arg1)
{
    InitCorrespdElevt(arg1.toInt());
}

void ICManager::InitCorrespdElevt(int pUnitNum)
{
    //初始化此单元楼有哪些电梯
    int tmpWidth = 70;
    int tmpHeight = 35;
    ui->scroll_unitElevt->setFixedSize(QSize(tmpWidth*4 + 20, tmpHeight*2));

    QList<QCheckBox*> tmplist = mUnitElevt.values();
    if(!tmplist.isEmpty())
    {
        for(int i=tmplist.count()-1;i>=0;i--)
        {
            tmplist.at(i)->deleteLater();
        }
    }
    mUnitElevt.clear();

    QStringList tmpEleList;
    QSqlQuery tmpElevatorQuery = mSqlParser->Exec(QString("select * from elevator where unit_num=\"%1\"").arg(pUnitNum));
    while(tmpElevatorQuery.next())
    {
        QSqlRecord tmpRecord = tmpElevatorQuery.record();
        QString tmpElevt = tmpRecord.value("elev_num").toString();
        tmpEleList.append(tmpElevt);
    }

    for(int i=0;i<tmpEleList.count();i++)
    {
        int row = i % 2;
        int column = i / 2;
        QCheckBox *tmpCheck = new QCheckBox(ui->scroll_unitElevt);
        QRect tmpRect = QRect(tmpWidth*column+20, tmpHeight*row, tmpWidth, tmpHeight);
        tmpCheck->setGeometry(tmpRect);
        QString tmpEleNum = tmpEleList.at(i);
        tmpCheck->setText(QString(GBKToUTF8("电梯%1")).arg(tmpEleNum));
        tmpCheck->show();
        mUnitElevt.insert(tmpEleNum, tmpCheck);
    }

    //初始化有多少层以及每层有哪些房间
    ui->combo_readcard_selectfloor->clear();
    QStringList  tmpLayerList;

    InitFlrMap(QString::number(pUnitNum));

    QSqlQuery tmpLayerQuery = mSqlParser->Exec(QString("select * from addr_layers where unit_num=\"%1\"").arg(pUnitNum));
    while(tmpLayerQuery.next())
    {
        QSqlRecord tmpRecord = tmpLayerQuery.record();
        QString tmpLayer = tmpRecord.value("layer_num").toString();

        QString tmpLayerMap = GetFlrMap(tmpLayer);
        tmpLayerList.append(tmpLayer);
        ui->combo_readcard_selectfloor->addItem(tmpLayerMap);//卡记录单元所对应的所有楼层
    }

    QList<QCheckBox*> FlrList = mOpenFlrs.values();  //清空公共楼层hash表中的值
    if(!FlrList.isEmpty())
    {
        for(int i=FlrList.count()-1;i>=0;i--)
        {
            FlrList.at(i)->deleteLater();
        }
    }
    mOpenFlrs.clear();

    ui->tabWidget_rooms->clear();
    QList<QCheckBox*> tmpChdList = ui->scrollContents_openfloor->findChildren<QCheckBox*>();//取出每个widget上所有checkbox
    foreach(QCheckBox* ckbtn,tmpChdList)
    {
        ckbtn->deleteLater();
    }

    for(int i=0;i<tmpLayerList.count();i++)
    {
        QString tmpLayer = tmpLayerList.at(i);
        QString tmpLayerMap = GetFlrMap(tmpLayer);
        //为page_maketoolcard_maskroom初始化
        QStringList tmpRoomList;
        QSqlQuery tmpRoomQuery = mSqlParser->Exec(QString("select * from addr_familys where (layer_num=\"%1\")and(unit_num=\"%2\")").arg(tmpLayer).arg(pUnitNum));
        while(tmpRoomQuery.next())
        {
            QSqlRecord tmpRecord = tmpRoomQuery.record();
            QString tmpRoom = tmpRecord.value("family_num").toString();
            tmpRoomList.append(tmpRoom);
        }
        QWidget *tmpWidget = new QWidget;
        for(int j=0;j<tmpRoomList.count();j++)
        {
            QString tmpRoom = tmpRoomList.at(j);
            int row = j % 4;
            int column = j / 4;
            QCheckBox *tmpBox = new QCheckBox(tmpWidget);
            QRect tmpRect = QRect(tmpWidth*column+20, tmpHeight*row, tmpWidth, tmpHeight);
            tmpBox->setGeometry(tmpRect);
            tmpBox->setText(QString(GBKToUTF8("%1室")).arg(tmpRoom));
        }
        ui->tabWidget_rooms->addTab(tmpWidget, QIcon(), QString(GBKToUTF8("%1层")).arg(tmpLayerMap));

        //为page_maketoolcard_openfloor初始化
        {
            int row = i % 4;
            int column = i / 4;
            QCheckBox *tmpBox = new QCheckBox(ui->scrollContents_openfloor);
            QRect tmpRect = QRect(tmpWidth*column+20, tmpHeight*row, tmpWidth, tmpHeight);
            tmpBox->setGeometry(tmpRect);
            tmpBox->setText(QString(GBKToUTF8("%1层")).arg(tmpLayerMap));
            tmpBox->show();
            mOpenFlrs.insert(tmpLayer,tmpBox);
        }
    }
}

void ICManager::on_btn_admin_ok_clicked()
{
    bool bit0,bit1,bit2,bit3,bit4,bit5,bit6 = 0;

    QString tmpName = ui->line_admin_name->text();
    QString tmpPhone = ui->line_admin_phone->text();
    QString tmpAddress = ui->line_admin_address->text();
    QString tmpAdminPassword = ui->line_admin_password->text();

   // if(ui->stacked_admin->currentWidget() == ui->page_adminadd)
    {
        QString tmpTopUp = ui->comboBox_topup->currentText();

        if(ui->checkBox_adminPower_makelevatimeallow->isChecked())
        {
            bit0 = 1;
        }
        else
        {
            bit0 = 0;
        }
        if(ui->checkBox_adminPower_makeopenfloorcard->isChecked())
        {
            bit1 = 1;
        }
        else
        {
            bit1 = 0;
        }
        if(ui->checkBox_adminPower_makemaskroomcard->isChecked())
        {
            bit2 = 1;
        }
        else
        {
            bit2 = 0;
        }
        if(ui->checkBox_adminPower_makereadcard->isChecked())
        {
            bit3 = 1;
        }
        else
        {
            bit3 = 0;
        }
        if(ui->checkBox_adminPower_makesyntimecard->isChecked())
        {
            bit4 = 1;
        }
        else
        {
            bit4 = 0;
        }
        if(ui->checkBox_adminPower_makeadmincard->isChecked())
        {
            bit5 = 1;
        }
        else
        {
            bit5 = 0;
        }
        if(ui->checkBox_adminPower_makeownercard->isChecked())
        {
            bit6 = 1;
        }
        else
        {
            bit6 = 0;
        }

        int tmpValue = 0;
        if(bit0 == true)
        {
            tmpValue |= (1<<0);
        }
        else
        {
            tmpValue &= ~(1<<0);
        }
        if(bit1 == true)
        {
            tmpValue |= (1<<1);
        }
        else
        {
            tmpValue &= ~(1<<1);
        }
        if(bit2 == true)
        {
            tmpValue |= (1<<2);
        }
        else
        {
            tmpValue &= ~(1<<2);
        }
        if(bit3 == true)
        {
            tmpValue |= (1<<3);
        }
        else
        {
            tmpValue &= ~(1<<3);
        }
        if(bit4 == true)
        {
            tmpValue |= (1<<4);
        }
        else
        {
            tmpValue &= ~(1<<4);
        }
        if(bit5 == true)
        {
            tmpValue |= (1<<5);
        }
        else
        {
            tmpValue &= ~(1<<5);
        }
        if(bit6 == true)
        {
            tmpValue |= (1<<6);
        }
        else
        {
            tmpValue &= ~(1<<6);
        }

        AddAdminToDB(tmpName,tmpPhone,tmpAddress,tmpValue,tmpTopUp,tmpAdminPassword);
        ui->line_admin_password->clear();
        resetPassword = false;
    }

   /* else if(ui->stacked_admin->currentWidget()== ui->page_admindel)
    {
        DelAdminFromDB(tmpName);
    }*/
}

void ICManager::on_btn_makeadmincard_ok_clicked()
{
    QString tmpName = ui->line_makeadmin_name->text();
    QString tmpCardNum = ui->line_makeadmin_cardnum->text();
    QString tmpPassword = ui->line_makeadmin_password->text();

    AddMakeAdminCardToDB(tmpName,tmpCardNum,tmpPassword);
}

void ICManager::on_btn_makecard_ok_clicked()
{
   QString tmpName = ui->line_business_name->text();
   QString tmpCardNum = ui->line_business_cardnum->text();
   QString tmpPassword = ui->line_business_password->text();

   AddMakeCardToDB(tmpName,tmpCardNum,tmpPassword);
}

void ICManager::on_btn_toolcard_ok_clicked()
{
    QString tmpCardNum = ui->line_maketoolcard_cardnum->text();
    QString tmpPassword = ui->line_maketoolcard_password->text();

    AddToolCardToDB(mAdminName,tmpCardNum,tmpPassword);
    if(ui->stacked_maketoolcard->currentWidget() == ui->page_maketoolcard_lostCard)
    {
        QString tmpUserCardNum = ui->line_LostCardNum->text();
        QSqlQuery tmpQuery = mSqlParser->Exec(QString("select * from card_list where card_num=\"%1\"")
                                            .arg(tmpUserCardNum));
        if(tmpQuery.next())
        {
            QSqlRecord tmpRecord = tmpQuery.record();
            QString pName = tmpRecord.value("name").toString();
            int pType = tmpRecord.value("card_type").toInt();
            QList<QVariant> tmpFieldVaules;
            tmpFieldVaules << tmpUserCardNum << pType << pName << "" <<GBKToUTF8("已挂失") << "" << "";  //界面参数在更改情况下，此条才会加到数据库中，内容改为此条信息（界面中的参数）
            if(!mSqlParser->ModifyRow("card_list","card_num",tmpUserCardNum,tmpFieldVaules))
            {
                QMessageBox::critical(0,GBKToUTF8("添加卡信息错误"),
                                      GBKToUTF8("无法添加此卡，请检查填写是否正确"));
                return;
            }
        }

    }
}

void ICManager::on_btn_cellinfo_ok_clicked()
{
    QString tmpUnit = ui->line_cellunit->text();
    QString tmpLayer = ui->line_celllayer->text();
    QString tmpFamily = ui->line_cellfamily->text();
    tmpLayer = GetFlrNum(tmpLayer);
    if(ui->check_addAuto->isChecked())
    {
        int numLayer = tmpLayer.toInt();
        int numFamily = tmpFamily.toInt();
        for(int i=1;i<=numLayer;i++)
        {
            for(int j=1;j<=numFamily;j++)
            {
                tmpLayer = QString::number(i,10);
                tmpFamily = QString::number(j,10);
                AddCellInfoToDB(tmpUnit,tmpLayer,tmpFamily);
            }
        }
    }
    else
    {
        AddCellInfoToDB(tmpUnit,tmpLayer,tmpFamily);
    }

}

void ICManager::on_btn_elevator_ok_clicked()
{
    QString tmpElevNum = ui->line_elevnum->text();
    QString tmpUnitNum = ui->combo_unitnum->currentText();
    QString tmpCtrlLayerNum = ui->line_ctrl_layernum->text();

    AddElevatorToDB(tmpElevNum,tmpUnitNum,tmpCtrlLayerNum);
}


void ICManager::on_btn_cardlist_search_clicked()
{
    QString tmpCardNum = ui->line_search_card->text();
    for(int i=0;i<ui->table_cardlist->rowCount();i++)
    {
        QTableWidgetItem *tmpItem = ui->table_cardlist->item(i, 0);
        if(!tmpItem->text().compare(tmpCardNum))
        {
            ui->table_cardlist->setCurrentCell(i, 0);
            return;
        }
    }
    QString tmpUserName = ui->line_search_card->text();
    for(int i=0;i<ui->table_cardlist->rowCount();i++)
    {
        QTableWidgetItem *tmpNameItem = ui->table_cardlist->item(i,2);
        if(!tmpNameItem->text().compare(tmpUserName))
        {
            ui->table_cardlist->setCurrentCell(i,2);
            return;
        }
    }
}

void ICManager::on_btn_user_search_clicked()
{
    QString tmpSearchUsr = ui->line_user_search->text();
    int totalRow = ui->table_user->rowCount();
    for(int i=0;i<totalRow;i++)
    {
        QTableWidgetItem *tmpNameItem = ui->table_user->item(i, 0);
        if(!tmpNameItem->text().compare(tmpSearchUsr))
        {
            ui->table_user->setCurrentCell(i, 0);
            return;
        }

        QTableWidgetItem *tmpPhoneItem = ui->table_user->item(i, 2);
        if(!tmpPhoneItem->text().compare(tmpSearchUsr))
        {
            ui->table_user->setCurrentCell(i, 2);
            return;
        }
    }
}


void ICManager::on_btn_admin_search_clicked()
{
    QString tmpSearchAdmin = ui->line_admin_search->text();
    int totalRow = ui->table_admin->rowCount();
    for(int i=0;i<totalRow;i++)
    {
        QTableWidgetItem *tmpNameItem = ui->table_admin->item(i, 0);
        if(!tmpNameItem->text().compare(tmpSearchAdmin))
        {
            ui->table_admin->setCurrentCell(i, 0);
            return;
        }

        QTableWidgetItem *tmpPhoneItem = ui->table_admin->item(i, 2);
        if(!tmpPhoneItem->text().compare(tmpSearchAdmin))
        {
            ui->table_admin->setCurrentCell(i, 2);
            return;
        }
    }

}



void ICManager::on_btn_maskelevt_clicked()
{
    for(int i=0;i<ui->tabWidget_rooms->count();i++)
    {
        QWidget* tmpWidget= ui->tabWidget_rooms->widget(i);
        QList<QCheckBox*> tmpCheckList = tmpWidget->findChildren<QCheckBox*>();
        for(int j=0;j<tmpCheckList.count();j++)
        {
            QCheckBox *tmpCheck = tmpCheckList.at(j);
            tmpCheck->setChecked(true);
        }
    }
}

void ICManager::on_btn_enablelevt_clicked()
{
    for(int i=0;i<ui->tabWidget_rooms->count();i++)
    {
        QWidget* tmpWidget = ui->tabWidget_rooms->widget(i);
        QList<QCheckBox*> tmpCheckList = tmpWidget->findChildren<QCheckBox*>();
        for(int j=0;j<tmpCheckList.count();j++)
        {
            QCheckBox *tmpCheck = tmpCheckList.at(j);
            tmpCheck->setChecked(false);
        }
    }
}

void ICManager::on_btn_maskfloor_clicked()
{
    QWidget* tmpWidget = ui->tabWidget_rooms->currentWidget();
    QList<QCheckBox*> tmpCheckList = tmpWidget->findChildren<QCheckBox*>();
    for(int j=0;j<tmpCheckList.count();j++)
    {
        QCheckBox *tmpCheck = tmpCheckList.at(j);
        tmpCheck->setChecked(true);
    }

}

void ICManager::on_btn_enablefloor_clicked()
{
    QWidget* tmpWidget = ui->tabWidget_rooms->currentWidget();
    QList<QCheckBox*> tmpCheckList = tmpWidget->findChildren<QCheckBox*>();
    for(int j=0;j<tmpCheckList.count();j++)
    {
        QCheckBox *tmpCheck = tmpCheckList.at(j);
        tmpCheck->setChecked(false);
    }
}

void ICManager::on_btn_owner_list_clicked()
{
    mReadCardFlag = 0;
    GotoPage(E_PAGE_USER);
}

void ICManager::on_combox_user_unit_num_currentIndexChanged(const QString &arg1)
{
    //初始化有多少层以及每层有哪些房间
    QStringList tmpLayerList;

    InitFlrMap(arg1);

    QSqlQuery tmpLayerQuery = mSqlParser->Exec(QString("select * from addr_layers where unit_num=\"%1\"").arg(arg1));
    while(tmpLayerQuery.next())
    {
        QSqlRecord tmpRecord = tmpLayerQuery.record();
        QString tmpLayer = tmpRecord.value("layer_num").toString();

        tmpLayer = GetFlrMap(tmpLayer);

        tmpLayerList.append(tmpLayer);
    }
    ui->combox_user_layer_num->clear();
    ui->combox_user_layer_num->addItems(tmpLayerList);

    //初始化此单元楼有哪些电梯
    int tmpWidth = 70;
    int tmpHeight = 30;

    QStringList tmpEleList;
    QSqlQuery tmpElevatorQuery = mSqlParser->Exec(QString("select * from elevator where unit_num=\"%1\"").arg(arg1));
    while(tmpElevatorQuery.next())
    {
        QSqlRecord tmpRecord = tmpElevatorQuery.record();
        QString tmpElevt = tmpRecord.value("elev_num").toString();
        tmpEleList.append(tmpElevt);
    }

    QList<QCheckBox*> tmpList = ui->wid_valideles->findChildren<QCheckBox*>();
    foreach(QCheckBox* tmpBox, tmpList)
    {
        tmpBox->deleteLater();
    }

    ui->wid_valideles->setFixedSize(QSize(tmpWidth*4 + 20, tmpHeight*2));
    for(int j=0;j<tmpEleList.count();j++)
    {
        QString tmpEle = tmpEleList.at(j);
        int row = j % 2;
        int column = j / 2;
        QCheckBox *tmpBox = new QCheckBox(ui->wid_valideles);
        QRect tmpRect = QRect(tmpWidth*column+20, tmpHeight*row, tmpWidth, tmpHeight);
        tmpBox->setGeometry(tmpRect);
        tmpBox->setText(QString(GBKToUTF8("电梯%1")).arg(tmpEle));
        tmpBox->show();
    }
    QList<QCheckBox*> FlrList = mUserOpenFlrs.values();  //清空公共楼层hash表中的值
    if(!FlrList.isEmpty())
    {
        for(int i=FlrList.count()-1;i>=0;i--)
        {
            FlrList.at(i)->deleteLater();
        }
    }
    mUserOpenFlrs.clear();

    QList<QCheckBox*> ValidFloorList = ui->widget_validfloors->findChildren<QCheckBox*>();
    foreach(QCheckBox* tmpBox, ValidFloorList)
    {
        tmpBox->deleteLater();
    }
//    ui->widget_validfloors->setFixedSize(QSize(tmpWidth*15 + 20, tmpHeight*4));
    for(int j=0;j<tmpLayerList.count();j++)
    {
        QString tmpLayer = tmpLayerList.at(j);
        int row = j % 3;
        int column = j / 3;
        QCheckBox *tmpBox = new QCheckBox(ui->widget_validfloors);
        QRect tmpRect = QRect(tmpWidth*column+20, tmpHeight*row, tmpWidth, tmpHeight);
        tmpBox->setGeometry(tmpRect);
        tmpBox->setText(QString(GBKToUTF8("%1层")).arg(tmpLayer));
        tmpBox->show();
        mUserOpenFlrs.insert(tmpLayer,tmpBox);
    }
}

void ICManager::on_combox_user_layer_num_currentIndexChanged(const QString &arg1)
{
    //room初始化
    QStringList tmpRoomList;

    QString tmpLayer = GetFlrNum(arg1);

    QSqlQuery tmpRoomQuery = mSqlParser->Exec(QString("select * from addr_familys where (layer_num=\"%1\")and(unit_num=\"%2\")").arg(tmpLayer).arg(ui->combox_user_unit_num->currentText()));
    while(tmpRoomQuery.next())
    {
        QSqlRecord tmpRecord = tmpRoomQuery.record();
        QString tmpRoom = tmpRecord.value("family_num").toString();
        tmpRoomList.append(tmpRoom);
    }
    ui->combox_user_family_num->clear();
    ui->combox_user_family_num->addItems(tmpRoomList);
}

void ICManager::on_btn_user_recharge_clicked()
{
    int tmpRechargeFee = ui->combox_recharge_value->currentText().toInt();
    int tmpFreeFee = ui->lineEdit_freefee->text().toInt();
    int tmpTotalFee = tmpRechargeFee + tmpFreeFee;
    ui->lineEdit_freefee->setText(QString("%1").arg(tmpTotalFee));
}

void ICManager::on_btn_admin_list_clicked()
{
    GotoPage(E_PAGE_ADMIN);
}

void ICManager::on_btn_manager_clicked()
{
    ManagerDialog  tmpManagerDialog(this);
    int ret = tmpManagerDialog.exec();
    if(ret == QDialog::Accepted)
    {

    }
}

void ICManager::on_btn_quit_clicked()
{
    qApp->quit();
}


void ICManager::on_btn_readcard_selectowner_clicked()
{
    mReadCardFlag = 1;
    GotoPage(E_PAGE_USER);
}


void ICManager::on_table_user_cellDoubleClicked(int row, int column)
{
    QTableWidgetItem *tmpItem = ui->table_user->item(row,0);
    if(mReadCardFlag)
    {
        mReadCardFlag = 0;
        GotoPage(E_PAGE_MAKETOOLCARD);
        QString tmpName = tmpItem->text();
        ui->line_readcard_selectowner->setText(tmpName);
        return;
    }
    else
    {
        mReadCardFlag = 1;
        GotoPage(E_PAGE_MAKEOWERCARD);
        ui->line_business_name->setText(tmpItem->text());
        return;
    }
}

void ICManager::on_combo_cardfee_type_currentTextChanged(const QString &arg1)
{
    if(&arg1 == QString(GBKToUTF8("不收费")))
    {
        ui->widget_chargestandard->setDisabled(true);
        ui->widget_card_value->setDisabled(true);
    }
    else
    {
        ui->widget_chargestandard->setDisabled(false);
        ui->widget_card_value->setDisabled(false);
    }
}


void ICManager::on_checkBox_available_time_clicked(bool checked)
{
    if(!checked)
    {
        ui->widget_available_time->setDisabled(true);
    }
    else
    {
        ui->widget_available_time->setDisabled(false);
    }
}

void ICManager::on_table_admin_cellDoubleClicked(int row, int column)
{
    QTableWidgetItem *tmpItem = ui->table_admin->item(row,0);
    GotoPage(E_PAGE_MAKEADMINCARD);
    ui->line_makeadmin_name->setText(tmpItem->text());
}

void ICManager::on_checkBox_admin_cardValidtime_clicked(bool checked) //管理员发卡界面“卡片有效期”的checkbox
{
    if(checked)
    {
       ui->widget_admin_cardvalidtime->setDisabled(false);
    }
    else
    {
       ui->widget_admin_cardvalidtime->setDisabled(true);
    }

}

void ICManager::on_checkBox_admin_availabletime_clicked(bool checked)
{
    if(checked)
    {
       ui->widget_admin_availabletime->setDisabled(false);
    }
    else
    {
       ui->widget_admin_availabletime->setDisabled(true);
    }

}

//void ICManager::on_btn_owner_replacecard_clicked()
//{
//    GotoPage(E_PAGE_MAKEOWERCARD);
//}

//void ICManager::on_btn_tools_replacecard_clicked()
//{
//    GotoPage(E_PAGE_MAKETOOLCARD);
//}

void ICManager::on_checkBox_time1_clicked(bool checked)
{
    if(checked)
    {
        ui->widget_time1->setDisabled(false);
    }
    else
    {
        ui->widget_time1->setDisabled(true);
    }
}

void ICManager::on_checkBox_time2_clicked(bool checked)
{
    if(checked)
    {
        ui->widget_time2->setDisabled(false);
    }
    else
    {
        ui->widget_time2->setDisabled(true);
    }

}

void ICManager::on_checkBox_time3_clicked(bool checked)
{
    if(checked)
    {
        ui->widget_time3->setDisabled(false);
    }
    else
    {
        ui->widget_time3->setDisabled(true);
    }
}

void ICManager::on_combo_unitnum_currentIndexChanged(const QString &arg1)
{
    ui->table_map->setRowCount(0);
    //ui->table_map->clear();
    mLayersList.clear();

    QSqlQuery tmpCellLayersQuery = mSqlParser->Exec(QString("select * from addr_layers where unit_num=\"%1\"").arg(arg1));
    while(tmpCellLayersQuery.next())
    {
        QSqlRecord tmpCellLayersRecord = tmpCellLayersQuery.record();
        QString tmpLayersNum = tmpCellLayersRecord.value("layer_num").toString();
        QString tmpLayersMap = tmpCellLayersRecord.value("layer_map").toString();

        /*QString tmpKey = QString("%1&%2").arg(arg1).arg(tmpLayersNum);
        mLayersList.append(tmpKey);*/

        QSqlQuery tmpfamilyQuery = mSqlParser->Exec(QString("select * from addr_familys where unit_num=\"%1\" and layer_num=\"%2\"").arg(arg1).arg(tmpLayersNum));
        QStringList tmpFamilyList;
        while(tmpfamilyQuery.next())
        {
            QString tmpFamilyNum = tmpfamilyQuery.record().value("family_num").toString();
            tmpFamilyList.append(tmpFamilyNum);
        }
        AddMapTable(tmpLayersNum,tmpLayersMap,QString(tmpFamilyList.join(",")));
    }
}

void ICManager::AddMapTable(QString pLayersNum,QString pLayersMap,QString pFamilyNum)
{
    int row = ui->table_map->rowCount();
    ui->table_map->insertRow(row);

    QTableWidgetItem *tmpItem0 = new QTableWidgetItem;
    tmpItem0->setText(pLayersNum);
    tmpItem0->setFlags(tmpItem0->flags()&~(Qt::ItemIsEditable));
    tmpItem0->setTextAlignment(Qt::AlignCenter);
    ui->table_map->setItem(row,0,tmpItem0);

    QTableWidgetItem *tmpItem1 = new QTableWidgetItem;
    tmpItem1->setText(pLayersMap);
    tmpItem1->setFlags(tmpItem1->flags()&~(Qt::ItemIsEditable));
    tmpItem1->setTextAlignment(Qt::AlignCenter);
    ui->table_map->setItem(row,1,tmpItem1);

    QTableWidgetItem *tmpItem2 = new QTableWidgetItem;
    tmpItem2->setText(pFamilyNum);
    tmpItem2->setFlags(tmpItem2->flags()&~(Qt::ItemIsEditable));
    tmpItem2->setTextAlignment(Qt::AlignCenter);
    ui->table_map->setItem(row,2,tmpItem2);

    QString tmpUnit = ui->combo_unitnum->currentText();
    QString tmpKey = QString("%1&%2").arg(tmpUnit).arg(pLayersNum);
    mLayersList.append(tmpKey);

}

void ICManager::on_table_map_cellClicked(int row, int column)
{
   ui->line_actualLayer->setText(ui->table_map->item(row,0)->text());
   ui->line_mapLayer->setText(ui->table_map->item(row,1)->text());
   ui->line_roomlist->setText(ui->table_map->item(row,2)->text());
}

void ICManager::on_btn_elevt_ok_clicked()
{
    QString pUnitNum = ui->combo_unitnum->currentText();
    QString pElevNum = ui->line_elevnum->text();
    QString pCtrlLayerNum = ui->line_ctrl_layernum->text();
    AddElevatorToDB(pElevNum, pUnitNum,pCtrlLayerNum);
}

void ICManager::AddMapFloorToDB(QString pUnit,QString pActualLayer,QString pMapLayer,QString pRoomList)
{
    QString tmpKey = QString("%1&%2").arg(pUnit).arg(pActualLayer);

    int index = mLayersList.indexOf(tmpKey);
    if(index >= 0)
    {
        QMessageBox::StandardButton tmpBtn = QMessageBox::question(0,GBKToUTF8("更新询问"),
                                                                   GBKToUTF8("是否更改楼层映射"),
                                                                   QMessageBox::Ok|QMessageBox::Cancel);
        if(tmpBtn != QMessageBox::Ok)
        {
            return;
        }
    }

    QList<QVariant> tmpFieldVaules;
    tmpFieldVaules << pUnit << pActualLayer << pMapLayer ;

    if(!mSqlParser->ModifyRow("addr_layers", QString("unit_num='%1' and layer_num='%2'").arg(pUnit).arg(pActualLayer), tmpFieldVaules))
    {
        QMessageBox::critical(0, GBKToUTF8("添加楼层映射错误"),
                              GBKToUTF8("无法添加楼层映射，请检查填写是否正确"));
        return;
    }

    QStringList tmpRoomList = pRoomList.split(",", QString::SkipEmptyParts);
    QStringList tmpDelList;

    QSqlQuery tmpfamilyQuery = mSqlParser->Exec(QString("select family_num from addr_familys where unit_num=\"%1\" and layer_num=\"%2\"").arg(pUnit).arg(pActualLayer));
    while(tmpfamilyQuery.next())
    {
        QString tmpFamilyNum = tmpfamilyQuery.record().value("family_num").toString(); //原有的room号
        if(tmpRoomList.contains(tmpFamilyNum))
        {
            tmpRoomList.removeOne(tmpFamilyNum);
        }
        else
        {
            tmpDelList.append(tmpFamilyNum);
        }
    }

    for(int i=0;i<tmpRoomList.count();i++)
    {
        QString tmpRoom = tmpRoomList.at(i);
        tmpFieldVaules.clear();
        tmpFieldVaules << pUnit << pActualLayer << tmpRoom ;
        if(!mSqlParser->AddRow("addr_familys", tmpFieldVaules))
        {
            QMessageBox::critical(0, GBKToUTF8("添加楼层房间错误"),
                                  GBKToUTF8("无法添加楼层房间，请检查填写是否正确"));
            return;
        }
    }

    for(int i=0;i<tmpDelList.count();i++)
    {
        QString tmpRoom = tmpDelList.at(i);

        QString tmpFilter = QString("unit_num='%1' and layer_num='%2' and family_num='%3'").arg(pUnit).arg(pActualLayer).arg(tmpRoom);
        if(!mSqlParser->DelRow("addr_familys", tmpFilter))
        {
            QMessageBox::critical(0, GBKToUTF8("删除楼层房间错误"),
                                  GBKToUTF8("无法删除楼层房间，请检查填写是否正确"));
            return;
        }
    }

    if(index >= 0)
    {
        ui->table_map->item(index,1)->setText(pMapLayer);
        ui->table_map->item(index,2)->setText(pRoomList);
    }
    else
    {
        AddMapTable(pActualLayer,pMapLayer,pRoomList);
    }
}


void ICManager::on_btn_addliftmap_clicked()
{
    QString tmpActualLayer = ui->line_actualLayer->text();
    QString tmpMapLayer = ui->line_mapLayer->text();
    QString tmpRoomList = ui->line_roomlist->text();
    QString tmpUnit = ui->combo_unitnum->currentText();

    AddMapFloorToDB(tmpUnit,tmpActualLayer,tmpMapLayer,tmpRoomList);
}

void ICManager::on_btn_delliftmap_clicked()
{
    int row  = ui->table_map->currentRow();

    QString tmpUnit = ui->combo_unitnum->currentText();

    QTableWidgetItem * tmpItem = ui->table_map->item(row, 0);
    QString tmpLayer = tmpItem->text();

    QString tmpKey = QString("%1&%2").arg(tmpUnit).arg(tmpLayer);

    ui->table_map->removeRow(row);

    mLayersList.removeOne(tmpKey);
   /* int index =  mLayersList.indexOf(tmpKey);
    if(index>=0)
         mLayersList = mLayersList.mid(0,index) << mLayersList.mid(index+1);*/

    if(!mSqlParser->DelRow("addr_layers", QString("unit_num='%1' and layer_num='%2'").arg(tmpUnit).arg(tmpLayer)))
    {
        QMessageBox::critical(0, GBKToUTF8("删除楼层映射错误"),
                              GBKToUTF8("无法删除楼层映射，请检查填写是否正确"));
        return;
    }
}

void ICManager::on_btn_cell_del_clicked()
{
    QMessageBox::StandardButton tmpret = QMessageBox::question(0, GBKToUTF8("删除房间"),
                                                                  GBKToUTF8("是否删除房间"),
                                                               QMessageBox::Ok | QMessageBox::Cancel);
    if(tmpret != QMessageBox::Ok)
    {
        return;
    }

    int row = ui->table_cellinfo->currentRow();
    QString tmpUnit = ui->table_cellinfo->item(row, 0)->text();
    QString tmpLayer = ui->table_cellinfo->item(row, 1)->text();

    //获取此映射对应的实际楼层
    QSqlQuery tmpQuery = mSqlParser->Exec(QString("select layer_num from addr_layers where unit_num=\"%1\" and layer_map=\"%2\"").arg(tmpUnit).arg(tmpLayer));
    if(tmpQuery.next())
    {
        QSqlRecord tmpRecord = tmpQuery.record();
        tmpLayer = tmpRecord.value("layer_num").toString();
    }

    QString tmpRoom = ui->table_cellinfo->item(row, 2)->text();

    if(!mSqlParser->DelRow("addr_familys", QString("unit_num=\"%1\" and layer_num=\"%2\" and family_num=\"%3\"").arg(tmpUnit).arg(tmpLayer).arg(tmpRoom)))
    {
        QMessageBox::critical(0, GBKToUTF8("删除房间错误"),
                              GBKToUTF8("无法删除此房间，请检查填写是否正确"));
        return;
    }

    ui->table_cellinfo->removeRow(row);
}

void ICManager::on_btn_dellift_clicked()
{
    QMessageBox::StandardButton tmpret = QMessageBox::question(0, GBKToUTF8("删除电梯"),
                                                                  GBKToUTF8("是否删除电梯"),
                                                               QMessageBox::Ok | QMessageBox::Cancel);
    if(tmpret != QMessageBox::Ok)
    {
        return;
    }

    int row = ui->table_elevator->currentRow();
    QString tmpElevtNum = ui->table_elevator->item(row,0)->text();
    QString tmpUnit = ui->table_elevator->item(row,1)->text();
    QString tmpCtrl = ui->table_elevator->item(row,2)->text();

    if(!mSqlParser->DelRow("elevator", QString("elev_num=\"%1\" and unit_num=\"%2\" and ctrlayer_cnt=\"%3\"").arg(tmpElevtNum).arg(tmpUnit).arg(tmpCtrl)))
    {
        QMessageBox::critical(0, GBKToUTF8("删除电梯错误"),
                              GBKToUTF8("无法删除此电梯，请检查填写是否正确"));
        return;
    }

    ui->table_elevator->removeRow(row);
    int index = mElevatorList.indexOf(QString("%1&%2").arg(tmpElevtNum).arg(tmpUnit));//要被删除的那行的位置
    mElevatorList = mElevatorList.mid(0,index) << mElevatorList.mid(index+1); //取出前面和后面的容器合在一起
    //ui->combo_unitnum->clear();
    ui->line_elevnum->clear();
    ui->line_ctrl_layernum->clear();
}


//挂失卡操作
void ICManager::on_btn_elector_list_clicked()
{
    GotoPage(E_PAGE_ELEVATOR);
}

void ICManager::on_btn_card_list_clicked()
{
   mLossCardFlag = 2;
   GotoPage(E_PAGE_CARDLIST);
}

void ICManager::on_btn_cardlist_cancel_4_clicked()
{

}

void ICManager::on_table_elevator_cellDoubleClicked(int row, int column)
{
   QTableWidgetItem *tmpItem = ui->table_elevator->item(row,0);
   GotoPage(E_PAGE_LOSSCARD);
   ui->lineEdit_Elevator_number->setText(tmpItem->text());
   return ;
}

void ICManager::on_table_cardlist_cellDoubleClicked(int row, int column)
{
    if(mLossCardFlag == 2)
    {
        QTableWidgetItem *tmpItem = ui->table_cardlist->item(row,0);
        GotoPage(E_PAGE_LOSSCARD);
        ui->lineEdit_card_number->setText(tmpItem->text());
        return ;
    }
    if(mLossCardFlag == 1)
    {
        QTableWidgetItem *tmpItem = ui->table_cardlist->item(row,0);
        GotoPage(E_PAGE_MAKETOOLCARD);
        ui->line_LostCardNum->setText(tmpItem->text());
        return ;
    }

}

void ICManager::on_btn_cardlist_ok_4_clicked()
{
    QString tmpLossCardnumber = ui->line_cardloss_cardnum->text();
    QString tmpLosspassword = ui->line_cardloss_password->text();
    AddLostCardToDB(tmpLossCardnumber,tmpLosspassword);

    QString tmpUserCardNum = ui->lineEdit_card_number->text();
    QSqlQuery tmpQuery = mSqlParser->Exec(QString("select * from card_list where card_num=\"%1\"")
                                        .arg(tmpUserCardNum));
    if(tmpQuery.next())
    {
        QSqlRecord tmpRecord = tmpQuery.record();
        QString pName = tmpRecord.value("name").toString();
        int pType = tmpRecord.value("card_type").toInt();
        QList<QVariant> tmpFieldVaules;
        tmpFieldVaules.clear();
        tmpFieldVaules << tmpUserCardNum << pType << pName << "" <<GBKToUTF8("已挂失") << "" << "";  //界面参数在更改情况下，此条才会加到数据库中，内容改为此条信息（界面中的参数）
        if(!mSqlParser->ModifyRow("card_list","card_num",tmpUserCardNum,tmpFieldVaules))
        {
            QMessageBox::critical(0,GBKToUTF8("添加卡信息错误"),
                                  GBKToUTF8("无法添加此卡，请检查填写是否正确"));
            return;
        }
    }



}

void ICManager::on_btn_owner_replacecard_clicked()
{
    GotoPage(E_PAGE_MAKEOWERCARD);
}

void ICManager::on_btn_tools_replacecard_clicked()
{
    GotoPage(E_PAGE_MAKETOOLCARD);
}


void ICManager::on_btn_GetLostCardNum_clicked()
{
    mLossCardFlag = 1;
    GotoPage(E_PAGE_CARDLIST);
}



void ICManager::on_combox_floor_num_currentIndexChanged(const QString &arg1)
{
    QList<QString> tmpFlrNumList = mUserOpenFlrs.keys();
    for(int i=0;i<tmpFlrNumList.count();i++)
    {
        QString tmpFlrKey = tmpFlrNumList.at(i);
        QCheckBox *tmpBtn = mUserOpenFlrs.value(tmpFlrKey);
        tmpBtn->setChecked(false);
    }
}


