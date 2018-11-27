////////////////////////////////////////////Copyright (c)//////////////////////////////////////////
//                                   广州周立功单片机发展有限公司
//                                         研    究    所
//                                           金卡产品线
//
//                                     http://www.zlgmcu.com
//
//-------------- 文件信息--------------------------------------------------------------------------
// 文 件 名:    SWCode.h
// 文件描述:    SW状态代码
// 版    本:    V1.0 
// 创 建 人:    曾本森
// 创建日期:    2009.07.17
// 说    明:	为了方便处理，将SW1 和SW2 合并为一个16位的数据进行处理SW1在高8位
//=================================================================================================
//-------------- 修改记录--------------------------------------------------------------------------
// 修改内容:
// 当前版本:    
// 修 改 人:
// 修改日期:
// 注    意: 
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __SW_CODE_H
#define __SW_CODE_H

//=================================================================================================
#define SW_SUCCESS					0x9000		// 操作成功
#define SW_OTHER_DATA				0x6100		// 操作成功，但还有xx(低8位指出)字节可以可以接收

#define SW_PART_DATA_INVALID		0x6281		// 部分数据可能无效
#define SW_FILE_OVER				0x6282		// 读出Le字节前，文件/记录已结束
#define SW_CHOOSE_FILE_INVALID		0x6283		// 选择的文件无效
#define SW_FCI_INVALID				0x6284		// FCI无效

#define SW_AUTH_ERR					0x6300		// 认证被锁定(认证识别码错误)
#define SW_LAST_WRITE_FULL_FILE		0x6381		// 通过最后写入来填满文件
#define SW_OTHER_ERR				0x63C0		// 0x63Cx x计数器剩余值(如剩余验证次数)

#define SW_FLAGE_NO_CHANGE			0x6400		// 标识状态位未变

//#define SW_NV_ERR					0x6500		// 存储器错误
#define SW_WRITE_ERR				0x6581		// 写出错

#define SW_LENGTH_ERR				0x6700		// 长度错误

#define SW_NV_ERR					0x6800		// 存储器错误
#define SW_NAD_NOT_SUPPORT			0x6881		// 逻辑信道不支持
#define SW_MAC_NOT_SUPPORT			0x6882		// 不支持安全报文模式

#define SW_CONNOT_PERFORM 			0x6900		// 无法处理的操作
#define SW_CMD_REJECT 				0x6901		// 命令不接受(无效状态)
#define SW_CMD_MISMATCH_FILE		0x6981		// 命令与文件结构不兼容
#define SW_SECURITY_NOT_SATISFIED	0x6982		// 安全状态不满足
#define SW_AUTH_LOCK				0x6983		// 认证方式被锁定
#define SW_RND_INVALID				0x6984		// 随机数无效
#define SW_USE_NOT_SATISFIED		0x6985		// 使用条件不满足(应用被锁定)
#define SW_CMD_PROHIBITED			0x6986		// 命令不被允许(无当前EF)
#define SW_NO_SM					0x6987		// 无安全报文(MAC)
#define SW_SM_DATA_ERR				0x6988		// 安全报文数据项不正确

#define SW_PARAMETER_ERR			0x6A00		// 参数错误
#define SW_APP_LOCKED				0x6A81		// 应用被锁定
#define SW_FILE_NOT_FIND			0x6A82		// 文件未找到
#define SW_RECORD_NOT_FIND			0x6A83		// 记录未找到
#define SW_FILE_SPACE_LESS			0x6A84		// 文件空间不足
#define SW_P1_P2_INVALID			0x6A86		// P1/P2参数不正确
#define SW_KEY_NOT_FIND				0x6A88		// 相关密钥未找到

#define SW_OFFSET_ERR				0x6B00		// 偏移量错误
#define SW_LE_ERR					0x6C00		// 0x6Cxx Le长度错误，实际长度是xx
#define SW_INS_INVALID				0x6D00		// INS无效/不支持
#define SW_CLA_INVALID				0x6E00		// CLA无效
#define SW_DATA_INVALID				0x6F00		// 数据无效

#define SW_MAC_ERR					0x9302		// MAC错误(还有0x6988)
#define SW_APP_PERMANENTLY_LOCKED	0x9303		// 应用被永久锁定

#define SW_INSUFFICIENT_MONEY		0x9401		// 金额不足
#define SW_KEY_INDEX_NONSUPPORT		0x9403		// 密钥索引不支持
#define SW_MAC_INVALID				0x9406		// MAC无效
//============= 其他与SW无关的状态代码 ============================================================
#define SW_OK						0x0000		// 操作成功
//=================================================================================================
#endif				// __SW_CODE_H