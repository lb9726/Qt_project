#ifndef MYSQL_H
#define MYSQL_H

#include "sqlbasic.h"

class mysql : public SQLBasic
{
    Q_OBJECT
public:
    explicit mysql(QObject *parent = 0);
    ~mysql();

};

#endif // MYSQL_H
