#ifndef SHAREDLIBRARY_H
#define SHAREDLIBRARY_H

#include "sharedlibrary_global.h"
#include <QWidget>
class SHAREDLIBRARYSHARED_EXPORT SharedLibrary: public QWidget
{
    Q_OBJECT
public:
    explicit SharedLibrary(QWidget *parent = 0);
    void updateBackground();
    int subtract(int a, int b);

private slots:
    void onClicked();
};

extern "C" {
SHAREDLIBRARYSHARED_EXPORT SharedLibrary* getSharedLibrary(); //获取类SharedLibrary对象
SHAREDLIBRARYSHARED_EXPORT int add(int a, int b);  //自定义的外部函数
}

#endif // SHAREDLIBRARY_H
