#ifndef IMAGESELECTDIALOG_H
#define IMAGESELECTDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
class ImageSelectDialog;
}

#define D_IMAGELISTNAME     10001

class ImageSelectDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ImageSelectDialog(QWidget *parent = 0);
    ~ImageSelectDialog();

    QListWidgetItem *AddIcon(QString tmpFile);
    
private slots:
    void on_btn_imageselect_clicked();
    void slot_ItemDoubleClicked(QListWidgetItem *pItem);

public:
    Ui::ImageSelectDialog *ui;

    QString             m_ImagePath;
    QString             m_RcSelectDir;
};

#endif // IMAGESELECTDIALOG_H

