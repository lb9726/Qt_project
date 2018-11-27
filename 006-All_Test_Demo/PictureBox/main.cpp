#include <QApplication>
#include "picturebox.h"
#include <QImage>
#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PictureBox mpicture;
    QString mRunPath = QCoreApplication::applicationDirPath();
    mpicture.setMode(mpicture.FIX_SIZE_CENTRED);
    QImage mmimg(mRunPath + "/0.png");
    qDebug()<<"mRunPath = "<< mRunPath;
    mpicture.setImage(mmimg, 1);
    mpicture.show();
    return a.exec();
}
