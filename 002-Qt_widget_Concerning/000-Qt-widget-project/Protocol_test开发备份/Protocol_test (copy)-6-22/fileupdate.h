#ifndef FILEUPDATE_H
#define FILEUPDATE_H

#include <QWidget>
#include <QDebug>
#include "protocoldeal.h"
namespace Ui {
class FileUpdate;
}

class FileUpdate : public QWidget
{
    Q_OBJECT

public:
    explicit FileUpdate(QWidget *parent = 0);
    ~FileUpdate();
    void SetWidgetBackGround(QString path);
public slots:
    void ChangeValue(int val);
//    void OnDeStroyScreen(FileUpdate *f);
private:
    Ui::FileUpdate *ui;
signals:
    void DeStroyScreenSignal(FileUpdate *f);
};

#endif // FILEUPDATE_H
