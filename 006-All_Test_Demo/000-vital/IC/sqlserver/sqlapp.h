#ifndef SQLAPP_H
#define SQLAPP_H

#include "sqlparser.h"

class SQLApp : public SQLBasic
{
    Q_OBJECT
public:
    explicit SQLApp(QObject *parent = 0);
    ~SQLApp();

private:
public slots:   
};

#endif // SQLAPP_H
