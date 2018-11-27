INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/..

QT += sql

SOURCES +=  $$PWD/sqlquery.cpp  \
            $$PWD/sqlbasic.cpp  \
            $$PWD/sqlparser.cpp

HEADERS +=  $$PWD/sqlquery.h \
            $$PWD/sqlbasic.h \
            $$PWD/sqlparser.h

contains(DEFINES, SQLITE) {
    SOURCES +=  $$PWD/sqlite.cpp
    HEADERS +=  $$PWD/sqlite.h
}

