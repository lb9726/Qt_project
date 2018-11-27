#include "menuitem.h"
#include "ui_menuitem.h"
#include <QPainter>
#include <QDebug>

MenuItem::MenuItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MenuItem)
{
    ui->setupUi(this);

}

MenuItem::~MenuItem()
{
    delete ui;
}

void MenuItem::SetName(QString pName)
{
    ui->label_name->setText(pName);
}

void MenuItem::InitList(int pCur, int pMin, int pMax)
{
    mCur = pCur;
    mMin = pMin;
    mMax = pMax;
    mType = 0;

    ui->label_value->setText(QString::number(mCur));
}

void MenuItem::InitList(int pCur, QStringList pImgList)
{
    mImgList = pImgList;
    if (pCur < 0)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"pCur is less 0 = "<<pCur;
        return;
    }
    mImgIndex = pCur;
    mType = 1;
    if(mImgIndex < mImgList.count())
    {
        QString tmpPic = mImgList.at(mImgIndex);
        QPixmap tmpPix = QPixmap(tmpPic);
        ui->label_value->setPixmap(tmpPix.scaled(ui->label_value->size(), Qt::KeepAspectRatio));
    }
}

void MenuItem::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    mPixmap = QPixmap(":/SetMenu/rc/SetMenu/setitem_green.png");
    update();
    emit sSelected();
}

void MenuItem::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    update();
}

void MenuItem::SetPressed(bool pFlag)
{
    if(pFlag)
    {
        mPixmap = QPixmap(":/SetMenu/rc/SetMenu/setitem_green.png");
    }
    else if(!pFlag)
    {
        mPixmap = QPixmap();
    }
    update();
}

void MenuItem::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.drawPixmap(rect(),mPixmap);
}

int setValue(int minValue, int maxValue, int pValue)
{
    if(pValue <= minValue)
    {
        pValue = minValue;
        return pValue;
    }
    else if(pValue >= maxValue)
    {
        pValue = maxValue;
        return pValue;
    }
    else
    {
        return pValue;
    }
}

void MenuItem::on_btn_left_clicked()
{
    if(mPixmap.isNull())
    {
        mPixmap = QPixmap(":/SetMenu/rc/SetMenu/setitem_green.png");
        update();
        emit sSelected();
    }

    if(mType == 0) //数字如音量
    {
        int tmpValue = ui->label_value->text().toInt();
        tmpValue = tmpValue - 10;
        mCur = setValue(mMin, mMax, tmpValue);
        QString tmpText = QString::number(mCur);
        if(ui->label_value->text().compare(tmpText))
        {
            ui->label_value->setText(tmpText);
            emit sDataChanged(tmpText);
        }
    }
    else if(mType == 1)  //字母
    {
        if(mImgIndex > 0)
        {
            --mImgIndex;
            QString tmpPic = mImgList.at(mImgIndex);
            ui->label_value->setPixmap(QPixmap(tmpPic).scaled(ui->label_value->size(), Qt::KeepAspectRatio));
            emit sDataChanged(tmpPic);
        }
    }
}

void MenuItem::on_btn_right_clicked()
{
    if(mPixmap.isNull())
    {
        mPixmap = QPixmap(":/SetMenu/rc/SetMenu/setitem_green.png");
        update();
        emit sSelected();
    }

    if(mType == 0)
    {
        int tmpValue = ui->label_value->text().toInt();
        tmpValue = tmpValue + 10;
        mCur = setValue(mMin, mMax, tmpValue);
        QString tmpText = QString::number(mCur);
        if(ui->label_value->text().compare(tmpText))
        {
            ui->label_value->setText(tmpText);
            emit sDataChanged(tmpText);
        }
    }
    else if(mType == 1)
    {
        if(mImgIndex < mImgList.count() - 1)
        {
            mImgIndex++;
            QString tmpText = mImgList.at(mImgIndex);
            ui->label_value->setPixmap(QPixmap(tmpText).scaled(ui->label_value->size(), Qt::KeepAspectRatio));
            emit sDataChanged(tmpText);
        }
    }
}
