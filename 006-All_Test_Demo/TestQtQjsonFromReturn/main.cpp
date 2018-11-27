#include <QCoreApplication>
#include "qjsontest.h"
#include <iostream>
#include <QByteArray>
using namespace std;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QjsonTest tmpjson;
    std::string tmps;
    QByteArray tmpByteArray = tmpjson.generateJsonData();
    tmps = tmpByteArray.toStdString();
    cout<<"Content-Type:text/html\n\n";
    cout<< tmps<<endl;
    return 0;
}
