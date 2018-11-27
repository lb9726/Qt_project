#ifndef SQLACCESS_H
#define SQLACCESS_H

#include "sqlbasic.h"

class sqlaccess : public SQLBasic
{
    Q_OBJECT
public:
    explicit sqlaccess(QObject *parent = 0);
    ~sqlaccess();

};

#endif // SQLACCESS_H
