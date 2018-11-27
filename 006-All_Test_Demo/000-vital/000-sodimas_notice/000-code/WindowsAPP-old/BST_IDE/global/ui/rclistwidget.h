#ifndef DEVICEUICLASS_H
#define DEVICEUICLASS_H

#include "bstui.h"
#include "physicalnet.h"

class DeviceManager;

#define D_RCALLNAME     10001

#define D_MENU_ADD          QString("&Add")
#define D_MENU_DELETE       QString("&Delete")
#define D_MENU_UPDATE       QString("&Update")

class RcListWidget : public QListWidget
{
    Q_OBJECT
public:
    RcListWidget(QWidget *parent = 0);
    ~RcListWidget();

    void InitForm(DeviceManager *pForm);
    bool InitThemeIconList(DeviceManager *pForm, QString pComName);
    QListWidgetItem *AddIcon(QString tmpFile);
    void UpdateIcon(QListWidgetItem *pItem, QString pFileName);

protected:
    void resizeEvent(QResizeEvent *e);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void startDrag(Qt::DropActions actions);

public slots:
    void slot_ItemDoubleClicked(QListWidgetItem *pItem);
    void slot_ItemPressed(QListWidgetItem *pItem);

    void slot_AddIcon();
    void slot_UpdateIcon();
    void slot_DelIcon();
    void slot_Explore();

public:
    QMenu                 *m_BlankMenu,
                          *m_ItemMenu;
    QListWidgetItem       *m_MenuOnItem;

    int                    m_StartDragX;
    QString                m_RcSelectDir;

    QString                m_ComName;
    QString                m_ComSrcPath,   //>@设备中文件的所在文件夹路径
                           m_ComRelPath;   //>@组件的相对路径

    DeviceManager             *mQesp;
};

class RcLabel : public QLabel
{
    Q_OBJECT
public:
    RcLabel(QWidget *parent = 0, Qt::WindowFlags f=0);
    RcLabel(const QString &text, QWidget *parent=0, Qt::WindowFlags f=0);

    void resizeEvent(QResizeEvent *e);
};


#endif // DEVICEUICLASS_H
