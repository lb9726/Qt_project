#include "rclistwidget.h"
#include "deviceinc.h"
#include "../BST_IDE/setting/imageselectdialog.h"

RcListWidget::RcListWidget(QWidget *parent):
    QListWidget(parent)
{
    m_MenuOnItem = 0;
    m_StartDragX = 0;
    setResizeMode(Adjust);
    setViewMode(IconMode);
    setFlow(LeftToRight);
    setWrapping(false);  //>@用于使图标横向显示
    setLayoutMode(SinglePass);
    setMovement(Snap);
    setAutoScroll(true);
    setAutoScrollMargin(20);
    setDragEnabled(true);
    setSpacing(10);
    setTextElideMode(Qt::ElideMiddle);

    /*Add*/
    m_BlankMenu = new QMenu;
    //m_BlankMenu->addAction(QIcon(":/icon/rc/icon/add.png"), D_MENU_ADD, this, SLOT(slot_AddIcon()));
    /*Change/Delete*/
    m_ItemMenu = new QMenu;
    m_ItemMenu->addAction(QIcon(":/icon/rc/icon/fresh.png"), D_MENU_UPDATE, this, SLOT(slot_UpdateIcon()));
    //m_ItemMenu->addAction(QIcon(":/icon/rc/icon/minus.png"), D_MENU_DELETE, this, SLOT(slot_DelIcon()));
    m_ItemMenu->addAction(QIcon(":/icon/rc/icon/grid.png"), D_MENU_EXPLORER, this, SLOT(slot_Explore()));

    connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(slot_ItemDoubleClicked(QListWidgetItem*)));
    connect(this, SIGNAL(itemPressed(QListWidgetItem *)), this, SLOT(slot_ItemPressed(QListWidgetItem *)));
}

RcListWidget::~RcListWidget()
{
    m_BlankMenu->deleteLater();
    m_ItemMenu->deleteLater();
}

void RcListWidget::InitForm(DeviceManager *pForm)
{
    mQesp = VAR_CAST<DeviceManager*>(pForm);
}

bool RcListWidget::InitThemeIconList(DeviceManager *pForm, QString pComName)
{
    mQesp = VAR_CAST<DeviceManager*>(pForm);
    if(!mQesp)
        return false;
    if(!(pForm && !pComName.isEmpty()))
        return false;
    mQesp->mNetTransFlag = false;
    m_ComName = pComName;
    QString tmpPath = D_FSRCPATH + D_TEMP_PATH;
    CreatPath(tmpPath);
    pForm->m_LanRouter->Get(QString("<Theme><%1>rclist</%1></Theme>").arg(pComName));
    if(QtWait(pForm->mNetTransFlag, 3000))
    {
        xmlParse *tmpSetParse = new xmlParse;
        if(tmpSetParse->setContent(pForm->mRetContent))
        {
            QDomElement tmpElement = tmpSetParse->documentElement();
            if(!tmpElement.isNull())
            {
                QStringList tmpRcList = tmpElement.text().split(';');
                if(!tmpRcList.isEmpty())
                {
                    m_ComSrcPath = tmpRcList.first();
                    int index = m_ComSrcPath.indexOf(QString("%1/").arg(pComName));
                    m_ComRelPath = m_ComSrcPath.mid(index);
                    for(int i=1;i<tmpRcList.count();i++)
                    {
                        QString tmpName = tmpRcList.at(i);
                        QString tmpSrcFile = m_ComSrcPath + tmpName;
                        QString tmpDstFile = tmpPath + m_ComRelPath + tmpName;
                        bool flag = true;
//                        if(!QFile::exists(tmpDstFile))
//                        {
//                            flag = pForm->mNetManager->GetFile("Y", tmpSrcFile, tmpDstFile, 5000);
//                            QtSleep(100);
//                            if(!flag)
//                                flag = pForm->mNetManager->GetFile("Y", tmpSrcFile, tmpDstFile, 10000);
//                        }
                        if(flag)
                            AddIcon(tmpDstFile);
                    }
                }
            }
        }
        tmpSetParse->deleteLater();
    }
    return true;
}

QListWidgetItem *RcListWidget::AddIcon(QString tmpFile)
{ 
    RC_TYPE tmpRcType = getRcType(tmpFile);
    QIcon tmpIcon;
    if(tmpRcType >= RC_BMP && tmpRcType <= RC_PNG)
    {
        if(QFile::exists(tmpFile))
            tmpIcon = QIcon(tmpFile);
        else
            tmpIcon = QIcon(":/icon/rc/icon/image.png");
    }
    else if(tmpRcType >= RC_MP3 && tmpRcType <= RC_WAV)
        tmpIcon = QIcon(":/icon/rc/icon/audio.png");
    else if(tmpRcType >= RC_MP4 && tmpRcType <= RC_FLV)
        tmpIcon = QIcon(":/icon/rc/icon/video.png");
    else
        return (QListWidgetItem *)0;

    QListWidgetItem *tmpItem = 0;
    bool find = false;
    for(int i=0;i<count();i++)
    {
        tmpItem = item(i);
        if(!tmpItem)
            continue;
        QString tmpOldFile = tmpItem->data(D_RCALLNAME).toString();
        if(!tmpOldFile.compare(tmpFile))
        {
            find = true;
            tmpItem->setIcon(tmpIcon);
            break;
        }
    }
    if(!find)
    {
        QListWidgetItem *tmpItem = new QListWidgetItem(tmpIcon, getFileName(tmpFile));
        tmpItem->setData(D_RCALLNAME, tmpFile);
        addItem(tmpItem);
    }
    quint32 tmpHeight = height();
    quint32 trueHeight = tmpHeight - 20;  //>@去掉字的大小
    setGridSize(QSize(tmpHeight, tmpHeight+15));  //>@根据实际情况微调15
    setIconSize(QSize(trueHeight, trueHeight));

    updateGeometries();
    return tmpItem;
}

void RcListWidget::slot_AddIcon()
{
//    QString tmpFileName = QFileDialog::getOpenFileName(this, tr("Select File ..."), m_RcSelectDir);
//    if (tmpFileName.isEmpty())
//        return;
//    m_RcSelectDir = getFileDirectory(tmpFileName);
//    AddIcon(tmpFileName);
}

void RcListWidget::UpdateIcon(QListWidgetItem *pItem, QString pFileName)
{
    if(!pItem || pFileName.isEmpty())
        return;
    //>@使用目标文件替换当前文件
    QString tmpFile = pItem->data(D_RCALLNAME).toString();
    if(!CopyFile(pFileName, tmpFile))
    {
        QMessageBox::warning(0, tr("error"),
                               tr("Copy %1 to %2 failed!").arg(pFileName).arg(tmpFile));
        return;
    }
    pItem->setIcon(QIcon(tmpFile));

    QString tmpFileName = getFileName(tmpFile);
    mQesp->ShowBusy(true);
    mQesp->m_TcpSocket->SendFile("Y", tmpFile, m_ComSrcPath+tmpFileName);
    mQesp->ShowBusy(false);

    quint32 tmpHeight = this->size().height();
    setGridSize(QSize(tmpHeight, tmpHeight+15));
    setIconSize(QSize(tmpHeight-20, tmpHeight-20));

    updateGeometries();
}

void RcListWidget::slot_UpdateIcon()
{
    if(!m_MenuOnItem)
        return;
    int row = this->row(m_MenuOnItem);
    IDE_TRACE_INT(row);
    QString tmpMessage = this->property("update").toString().arg(row);
    //>@发送消息
    IDE_DEBUG(tmpMessage);
    if(tmpMessage.isEmpty()||(!mQesp))
        return;
    if(mQesp->isOfflineLogin())
    {
        if(!mQesp->mMdpManager)
            return;
        QStringList tmpList = tmpMessage.split('#');
        if(tmpList.count() < 2)
            return;
        QStringList tmpCopyList = tmpList.at(1).split(';');
        if(tmpCopyList.count() < 3)
            return;
        QFileInfo tmpInfo(tmpCopyList.at(1));
        if(tmpInfo.isFile())
            CopyFile(tmpCopyList.at(1), tmpCopyList.at(2));
        else
            CopyFolder(tmpCopyList.at(1), tmpCopyList.at(2));
    }
    else
    {
        if(!mQesp->m_TcpSocket)
            return;
        tmpMessage.replace("/usr/bst/", "/mnt/nand1-2/");
        mQesp->m_TcpSocket->Cmd(tmpMessage);
    }

//    QString tmpFileName = QFileDialog::getOpenFileName(this, tr("Select File ..."), m_RcSelectDir);
//    if (tmpFileName.isEmpty())
//        return;
//    m_RcSelectDir = getFileDirectory(tmpFileName);
//    UpdateIcon(m_MenuOnItem, tmpFileName);
}

void RcListWidget::slot_Explore()
{
    if(!m_MenuOnItem)
        return;
    QString tmpFile = m_MenuOnItem->data(D_RCALLNAME).toString();
    QString tmpDir = getFileDirectory(tmpFile);
    QDesktopServices::openUrl(QUrl(tmpDir, QUrl::TolerantMode));
}

void RcListWidget::slot_DelIcon()
{
//    if(!m_MenuOnItem)
//        return;
//    int ret = QMessageBox::question(0, tr("Question"),
//                                   tr("Do you want to delete?"),
//                                   QMessageBox::Yes | QMessageBox::No,
//                                   QMessageBox::No);
//    if(ret == QMessageBox::Yes)
//    {
//        int tmpRow = row(m_MenuOnItem);
//        QListWidgetItem *tmpItem = takeItem(tmpRow);
//        if(tmpItem)
//        {
//            DelFile(tmpItem->data(D_RCALLNAME).toString());
//            delete tmpItem;
//        }
//        m_MenuOnItem = 0;
//        updateGeometries();
//    }
}

void RcListWidget::slot_ItemDoubleClicked(QListWidgetItem *pItem)
{
    if(!pItem)
        return;
    //>@预览
    ImageSelectDialog tmpImageDialog;
    tmpImageDialog.setWindowTitle(tr("Select File"));
    QString tmpPath = D_FSRCPATH + QString("COM/%1/").arg(m_ComName);
    QStringList tmpList = getAllFileInFloders(tmpPath);
    for(int i=0;i<tmpList.count();i++)
        tmpImageDialog.AddIcon(tmpList.at(i));
    int ret = tmpImageDialog.exec();
    if(ret == QDialog::Accepted)
    {
        UpdateIcon(pItem, tmpImageDialog.m_ImagePath);
    }
}

void RcListWidget::slot_ItemPressed(QListWidgetItem *pItem)
{
    if(!pItem)
        return;
    if(QApplication::mouseButtons() == Qt::RightButton)
    {
        m_MenuOnItem = pItem;
        m_ItemMenu->exec(QCursor::pos());
    }
    return;
}

void RcListWidget::mousePressEvent(QMouseEvent *event)
{
    if(!itemAt(event->pos()))
    {
        m_MenuOnItem = 0;
        if(event->button() == Qt::RightButton)
        {
            if(m_BlankMenu)
                m_BlankMenu->exec(QCursor::pos());
        }
    }
    else
    {
        if(event->button() == Qt::LeftButton)
        {
            m_StartDragX = event->x();
            setCursor(Qt::OpenHandCursor);
        }
    }
    QListWidget::mousePressEvent(event);
}

void RcListWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(cursor().shape() == Qt::OpenHandCursor)
    {
        //scroll(event->x()-m_StartDragX, 0);
        //update();
    }
    QListWidget::mouseMoveEvent(event);
}

void RcListWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_StartDragX = 0;
    setCursor(Qt::ArrowCursor);
    QListWidget::mouseReleaseEvent(event);
}

void RcListWidget::resizeEvent(QResizeEvent *e)
{
    if(!e)
        return;

    quint32 tmpHeight = e->size().height();
    quint32 trueHeight = tmpHeight - 20;  //>@去掉字的大小
    setGridSize(QSize(tmpHeight, tmpHeight+15));  //>@根据实际情况微调15
    setIconSize(QSize(trueHeight, trueHeight));

    updateGeometries();
    //resizeEvent(e);
}

void RcListWidget::startDrag(Qt::DropActions actions)
{
}

RcLabel::RcLabel(QWidget *parent, Qt::WindowFlags f) :
    QLabel(parent, f)
{

}

RcLabel::RcLabel(const QString &text, QWidget *parent, Qt::WindowFlags f) :
    QLabel(text, parent, f)
{

}

void RcLabel::resizeEvent(QResizeEvent *e)
{
    int tmpHeight = e->size().height();
    int tmpPoint = CalcPointFromPixel(tmpHeight);
    QString tmpSheet = QString("font:800 %1px/35px \"黑体\";"
                               "color: rgba(255,255,255,250);"
                               "border: 2px dashed rgba(180,130,100,0);"
                               "border-radius: 15px;"
                               "padding: 2px 4px;").arg(tmpPoint);
    setStyleSheet(tmpSheet);
}
