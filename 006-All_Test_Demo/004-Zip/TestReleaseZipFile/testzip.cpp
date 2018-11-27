#include "testzip.h"
#include <QCoreApplication>

TestZip::TestZip(QObject *parent) : QObject(parent)
  , m_runPath(QCoreApplication::applicationDirPath())
{
    IDE_TRACE();
}

bool TestZip::UnZip(QString pSrc, QString pDst, qint64 pTimeout, bool isLocal)
{
    Q_UNUSED(pTimeout);
    Q_UNUSED(isLocal);
    IDE_DEBUG(QString("is call3"));
//    ztt.startZip();
    ztt.startUnZip(pSrc, pDst, true);
    return true;
}

void TestZip::init()
{
    IDE_DEBUG(QString("is call1"));
    m_runThread = true;
    QString pSrc = m_runPath + "/CanFestival-3-8bfe0ac00cdb.zip";
    QString pDst = m_runPath + "/000Test";
    IDE_DEBUG(QString("is call2"));
    UnZip(pSrc, pDst, 60000, true);
    IDE_DEBUG(QString("is call4"));
}

void TestZip::sProgress(quint32 ppro)
{
    if (ppro >= 100)
    {
        m_runThread = false;
    }
    IDE_TRACE_INT(ppro);
}



