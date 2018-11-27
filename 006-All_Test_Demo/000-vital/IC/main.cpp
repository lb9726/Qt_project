#include "icmanager.h"
#include <QApplication>
#include "loginform.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec*codec=QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
#if (QT_VERSION <= QT_VERSION_CHECK(4, 9, 9))
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif

    LoginForm tmpForm;
    int ret = tmpForm.exec();
    if(ret == QDialog::Rejected)
        return 0;

    ICManager w;
    w.mAdminName = tmpForm.mUsrName;
    w.showMaximized();
    return a.exec();
}
