#include <QApplication>
#include <QQuickView>
#include <QWidget>
#include "taid.h"

#if defined Q_OS_WIN32
#include <windows.h>
bool checkOnly()
{
    HANDLE m_hMutex  =  CreateMutex(NULL, FALSE,  L"taid_2016_05_27" );
    if  (GetLastError()  ==  ERROR_ALREADY_EXISTS)  {
     CloseHandle(m_hMutex);
     m_hMutex  =  NULL;
      return  false;
    }
    else{
        return true;
    }
}
#endif


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    if(!checkOnly()){
        QMessageBox msgBox;
        msgBox.setText("A taid instance already running, this will quit.");
        msgBox.exec();
        return 1;
    }
    QApplication::addLibraryPath("./plugins");
    app.setWindowIcon(QIcon("icon.ico"));
    QQmlApplicationEngine engine;
    Taid  *taid = new Taid(&engine);
    taid->Init();
    engine.load(taid->mainQml);
    int ret = app.exec();
    delete taid;
    return ret;
}
