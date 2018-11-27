#include "widget.h"
#include <QApplication>
#include "sharedlibrary.h"
#include <QLibrary>
#include <QDebug>
#include <QMessageBox>

typedef SharedLibrary* (*GetSharedLibrary)();//定义函数指针,获取类TestDLL对象;
typedef int (*Fun)(int, int); //定义函数指针,int add(int a,int b);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //    SharedLibrary w;
    //    w.resize(400, 200);
    //    w.updateBackground();
    //    w.show();

    //    int nSubtract = w.subtract(10, 4);
    //    int nAdd = add(5, 5);
    //    QMessageBox::information(NULL, "Tip", QString("subtract:%1  add:%2").arg(nSubtract).arg(nAdd));

    //声明所用到的dll文件
    QLibrary mylib("H:/Qt_windows_projects_total/TestSharedLibrary/SharedLibrary/SharedLibrary.dll");

    //判断是否正确加载
    if (mylib.load())
    {
        qDebug() << "library loaded";

        GetSharedLibrary getSharedLibrary = (GetSharedLibrary)mylib.resolve("getSharedLibrary");
        Fun add = (Fun)mylib.resolve("add");
        if (getSharedLibrary)
        {
            SharedLibrary *pLib = getSharedLibrary();
            pLib->updateBackground();
            pLib->subtract(10, 4);
            pLib->show();
            pLib->setWindowTitle("Test ShareLibrary");
        }
        else
        {
            qDebug() << "Could not show widget from the loaded library";
        }

        if (add)
        {
            int nResult = add(5, 5);
            QMessageBox::information(NULL, "Tip", QString::number(nResult));
        }
        else
        {
            qDebug() << "Could not add from the loaded library";
        }
    }
    //加载失败
    else
    {
        qDebug() << mylib.errorString();
    }
    return a.exec();
}
