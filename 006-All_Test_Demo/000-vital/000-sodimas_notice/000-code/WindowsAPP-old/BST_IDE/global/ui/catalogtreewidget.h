#ifndef CATALOGTREEWIDGET_H
#define CATALOGTREEWIDGET_H

#include "global.h"
#include <QTreeWidget>

class CatalogTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit CatalogTreeWidget(QWidget *parent = 0);
    ~CatalogTreeWidget();
    
    void DelChildTreeItem(QTreeWidgetItem *parentItem);
    QTreeWidgetItem *DelTreeItem(QTreeWidgetItem *pItem);
    QTreeWidgetItem *AddTreeItem(QTreeWidgetItem *parentItem, QString content, QIcon pIcon, bool Editable=false);
    void ScanDirToTreeList(QTreeWidgetItem *parentItem, QString pPath, QStringList pSuffixList);

signals:
    
public slots:
    void UpdateTreeWidget(QString pPath, QStringList pSuffixList);
};

#endif // CATALOGTREEWIDGET_H
