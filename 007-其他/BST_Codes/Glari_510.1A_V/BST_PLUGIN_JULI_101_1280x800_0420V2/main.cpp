#include <QtGui/QApplication>
#include "./plugin/mainform.h"
#ifdef LINUX
#include <QWSServer>
#endif
#include<QDebug>

#ifdef X86
#include <QScrollArea>
#endif
int main(int argc, char *argv[])
{
#ifdef LINUX
    QApplication a(argc, argv, QApplication::GuiServer);
    QWSServer::setCursorVisible(false);
#else
    QApplication a(argc, argv);
#endif

    QTextCodec*codec=QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    MainForm *tmpForm = new MainForm;
#ifdef X86
    QScrollArea w;
    //w.setWindowFlags(Qt::FramelessWindowHint);
    w.setWidget(tmpForm);
    QString tmpPath = QString("%1/../../usrfs/THEME/PLUGIN/").arg(QApplication::applicationDirPath());
    IDE_TRACE_STR(tmpPath);
    tmpForm->InitUI((EventMap *)0, tmpPath);
    w.resize(522, a.desktop()->height()-80);
    w.show();
#else
    QString tmpPath = QString("/usr/bst/THEME/PLUGIN/").arg(QApplication::applicationDirPath());
    tmpForm->InitUI((EventMap *)0, tmpPath);
    tmpForm->setWindowFlags(Qt::FramelessWindowHint);
#endif
    tmpForm->show();

    return a.exec();
}
