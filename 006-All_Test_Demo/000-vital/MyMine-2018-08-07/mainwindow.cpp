#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAction>
#include "logcat.h"
#include "mygraphicsscene.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initWindow();
    mCurScene = nullptr;
    mLastScene = nullptr;
    connect(&mMyMsgBox, &MyMsgBox::sigBtnSelect, this, &MainWindow::sltBtnClikced);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (mCurScene)
    {
        delete mCurScene;
        mCurScene = nullptr;
        MYDebug("will delete mCurScene");
    }
}

void MainWindow::initWindow()
{
    ui->BtnFace->setIconSize(QSize(30, 30));
    ui->BtnFace->setFixedSize(40, 40);
    ui->BtnFace->setIcon(QIcon(tr(":/images/Smiling_face.png")));
    connect(ui->BtnFace, &QPushButton::clicked, this, &MainWindow::sltNewGame);
    connect(ui->menu_Action_NewGame, &QAction::triggered, this, &MainWindow::sltNewGame);
    connect(ui->menu_Action_Exit, &QAction::triggered, this, &MainWindow::sltExitGame);
    connect(ui->menu_Action_GameRules, &QAction::triggered, this, &MainWindow::sltGameRulesIntroduce);
    connect(ui->Menu_Action_9And9And10, &QAction::triggered, this, &MainWindow::sltGame9And9And10);
    connect(ui->menu_Action_16And16And40, &QAction::triggered, this, &MainWindow::sltGame16And16And40);
    connect(ui->menu_Action_Prompt, &QAction::triggered, this, &MainWindow::sltGamePrompt);
    connect(ui->menu_Action_SelfDefine, &QAction::triggered, this, &MainWindow::sltSelfDefineGame);
    connect(ui->menu_Action_Winner, &QAction::triggered, this, &MainWindow::sltWinnerHistory);
}

void MainWindow::sltNewGame()
{
    MYDebug();
    mLastScene = mCurScene;
    mCurScene = new MyGraphicsScene;
    if (mCurScene)
    {
        mCurScene->setMineScene(9, 9, 10);
        sltSetMineCounts(mCurScene->getMineCounts());
        mCurScene->initMineScenery();
        mCurScene->layMines();
        mCurScene->setItemMineCounts();
        ui->graphicsView->setScene(mCurScene); // 加入场景
        mCurScene->printMinePos();
        mCurScene->prinMineNumber();
    }
    if (mLastScene)
    {
        delete mLastScene;
        mLastScene = nullptr;
        MYDebug("will delete mLastScene");
    }
}

void MainWindow::sltExitGame()
{
    MYDebug();
}

void MainWindow::sltGameEnded()
{
    MYDebug();
}

void MainWindow::sltGameRulesIntroduce()
{
    MYDebug();
    // Qt::WindowModal 点击了最小化之后，可以再点击主界面
    // Qt::ApplicationModal 点击了最小化之后，再点击主界面，也不能点击
    mIntroduceMine.setWindowModality(Qt::ApplicationModal);
    mIntroduceMine.show();
}

void MainWindow::sltGamePrompt()
{
    MYDebug();
}

void MainWindow::sltGame9And9And10()
{
    MYDebug();
    MYDebug();
    mLastScene = mCurScene;
    mCurScene = new MyGraphicsScene;
    if (mCurScene)
    {
        mCurScene->setMineScene(9, 9, 10);
        sltSetMineCounts(mCurScene->getMineCounts());
        mCurScene->initMineScenery();
        mCurScene->layMines();
        mCurScene->setItemMineCounts();
        ui->graphicsView->setScene(mCurScene); // 加入场景
        mCurScene->printMinePos();
        mCurScene->prinMineNumber();
    }
    if (mLastScene)
    {
        delete mLastScene;
        mLastScene = nullptr;
        MYDebug("will delete mLastScene");
    }
}

void MainWindow::sltGame16And16And40()
{
    MYDebug();
    mLastScene = mCurScene;
    mCurScene = new MyGraphicsScene;
    if (mCurScene)
    {
        mCurScene->setMineScene(16, 16, 40);
        sltSetMineCounts(mCurScene->getMineCounts());
        mCurScene->initMineScenery();
        mCurScene->layMines();
        mCurScene->setItemMineCounts();
        ui->graphicsView->setScene(mCurScene); // 加入场景
        mCurScene->printMinePos();
        mCurScene->prinMineNumber();
    }
    if (mLastScene)
    {
        delete mLastScene;
        mLastScene = nullptr;
        MYDebug("will delete mLastScene");
    }
}

void MainWindow::sltSelfDefineGame()
{
    MYDebug();
    mMyselfDefineGame.setWindowModality(Qt::ApplicationModal);
    mMyselfDefineGame.show();
}

void MainWindow::sltWinnerHistory()
{
    MYDebug();
    mMyMsgBox.setTextContent("this is history");
    mMyMsgBox.setTitleName("this is history");
    mMyMsgBox.exec();
}

void MainWindow::sltBtnClikced(QString pYes)
{
    if (!QString::compare("yes", pYes))
    {
        mMyMsgBox.accept();
        MYDebug("is yes clicked");
    }
    else if (!QString::compare("no", pYes))
    {
        mMyMsgBox.reject();
        MYDebug("is no clicked");
    }
}

void MainWindow::sltSetMineCounts(int pMineNumber)
{
    ui->mineCount->setText(QString::number(pMineNumber));
}
