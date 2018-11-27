#include "imageselectdialog.h"
#include "ui_imageselectdialog.h"

ImageSelectDialog::ImageSelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageSelectDialog)
{
    ui->setupUi(this);

    ui->listWidget->setResizeMode(QListWidget::Adjust);
    ui->listWidget->setViewMode(QListWidget::IconMode);
    ui->listWidget->setFlow(QListWidget::LeftToRight);
    ui->listWidget->setWrapping(false);  //>@用于使图标横向显示
    ui->listWidget->setLayoutMode(QListWidget::SinglePass);
    ui->listWidget->setMovement(QListWidget::Snap);
    ui->listWidget->setAutoScroll(true);
    ui->listWidget->setAutoScrollMargin(20);
    ui->listWidget->setDragEnabled(true);
    ui->listWidget->setSpacing(10);
    ui->listWidget->setTextElideMode(Qt::ElideMiddle);
    quint32 tmpHeight = ui->listWidget->height();
    quint32 trueHeight = tmpHeight - 20;  //>@去掉字的大小
    ui->listWidget->setGridSize(QSize(tmpHeight, tmpHeight+15));  //>@根据实际情况微调15
    ui->listWidget->setIconSize(QSize(trueHeight, trueHeight));
    ui->listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(slot_ItemDoubleClicked(QListWidgetItem*)));
}

ImageSelectDialog::~ImageSelectDialog()
{
    delete ui;
}

QListWidgetItem *ImageSelectDialog::AddIcon(QString tmpFile)
{
    if(tmpFile.isEmpty())
        return (QListWidgetItem *)0;
    QListWidgetItem *tmpItem = new QListWidgetItem(QIcon(tmpFile), getFileName(tmpFile));
    tmpItem->setData(D_IMAGELISTNAME, tmpFile);
    ui->listWidget->addItem(tmpItem);
    return tmpItem;
}

void ImageSelectDialog::slot_ItemDoubleClicked(QListWidgetItem *pItem)
{
    m_ImagePath = pItem->data(D_IMAGELISTNAME).toString();
    ui->line_imagepath->clear();
    ui->label_preview->setPixmap(pItem->icon().pixmap(ui->label_preview->size()));
}

void ImageSelectDialog::on_btn_imageselect_clicked()
{
    QString tmpFileName = QFileDialog::getOpenFileName(this, tr("Select File ..."), m_RcSelectDir);
    if (tmpFileName.isEmpty())
        return;
    m_RcSelectDir = getFileDirectory(tmpFileName);

    m_ImagePath = tmpFileName;
    ui->line_imagepath->setText(tmpFileName);
    ui->label_preview->setPixmap(QPixmap(tmpFileName));
}

