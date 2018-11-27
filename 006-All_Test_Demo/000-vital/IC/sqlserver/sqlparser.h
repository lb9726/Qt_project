#ifndef SQLPARSER_H
#define SQLPARSER_H

#include "define.h"

#ifdef SQLITE
    #include "sqlite.h"
    typedef sqlite SQL_PARSER;
#endif
#ifdef MSSQL
    #include "mssql.h"
    typedef mssql SQL_PARSER;
#endif
#ifdef MYSQL
    #include "mysql.h"
    typedef mysql SQL_PARSER;
#endif
#ifdef SQLACCESS
    #include "sqlaccess.h"
    typedef sqlaccess SQL_PARSER;
#endif

#endif // SQLPARSER_H
