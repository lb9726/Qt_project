
#define SEQNR		0
#define COMMAND		1	
#define STATUS		1
#define LENGTH		2		
#define DATA		3

#define MI_OK			0	//函数调用成功
#define COMM_OK			0
#define MI_NOTAGERR		1	//在有效区域内没有卡
#define MI_CRCERR		2	//从卡中接收到了错误的CRC校验和
#define MI_EMPTY		3	//值溢出
#define MI_AUTHERR		4	//不能验证
#define MI_PARITYERR	5	//从卡中接收到了错误的校验位
#define MI_CODEERR		6	//通信错误
#define MI_SENDRERR		8	//在防冲突时读到了错误的串行码
#define MI_NOTAUTHERR	10	//卡没有验证
#define MI_BITCOUNTERR	11	//从卡中接收到了错误数量的位
#define MI_BYTECOUNTERR	12	//从卡中接收了错误数量的字节
#define MI_TRANSERR		14	//调用Transfer函数出错
#define MI_WRITEERR		15	//调用Write函数出错
#define MI_INCRERR		16	//调用Increment函数出错
#define MI_DECRERR		17	//调用Decrment函数出错
#define MI_READERR		18	//调用Read函数出错
#define MI_COLLERR		24	//冲突错
#define MI_QUIT			30	//上一次了送命令时被打断
#define MIS_CHK_OK		0	//Check Write正确
#define MIS_CHK_FAILED	1	//Check Write出错
#define MIS_CHK_COMPERR	2	//Check Write:写出错（比较出错）
#define COMM_ERR		255	//串行通信错误

#define ALL		0x01
#define IDLE	0x00
#define KEYA	0x00
#define KEYB	0x04
#define PICC_ANTICOLL1     0x93					// 选择等级1
#define PICC_ANTICOLL2     0x95					// 选择等级2
#define PICC_ANTICOLL3     0x97					// 选择等级3
#define PICC_DECREMENT     0xC0         		// 减值
#define PICC_INCREMENT     0xC1         		// 加值

unsigned char __stdcall RC500USB_request(unsigned char mode,unsigned short &tagtype);
unsigned char __stdcall RC500USB_anticoll(unsigned char bcnt,unsigned long &snr);
unsigned char __stdcall RC500USB_anticoll2(unsigned char encoll,unsigned char bcnt,unsigned long &snr);
unsigned char __stdcall RC500USB_select(unsigned long snr,unsigned char &size);
unsigned char __stdcall RC500USB_authentication(unsigned char mode,unsigned char secnr);
unsigned char __stdcall RC500USB_authentication2(unsigned char mode,unsigned char secnr,unsigned char keynr);
unsigned char __stdcall RC500USB_authkey(unsigned char mode,unsigned char *key,unsigned char secnr);
unsigned char __stdcall RC500USB_halt(void);
unsigned char __stdcall RC500USB_read(unsigned char addr,unsigned char *data);
unsigned char __stdcall RC500USB_write(unsigned char addr,unsigned char *data);
unsigned char __stdcall RC500USB_writeval(unsigned char addr,long value);
unsigned char __stdcall RC500USB_readval(unsigned char addr,long &value);
unsigned char __stdcall RC500USB_changepwd(unsigned char mode,unsigned char secnr,unsigned char *pwd);
unsigned char __stdcall RC500USB_value(unsigned char mode,unsigned char addr,long &value,unsigned char trans_addr);
unsigned char __stdcall RC500USB_load_key(unsigned char mode,unsigned char secnr,unsigned char *key);
unsigned char __stdcall RC500USB_reset(unsigned char msec);
unsigned char __stdcall RC500USB_close(void);
unsigned char __stdcall RC500USB_config(void);
unsigned char __stdcall RC500USB_get_info(unsigned char *info);
unsigned char __stdcall RC500USB_check_write(unsigned char snr,unsigned char authenmode,unsigned char addr,unsigned char *data);
unsigned char __stdcall RC500USB_set_control_bit();
unsigned char __stdcall RC500USB_clr_control_bit();
unsigned char __stdcall RC500USB_buzzer(unsigned char contrl,unsigned char opentm,unsigned char closetm,unsigned char repcnt);
unsigned char __stdcall RC500USB_read_E2(unsigned char addr,unsigned char length,unsigned char *data);
unsigned char __stdcall RC500USB_write_E2(unsigned char addr,unsigned char length,unsigned char *data);

unsigned char __stdcall RC500USB_authshc1102(unsigned char keyblock,unsigned char* key);
unsigned char __stdcall RC500USB_readshc1102(unsigned char block,unsigned char* data);
unsigned char __stdcall RC500USB_writeshc1102(unsigned char block,unsigned char* data);

unsigned char  __stdcall RC500USB_init();
unsigned char  __stdcall RC500USB_Reset();
void __stdcall RC500USB_exit();

//new functions for V1.4
void __stdcall RC500USB_GetDllVer(char *ver);
unsigned char __stdcall RC500USB_CaseAnticoll(unsigned char bcnt,unsigned char code,
											  unsigned long &snr);
unsigned char __stdcall RC500USB_CaseSelect(unsigned char code,unsigned long snr,unsigned char &sak);
unsigned char __stdcall RC500USB_ULwrite(unsigned char addr,unsigned char* data);
unsigned char __stdcall RC500USB_ValueDebit(unsigned char mode,unsigned char addr,
											long val);
unsigned char __stdcall RC500USB_Lread(unsigned char addr,unsigned char *data);
unsigned char __stdcall RC500USB_Lwrite(unsigned char addr,unsigned char *data);

unsigned char __stdcall RC500USB_RATS(unsigned char CID,unsigned char *Ats);
unsigned char __stdcall RC500USB_DeSelect(unsigned char CID);
unsigned char __stdcall RC500USB_DSFBTrans(unsigned char *sendbfr,
										   unsigned char sendlen,
										   unsigned char *rcvbfr,
										   unsigned char *rcvlen);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

short int __stdcall mifs_pctomsr(unsigned char *psendbuff,short int len);
short int __stdcall mifs_msrtopc(unsigned char *precbuff,short int len,
								 int waittime=1000);
short int __stdcall mifs_senddata(unsigned char *psendbuff,short int nsendlen,
								  unsigned char *precbuff,short int nreclen=64,
								  int waittime=1000);
void __stdcall mifs_calculate_bcc(unsigned char *pbuff,short int len,unsigned char *presult);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:	unsigned char RC500USB_ExChangeBlock(unsigned char *pDataBfr, 
//													 unsigned char ucSendLen,
//													 unsigned char *pRcvLen, 
//													 unsigned char ucWTXM_CRCen,
//													 unsigned char ucFWI)
// 函数功能:    PCD与PICC交换数据块,命令码0x7e
// 入口参数:    unsigned char *pDataBfr			// 发送数据缓冲区首址
//   			unsigned char ucSendLen    		// 发送数据长度
//				unsigned char ucWTXM_CRCen		// b7--b2为WTXM；
//												// b1 = 0,RFU
//												// b0 = 1，使能CRC；b0 = 0，禁止CRC 
//              unsigned char ucFWI				// 超时时间 = ((0x01 << FWI) * 302) us 
// 出口参数:    unsigned char *pDataBfr			// 接收数据缓冲区首址
//				unsigned char *pRcvLen			// 接收数据长度
// 返 回 值:    执行结果
// 描　  述:    WTXM为时间倍数因子，默认值为4
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char __stdcall RC500USB_ExChangeBlock(unsigned char *pDataBfr, 
											   unsigned char ucSendLen,
											   unsigned char *pRcvLen, 
											   unsigned char ucWTXM_CRCen,
									           unsigned char ucFWI);