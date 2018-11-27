#ifndef UIBASE_H
#define UIBASE_H

#include <QObject>
#include <QHash>
#include <QDeclarativeItem>

#include "globalfun.h"
#include "themeparser.h"

class RcItem
{
public:
    RcItem() {mRcType = RC_INVALID; clear();}
    ~RcItem() {clear();}
    void clear();
public:
    QDomElement             mElement;
    QString                 mRcFile;    ///资源文件路径
    RC_TYPE                 mRcType;    ///资源类型
    QString                 mTipText;   ///提示文本
    QHash<int, QVariant>    mParameterHash;  ///对于某个资源的参数定义
};

class UiBase : public QObject
{
    Q_OBJECT
public:
    UiBase(QObject *parent = NULL);
    UiBase(ThemeParser *pthemeManager);
    UiBase(ThemeParser *pthemeManager,QDomElement &pElement);
    ~UiBase();

    void setThemeManager(ThemeParser *pthemeManager);

    QRectF getComRect(DIRECTION direction, int layout);
    bool getComResource(int index, QString &rcPath, QString &tipText);

    bool initComponent(QDomElement &pElement);
    void releaseRC();

    void parserParameter(QString pParameter);
    RcItem *loadRc(int pKey, QDomElement pRcElement);
    virtual QString getResourcePath(int index);

    virtual bool initUi(DIRECTION direction);
    virtual bool initGeometry(DIRECTION direction, int layout);

    bool invokeQmlMethod(char *name,QVariant arg1,QVariant arg2,QVariant arg3,QVariant arg4);
    bool invokeQmlMethod(char *name, QVariant arg1, QVariant arg2, QVariant arg3);
    bool invokeQmlMethod(char *name, QVariant arg1, QVariant arg2);
    bool invokeQmlMethod(char *name, QVariant arg1);
    bool setQmlProperty(char *name, const QVariant &value);

signals:

public slots:

public:
    QDomElement                         m_ComElement;
    COM_TYPE                            m_ComType;
    QString                             m_ComPath;
    QRectF                              m_ComRectLayout[2];
    QRectF                              m_ComRectLayout1[2];

    QHash<int, RcItem*>                 m_ComponentRCs;
    QMap<QString, QString>              m_ComponentParameter;
    ThemeParser *themeManager;
    QDeclarativeItem* m_QmlItem;

    static QString themeResourceDir;
    static QString bundleResourceDir;
};

#endif // UIBASE_H
