#include <QCoreApplication>


#include <QFile>
#include <QString>
#include <QByteArray>
#include "tcpclient.h"

extern "C"
{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "cgic.h"
}

void writeHtmlTextLengthToFile(const QString& fileName, int len);
void writeHtmlTextToFile(const QString& fileName, char *pStr);
void getHtmlClientData(char *pStr);

int cgiMain()
{
//    int argc = 1;
//    char **argv = NULL;

//    QCoreApplication a(argc, argv);

    char pass[1024];
    getHtmlClientData(pass);
//    QByteArray tmpArray(pass);
//    TcpClient tmpClient;
//    tmpClient.connectToServer();
//    tmpArray = tmpClient.generateBuffer(tmpArray);
//    tmpClient.writeMsgToServer(tmpArray, tmpArray.length());
    cgiHeaderContentType("text/html");
    fprintf(cgiOut, "Your String is %s\n", pass);
    return 0;
//    return a.exec();
}

void writeHtmlTextLengthToFile(const QString &fileName, int len)
{
    QString str = QString::number(len);
    QFile tmpFile(fileName);
    if (tmpFile.open(QIODevice::WriteOnly))
    {
        tmpFile.write(str.toLatin1().data(), str.length());
    }
    tmpFile.close();
}

void writeHtmlTextToFile(const QString &fileName, char *pStr)
{
    QFile tmpFile(fileName);
    if (tmpFile.open(QIODevice::WriteOnly))
    {
        tmpFile.write(pStr, strlen(pStr));
    }
    tmpFile.close();
}

void getHtmlClientData(char *pStr)
{
    char *req_method = NULL;
    FILE *fp = stdin;
    char *input = NULL;
    req_method = getenv("REQUEST_METHOD");
    int i = 0;
    int len = 0;
    if (!strcmp(req_method, "GET"))
    {
        input = getenv("QUERY_STRING");
        memcpy(pStr, input, strlen(input));
    }
    else if (!strcmp(req_method, "POST"))
    {
        len = atoi(getenv("CONTENT_LENGTH"));

//        writeHtmlTextLengthToFile(QString("/home/libo/Desktop/000-libo-len.txt"), len);
//        writeHtmlTextLengthToFile(QString("/usr/bst/000-libo-len.txt"), len);
        if (0 == len)
        {
            pStr[0] = '\0';
        }
        else
        {
            while(1)
            {
                pStr[i] = (char)fgetc(fp);
                len--;
                if (0 == len)
                {
                    pStr[i+1] = '\0';
                    break;
                }
                ++i;
            }
        }
    }
//    writeHtmlTextToFile(QString("/home/libo/Desktop/000-libo.txt"), pStr);
//    writeHtmlTextToFile(QString("/usr/bst/000-libo.txt"), pStr);
}
