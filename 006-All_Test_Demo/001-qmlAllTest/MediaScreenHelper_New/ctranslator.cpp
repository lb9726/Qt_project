#include "ctranslator.h"
#include <QFile>
CTranslator* CTranslator::m_instance = NULL;
CTranslator::Garbo CTranslator::garbo;

CTranslator::CTranslator(QObject *parent)
    : QTranslator(parent)
    , runPath(QCoreApplication::applicationDirPath())
    , filename("")
{

}

/*加载相应的切换的翻译文件*/
void CTranslator::load(int index)
{
    qDebug()<<"runpath = " <<runPath;
    switch (index) {
    case 0:
        filename = "language_china";
        break;
    case 1:
        filename = "language_traditionchina";
        break;
    case 2:
        filename = "language_english";
        break;
    case 3:
        filename = "language_Russian";
        break;
    default:
        break;
    }
    // 得到需要加载的翻译的文件的文件路径和名字
    bool flag = QTranslator::load(QString("%1/%2/%3").arg(runPath).arg("translations").arg(filename));
    if (flag)
    {
        qApp->installTranslator(CTranslator::instance()); //重新安装翻译
    }
    qDebug()<<"filename = " <<filename;
    qDebug()<<"flag = " << flag;
    emit languageChanges();
}

void CTranslator::exitApp(int code)
{
    qDebug()<<__PRETTY_FUNCTION__;
    qApp->exit(code);
}

CTranslator *CTranslator::instance()
{
    if (!m_instance)
    {
        m_instance = new CTranslator();
    }
    return m_instance;
}

bool CTranslator::createChooseUiFile()
{
    QString filename = runPath + "/config.con";
    QFile file(filename);
    bool flag = file.open(QIODevice::WriteOnly);
    qDebug()<<"flag = "<< flag;
    return flag;
}

bool CTranslator::writeConfigFile(int index)
{
    qDebug()<<__PRETTY_FUNCTION__;
    QString filename = runPath + "/config.con";
    QFile file(filename);
    bool flag = file.open(QIODevice::WriteOnly);
    qDebug()<<"flag = "<< flag;
    char c = index + '0';
    file.write(&c);
    return flag;
}

char CTranslator::readConfigFile()
{
    qDebug()<<__PRETTY_FUNCTION__;
    QString filename = runPath + "/config.con";
    QFile file(filename);
    bool flag = file.open(QIODevice::ReadOnly);
    qDebug()<<"flag = "<< flag;
    char c;
    file.read(&c, 1);
    return c;
}
