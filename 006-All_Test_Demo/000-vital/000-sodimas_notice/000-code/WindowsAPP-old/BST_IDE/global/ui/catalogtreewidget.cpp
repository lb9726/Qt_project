#include "catalogtreewidget.h"

CatalogTreeWidget::CatalogTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    setHeaderLabel(tr("Calatlog"));
    QTreeWidgetItem *tmpTopLevelItem = new QTreeWidgetItem;
    tmpTopLevelItem->setIcon(0, QIcon(":/Page/rc/Page/rc.png"));
    this->addTopLevelItem(tmpTopLevelItem);
}

CatalogTreeWidget::~CatalogTreeWidget()
{
    DelChildTreeItem(topLevelItem(0));
}

void CatalogTreeWidget::DelChildTreeItem(QTreeWidgetItem *parentItem)
{
    if(parentItem == 0)
        return;
    QList<QTreeWidgetItem*> ItemList = parentItem->takeChildren();
    QTreeWidgetItem* tmpItem;
    for(int i=0;i<ItemList.count();i++)
    {
        tmpItem = ItemList.at(i);
        if(tmpItem == 0)
            continue;
        DelChildTreeItem(tmpItem);
        delete tmpItem;
    }
}

//>@
QTreeWidgetItem *CatalogTreeWidget::DelTreeItem(QTreeWidgetItem *pItem)
{
    if(pItem == 0)
        return (QTreeWidgetItem *)0;
    QTreeWidgetItem* tmpParentItem  = pItem->parent();
    if(tmpParentItem == 0)
        return 0;
    tmpParentItem->removeChild(pItem);
    delete pItem;
    return tmpParentItem;
}

//>@
QTreeWidgetItem *CatalogTreeWidget::AddTreeItem(QTreeWidgetItem *parentItem, QString content, QIcon pIcon, bool Editable)
{
    if(parentItem == 0)
        return (QTreeWidgetItem *)0;
    static quint32 i = 0;
    if(i > 0x0fffffffff)
       i = 0;
    i++;
    QTreeWidgetItem *pNewItem = new QTreeWidgetItem(parentItem, QStringList(getFileName(content)));
    pNewItem->setStatusTip(0, content);
    pNewItem->setTextAlignment(0, Qt::AlignLeft || Qt::AlignVCenter);
    if(i % 2 == 1)
    {
        QLinearGradient linearGrad(QPointF(0, 0), QPointF(200, 200));
        linearGrad.setColorAt(0, Qt::lightGray);
        linearGrad.setColorAt(1, Qt::white);
        QBrush brush(linearGrad);
        pNewItem->setBackground(0, brush);
    }
    pNewItem->setIcon(0, pIcon);
    if(Editable)
        pNewItem->setFlags(Qt::ItemIsEditable |Qt::ItemIsEnabled);
    parentItem->addChild(pNewItem);
    return pNewItem;
}

void CatalogTreeWidget::ScanDirToTreeList(QTreeWidgetItem *parentItem, QString pPath, QStringList pSuffixList)
{
    if(parentItem == 0)
        return;
    if(!pPath.endsWith("/"))
        pPath.append("/");
    QDir tmpDir(pPath);
    if(!tmpDir.exists())
        return;
    QStringList folders = tmpDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (int i = 0; i < folders.size(); ++i)
    {
        QString tmpSubPath = QString("%1%2").arg(pPath).arg(folders[i]);
        QFileIconProvider file_icon;
        QTreeWidgetItem *tmpSubDirTreeItem = AddTreeItem(parentItem, tmpSubPath, file_icon.icon(QFileInfo(tmpSubPath)));
        ScanDirToTreeList(tmpSubDirTreeItem, tmpSubPath, pSuffixList);
        QtSleep(10);
    }
    QStringList files = findFiles(pPath, "*", "", Qt::CaseInsensitive);
    for(int i = 0; i < files.size(); ++i)
    {
        QFileInfo tmpFileInfo(files[i]);
        QString tmpSuffix = tmpFileInfo.suffix();
        for(int j=0;j<pSuffixList.count();j++)
        {
            if(tmpSuffix.compare(pSuffixList[j], Qt::CaseInsensitive) == 0)
            {
                QFileIconProvider file_icon;
                AddTreeItem(parentItem, files[i], file_icon.icon(tmpFileInfo));
                break;
            }
        }
    }
}

void CatalogTreeWidget::UpdateTreeWidget(QString pPath, QStringList pSuffixList)
{
    DelChildTreeItem(topLevelItem(0));
    setDisabled(true);
    ScanDirToTreeList(topLevelItem(0), pPath, pSuffixList);
    expandAll();
    setDisabled(false);
}
