#include "screen.h"
#include "ui_screen.h"

Screen::Screen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Screen)
{
    ui->setupUi(this);
    mRunPath = QCoreApplication::applicationDirPath();
    if (!mRunPath.endsWith("/"))
    {
        mRunPath.append("/");
    }
    mDataAggregation.readDataFile(mRunPath + "000-userData/data.txt");
    mDataAggregation.printQList();
    mDataAggregation.getUserNumbers();
    mDataAggregation.generateBigPrime();
    mDataAggregation.checkBigPrime();
    mDataAggregation.generatePublicKeyAndPrivateKeyAndEncryptionKey();
    mDataAggregation.checkPrivateKey();
    mDataAggregation.checkPubKey();
    mDataAggregation.encryptionFileData();
}

Screen::~Screen()
{
    delete ui;
}
