#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include "globalfun.h"
#include <QTime>
#include <QRegExpValidator>
#include <QKeyEvent>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QLineEdit>
#include <QSlider>

#include <QLabel>
#include <QMovie>
#include <QImageReader>

#include <QTreeWidget>
#include <QTableWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QScrollBar>
#include <QEvent>
#include <QListWidget>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QMouseEvent>
#include <QTime>
#include <QRadioButton>

class MainWindow;

void HideFocus(QWidget *pWidget);

class BstUiManager
{
public:
    BstUiManager(){}
    virtual bool ProcessCommand(int pType, QString pCmd, bool pSync = true) {}
};

#define D_BGCOLOR           QString("241,241,241,20")
#define D_TEXTCOLOR         QString("83,83,83,255")
#define D_FRAMECOLOR        QString("83,83,83,255")
#define D_EDITCOLOR         QString("230,230,230,255")

#define D_BUTTONTIP         QString("Button")
#define D_CHECKBOXTIP       QString("CheckBox")
#define D_COMBOXTIP         QString("Combox")
#define D_ENETTIP           QString("EnetAddr")
#define D_SWITCHTIP         QString("Switch")
#define D_TEXTEDITTIP       QString("TextEdit")

typedef enum{
    STATE_UNFOCUS=0,
    STATE_FOCUS=1,
    STATE_EDITABLE=2
} LOGICSTATE;

class BstUiBase
{
public:
    BstUiBase();
    void KeyEvent(QKeyEvent *e);        //>@?ü?ì′|àí?? y
    void MouseEvent(QMouseEvent* e);    //>@ ó±ê′|àí?? y
    void WheelEvent(QWheelEvent* e);    //>@????′|àí?? y
    void FocusInEvent();
    void FocusOutEvent();
    virtual void ProcessKeyEvent(QKeyEvent *e);
    virtual void ProcessMouseEvent(QMouseEvent* e);
    virtual void ProcessWheelEvent(QWheelEvent* e);
    virtual bool isFocus();
    virtual void PaintBorder();
    bool IsChanged() { return m_IsChanged; }
public:
    LOGICSTATE      m_LogicState;
    bool            m_IsChanged;
};

/*****************************************************
                Label
  ****************************************************/

class Label : public QWidget
{
    Q_OBJECT
public:
    Label(QString text = QString(), QWidget *parent = 0);

    void SetText(QString text);

    void paintEvent(QPaintEvent *e);
    void SetFocus(bool falg);

public:
    QFont           m_Font;
    QColor          m_Color;
    QString         m_Content;
};

class TipLabel : public QLabel
{
    Q_OBJECT
public:
    TipLabel(QWidget *parent=0, Qt::WindowFlags f=0);
    TipLabel(const QString &text, QWidget *parent=0, Qt::WindowFlags f=0);
    ~TipLabel();

    void SetText(QString pContent);
    void SetAnimEnable(bool pEnable);

public slots:
    void slot_FrameChanged(int pCount);
    void slot_StateChanged(QTimeLine::State pState);

public:
    QTimeLine       mTimeLine;
    bool            mAnimateEn;
    QString         mContent;
    QString         mContentBuf;
};

class gifplayer : public QLabel
{
    Q_OBJECT
public:
    gifplayer(QWidget *parent = 0);
    gifplayer(QString pGifFile, bool pRepeat, QWidget *parent = 0);
    void Play(QString pGifFile, bool pRepeat);
    void Play();
    void Stop();

public slots:
    void slot_Finished();
    void slot_Error(QImageReader::ImageReaderError pErr);

public:
    QMovie  *m_GifMovie;
    bool     m_Repeat;
};

/*
    í???
    QML
    gif svg
    video
    stream
    audio
*/

class PreviewLabel : public QLabel
{
    Q_OBJECT
public:
    explicit PreviewLabel(QWidget *parent = 0);
    ~PreviewLabel();
    void Release();

    void ShowPixmap(QString pFilePath);
    void ShowGIF(QString pFilePath);
    void ShowSVG(QString pFilePath);
    void ShowQML(QString pFilePath);
    void PlayMedia(QString pFilePath);

    void Preview(QString pFile);

    void resizeEvent(QResizeEvent *e);

signals:
    void UpdatePreview();

public slots:

public:
    QMovie          *m_GifMovie;
#ifdef SURRPORT_SVG
    QSvgRenderer    *m_SvgRender;
#endif
    QProcess        *m_MediaProcess;
};

/*****************************************************
                Container
  ****************************************************/

class GroupBox : public QGroupBox
{
    Q_OBJECT
public:
    GroupBox(QWidget* parent=0);
    GroupBox(const QString &title, QWidget* parent=0);

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
};

class Groupbox : public QWidget
{
    Q_OBJECT
public:
    Groupbox(QWidget *parent = 0);
    Groupbox(QString pText=QString(), QWidget *parent = 0);
    ~Groupbox();

    //bool AddControl();

    void InitBrush(QRect pRect);

    bool event(QEvent *e);
    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *e);

public:
    QVBoxLayout *m_VLayout;
    QHash<QString, QObject*>     mParaItems;       //>@±￡′??? y??ó|μ????t

    QBrush       m_BgBrush, m_FgBrush,
                 m_BGlowBrush,      //>@μ×???a??
                 m_TGlassBrush, m_TGlassBrush2,     //>@?????￡á§??é°D§??
                 m_LightBrush;      //>@???D??ááD§??
    QColor       m_BgPenColor, m_FgPenColor;
    QRect        m_BgRect, m_FgRect,
                 m_BGlowRect,      //>@μ×???a??
                 m_TGlassRect,     //>@?????￡á§??é°D§??
                 m_LightRect;      //>@???D??ááD§??

    QString      m_Content;
    QFont        m_ContentFont;
    QColor       m_ContentColor;
};

class ToolContainer : public QWidget
{
    Q_OBJECT
public:
    explicit ToolContainer(QWidget *parent = 0);

    void keyPressEvent(QKeyEvent *event);

    bool eventFilter(QObject *target, QEvent *event);
signals:

public slots:

};

/*****************************************************
                Slider
  ****************************************************/

class Slider :public QSlider
{
    Q_OBJECT
public:
    explicit Slider(QWidget *parent=0);
    ~Slider();

    void bindLabel(QLabel *pLabel);

private slots:
    void slot_ValueChanged(int);

signals:
    void sValueChanged(int);

public:
    QPointer<QLabel>        mLabel;
};

class MySlider :public QSlider
{
    Q_OBJECT
public:
    explicit MySlider(QWidget *parent=0);
    ~MySlider();

    void SetType(int pType);

    int GetValue(int pVaule);
    void WriteValue(int pValue);
    void bindLabel(QLabel *pLabel, QString pFormat=QString("%1%"), QString pEquation=QString());

private slots:
    void slot_ValueChanged(int);
    void slot_SliderReleased();

public:
    QPointer<QLabel>     mLabel;
    BstUiManager        *mQesp;
    QString              mFormat;
    QString              mEquation;

    int                 oldValue;
};

class ScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    ScrollArea(QWidget *parent = 0);

    void InitMovePara(DIRECTION pDirection, int pHScrollSize=-1, int pHSubWidSize=-1, int pVScrollSize=-1, int pVSubWidSize=-1);
    bool eventFilter(QObject *target, QEvent *event);
    void resizeEvent(QResizeEvent *event);

    void UpdateEventFilter();

public:
    DIRECTION                 m_SliderDirection;

    QPoint                    m_MousePosBk[2];
    QTime                     m_MouseBaseTime;
    int                       m_MouseTime[2];
    int                       m_XVelocity, m_YVelocity;
    qreal                     m_HRelDiff, m_VRelDiff;  //>@widgetò???ò???????Dèò?ò???scrool?àéù

    QPointer<QScrollBar>      m_HScrollBar;
    QPointer<QScrollBar>      m_VScrollBar;

#ifndef QT_V453
    QPropertyAnimation       *m_HAnimation;
    QPropertyAnimation       *m_VAnimation;
#endif
    bool                      m_IsMoving;
    QEvent::Type              m_MouseStatus;
};

/*****************************************************
                Button
  ****************************************************/

class MySpinbox :public QSpinBox
{
    Q_OBJECT
public:
    explicit MySpinbox(QWidget *parent=0);
    ~MySpinbox();

    int GetValue(int pVaule);
    void WriteValue(int pValue);
    void bindLabel(QLabel *pLabel, QString pFormat=QString("%1%"), QString pEquation=QString());

    QPointer<QLabel>     mLabel;
    BstUiManager       *mQesp;
    QString              mFormat;
    QString              mEquation;
private slots:
    void slot_ValueChanged(int);
};

/*****************************************************
                Button
  ****************************************************/

class Button : public QWidget
{
    Q_OBJECT
public:
    Button(QString pText, bool pCheckable, QWidget *parent = 0);
    Button(QString pText, bool pCheckable, QPixmap pNormalImg, QPixmap pDownImg, QWidget *parent = 0);

    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent *e);
    void focusOutEvent(QFocusEvent *e);

    QString text() {return m_Content;}
    bool isChecked()  {return m_CheckState;}
    void setChecked(bool falg);
    void paintEvent(QPaintEvent *e);

signals:
    void sPressed(bool flag);

public:
    bool        m_Checkable;
    bool        m_CheckState;

    QPixmap     m_NormalImage;
    QPixmap     m_DownImage;

    QRect       m_ContentRect;
    QFont       m_Font;
    QColor      m_Color;
    QString     m_Content;
};

class MyPushButton :public QPushButton
{
    Q_OBJECT
public:
    explicit MyPushButton(QWidget *parent=0);
    ~MyPushButton();

    BstUiManager             *mQesp;
private slots:
    void slot_Toggled(bool);
};

class MyRadioButton :public QRadioButton
{
    Q_OBJECT
public:
    explicit MyRadioButton(QWidget *parent=0);
    ~MyRadioButton();

    BstUiManager             *mQesp;
protected:
private slots:
    void slot_Toggled(bool);
};

class Switch : public Button
{
    Q_OBJECT
public:
    Switch(QString pSwitch, QString pOnText, QString pOffText, QWidget *parent = 0);

signals:
    void sChanged(QObject*);
public slots:
    void slot_ButtonClicked(bool flag);

public:
    QString     m_OnText, m_OffText;
};

class ButtonList : public QHBoxLayout
{
    Q_OBJECT
public:
    explicit ButtonList(QStringList pParaList, QWidget *parent = 0);
    ~ButtonList();

    QStringList GetCheckedButton();
    void SetButtonCheckable(bool pEnable);

public slots:
    void slot_ButtonClicked(bool flag);

signals:
    void sChanged(QObject*);
};

class CheckList : public QHBoxLayout
{
    Q_OBJECT
public:
    explicit CheckList(QStringList pParaList, QStringList pSelectList=QStringList(), QWidget *parent = 0);
    ~CheckList();

    void InitCheckList(QStringList pList);
    void RestoreCheckList();

    Button *GetCheckbox(int index);
    Button *GetCheckbox(QString pName);

    QStringList GetSelectList();
    QString GetSelectText(QStringList pList);
    QString GetSelectText();

signals:
    void sChanged(QObject*);

public slots:
    void slot_ButtonClicked(bool flag);

public:
    QStringList         m_SelectList;
};

class Checkbox : public QCheckBox, public BstUiBase
{
    Q_OBJECT
public:
    Checkbox(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseDoubleClickEvent(QMouseEvent* e);
    void wheelEvent(QWheelEvent *e);

    void keyPressEvent(QKeyEvent *e);

    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);

    //>@Dé?? y￡? μ???ùóD?|?ü
    void ProcessMouseEvent(QMouseEvent* e);
    void ProcessKeyEvent(QKeyEvent *e);
    void ProcessWheelEvent(QWheelEvent *e);
    bool isFocus();
    void PaintBorder();
};

class MyCheckBox :public QCheckBox
{
    Q_OBJECT
public:
    explicit MyCheckBox(QWidget *parent=0);
    ~MyCheckBox();

    bool GetStatus();
    void SetStatus(bool pStatus);
    void bindLabel(QLabel *pLabel, QString pCheckedText=QString(""), QString pUncheckedText=QString(""));

    QPointer<QLabel>     mLabel;
    BstUiManager       *mQesp;
    QString              mCheckedText;
    QString              mUncheckedText;
private slots:
    void slot_StateChanged(int);
};

class BaseToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit BaseToolBar(QWidget *parent=0);
    ~BaseToolBar();

public:
    MainWindow          *mMainWindow;
};

/*****************************************************
                Text Edit
  ****************************************************/

//>------------------------Completer-----------------------

typedef enum{
    Mode_UnsortedFS = 0,
    Mode_FullpathFS,
    Mode_Country,
    Mode_Word
} COMPLETER_MODE;

class FileSystemModel : public QFileSystemModel
{
public:
    FileSystemModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

class CompleterLineEdit : public QLineEdit
{
public:
    CompleterLineEdit(QWidget *parent=0, COMPLETER_MODE pMode=Mode_FullpathFS);
    ~CompleterLineEdit();

    QAbstractItemModel *modelFromFile(const QString& fileName);

    void CreateCompleter();
    void ReleaseCompleter();
    void changeModel(COMPLETER_MODE mode);

public:
    QCompleter      *m_Completer;
};

class ENetAddrEdit : public QWidget
{
    Q_OBJECT
public:
    explicit ENetAddrEdit(QString pIP=QString(), QWidget *parent = 0);

    void SetState(int pState);
    void SetIP(QString pIP);
    void SetAreaIP(int index, quint8 pNum);
    void GotoArea(int index);
    void MoveAreaRight();

    quint8 GetAreaIP(int pAreaIndex);
    QString GetIP();

    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent *e);
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);

    void paintEvent(QPaintEvent *e);

public slots:
    void slot_FlowChange();

signals:
    void sChanged(QObject*);

public:
    typedef enum {BgOff = 0, MinusOn, AddOn, IPNum1, IPNum2, IPNum3, IPNum4} ENET_STATE;
    ENET_STATE          m_State;

    QPixmap             m_Pixmap[3];
    QPixmap             m_CurPixmap;
    QPixmap             m_FocusPixmap;
    int                 m_FocusAreaIndex;  //>@????????óò
    QRect               m_AreaRect[7];

    QTimer              m_FlowTimer;  //>@?ó ±???¤?¨ ±?÷
    QFont               m_Font, m_FocusFont;
    QColor              m_Color, m_FocusColor;
    ENET_ADDR           m_IPAddr;
};

class LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    LineEdit(QWidget *parent = 0);
    LineEdit(const QString &str, QWidget* parent=0);
    ~LineEdit();

    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);

signals:
    void sActive();
    void sUnActive();
};

class MyLineEdit :public LineEdit
{
    Q_OBJECT
public:
    explicit MyLineEdit(QWidget *parent=0);
    ~MyLineEdit();

    QString GetText();
    void SetText(QString pText);

    BstUiManager       *mQesp;

public Q_SLOTS:
    void slot_TextChanged();
};

class TextEdit : public QLineEdit
{
    Q_OBJECT
public:
    TextEdit(QWidget *parent = 0);
    TextEdit(QString pText, QWidget *parent = 0);

    void UpdateSelection(int pos);
    void UpdateSelectionData(Qt::Key pKey);

signals:
    void ShowKeyBoard(QPoint pos);
    void HideKeyBoard();
    void sChanged(QObject*);
public slots:
    void slot_TextChanged(QString);
};

class ComboBox : public QWidget
{
    Q_OBJECT
public:
    ComboBox(QStringList pParaList, QString pSelectPara, QWidget *parent = 0);

    void InitEnumList(QStringList pList, QString pValue);
    bool setCurrentText(QString pText);
    void setCurrentIndex(int pIndex);
    QString currentText();
    int currentIndex();

    void SetState(int pState);

    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent *e);

    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);

    void paintEvent(QPaintEvent *e);

signals:
    void sChanged(QObject*);

public:
    typedef enum {BgOff = 0, MinusOn, AddOn, Content} COMBOX_STATE;
    QRect           m_AreaRect[4];
    QPixmap         m_CurPixmap;
    QPixmap         m_Pixmap[3];

    COMBOX_STATE    m_State;
    QFont           m_Font;
    QColor          m_Color;
    QStringList     m_EnumList;
    int             m_CurIndex;
};

class MyComboBox :public QComboBox
{
    Q_OBJECT
public:
    explicit MyComboBox(QWidget *parent=0);
    ~MyComboBox();

    void SetCurrentIndex(int pIndex);
    BstUiManager             *mQesp;

private slots:
    void slot_CurrentIndexChanged(int);
};

/*
    columnlabels  QCheckBox????
    data          QListWidgetItemμ?ó??§×??¨ò? y?Y
    cmb           ìí?óáD±í??μ?QComboBox,′?′|?÷ò?ó?óú?° ±μ???ó??§????μ??á???? ???à′
*/
class ComboListView : public QListWidget
{
    Q_OBJECT    //×??¨ò?D????? ±Dè?óé????ê￡?·??òá′?óD????????á §°ü
public:
    ComboListView(QWidget* parent=0);

    bool AddCombox(QComboBox* pCmb);
    bool DelCombox(QComboBox* pCmb);

    bool AddItem(QString pName, QVariant pData);
    bool DelItem(QString pName);

    //·μ??ó??§??????μ?ó??§×??¨ò? y?Y
    void GetSelectDatas(QList<QVariant> &pDatas);

private slots:
    void set_select_item(int state);   //QCheckBox?′??????′|àí

private:
    QList<QComboBox*> m_cmboxs;
    QList<bool>       m_bchecked;
    QList<QVariant>   m_data;
    QList<QCheckBox*> m_checklist;
    QList<QString>    m_fslist;
};

class ComboList : public QComboBox
{
    Q_OBJECT
public:
    ComboList(QWidget* parent = 0);
    void SetModel(ComboListView *pView, bool pFirst);
};


class SpinEdit : public QWidget
{
    Q_OBJECT
public:
    SpinEdit(int pValue, int minValue, int maxValue, QWidget *parent = 0);

    void SetState(int pState);
    void setValue(int pValue);
    int value()  {return m_Value;}

    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent *e);

    void keyPressEvent(QKeyEvent *e);

    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);

    void paintEvent(QPaintEvent *e);

public slots:
    void slot_FlowChange();

signals:
    void sChanged(QObject*);

public:
    typedef enum {BgOff = 0, MinusOn, NumOn, AddOn} SPIN_STATE;
    QRect           m_AreaRect[4];
    QPixmap         m_CurPixmap;
    QPixmap         m_Pixmap[4];

    QTimer          m_FlowTimer;  //>@?ó ±???¤?¨ ±?÷
    SPIN_STATE      m_State;
    QFont           m_Font;
    QColor          m_Color;
    int             m_MinValue, m_MaxValue, m_Value;
};


class DateTimeEdit : public QWidget
{
    Q_OBJECT
public:
    DateTimeEdit(QDateTime pDateTime, QWidget *parent = 0);

    void SetState(int pState);

    void SetDateTime(QDateTime pDateTime);
    void SetDateTime(QString pDateTime) {SetDateTime(QDateTime::fromString(pDateTime));}
    QDateTime GetDateTime();

    void SetAreaTime(int index, quint8 pNum);
    quint8 GetAreaTime(int pAreaIndex);
    void GotoArea(int index);
    void MoveAreaRight();

    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent *e);
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);

    void paintEvent(QPaintEvent *e);

public slots:
    void slot_FlowChange();

signals:
    void sChanged(QObject*);

public:
    typedef enum {BgOff = 0, MinusOn, AddOn, DateYear, DateMonth, DateDay, TimeHour, TimeMinute, TimeSecond} DATETIME_STATE;
    DATETIME_STATE      m_State;

    QPixmap             m_Pixmap[3];
    QPixmap             m_CurPixmap;
    QPixmap             m_FocusPixmap;
    int                 m_FocusAreaIndex;
    QRect               m_AreaRect[6];
    quint8              m_DateTime[6];

    QTimer              m_FlowTimer;  //>@?ó ±???¤?¨ ±?÷
    QFont               m_Font, m_FocusFont;
    QColor              m_Color, m_FocusColor;
};

class MyTimeEdit :public QTimeEdit
{
    Q_OBJECT
public:
    explicit MyTimeEdit(QWidget *parent=0);
    ~MyTimeEdit();

    BstUiManager             *mQesp;

private slots:
    void slot_TimeChanged(QTime);
};

class MyDateTimeEdit :public QDateTimeEdit
{
    Q_OBJECT
public:
    explicit MyDateTimeEdit(QWidget *parent=0);
    ~MyDateTimeEdit();

    BstUiManager             *mQesp;
protected:
private slots:
    void slot_DateTimeChanged(QDateTime);
};

/*****************************************************
                Progress
  ****************************************************/

class ProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit ProgressBar(int pValue = 0, QWidget *parent = 0);

    void InitBrush();

    void setProgress(int pValue);
    void setMessage(QString pMsg);
    void setErrorStatus(int pErrorLevel);  //>@éè??′í?óμè??

    bool event(QEvent *e);
    void paintEvent(QPaintEvent *e);

signals:

public slots:

public:
    QBrush       m_BgBrush, m_TGlassBrush;      //>@???D??ááD§??
    QColor       m_BgPenColor, m_TGlassPenColor;
    QRect        m_ValidRect;

    QString      m_Content;
    QFont        m_ContentFont;
    QColor       m_ContentColor;
};

/*****************************************************
                messagebox
  ****************************************************/
#include <QMessageBox>
#include <QEventLoop>

//>@调用会阻塞主UI线程，但可在任意线程中直接调用。
//>@直接静态调用：ThreadMessageBox::show(LEVEL_INFO, "Title", "Message");
class ThreadMessageBox: public QObject
{
    Q_OBJECT
public:
    ThreadMessageBox(MSG_LEVEL type, const QString &title, const QString &message);

    static void show(MSG_LEVEL type, const QString &title, const QString &message);

private:
    void readyShow(void);

private slots:
    void onShow(void);

public:
    MSG_LEVEL       m_type;  //>@
    const QString   m_title;
    const QString   m_message;
};

/*****************************************************
                items
  ****************************************************/

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
class QDragMoveEvent;
QT_END_NAMESPACE

/**
//>@UpQTreeWidget******************************************************
*/
class UpQTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    UpQTreeWidget(QWidget *parent = 0);
    ~UpQTreeWidget();

    void ClearAllItem();
    void ClearTreeItem(QTreeWidgetItem *parentItem);
    QTreeWidgetItem *DelTreeItem(QTreeWidgetItem *pItem);

    void UpDragEnterEvent(QDragEnterEvent *event)
    {
        QTreeWidget::dragEnterEvent(event);
    }
    void UpDragLeaveEvent(QDragLeaveEvent *event)
    {
        QTreeWidget::dragLeaveEvent(event);
    }
    void UpDragMoveEvent(QDragMoveEvent *event)
    {
        QTreeWidget::dragMoveEvent(event);
    }
    void UpDropEvent(QDropEvent *event)
    {
        QTreeWidget::dropEvent(event);
    }
    void UpStartDrag(Qt::DropActions actions)
    {
        QTreeWidget::startDrag(actions);
    }
private:
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void startDrag(Qt::DropActions actions);
signals:
    void sDragEnter(QDragEnterEvent *event);
    void sDragLeave(QDragLeaveEvent *event);
    void sDragMove(QDragMoveEvent *event);
    void sDrop(QDropEvent *event);
    void sStartDrag(Qt::DropActions actions);
};

//>@--UpMouseQTreeWidget
class UpMouseQTreeWidget : public UpQTreeWidget
{
    Q_OBJECT
public:
    UpMouseQTreeWidget(QWidget *parent = 0);
    ~UpMouseQTreeWidget();

    void downMousePressEvent(QMouseEvent *event)
    {
        UpQTreeWidget::mousePressEvent(event);
    }
private:
protected:
    void mousePressEvent(QMouseEvent *event);
signals:
    void sMousePressEvent(QMouseEvent *event);
private slots:
};

/**
//>@UpQTableWidget******************************************************
*/
class UpQTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    UpQTableWidget(QWidget *parent = 0);
    ~UpQTableWidget();

    void ClearAllItem();

    void UpDragEnterEvent(QDragEnterEvent *event)
    {
        QTableWidget::dragEnterEvent(event);
    }
    void UpDragLeaveEvent(QDragLeaveEvent *event)
    {
        QTableWidget::dragLeaveEvent(event);
    }
    void UpDragMoveEvent(QDragMoveEvent *event)
    {
        QTableWidget::dragMoveEvent(event);
    }
    void UpDropEvent(QDropEvent *event)
    {
        QTableWidget::dropEvent(event);
    }
    void UpStartDrag(Qt::DropActions actions)
    {
        QTableWidget::startDrag(actions);
    }
private:
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void startDrag(Qt::DropActions actions);
signals:
    void sDragEnter(QDragEnterEvent *event);
    void sDragLeave(QDragLeaveEvent *event);
    void sDragMove(QDragMoveEvent *event);
    void sDrop(QDropEvent *event);
    void sStartDrag(Qt::DropActions actions);
};

/**
//>@UpQListWidget******************************************************
*/
class UpQListWidget : public QListWidget
{
    Q_OBJECT
public:
    UpQListWidget(QWidget *parent = 0);
    ~UpQListWidget();

    void UpDragEnterEvent(QDragEnterEvent *event)
    {
        QListWidget::dragEnterEvent(event);
    }
    void UpDragLeaveEvent(QDragLeaveEvent *event)
    {
        QListWidget::dragLeaveEvent(event);
    }
    void UpDragMoveEvent(QDragMoveEvent *event)
    {
        QListWidget::dragMoveEvent(event);
    }
    void UpDropEvent(QDropEvent *event)
    {
        QListWidget::dropEvent(event);
    }
    void UpStartDrag(Qt::DropActions actions)
    {
        QListWidget::startDrag(actions);
    }
private:
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void startDrag(Qt::DropActions actions);
signals:
    void sDragEnter(QDragEnterEvent *event);
    void sDragLeave(QDragLeaveEvent *event);
    void sDragMove(QDragMoveEvent *event);
    void sDrop(QDropEvent *event);
    void sStartDrag(Qt::DropActions actions);
};

//>@--UpSelecQListWidget
class UpSelecQListWidget : public UpQListWidget
{
    Q_OBJECT
public:
    UpSelecQListWidget(QWidget *parent = 0);
    ~UpSelecQListWidget();
private:
protected:
signals:
private slots:
    void slot_ItemClicked(QListWidgetItem* pItem);
};

/**
//>@UpQGraphicsView******************************************************
*/
class UpQGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    UpQGraphicsView(QWidget *parent = 0);
    ~UpQGraphicsView();

    void DownDropEvent(QDropEvent *event)
    {
        QGraphicsView::dropEvent(event);
    }
    virtual void drawBackground(QPainter *painter, const QRectF &rect);
protected:
    void resizeEvent(QResizeEvent *event);
    void dropEvent(QDropEvent *event);
signals:
    void sDropEvent(QDropEvent*);

public:
    bool        m_FitInView;
};

#ifndef QT_V453

QListWidgetItem* GetListItem(QListWidget *pListWidget, QString pPath, QString pName);
QListWidgetItem* AddListItem(QListWidget *pListWidget, QString pPath, QString pName);
bool DelListItem(QListWidget *pListWidget, QListWidgetItem *pItem);
bool DelListItem(QListWidget *pListWidget, QString pPath, QString pName);

QListWidgetItem* GetListCurItem(QListWidget *pList);
int GetListCurRow(QListWidget *pList);
bool SetListCurRow(QListWidget *pList, int pRow, bool pMiddle=false);

//>@?§?? ???í???
class ListWidget : public UpQListWidget
{
    Q_OBJECT
public:
    ListWidget(QWidget *parent = 0);

    void SetAlignMode(bool pEnable, int pMode = 2, bool pAnimat = true);
    void InitMovePara(DIRECTION pDirection, QSizeF pMinMatrix, QSizeF pMatrix=QSizeF());
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

public slots:
    void slot_ItemCountChanged(QSizeF pMatrix);

signals:
    void sItemChanged(int pRow);

public:
    DIRECTION                 m_SliderDirection;
    QPoint                    m_MousePosBk;

    bool                      m_AlignEnable;  //>@ ?·?×???????
    int                       m_AlignMode;    //>@ 1:°′??×óé?????DD????￡? 2￡?°′???D??μ???DD????
    bool                      m_AnimatEnable;

    QSizeF                    m_MinMatrix;
    QSizeF                    m_ItemSize;

    NUM_INFO                  m_HScrollValue;
    qreal                     m_HRelDiff;  //>@widgetò???ò???????Dèò?ò???scrool?àéù
    QPointer<QScrollBar>      m_HScrollBar;
    QPropertyAnimation       *m_HAnimation;

    NUM_INFO                  m_VScrollValue;
    qreal                     m_VRelDiff;
    QPointer<QScrollBar>      m_VScrollBar;
    QPropertyAnimation       *m_VAnimation;

    bool                      m_IsMoving;
    QEvent::Type              m_MouseStatus;
};

class TimeListWidget : public ListWidget
{
    Q_OBJECT
public:
    TimeListWidget(QWidget *parent = 0);

public slots:
    void slot_Plus();
    void slot_Minus();
};


class TableWidget : public UpQTableWidget
{
    Q_OBJECT
public:
    TableWidget(QWidget *parent = 0);

    void InitMovePara(DIRECTION pDirection, QSizeF pMinMatrix, QSizeF pMatrix=QSizeF());
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

public slots:
    void slot_ItemCountChanged(QSizeF pMatrix);

public:
    DIRECTION                 m_SliderDirection;
    QPoint                    m_MousePosBk;
    qreal                     m_HRelDiff, m_VRelDiff;  //>@widgetò???ò???????Dèò?ò???scrool?àéù

    QSizeF                    m_MinMatrix;
    QSizeF                    m_ItemSize;

    QPointer<QScrollBar>      m_HScrollBar;
    QPointer<QScrollBar>      m_VScrollBar;
    NUM_INFO                  m_HScrollValue;
    NUM_INFO                  m_VScrollValue;

    bool                      m_IsMoving;
    QEvent::Type              m_MouseStatus;
};

class TreeWidget : public UpQTreeWidget
{
    Q_OBJECT
public:
    TreeWidget(QWidget *parent = 0);

    void InitMovePara(DIRECTION pDirection);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

public:
    DIRECTION                 m_SliderDirection;
    QPoint                    m_MousePosBk;
    qreal                     m_HRelDiff, m_VRelDiff;  //>@widgetò???ò???????Dèò?ò???scrool?àéù

    QPointer<QScrollBar>      m_HScrollBar;
    QPointer<QScrollBar>      m_VScrollBar;
    NUM_INFO                  m_HScrollValue;
    NUM_INFO                  m_VScrollValue;

    bool                      m_IsMoving;
    QEvent::Type              m_MouseStatus;
};

#endif

/*****************************************************
                Show  instrument
  ****************************************************/

class DieselInstrument : public QWidget
{
    Q_OBJECT
public:
    explicit DieselInstrument(QWidget *parent = 0);

public slots:
    void setValue(int newvalue);
    void init();

public:
    void paintEvent(QPaintEvent *event);
    int mValue;
};

class SpeedInstrument : public QWidget
{
    Q_OBJECT
public:
    explicit SpeedInstrument(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void focusOutEvent(QFocusEvent *e);

public slots:
    void setValue(int);
    void init(void);

protected:
    void paintEvent(QPaintEvent *event);
    int mValue;

    bool m_DragFlag;
};

class TimeInstrument : public QWidget
{
    Q_OBJECT

public:
    TimeInstrument(QWidget *parent = 0);
    ~TimeInstrument()
    {
        if(m_Timer)
        {
            m_Timer->stop();
            m_Timer = 0;
        }
    }

    void Initial(CLOCK_TYPE pType);

    void updateEffect(CLOCK_INFO pTime);

    bool LoadPixmap(int pKey, QString pPixmapName);
    bool LoadPixmap(int pKey, QString pPixmapName, QSize pSize);
    //>@è?????×÷×??à??Dèò?á???￡?è???óDá???￡??òμúò??????¨ ?×′ì?CHANGE￡?μú?t??ò??¨ ?×′ì?μ??ù×÷±???
    void OperateStep0();
public slots:
    void OperateStep1();
    void UpdateTime();

protected:
    void StartTimeline(int pStart, int pEnd, int pInterval, QTimeLine::CurveShape pShape);
    bool DrawPixmap(QPainter &p, QPixmap pPixmap, QRect pTargetRect);
    bool DrawPixmap(QPainter &p, QPixmap pPixmap, QRect pTargetRect, QRect pSourceRect);
    void CalcElementInfo(CLOCK_TYPE pType);
    void CalcAnalogClockPointer();
    void paintEvent(QPaintEvent *event);
    void Paint2D_None(QPainter &p, QRect targetRect);
    void Paint2D_Flip(QPainter &p,QRect targetRect);
    void Paint2D_Roll(QPainter &p,QRect targetRect);

public:
    QTimer                             *m_Timer;
    QHash<int, QPixmap>                 m_EffectImage;  //>@?ó??μ??ú′?μ?× ?′
    QTimeLine                           m_Animator;
    QRect                               m_Rect;         //>@??óò′óD?
    bool                                m_EffectValid;

    OPERATE_INFO                        m_OperateInfo[2];
    PIXMAP_POINTER                      m_PixmapPointer;

    CLOCK_PATTERN                       m_Parttern;
    CLOCK_TYPE                          m_Type;
    AREA_ANIMATE                        m_AreaAnimate;

    //>@Digital : í???(???? ? y×?μ?ò???????óò)?? y??19??￡?RectYear0~RectSecond1 ---- 0~9 SUN~WES : -
    //>@          í???× ?′?? y??10+7+1+1 = 19
    //>@          0~9 Sun~Sat : -
    //>@Analog : í????? y??11+3??￡?RectYear0~RectWeek   ±???? ·????? ??????
    //>@         í???× ?′?? y??10+7+1+4+1 = 23(1??±í??×?é???μ?μ?)??
    //>@         0~9 Sun~Sat -  ±???? ·????? ??????  ±?óμ×í?
    QHash<CLOCKE_ELEMENT, CLOCK_ELEMENTINFO*>    m_ClockElement;
    qreal                    m_Diff;                 //>@í???ò?′?ò???(Dy×?)?àà?
    CLOCK_INFO               m_SavedTime;
    qreal                    m_ItemWidth, m_ItemHeight;
    int                      m_FontSize;

    //>@????Analog ±?ó
    qreal       m_PointerLength[3];
    qreal       m_PointerAngle[3][2];   //>@??óú????￡???óò???¤?è?í???è(′yDy×?μ????è￡?ò??′???è??)
};

class PieInstrument : public QWidget
{
    Q_OBJECT

public:
    PieInstrument(QWidget *parent = 0);
    ~PieInstrument();

    void paintEvent(QPaintEvent *e);
};


/******************************************************
        AxWidget
  ****************************************************/
#ifdef SURRPORT_ACTIVEX
#include <QAxWidget>

class AxWidget : public QAxWidget
{
public:
    AxWidget(QWidget* parent = 0);
    ~AxWidget();

    void PlaySwf(QString pFile);
    void OpenWebPagInIE(QString pUrl);
    void OpenXls(QString pFile);
    void OpenDoc(QString pFile);
};

#endif

#endif // TOOLBUTTON_H

