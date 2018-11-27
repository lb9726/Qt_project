#ifndef COMMON_H
#define COMMON_H

#include <QString>

enum gameLevel
{
    LOW_LEVEL = 1,      //初级
    MIDDLE_LEVEL = 2,   //中级
    HIGH_LEVEL = 3      //高级
};

struct gameRecodeInfo
{
//    gameLevel m_gameLevel;
    int       m_gameTime;
    QString   m_playerName;
};

#endif // COMMON_H
