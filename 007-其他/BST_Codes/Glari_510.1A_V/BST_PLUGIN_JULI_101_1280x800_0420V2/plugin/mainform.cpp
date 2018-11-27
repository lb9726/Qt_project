#include "mainform.h"
#include "ui_mainform.h"
#include <QDebug>

MainForm::MainForm(QWidget *parent) :
    PluginWidget(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);
}

MainForm::~MainForm()
{
    delete ui;
}

void MainForm::InitUI(EventMap *pEventMap, QString pRcPath)
{
    #ifdef X86
        m_SerialPort = new SerialPort(GetSerialPort("3"), BAUD9600);
    #else
        m_SerialPort = new SerialPort(GetSerialPort("1"), BAUD9600);
    #endif
        m_SerialPort->setParity(PAR_NONE);

        SetRcPath(pRcPath);
        if(!pRcPath.isEmpty())
        {
            ui->MainWidget->InitRC(pRcPath + QString("/Desktop/"), true);

            ui->Arrow->InitRC(pRcPath + QString("/Arrow/"), false);
            EffectRoll *tmpArrowEffect = new EffectRoll;
            tmpArrowEffect->setEffect(1000, 0, Vertical, Negative);
            tmpArrowEffect->render(ui->Arrow, OPERATE_RUN);
            mArrow_bk = ArrowUpDownhalt;
            connect(this,SIGNAL(sArrow(AREA_OPERATE,QVariant)), ui->Arrow, SLOT(slot_ExecOperate(AREA_OPERATE,QVariant)));

            ui->Floor->InitFloor(0,ui->ten,ui->single);
            ui->Floor->InitRC(pRcPath + QString("/Floor/"), false);
            mFloor_bk = FLOOR_INFO((char)0x7f, (char)0x7f, (char)0x7f);
            connect(this,SIGNAL(sFloor(AREA_OPERATE,QVariant)), ui->Floor, SLOT(slot_ExecOperate(AREA_OPERATE,QVariant)));



            ui->Media->InitRC(pRcPath + "/Media/", false, 90);
            ui->Media->m_MediaPlayer->SetVolume(100);
            ui->Media->SetPlayMode(0);
            //ui->Media->slot_ExecOperate(OPERATE_CHANGE,1);

      //      ui->label->setPixmap(pRcPath + QString("logo/0.png"));

            ui->Function->InitRC(pRcPath + QString("Function/"), false);
            EffectZoom *tmpFunctionEffect = new EffectZoom;
            tmpFunctionEffect->setEffect(600, 1, Vertical, Negative);
            tmpFunctionEffect->render(ui->Function, OPERATE_CHANGE);
            connect(this, SIGNAL(sFunction(AREA_OPERATE,QVariant)), ui->Function, SLOT(slot_ExecOperate(AREA_OPERATE,QVariant)), Qt::QueuedConnection);

            mFlrBtnList.insert(4, ui->Button_1);
            ui->Button_1->InitRC(QString(pRcPath + QString("/Button/Button_1/")));
            mFlrBtnList.insert(5, ui->Button_2);
            ui->Button_2->InitRC(QString(pRcPath + QString("/Button/Button_2/")));
//            mFlrBtnList.insert(3, ui->Button_3);
//            ui->Button_3->InitRC(QString(pRcPath + QString("/Button/Button_3/")));
//            mFlrBtnList.insert(4, ui->Button_4);
//            ui->Button_4->InitRC(QString(pRcPath + QString("/Button/Button_4/")));


            mFlrBtnList.insert(45, ui->Button_open);
            ui->Button_open->InitRC(QString(pRcPath + QString("/Button/Open/")));
            mFlrBtnList.insert(46, ui->Button_close);
            ui->Button_close->InitRC(QString(pRcPath + QString("/Button/Close/")));


            }


       Init(false,1,2);


       QList<int> tmpList = ui->Media->m_FileList.keys();
           if(!tmpList.isEmpty())
           {
               ui->Media->slot_ExecOperate(OPERATE_CHANGE,tmpList.first());
           }
}

void MainForm::slot_ImageChange()
{

}

void MainForm::slot_DateTime(AREA_OPERATE pOperate, QVariant pPara)
{
    QDateTime tmpDateTime = QDateTime::currentDateTime();
    QString tmpTime = tmpDateTime.time().toString("hh:mm");
    QString tmpDate = tmpDateTime.date().toString("yyyy-MM-dd");
    ui->time->setText(tmpTime);
    ui->date->setText(tmpDate);
}

void MainForm::slot_Tick()
{
    if(m_Ticks%60 == 0)
    {
        emit sDateTime(OPERATE_CHANGE, QDateTime::currentDateTime());
    }
//    if(m_Ticks%5== 1)
//    {
//        emit sFunction(OPERATE_CHANGE, NEXTRC);
//    }
    PluginWidget::slot_Tick();
}

void MainForm::ParseFloor(int pType)
{
    FLOOR_INFO tmpFloor(mCurFrame[pType].at(2), mCurFrame[pType].at(3), mCurFrame[pType].at(4));
    if(mFloor_bk != tmpFloor)
    {
        mFloor_bk = tmpFloor;
        ui->Floor->slot_ExecOperate(OPERATE_CHANGE, tmpFloor.toInt());
    }

    ARROWSTATE tmpArrow = (ARROWSTATE)((mCurFrame[pType].at(5)) & D_ArrowArea);
    if(mArrow_bk != tmpArrow)
    {
        ui->Arrow->slot_ExecOperate(OPERATE_CHANGE, tmpArrow);
        mArrow_bk = tmpArrow;
    }

    quint8 tmpFuction = (ARROWSTATE)((mCurFrame[pType].at(6)) & 0x7f);
    if(tmpFuction == 0x01)
    {
        ui->Function->slot_ExecOperate(OPERATE_CHANGE, 1);
//        emit sFunction(OPERATE_CHANGE, 0);
    }
    else if(tmpFuction == 0x02)
    {
        ui->Function->slot_ExecOperate(OPERATE_CHANGE, 2);
    }
    else if(tmpFuction == 0x04)
    {
        ui->Function->slot_ExecOperate(OPERATE_CHANGE, 3);
    }
    else if(tmpFuction == 0x08)
    {
        ui->Function->slot_ExecOperate(OPERATE_CHANGE, 4);
    }
    else if(tmpFuction == 0x10)
    {
        ui->Function->slot_ExecOperate(OPERATE_CHANGE, 5);
    }
    else
        ui->Function->slot_ExecOperate(OPERATE_CHANGE, 0);
}

void MainForm::ExecOperate(AREA_OPERATE pOperate, QVariant pPara)
{
}

Q_EXPORT_PLUGIN2(mainform, MainForm)
