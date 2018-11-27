#ifndef FLRMAPBAR_H
#define FLRMAPBAR_H

#include "global.h"
#include "bstui.h"
#include "messagebox.h"
#include "textinput.h"

namespace Ui {
class FlrmapBar;
}

class DeviceBase;

QString StrToAscii(QString pStr);
QString NumToAscii(int pNum);

class FlrmapBar : public QWidget
{
    Q_OBJECT
    
public:
    explicit FlrmapBar(QWidget *parent = 0);
    ~FlrmapBar();
    
    void SetCanDel(bool pFlag);
    void SetShowPreview(bool pFlag);

    bool isValid();

    void ClearInputFocus();

    QString GetLevle();
    QString GetMapping();

    bool SetMap(RC_MAP pFloorMap, RC_MAP pAudioMap);
    bool GetMap(RC_MAP &pFloorMap, RC_MAP &pAudioMap);

signals:
    void sActive();
    void sDelete();

private slots:
    void slot_Resize();

    void on_btn_level_clicked();
    void on_btn_mapping_clicked();
    void on_btn_audio_clicked();
    void on_btn_del_clicked();

    void slot_ChangeRcs();
    void slot_ValidLevel(QString pText);
    void slot_Preview(QString pText);
    void on_btn_audtest_clicked();

    void on_btn_switch_clicked();

public:
    DeviceBase    *m_DeviceManager;
    Ui::FlrmapBar *ui;
};

#endif // FLRMAPBAR_H

