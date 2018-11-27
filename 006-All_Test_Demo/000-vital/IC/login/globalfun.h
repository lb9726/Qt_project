#ifndef GLOBALFUN_H
#define GLOBALFUN_H

#include "define.h"

QIcon GetIcon(QPixmap pPixmap, QRect pRect);
QPixmap GetPixmap(QPixmap pPixmap, QRect pRect);

bool isDigitStr(QString src);

void QtSleep(unsigned int msec, QEventLoop::ProcessEventsFlags flags = QEventLoop::AllEvents);
bool QtWait(bool &pFlag, bool pEqlFlag, unsigned int msec);
bool QtWait(int &pFlag, int pEqlFlag, unsigned int msec);

//>@注意key不能为“/”，否则会死机
class iniParse : public QSettings
{
    Q_OBJECT
public:
    iniParse(QString pfileName);
    void setProperty(QString key,QString value) { setValue(key, value); }
    void setProperty(QString key,int value)     { setValue(key, value); }

    bool getProperty(QString pkey, QString &pValue);
    bool getProperty(QString pkey, int &pValue);

    /*
    可以直接读取中文的QSTRING的，不过真正的意义在于把QSETTING的读取返回值放入 到QByteArray中才是关键。
    QTextCodec *codec=QTextCodec ::codecForName("GBK"); //设定字符集为GBK（大小写均可，还可用GB18030代替）
    QString outStr = codec->toUnicode(myArray); //这里就好解释了，通过译码器将字节数组中的值转化为UNICODE的QSTRING就O了
    */
};

#endif // GLOBALFUN_H
