/*******************************************************************************
	Copyright (C), 2010-2011, BST Electric Co., Ltd.
	File name: BvtPtl.h
	Author: zhang zhiwei Version: V0 Date: 20. Feb. 2017
	Description: 
		贝思特多媒体显示器通讯协议层
	Others: 
	Function List: 

	History: 
	1.
		Date:
		Author:
		Modification:
	2. 
******************************************************************************/
#ifndef _BVTPTL_H_
#define _BVTPTL_H_

//* Public Macros/Configuration and Data type --------------------------------
//* @{公共宏/配置和数据定义区域
#pragma pack(push)	
#pragma pack(1)

//*ID = 0的帧内容*/
/* 数据2的数据结构 */
typedef struct _id0_dat2_{
	unsigned char Hund: 7;	/* 百位显示字符ASCII码表示 */
	unsigned char Reserved: 1;
}s_ID0DAT2_T;
/* 数据3的数据结构 */
typedef struct _id0_dat3_{
	unsigned char Tend: 7;	/* 十位显示字符ASCII码表示 */
	unsigned char Reserved: 1;
}s_ID0DAT3_T;
/* 数据4的数据结构 */
typedef struct _id0_dat4_{
	unsigned char Unit: 7;	/* 个位显示字符ASCII码表示 */
	unsigned char Reserved: 1;
}s_ID0DAT4_T;
/* 数据5的数据结构 */
typedef struct _id0_dat5_{
	unsigned char ArrowUp: 1;	/*上箭头*/
	unsigned char ArrowDn: 1;	/*下箭头*/
	unsigned char Runing: 1;	/*运行*/
	unsigned char Arrived: 1;	/*到站使能*/
	unsigned char VoidEn: 1;	/*语音报站使能*/
	unsigned char PowerDown: 1;	/*节能使能*/
	unsigned char Reserved: 1;	/*预留*/
	unsigned char Reserved1: 1;	/*预留*/
}s_ID0DAT5_T;

typedef struct _bvt_id0_{
	s_ID0DAT2_T Data2;	/* 数据2 */
	s_ID0DAT3_T Data3;	/* 数据3 */
	s_ID0DAT4_T Data4;	/* 数据4 */
	s_ID0DAT5_T Data5;	/* 数据5 */
	unsigned char Func[2];/*数据6/7/8*/
	unsigned char Language1;
	unsigned char Language2;
	unsigned char Year;
	unsigned char Month;
	unsigned char Day;
	unsigned char Week;
	unsigned char Hour;
	unsigned char Min;
	unsigned char Second;
}s_BVTID0_T;

/*心跳ID帧数据*/
typedef struct _bvt_id1_{
	unsigned char Data1;
	unsigned char Data2;
}s_BVTID1_T;


//ID类型枚举
typedef enum _id_type_{
	ID00_BASE = 0,	/* 基本信息ID 包含楼层，箭头、功能信息*/
	ID01_HEAR,		/* 心跳ID */
	
	ID_UNKNOW,		/* 未知ID信息 */
}e_IDTYPE_T;


#pragma pack(pop)
//**
// * @}	

//* Public function declaration -----------------------------------------------
//* @{公共函数声明区域(可供外部文件使用)

void BstBvtPtlInit(void);
void BstBvtSetFrameData(e_IDTYPE_T id,void *dat);
void BstBvtCopyFrameData(e_IDTYPE_T id,void *dat);
e_IDTYPE_T BstBvtPtlMonitor(void);
//**
// * @}	

#endif //_BVTPTL_H_ 
