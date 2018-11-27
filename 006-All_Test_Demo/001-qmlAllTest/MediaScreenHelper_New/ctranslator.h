#ifndef CTRANSLATOR_H
#define CTRANSLATOR_H

#include <QTranslator>
#include <QCoreApplication>
#include <QDebug>
#include <QString>

class CTranslator:public QTranslator
{
    Q_OBJECT
private:
    CTranslator(QObject *parent = 0);
    static CTranslator *m_instance;
    class Garbo
    {
    public:
        ~Garbo()
        {
            if (NULL != CTranslator::m_instance)
            {
                qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"will delete m_instance";
                delete CTranslator::m_instance;
                CTranslator::m_instance = 0;
            }
        }
    };
    static Garbo garbo;
public slots:
    void load(int index);
    void exitApp(int code);

public:
    static CTranslator *instance();
    bool createChooseUiFile();
    bool writeConfigFile(int index);
    char readConfigFile();

public:
    QString runPath;
    QString filename;

signals:
    void languageChanges();
};

#endif // CTRANSLATOR_H
