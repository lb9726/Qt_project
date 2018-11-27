#ifndef SHOWSCREEN_H
#define SHOWSCREEN_H

#include <QObject>
#include "widget.h"
#include "fileupdate.h"
#include "protocoldeal.h"
#include <QThread>
class ShowScreen : public QObject
{
    Q_OBJECT
public:
    explicit ShowScreen(QObject *parent = 0);
    ~ShowScreen();
signals:

public slots:
    void ChoiceScreen(int index);
    void OnHideScreen(int index);
private:
    Protocoldeal *pro;
    Widget *wid;
    FileUpdate *fileup;
};

#endif // SHOWSCREEN_H
