////////////////////////////////////////////////Copyright (c)////////////////////////////////////////////////
//                                      广州周立功单片机发展有限公司
//                                            研    究    所
//                                              金卡产品线 
//
//                                          http://www.zlgmcu.com
//
//--------------文件信息-------------------------------------------------------------------------------------
// 文 件 名:    MUR200Cmd.h
// 文件描述:    MUR-200命令处理
// 版    本:    V1.00 
// 创 建 人:    曾本森
// 创建日期:    2010.03.11
//===========================================================================================================
//-----------------修改记录----------------------------------------------------------------------------------
// 修改内容:
// 当前版本:    
// 修 改 人:
// 修改日期:
//-----------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __MUR200_H
#define __MUR200_H
//===========================================================================================================
#include "StatusCode.h"
#include "SWCode.h"
//===========================================================================================================
#ifdef __cplusplus
	extern "C" {
#endif
//============= 常量定义 ====================================================================================
// PCD配置模式
#define ISO14443_TYPEA				0
#define ISO14443_TYPEB				1
#define ISO18092_NFCIP				2			// RC523不支持

#define ISO14443_3A_REQALL          0x52		// 请求所有的卡
#define ISO14443_3A_REQIDL          0x26		// 请求空闲的卡
#define SELECT_LEVEL_1				0x93		// 一级防碰撞/选择
#define SELECT_LEVEL_2				0x95		// 二级防碰撞/选择
#define SELECT_LEVEL_3				0x97		// 三级防碰撞/选择

#define KEY_TYPE_A            		0x60		// 密钥A
#define KEY_TYPE_B            		0x61		// 密钥B

#define KEY_SOURCE_EXT				0x00		// 使用外部输入的密钥验证
#define KEY_SOURCE_E2				0x80		// 使用内部E2的密钥验证
		
#define MIFARE_INCREMENT            0xC1		// 值增模式
#define MIFARE_DECREMENT            0xC0		// 值减模式
#define MIFARE_RESTORE              0xC2		// 复制模式

#define SL3_INCREMENT				0xB7		// 值增模式
#define SL3_DECREMENT				0xB9		// 值减模式
#define SL3_RESTORE					0xC3		// 复制模式
#define SL3_TRANSFER				0xB5		// 传输模式

#define SL3_PLAIN_READ				0x33		// 明文读模式
#define SL3_CIPHER_READ				0x31		// 密文读模式
#define SL3_PLAIN_WRITE				0xA3		// 明文写模式
#define SL3_CIPHER_WRITE			0xA1		// 密文写模式

// TypeB卡
#define	ISO14443_3B_REQIDL			0x00		// 空闲的卡
#define	ISO14443_3B_REQALL			0x08		// 所有的卡
//============= APDU命令体类型定义 ==========================================================================
#define	APDU_STATE1 				1			// APDU命令情况1:	CLA	INS	P1	P2
#define	APDU_STATE2 				2			// APDU命令情况2:	CLA	INS	P1	P2			Le
#define	APDU_STATE3 				3			// APDU命令情况3:	CLA	INS	P1	P2	Lc Data
#define	APDU_STATE4 				4			// APDU命令情况4:	CLA	INS	P1	P2	Lc Data Le
//============= 全局变量/函数定义 ===========================================================================

typedef struct 
{
	unsigned char ATQ[2];						// ISO14443A卡请求回应
	unsigned char UIDLen;						// ISO14443A卡UID的字节数
	unsigned char UID[10];						// ISO14443A卡UID
	unsigned char SAK;							// ISO14443A卡选择应答
} ACTIVEPARAA;

typedef struct 
{
	unsigned char PUPI[4];						// 伪唯一PICC标识符
	unsigned char AppDat[4];					// 应用数据
	unsigned char ProtInf[3];					// 参数信息
} ACTIVEPARAB;	

typedef struct
{
	unsigned char Case;							// APDU命令体类型
	unsigned char CLA;
	unsigned char INS;
	unsigned char P1;
	unsigned char P2;
	unsigned char Lc;
	unsigned char *pSBuf;
	unsigned char Le;
}C_APDU;										// APDU 命令

typedef struct
{
	unsigned short SW;
	unsigned int RcvLen;						// 接收的字节数，不包括SW
	unsigned char *pRBuf;						// 回应的数据，不包括SW
}R_APDU;										// APDU 回应
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:	int __stdcall MUR200Init(void)
// 函数功能:	MUR200初始化
// 入口参数:	-					
// 出口参数:	-
// 返 回 值:	当前存在MUR-200的个数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int __stdcall MUR200Init(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:	unsigned char MUR200Open(unsigned char ucDvcIndex)
// 函数功能:	打开MUR-200
// 入口参数:	unsigned char ucDvcIndex		// 设备索引号					
// 出口参数:	-
// 返 回 值:	STATUS_COMM_OK -- 打开成功；	其他值 -- 失败
// 说    明:	若有多个MUR-200设备，则ucDvcIndex分别为0、1、2......
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MUR200Open(unsigned char ucDvcIndex);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:	unsigned char MUR200Reset(void)
// 函数功能:	MUR-200复位
// 入口参数:	-				
// 出口参数:	-
// 返 回 值:	STATUS_COMM_OK -- 打开成功；	其他值 -- 失败
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MUR200Reset(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:	int __stdcall MUR200Exit(void)
// 函数功能:	MUR-200关闭退出
// 入口参数:	-				
// 出口参数:	-
// 返 回 值:	STATUS_COMM_OK -- 打开成功；	其他值 -- 失败
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char  __stdcall MUR200Exit(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:	unsigned char GetDvcInfo(unsigned int *pInfoLen, unsigned char *pInfo)
// 函数功能:	获取设备信息
// 入口参数:	unsigned int nRBufSize			// 接收缓冲区的长度				
// 出口参数:	unsigned int *pInfoLen			// 设备信息长度
//				unsigned char *pInfo			// 设备信息
// 返 回 值:	STATUS_SUCCESS -- 操作成功，其他值 -- 操作失败	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall GetDvcInfo(unsigned int nRBufSize, unsigned int *pInfoLen, unsigned char *pInfo);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:	unsigned char PcdConfig(unsigned char ucType)
// 函数功能:	配置读写器的PCD
// 入口参数:	unsigned char ucType			// TYPEA -- ISO14443A,TYPEB -- ISO14443B
// 出口参数:	-
// 返 回 值:	STATUS_SUCCESS -- 操作成功，其他值 -- 操作失败	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PcdConfig(unsigned char ucType);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:	unsigned char PcdClose(void)
// 函数功能:	关闭读写器的PCD
// 入口参数:	--
// 出口参数:	-
// 返 回 值:	STATUS_SUCCESS -- 操作成功，其他值 -- 操作失败	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PcdClose(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char PcdLoadKey(unsigned char ucKeyType, unsigned char ucKeySector,
//										 unsigned char ucKeyLen, const unsigned char *pKey)
// 函数功能:    装载密钥
// 入口参数:    unsigned char ucKeyType			// 密钥类型 KEY_A -- 密钥A;KEY_B -- 密钥B;
//              unsigned char ucKeySector		// 密钥存放扇区
//				unsigned char ucKeyLen			// 密钥的字节数(6/16字节)
//				unsigned char *pKey				// 写入的密钥
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 操作成功，其他值 -- 操作失败
// 说    明:	该函数是对读写器操作，不是对卡片执行验证密钥操作，
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PcdLoadKey(	unsigned char ucKeyType, unsigned char ucKeySector,
									unsigned char ucKeyLen, const unsigned char *pKey);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char PcdWriteReg(unsigned char ucReg, unsigned char ucValue)
// 函数功能:    写寄存器
// 入口参数:    unsigned char Reg               // 寄存器地址
//              unsigned char Value             // 写入的数据
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 操作成功，其他值 -- 操作失败
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PcdWriteReg(unsigned char ucReg, unsigned char ucValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char PcdReadReg(unsigned char ucReg, unsigned char *pValue)
// 函数功能:    写寄存器
// 入口参数:    unsigned char ucReg				// 寄存器地址
// 出口参数:    unsigned char *pValue			// 读出的数据
// 返 回 值:    STATUS_SUCCESS -- 操作成功，其他值 -- 操作失败
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PcdReadReg(unsigned char ucReg, unsigned char *pValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char PcdISOType(unsigned char ucTXMode)
// 函数功能:    设置PCD的协议模式
// 入口参数:    unsigned char ucType			// TYPEA -- ISO14443A,TYPEB -- ISO14443B
// 出口参数:	-
// 返 回 值:    STATUS_SUCCESS -- 操作成功，其他值 -- 操作失败
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PcdISOType(unsigned char ucType);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SetIO(unsigned char ucContrl, unsigned char ucOpenTm,
//									unsigned char ucCloseTm, unsigned char ucRepCnt)
// 函数功能:    打开ucContrl中置1的位
// 入口参数:    unsigned char ucContrl			// Contrl^0 蜂鸣器,	Contrl^1 红灯,
//												// Contrl^2 绿灯,   Contrl^3 蓝灯,	
//				unsigned char ucOpenTm;			// 打开时间，取值0-255， 10ms 的分辨率(0--一直打开)
//				unsigned char ucCloseTm;		// 关闭时间，取值0-255， 10ms 的分辨率
//				unsigned char ucRepCnt;			// 重复次数
// 出口参数:	-
// 返 回 值:    STATUS_SUCCESS -- 操作成功，其他值 -- 操作失败
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SetIO(  unsigned char ucContrl, unsigned char ucOpenTm,
								unsigned char ucCloseTm, unsigned char ucRepCnt);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char ClearIO( unsigned char ucContrl)
// 函数功能:    关闭ucContrl中置1的位
// 入口参数:    unsigned char ucContrl			// Contrl^0 蜂鸣器,	Contrl^1 红灯,
//												// Contrl^2 绿灯,   Contrl^3 蓝灯,	
// 出口参数:	-
// 返 回 值:    STATUS_SUCCESS -- 操作成功，其他值 -- 操作失败
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//unsigned char __stdcall ClearIO(unsigned char ucContrl);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char ReadIO(unsigned char *pIoStatus, 
//									 unsigned char *pContrl, unsigned char *pOpenTm,
//									 unsigned char *pCloseTm, unsigned char *pRepCnt)
// 函数功能:    打开ucContrl中置1的位
// 入口参数:    -
// 出口参数:	unsigned char *pIoStatus		// IO口当前状态
//				unsigned char *pContrl			// IO上次设置的值
//				unsigned char *pOpenTm;			// IO上次设置的值
//				unsigned char *pCloseTm;		// IO上次设置的值
//				unsigned char *pRepCnt;			// IO上次设置的值
// 返 回 值:    STATUS_SUCCESS -- 操作成功，其他值 -- 操作失败
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ReadIO( unsigned char *pIoStatus, 
							    unsigned char *pContrl, unsigned char *pOpenTm,
								unsigned char *pCloseTm, unsigned char *pRepCnt);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:	void PiccReset(INT8U ucPause_1ms,INT8U ucWait_1ms)
// 函数功能:	使RF场产生1个暂停，让卡复位
// 入口参数:	INT8U ucPause_1ms				// 暂停时间，关闭RF场该时间后重新打开，
//												// 若为0则不重新打开
//				INT8U ucWait_1ms				// RF场重新打开后等待时间，若为0则不等待
// 出口参数:	-
// 返 回 值:	-
// 说    明:	等待时间可根据卡的功耗而定，如Mifare1卡的功耗较小，等待数毫秒即可，
//				而CPU卡功耗较大，需要等待80毫秒左右。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall  PiccReset(unsigned char ucPause_1ms, unsigned char ucWait_1ms);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char DvcWriteUserKey(const unsigned char *pKey)
// 函数功能:    写入用户认证读卡器的密钥
// 入口参数:    const unsigned char *pKey		// 用户密钥
// 出口参数:	-
// 返 回 值:    STATUS_SUCCESS -- 操作成功，其他值 -- 操作失败
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall DvcWriteUserKey(const unsigned char *pKey);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char DvcAuthUserKey(unsigned char ucMode, const unsigned char *pKey)
// 函数功能:    用户认证读卡器
// 入口参数:    unsigned char ucMode			// 认证模式	0x01--明文模式；	0x02--密文模式
//				const unsigned char *pKey		// 用户密钥
// 出口参数:	-
// 返 回 值:    STATUS_SUCCESS -- 操作成功，其他值 -- 操作失败
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall DvcAuthUserKey(unsigned char ucMode, const unsigned char *pKey);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char DvcWriteUserEEPROM(unsigned int uiOffset, unsigned int nBytes, 
//												 const unsigned char *pBuf)
// 函数功能:    写用户E2PROM
// 入口参数:    unsigned int uiOffset			// 偏移量
//				unsigned int nBytes				// 写入的字节数
//				unsigned char *pBuf				// 写入的数据
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	若写入的数据溢出，会自动翻转
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall DvcWriteUserE2PROM(unsigned int uiOffset, unsigned int nBytes, 
										   const unsigned char *pBuf);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char DvcReadUserE2PROM(unsigned int uiOffset, unsigned int nBytes, 
//												unsigned char *pBuf)
// 函数功能:    读用户E2PROM
// 入口参数:    unsigned int uiOffset			// 偏移量
//				unsigned int nBytes				// 读出的字节数
//				unsigned char *pBuf				// 读出的数据
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	若写入的数据溢出，会自动翻转
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall DvcReadUserE2PROM(unsigned int uiOffset, unsigned int nBytes, unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char ProdRecord(unsigned int uiSLen, const unsigned char *pSBuf,   
//										 unsigned int nRBufSize, unsigned short *pRLen, unsigned char *pRBuf) 
// 函数功能:    生产/测试记录
// 入口参数:    unsigned int uiSLen				// 发送信息的长度
//				unsigned char *pSBuf			// 信息
//				unsigned int nRBufSize			// 接收缓冲区的长度
// 出口参数:	unsigned int *pRLen				// 接收信息的字节
//				unsigned char *pRBuf			// 接收的信息
// 返 回 值:    STATUS_SUCCESS -- 操作成功，其他值 -- 操作失败
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ProdRecord(unsigned int ucSLen, const unsigned char *pSBuf, 
								   unsigned int nRBufSize, unsigned int *pRLen, unsigned char *pRBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char ProdRecord(unsigned int uiSLen, const unsigned char *pSBuf,   
//										 unsigned int nRBufSize, unsigned short *pRLen, unsigned char *pRBuf) 
// 函数功能:    生产/测试记录
// 入口参数:    unsigned int uiSLen				// 发送信息的长度
//				unsigned char *pSBuf			// 信息
//				unsigned int nRBufSize			// 接收缓冲区的长度
// 出口参数:	unsigned int *pRLen				// 接收信息的字节
//				unsigned char *pRBuf			// 接收的信息
// 返 回 值:    STATUS_SUCCESS -- 操作成功，其他值 -- 操作失败
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ProdRecord(unsigned int ucSLen, const unsigned char *pSBuf, 
								   unsigned int nRBufSize, unsigned int *pRLen, unsigned char *pRBuf);


//===========================================================================================================
//CMD_TYPE_MIFARE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char MFActivate( unsigned char ucMode, unsigned char ucReqCode,  
//											ACTIVEPARAA *pAInfo)
// 函数功能:   	A型卡激活
// 入口参数:    unsigned char ucMode			// 防碰撞模式
//				unsigned char ucReqCode			// 请求代码
// 出口参数:    ACTIVEPARAA *pAInfo				// ISO14443 A卡激活信息
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。  
// 说    明:	该函数自动识别M1 和PLUS CPU卡的SL1、SL2和SL3
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MFActivate( unsigned char ucMode, unsigned char ucReqCode, ACTIVEPARAA *pAInfo);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char MFDeSelect(void)
// 函数功能:    将激活的卡挂起
// 输入参数:    -		
// 输出参数:    -
// 返 回 值:    命令执行结果
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MFDeSelect(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    void ValueToData(unsigned char *pBuf, unsigned char usBNr, long lValue)
// 功能描述:    将值转化为数据块模式
// 输入参数:    unsigned char usBNr				// 值存放地址
//				long iValue						// 值
// 输出参数:    unsigned char *pBuf				// 16字节值块格式数据
// 返 回 值:    -
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __stdcall ValueToData(unsigned char *pBuf, unsigned char ucBNr, long lValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char DataToValue(unsigned char *pBuf, long *pValue)
// 功能描述:    将数据块转化为值
// 输入参数:    unsigned char *pBuf				// 16字节值块格式数据	
// 输出参数:    long *pValue					// 值()
// 返 回 值:    -
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall DataToValue(unsigned char *pBuf, long *pValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char MFHaltA(void)
// 函数功能:   	将卡置为HALT状态
// 入口参数:    -
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。   
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MFHalt(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char MFAuth(unsigned char ucAuthMode, unsigned char ucBlock, 
//									 const unsigned char *pKey)
// 函数功能:   	Mifare卡密钥验证
// 入口参数:    unsigned char ucAuthMode		// 验证模式 
//													KEY_TYPE_A | KEY_SOURCE_EXT	外部输入的密钥A验证
//													KEY_TYPE_A | KEY_SOURCE_E2	内部E2的密钥A验证
//													KEY_TYPE_B | KEY_SOURCE_EXT	外部输入的密钥B验证
//													KEY_TYPE_B | KEY_SOURCE_E2	内部E2的密钥B验证
//				unsigned char ucBlock			// 验证的块号
//				unsigned char *pKey				// 使用KEY_SOURCE_E2模式时，pKey[0]为密钥存放的扇区,其它无效
//												// 使用KEY_SOURCE_EXT模式时，pKey为16字节密钥
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	该函数自动识别M1 和PLUS CPU卡的SL1、SL2和SL3
//				对于只有6字节密钥的情况(如M1 S50/70卡)则截取输入的16字节密钥的前6字节作为密钥
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MFAuth(unsigned char ucAuthMode, unsigned char ucBlock, const unsigned char *pKey);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char MFRead(unsigned char ucBNr, unsigned char ucExt, unsigned char *pBuf)
// 函数功能:   	MF卡多块读
// 入口参数:    unsigned char ucBNr				// 读的起始块号
//				unsigned char ucExt				// 读的块数(取值范围1～3)
// 出口参数:    unsigned char *pBuf				// 读出的数据
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	该函数自动识别M1 和PLUS CPU卡的SL1、SL2和SL3
//				对于MF1和PLUS SL1的卡，所有被操作的块需要在同一扇区内，否则会出错
//				对于PLUS SL2/SL3的卡，建议所操作的块均在同一扇区内(因为会自动跳过配置块)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MFRead(unsigned char ucBNr, unsigned char ucExt, unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char MFWrite(unsigned char ucBNr, unsigned char ucExt, const unsigned char *pBuf)
// 函数功能:   	MF卡多块写
// 入口参数:    unsigned char ucBNr				// 写的起始块号
//				unsigned char ucExt				// 写的块数(取值范围1～3)
// 出口参数:    unsigned char *pBuf				// 写入的数据
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	该函数自动识别M1 和PLUS CPU卡的SL1、SL2和SL3
//				对于MF1和PLUS SL1的卡，所有被操作的块需要在同一扇区内，否则会出错
//				对于PLUS SL2/SL3的卡，建议所操作的块均在同一扇区内(因为会自动跳过配置块)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MFWrite(unsigned char ucBNr, unsigned char ucExt, const unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char MFReadValue(unsigned char ucBNr, long *pValue)
// 函数功能:   	MF卡读取值块
// 入口参数:    unsigned char ucBNr				// 读的块号
// 出口参数:    long *pValue					// 读出的值
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	该函数自动识别M1 和PLUS CPU卡的SL1、SL2和SL3
//				PLUS S/X SL2 和 PLUS S SL3不支持值块
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MFReadValue(unsigned char ucBNr, long *pValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char MFWriteValue(unsigned char ucBNr, long lValue)
// 函数功能:   	MF卡写入值
// 入口参数:    unsigned char ucBNr				// 写的块号
//				long lValue						// 写入的值
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	该函数自动识别M1 和PLUS CPU卡的SL1、SL2和SL3
//				PLUS S/X SL2 和 PLUS S SL3不支持值块
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MFWriteValue(unsigned char ucBNr, long lValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char MFValueOper(unsigned char ucOperMode, unsigned char ucSBlock, 
//										  unsigned char ucDBlock, long lValue)
// 函数功能:    Mifare值操作命令(PLUS CPU卡的SL2无值操作)
// 入口参数:    unsigned char ucOperMode		// 值操作模式
//													MIFARE_INCREMENT 将源块的值加上指定的值存放到目的块
//													MIFARE_DECREMENT 将源块的值减去指定的值存放到目的块
//													MIFARE_RESTORE   将源块的值复制到目的块
//				unsigned char ucSBlock			// 操作的源块地址。
//				unsigned char ucDBlock			// 值操作的目地块
//				long lValue						// 非负的有符号数。
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	该函数自动识别M1 和PLUS CPU卡的SL1、SL2和SL3
//				MIFARE_RESTORE 操作模式是将指定的值块复制到目的块，一般用于值块操作的备份处理(该模式下，
//				参数值无效，为任意值均可)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall  MFValueOper(unsigned char ucOperMode, unsigned char ucSBlock, 
									 unsigned char ucDBlock, long lValue);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char MF0ULWrite(unsigned char ucBlock, const unsigned char *pBuf)
// 函数功能:    MF0 UL写命令
// 入口参数:    unsigned char ucBlock			// 写入的块地址。
//				unsigned char *pBuf				// 4字节写数据缓冲区首址。
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MF0ULWrite(unsigned char ucBlock, const unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char MF1Auth(unsigned char ucAuthMode, unsigned char ucBlock, unsigned char *pKey)
// 函数功能:    MF1 S50/70卡验证密钥
// 入口参数:    unsigned char ucAuthMode		// 验证模式 
//													KEY_TYPE_A | KEY_SOURCE_EXT	外部输入的密钥A验证
//													KEY_TYPE_A | KEY_SOURCE_E2	内部E2的密钥A验证
//													KEY_TYPE_B | KEY_SOURCE_EXT	外部输入的密钥B验证
//													KEY_TYPE_B | KEY_SOURCE_E2	内部E2的密钥B验证
//				unsigned char ucBlock			// 验证的块号	MF1 S50——0～63
//												//				MF1 S70——0～255
//				unsigned char *pKey				// 使用KEY_SOURCE_E2模式时，pKey[0]为密钥存放的扇区,其它无效
//												// 使用KEY_SOURCE_EXT模式时，pKey为6字节密钥
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MF1Auth(unsigned char ucAuthMode, unsigned char ucBlock, const unsigned char *pKey);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char MF1Read(unsigned char ucBNr, unsigned char *pBuf)
// 函数功能:   	MF1 S50/70读数据块
// 入口参数:    unsigned char ucBNr				// 读的起始块号
// 出口参数:    unsigned char *pBuf				// 读出的数据
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MF1Read(unsigned char ucBNr, unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char MF1Write(unsigned char ucBNr, const unsigned char *pBuf)
// 函数功能:   	MF1 S50/S70写数据块
// 入口参数:    unsigned char ucBNr				// 写的起始块号
//				unsigned char *pBuf				// 写入的数据
// 出口参数:	-
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MF1Write(unsigned char ucBNr, const unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char MF1ReadValue(unsigned char ucBNr, long *pValue)
// 函数功能:   	MF1 S50/S70卡读取值块
// 入口参数:    unsigned char ucBNr				// 读的块号
// 出口参数:    long *pValue					// 读出的值
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MF1ReadValue(unsigned char ucBNr, long *pValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char MF1WriteValue(unsigned char ucBNr, long lValue)
// 函数功能:   	MF1 S50/S70卡写入值
// 入口参数:    unsigned char ucBNr				// 写的块号
//				long lValue					// 写出的值
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall MF1WriteValue(unsigned char ucBNr, long lValue);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char MF1ValueOper(unsigned char ucOperMode, unsigned char ucSBlock, 
//										   unsigned char ucDBlock, long lValue)
// 函数功能:    MF1 S50/S70值操作
// 入口参数:    unsigned char ucOperMode		// 值操作模式
//													MIFARE_INCREMENT 将源块的值加上指定的值存放到目的块
//													MIFARE_DECREMENT 将源块的值减去指定的值存放到目的块
//													MIFARE_RESTORE   将源块的值复制到目的块
//				unsigned char ucSBlock			// 操作的源块地址。
//				unsigned char ucDBlock			// 值操作的目地块
//				long lValue						// 非负的有符号数。
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	模式为MIFARE_RESTORE时，参数‘lValue’无效；
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall  MF1ValueOper(unsigned char ucOperMode, unsigned char ucSBlock, 
									  unsigned char ucDBlock, long lValue);

//===========================================================================================================
//CMD_TYPE_ISO7816
//===========================================================================================================
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SelCurCard(unsigned char ucCardNo)
// 函数功能:    选择当前操作的SAM卡座
// 入口参数:    unsigned char ucCardNo			// SAM卡座号(1～8)，若不在该范围内则保持原样
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 操作成功，其他值 -- 操作失败
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SelCurCard(unsigned char ucCardNo);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char GetCurCard(unsigned char *pCardNo)
// 函数功能:    获取当前操作的SAM卡卡座号
// 入口参数:    -				
// 出口参数:    unsigned char *pCardNo			// SAM卡座号(1～8)
// 返 回 值:    STATUS_SUCCESS -- 操作成功，其他值 -- 操作失败
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall GetCurCard(unsigned char *pCardNo);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char __stdcall SAMClose(unsigned char ucCardNo)
// 函数功能:    关闭SAM卡
// 入口参数:    unsigned char ucCardNo			// SAM卡座号(1～8)，若不在该范围内，则为关闭所有的SAM
// 出口参数:	
// 返 回 值:    STATUS_SUCCESS -- 操作成功，其他值 -- 操作失败
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SAMClose(unsigned char ucCardNo);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char CiccReset(unsigned char ucCardNo, unsigned char ucRstFD, unsigned int nRBufSize,
//										unsigned int *pATRLen, unsigned char *pATRBuf)
// 函数功能:    卡片复位(若是第一次执行是冷复位，之后执行则是热复位)
// 入口参数:    unsigned char ucCardNo			// 卡座号，1～8
//				unsigned char ucRstFD			// 复位时的波特率	0x11--9600bps
//																	0x13--38400bps
//				unsigned int nRBufSize			// 接收缓冲区的长度
// 出口参数:    unsigned int *pATRLen			// 复位信息的字节数
//				unsigned char *pATRBuf			// 复位信息
// 返 回 值:    -
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall CiccReset(unsigned char ucCardNo, unsigned char ucRstFD, unsigned int nRBufSize,
								  unsigned int *pATRLen, unsigned char *pATRBuf);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char PPS(unsigned char *pPPS)
// 函数功能:    协议和参数选择
// 入口参数:    unsigned char *pPPS				// 协议参数
//				PPS[0]							// 指定是否存在PPS1、PPS2、PPS3
//				PPS[1]							// F/D
//				PPS[2]							// N
//				PPS[3]							// 待定
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功;	其它 -- 失败。
// 注    意:	PPS[0].4	0--PPS1不存在；	1--PPS1存在
// 				PPS[0].5	0--PPS2不存在；	1--PPS2存在
// 				PPS[0].6	0--PPS3不存在；	1--PPS3存在
//				若PPS[0]中指定不存在的参数，该参数仍需要输入(任意值均可)，
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall CiccPPS(unsigned char *pPPS);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char CiccTP0(unsigned int nSLen, const unsigned char *pSBuf, unsigned int nRBufSize
//									  unsigned int *pRLen, unsigned char *pRBuf)
// 函数功能:    接触式IC卡的传输协议T = 0
// 入口参数:    unsigned int nSLen				// 发送的字节数	
//				unsigned char *pSBuf			// 发送数据缓冲区
//				unsigned int nRBufSize			// 用户接收缓冲区长度		
// 出口参数:	unsigned int *pRLen				// 接收数据的长度
//				unsigned char *pRBuf			// 接收数据缓冲区
// 返 回 值:    STATUS_SUCCESS -- 成功;	其它 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall CiccTP0(unsigned int nSLen, const unsigned char *pSBuf, unsigned int nRBufSize, 
								unsigned int *pRLen, unsigned char *pRBuf);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char CiccTP1(unsigned int nSLen, const unsigned char *pSBuf, unsigned int nRBufSize, 
//									  unsigned int *pRLen, unsigned char *pRBuf)
// 函数功能:    接触式IC卡的传输协议T = 1(不主动发送S块)
// 入口参数:    unsigned int nSLen				// 发送的字节数	
//				unsigned char *pSBuf			// 发送数据缓冲区
//				unsigned int nRBufSize			// 用户接收缓冲区长度		
// 出口参数:	unsigned int *pRLen				// 接收数据的长度
//				unsigned char *pRBuf			// 接收数据缓冲区
// 返 回 值:    STATUS_SUCCESS -- 成功;	其它 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall CiccTP1(unsigned int nSLen, const unsigned char *pSBuf, unsigned int nRBufSize, 
								unsigned int *pRLen, unsigned char *pRBuf);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char CiccSearch(unsigned int nRBufSize, unsigned int *pRLen, unsigned char *pRBuf)
// 函数功能:    搜索卡座中的卡
// 入口参数:    unsigned int nRBufSize			// 接收缓冲区大小		
// 出口参数:	unsigned int *pRLen				// 接收数据的长度
//				unsigned char *pRBuf			// 接收数据缓冲区
// 返 回 值:    STATUS_SUCCESS -- 成功;	其它 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall CiccSearch(unsigned int nRBufSize, unsigned int *pRLen, unsigned char *pRBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned short __stdcall CiccTPDU(const C_APDU *pAPDUCmd, R_APDU *pAPDURes)
// 函数功能:    T = 0 或T = 1协议的APDU命令
// 入口参数:    C_APDU *pAPDUCmd				// 命令的APDU结构体
//				unsigned int nRBufSize			// 接收缓冲区的大小
// 出口参数:    R_APDU *pAPDURes				// 回应的APDU结构体
// 返 回 值:    命令执行状态或SW(命令执行成功返回SW, 否则返回命令执行其他的结果)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned short __stdcall CiccTPDU(const C_APDU *pAPDUCmd, unsigned int nRBufSize, R_APDU *pAPDURes);

//===========================================================================================================
// CMD_TYPE_COMM_PROT	
//===========================================================================================================
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char PiccRequest(unsigned char ucReqCode,unsigned char *pATQ)
// 函数功能:    A型卡激活命令
// 入口参数:    unsigned char ucReqCode			// 请求代码	ISO14443_3A_REQIDL	0x26	IDLE
//												// 			ISO14443_3A_REQALL	0x52	ALL
// 出口参数:    unsigned char *pATQ				// 请求回应码,2字节
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccRequest(unsigned char ucReqCode, unsigned char *pATQ);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char PiccAnticoll( unsigned char unMode, unsigned char ucSelCode,  
//											unsigned char ucBitCnt, unsigned char *pUID)
// 函数功能:    位方式防碰撞
// 入口参数:    unsigned char unMode			// 1 -- 执行防碰撞循环，0 -- 只执行一次防碰撞
//				unsigned char ucSelCode			// 防碰撞代码	SELECT_CASCADE_LEVEL_1：第1级
//												// 				SELECT_CASCADE_LEVEL_2：第2级
//												// 				SELECT_CASCADE_LEVEL_3：第3级
//				unsigned char ucBitCnt			// 已知卡号的位数
// 出口参数:    unsigned char *pUID				// 返回序列号缓冲区首址，4字节。
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	unMode = 0 时，只执行一次防碰撞，该模式用于同一时刻，只允许一张卡在天线感应区内    
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccAnticoll(unsigned char unMode, unsigned char ucSelCode, 
							   		 unsigned char ucBitCnt, unsigned char *pUID);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char PiccSelect(unsigned char ucSelCode, unsigned char *pUID, unsigned char *pSAK)
// 函数功能:    选择卡
// 入口参数:    unsigned char ucSelCode			// 防碰撞代码	SELECT_LEVEL_1：第1级
//												// 				SELECT_LEVEL_2：第2级
//												// 				SELECT_LEVEL_3：第3级
//				unsigned char *pUID				// 4字节序列号。
// 出口参数:    unsigned char *pSAK				// 返回序列号缓冲区首址，4字节。
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。  
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccSelect(unsigned char ucSelCode, unsigned char *pUID, unsigned char *pSAK);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char PiccHaltA(void)
// 函数功能:   	将卡置为HALT状态
// 入口参数:    -
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccHaltA(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char PiccRequestATS(unsigned char ucCID,unsigned char *pATS)
// 函数功能:    请求选择以应答
// 输入参数:    unsigned char ucCID				// 被寻址的PICC逻辑编号 0x00~0x0E		
// 输出参数:    unsigned char *pATS				// 应答以选择（ATS）
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccRequestATS(unsigned char ucCID,unsigned char *pATS);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char PiccPPSRequest(unsigned char DSI_DRI)
// 函数功能:    协议和参数选择请求
// 输入参数:    unsigned char DSI_DRI			// b3:2 -- PCD  -> PICC的位率编码
//												// b1:0 -- PICC -> PCD 的位率编码
// 输出参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	CID不用输入，程序自动从'请求选择以应答'获取
//				DSI_DRI:	(00)b -> 106Kb	(01)b -> 212Kb	(10)b -> 424Kb	(11)b -> 847Kb
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccPPSRequest(unsigned char DSI_DRI);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char PiccDeSelect(void)
// 函数功能:    将激活的卡挂起
// 输入参数:    -		
// 输出参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccDeSelect(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char PiccRequestB( unsigned char ucReqCode, unsigned char ucAFI, 
//											unsigned char N, ACTIVEPARAB *pActiParaB)
// 函数功能:    B型卡请求
// 入口参数:    unsigned char ucReqCode			// 请求代码	ISO14443_3B_REQIDL 0x00 -- 空闲的卡
//												//			ISO14443_3B_REQALL 0x08 -- 所有的卡
//				unsigned char ucAFI				// 应用标识符，0x00：全选
//				unsigned char N					// 时隙总数,取值范围0--4。
// 出口参数:    ACTIVEPARAB *pActiParaB			// 请求应答，11字节
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccRequestB(unsigned char ucReqCode, unsigned char ucAFI, 
									 unsigned char N, ACTIVEPARAB *pActiParaB);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char PiccSlotMarker(unsigned char N, ACTIVEPARAB *pActiParaB)
// 函数功能:    时隙标记防碰撞
// 入口参数:    unsigned char N，				// 时隙标记，取值范围2-16，与PiccRequestB()函数有关
// 出口参数:    ACTIVEPARAB *pActiveParaB		// 请求应答
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccSlotMarker(unsigned char N, ACTIVEPARAB *pActiParaB);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char PiccAttrib(unsigned char *pPUPI, unsigned char ucCID, unsigned char ucProType, 
//										 unsigned char ucINFLen, const unsigned char *pHigherLayerINF, 
//										 unsigned char *pRLen, unsigned char *pAATTRIB)
// 函数功能:    选择PICC
// 入口参数:    unsigned char *pPUPI			// 4字节PICC标识符
//				unsigned char ucCID				// 0 - 14,若不支持CID，则设置为0000
//				unsigned char ucProType			// 支持的协议，由请求回应中的ProtocolType指定
//				unsigned char ucINFLen			// 高层命令字节数
//				unsigned char *pHigherLayerINF	// 高层命令信息
//				unsigned int nRBufSize			// 接收缓冲区的长度	
// 出口参数:    unsigned char *pRLen			// ATTRIB命令回应的字节数
//				unsigned char *pAATTRIB			// ATTRIB命令回应
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccAttrib(unsigned char *pPUPI, unsigned char ucCID, unsigned char ucProType, 
								   unsigned char ucINFLen, const unsigned char *pHigherLayerINF, 
								   unsigned int nRBufSize, unsigned char *pRLen, unsigned char *pAATTRIB);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char PiccHaltB(unsigned char *pPUPI)
// 函数功能:    挂起卡
// 入口参数:    unsigned char *pPUPI					// 4字节PICC标识符
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccHaltB(unsigned char *pPUPI);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char PiccTPCL(unsigned int nSendLen, const unsigned char *pSendBuf, 
//									   unsigned int nRBufSize, unsigned char *pRcvBuf, unsigned int *pRcvLen)
// 函数功能:    实现非接触式IC卡传输协议(T = CL)，即ISO14443-4 
// 输入参数:    unsigned int  uiSendLen			// 发送的字节数	
//				unsigned char *pSendBuf			// 发送的数据
//				unsigned int nRBufSize			// 接收缓冲区大小	
// 输出参数:    unsigned int*pRcvLen			// 接收的字节数
//				unsigned char *pRcvBuf			// 接收的数据
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccTPCL(unsigned int nSendLen, const unsigned char *pSendBuf, unsigned int nRBufSize,
								 unsigned int *pRcvLen, unsigned char *pRcvBuf);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned short __stdcall PiccTPDU(const C_APDU *pAPDUCmd, R_APDU *pAPDURes)
// 函数功能:    T = CL协议的APDU命令
// 入口参数:    C_APDU *pAPDUCmd				// 命令的APDU结构体
//				unsigned int nRBufSize			// 接收缓冲区的大小
// 出口参数:    R_APDU *pAPDURes				// 回应的APDU结构体
// 返 回 值:    命令执行状态或SW(命令执行成功返回SW, 否则返回命令执行其他的结果)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned short __stdcall PiccTPDU(const C_APDU *pAPDUCmd, unsigned int nRBufSize, R_APDU *pAPDURes);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char PiccActivate( unsigned char ucMode, unsigned char ucReqCode,  
//											ACTIVEPARAA *pAInfo)
// 函数功能:   	A型卡激活
// 入口参数:    unsigned char ucMode			// 防碰撞模式
//				unsigned char ucReqCode			// 请求代码
// 出口参数:    ACTIVEPARAA *pAInfo				// ISO14443 A卡激活信息
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。   
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccActivate( unsigned char ucMode, unsigned char ucReqCode, ACTIVEPARAA *pAInfo);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char PiccActivateB(unsigned char ucReqCode, unsigned char ucAFI, 
//											ACTIVEPARAB *pActiParaB)
// 函数功能:    B型卡激活
// 入口参数:    unsigned char ucReqCode			// 请求代码	ISO14443_3B_REQIDL 0x00 -- 空闲的卡
//												//			ISO14443_3B_REQALL 0x08 -- 所有的卡
//				unsigned char ucAFI				// 应用标识符，0x00：全选
// 出口参数:    ACTIVEPARAB *pActiParaB			// 请求应答，11字节
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	-   
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall PiccActivateB(unsigned char ucReqCode, unsigned char ucAFI, ACTIVEPARAB *pActiParaB);
		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char ExChangeBlock3(unsigned char CRC_EN, unsigned int uiTpr, unsigned int uiTre,
//											 unsigned int nSendBits, const unsigned char *pSBuf, 
//											 unsigned int nRBufSize, unsigned int *pTime,
//										     unsigned int *pRcvBits, unsigned char *pRBuf)
// 函数功能:    PCD与PICC交换数据块
// 入口参数:    unsigned char CRC_EN,			// CRC_EN.7 = 1 -- 发送使能CRC;	0 -- 发送禁止CRC; 
//												// CRC_EN.6 = 1 -- 接收使能CRC; 0 -- 接收禁止CRC; 
//				unsigned int uiTpr				// 定时器分频值
//				unsigned int uiTre				// 超时时间 = (uiTre + 1) * ((uiTpr + 1) / 6.78)us
//				unsigned int nSendBits			// 发送的位数，若发送的位数不是8的整数倍，则发送CRC使能无效	
//				unsigned char *pSBuf			// 发送的数据	
//				unsigned int nRBufSize			// 接收缓冲区的大小
// 出口参数:    unsigned int *pTime				// 执行的时间 = pTime * ((uiTpr + 1) / 6.78)us
//				unsigned int *pRcvBits			// 接收的位数
//				unsigned char *pRBuf			// 接收的数据
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall ExChangeBlock3( unsigned char CRC_EN, unsigned int uiTpr, unsigned int uiTre,
										unsigned int nSendBits, const unsigned char *pSBuf, 
										unsigned int nRBufSize, unsigned int *pTime,
										unsigned int *pRcvBits, unsigned char *pRBuf);
//===========================================================================================================
//CMD_TYPE_PLUS_CPU
//===========================================================================================================
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SL0WritePerso(unsigned int uiBNr, const unsigned char *pBuf)
// 函数功能:    SL0 个人化更新数据
// 入口参数:    unsigned int uiBNr				// 块地址
//				unsigned char *pBuf				// 写入的数据
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL0WritePerso(unsigned int uiBNr, const unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SL0CommitPerso(void)
// 函数功能:    SL0 提交个人化
// 入口参数:    -
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
//				在SL0的PLUS CPU卡，只有修改了以下地址才能执行"SL0提交个人化"命令：
//				0x9000(主控密钥)
//				0x9001(配置块密钥)
//				0x9002(SL2提升密钥，只有支持SL2的卡才有该密钥)
//				0x9003(SL3主控密钥，只有支持SL3的卡才有该密钥)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL0CommitPerso(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SL1Auth(unsigned char ucAuthMode, unsigned char ucBlock, unsigned char *pKey)
// 函数功能:    SL1验证密钥
// 入口参数:    unsigned char ucAuthMode		// 验证模式 
//													KEY_TYPE_A | KEY_SOURCE_EXT	外部输入的密钥A验证
//													KEY_TYPE_A | KEY_SOURCE_E2	内部E2的密钥A验证
//													KEY_TYPE_B | KEY_SOURCE_EXT	外部输入的密钥B验证
//													KEY_TYPE_B | KEY_SOURCE_E2	内部E2的密钥B验证
//				unsigned char ucBlock			// 验证的块号	MF1 S50——0～63
//												//				MF1 S70——0～255
//				unsigned char *pKey				// 使用KEY_SOURCE_E2模式时，pKey[0]为密钥存放的扇区,其它无效
//												// 使用KEY_SOURCE_EXT模式时，pKey为6字节密钥
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL1Auth(unsigned char ucAuthMode, unsigned char ucBlock, const unsigned char *pKey);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SL1Read(unsigned char ucBlock, unsigned char *pBuf)
// 函数功能:   	SL1读数据块
// 入口参数:    unsigned char ucBlock			// 读的起始块号
// 出口参数:    unsigned char *pBuf				// 读出的数据
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL1Read(unsigned char ucBlock, unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SL1Write(unsigned char ucBlock, const unsigned char *pBuf)
// 函数功能:   	SL1写数据块
// 入口参数:    unsigned char ucBNr				// 写的起始块号
//				unsigned char *pBuf				// 写入的数据
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL1Write(unsigned char ucBlock, const unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SL1ReadValue(unsigned char ucBNr, long *pValue)
// 函数功能:   	SL1卡读取值块
// 入口参数:    unsigned char ucBNr				// 读的块号
// 出口参数:    long *pValue					// 读出的值
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL1ReadValue(unsigned char ucBNr, long *pValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SL1WriteValue(unsigned char ucBNr, long lValue)
// 函数功能:   	SL1卡写入值
// 入口参数:    unsigned char ucBNr				// 写的块号
//				long *pValue					// 写出的值
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL1WriteValue(unsigned char ucBNr, long lValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SL1ValueOper(unsigned char ucOperMode, unsigned char ucSBlock, 
//										   unsigned char ucDBlock, long lValue)
// 函数功能:    SL1值操作
// 入口参数:    unsigned char ucOperMode		// 值操作模式
//													MIFARE_INCREMENT 将源块的值加上指定的值存放到目的块
//													MIFARE_DECREMENT 将源块的值减去指定的值存放到目的块
//													MIFARE_RESTORE   将源块的值复制到目的块
//				unsigned char ucSBlock			// 操作的源块地址。
//				unsigned char ucDBlock			// 值操作的目地块
//				long lValue						// 非负的有符号数。
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	模式为MIFARE_RESTORE时，参数‘lValue’无效；
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall  SL1ValueOper(unsigned char ucOperMode, unsigned char ucSBlock, 
									  unsigned char ucDBlock, long lValue);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SL2AESAuth(unsigned char ucAuthMode, unsigned int uiKNr, 
//										 const unsigned char *pAESKey)
// 函数功能:    SL2验证密钥
// 入口参数:    unsigned char ucAuthMode		// 验证模式 
//													KEY_SOURCE_EXT	外部输入的密钥验证
//													KEY_SOURCE_E2	内部E2的密钥验证
//				unsigned int uiKNr				// 2KByte的卡：取值范围0x4000～0x403F
//												// 4KByte的卡：取值范围0x4000～0x404F
//				unsigned char *pKey				// 使用KEY_SOURCE_E2模式时，pKey[0]为密钥存放的扇区,其它无效
//												// 使用KEY_SOURCE_EXT模式时，pKey为6字节密钥
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
//				uiKNr的取值不止0x4000～0x404F，详细内容请参阅PLUS CPU卡的资料
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL2AESAuth(unsigned char ucAuthMode, unsigned int uiKNr, 
								   const unsigned char *pAESKey);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SL2CryptoAuth(unsigned char ucAuthMode, unsigned int uiKNr, 
//											const unsigned char *pKey)
// 函数功能:    SL2验证密钥
// 入口参数:    unsigned char ucAuthMode		// 验证模式 
//													KEY_TYPE_A | KEY_SOURCE_EXT	外部输入的密钥A验证
//													KEY_TYPE_A | KEY_SOURCE_E2	内部E2的密钥A验证
//													KEY_TYPE_B | KEY_SOURCE_EXT	外部输入的密钥B验证
//													KEY_TYPE_B | KEY_SOURCE_E2	内部E2的密钥B验证
//				unsigned char ucBlock			// 2KByte的卡：取值范围0～127
//												// 4KByte的卡：取值范围0～255
//				unsigned char *pKey				// 使用KEY_SOURCE_E2模式时，pKey[0]为密钥存放的扇区,其它无效
//												// 使用KEY_SOURCE_EXT模式时，pKey为6字节密钥
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL2CryptoAuth(unsigned char ucAuthMode, unsigned char ucBlock, 
									  const unsigned char *pKey);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SL2MultiBlockRead(unsigned char ucBNr, unsigned char ucExt, unsigned char *pBuf)
// 函数功能:   	SL2多块读
// 入口参数:    unsigned char ucBNr				// 读的起始块号
//				unsigned char ucExt				// 读的块数(取值范围1～3)
// 出口参数:    unsigned char *pBuf				// 读出的数据
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL2MultiBlockRead(unsigned char ucBNr, unsigned char ucExt, unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SL2MultiBlockWrite(unsigned char ucBNr, unsigned char ucExt, 
//												 const unsigned char *pBuf)
// 函数功能:   	SL2 多块写
// 入口参数:    unsigned char ucBNr				// 写的起始块号
//				unsigned char ucExt				// 写的块数(取值范围1～3)
//				unsigned char *pBuf				// 写入的数据
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL2MultiBlockWrite(unsigned char ucBNr, unsigned char ucExt, 
										   const unsigned char *pBuf);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SL3FirstAuth( unsigned char ucAuthMode, unsigned int uiKNr, 
//											const unsigned char *pAESKey)
// 函数功能:    SL3 首次验证
// 入口参数:    unsigned char ucAuthMode		// 验证模式 
//													KEY_SOURCE_EXT	外部输入的密钥验证
//													KEY_SOURCE_E2	内部E2的密钥验证
//				unsigned int uiKNr				// 密钥地址
//				unsigned char *pKey				// 使用KEY_SOURCE_E2模式时，pKey[0]为密钥存放的扇区,其它无效
//												// 使用KEY_SOURCE_EXT模式时，pKey为6字节密钥
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
//				uiKNr的取值不止0x4000～0x404F，详细内容请参阅PLUS CPU卡的资料
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL3FirstAuth(unsigned char ucAuthMode, unsigned int uiKNr, 
									 const unsigned char *pAESKey);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SL3FollowingAuth( unsigned char ucAuthMode, unsigned int uiKNr, 
//												const unsigned char *pAESKey)
// 函数功能:    SL3 跟随验证
// 入口参数:    unsigned char ucAuthMode		// 验证模式 
//													KEY_SOURCE_EXT	外部输入的密钥验证
//													KEY_SOURCE_E2	内部E2的密钥验证
//				unsigned int uiKNr				// 密钥地址
//				unsigned char *pKey				// 使用KEY_SOURCE_E2模式时，pKey[0]为密钥存放的扇区,其它无效
//												// 使用KEY_SOURCE_EXT模式时，pKey为6字节密钥
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
//				数据块的扇区密钥地址为0x4000～0x404F偶数地址为密钥A,奇数地址为密钥B
//				uiKNr的取值不止0x4000～0x404F，详细内容请参阅PLUS CPU卡的资料
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL3FollowingAuth(unsigned char ucAuthMode, unsigned int uiKNr, 
										 const unsigned char *pAESKey);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SL3ResetAuth(void)
// 函数功能:    SL3 复位验证
// 入口参数:    -
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL3ResetAuth(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SL3Read(  unsigned char ucMode, unsigned char ucBNr, 
//										unsigned char ucExt, unsigned char *pBuf)
// 函数功能:   	SL3 读数据块
// 入口参数:    unsigned char ucMode			// 读的模式	0x31 -- 命令有MAC，数据密文，回应有MAC
//												//			0x33 -- 命令有MAC，数据明文，回应有MAC
//				unsigned int uiBNr				// 读的起始块号
//				unsigned char ucExt				// 读的块数
// 出口参数:    unsigned char *pBuf				// 读出的数据
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	模式中的密文/明文是指RF场中的数据传输是否加密，读写器与PC之间传输的数据始终是明文
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL3Read(unsigned char ucMode, unsigned int uiBNr, 
								unsigned char ucExt, unsigned char *pBuf);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SL3Write( unsigned char ucMode, unsigned char ucBNr, 
//										unsigned char ucExt, const unsigned char *pBuf)
// 函数功能:   	SL3 写
// 入口参数:    unsigned char ucMode			// 写的模式	0xA1 -- 命令有MAC，数据密文，回应有MAC；
//															0xA3 -- 命令有MAC，数据明文，回应有MAC；
//				unsigned int uiBNr				// 写的起始块号
//				unsigned char ucExt				// 写的块数(取值范围1～3)
//				unsigned char *pBuf				// 写入的数据
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	模式中的密文/明文是指RF场中的数据传输是否加密，读写器与PC之间传输的数据始终是明文
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL3Write(unsigned char ucMode, unsigned int uiBNr, 
								 unsigned char ucExt, const unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SL3ReadValue(unsigned char ucMode, unsigned int uiBNr, long *pValue)
// 函数功能:   	SL3卡读取值块
// 入口参数:    unsigned char ucMode			// 读的模式	0x31 -- 命令有MAC，数据密文，回应有MAC
//												//			0x33 -- 命令有MAC，数据明文，回应有MAC
//				unsigned int uiBNr				// 读的块号
// 出口参数:    long *pValue					// 读出的值
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL3ReadValue(unsigned char ucMode, unsigned int uiBNr, long *pValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SL3WriteValue(unsigned char ucMode, unsigned int uiBNr, long lValue)
// 函数功能:   	SL3卡写入值
// 入口参数:    unsigned char ucMode			// 写的模式	0xA1 -- 命令有MAC，数据密文，回应有MAC；
//															0xA3 -- 命令有MAC，数据明文，回应有MAC；
//				unsigned int uiBNr				// 写的块号
//				long *pValue					// 写出的值
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL3WriteValue(unsigned char ucMode, unsigned int uiBNr, long lValue);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SL3ValueOper( unsigned char ucMode, unsigned int uiSBNr, 
//											unsigned int uiDBNr, long lValue)
// 函数功能:    SL3值操作
// 入口参数:    unsigned char ucOperMode		// 值操作模式
//													SL3_INCREMENT	将源块的值加上指定的值存放到目的块
//													SL3_DECREMENT	将源块的值减去指定的值存放到目的块
//													SL3_RESTORE		将源块的值复制到目的块
//													SL3_TRANSFER	将缓冲区中的数据传输到指定的块	
//				unsigned int uiSBNr				// 操作的源块地址(只能是数据块)
//				unsigned int uiDBNr				// 值操作的目地块(只能是数据块)
//				long lValue						// 非负的有符号数。
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	模式为SL3_RESTORE时，参数‘lValue’无效；
//				模式为SL3_TRANSFER时，参数‘lValue’无效 和‘ucSBlock’无效；
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall  SL3ValueOper(unsigned char ucOperMode,  unsigned int uiSBNr, 
									  unsigned int uiDBNr, long lValue);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SL3ProximityCheck(unsigned char ucKeyMode, 
//												const unsigned char *pProxCheckKey, unsigned int *pTime)
// 功能描述:    中继攻击检查
// 输入参数:    unsigned char ucKeyMode			// 0 -- 使用PICC验证时的会话Key, 
//												// 1 -- 使用ProximityCheckKey
//				unsigned char *pProxCheckKey	// ProximityCheckKey(密钥地址:0xA001),仅当ucKeyMode=1时有效
// 输出参数:    unsigned int *pTime				// 函数执行时间(us)
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	PPS1固定为0x00，该程序不支持变波特率
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL3ProximityCheck(unsigned char ucKeyMode, const unsigned char *pProxCheckKey, 
										  unsigned int *pTime);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char unsigned char PLUSSL3VCSupport(const unsigned char *pIID)
// 功能描述:    选择应用(虚拟卡)
// 输入参数:    unsigned char *pIID						// 应用标识(Installation Identifier,0xB001)
// 输出参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	PLUS S不支持该命令
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL3VCSupport(const unsigned char *pIID);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SL3VCSupportLast( const unsigned char *pIID, const unsigned char *pENCKey, 
//												const unsigned char *pMACKey, unsigned char *pCardInfo)
// 功能描述:    选择应用(虚拟卡，返回实际UID等信息)
// 输入参数:    unsigned char *pIID						// 应用标识(Installation Identifier,0xB001)
//				unsigned char *pENCKey					// 数据加密密钥(密钥地址:0xA080)
//				unsigned char *pMACKey					// MAC计算密钥 (密钥地址:0xA081)
// 输出参数:    unsigned char *pCardInfo				// 返回16字节数据 供选择卡片使用
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	若IID不符合，则返回的信息为16字节的随机数，若IID有效，则信息如下
//				pCardInfo[1:2] -- PiccCap2
//				pCardInfo[0] = 0x83		4字节卡号	pCardInfo[3:6] -- 4字节卡号
//				pCardInfo[0] = 0x03		7字节卡号	pCardInfo[3:9] -- 7字节卡号
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL3VCSupportLast(const unsigned char *pIID, const unsigned char *pENCKey, 
										 const unsigned char *pMACKey, unsigned char *pCardInfo);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SL3VCSelect(unsigned char *pCardInfo, unsigned char *pMACKey)
// 功能描述:    选择卡(虚拟卡)
// 输入参数:    unsigned char *pCardInfo				// SupportLast返回16字节数据 供选择卡片使用
//              unsigned char *pMACKey					// 计算MAC的密钥(密钥地址: 0xA000)
// 输出参数:    -             
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	PLUS S不支持该命令	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL3VCSelect(const unsigned char *pCardInfo, const unsigned char *pMACKey);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char SL3VCDeselect(void)
// 功能描述:    取消虚拟卡选择
// 输入参数:    -
// 输出参数:    -             
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	PLUS S不支持该命令	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall SL3VCDeselect(void);
//===========================================================================================================
#ifdef __cplusplus
	}
#endif

#endif				// __MUR200_H
