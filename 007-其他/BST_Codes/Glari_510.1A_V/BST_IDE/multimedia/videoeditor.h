#ifndef VIDEOEDITOR_H
#define VIDEOEDITOR_H

#include "tabpage.h"
#include "basemedia.h"
#include <QFileDialog>
#include <QQueue>
#include <QProgressBar>
#include <QThread>
#include <QTreeWidgetItem>
#include <QPainter>
#include <QPalette>
#include <QScrollArea>
#include <QFileInfoList>
#include <QDir>
#include <QFile>
#include <QTimer>
#include <QMouseEvent>
#include <QWidget>

#define D_DEFAULT_SAVE_PATH QString("D:/MyProject/IDE/RuntimeRc/FSRC/TOOL/Windows/Res/Save")
#define D_SENSITIVE 20

typedef enum{
    Canvas_Nothing,
    Canvas_GrabScreen,
    Canvas_Watermark,
    Canvas_Text,
    Canvas_Del,
    Canvas_Save
} CANVAS_BTN_TYPE;

typedef enum{
    Rect_UnSelect = 0,
    Rect_Select,
    Rect_LeftTOP,
    Rect_LeftBottom,
    Rect_RightBottom,
    Rect_RightTOP,
    Rect_Left,
    Rect_Bottom,
    Rect_Right,
    Rect_TOP
} RECT_TRIGER;

typedef struct{
    bool                    mActive;
    QPoint                  mMouseInPos;
    QRect                   mMouseInRect;
    RECT_TRIGER             mTriger;
    QBrush                  mColor;
    CANVAS_BTN_TYPE         mType;
    QRect                   mRect;
}CANVAS_RECT_INFO;

typedef enum{
    Convert_Wait,
    Convert_Busy,
    Convert_Err,
    Convert_Finished
} CONVERT_STATE;

typedef struct{
    Media_Info              mFileInfo;
    CONVERT_STATE           mState;
    quint8                  mProgress;
}LIST_MEMBER_INFO;

typedef struct{
    QString                 mType;
    QSize                   mReslution;
    int                     mFrame;
    QRect                   mRect;
    QString                 mDstDir;
    int                     mDietime;
}ConvertSetting;

class ConvertThread;
class Canvas;

namespace Ui {
class VideoEditor;
}

class VideoEditor : public TabPage
{
    Q_OBJECT
    friend class ConvertThread;
public:
    explicit VideoEditor(QWidget *parent = 0);
    ~VideoEditor();

    void AppendList(LIST_MEMBER_INFO pListInfo)
    {
        m_List.append(pListInfo);
    }
    void DelListMember(int pDex)
    {
        quint8 tmpNum = m_List.count();
        if(tmpNum > pDex)
        {
            m_List.removeAt(pDex);
        }
    }
    void ClearList()
    {
        m_List.clear();
        RefreshTreelist();
    }
    void ReplaceListMember(int pDex, LIST_MEMBER_INFO pMember)
    {
        quint8 tmpNum = m_List.count();
        if(tmpNum > pDex)
        {
            m_List.replace(pDex, pMember);
        }
    }
    void MoveListMemberPostion(int pSelectDex, int pUpDown)
    {
        if(pUpDown == 1)
        {
            if(pSelectDex <1)
                return;
        }
        else if(pUpDown == -1)
        {
            if(pSelectDex > (m_List.count()-2))
                return;
        }
        quint8 tmpPostion = pSelectDex - pUpDown;
        IDE_TRACE_INT(tmpPostion);
        m_List.move(pSelectDex,tmpPostion);
        RefreshTreelist();
    }
    void ItemChangeState(int row, CONVERT_STATE pState)
    {
        LIST_MEMBER_INFO curSrcInfo = m_List.at(row);
        curSrcInfo.mState = Convert_Busy;
        ReplaceListMember(row, curSrcInfo);
    }
    void ItemChangeProgress(int row, int pVaule)
    {
        pVaule = (pVaule>=100)? 100: pVaule;
        LIST_MEMBER_INFO curSrcInfo = m_List.at(row);
        curSrcInfo.mProgress = pVaule;
        ReplaceListMember(row, curSrcInfo);
    }
    void RefreshTreelist();

    bool GetParameter();
    void StopConvertThread();
    void KillConvertThread();
    void StartConvertThread();
    void GetSrcInfo(QStringList pList);

    QList<LIST_MEMBER_INFO>     m_List;
    ConvertSetting              mConvertSetting;
    QString                     m_TempPath;
    QScrollArea*                m_ScrollArea;
private:
    void InitPara();
    void InitUi();
    void InitUiCanvas();
    void initUiTreeList();
    void initUiParameter();
    void InitPlayer();
    void ReleasePlayer();

    void LockParameSet();
    void UnlockParameSet();
    void LockTreeList();
    void UnlockTreeList();
    void LockContrlBar();
    void UnlockContrlBar();

    ConvertThread*              mConvertThread;
    Canvas                      *m_Canvas;
    basemedia                   *m_Player;
signals:
private slots:
    void slot_FileInfo(QTreeWidgetItem *item, int column);
    void slot_Play(QTreeWidgetItem *item, int column);
    void slot_RefreshTreelist();
    void slot_Finished();

    void on_add_clicked();
    void on_del_clicked();
    void on_up_clicked();
    void on_down_clicked();
    void on_savelist_clicked();
    void on_start_clicked();
    void on_stop_clicked();
    void on_BtnGrabScreen_clicked();

    void on_BtnStart_clicked();
    void on_BtnStop_clicked();
    void on_BtnWatermark_clicked();
    void on_BtnText_clicked();
    void on_BtnDel_clicked();
    void on_BtnSave_clicked();

private:
    Ui::VideoEditor *ui;
};

class ConvertThread : public QThread
{
    Q_OBJECT
public:
    ConvertThread(VideoEditor *qesp)
        :QThread()
    {
        connect(&mConvert, SIGNAL(sProgress(quint8)),this, SLOT(slot_Progress(quint8)));
        mConvert.moveToThread(this);
        mQesp = qesp;
        termin = false;
    }
    ~ConvertThread()
    {
        disconnect(&mConvert, SIGNAL(sProgress(quint8)),0, 0);
        mConvert.KillProcess();
    }
    void Stop()
    {
        termin = true;
        mConvert.KillProcess();
    }
    void GetFileInfo(QStringList pList)
    {
        for(int i=0;i<pList.count();i++)
        {
            mConvert.GetFileInfo(pList.at(i), 2000);
            QtSleep(10);
            LIST_MEMBER_INFO MembersInfo;
            MembersInfo.mFileInfo = mConvert.m_FileInfo;
            if(MembersInfo.mFileInfo.mType == RC_INVALID)
                MembersInfo.mState = Convert_Err;
            else
                MembersInfo.mState = Convert_Wait;
            MembersInfo.mProgress = 0;
            mQesp->AppendList(MembersInfo);
        }
        emit sRefreshTreelist();
        emit sFinished();
    }

    bool                    termin;
    VideoEditor*            mQesp;
    quint8                  mCurRow;
    MULTIMEDIA_CONVERT      mConvert;
protected:
    virtual void run()
    {
        for(int i=0;i<mQesp->m_List.count();i++)
        {
            mCurRow = i;
            QString src = mQesp->m_List.at(i).mFileInfo.mPath;
            QSize size = mQesp->mConvertSetting.mReslution;
            int frame = mQesp->mConvertSetting.mFrame;
            QString dst = QString("%1%2.%3").
                            arg(mQesp->mConvertSetting.mDstDir).
                            arg(getFileName(src)).
                            arg(mQesp->mConvertSetting.mType);
            int dietime = mQesp->mConvertSetting.mDietime;
            if(mQesp->m_List.at(i).mFileInfo.mType == RC_INVALID)
                continue;
            mQesp->ItemChangeState(i, Convert_Busy);
            emit sRefreshTreelist();
            if(mConvert.ConvertVideo(src, size, frame, dst, dietime))
            {
                mQesp->ItemChangeState(i, Convert_Finished);
                mQesp->ItemChangeProgress(i, 100);
                emit sRefreshTreelist();
            }
            else
            {
                mQesp->ItemChangeState(i, Convert_Err);
                emit sRefreshTreelist();
            }
            if(termin)
                break;
        }
        emit sFinished();
    }
signals:
    void sFinished();
    void sRefreshTreelist();
public slots:
    void slot_Progress(quint8 pVaule)
    {
        mQesp->ItemChangeProgress(mCurRow, pVaule);
        emit sRefreshTreelist();
    }
};

class Canvas : public QWidget
{
    Q_OBJECT
public:
    Canvas(VideoEditor *pFriend);
    ~Canvas();
    void InitPara();
    void InitHash();
    void RefreshHash();
    void GetBackground(QString pPath);
    void GetRectTrigerState(QPoint pPos);
    void RectChangeFllowPos(QPoint pPos);
public:
    VideoEditor                 *m_Friend;
    QTimer                      m_BrushTimer;
    QPixmap                     m_Pixmap;
    CANVAS_RECT_INFO            m_CurRectInfo;
    QHash<CANVAS_BTN_TYPE, CANVAS_RECT_INFO> m_RectsInfo;
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
signals:
public slots:
    void slot_Brush();
};

#endif // VIDEOEDITOR_H
