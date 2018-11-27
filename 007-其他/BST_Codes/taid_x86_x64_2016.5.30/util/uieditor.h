#ifndef UIEDITOR_H
#define UIEDITOR_H
#include <QObject>
#include "uiparser.h"

class UiEditor : public QObject
{
    Q_OBJECT
public:
    explicit UiEditor(QObject *parent = 0);

    bool InitCom(QString pPath);
    bool AddCom(UI_TYPE pComType, QDomElement &pComElement);
    bool RemoveUI(UI_TYPE pComType);
    void ReleaseUIs();

signals:

public slots:

public:
    DIRECTION                               mDirection;
    UiParser                                *mUiManager;
    QHash<int, UiComParser*>                mUiList;

};

#endif // UIEDITOR_H
