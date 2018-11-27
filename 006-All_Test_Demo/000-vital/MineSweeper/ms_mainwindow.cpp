#include "ms_mainwindow.h"


/***********************************************************************
函数名字：MS_MainWindow(QWidget *parent)
函数功能：构造函数，初始化主要的数据成员，创建相应的界面组件，创建主程序界面
函数流程：
    初始化成员
    读取注册表 readSettings
    创建菜单 createMenu();
    创建游戏 createGame();
************************************************************************/
MS_MainWindow::MS_MainWindow(QWidget *parent)
    : QMainWindow(parent)/*,m_timer(0),m_widget(0),m_mineLab(0),m_timeLab(0),\
      m_mineLCD(0),m_timeLCD(0),m_smileBtn(0),vboxlayout(0),m_view(0),m_scene(0)*/
{
    this->setWindowTitle("扫雷");
    this->setWindowIcon(QIcon(":/images/icon.png"));
    m_isCustom = false;
    m_isgameBegin = false;
    m_isUpdate = false;
    m_isrecountHeroRollScore = false;
    m_soundOpen = false;
    m_gameLevel = LOW_LEVEL;
    sound_1 = new QSound(":/sounds/question.wav");//开局
    sound_2 = new QSound(":/sounds/click.wav");//单击
    m_playerDialog = new playerDialog(this);
    connect(m_playerDialog,SIGNAL(sendPlayerName(QString)),this,SLOT(recvPlayerName(QString)));
    m_time = 0;
    readSettings();
    createMenu();
    createGame();


}

/***********************************************************************
函数名字：createMenu()
函数功能：初始化菜单对象，创建菜单
函数流程：
    初始化成员
    把对象添加到菜单
************************************************************************/
void MS_MainWindow::createMenu()     //创建菜单
{
    QMenuBar *menubar = this->menuBar();
    m_gameMenu = menubar->addMenu("游戏(&G)");
    m_helpMenu = menubar->addMenu("帮助(&H)");

    m_newGameAct = new QAction("新建(&N)",this);
    m_newGameAct->setShortcut(tr("F2"));
    this->connect(m_newGameAct,SIGNAL(triggered()),this,SLOT(newGame()));

    m_lowLevelAct= new QAction("初级(&B)",this);
    m_lowLevelAct->setCheckable(true);
    this->connect(m_lowLevelAct,SIGNAL(triggered()),this,SLOT(lowLevelGame()));

    m_middleLevelAct = new QAction("中级(&I)",this);
    m_middleLevelAct->setCheckable(true);
    this->connect(m_middleLevelAct,SIGNAL(triggered()),this,SLOT(middleLevelGame()));

    m_highLevelAct = new QAction("高级(&E)",this);
    m_highLevelAct->setCheckable(true);
    this->connect(m_highLevelAct,SIGNAL(triggered()),this,SLOT(highLevelGame()));

    m_gameSetAct = new QAction("自定义(&C)",this);
    m_gameSetAct->setCheckable(true);
    this->connect(m_gameSetAct,SIGNAL(triggered()),this,SLOT(gameSet()));//

    m_soundAct = new QAction("声音(&S)",this);
    m_soundAct->setCheckable(true);
    m_soundAct->setShortcut(tr("F3"));
    this->connect(m_soundAct,SIGNAL(triggered()),this,SLOT(soundAction()));

    m_heroRollAct = new QAction("扫雷英雄榜(&T)",this);
    m_heroRollAct->setShortcut(tr("F4"));
    this->connect(m_heroRollAct,SIGNAL(triggered()),this,SLOT(displayHeroRoll()));

    m_exitAct = new QAction("退出(&X)",this);
    this->connect(m_exitAct,SIGNAL(triggered()),this,SLOT(close()));

    m_aboutAct = new QAction("关于(&A)",this);
    this->connect(m_aboutAct,SIGNAL(triggered()),this,SLOT(about()));

    m_gameMenu->addAction(m_newGameAct);
    m_gameMenu->addSeparator();
    QActionGroup *actiongroup1 = new QActionGroup(this);

    actiongroup1->addAction(m_lowLevelAct);
    actiongroup1->addAction(m_middleLevelAct);
    actiongroup1->addAction(m_highLevelAct);

    actiongroup1->addAction(m_gameSetAct);
    if(m_isCustom)
        m_gameSetAct->setChecked(true);
    else
        switch(m_gameLevel)
        {
        case LOW_LEVEL: m_lowLevelAct->setChecked(true);
            break;
        case MIDDLE_LEVEL: m_middleLevelAct->setChecked(true);
            break;
        case HIGH_LEVEL: m_highLevelAct->setChecked(true);
            break;
        default:
            break;
        }

    m_gameMenu->addAction(m_lowLevelAct);
    m_gameMenu->addAction(m_middleLevelAct);
    m_gameMenu->addAction(m_highLevelAct);

    m_gameMenu->addAction(m_gameSetAct);
    m_gameMenu->addSeparator();
    QActionGroup *actiongroup2 = new QActionGroup(this);

    actiongroup2->addAction(m_soundAct);
    m_soundAct->setChecked(false);
    m_gameMenu->addAction(m_soundAct);

    m_gameMenu->addSeparator();
    m_gameMenu->addAction(m_heroRollAct);
    m_gameMenu->addSeparator();

    m_gameMenu->addAction(m_exitAct);
    m_helpMenu->addAction(m_aboutAct);
}

/***********************************************************************
函数名字：createGame()
函数功能：初始化游戏对象，创建游戏
函数流程：
    初始化成员
    初始化itme对象，设置雷数，建立itme对象的连接
    初始化定时器
    创建游戏界面
************************************************************************/
void MS_MainWindow::createGame()     //创建游戏
{
    if(m_soundOpen)
        sound_1->play();
    m_widget = new QWidget;

    this->setCentralWidget(m_widget);
    m_mineLab = new QLabel;
    m_mineLab->setPixmap(QPixmap(":/images/mine.png"));

    m_timeLab = new QLabel;
    m_timeLab->setPixmap(QPixmap(":/images/clock.png"));
    QPalette palette;

    palette.setColor(QPalette::Window,Qt::darkCyan);
    m_mineLCD = new QLCDNumber;    //显示雷数
    m_mineLCD->setDigitCount(3);

    m_mineLCD->setPalette(palette);
    m_mineLCD->setAutoFillBackground(true);
    m_mineLCD->setStyleSheet("color:white");

    m_timeLCD = new QLCDNumber(this);    //显示时间
    m_timeLCD->setDigitCount(3);
    m_timeLCD->setPalette(palette);

    m_timeLCD->setAutoFillBackground(true);
    m_timeLCD->setStyleSheet("color:white");

    m_smileBtn = new QPushButton;  //笑脸按钮
    m_smileBtn->setIcon(QIcon(":/images/easy.png"));
    m_smileBtn->setIconSize(QSize(29,29));
    connect(m_smileBtn,SIGNAL(clicked()),this,SLOT(newGame()));

    hboxlayout = new QHBoxLayout;
    hboxlayout->addWidget(m_mineLab);
    hboxlayout->addWidget(m_mineLCD);
    hboxlayout->addStretch(Qt::Horizontal);
    hboxlayout->addWidget(m_smileBtn);
    hboxlayout->addStretch(Qt::LeftButton);
    hboxlayout->addWidget(m_timeLab);
    hboxlayout->addWidget(m_timeLCD);

    m_view = new QGraphicsView;    //镜头
    vboxlayout = new QVBoxLayout(m_widget);
    vboxlayout->addLayout(hboxlayout);
    vboxlayout->addWidget(m_view);

    m_scene = new myScene;
    if(m_soundOpen)
        m_scene->m_soundOpen = true;
    if(m_isCustom)
    {
        m_scene->m_rowNum = m_value[0];//方块的行数
        m_scene->m_colNum = m_value[1];//方块的列数
        m_scene->m_mineNum = m_value[2];//方块的雷数
    }
    else
        switch(m_gameLevel)
        {
            case LOW_LEVEL: m_scene->m_rowNum = 9;
                            m_scene->m_colNum = 9;
                            m_scene->m_mineNum = 10;
                            m_value[0] = 9;
                            m_value[1] = 9;
                            m_value[2] = 10;
                            break;
            case MIDDLE_LEVEL: m_scene->m_rowNum = 16;
                               m_scene->m_colNum = 16;
                               m_scene->m_mineNum = 40;
                               m_value[0] = 16;
                               m_value[1] = 16;
                               m_value[2] = 40;
                               break;
            case HIGH_LEVEL: m_scene->m_rowNum = 16;
                             m_scene->m_colNum = 30;
                             m_scene->m_mineNum = 99;
                             m_value[0] = 16;
                             m_value[1] = 30;
                             m_value[2] = 99;
                             break;
            default:
                break;
        }

    int width = QPixmap(":/images/enter.png").width()*m_scene->m_colNum;
    int height = QPixmap(":/images/enter.png").height()*m_scene->m_rowNum;
    int t_mineNum = m_scene->m_mineNum;

    m_scene->setSceneRect(-width/2,-height/2,width,height);
    m_view->setScene(m_scene);
    m_view->setFixedSize(width+4,height+4);
    this->setFixedSize(width+20,height+80);

    for(int i = 0; i < m_scene->m_rowNum; i++)
    {
        for(int j = 0; j < m_scene->m_colNum; j++)
        {
            myItem *t_item = new myItem(i,j,QPixmap(":/images/enter.png"),this);
            m_scene->t_vec->push_back(t_item);
        }
    }

    for(int i = 0; i < m_scene->m_rowNum; i++)
    {
        for(int j = 0; j < m_scene->m_colNum; j++)
        {
            m_scene->t_vec->at(i*m_scene->m_colNum + j)->setPos(-width/2+j*QPixmap(":/images/enter.png").width(),-height/2+i*QPixmap(":/images/enter.png").height());
            m_scene->addItem(m_scene->t_vec->at(i*m_scene->m_colNum + j));
        }
    }

    srand(time(NULL));
    while(t_mineNum)
    {
        //随机产生一个方块的坐标
        int t_rows = rand()%m_scene->m_rowNum;
        int t_cols = rand()%m_scene->m_colNum;

        //设置该方块为雷
        if(!m_scene->t_vec->at(t_rows*m_scene->m_colNum + t_cols)->m_isMine)
        {
            m_scene->t_vec->at(t_rows*m_scene->m_colNum + t_cols)->m_isMine = true;
//            m_scene->t_vec->at(t_rows*m_scene->m_colNum + t_cols)->setPixmap(QPixmap(":/images/end.png"));
            //将该雷周围的八点的方块上的数字加一
            m_scene->countAroundMines(m_scene->t_vec->at(t_rows*m_scene->m_colNum + t_cols));
            t_mineNum--;
        }
    }

    for(int i = 0; i < m_scene->m_rowNum; i++)
    {
        for(int j = 0; j < m_scene->m_colNum; j++)
        {
            connect(m_scene->t_vec->at(i*m_scene->m_colNum + j),SIGNAL(signal_openAllItems()),m_scene,SLOT(openAllItems()));
            connect(m_scene->t_vec->at(i*m_scene->m_colNum + j),SIGNAL(signal_expendWater(myItem*)),m_scene,SLOT(expendWater(myItem*)));
            connect(m_scene->t_vec->at(i*m_scene->m_colNum + j),SIGNAL(signal_openAroundItems(myItem*)),m_scene,SLOT(openAroundItems(myItem*)));
            connect(m_scene->t_vec->at(i*m_scene->m_colNum + j),SIGNAL(signal_beginGame()),this,SLOT(beginGame()));
            connect(m_scene->t_vec->at(i*m_scene->m_colNum + j),SIGNAL(signal_endGame(int)),this,SLOT(endGame(int)));
            connect(m_scene->t_vec->at(i*m_scene->m_colNum + j),SIGNAL(signal_setMineNum(int)),this,SLOT(displayMineNum(int)));
        }
    }

    m_timer = new QTimer;
    m_mineNum = m_scene->m_mineNum;
    m_mineLCD->display(m_mineNum);

}

/***********************************************************************
函数名字：beginGame() （槽）
函数功能：接收item对象发送的信号，判断后开始游戏
函数流程：
    接收信号
    判断信号正确后启动定时器
************************************************************************/
void MS_MainWindow::beginGame()           //开局
{
    if(!m_isgameBegin)
    {
        m_timeLCD->connect(m_timer,SIGNAL(timeout()),this,SLOT(displayTime()));
        m_timer->start(1000);
        m_isgameBegin = true;
    }
}

/***********************************************************************
函数名字：endGame(int flag)  （槽）
函数功能：接收item对象发送的信号，判断后处理
函数流程：
    接收信号
    判断信号
    做出相应响应
************************************************************************/
void MS_MainWindow::endGame(int flag)           //游戏结束处理
{
    if(flag == 3)
        m_smileBtn->setIcon(QIcon(":/images/normal.png"));
    if(flag == 4)
    {
        m_scene->m_remainNoMines++;
        m_smileBtn->setIcon(QIcon(":/images/easy.png"));
    }

    if(flag == 0)
    {
        m_smileBtn->setIcon(QIcon(":/images/difficult.png"));
        m_timer->stop();
        m_time = 0;
        int button = m_messageDialog->information(this,"游戏对话","笨蛋倒霉鬼，游戏结束！\n是否开始新游戏？",QMessageBox::Ok | QMessageBox::Cancel);
        switch(button)
        {
        case QMessageBox::Ok: newGame();
            break;
        case QMessageBox::Cancel: this->close();
            break;
        }
    }

    if(flag == 1)
    {
        if(m_soundOpen)
            sound_2->play();
        m_smileBtn->setIcon(QIcon(":/images/easy.png"));
        m_scene->m_remainNoMines++;
    }

    if(m_scene->m_isGameOver)
    {
        m_smileBtn->setIcon(QIcon(":/images/difficult.png"));
        m_timer->stop();
        m_time = 0;
        m_scene->openAllItems();
        int button = m_messageDialog->information(this,"游戏对话","笨蛋倒霉鬼，游戏结束！\n是否开始新游戏？",QMessageBox::Ok | QMessageBox::Cancel);
        switch(button)
        {
        case QMessageBox::Ok: newGame();
            break;
        case QMessageBox::Cancel: this->close();
            break;
        }
    }

    if(m_value[2] == m_value[0]*m_value[1] - m_scene->m_remainNoMines)
    {
        m_timer->stop();
        switch(m_gameLevel)
        {
        case LOW_LEVEL: m_playerTime = m_gameRecode[0].m_gameTime;
            break;
        case MIDDLE_LEVEL: m_playerTime = m_gameRecode[1].m_gameTime;
            break;
        case HIGH_LEVEL: m_playerTime = m_gameRecode[2].m_gameTime;
            break;
        default:
            break;
        }
        if(m_playerTime > m_time && !m_isCustom)
        {
            m_playerTime = m_time;
            m_isUpdate = true;
            m_playerDialog->show();
        }
        else
        {
            int button;
            if(m_isCustom)
                button = m_messageDialog->information(this,"游戏对话","你过关了，这是个自定义游戏！\n是否开始新游戏？",QMessageBox::Ok | QMessageBox::Cancel);
            else
                button = m_messageDialog->information(this,"游戏对话","恭喜过关，但你还不是最优秀的人！\n是否开始新游戏？",QMessageBox::Ok | QMessageBox::Cancel);
            switch(button)
            {
            case QMessageBox::Ok: newGame();
                break;
            case QMessageBox::Cancel: this->close();
                break;
            }
        }
        m_time = 0;
    }
}


/***********************************************************************
函数名字：newGame()
函数功能：新游戏
函数流程：
    停止定时器
    清除上一盘游戏的数据
    调用创建游戏
************************************************************************/
void MS_MainWindow::newGame()           //新游戏
{
    if(m_isgameBegin == true)
    {
        m_timer->stop();
        m_time = 0;
    }
    m_isgameBegin = false;
    m_timer->deleteLater();
    m_widget->deleteLater();
    m_mineLab->deleteLater();
    m_timeLab->deleteLater();
    m_mineLCD->deleteLater();
    m_timeLCD->deleteLater();
    m_smileBtn->deleteLater();
    vboxlayout->deleteLater();
    hboxlayout->deleteLater();
    m_view->deleteLater();
    m_scene->deleteLater();
    createGame();
}

/***********************************************************************
函数名字：lowLevelGame()
函数功能：切换至初级游戏
函数流程：
    设置游戏级别
    新游戏
************************************************************************/
void MS_MainWindow::lowLevelGame()     //初级游戏
{
    m_gameLevel = LOW_LEVEL;
    m_isCustom = false;
    m_value[0] = 9;
    m_value[1] = 9;
    m_value[2] = 10;
    newGame();
}

/***********************************************************************
函数名字：middleLevelGame()
函数功能：切换至初级游戏
函数流程：
    设置游戏级别
    新游戏
************************************************************************/
void MS_MainWindow::middleLevelGame()   //中级游戏
{
    m_gameLevel = MIDDLE_LEVEL;
    m_isCustom = false;
    m_value[0] = 16;
    m_value[1] = 16;
    m_value[2] = 40;
    newGame();
}

/***********************************************************************
函数名字：highLevelGame()
函数功能：切换至初级游戏
函数流程：
    设置游戏级别
    新游戏
************************************************************************/
void MS_MainWindow::highLevelGame()     //高级游戏
{
    m_gameLevel = HIGH_LEVEL;
    m_isCustom = false;
    m_value[0] = 16;
    m_value[1] = 30;
    m_value[2] = 99;
    newGame();
}

/***********************************************************************
函数名字：gameSet() （槽）
函数功能：设置自定义游戏
函数流程：
    设置对话
    输入扫雷相关的数据
************************************************************************/
void MS_MainWindow::gameSet() //自定义游戏slot
{
    m_setDialog = new setDialog(this);
    connect(m_setDialog,SIGNAL(sendSetInfo(int,int,int)),this,SLOT(setValue(int,int,int)));
    m_setDialog->spinBox_row->setValue(m_value[0]);
    m_setDialog->spinBox_col->setValue(m_value[1]);
    m_setDialog->spinBox_mine->setValue(m_value[2]);
    m_setDialog->show();
}

/***********************************************************************
函数名字：setValue(int t_rowNum,int t_colNum,int t_mineNum) //自定义游戏
函数功能：自定义游戏
函数流程：
    接收对话框输入的数据
    设置自定义游戏的数据
    新游戏
************************************************************************/
void MS_MainWindow::setValue(int t_rowNum,int t_colNum,int t_mineNum) //自定义游戏
{
    if(t_rowNum >= 9 && t_rowNum <= 24 && t_colNum >= 9 && t_colNum <= 30 \
            && t_mineNum >= 1 && t_mineNum <= 668 && t_mineNum <= t_rowNum*t_colNum)
    {
        m_value[0] = t_rowNum;
        m_value[1] = t_colNum;
        m_value[2] = t_mineNum;
        m_isCustom = true;
    }
    newGame();
}

/***********************************************************************
函数名字：soundAction()
函数功能：是否开启声音
函数流程：
************************************************************************/
void MS_MainWindow::soundAction()      //是否开启声音
{
    if(m_soundOpen == false)
    {
        m_soundOpen = true;
        m_scene->m_soundOpen = true;
        m_soundAct->setChecked(true);
    }
    else
    {
        m_soundOpen = false;
        m_scene->m_soundOpen = false;
        m_soundAct->setChecked(false);
    }

}

/***********************************************************************
函数名字：displayMineNum(int t_signedMineNum)
函数功能：在窗口显示雷数
函数流程：
************************************************************************/
void MS_MainWindow::displayMineNum(int t_signedMineNum) //显示雷数
{
    m_mineNum -= t_signedMineNum;
    m_mineLCD->display(m_mineNum);
}

/***********************************************************************
函数名字：displayHeroRoll()
函数功能：在窗口显示扫雷英雄榜
函数流程：
************************************************************************/
void MS_MainWindow::displayHeroRoll()//显示扫雷英雄榜
{
    m_heroDialog = new heroDialog(this);
    connect(m_heroDialog->pushButton,SIGNAL(released()),this,SLOT(recountHeroRollScore()));
    m_heroDialog->label_low_time->setNum(m_gameRecode[0].m_gameTime);
    m_heroDialog->label_low_name->setText(m_gameRecode[0].m_playerName);

    m_heroDialog->label_middle_time->setNum(m_gameRecode[1].m_gameTime);
    m_heroDialog->label_middle_name->setText(m_gameRecode[1].m_playerName);
    m_heroDialog->label_high_time->setNum(m_gameRecode[2].m_gameTime);

    m_heroDialog->label_high_name->setText(m_gameRecode[2].m_playerName);
    m_heroDialog->show();
}

/***********************************************************************
函数名字：displayTime()
函数功能：在窗口显示时间
函数流程：
************************************************************************/
void MS_MainWindow::displayTime()//显示时间
{
    m_time++;
    m_timeLCD->display(m_time);
}

/***********************************************************************
函数名字：readSettings()
函数功能：从注册表读入数据
函数流程：
************************************************************************/
void MS_MainWindow::readSettings()
{
    //创建一个QSettings对象前一个参数是公司名称，第2个参数为应用程序名称
    QSettings setting("DEJ","MineSweeper");

//    setting.beginGroup("MAINWINDOW");
//    QPoint pos = setting.value("position").toPoint();
//    QSize size = setting.value("size").toSize();
//    setting.endGroup();

    //读取游戏当前等级
    setting.beginGroup("LEVEL");
    int level = setting.value("level").toInt();
    bool custom = setting.value("custom").toBool();
    setting.endGroup();

    switch(level)
    {
    case 1: m_gameLevel = LOW_LEVEL;
        break;
    case 2: m_gameLevel = MIDDLE_LEVEL;
        break;
    case 3: m_gameLevel = HIGH_LEVEL;
        break;
    default:
        break;
    }
    m_isCustom = custom;

    //读取游戏当前排名
    setting.beginGroup("RANK");
    int low_time = setting.value("low_time").toInt();
    QString low_user =  setting.value("low_user").toString();
    int middle_time = setting.value("middle_time").toInt();

    QString middle_user =  setting.value("middle_user").toString();
    int high_time = setting.value("high_time").toInt();
    QString high_user =  setting.value("high_user").toString();
    setting.endGroup();

    m_gameRecode[0].m_gameTime = low_time;
    m_gameRecode[0].m_playerName = low_user;
    m_gameRecode[1].m_gameTime = middle_time;

    m_gameRecode[1].m_playerName = middle_user;
    m_gameRecode[2].m_gameTime = high_time;
    m_gameRecode[2].m_playerName = high_user;


    //读取游戏当前参数
    setting.beginGroup("GAME_VALUE");
    int rowNum = setting.value("RowNum").toInt();
    int colNum = setting.value("ColNum").toInt();
    int mineNum = setting.value("MineNum").toInt();
    setting.endGroup();
    m_value[0] = rowNum;
    m_value[1] = colNum;
    m_value[2] = mineNum;
}

/***********************************************************************
函数名字：writeSettings()
函数功能：往注册表写入数据
函数流程：
************************************************************************/
void MS_MainWindow::writeSettings()
{
    //创建一个QSettings对象前一个参数是公司名称，第2个参数为应用程序名称
    QSettings setting("DEJ","MineSweeper");

    setting.beginGroup("MAINWINDOW");
    setting.setValue("position",this->pos());
    setting.setValue("size",this->size());
    setting.endGroup();

    //游戏当前排名
    setting.beginGroup("RANK");
    if((!m_isCustom && m_isUpdate) || m_isrecountHeroRollScore)
    {
        setting.setValue("low_time",m_gameRecode[0].m_gameTime);
        setting.setValue("low_user",m_gameRecode[0].m_playerName);
        setting.setValue("middle_time",m_gameRecode[1].m_gameTime);

        setting.setValue("middle_user",m_gameRecode[1].m_playerName);
        setting.setValue("high_time",m_gameRecode[2].m_gameTime);
        setting.setValue("high_user",m_gameRecode[2].m_playerName);
    }
    setting.endGroup();

    //游戏当前等级
    setting.beginGroup("LEVEL");
    setting.setValue("level",m_gameLevel);
    setting.setValue("custom",m_isCustom);
    setting.endGroup();

    //游戏当前参数
    setting.beginGroup("GAME_VALUE");
    setting.setValue("RowNum",m_value[0]);
    setting.setValue("ColNum",m_value[1]);
    setting.setValue("MineNum",m_value[2]);
    setting.endGroup();
}

/***********************************************************************
函数名字：closeEvent(QCloseEvent *event)
函数功能：QMainWindow对象被关闭响应
函数流程：
************************************************************************/
void MS_MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
}


/***********************************************************************
函数名字：recvPlayerName(QString t_playerName)//接收刷新记录的玩家名
函数功能：刷新纪录
函数流程：
    根据游戏级别写入刷新的时间及接收到对话输入的玩家名
    写入注册表
************************************************************************/
void MS_MainWindow::recvPlayerName(QString t_playerName)//接收刷新记录的玩家名
{
    switch(m_gameLevel)
    {
    case LOW_LEVEL: m_gameRecode[0].m_gameTime = m_playerTime;
        m_gameRecode[0].m_playerName = t_playerName;
        break;
    case MIDDLE_LEVEL: m_gameRecode[1].m_gameTime = m_playerTime;
        m_gameRecode[1].m_playerName = t_playerName;
        break;
    case HIGH_LEVEL: m_gameRecode[2].m_gameTime = m_playerTime;
        m_gameRecode[2].m_playerName = t_playerName;
        break;
    default:
        break;
    }
    m_isrecountHeroRollScore = true;
    writeSettings();
    m_isrecountHeroRollScore = false;
    newGame();
}

/***********************************************************************
函数名字：recountHeroRollScore()
函数功能：重新计算英雄榜的分数
函数流程：
    根据游戏级别写入刷新的时间及接收到对话输入的玩家名
    写入注册表
************************************************************************/
void MS_MainWindow::recountHeroRollScore()//重新计算英雄榜的分数
{
    for(int i = 0; i < 3; i++)
    {
        m_gameRecode[i].m_gameTime = 999;
        m_gameRecode[i].m_playerName = "匿名";
    }
    m_isrecountHeroRollScore = true;
    writeSettings();
    m_isrecountHeroRollScore = false;
}

/***********************************************************************
函数名字：about()
函数功能：关于信息
函数流程：
************************************************************************/
void MS_MainWindow::about()
{
    m_messageDialog->about(this,"关于扫雷","本游戏内含十八禁内容，未满十八岁请自觉离开！！！");
}

MS_MainWindow::~MS_MainWindow()
{
    
}
