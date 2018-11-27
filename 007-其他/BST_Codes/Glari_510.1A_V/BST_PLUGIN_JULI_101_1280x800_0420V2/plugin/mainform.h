#ifndef MAINFORM_H
#define MAINFORM_H

#include "../interface/pluginwidget.h"
#include "../effect/effectbase.h"
#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainForm;
}

class MainForm;

class MainForm : public PluginWidget
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
public:
    explicit MainForm(QWidget *parent = 0);
    ~MainForm();

    void InitUI(EventMap *pEventMap, QString pRcPath = QString());
    MainForm* Widget() { return this; }
    void ExecOperate(AREA_OPERATE pOperate, QVariant pPara);

    void ParseFloor(int pType = 0);

signals:
    void sImageChange();

public slots:
    void slot_Tick();
    void slot_DateTime(AREA_OPERATE pOperate, QVariant pPara);
    void slot_ImageChange();

public:
    Ui::MainForm        *ui;

    FLOOR_INFO          mFloor_bk;
    ARROWSTATE          mArrow_bk;
    quint32             mResvFunc_bk;

    quint8              mButtons_bk[7];
};

#endif // MAINFORM_H
