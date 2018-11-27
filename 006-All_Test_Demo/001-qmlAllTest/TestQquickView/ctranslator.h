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
    ~CTranslator();
    static CTranslator *m_instance;
public slots:
    void load(int index);
    void exitApp(int code);
//signals:
//    void languageChanged();
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
