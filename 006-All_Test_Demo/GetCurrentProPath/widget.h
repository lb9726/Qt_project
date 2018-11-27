#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    QString getCurrenFileAbsolutePath(QString appPath, QString curFileRelativePath);
};

#endif // WIDGET_H
