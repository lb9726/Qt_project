#ifndef EXEUPDATE_H
#define EXEUPDATE_H

#include <QObject>
#include <QString>
class ExeUpdate : public QObject
{
    Q_OBJECT
public:
    explicit ExeUpdate(QObject *parent = 0);

signals:

public slots:
    bool checkUpdate();
    bool updateFile(QString filePath, int type);//文件路径;type:0-本地文件, 1- 网络更新。
    //网络时先检测是否存在可用更新，待生成包时再实际下载
private:
   bool m_supportNetUpdate;//是否支持网络检测更新
   QString exefilePath;
   QList<QString> updateinfomation;
};

#endif // EXEUPDATE_H
