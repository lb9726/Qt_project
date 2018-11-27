#ifndef ICDATA_H
#define ICDATA_H

#include "define.h"

typedef union
{
    struct _CardPasswd
    {
        unsigned char mPw1:4;
        unsigned char mPw2:4;
        unsigned char mPw3:4;
        unsigned char mPw4:4;
        unsigned char mPw5:4;
        unsigned char mPw6:4;
    } mPwList;
    unsigned char   mPwData[3];
}CardPasswd;

typedef union
{
    struct _CardByte
    {
        unsigned char mBit0:1;
        unsigned char mBit1:1;
        unsigned char mBit2:1;
        unsigned char mBit3:1;
        unsigned char mBit4:1;
        unsigned char mBit5:1;
        unsigned char mBit6:1;
        unsigned char mBit7:1;
    } mBits;
    unsigned char   mByte;
}CardByte;

typedef union
{
    unsigned short  mShortFee;
    unsigned char   mCharFee[2];
}CardFreeFee;

typedef struct
{
    unsigned char   mPasswd[3];
    unsigned char   mState;
    unsigned char   mType;
    unsigned char   mMoney[2];
    unsigned char   mPrice;
    unsigned char   mValidEles[8];

    unsigned char   mValidStartDate[3];
    unsigned char   mValidEndDate[3];
    unsigned char   mCallEleType;
    unsigned char   mResponseTime;
    unsigned char   mValidStartTime[3];
    unsigned char   mValidEndTime[3];
    unsigned char   mValidWeek;
    unsigned char   mAvailableTime; //可用时段使能
    unsigned char   mChargeType;
    unsigned char   mUnused[15];
} UsrCard;

typedef struct
{
    unsigned char   mPasswd[3];
    unsigned char   mState;
    unsigned char   mType;
    unsigned char   mUnused[11];

    unsigned char   mValidStartDate[3];
    unsigned char   mValidEndDate[3];
    unsigned char   mValidStartTime[3];
    unsigned char   mValidEndTime[3];
    unsigned char   mValidWeek;
    unsigned char   mValidDateEnable;//有效日期使能
    unsigned char   mAvailableTime;
    unsigned char   mUnused1[17];

} AdminCard;

typedef struct
{
    unsigned char   mPasswd[3];
    unsigned char   mState;
    unsigned char   mType;
    unsigned char   mUnused[3];
    unsigned char   mElevator;
    unsigned char   mUnused1[7];
    unsigned char   mUserId[4];
    unsigned char   mUnused2[28];
}LossCard;

typedef union
{
    struct _Date
    {
        unsigned char  mYear;
        unsigned char  mMonth;
        unsigned char  mDay;
    }mDateList;
    struct _Time
    {
        unsigned char  mHour;
        unsigned char  mMinute;
        unsigned char  mSecond;
    }mTimeList;
    unsigned char   mData[3];
}DateTime;

typedef struct
{
    unsigned char   mPasswd[3];
    unsigned char   mState;
    unsigned char   mType;
    unsigned char   mUnused[2];
    unsigned char   mUnit;
    unsigned char   mElevator[8];
}ToolCard_Header;

typedef struct
{
    ToolCard_Header mToolHeader;

    unsigned char   mDateTimeSyn[6];
}ToolCard_DateTimeSyn;

typedef struct
{
    ToolCard_Header mToolHeader;
}ToolCard_ElevatorNum;

typedef struct
{
    ToolCard_Header mToolHeader;

    unsigned char   mRoom[352][2];
}ToolCard_MaskRoom;


typedef struct
{
    ToolCard_Header mToolHeader;

    unsigned char   mDateTimeStart[5];
    unsigned char   mDateTimeEnd[5];
    unsigned char   mFloor;
    unsigned char   mUnused[5];
    unsigned char   mName[8];

}ToolCard_CardRecord;

typedef struct
{
    ToolCard_Header mToolHeader;

    unsigned char  mOpenFloor[12];

}ToolCard_Openfloor;

typedef struct
{
    ToolCard_Header mToolHeader;
    unsigned char  userID[4];

}ToolCard_LostCart;

typedef struct
{
    unsigned char usrOpenFloorOne[12];
    unsigned char usrOpenFloorTow[12];
    unsigned char usrOpenFloorThree[12];
    unsigned char usrOpenFloorFour[12];
    unsigned char usrOpenFloorFives[12];
    unsigned char usrOpenFloorSix[12];
    unsigned char usrOpenFloorSeven[12];
    unsigned char usrOpenFloorEight[12];
}UsrCard_OpenFloor;

//typedef struct
//{
//    unsigned char usrOpenFloorFour[12];
//    unsigned char mUnused1[4];
//    unsigned char usrOpenFloorFives[12];
//    unsigned char mUnused2[4];
//    unsigned char usrOpenFloorSix[12];
//    unsigned char mUnused3[4];
//}UsrCard_OpenFloor1;
//typedef struct
//{
//    unsigned char usrOpenFloorSeven[12];
//    unsigned char mUnused1[4];
//    unsigned char usrOpenFloorEight[12];
//    unsigned char mUnused2[20];
//}UsrCard_OpenFloor2;


typedef struct
{
    ToolCard_Header mToolHeader;

    unsigned char  mTimeallowStart1[3];
    unsigned char  mTimeallowEnd1[3];
    unsigned char  mTimeallowStart2[3];
    unsigned char  mTimeallowEnd2[3];
    unsigned char  mTimeallowStart3[3];
    unsigned char  mUnused1;
    unsigned char  mTimeallowEnd3[3];
    unsigned char  mWeek;
    unsigned char  mTimeEnable1;
    unsigned char  mTimeEnable2;
    unsigned char  mTimeEnable3;

}ToolCard_Elevtimeallow;



#endif // ICDATA_H
