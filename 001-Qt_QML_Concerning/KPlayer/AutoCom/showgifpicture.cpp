#include "showgifpicture.h"
#include <QString>
#include <QStringList>

showGifPicture::showGifPicture(QObject *parent) : QObject(parent)
{

}

QVariant showGifPicture::isShowGif(QString filename)
{
    if (!filename.isEmpty())
    {
        QString tmp = filename.split(".").last();
        if ("mp3" == tmp || "wav" == tmp)
        {
            return true;
        }
        return false;
    }
    else
    {
        return false;
    }
}

