#include "videoeditor.h"
#include "ui_videoeditor.h"

VideoEditor::VideoEditor(QWidget *parent) :
    TabPage(parent),
    ui(new Ui::VideoEditor)
{
    ui->setupUi(this);
    InitPara();
    InitUi();
    InitPlayer();
}

VideoEditor::~VideoEditor()
{
    delete ui;
    InitPara();
    ReleasePlayer();
    KillConvertThread();
}
/************************************************************************
//>@Function
************************************************************************/
void VideoEditor::InitPara()
{
    mConvertThread = 0;
    mConvertSetting.mDstDir = D_DEFAULT_SAVE_PATH;
    m_TempPath.clear();
    ClearList();
}

void VideoEditor::InitUi()
{
    InitUiCanvas();
    initUiTreeList();
    initUiParameter();
    LockParameSet();
    UnlockTreeList();
    LockContrlBar();
}

void VideoEditor::InitUiCanvas()
{
    m_Canvas = new Canvas(this);
    m_ScrollArea = new QScrollArea(0);
    m_ScrollArea->setBackgroundRole(QPalette::Dark);
    m_ScrollArea->setWidget(m_Canvas);
    ui->verticalLayout_5->insertWidget(0,m_ScrollArea);
//    QHBoxLayout *tmpHLayout = new QHBoxLayout(0);
//    tmpHLayout->setMargin(0);
//    tmpHLayout->setSpacing(0);
//    tmpHLayout->addWidget(m_Canvas);
//    ui->scrollArea->setLayout(tmpHLayout);
}

void VideoEditor::initUiTreeList()
{
    ui->tree_list->setRootIsDecorated(true);//折叠
    ui->tree_list->setSelectionMode(QAbstractItemView::ExtendedSelection);//多选
    ui->tree_list->setAutoFillBackground(true);
    ui->tree_list->setStyleSheet("QTreeWidget::item{height:18px}");  //格式
    ui->tree_list->setWindowTitle("VedioList");
//    ui->tree_list->setSortingEnabled(true);//排序
//    ui->tree_list->setDragEnabled(true);
    ui->tree_list->header()->setSortIndicatorShown(true);
    ui->tree_list->header()->setSortIndicator(0, Qt::AscendingOrder);
    ui->tree_list->setColumnCount(4);
    ui->tree_list->setColumnWidth(0, 75);
    ui->tree_list->setColumnWidth(1, 75);
    ui->tree_list->setColumnWidth(2, 75);
    ui->tree_list->setColumnWidth(3, 75);
    QList<QString>HeaderLabelList;
    HeaderLabelList.clear();
    HeaderLabelList<<"State"<<"name"<<"progress"<<"Path";
    ui->tree_list->setHeaderLabels(HeaderLabelList);
    connect(ui->tree_list, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(slot_FileInfo(QTreeWidgetItem *, int)));
    connect(ui->tree_list, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(slot_Play(QTreeWidgetItem *, int)));
}

void VideoEditor::initUiParameter()
{
    QStringList tmpParameterList;
    tmpParameterList.clear();
    tmpParameterList<<"mp4";
    ui->v_encoder->addItems(tmpParameterList);
    tmpParameterList.clear();
    tmpParameterList<<"1024x768"<<"768x480"<<"480x320"<<"320x240";
    ui->resolution->addItems(tmpParameterList);
    tmpParameterList.clear();
    tmpParameterList<<"128 kbps"<<"256 kbps"<<"384 kbps"<<"512 kbps"<<"768 kbps"<<"1024 kbps";
    ui->v_bit_rate->addItems(tmpParameterList);
    tmpParameterList.clear();
    tmpParameterList<<"15"<<"20"<<"24"<<"25"<<"30"<<"60";
    ui->frame_rate->addItems(tmpParameterList);

    tmpParameterList.clear();
    tmpParameterList<<"mp3"<<"wav";
    ui->a_encoder->addItems(tmpParameterList);
    tmpParameterList.clear();
    tmpParameterList<<"22050"<<"32000"<<"44100"<<"48000";
    ui->sampe_rate->addItems(tmpParameterList);
    tmpParameterList.clear();
    tmpParameterList<<"48"<<"64"<<"128"<<"160"<<"192"<<"224"<<"320"<<"512";
    ui->a_bit_rate->addItems(tmpParameterList);
    tmpParameterList.clear();
    tmpParameterList<<"1"<<"2";
    ui->channel->addItems(tmpParameterList);
}

void VideoEditor::InitPlayer()
{
    m_Player = new basemedia(this);
}

void VideoEditor::ReleasePlayer()
{
    if(m_Player != 0)
    {
        delete m_Player;
        m_Player = 0;
    }
}

//>@Lock
void VideoEditor::LockParameSet()
{
    ui->groupBox_2->setEnabled(false);
    ui->groupBox_3->setEnabled(false);
}

void VideoEditor::UnlockParameSet()
{
    ui->groupBox_2->setEnabled(true);
    ui->groupBox_3->setEnabled(true);
}

void VideoEditor::LockTreeList()
{
    ui->groupBox_4->setEnabled(false);
}

void VideoEditor::LockContrlBar()
{
    ui->BtnGrabScreen->setEnabled(false);
    ui->BtnWatermark->setEnabled(false);
    ui->BtnText->setEnabled(false);
    ui->BtnDel->setEnabled(false);
    ui->BtnSave->setEnabled(false);

    ui->BtnPre->setEnabled(false);
    ui->BtnSound->setEnabled(false);
    ui->BtnStart->setEnabled(false);
    ui->BtnStop->setEnabled(false);
    ui->BtnNext->setEnabled(false);
    ui->SliderSound->setEnabled(false);
    ui->SliderTimeline->setEnabled(false);
}

void VideoEditor::UnlockContrlBar()
{
    ui->BtnGrabScreen->setEnabled(true);
    ui->BtnWatermark->setEnabled(true);
    ui->BtnText->setEnabled(true);
    ui->BtnDel->setEnabled(true);
    ui->BtnSave->setEnabled(true);

    ui->BtnPre->setEnabled(true);
    ui->BtnSound->setEnabled(true);
    ui->BtnStart->setEnabled(true);
    ui->BtnStop->setEnabled(true);
    ui->BtnNext->setEnabled(true);
    ui->SliderSound->setEnabled(true);
    ui->SliderTimeline->setEnabled(true);
}

void VideoEditor::UnlockTreeList()
{
    ui->groupBox_4->setEnabled(true);
}

void VideoEditor::RefreshTreelist()
{
    ui->tree_list->clear();
    quint8 tmpNum = m_List.count();
    if(tmpNum > 0)
    {
        for(int i=0;i<tmpNum;i++)
        {
            LIST_MEMBER_INFO tmpMember = m_List.at(i);
            QTreeWidgetItem *tmpItem = new QTreeWidgetItem;
            switch(tmpMember.mState)
            {
                case Convert_Wait:
                    tmpItem->setIcon(0,QIcon(":/Menu/rc/Menu/pass.png"));
                    break;
                case Convert_Busy:
                    tmpItem->setIcon(0,QIcon(":/Menu/rc/Menu/debug.png"));
                    break;
                case Convert_Err:
                    tmpItem->setIcon(0,QIcon(":/Menu/rc/Menu/delete.png"));
                    break;
                case Convert_Finished:
                    tmpItem->setIcon(0,QIcon(":/Menu/rc/Menu/success.png"));
                    break;
            }
            tmpItem->setText(1, m_List.at(i).mFileInfo.mName);
            tmpItem->setText(3, m_List.at(i).mFileInfo.mPath);
//            tmpItem->setCheckState(0, Qt::Checked);
            ui->tree_list->addTopLevelItem(tmpItem);
            switch(tmpMember.mState)
            {
                case Convert_Wait:
                {
                    break;
                }
                case Convert_Busy:
                {
                    QColor tmpColor(50,30,0,100);
                    ui->tree_list->topLevelItem(i)->setBackgroundColor(0, tmpColor);
                    ui->tree_list->topLevelItem(i)->setBackgroundColor(1, tmpColor);
                    ui->tree_list->topLevelItem(i)->setBackgroundColor(2, tmpColor);
                    ui->tree_list->topLevelItem(i)->setBackgroundColor(3, tmpColor);
                    break;
                }
                case Convert_Err:
                {
                    QColor tmpColor(100,0,0,100);
                    ui->tree_list->topLevelItem(i)->setBackgroundColor(0, tmpColor);
                    ui->tree_list->topLevelItem(i)->setBackgroundColor(1, tmpColor);
                    ui->tree_list->topLevelItem(i)->setBackgroundColor(2, tmpColor);
                    ui->tree_list->topLevelItem(i)->setBackgroundColor(3, tmpColor);
                    break;
                }
                case Convert_Finished:
                {
                    QColor tmpColor(0,200,0,100);
                    ui->tree_list->topLevelItem(i)->setBackgroundColor(0, tmpColor);
                    ui->tree_list->topLevelItem(i)->setBackgroundColor(1, tmpColor);
                    ui->tree_list->topLevelItem(i)->setBackgroundColor(2, tmpColor);
                    ui->tree_list->topLevelItem(i)->setBackgroundColor(3, tmpColor);
                    break;
                }
            }
            QProgressBar *tmpProgressBar = new QProgressBar(this);
            tmpProgressBar->setMaximumHeight(20);
//            if(tmpMember.mProgress >= 100)
//                tmpMember.mProgress = 100;
            tmpProgressBar->setValue(tmpMember.mProgress);
            ui->tree_list->setItemWidget(tmpItem, 2, tmpProgressBar);
        }
    }
}

void VideoEditor::slot_FileInfo(QTreeWidgetItem *item, int column)
{
    ui->textBrowser->clear();
    quint8 tmpDex = ui->tree_list->indexOfTopLevelItem(item);
    LIST_MEMBER_INFO tmpMember = m_List.at(tmpDex);
    QString tmpString;
    switch(tmpMember.mFileInfo.mType)
    {
        case RC_MP4:
            tmpString = QString("FileType:%1").arg("MP4");
            break;
        case RC_AVI:
            tmpString = QString("FileType:%1").arg("AVI");
            break;
        case RC_RMVB:
            tmpString = QString("FileType:%1").arg("RMVB");
            break;
        case RC_FLV:
            tmpString = QString("FileType:%1").arg("FLV");
            break;
        default:
            tmpString = QString("FileType:%1").arg("INVALID");
            break;
    }
    ui->textBrowser->append(tmpString);
    tmpString = QString("FilePath:%1").arg(tmpMember.mFileInfo.mPath);
    ui->textBrowser->append(tmpString);
    tmpString = QString("FileName:%1").arg(tmpMember.mFileInfo.mName);
    ui->textBrowser->append(tmpString);
    tmpString = QString("FileSize:%1b").arg(tmpMember.mFileInfo.mSize);
    ui->textBrowser->append(tmpString);
    tmpString = QString("FileResolution:%1x%2")
            .arg(tmpMember.mFileInfo.mResolution.width())
            .arg(tmpMember.mFileInfo.mResolution.height());
    ui->textBrowser->append(tmpString);
    tmpString = QString("FileLenth:%1s").arg(tmpMember.mFileInfo.mLenth);
    ui->textBrowser->append(tmpString);
}

void VideoEditor::slot_Play(QTreeWidgetItem *item, int column)
{
    quint8 tmpDex = ui->tree_list->indexOfTopLevelItem(item);
    LIST_MEMBER_INFO tmpMember = m_List.at(tmpDex);
    m_Canvas->resize(tmpMember.mFileInfo.mResolution.width(),
                     tmpMember.mFileInfo.mResolution.height());
    m_TempPath = D_GrabScreenPath;
    //m_Player->VideoToPic(tmpMember.mFileInfo.mPath,m_TempPath,"jpeg");
//    m_Player->SetVideoRect(m_Canvas->rect(), 0);
//    m_Player->Play(tmpMember.mFileInfo.mPath);
}

void VideoEditor::slot_RefreshTreelist()
{
    RefreshTreelist();
}

void VideoEditor::slot_Finished()
{
    ui->start->setEnabled(true);
    ui->stop->setEnabled(false);
    UnlockParameSet();
    UnlockTreeList();
    UnlockContrlBar();
}

bool VideoEditor::GetParameter()
{
    mConvertSetting.mType = ui->v_encoder->currentText();
    QString reslution = ui->resolution->currentText();
    QStringList paraList = reslution.split("x", QString::SkipEmptyParts);
    if(paraList.count() != 2)
        return false;
    mConvertSetting.mReslution = QSize(paraList.at(0).toInt(), paraList.at(1).toInt());
    if(!mConvertSetting.mReslution.isValid())
        return false;
    mConvertSetting.mFrame = ui->frame_rate->currentText().toInt();
    mConvertSetting.mDietime = 600000;
    return true;
}

void VideoEditor::StopConvertThread()
{
    if(mConvertThread != 0)
    {
        if (mConvertThread->isRunning())
        {
            mConvertThread->Stop();
            if (QThread::currentThread() != mConvertThread)
                mConvertThread->wait();
        }
    }
}

void VideoEditor::KillConvertThread()
{
    if(mConvertThread != 0)
    {
        StopConvertThread();
        disconnect(mConvertThread, SIGNAL(sFinished()), 0, 0);
        disconnect(mConvertThread, SIGNAL(sRefreshTreelist()), 0, 0);
        mConvertThread->quit();
        mConvertThread->deleteLater();
        mConvertThread = 0;
    }
}

void VideoEditor::StartConvertThread()
{
    KillConvertThread();
    mConvertThread = new ConvertThread(this);
    connect(mConvertThread, SIGNAL(sFinished()), this, SLOT(slot_Finished()));
    connect(mConvertThread, SIGNAL(sRefreshTreelist()), this, SLOT(slot_RefreshTreelist()));
    mConvertThread->start();
}

void VideoEditor::GetSrcInfo(QStringList pList)
{
    KillConvertThread();
    mConvertThread = new ConvertThread(this);
    connect(mConvertThread, SIGNAL(sFinished()), this, SLOT(slot_Finished()));
    connect(mConvertThread, SIGNAL(sRefreshTreelist()), this, SLOT(slot_RefreshTreelist()));
    mConvertThread->GetFileInfo(pList);
}
/************************************************************************
//>@Conctrl
************************************************************************/
void VideoEditor::on_add_clicked()
{
    QStringList fileList =
        QFileDialog::getOpenFileNames(
                this,
                "Select one or more files to open",
                QDir::currentPath(),
                "Video(*.avi *.mp4 *.flv *.rmvb);;All(*)");
    GetSrcInfo(fileList);
}

void VideoEditor::on_del_clicked()
{
    QList<QTreeWidgetItem*> tmpItemList = ui->tree_list->selectedItems();
    for(int i=0;i<tmpItemList.count();i++)
    {
        DelListMember(ui->tree_list->indexOfTopLevelItem(tmpItemList.at(i))-i);
    }
    RefreshTreelist();
}

void VideoEditor::on_up_clicked()
{
    MoveListMemberPostion(ui->tree_list->indexOfTopLevelItem(ui->tree_list->currentItem()), 1);
}

void VideoEditor::on_down_clicked()
{
    MoveListMemberPostion(ui->tree_list->indexOfTopLevelItem(ui->tree_list->currentItem()), 1);
}

void VideoEditor::on_savelist_clicked()
{
    mConvertSetting.mDstDir =
        QFileDialog::getExistingDirectory(
            this, tr("Open Directory"),
            QDir::currentPath(),
            QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    if(!mConvertSetting.mDstDir.endsWith("/"))
        mConvertSetting.mDstDir.append("/");
}

void VideoEditor::on_start_clicked()
{
    if(!GetParameter())
        return;
    ui->start->setEnabled(false);
    ui->stop->setEnabled(true);
    LockParameSet();
    LockTreeList();
    LockContrlBar();
    StartConvertThread();
}

void VideoEditor::on_stop_clicked()
{
    ui->start->setEnabled(true);
    ui->stop->setEnabled(false);
    StopConvertThread();
}

void VideoEditor::on_BtnGrabScreen_clicked()
{
    m_Canvas->m_CurRectInfo = m_Canvas->m_RectsInfo.value(Canvas_GrabScreen);
    m_Canvas->m_CurRectInfo.mActive = true;
    m_Canvas->RefreshHash();
}

void VideoEditor::on_BtnWatermark_clicked()
{
    m_Canvas->m_CurRectInfo = m_Canvas->m_RectsInfo.value(Canvas_Watermark);
    m_Canvas->m_CurRectInfo.mActive = true;
    m_Canvas->RefreshHash();
}

void VideoEditor::on_BtnText_clicked()
{
    m_Canvas->m_CurRectInfo = m_Canvas->m_RectsInfo.value(Canvas_Text);
    m_Canvas->m_CurRectInfo.mActive = true;
    m_Canvas->RefreshHash();
}

void VideoEditor::on_BtnDel_clicked()
{
    m_Canvas->m_CurRectInfo.mActive = false;
    m_Canvas->RefreshHash();
}

void VideoEditor::on_BtnSave_clicked()
{
//    m_Canvas->RefreshHash();

//    CONVERT_SET tmpConvertSet;
//    GetParameter(tmpConvertSet);
//    CANVAS_RECT_INFO tmpRectInfo;
//    tmpRectInfo = m_Canvas->m_RectsInfo.value(Canvas_GrabScreen);
//    if(tmpRectInfo.mActive)
//    {
//        IDE_TRACE_STR("Canvas_GrabScreen");
//        tmpConvertSet.mVideoPara.mGrabScreen.mEnable = true;
//        tmpConvertSet.mVideoPara.mGrabScreen.mPara = QString("");
//    }
//    tmpRectInfo = m_Canvas->m_RectsInfo.value(Canvas_Watermark);
//    if(tmpRectInfo.mActive)
//    {
//        IDE_TRACE_STR("Canvas_Watermark");
//        tmpConvertSet.mVideoPara.mGrabScreen.mEnable = true;
//        tmpConvertSet.mVideoPara.mGrabScreen.mPara = QString("");
//    }
//    tmpRectInfo = m_Canvas->m_RectsInfo.value(Canvas_Text);
//    if(tmpRectInfo.mActive)
//    {
//        IDE_TRACE_STR("Canvas_Text");
//        tmpConvertSet.mVideoPara.mGrabScreen.mEnable = true;
//        tmpConvertSet.mVideoPara.mGrabScreen.mPara = QString("");
//    }
//    emit sSetParameter(tmpConvertSet);
}

void VideoEditor::on_BtnStart_clicked()
{
    quint8 tmpCnt = 0;
    tmpCnt++;
    if(tmpCnt>1)
        tmpCnt = 0;
//    if(tmpCnt == 1)
//        m_Player->Pause();
//    else
//        m_Player->Resume();
    m_Canvas->repaint(m_Canvas->rect());
}

void VideoEditor::on_BtnStop_clicked()
{
    //m_Player->Stop();
}
/************************************************************************
//>@convert
************************************************************************/
Canvas::Canvas(VideoEditor *pFriend)
{
    m_Friend = pFriend;
//    setWindowFlags(Qt::FramelessWindowHint);
    setAutoFillBackground(false);
//    setAttribute(Qt::WA_DeleteOnClose);
//    setAttribute(Qt::WA_OpaquePaintEvent, true);
//    setAttribute(Qt::WA_NoSystemBackground, true);
//    setAttribute(Qt::WA_TranslucentBackground, true);
//    setWindowOpacity(0.5);
    InitPara();
    InitHash();
    m_BrushTimer.start(30);
    connect(&m_BrushTimer, SIGNAL(timeout()), this, SLOT(slot_Brush()));
}

Canvas::~Canvas()
{

}

void Canvas::InitPara()
{
    m_CurRectInfo.mMouseInPos = QPoint(0,0);
    m_CurRectInfo.mMouseInRect = QRect(0,0,0,0);
    m_CurRectInfo.mType = Canvas_Nothing;
    m_CurRectInfo.mTriger = Rect_UnSelect;
    m_CurRectInfo.mColor = Qt::black;
    m_CurRectInfo.mRect = QRect(0,0,0,0);
}

void Canvas::InitHash()
{
    m_RectsInfo.clear();
    CANVAS_RECT_INFO tmpRectInfo;

    tmpRectInfo.mActive = false;
    tmpRectInfo.mType = Canvas_GrabScreen;
    tmpRectInfo.mTriger = Rect_UnSelect;
    tmpRectInfo.mColor = Qt::darkRed;
    tmpRectInfo.mRect = QRect(50,50,80,60);
    m_RectsInfo.insert(Canvas_GrabScreen, tmpRectInfo);

    tmpRectInfo.mActive = false;
    tmpRectInfo.mType = Canvas_Watermark;
    tmpRectInfo.mTriger = Rect_UnSelect;
    tmpRectInfo.mColor = Qt::darkGreen;
    tmpRectInfo.mRect = QRect(50,50,80,60);
    m_RectsInfo.insert(Canvas_Watermark, tmpRectInfo);

    tmpRectInfo.mActive = false;
    tmpRectInfo.mType = Canvas_Text;
    tmpRectInfo.mTriger = Rect_UnSelect;
    tmpRectInfo.mColor = Qt::darkBlue;
    tmpRectInfo.mRect = QRect(50,50,80,60);
    m_RectsInfo.insert(Canvas_Text, tmpRectInfo);
}

void Canvas::RefreshHash()
{
    switch(m_CurRectInfo.mType)
    {
        case Canvas_Nothing:
            return;
        case Canvas_GrabScreen:
            m_RectsInfo.remove(Canvas_GrabScreen);
            m_RectsInfo.insert(Canvas_GrabScreen, m_CurRectInfo);
            break;
        case Canvas_Watermark:
            m_RectsInfo.remove(Canvas_Watermark);
            m_RectsInfo.insert(Canvas_Watermark, m_CurRectInfo);
            break;
        case Canvas_Text:
            m_RectsInfo.remove(Canvas_Text);
            m_RectsInfo.insert(Canvas_Text, m_CurRectInfo);
            break;
    }
}

void Canvas::paintEvent( QPaintEvent * event)
{
    QPainter p(this);
    QFont font("Arial",20,QFont::Black,true);
    font.setUnderline(false);
    font.setOverline(false);
    font.setCapitalization(QFont::SmallCaps);
    font.setLetterSpacing(QFont::AbsoluteSpacing,5);
    p.setFont(font);
    p.drawPixmap(this->rect(), m_Pixmap, this->rect());

    QString tmpString;
    qreal tmpOpacity = 0.3;
    int tmpleft = m_CurRectInfo.mRect.left();
    int tmptop = m_CurRectInfo.mRect.top();
    int tmpwidth = m_CurRectInfo.mRect.width();
    int tmpheight = m_CurRectInfo.mRect.height();
    int tmpright = m_CurRectInfo.mRect.right();
    int tmpbottom = m_CurRectInfo.mRect.bottom();
    if(tmpwidth < 20)
        tmpwidth = 20;
    if(tmpheight < 20)
        tmpheight = 20;
    m_CurRectInfo.mRect = QRect(tmpleft, tmptop, tmpwidth, tmpheight);
    switch(m_CurRectInfo.mType)
    {
        case Canvas_Nothing:
            return;
        case Canvas_GrabScreen:
            tmpString = QString("GrabScreen");
            break;
        case Canvas_Watermark:
            tmpString = QString("Watermark");
            break;
        case Canvas_Text:
            tmpString = QString("Text");
            break;
    }
    if(!m_CurRectInfo.mActive)
        return;
    if(m_CurRectInfo.mTriger > 0)
    {
        tmpOpacity = 1.0;
        p.setOpacity(tmpOpacity);
        p.save();
        p.setPen(QPen(m_CurRectInfo.mColor, 10, Qt::SolidLine, Qt::SquareCap));
        p.drawPoint(tmpleft,tmptop);
        p.drawPoint(tmpleft,tmpbottom);
        p.drawPoint(tmpright,tmpbottom);
        p.drawPoint(tmpright,tmptop);
        p.drawPoint(tmpleft,tmptop+tmpheight/2);
        p.drawPoint(tmpleft+tmpwidth/2,tmpbottom);
        p.drawPoint(tmpright,tmptop+tmpheight/2);
        p.drawPoint(tmpleft+tmpwidth/2,tmptop);
        p.restore();
    }
    p.setOpacity(tmpOpacity);
    p.setPen(QPen(m_CurRectInfo.mColor, 3, Qt::DashLine, Qt::SquareCap));
    p.drawRect(m_CurRectInfo.mRect);
    p.setPen(QPen(m_CurRectInfo.mColor, 3, Qt::SolidLine, Qt::SquareCap));
    p.drawText(m_CurRectInfo.mRect, Qt::AlignCenter,tmpString);
}

void Canvas::GetBackground(QString pPath)
{
    QDir tmpDir(pPath);
    if(!tmpDir.exists())
        return;
    quint32 tmpCnt = 0;
    QString tmpSring;
    tmpSring.clear();
    QFileInfoList tmpFileInfoList;
    tmpFileInfoList.clear();
    tmpFileInfoList = tmpDir.entryInfoList(QDir::Files);
    tmpCnt = tmpFileInfoList.count();
    for(int i=0;i<tmpCnt;i++)
    {
        if(tmpFileInfoList.at(i).fileName() == "." || tmpFileInfoList.at(i).fileName() == "..")
            continue;
        tmpSring = tmpFileInfoList.at(i).filePath();
        if(i == tmpCnt-1)
            m_Pixmap.load(tmpSring, 0, Qt::OrderedDither);
        QFile tmpFile(tmpSring);
        if(!tmpFile.exists())
            continue;
        if(!tmpFile.setPermissions(QFile::WriteOther))
            continue;
        if(!tmpFile.remove())
            continue;
    }
}

void Canvas::GetRectTrigerState(QPoint pPos)
{
    if(m_CurRectInfo.mType == Canvas_Nothing)
        return;
    m_CurRectInfo.mMouseInPos = pPos;
    m_CurRectInfo.mMouseInRect = m_CurRectInfo.mRect;
    int tmpX = pPos.rx();
    int tmpY = pPos.ry();
    int tmpleft = m_CurRectInfo.mRect.left();
    int tmptop = m_CurRectInfo.mRect.top();
    int tmpright = m_CurRectInfo.mRect.right();
    int tmpbottom = m_CurRectInfo.mRect.bottom();

    if((tmpX < tmpleft)||(tmpX >tmpright)||
       (tmpY < tmptop)||(tmpY >tmpbottom))
        m_CurRectInfo.mTriger = Rect_UnSelect;
    else
        m_CurRectInfo.mTriger = Rect_Select;

    if((tmpX > tmpleft-D_SENSITIVE)&&(tmpX < tmpleft+D_SENSITIVE)&&
       (tmpY > tmptop-D_SENSITIVE)&&(tmpY < tmptop+D_SENSITIVE))
       m_CurRectInfo.mTriger = Rect_LeftTOP;
    else if((tmpX > tmpleft-D_SENSITIVE)&&(tmpX < tmpleft+D_SENSITIVE)&&
            (tmpY > tmpbottom-D_SENSITIVE)&&(tmpY < tmpbottom+D_SENSITIVE))
       m_CurRectInfo.mTriger = Rect_LeftBottom;
    else if((tmpX < tmpright+D_SENSITIVE)&&(tmpX > tmpright-D_SENSITIVE)&&
            (tmpY > tmpbottom-D_SENSITIVE)&&(tmpY < tmpbottom+D_SENSITIVE))
       m_CurRectInfo.mTriger = Rect_RightBottom;
    else if((tmpX < tmpright+D_SENSITIVE)&&(tmpX > tmpright-D_SENSITIVE)&&
            (tmpY > tmptop+D_SENSITIVE)&&(tmpY < tmptop+D_SENSITIVE))
       m_CurRectInfo.mTriger = Rect_RightTOP;
    else if((tmpX > tmpleft-D_SENSITIVE)&&(tmpX < tmpleft+D_SENSITIVE)&&
            (tmpY > tmptop+D_SENSITIVE)&&(tmpY < tmpbottom-D_SENSITIVE))
       m_CurRectInfo.mTriger = Rect_Left;
    else if((tmpX > tmpleft+D_SENSITIVE)&&(tmpX < tmpright-D_SENSITIVE)&&
            (tmpY > tmpbottom-D_SENSITIVE)&&(tmpY < tmpbottom+D_SENSITIVE))
       m_CurRectInfo.mTriger = Rect_Bottom;
    else if((tmpX > tmpright-D_SENSITIVE)&&(tmpX < tmpright+D_SENSITIVE)&&
            (tmpY > tmptop+D_SENSITIVE)&&(tmpY < tmpbottom-D_SENSITIVE))
       m_CurRectInfo.mTriger = Rect_Right;
    else if((tmpX > tmpleft+D_SENSITIVE)&&(tmpX < tmpright-D_SENSITIVE)&&
            (tmpY > tmptop-D_SENSITIVE)&&(tmpY < tmptop+D_SENSITIVE))
       m_CurRectInfo.mTriger = Rect_TOP;
}

void Canvas::RectChangeFllowPos(QPoint pPos)
{
    int tmpXMove = pPos.rx() - m_CurRectInfo.mMouseInPos.rx();
    int tmpYMove = pPos.ry() - m_CurRectInfo.mMouseInPos.ry();
    int tmpleft = m_CurRectInfo.mMouseInRect.left();
    int tmptop = m_CurRectInfo.mMouseInRect.top();
    int tmpwidth = m_CurRectInfo.mMouseInRect.width();
    int tmpheight = m_CurRectInfo.mMouseInRect.height();
//    int tmpright = m_CurRectInfo.mMouseInRect.right();
//    int tmpbottom = m_CurRectInfo.mMouseInRect.bottom();
    switch(m_CurRectInfo.mTriger)
    {
        case Rect_UnSelect:
            return;
        case Rect_Select:
        {
            m_CurRectInfo.mRect = QRect(tmpleft+tmpXMove, tmptop+tmpYMove, tmpwidth, tmpheight);
            break;
        }
        case Rect_LeftTOP:
        {
            m_CurRectInfo.mRect = QRect(tmpleft+tmpXMove, tmptop+tmpYMove, tmpwidth-tmpXMove, tmpheight-tmpYMove);
            break;
        }
        case Rect_LeftBottom:
        {
            m_CurRectInfo.mRect = QRect(tmpleft+tmpXMove, tmptop, tmpwidth-tmpXMove, tmpheight+tmpYMove);
            break;
        }
        case Rect_RightBottom:
        {
            m_CurRectInfo.mRect = QRect(tmpleft, tmptop, tmpwidth+tmpXMove, tmpheight+tmpYMove);
            break;
        }
        case Rect_RightTOP:
        {
            m_CurRectInfo.mRect = QRect(tmpleft, tmptop+tmpYMove, tmpwidth+tmpXMove, tmpheight-tmpYMove);
            break;
        }
        case Rect_Left:
        {
            m_CurRectInfo.mRect = QRect(tmpleft+tmpXMove, tmptop, tmpwidth-tmpXMove, tmpheight);
            break;
        }
        case Rect_Bottom:
        {
            m_CurRectInfo.mRect = QRect(tmpleft, tmptop, tmpwidth, tmpheight+tmpYMove);
            break;
        }
        case Rect_Right:
        {
            m_CurRectInfo.mRect = QRect(tmpleft, tmptop, tmpwidth+tmpXMove, tmpheight);
            break;
        }
        case Rect_TOP:
        {
            m_CurRectInfo.mRect = QRect(tmpleft, tmptop+tmpYMove, tmpwidth, tmpheight-tmpYMove);
            break;
        }
    }
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    QPoint tmpPos = event->pos();
    GetRectTrigerState(tmpPos);
//    if(event = QMouseEvent::)
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    QPoint tmpPos = event->pos();
    RectChangeFllowPos(tmpPos);
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    RefreshHash();
}

void Canvas::mouseDoubleClickEvent(QMouseEvent *event)
{

}

void Canvas::slot_Brush()
{
    GetBackground(D_GrabScreenPath);
    repaint();
}

