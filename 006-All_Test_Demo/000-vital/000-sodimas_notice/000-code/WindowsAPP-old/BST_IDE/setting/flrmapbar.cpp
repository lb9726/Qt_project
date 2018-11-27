#include "flrmapbar.h"
#include "ui_flrmapbar.h"
#include "deviceinc.h"

FlrmapBar::FlrmapBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FlrmapBar)
{
    m_DeviceManager = VAR_CAST<DeviceBase *>(parent);
    ui->setupUi(this);

    ui->edit_audio->setInputMask("000");

    ui->stacked_mapping->setCurrentIndex(0);

    connect(ui->edit_level, SIGNAL(sActive()), this, SIGNAL(sActive()));
    connect(ui->edit_mapping, SIGNAL(sActive()), this, SIGNAL(sActive()));
    connect(ui->edit_audio, SIGNAL(sActive()), this, SIGNAL(sActive()));

    connect(ui->edit_level, SIGNAL(textChanged(QString)), this, SLOT(slot_ValidLevel(QString)));
    connect(ui->edit_mapping, SIGNAL(textChanged(QString)), this, SLOT(slot_Preview(QString)));

    QTimer::singleShot(100, this, SLOT(slot_Resize()));
}

FlrmapBar::~FlrmapBar()
{
    delete ui;
}

void FlrmapBar::ClearInputFocus()
{
    ui->edit_level->clearFocus();
    ui->edit_mapping->clearFocus();
    ui->edit_audio->clearFocus();
}

void FlrmapBar::slot_ChangeRcs()
{
    slot_Preview(ui->edit_mapping->text());
}

void FlrmapBar::slot_ValidLevel(QString pText)
{
    if(pText.count() > 3)
        ui->edit_level->setText(pText.mid(0, 3));
}

void FlrmapBar::slot_Preview(QString pText)
{
    if(!m_DeviceManager)
        return;
    if(pText.isEmpty())
    {
        ui->label_single->setPixmap(m_DeviceManager->m_TranparentPix);
        ui->label_ten->setPixmap(m_DeviceManager->m_TranparentPix);
        ui->label_hundred->setPixmap(m_DeviceManager->m_TranparentPix);
    }
    int minWidth = (qreal)(ui->preview->width())/pText.count();
    int minHeight = ui->preview->height();
    if(pText.count() == 1)
    {
        ui->label_single->setGeometry(QRect(0,0,minWidth,minHeight));
        if(!m_DeviceManager->m_FloorPixs.isEmpty())
            ui->label_single->setPixmap(m_DeviceManager->m_FloorPixs.value((int)(pText.toLatin1().at(0))));
        ui->label_ten->setPixmap(m_DeviceManager->m_TranparentPix);
        ui->label_hundred->setPixmap(m_DeviceManager->m_TranparentPix);
    }
    else if(pText.count() == 2)
    {
        ui->label_single->setGeometry(QRect(minWidth,0,minWidth,minHeight));
        ui->label_ten->setGeometry(QRect(0,0,minWidth,minHeight));
        if(!m_DeviceManager->m_FloorPixs.isEmpty())
        {
            ui->label_single->setPixmap(m_DeviceManager->m_FloorPixs.value((int)(pText.toLatin1().at(1))));
            ui->label_ten->setPixmap(m_DeviceManager->m_FloorPixs.value((int)(pText.toLatin1().at(0))));
        }
        ui->label_hundred->setPixmap(m_DeviceManager->m_TranparentPix);
    }
    else if(pText.count() == 3)
    {
        ui->label_single->setGeometry(QRect(minWidth*2,0,minWidth,minHeight));
        ui->label_ten->setGeometry(QRect(minWidth,0,minWidth,minHeight));
        ui->label_hundred->setGeometry(QRect(0,0,minWidth,minHeight));
        if(!m_DeviceManager->m_FloorPixs.isEmpty())
        {
            ui->label_single->setPixmap(m_DeviceManager->m_FloorPixs.value((int)(pText.toLatin1().at(2))));
            ui->label_ten->setPixmap(m_DeviceManager->m_FloorPixs.value((int)(pText.toLatin1().at(1))));
            ui->label_hundred->setPixmap(m_DeviceManager->m_FloorPixs.value((int)(pText.toLatin1().at(0))));
        }
    }
    else
    {
        ui->edit_mapping->setText(pText.mid(0, 3));
    }
}

void FlrmapBar::SetCanDel(bool pFlag)
{
    if(pFlag)
        ui->btn_del->show();
    else
        ui->btn_del->hide();
}

void FlrmapBar::SetShowPreview(bool pFlag)
{
    if(pFlag)
        ui->preview->show();
    else
        ui->preview->hide();
}

bool FlrmapBar::isValid()
{
    if(ui->edit_level->text().isEmpty())
        return false;
    if(ui->edit_mapping->text().isEmpty())
        return false;
//    if(ui->edit_audio->text().isEmpty())
//        return false;
    return true;
}

//>@323248
bool FlrmapBar::SetMap(RC_MAP pFloorMap, RC_MAP pAudioMap)
{
    if(pFloorMap.mSrc.isEmpty() || pFloorMap.mMap.isEmpty() || pAudioMap.mMap.isEmpty())
    {
        IDE_TRACE();
        return false;
    }
    if(pFloorMap.mSrc.count() % 2)
    {
        IDE_TRACE();
        return false;
    }
    if(pFloorMap.mMap.count() % 2)
    {
        IDE_TRACE();
        return false;
    }
    QString tmpLevel;
    int index = 0;
    for(;index<pFloorMap.mSrc.count();)
    {
        bool ok = false;
        char tmpChar = (char)(pFloorMap.mSrc.mid(index, 2).toInt(&ok, 10));
        if(!ok)
        {
            IDE_TRACE();
            return false;
        }
        if((tmpChar >= ' ') && (tmpChar <= '`'))
            tmpLevel.append(QChar(tmpChar));
        else
        {
            IDE_TRACE();
            return false;
        }
        index+=2;
    }
    QString tmpMapping;
    if(pFloorMap.mMap.count() == 6)
    {
        index = 0;
        for(;index<pFloorMap.mMap.count();)
        {
            bool ok = false;
            char tmpChar = (char)(pFloorMap.mMap.mid(index, 2).toInt(&ok, 10));
            if(!ok)
            {
                IDE_TRACE();
                return false;
            }
            if((tmpChar >= ' ') && (tmpChar <= '`'))
                tmpMapping.append(QChar(tmpChar));
            else
            {
                IDE_TRACE();
                return false;
            }
            index+=2;
        }
        ui->stacked_mapping->setCurrentIndex(0);
        ui->edit_mapping->setText(tmpMapping);
    }
    else if(pFloorMap.mMap.count() == 2)
    {
        ui->stacked_mapping->setCurrentIndex(1);
        int tmpValue = pFloorMap.mMap.toInt();
        QString tmpFile = m_DeviceManager->mUiManager->m_FileDir + QString("Floor/Resource/Image/T%1.png").arg(tmpValue);
        ui->label_mapping->setPixmap(QPixmap(tmpFile));
        ui->label_mapping->setProperty("num_string", pFloorMap.mMap);
    }
    ui->edit_level->setText(tmpLevel);
    ui->edit_audio->setText(pAudioMap.mMap);
    return true;
}

QString NumToAscii(int pNum)
{
    int tmpHunderd = pNum / 100;
    int tmpTen = pNum % 100 / 10;
    int tmpSingle = pNum % 10;
    if(tmpHunderd > 0 && tmpHunderd <= 9) //>@百位如果为0，则使用空格替代
        tmpHunderd += '0';
    else if(tmpHunderd >= 'A' && tmpHunderd <= 'Z')
        tmpHunderd += 'A';
    else
        tmpHunderd = ' ';
    if(tmpTen == 0)
    {
        if(tmpHunderd == ' ')
            tmpTen = ' ';
        else
            tmpTen = '0';
    }
    else if(tmpTen > 0 && tmpTen <= 9)
        tmpTen += '0';
    else if(tmpTen >= 'A' && tmpTen <= 'Z')
        tmpTen += 'A';
    else
        tmpTen = 32;
    if(tmpSingle >= 0 && tmpSingle <= 9)
        tmpSingle += '0';
    else if(tmpSingle >= 'A' && tmpSingle <= 'Z')
        tmpSingle += 'A';
    else
        tmpSingle = 32;
    return QString("%1%2%3").arg(tmpHunderd).arg(tmpTen).arg(tmpSingle);
}

QString StrToAscii(QString pStr)
{
    QString tmpLevel;
    for(int i=0;i<pStr.count();i++)
    {
        char tmpChar = pStr.at(i).toLatin1();
        if((tmpChar >= ' ') && (tmpChar <= '`'))
            tmpLevel.append(QString("%1").arg((int)tmpChar));//>@如果不用int强制转换tmochar，则%1会是tmpchar字符，而不是数字。
        else
            return QString();
    }
    return tmpLevel;
}

QString FlrmapBar::GetLevle()
{
    QString tmpValue = ui->edit_level->text();
    return StrToAscii(tmpValue);
}

QString FlrmapBar::GetMapping()
{
    QString tmpValue;
    if(ui->stacked_mapping->currentIndex() == 0)
    {
        QString tmpMapping = ui->edit_mapping->text();
        bool flag = false;
        while(tmpMapping.count() < 3)
        {
            tmpMapping.prepend(" ");
            flag = true;
        }
        if(flag)
            ui->edit_mapping->setText(tmpMapping);
        tmpValue = StrToAscii(tmpMapping);
    }
    else
        tmpValue = ui->label_mapping->property("num_string").toString();
    if(tmpValue.count() % 2)
        tmpValue.prepend("0");
    return tmpValue;
}

bool FlrmapBar::GetMap(RC_MAP &pFloorMap, RC_MAP &pAudioMap)
{
    pFloorMap.mSrc = GetLevle();
    pFloorMap.mMap = GetMapping();
    pAudioMap.mSrc = QString::number(ui->edit_level->text().toInt());
    pAudioMap.mMap = ui->edit_audio->text();
    if(pFloorMap.mSrc.isEmpty() || pFloorMap.mMap.isEmpty() ||
       pAudioMap.mSrc.isEmpty() || pAudioMap.mMap.isEmpty())
        return false;
    return true;
}

void FlrmapBar::on_btn_level_clicked()
{
    ui->edit_level->setText("000");
    emit sActive();
}

void FlrmapBar::on_btn_mapping_clicked()
{
    QString tmpDir = m_DeviceManager->mUiManager->m_FileDir + QString("Floor/Resource/Image/");
    QString tmpSelectFile = QFileDialog::getOpenFileName(this, tr("Select images"), tmpDir, "Images (*.png)");
    if(tmpSelectFile.isEmpty())
        return;
    QString tmpFile;
    if(!tmpSelectFile.contains(tmpDir))
    {
        MsgBox tmpMsgBox(tr("The file isn't in current dir,\n"
                            "Do you want to use it!"), true, LEVEL_QUESTION);
        int ret = tmpMsgBox.exec();
        if(ret == QDialog::Rejected)
            return;
        TextInput tmpInput;
        ret = tmpInput.exec();
        if(ret == QDialog::Rejected)
            return;
        QString tmpInputStr = tmpInput.GetInput();
        if(tmpInputStr.isEmpty())
            return;
        //>@拷贝文件
        tmpFile = m_DeviceManager->mUiManager->m_FileDir + QString("Floor/Resource/Image/T%1.png").arg(tmpInputStr);
        CopyFile(tmpSelectFile, tmpFile);
    }
    else
    {
        tmpFile = tmpSelectFile;
    }
    QString tmpName = QFileInfo(tmpFile).fileName();
    int index = tmpName.indexOf('.');
    if(index < 0)
    {
        MsgBox tmpMsgBox(tr("The file is invalid,\n"
                            "Please reselect!"));
        tmpMsgBox.exec();
        return;
    }
    QString tmpNumString = tmpName.mid(1, index-1);
    ui->label_mapping->setPixmap(QPixmap(tmpFile));
    ui->label_mapping->setProperty("num_string", tmpNumString);
    ui->stacked_mapping->setCurrentIndex(1);
    emit sActive();
}

void FlrmapBar::on_btn_audio_clicked()
{
    QString tmpDir = m_DeviceManager->mUiManager->m_FileDir + QString("Media/Resource/Audio/");
    QString tmpFile = QFileDialog::getOpenFileName(this, tr("Select audios"), tmpDir, "Audios (*.wav)");
    if(tmpFile.isEmpty())
        return;
    if(!tmpFile.contains(tmpDir))
    {
        MsgBox tmpMsgBox(tr("The file isn't in current dir,\n"
                            "Please reselect!"));
        tmpMsgBox.exec();
        return;
    }
    QString tmpName = QFileInfo(tmpFile).fileName();
    int tmpKey = GetKey(tmpName, QString("M"));
    ui->edit_audio->setText(QString("%1").arg(tmpKey));
    emit sActive();
}

void FlrmapBar::on_btn_del_clicked()
{
    emit sDelete();
}

void FlrmapBar::on_btn_audtest_clicked()
{
    QString tmpAudNum = ui->edit_audio->text();
    QString tmpFile = m_DeviceManager->mUiManager->m_FileDir + QString("Media/Resource/Audio/M%1.wav").arg(tmpAudNum);

    m_DeviceManager->m_MediaPlayer->Add(AO_BEEP, tmpFile, false, false, 100);
}

void FlrmapBar::on_btn_switch_clicked()
{
    ui->stacked_mapping->setCurrentIndex(0);
    if(ui->edit_mapping->text().isEmpty())
        ui->edit_mapping->setText("   ");
}

void FlrmapBar::slot_Resize()
{
#ifdef QT5
    //>@更改DIP
    //>@计算比例：pixelSize = DPI * pointSize/72
    QScreen *screen = qApp->primaryScreen();
    qreal tmpDpi = screen->logicalDotsPerInch();
    qreal tmpPtDiff = ((qreal)(90) / (qreal)(tmpDpi));
    qreal tmpPxDiff = 1.0 / tmpPtDiff;

    QList<QLayout*>tmpLayoutList = findChildren<QLayout*>();
    QLayout *tmpLayout = 0;
    foreach(tmpLayout, tmpLayoutList)
    {
        if(tmpLayout)
        {
            int space = tmpLayout->spacing();
            if(space > 0)
                tmpLayout->setSpacing(space*tmpPxDiff);
            int left, top, right, bottom;
            tmpLayout->getContentsMargins(&left, &top, &right, &bottom);
            if(left > 0)
                left *= tmpPxDiff;
            if(top > 0)
                top *= tmpPxDiff;
            if(right > 0)
                right *= tmpPxDiff;
            if(bottom > 0)
                bottom *= tmpPxDiff;
            tmpLayout->setContentsMargins(left, top, right, bottom);
        }
    }

    //>@修改Stylesheet中的字体
    QString tmpStyleSheet = styleSheet();
    int index = 0;
    while(1)
    {
        int index1 = tmpStyleSheet.indexOf("pt", index);
        if(index1 < 0)
            break;
        int index2 = tmpStyleSheet.lastIndexOf(" ", index1);
        if(index2 < 0)
            continue;
        int tmpindex = index2 + 1;
        QString tmpSizeStr = tmpStyleSheet.mid(tmpindex, index1-tmpindex);
        int tmpSize = tmpSizeStr.toInt();
        tmpSize*=tmpPtDiff;
        tmpStyleSheet.replace(tmpindex, index1-tmpindex, QString::number(tmpSize));

        index = index1 + 2;
    }
    index = 0;
    while(1)
    {
        int index1 = tmpStyleSheet.indexOf("px", index);
        if(index1 < 0)
            break;
        int index2 = tmpStyleSheet.lastIndexOf(" ", index1);
        if(index2 < 0)
            continue;
        int tmpindex = index2 + 1;
        QString tmpSizeStr = tmpStyleSheet.mid(tmpindex, index1-tmpindex);
        int tmpSize = tmpSizeStr.toInt();
        tmpSize*=tmpPxDiff;
        tmpStyleSheet.replace(tmpindex, index1-tmpindex, QString::number(tmpSize));

        index = index1 + 2;
    }
    setStyleSheet(tmpStyleSheet);
#endif
}

