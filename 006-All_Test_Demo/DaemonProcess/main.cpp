#include <QCoreApplication>
#include "mydaemon.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MyDaemon tmpMyDaemon;
    tmpMyDaemon.startExternProcess();
    return a.exec();
}
