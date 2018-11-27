#ifndef SQLITE_H
#define SQLITE_H

#include "sqlbasic.h"

class sqlite : public SQLBasic
{
    Q_OBJECT
public:
    explicit sqlite(QObject *parent = 0);
    ~sqlite();

    bool open(QString pPath, QString pName, OpenMode mode=ReadWrite);

    bool init();
};

#endif // SQLITE_H
