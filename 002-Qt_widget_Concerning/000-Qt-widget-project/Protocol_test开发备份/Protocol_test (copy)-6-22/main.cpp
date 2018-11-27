#include "widget.h"
#include "fileupdate.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication a(argc, argv);
//    Widget w;
//    w.show();
//    w.showFullScreen();
//    FileUpdate f;
//    f.show();
/*    Protocoldeal *pro = */
    Protocoldeal::GetInstance();
//    if (NULL != pro)
//    {
//        if (pro->GetVersionFlag())
//        {
//            FileUpdate *f = new FileUpdate;
//            f->show();
//            connect(f, SIGNAL(DeStroyScreenSignal(FileUpdate*)), f, SLOT(OnDeStroyScreen(FileUpdate*)));
//        }
//        else
//        {
//            Widget w;
//            w.show();
//        }
//    }
    return a.exec();
}
