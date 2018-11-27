#ifndef TEMPLETELOAD_H
#define TEMPLETELOAD_H

#include "uieditor.h"
#include <QObject>
//加载所支持的ui文件模板
class TempleteLoad : public QObject
{
    Q_OBJECT
public:
    explicit TempleteLoad(QObject *parent = 0);

signals:

public slots:
    void scanTemplete();
    QStringList getTemplete();
    bool createTemplete(QString path, QString proName, int tempIndex);

private:
    QStringList m_templeteList;
    QString m_templeteDir;
    UiEditor* m_editor;
};

#endif // TEMPLETELOAD_H
