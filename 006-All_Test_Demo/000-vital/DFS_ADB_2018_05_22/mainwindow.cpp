#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <linux/watchdog.h>
//#include <sys/ioctl.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);
    ui->label_echo->clear();

    mPort = new SerialPort;
    mPort->Init(1, 9600);
    connect(mPort, SIGNAL(readyRead()), this, SLOT(slot_Read()));

    timer1 = 0;
    timer2 = 0;
    timer3 = 0;

    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_CallEleTimeout()));
    timer->setSingleShot(true);

    timer1 = new QTimer;
    connect(timer1, SIGNAL(timeout()), this, SLOT(slot_AutoCall()));
    timer1->setSingleShot(true);

    timer2 = new QTimer;
    connect(timer2, SIGNAL(timeout()), this, SLOT(slot_inputTips()));
    timer2->setInterval(2000);
    timer2->setSingleShot(true);

    timer3 = new QTimer;
    connect(timer3, SIGNAL(timeout()), this, SLOT(slot_ClearOldArray()));
    timer3->setSingleShot(true);

    timer4 = new QTimer;
    connect(timer4, SIGNAL(timeout()), this, SLOT(slot_GotoPage())); //长按5s 0
    timer4->setSingleShot(true);

    timer5 = new QTimer;
    connect(timer5, SIGNAL(timeout()), this, SLOT(slot_BackHome()));
    timer5->setSingleShot(true);

//    QList<QPushButton*> tmpBtnList = findChildren<QPushButton*>();
//    foreach (QPushButton* tmpBtn, tmpBtnList) {
//        tmpBtn->setFocusPolicy(Qt::NoFocus);
//    }

    connect(&mMapper, SIGNAL(mapped(int)), this, SLOT(slot_BtnClicked(int)));

    connect(ui->btn_0, SIGNAL(pressed()), &mMapper, SLOT(map()));
    mMapper.setMapping(ui->btn_0, 0);

    connect(ui->btn_1, SIGNAL(pressed()), &mMapper, SLOT(map()));
    mMapper.setMapping(ui->btn_1, 1);

    connect(ui->btn_2, SIGNAL(pressed()), &mMapper, SLOT(map()));
    mMapper.setMapping(ui->btn_2, 2);

    connect(ui->btn_3, SIGNAL(pressed()), &mMapper, SLOT(map()));
    mMapper.setMapping(ui->btn_3, 3);

    connect(ui->btn_4, SIGNAL(pressed()), &mMapper, SLOT(map()));
    mMapper.setMapping(ui->btn_4, 4);

    connect(ui->btn_5, SIGNAL(pressed()), &mMapper, SLOT(map()));
    mMapper.setMapping(ui->btn_5, 5);

    connect(ui->btn_6, SIGNAL(pressed()), &mMapper, SLOT(map()));
    mMapper.setMapping(ui->btn_6, 6);

    connect(ui->btn_7, SIGNAL(pressed()), &mMapper, SLOT(map()));
    mMapper.setMapping(ui->btn_7, 7);

    connect(ui->btn_8, SIGNAL(pressed()), &mMapper, SLOT(map()));
    mMapper.setMapping(ui->btn_8, 8);

    connect(ui->btn_9, SIGNAL(pressed()), &mMapper, SLOT(map()));
    mMapper.setMapping(ui->btn_9, 9);

    connect(ui->btn_enter, SIGNAL(pressed()), &mMapper, SLOT(map()));
    mMapper.setMapping(ui->btn_enter, 13);

    connect(ui->btn_del, SIGNAL(pressed()), &mMapper, SLOT(map()));
    mMapper.setMapping(ui->btn_del, 127);


    connect(&mMapperPassWd, SIGNAL(mapped(int)), this, SLOT(slot_BtnPassWdClicked(int)));

    connect(ui->btn_pass0, SIGNAL(pressed()), &mMapperPassWd, SLOT(map()));
    mMapperPassWd.setMapping(ui->btn_pass0, 0);

    connect(ui->btn_pass1, SIGNAL(pressed()), &mMapperPassWd, SLOT(map()));
    mMapperPassWd.setMapping(ui->btn_pass1, 1);

    connect(ui->btn_pass2, SIGNAL(pressed()), &mMapperPassWd, SLOT(map()));
    mMapperPassWd.setMapping(ui->btn_pass2, 2);

    connect(ui->btn_pass3, SIGNAL(pressed()), &mMapperPassWd, SLOT(map()));
    mMapperPassWd.setMapping(ui->btn_pass3, 3);

    connect(ui->btn_pass4, SIGNAL(pressed()), &mMapperPassWd, SLOT(map()));
    mMapperPassWd.setMapping(ui->btn_pass4, 4);

    connect(ui->btn_pass5, SIGNAL(pressed()), &mMapperPassWd, SLOT(map()));
    mMapperPassWd.setMapping(ui->btn_pass5, 5);

    connect(ui->btn_pass6, SIGNAL(pressed()), &mMapperPassWd, SLOT(map()));
    mMapperPassWd.setMapping(ui->btn_pass6, 6);

    connect(ui->btn_pass7, SIGNAL(pressed()), &mMapperPassWd, SLOT(map()));
    mMapperPassWd.setMapping(ui->btn_pass7, 7);

    connect(ui->btn_pass8, SIGNAL(pressed()), &mMapperPassWd, SLOT(map()));
    mMapperPassWd.setMapping(ui->btn_pass8, 8);

    connect(ui->btn_pass9, SIGNAL(pressed()), &mMapperPassWd, SLOT(map()));
    mMapperPassWd.setMapping(ui->btn_pass9, 9);

    connect(ui->btn_passEnter, SIGNAL(pressed()), &mMapperPassWd, SLOT(map()));
    mMapperPassWd.setMapping(ui->btn_passEnter, 13);

    connect(ui->btn_passdel, SIGNAL(pressed()), &mMapperPassWd, SLOT(map()));
    mMapperPassWd.setMapping(ui->btn_passdel, 127);

    ui->stackedWidget_tips->setCurrentIndex(3);
    ui->label_inputDF->setText(QString("Input destination floor")); //请输入目的楼层

    connect(&mMenuGroup, SIGNAL(buttonClicked(int)), this, SLOT(slot_MenuPage(int)));
    mMenuGroup.addButton(ui->btn_arrmapping, 0);
    mMenuGroup.addButton(ui->btn_flrmapping, 1);

    mSetFile = qApp->applicationDirPath() + "/Setting.ini";
    QSettings tmpSetting(mSetFile, QSettings::IniFormat);
    tmpSetting.setIniCodec("UTF-8");

    tmpSetting.beginGroup("KeyMapping");
    QString tmpEnterMapPic = tmpSetting.value("Enter").toString();
    QString tmpDelMapPic = tmpSetting.value("Del").toString();
    QString tmp4MapPic = tmpSetting.value("key4").toString();
    tmpSetting.endGroup();

    tmpSetting.beginGroup("DirArrMapping");
    QString tmpAMapPic = tmpSetting.value("A").toString();
    QString tmpBMapPic = tmpSetting.value("B").toString();
    QString tmpCMapPic = tmpSetting.value("C").toString();
    QString tmpDMapPic = tmpSetting.value("D").toString();
    tmpSetting.endGroup();

    QStringList tmpDirList = QStringList() << ":/rc/dir_left.png" << ":/rc/dir_right.png";
    int tmpAIndex = tmpDirList.indexOf(tmpAMapPic);
    int tmpBIndex = tmpDirList.indexOf(tmpBMapPic);
    int tmpCIndex = tmpDirList.indexOf(tmpCMapPic);
    int tmpDIndex = tmpDirList.indexOf(tmpDMapPic);

    ui->widget_arrow_A->SetName(QString("A Lift"));
    ui->widget_arrow_A->InitList(tmpAIndex, tmpDirList);

    ui->widget_arrow_B->SetName(QString("B Lift"));
    ui->widget_arrow_B->InitList(tmpBIndex, tmpDirList);

    ui->widget_arrow_C->SetName(QString("C Lift"));
    ui->widget_arrow_C->InitList(tmpCIndex, tmpDirList);

    ui->widget_arrow_D->SetName(QString("D Lift"));
    ui->widget_arrow_D->InitList(tmpDIndex, tmpDirList);

    connect(ui->widget_arrow_A, SIGNAL(sSelected()), this, SLOT(slot_ArrowSelected()));
    connect(ui->widget_arrow_B, SIGNAL(sSelected()), this, SLOT(slot_ArrowSelected()));
    connect(ui->widget_arrow_C, SIGNAL(sSelected()), this, SLOT(slot_ArrowSelected()));
    connect(ui->widget_arrow_D, SIGNAL(sSelected()), this, SLOT(slot_ArrowSelected()));

    connect(ui->widget_key_enter, SIGNAL(sSelected()), this, SLOT(slot_KeySelected()));
    connect(ui->widget_key_del, SIGNAL(sSelected()), this, SLOT(slot_KeySelected()));
    connect(ui->widget_key_4, SIGNAL(sSelected()), this, SLOT(slot_KeySelected()));

    ui->widget_key_enter->SetName(GBKToUTF8("Enter Key"));
    mEnterList = QStringList()
                            << ":/rc/btn_enter.png"
                            << ":/rc/btn_delete.png"
                            << ":/rc/btn_A.png"
                            << ":/rc/btn_B.png";

    ui->widget_key_enter->InitList(0, mEnterList);

    int EnterIndex = mEnterList.indexOf(tmpEnterMapPic);
    if(EnterIndex > 0)
    {
         ui->widget_key_enter->InitList(EnterIndex, mEnterList);
    }


    ui->widget_key_del->SetName(QString("Delete Key"));
    mDelList = QStringList()
                            << ":/rc/btn_delete.png"
                            << ":/rc/btn_minus.png"
                            << ":/rc/btn_A.png"
                            << ":/rc/btn_B.png";
    ui->widget_key_del->InitList(0, mDelList); //后台设置初始化

    int DelIndex = mDelList.indexOf(tmpDelMapPic);
    if(DelIndex > 0)  //如果为0的话，就不重复设置了
    {
         ui->widget_key_del->InitList(DelIndex, mDelList);
    }

    ui->widget_key_4->SetName(GBKToUTF8("4 Key"));
    m4KeyList = QStringList() << ":/rc/btn_4.png"
                              << ":/rc/btn_A.png"
                              << ":/rc/btn_B.png";
    ui->widget_key_4->InitList(0, m4KeyList);

    int tmp4KeyIndex = m4KeyList.indexOf(tmp4MapPic);
    if(tmp4KeyIndex > 0)
    {
        ui->widget_key_4->InitList(tmp4KeyIndex, m4KeyList);
    }

    {
        QString tmpPic = tmpEnterMapPic;
        int index = tmpEnterMapPic.indexOf(".png");
        if(index > 0)
        {
            QString tmpPicLight = tmpEnterMapPic.insert(index, "_light");
            ui->btn_enter->setStyleSheet(QString("QPushButton::pressed{border-image:url(%1);background-color: rgba(200, 200, 200, 50);border-radius:10;}"
                                                 "QPushButton{border-image:url(%2);}").arg(tmpPicLight)
                                                                                      .arg(tmpPic));
        }
    }

    {
        QString tmpPic = tmpDelMapPic;
        int index = tmpDelMapPic.indexOf(".png");
        if(index > 0)
        {
            QString tmpPicLight = tmpDelMapPic.insert(index, "_light");
            ui->btn_del->setStyleSheet(QString("QPushButton::pressed{border-image:url(%1);background-color: rgba(200, 200, 200, 50);border-radius:10;}"
                                                 "QPushButton{border-image:url(%2);}").arg(tmpPicLight)
                                                                                      .arg(tmpPic));
        }
    }

    {
        QString tmpPic = tmp4MapPic;
        int index = tmp4MapPic.indexOf(".png");
        if(index > 0)
        {
            QString tmpPicLight = tmp4MapPic.insert(index, "_light");
            ui->btn_4->setStyleSheet(QString("QPushButton::pressed{border-image:url(%1);background-color: rgba(200, 200, 200, 50);border-radius:10;}"
                                                 "QPushButton{border-image:url(%2);}").arg(tmpPicLight)
                                                                                      .arg(tmpPic));
        }
    }

    connect(ui->widget_key_enter, SIGNAL(sDataChanged(QString)), this, SLOT(slot_KeyChanged(QString)));
    connect(ui->widget_key_del, SIGNAL(sDataChanged(QString)), this, SLOT(slot_KeyChanged(QString)));
    connect(ui->widget_key_4, SIGNAL(sDataChanged(QString)), this, SLOT(slot_KeyChanged(QString)));

    connect(ui->widget_arrow_A, SIGNAL(sDataChanged(QString)), this, SLOT(slot_DirArrChanged(QString)));
    connect(ui->widget_arrow_B, SIGNAL(sDataChanged(QString)), this, SLOT(slot_DirArrChanged(QString)));
    connect(ui->widget_arrow_C, SIGNAL(sDataChanged(QString)), this, SLOT(slot_DirArrChanged(QString)));
    connect(ui->widget_arrow_D, SIGNAL(sDataChanged(QString)), this, SLOT(slot_DirArrChanged(QString)));

    ui->label_passwd->clear();
/*zzf
    mWDFile = new QFile;   //watchdog
    mWDFile->setFileName(QString("/dev/watchdog"));
    if(mWDFile->open(QFile::ReadWrite | QFile::Unbuffered))
    {
        int tmpSecond = 5;
        //zzf ioctl(mWDFile->handle(), WDIOC_SETTIMEOUT, tmpSecond);
    }
*/
    //zzf timer_Wd = new QTimer;   //watchdog
    //zzf timer_Wd->start(2500);
    //zzf connect(timer_Wd, SIGNAL(timeout()), this, SLOT(slot_wd()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_wd()    //watchdog
{
    if(mWDFile->isOpen() && mWDFile->handle())
    {
        //zzf ioctl(mWDFile->handle(), WDIOC_KEEPALIVE, 0);
    }
}

void MainWindow::slot_DirArrChanged(QString pDirArrPic) //方向箭头左右键一点击，值改变
{
    MenuItem * tmpItem = qobject_cast<MenuItem *>(sender());
    if(tmpItem)
    {
        if(tmpItem == ui->widget_arrow_A)  //如是A的方向箭头改变，则保存到ini中
        {
            QSettings tmpSetting(mSetFile, QSettings::IniFormat);
            tmpSetting.setIniCodec("UTF-8");
            tmpSetting.beginGroup("DirArrMapping");
            tmpSetting.setValue("A", pDirArrPic);
            tmpSetting.endGroup();
        }
        else if(tmpItem == ui->widget_arrow_B)
        {
            QSettings tmpSetting(mSetFile, QSettings::IniFormat);
            tmpSetting.setIniCodec("UTF-8");
            tmpSetting.beginGroup("DirArrMapping");
            tmpSetting.setValue("B", pDirArrPic);
            tmpSetting.endGroup();
        }
        else if(tmpItem == ui->widget_arrow_C)
        {
            QSettings tmpSetting(mSetFile, QSettings::IniFormat);
            tmpSetting.setIniCodec("UTF-8");
            tmpSetting.beginGroup("DirArrMapping");
            tmpSetting.setValue("C", pDirArrPic);
            tmpSetting.endGroup();
        }
        else if(tmpItem == ui->widget_arrow_D)
        {
            QSettings tmpSetting(mSetFile, QSettings::IniFormat);
            tmpSetting.setIniCodec("UTF-8");
            tmpSetting.beginGroup("DirArrMapping");
            tmpSetting.setValue("D", pDirArrPic);
            tmpSetting.endGroup();
        }
    }
}

void MainWindow::slot_KeyChanged(QString pPic)
{
    MenuItem* tmpItem = qobject_cast<MenuItem *>(sender());
    if(tmpItem)
    {
        if(tmpItem == ui->widget_key_enter)
        {
            QSettings tmpSetting(mSetFile, QSettings::IniFormat);
            tmpSetting.setIniCodec("UTF-8");
            tmpSetting.beginGroup("KeyMapping");
            tmpSetting.setValue("Enter", pPic);
            tmpSetting.endGroup();

            ui->btn_enter->setStyleSheet(QString("border-image:url(%1);").arg(pPic));
            {
                QString tmpPic = pPic;
                int index = pPic.indexOf(".png");
                if(index > 0)
                {
                    QString tmpPicLight = pPic.insert(index, "_light");
                    ui->btn_enter->setStyleSheet(QString("QPushButton::pressed{border-image:url(%1);background-color: rgba(200, 200, 200, 50);border-radius:10;}"
                                                         "QPushButton{border-image:url(%2);}").arg(tmpPicLight)
                                                                                              .arg(tmpPic));
                }
            }
        }
        else if(tmpItem == ui->widget_key_del)
        {
            QSettings tmpSetting(mSetFile, QSettings::IniFormat);
            tmpSetting.setIniCodec("UTF-8");
            tmpSetting.beginGroup("KeyMapping");
            tmpSetting.setValue("Del", pPic);
            tmpSetting.endGroup();
            //ui->btn_del->setStyleSheet(QString("border-image:url(%1);").arg(pPic));
            {
                QString tmpPic = pPic;
                int index = pPic.indexOf(".png");
                if(index > 0)
                {
                    QString tmpPicLight = pPic.insert(index, "_light");
                    ui->btn_del->setStyleSheet(QString("QPushButton::pressed{border-image:url(%1);background-color: rgba(200, 200, 200, 50);border-radius:10;}"
                                                         "QPushButton{border-image:url(%2);}").arg(tmpPicLight)
                                                                                              .arg(tmpPic));
                }
            }
        }
        else if(tmpItem == ui->widget_key_4)
        {
            QSettings tmpSetting(mSetFile, QSettings::IniFormat);
            tmpSetting.setIniCodec("UTF-8");
            tmpSetting.beginGroup("KeyMapping");
            tmpSetting.setValue("key4", pPic);
            tmpSetting.endGroup();
            //ui->btn_4->setStyleSheet(QString("border-image:url(%1);").arg(pPic));
            {
                QString tmpPic = pPic;
                int index = pPic.indexOf(".png");
                if(index > 0)
                {
                    QString tmpPicLight = pPic.insert(index, "_light");
                    ui->btn_4->setStyleSheet(QString("QPushButton::pressed{border-image:url(%1);background-color: rgba(200, 200, 200, 50);border-radius:10;}"
                                                         "QPushButton{border-image:url(%2);}").arg(tmpPicLight)
                                                                                              .arg(tmpPic));
                }
            }
        }
    }

}

void MainWindow::slot_BtnClicked(int id)
{
    qDebug() << id;

    if(ui->stackedWidget_tips->currentIndex() == 0)  //目的梯、箭头指向
    {
        return;
    }
    else if(ui->stackedWidget_tips->currentIndex() == 2)   //提示页 错误、派梯
    {
        ui->label_echo->clear();
        return;
    }

    if(id == 0)  //单按0，或长按5s 0
    {
        QString tmpEcho = ui->label_echo->text();
        if(tmpEcho.size() >= 3)   //超过3个0就不能操作
        {
            return;
        }
        if(!tmpEcho.isEmpty())
        {
            ui->label_echo->setText(tmpEcho.append(QString("0")));
        }

        if(!timer4->isActive())
        {
            timer4->start(5000);  //slot_GotoPage()
        }
    }
    else if(id == 4)    //按按钮4
    {
        QSettings tmpSetting(mSetFile, QSettings::IniFormat);
        tmpSetting.setIniCodec("UTF-8");
        tmpSetting.beginGroup("KeyMapping");
        QString tmpKey4MapPic = tmpSetting.value("key4").toString();
        tmpSetting.endGroup();

        int tmp4KeyIndex = m4KeyList.indexOf(tmpKey4MapPic);

        ui->stackedWidget_tips->setCurrentIndex(1); //回显区
        if(tmp4KeyIndex == 0)  //界面显示4
        {
            QString tmpEcho = ui->label_echo->text();
            if(tmpEcho.size() >= 3)
            {
                return;
            }
            ui->label_echo->setText(tmpEcho.append(QString("4")));
            timer1->start(3000);   //自动呼梯
        }
        else if(tmp4KeyIndex == 1)  //A
        {
            QString tmpEcho = ui->label_echo->text();
            if(tmpEcho.size() >= 3)
            {
                return;
            }
            ui->label_echo->setText(tmpEcho.append(QString("A")));

            timer1->start(3000);  //自动呼梯
        }
        else if(tmp4KeyIndex == 2)   //B
        {
            QString tmpEcho = ui->label_echo->text();
            if(tmpEcho.size() >= 3)
            {
                return;
            }
            ui->label_echo->setText(tmpEcho.append(QString("B")));

            timer1->start(3000);  //自动呼梯
        }

    }
    else if(id >0 && id<=9)
    {
        ui->stackedWidget_tips->setCurrentIndex(1);
        QString tmpText = ui->label_echo->text();
        if(tmpText.size() >= 3)
        {
            return;
        }
        ui->label_echo->setText(tmpText.append(QString::number(id)));

//        if(id > 4)
//        {
//            timer1->start(500);  //立即自动呼梯
//        }
//        else
//        {
//            timer1->start(3000);
//        }
        timer1->start(3000);
    }
    else if(id == 13)   //按enter键
    {
        QSettings tmpSetting(mSetFile, QSettings::IniFormat);
        tmpSetting.setIniCodec("UTF-8");

        tmpSetting.beginGroup("KeyMapping");
        QString tmpEnterMapPic = tmpSetting.value("Enter").toString();
        tmpSetting.endGroup();

        int tmpEnterIndex = mEnterList.indexOf(tmpEnterMapPic);
        if(tmpEnterIndex == 1) //tmpEnterMapPic == QString(":/rc/btn_del.png")当删除按钮
        {
            QString tmpEcho = ui->label_echo->text();
            if(ui->stackedWidget_tips->currentIndex() == 1) //如果在回显界面
            {
                if(tmpEcho.size() >= 2)
                {
                    QString tmpString = tmpEcho.mid(0, tmpEcho.size()-1);
                    ui->label_echo->setText(tmpString);
                }
                else if(tmpEcho.size() == 1)
                {
                    ui->label_echo->clear();
                    //QtSleep(1000);
                    ui->stackedWidget_tips->setCurrentIndex(3);   //如果在“请输入目的楼层”界面
                    ui->label_inputDF->setText(QString("Input destination floor"));
                    timer1->stop();
                }
            }
            else
            {
                return;
            }
        }
        else if(tmpEnterIndex == 2) //enter为A图片
        {
            ui->stackedWidget_tips->setCurrentIndex(1);
            QString tmpText = ui->label_echo->text();
            if(tmpText.size() >= 3)
            {
                return;
            }
            ui->label_echo->setText(tmpText.append(QString("A")));

            timer1->start(3000);  //自动呼梯
        }

        else if(tmpEnterIndex == 3) //为B图片
        {
            ui->stackedWidget_tips->setCurrentIndex(1);
            QString tmpText = ui->label_echo->text();
            if(tmpText.size() >= 3)
            {
                return;
            }
            ui->label_echo->setText(tmpText.append(QString("B")));

            timer1->start(3000);   //自动呼梯

        }

        else if(tmpEnterIndex == 0) //为enter图片
        {
            timer1->stop();

            if(ui->stackedWidget_tips->currentIndex() != 1)   //不在回显区按enter没有任何改变
            {
                return;
            }

            QString tmpText = ui->label_echo->text();

            QSettings tmpSetting(mSetFile, QSettings::IniFormat);
            tmpSetting.setIniCodec("UTF-8");

            tmpSetting.beginGroup("FlrMapping");
            QString tmpFlrMapValue = tmpSetting.value(tmpText).toString();
            tmpSetting.endGroup();

            if(!tmpFlrMapValue.isEmpty()) //有映射
            {
                tmpText = tmpFlrMapValue;
            }
            else       //在配置表找不到映射
            {
                ui->stackedWidget_tips->setCurrentIndex(2);    //提示区错误
                ui->label_message->setText(QString("Input Error"));
                ui->label_echo->clear();

                QtSleep(2000);

                ui->stackedWidget_tips->setCurrentIndex(3);
                ui->label_inputDF->setText(QString("Input destination floor"));
                return;
            }

            if(tmpText.isEmpty())
            {
                ui->stackedWidget_tips->setCurrentIndex(3);
                ui->label_inputDF->setText(QString("Input destination floor"));
                ui->label_inputDF->setAlignment(Qt::AlignCenter);
                return;
            }

            QByteArray tmpArray;
            tmpArray.append((char)0x80);
            tmpArray.append((char)0x07);
            tmpArray.append((char)0x00);
            tmpArray.append((char)0x00);
            tmpArray.append((char)tmpText.toInt());

            tmpArray.append((char)0x00);
            tmpArray.append((char)0x00);
            tmpArray.append((char)0x00);
            tmpArray.append((char)0x00);
            tmpArray.append((char)0x00);

            tmpArray.append((char)0x81);

            int tmpwrite = mPort->write(tmpArray);
            qDebug()<<tmpwrite;

            ui->stackedWidget_tips->setCurrentIndex(2);   //提示区派梯
            ui->label_message->setText(QString("Distributing the elevator"));   //正在派梯中...
            timer->start(3000); //超时

            ui->label_echo->clear();
            ui->label_direction->clear();
            ui->label_elevator->clear();
        }

    }
    else if(id == 127)  //删除按钮
    {
        QSettings tmpSetting(mSetFile, QSettings::IniFormat);
        tmpSetting.setIniCodec("UTF-8");

        tmpSetting.beginGroup("KeyMapping");
        QString tmpDelMapPic = tmpSetting.value("Del").toString();
        tmpSetting.endGroup();

        int tmpDelMapIndex = mDelList.indexOf(tmpDelMapPic);

        if(tmpDelMapIndex == 1) //按下负号-
        {
            ui->stackedWidget_tips->setCurrentIndex(1);
            QString tmpEcho = ui->label_echo->text();
            if(tmpEcho.size() >= 3)
            {
                return;
            }
            if(!tmpEcho.contains("-"))
            {
                ui->label_echo->setText(tmpEcho.prepend(QString("-")));
            }
            timer1->start(3000);
        }
        else if(tmpDelMapIndex == 2) //按下A
        {
            ui->stackedWidget_tips->setCurrentIndex(1);
            QString tmpEcho = ui->label_echo->text();
            if(tmpEcho.size() >= 3)
            {
                return;
            }
            ui->label_echo->setText(tmpEcho.append(QString("A")));
            timer1->start(3000);
        }
        else if(tmpDelMapIndex == 3) //按下B
        {
            ui->stackedWidget_tips->setCurrentIndex(1);
            QString tmpEcho = ui->label_echo->text();
            if(tmpEcho.size() >= 3)
            {
                return;
            }
            ui->label_echo->setText(tmpEcho.append(QString("B")));
            timer1->start(3000);
        }
//        else if(tmpDelMapIndex == 4) //按下C
//        {
//            ui->stackedWidget_tips->setCurrentIndex(1);
//            QString tmpEcho = ui->label_echo->text();
//            if(tmpEcho.size()>=3)
//            {
//                return;
//            }
//            ui->label_echo->setText(tmpEcho.append(QString("C")));
//            timer1->start();
//        }
        else if(tmpDelMapIndex == 0) //del功能
        {
            QString tmpEcho = ui->label_echo->text();
            if(ui->stackedWidget_tips->currentIndex() == 1) //如果在回显界面
            {
                if(tmpEcho.size() >= 2)
                {
                    QString tmpString = tmpEcho.mid(0, tmpEcho.size()-1);
                    ui->label_echo->setText(tmpString);
                }
                else if(tmpEcho.size() == 1)
                {
                    ui->label_echo->clear();
                    //QtSleep(1000);
                    ui->stackedWidget_tips->setCurrentIndex(3);
                    ui->label_inputDF->setText(QString("Input destination floor"));
                    timer1->stop();
                }
            }
            else
            {
                return;
            }
        }

    }

}

void MainWindow::slot_BtnPassWdClicked(int id) //密码按钮区域
{
    timer5->stop();
    timer5->start(3000);

    if(id >= 0 && id <= 9 )
    {
        if(ui->label_passwd->text() == QString("Err, Please re-input"))
        {
            ui->label_passwd->clear();
        }
        QString tmpPasswd = ui->label_passwd->text();
        ui->label_passwd->setText(tmpPasswd.append(QString::number(id)));
    }

    if(id == 13)
    {
        QSettings tmpSetting(mSetFile, QSettings::IniFormat);
        tmpSetting.setIniCodec("UTF-8");
        tmpSetting.beginGroup("PassWord");
        QString tmpPasswdMap = tmpSetting.value("passwd").toString();
        tmpSetting.endGroup();

        QString tmpPasswd = ui->label_passwd->text();
        if(tmpPasswdMap == tmpPasswd) //密码相同进入设置页面
        {
            timer5->stop();
            ui->stackedWidget->setCurrentIndex(2);
            ui->stacked_MenuPage->setCurrentIndex(1);
            ui->btn_flrmapping->setChecked(true);
        }
        else
        {
            ui->stackedWidget->setCurrentIndex(1);
            ui->label_passwd->setText(QString("Err, Please re-input"));
        }
    }

    if(id == 127)
    {
        QString tmpPasswd = ui->label_passwd->text();

        if(tmpPasswd.count() > 1)
        {
            ui->label_passwd->setText(ui->label_passwd->text().mid(0, tmpPasswd.count()-1));
        }
        else if(tmpPasswd.count() == 1)
        {
            ui->label_passwd->clear();
        }
    }
}

void MainWindow::slot_AutoCall()
{
    ui->stackedWidget_tips->setCurrentIndex(1);
    QString tmpEcho = ui->label_echo->text();

    QSettings tmpSetting(mSetFile, QSettings::IniFormat);
    tmpSetting.setIniCodec("UTF-8");

    tmpSetting.beginGroup("FlrMapping");
    QString tmpFlrMapValue = tmpSetting.value(tmpEcho).toString();
    tmpSetting.endGroup();

    if(!tmpFlrMapValue.isEmpty()) //有映射
    {
        tmpEcho = tmpFlrMapValue;
    }
    else
    {
        ui->stackedWidget_tips->setCurrentIndex(2);
        ui->label_message->setText(QString("Input Error"));
        ui->label_echo->clear();

        QtSleep(2000);

        ui->stackedWidget_tips->setCurrentIndex(3);
        ui->label_inputDF->setText(QString("Input destination floor"));
        return;
    }

    if(tmpEcho.isEmpty())
    {
        ui->stackedWidget_tips->setCurrentIndex(3);
        ui->label_inputDF->setText(GBKToUTF8("Input destination floor"));
        return;
    }

    QByteArray tmpArray;
    tmpArray.append((char)0x80);
    tmpArray.append((char)0x07);
    tmpArray.append((char)0x00);
    tmpArray.append((char)0x00);
    tmpArray.append((char)tmpEcho.toInt());

    tmpArray.append((char)0x00);
    tmpArray.append((char)0x00);
    tmpArray.append((char)0x00);
    tmpArray.append((char)0x00);
    tmpArray.append((char)0x00);

    tmpArray.append((char)0x81);
    qDebug() << tmpArray;
    mPort->write(tmpArray);

    ui->stackedWidget_tips->setCurrentIndex(2);
    ui->label_message->setText(QString("Distributing the elevator"));
    timer->start(3000);

    ui->label_echo->clear();
    ui->label_direction->clear();
    ui->label_elevator->clear();
}

void MainWindow::slot_CallEleTimeout()  //timer->start()3s之后
{
    ui->stackedWidget_tips->setCurrentIndex(2);
    ui->label_message->setText(QString("Timeout"));

    QtSleep(2000);

    ui->stackedWidget_tips->setCurrentIndex(3);
    ui->label_inputDF->setText(QString("Input destination floor"));
}


void QtSleep(unsigned int msec, QEventLoop::ProcessEventsFlags flags)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents(flags, 1);
#ifdef WINDOWS
        Sleep(1);
#else
        sleep(1);
#endif
//        struct timeval delay;
//        delay.tv_sec = 0;
//        delay.tv_usec = 1000; // 1 ms
//        select(0, NULL, NULL, NULL, &delay);
    }
}

void MainWindow::slot_Read()  //任何时候有数据过来
{
    QByteArray tmpRead = mPort->readAll();
    int count = tmpRead.count();

    if(count != 11)
    {
        return;
    }

    if(mOldRead == tmpRead)
    {
        if(!timer3->isActive())   //定时器关闭情况
        {
            timer3->start(1000);  //第一次如果发来数据相同，打开定时器，先把数据显示一次
        }
        else    //第二次，还发来相同数据，由于定时器开启进入
        {
            return;   //就是不让A重复显示的，直到打开的定时器到1s后，让mOldRead清空
        }
    }
    else
    {
        mOldRead = tmpRead;
        timer3->stop();
    }

    IDE_TRACE_INT(count);
    for(int i = 0; i < count; ++i)
    {
        IDE_TRACE_INT((unsigned char)tmpRead.at(i));
    }

    if((unsigned char)(tmpRead.at(0)) != 0x80)
    {
        return;
    }
    if((unsigned char)(tmpRead.at(1)) != 0x07)
    {
        return;
    }
    if((unsigned char)(tmpRead.at(10)) != 0x81)
    {
        return;
    }

    ui->stackedWidget_tips->setCurrentIndex(0);

    if((unsigned char)(tmpRead.at(2)) == 0x01)
    {
        ui->label_elevator->setText("A");

        QSettings tmpSetting(mSetFile, QSettings::IniFormat);
        tmpSetting.setIniCodec("UTF-8");

        tmpSetting.beginGroup("DirArrMapping");
        QString tmpDirArrMapPic = tmpSetting.value("A").toString();
        tmpSetting.endGroup();
        ui->label_direction->setPixmap(QPixmap(tmpDirArrMapPic));

    }
    else if((unsigned char)(tmpRead.at(2)) == 0x02)
    {
        ui->label_elevator->setText("B");

        QSettings tmpSetting(mSetFile, QSettings::IniFormat);
        tmpSetting.setIniCodec("UTF-8");

        tmpSetting.beginGroup("DirArrMapping");
        QString tmpDirArrMapPic = tmpSetting.value("B").toString();
        tmpSetting.endGroup();
        ui->label_direction->setPixmap(QPixmap(tmpDirArrMapPic));

    }

    else if((unsigned char)(tmpRead.at(2)) == 0x03)
    {
        ui->label_elevator->setText("C");
        QSettings tmpSetting(mSetFile, QSettings::IniFormat);
        tmpSetting.setIniCodec("UTF-8");

        tmpSetting.beginGroup("DirArrMapping");
        QString tmpDirArrMapPic = tmpSetting.value("C").toString();
        tmpSetting.endGroup();
        ui->label_direction->setPixmap(QPixmap(tmpDirArrMapPic));
    }

    else if((unsigned char)(tmpRead.at(2)) == 0x04)
    {
        ui->label_elevator->setText("D");

        QSettings tmpSetting(mSetFile, QSettings::IniFormat);
        tmpSetting.setIniCodec("UTF-8");

        tmpSetting.beginGroup("DirArrMapping");
        QString tmpDirArrMapPic = tmpSetting.value("D").toString();
        tmpSetting.endGroup();
        ui->label_direction->setPixmap(QPixmap(tmpDirArrMapPic));
    }

    else if((unsigned char)(tmpRead.at(2)) == 0x7F)
    {
        ui->stackedWidget_tips->setCurrentIndex(2);
        ui->label_message->setText(QString("Distributed failure"));
    }

    else
    {
        ui->label_elevator->setText(QString());
        ui->label_direction->setPixmap(QPixmap());
    }

    timer->stop();

    timer2->start();
}

void MainWindow::slot_inputTips()
{
    ui->stackedWidget_tips->setCurrentIndex(3);
    ui->label_inputDF->setText(QString("Input destination floor"));
}

void MainWindow::slot_ClearOldArray()
{
    mOldRead.clear();
}

void MainWindow::slot_MenuPage(int id)
{
    if(id < ui->stacked_MenuPage->count())
    {
        ui->stacked_MenuPage->setCurrentIndex(id);
        if(id == 0) //箭头页面
        {
            ui->btn_arrmapping->setChecked(true);
            ui->btn_flrmapping->setChecked(false);


            ui->widget_key_enter->SetPressed(false);
            ui->widget_key_del->SetPressed(false);
        }
        else if(id == 1) //设置楼层页面
        {
            ui->btn_arrmapping->setChecked(false);
            ui->btn_flrmapping->setChecked(true);

            ui->widget_arrow_A->SetPressed(false);
            ui->widget_arrow_B->SetPressed(false);
            ui->widget_arrow_C->SetPressed(false);
            ui->widget_arrow_D->SetPressed(false);
        }
    }
}

void MainWindow::slot_ArrowSelected()
{
    MenuItem *tmpItem = qobject_cast<MenuItem *>(sender());
    if(tmpItem)
    {
        if(tmpItem == ui->widget_arrow_A)
        {
            ui->widget_arrow_B->SetPressed(false);
            ui->widget_arrow_C->SetPressed(false);
            ui->widget_arrow_D->SetPressed(false);
        }
        else if(tmpItem == ui->widget_arrow_B)
        {
            ui->widget_arrow_A->SetPressed(false);
            ui->widget_arrow_C->SetPressed(false);
            ui->widget_arrow_D->SetPressed(false);
        }
        else if(tmpItem == ui->widget_arrow_C)
        {
            ui->widget_arrow_A->SetPressed(false);
            ui->widget_arrow_B->SetPressed(false);
            ui->widget_arrow_D->SetPressed(false);
        }
        else if(tmpItem == ui->widget_arrow_D)
        {
            ui->widget_arrow_A->SetPressed(false);
            ui->widget_arrow_B->SetPressed(false);
            ui->widget_arrow_C->SetPressed(false);
        }
    }
}

void MainWindow::slot_KeySelected()
{
    MenuItem *tmpItem = qobject_cast<MenuItem *>(sender());
    if(tmpItem == ui->widget_key_enter)
    {
        ui->widget_key_del->SetPressed(false);
        ui->widget_key_4->SetPressed(false);
    }
    else if(tmpItem == ui->widget_key_del)
    {
        ui->widget_key_enter->SetPressed(false);
        ui->widget_key_4->SetPressed(false);
    }
    else if(tmpItem == ui->widget_key_4)
    {
        ui->widget_key_del->SetPressed(false);
        ui->widget_key_enter->SetPressed(false);
    }
}

void MainWindow::slot_GotoPage()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->label_passwd->clear();
    timer5->start(3000);
}

void MainWindow::slot_BackHome()
{
    ui->stackedWidget->setCurrentIndex(0);
}

//pValue 0~100
int MainWindow::SetLuminance(int pValue)
{
    int tmpValue = pValue*255/100;
    QProcess tmpProcess;
    QString tmpCmd = QString("echo /sys/class/backlight/pwm-backlight.0/brightness  %1").arg(tmpValue);
    tmpProcess.startDetached(tmpCmd);
    return 0;
}

void MainWindow::on_btn_0_released()
{
     timer4->stop();
}

void MainWindow::on_btn_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
