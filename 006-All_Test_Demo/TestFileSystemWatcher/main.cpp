#include "mainwindow.h"
#include <QApplication>
#include "filesystemwatcher.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    FileSystemWatcher::addWatchPath("/home/libo/Desktop/001-Compass/");
    return a.exec();
}
