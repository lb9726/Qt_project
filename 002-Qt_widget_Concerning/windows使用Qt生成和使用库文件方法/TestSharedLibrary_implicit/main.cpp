#include "widget.h"
#include <QApplication>
#include "sharedlibrary.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SharedLibrary sl;
    sl.subtract(3,5);
    sl.updateBackground();
    sl.show();
    return a.exec();
}
