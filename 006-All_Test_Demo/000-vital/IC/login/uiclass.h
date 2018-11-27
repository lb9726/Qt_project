#ifndef UICLASS_H
#define UICLASS_H

#include "globalfun.h"

class UiClass
{
public:
    virtual void SetFlag(quint32 pFlag, QVariant pPara = QVariant()) { m_Flag.insert(pFlag, pPara); }
public:
    QHash<quint32, QVariant>   m_Flag;
};

class LinkButton : public QPushButton, public UiClass
{
    Q_OBJECT
public:
    LinkButton(QWidget *parent = 0);
    LinkButton(const QString &text, QWidget *parent=0);

    enum{
        FLAG_FRAME,
        FLAG_STYLESHEET
    };

    void Init();

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    bool event(QEvent *e);
    void paintEvent(QPaintEvent *e);

public:
    int         m_State;
};

class LineEdit : public QLineEdit, public UiClass
{
    Q_OBJECT
public:
    LineEdit(QWidget *parent = 0);
    LineEdit(const QString &pText, QWidget* parent=0);

    void Init();

    void SetStatusTip(QString pText);
    void SetText(QString pText);

    void focusOutEvent(QFocusEvent *e);
    void focusInEvent(QFocusEvent *e);

signals:
    void sFocusOut();
    void sFocusIn();

public slots:
    void slot_ResetCursorPos();
    void slot_TextChanged(QString pText);
    void slot_SelectionChanged();
    void slot_CursorPositionChanged(int pOld,int pNew);

public:
    QString        mText;
    bool           mPasswdEcho;
};

namespace Ui {
class LineEditForm;
}

class LineEditForm : public QWidget, public UiClass
{
    Q_OBJECT

public:
    explicit LineEditForm(QWidget *parent = 0);
    ~LineEditForm();

    enum{
        FLAG_ICON,
        FLAG_ALIGN,
        FLAG_PASSWD
    };
    void SetFlag(quint32 pFlag, QVariant pPara = QVariant());
    void SetText(QString pText);
    QString Text();
    void SetStatusTip(QString pText);

    void SetStatus(int pFlag);  //pError : 0表示错误，1：表示正常
    void paintEvent(QPaintEvent *);

signals:
    void sFocusOut();
    void sFocusIn();

public slots:
    void slot_FocusOut();
    void slot_FocusIn();

private:
    int                 mFlag;
    bool                mHasFocus;
    Ui::LineEditForm *ui;
};

class ImageForm : public QWidget, public UiClass
{
    Q_OBJECT
public:
    explicit ImageForm(QWidget *parent = 0);
    ~ImageForm();

    void Add(QPixmap pPixmap, bool pInsert = false);
    void Show(QPixmap pPixmap, bool pInsert = false);
    void paintEvent(QPaintEvent *e);

public slots:
    void slot_Timeout();

public:
    QTimer              m_Timer;
    QList<QPixmap>      m_ImgList;
    int                 m_ImgIndex;
};

#endif // UICLASS_H
