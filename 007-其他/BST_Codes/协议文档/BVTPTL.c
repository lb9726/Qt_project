/*****************************************************************************
	Copyright (C), 2010-2011, BST Electric Co., Ltd.
	File name: BstMdc.c
		Author: zhang zhiwei Version: V0 Date: 20. Feb. 2017
	Description: 
		贝思特多媒体显示器通讯协议层
	Version: V0
	Function List: 

	History: 
	<author>  <time>  <version >  <desc>
******************************************************************************/
//* Private include files -----------------------------------------------------
//* @{引用头文件声明区域(仅限本文件内使用)
#include "BvtPtl.h"
#include "SendFifo.h"
#include "RecvFifo.h"

#define BVT_ESC 0x8A	/* 转换字符 */
#define BVT_STX 0x80	/* 帧起始字符 */
#define BVT_ETX 0x81	/* 帧结束字符 */
#define BVT_STX_AF 0xE7	/* 帧起始转换后增加字符 */
#define BVT_ETX_AF 0xE8	/* 帧结束转换后增加字符 */
#define BVT_ESC_AF 0x00	/* 转换字符转换后增加字符 */

#define BVT_MAX_FRAME_LENTH 44	/* 最大帧字节长度 */
#define BVT_SEND_INTERVAL 50	/* 发送数据间隔,单位ms */
#define BVT_RECV_CHANNEL_SIZE 1	/* 接收通道数据类型大小 */
#define BVT_PTL_FRAMEID_POS 1	/* 协议帧ID地址 */
#define BVT_PTL_DATASTART_POS 2	/* 协议帧数据地址 */

//**
// * @}

//* Private Macros and Data type -----------------------------------------------
//* @{私有宏和数据定义区域(仅限本文件内使用的全局变量和宏)
#pragma pack(push)	
#pragma pack(1)

static unsigned char gBvtRecStatus = 0;
static unsigned char gBvtRecCnt = 0;
static unsigned char gBvtFrameBuf[BVT_MAX_FRAME_LENTH];

#pragma pack(pop)
//**
// * @}

//* Public function declaration ------------------------------------------------
//* @{私有函数声明区域(仅限本文件内使用)

unsigned char BstBvtGetFrameDatLen(e_IDTYPE_T id);
unsigned char BstBvtRecvMonitor(void);
void BstBvtSendMonitor(void);

unsigned long BstBvtRecoverFrame(void *src,unsigned long srclen);
unsigned long BstBvtTransformFrame(void *src,unsigned long srclen,void *dst);
unsigned char BstBvtVerify(unsigned char *data, unsigned long length);


/*!
 * BstBvtPtlInit
 * 贝思特多媒体显示器通讯协议层初始化
 * @param[in]   None
 * @param[out]  None
 * @retval  	None
 */
void BstBvtPtlInit(void)
{
	BstFifoMemSet(gBvtFrameBuf,0,BVT_MAX_FRAME_LENTH);
	SendFifoChannelInit(BST_BVTSEND_USE,BVT_SEND_INTERVAL,BstBvtHwSendData);//发送通道初始化
	RecvFifoChannelInit(BST_BVTRECV_USE,BVT_RECV_CHANNEL_SIZE);//接收通道初始化
}
/*!
 * BstBvtCopyFrameData
 * 拷贝帧数据
 * @param[in]   id：帧ID	dat：帧数据地址
 * @param[out]  None
 * @retval  	None
 */
void BstBvtCopyFrameData(e_IDTYPE_T id,void *dat)
{
	unsigned long FrameLen = 0;
	
	if(id >= ID_UNKNOW)/*未知ID*/
		return;
	FrameLen = BstBvtGetFrameDatLen(id);
	BstFifoMemCpy(dat,&gBvtFrameBuf[BVT_PTL_DATASTART_POS],FrameLen);
}
/*!
 * BstBvtSetFrameData
 * 设置帧数据
 * @param[in]   id：帧ID	dat：帧数据地址
 * @param[out]  None
 * @retval  	None
 */
void BstBvtSetFrameData(e_IDTYPE_T id,void *dat)
{
	unsigned char TranBuf[BVT_MAX_FRAME_LENTH] = {0};
	unsigned char FrameBuf[BVT_MAX_FRAME_LENTH] = {0};
	unsigned char *pFrameBuf = FrameBuf;
	unsigned char *pStarVer;
	unsigned char DatLen = 0;
	unsigned long TranLen = 0;/*转换后帧长度*/
	
	if(id >= ID_UNKNOW)/*未知ID*/
		return;
	DatLen = BstBvtGetFrameDatLen(id);
	*pFrameBuf++ = BVT_STX;	/*加入帧头*/
	pStarVer = pFrameBuf;
	*pFrameBuf++ = id;	/*加入ID*/
	BstFifoMemCpy(pFrameBuf,dat,DatLen);/*加入数据*/
	pFrameBuf += DatLen;
	*pFrameBuf++ = BstBvtVerify(pStarVer,DatLen+1);/*加入校验值*/
	*pFrameBuf++ = BVT_ETX;/*加入帧尾*/
	TranLen = BstBvtTransformFrame(FrameBuf,pFrameBuf-FrameBuf,TranBuf);/*帧转换*/
	SendFifoChannelPush(BST_BVTSEND_USE,TranBuf,TranLen);/*压入发送FIFO*/
}
/*!
 * BstBvtPtlMonitor
 * 贝思特多媒体通讯协议监控
 * @param[in]   None
 * @param[out]  None
 * @retval  	帧ID
 */
e_IDTYPE_T BstBvtPtlMonitor(void)
{		
	unsigned char RecSta;
	
	BstBvtSendMonitor();//发送检测
	RecSta = BstBvtRecvMonitor();//接收检测
	
	if(RecSta == 0)
		return ID_UNKNOW;
	if(gBvtFrameBuf[BVT_PTL_FRAMEID_POS] >= ID_UNKNOW)
		return ID_UNKNOW;
	return (e_IDTYPE_T)gBvtFrameBuf[BVT_PTL_FRAMEID_POS];
}
/*!
 * BstBvtRecvMonitor
 * 协议数据接收监控
 * @param[in]   None
 * @param[out]  None
 * @retval  	接收状态 1-有数据	0-无数据
 */
unsigned char BstBvtRecvMonitor(void)
{
	unsigned char lDat;
	unsigned long TranFrameLen = 0;	/*转换后长度*/
	
	while(RecvFifoChannelCheck(BST_BVTRECV_USE) != 0)//检测接收FIFO
	{
		BstFifoMemCpy(&lDat,RecvFifoChannelPop(BST_BVTRECV_USE),BVT_RECV_CHANNEL_SIZE);/* 取出数据 */
		if(gBvtRecStatus == 0)/* 未接收到帧头 */
		{
			if(lDat == BVT_STX)/* 收到帧头 */
			{
				gBvtFrameBuf[gBvtRecCnt++] = lDat;
				gBvtRecStatus = 1;
			}else{
				continue;
			}
		}else if(gBvtRecStatus == 1){/* 正在接收数据 */
			if(gBvtRecCnt < BVT_MAX_FRAME_LENTH)
			{
				if(lDat == BVT_ETX)/* 收到帧尾 */
				{
					gBvtRecStatus = 2;
				}
				gBvtFrameBuf[gBvtRecCnt++] = lDat;				
			}else{/* 无效帧 */
				gBvtRecStatus = 0;
				gBvtRecCnt = 0;
			}
		}
		if(gBvtRecStatus == 2){/* 接收完数据 */
			TranFrameLen = BstBvtRecoverFrame(gBvtFrameBuf,gBvtRecCnt);/* 恢复帧数据 */
			gBvtRecStatus = 0;
			gBvtRecCnt = 0;	
			if(gBvtFrameBuf[TranFrameLen-2] != 
				BstBvtVerify(&gBvtFrameBuf[1],TranFrameLen-3))/* 数据校验失败 */
			{
				return 0;
			}else{
				return 1;
			}
		}
	}
	return 0;
}
/*!
 * BstBvtSendMonitor
 * 协议数据发送监控
 * @param[in]   None
 * @param[out]  None
 * @retval  	None
 */
void BstBvtSendMonitor(void)
{
	if(SendFifoChannelCheck(BST_BVTSEND_USE) == 0)/* 通道无数据 */
		return;
	SendFifoChannelPop(BST_BVTSEND_USE);/* 弹出通道数据*/
}
/*!
 * BstBvtTransformFrame
 * 协议转换帧数据
 * @param[in]   src：原数据地址 dst：转换后数据地址 
				len：原数据长度
 * @param[out]  None
 * @retval  	返回转换后的长度
 */
unsigned long BstBvtTransformFrame(void *src,unsigned long srclen,void *dst)
{
	unsigned long Len = srclen-2;/*去掉帧头和帧尾*/
	unsigned long TranLen = 2;
	unsigned char Cnt = 0;
	unsigned char *lDst = (unsigned char*)dst;
	unsigned char *lSrc = (unsigned char*)src;
	
	*lDst++ = *lSrc++;/*取出帧头数据*/
	while(Cnt < Len)
	{
		if(*lSrc == BVT_STX)/*遇到帧头数据*/
		{
			*lDst++ = BVT_ESC;
			*lDst++ = BVT_STX_AF;
			TranLen += 2;
		}else if(*lSrc == BVT_ETX){/*遇到帧尾数据*/
			*lDst++ = BVT_ESC;
			*lDst++ = BVT_ETX_AF;
			TranLen += 2;
		}else if(*lSrc == BVT_ESC){/*遇到转换字符*/
			*lDst++ = BVT_ESC;
			*lDst++ = BVT_ESC_AF;
			TranLen += 2;
		}else{
			*lDst++ = *lSrc;
			TranLen++;
		}
		lSrc++;
		Cnt++;	
	}	
	*lDst = *lSrc;/*取出帧尾数据*/
	return TranLen;
}
/*!
 * BstBvtRecoverFrame
 * 协议去除字符转换，恢复数据
 * @param[in]   src：原数据地址 len：原数据长度
 * @param[out]  None
 * @retval  	恢复后的帧长度
 */
unsigned long BstBvtRecoverFrame(void *src,unsigned long srclen)
{
	unsigned long Len = srclen-2;/*去掉帧头和帧尾*/
	unsigned long TranLen = 2;/*恢复后长度,帧头和帧尾*/
	unsigned char Cnt = 0;
	unsigned char *lDst = (unsigned char*)src;
	unsigned char *lSrc = (unsigned char*)src;
	
	*lDst++ = *lSrc++;/*取出帧头数据*/
	while(Cnt < Len)
	{
		if(*lSrc == BVT_ESC)
		{
			if(*(lSrc+1) == BVT_STX_AF)
				*lDst++ = BVT_STX;
			else if(*(lSrc+1) == BVT_ETX_AF)
				*lDst++ = BVT_ETX;
			else if(*(lSrc+1) == BVT_ESC_AF)
				*lDst++ = BVT_ESC;
			lSrc += 2;
			Cnt += 2;
		}else{
			*lDst++ = *lSrc++;
			Cnt++;
		}
		TranLen++;
	}
	*lDst = *lSrc;/*取出帧尾数据*/
	return TranLen;
}
/*!
 * BstBvtVerify
 * 协议校验算法
 * @param[in]   data：待校验的数据指针	length：待校验的数据长度
 * @param[out]  None
 * @retval  	None
 */
unsigned char BstBvtVerify(unsigned char *data, unsigned long length)
{
	unsigned char result = 0;
	unsigned int i;
	
	for (i = 0; i < length; i++)
	{
		result = result ^ data[i];
	}	
	return result & 0x7f;
}
/*!
 * BstBvtGetFrameDatLen
 * 获取帧数据长度
 * @param[in]   id：帧ID	
 * @param[out]  None
 * @retval  	帧ID对应的数据长度
 */
unsigned char BstBvtGetFrameDatLen(e_IDTYPE_T id)
{
	unsigned char len = 0;
	switch(id)
	{
		case ID00_BASE: len = sizeof(s_BVTID0_T);break;
		case ID01_HEAR: len = sizeof(s_BVTID1_T);break;
		default:
			break;
	}
	return len;
}

