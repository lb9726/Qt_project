#ifndef CONTROLCAN_H
#define CONTROLCAN_H

//接口卡类型定义
#define VCI_PCI5121		1
#define VCI_PCI9810		2
#define VCI_USBCAN1		3
#define VCI_USBCAN2		4
#define VCI_USBCAN2A	4
#define VCI_PCI9820		5
#define VCI_CAN232		6
#define VCI_PCI5110		7
#define VCI_CANLITE		8
#define VCI_ISA9620		9
#define VCI_ISA5420		10
#define VCI_PC104CAN	11
#define VCI_CANETUDP	12
#define VCI_CANETE		12
#define VCI_DNP9810		13
#define VCI_PCI9840		14
#define VCI_PC104CAN2	15
#define VCI_PCI9820I	16
#define VCI_CANETTCP	17
#define VCI_PEC9920		18
#define VCI_PCIE_9220	18
#define VCI_PCI5010U	19
#define VCI_USBCAN_E_U	20
#define VCI_USBCAN_2E_U 21
#define VCI_PCI5020U	22
#define VCI_EG20T_CAN	23
#define VCI_PCIE9221	24
#define VCI_WIFICAN_TCP  25
#define VCI_WIFICAN_UDP 26
#define VCI_PCIe9120 27
#define VCI_PCIe9110 28
#define VCI_PCIe9140 29


//CAN错误码
#define	ERR_CAN_OVERFLOW			0x0001	//CAN控制器内部FIFO溢出
#define	ERR_CAN_ERRALARM			0x0002	//CAN控制器错误报警
#define	ERR_CAN_PASSIVE				0x0004	//CAN控制器消极错误
#define	ERR_CAN_LOSE				0x0008	//CAN控制器仲裁丢失
#define	ERR_CAN_BUSERR				0x0010	//CAN控制器总线错误
#define ERR_CAN_BUSOFF				0x0020 //总线关闭错误
//通用错误码
#define	ERR_DEVICEOPENED			0x0100	//设备已经打开
#define	ERR_DEVICEOPEN				0x0200	//打开设备错误
#define	ERR_DEVICENOTOPEN			0x0400	//设备没有打开
#define	ERR_BUFFEROVERFLOW			0x0800	//缓冲区溢出
#define	ERR_DEVICENOTEXIST			0x1000	//此设备不存在
#define	ERR_LOADKERNELDLL			0x2000	//装载动态库失败
#define ERR_CMDFAILED				0x4000	//执行命令失败错误码
#define	ERR_BUFFERCREATE			0x8000	//内存不足


//函数调用返回状态值
#define	STATUS_OK					1
#define STATUS_ERR					0

#define CMD_DESIP			0
#define CMD_DESPORT			1
#define CMD_CHGDESIPANDPORT		2
#define CMD_SRCPORT			2		
#define CMD_TCP_TYPE		4					//tcp 工作方式，服务器:1 或是客户端:0
#define TCP_CLIENT			0
#define TCP_SERVER			1
//服务器方式下有效
#define CMD_CLIENT_COUNT    5					//连接上的客户端计数
#define CMD_CLIENT			6					//连接上的客户端
#define CMD_DISCONN_CLINET  7					//断开一个连接
#define CMD_SET_RECONNECT_TIME 8			//使能自动重连

typedef struct tagRemoteClient{
	int iIndex;
        unsigned short port;
        unsigned int hClient;
	char szip[32];
}REMOTE_CLIENT;


typedef struct _tagChgDesIPAndPort
{
	char szpwd[10];
	char szdesip[20];
	int desport;
        char blistenonly;
}CHGDESIPANDPORT;

//1.ZLGCAN系列接口卡信息的数据类型。
typedef  struct  _VCI_BOARD_INFO{
        unsigned short	hw_Version;
        unsigned short	fw_Version;
        unsigned short	dr_Version;
        unsigned short	in_Version;
        unsigned short	irq_Num;
        char	can_Num;
        char	str_Serial_Num[20];
        char	str_hw_Type[40];
        unsigned short	Reserved[4];
} VCI_BOARD_INFO,*PVCI_BOARD_INFO; 

//2.定义CAN信息帧的数据类型。
typedef  struct  _VCI_CAN_OBJ{
    unsigned int	ID;
    unsigned int	TimeStamp;
    char	TimeFlag;
    char	SendType;
    char	RemoteFlag;//是否是远程帧
    char	ExternFlag;//是否是扩展帧
    char	DataLen;
    char	Data[8];
    char	Reserved[3];
}VCI_CAN_OBJ,*PVCI_CAN_OBJ;

//3.定义CAN控制器状态的数据类型。
typedef struct _VCI_CAN_STATUS{
        unsigned char	ErrInterrupt;
        unsigned char	regMode;
        unsigned char	regStatus;
        unsigned char	regALCapture;
        unsigned char	regECCapture;
        unsigned char	regEWLimit;
        unsigned char	regRECounter;
        unsigned char	regTECounter;
        unsigned short	Reserved;
}VCI_CAN_STATUS,*PVCI_CAN_STATUS;

//4.定义错误信息的数据类型。
typedef struct _VCI_ERR_INFO{
        unsigned int	ErrCode;
        char	Passive_ErrData[3];
        char	ArLost_ErrData;
} VCI_ERR_INFO,*PVCI_ERR_INFO;

//5.定义初始化CAN的数据类型
typedef struct _VCI_INIT_CONFIG{
    unsigned long	AccCode;
    unsigned long	AccMask;
    unsigned long	Reserved;
    unsigned char	Filter;
    unsigned char	Timing0;
    unsigned char	Timing1;
    unsigned char	Mode;
}VCI_INIT_CONFIG,*PVCI_INIT_CONFIG;


///////// new add struct for filter /////////
typedef struct _VCI_FILTER_RECORD{
    unsigned long ExtFrame;	//是否为扩展帧
    unsigned long Start;
    unsigned long End;
}VCI_FILTER_RECORD,*PVCI_FILTER_RECORD;

//定时自动发送帧结构
typedef struct _VCI_AUTO_SEND_OBJ{
        char Enable;//使能本条报文.  0：禁能   1：使能
        char Index;  //报文编号.   最大支持32条报文
        unsigned short Interval;//定时发送时间。1ms为单位
	VCI_CAN_OBJ obj;//报文
}VCI_AUTO_SEND_OBJ,*PVCI_AUTO_SEND_OBJ;

#define EXTERNC		extern "C"

EXTERNC unsigned long __stdcall VCI_OpenDevice(unsigned long DeviceType,unsigned long DeviceInd,unsigned long Reserved);
EXTERNC unsigned long __stdcall VCI_CloseDevice(unsigned long DeviceType,unsigned long DeviceInd);
EXTERNC unsigned long __stdcall VCI_InitCAN(unsigned long DeviceType, unsigned long DeviceInd, unsigned long CANInd, PVCI_INIT_CONFIG pInitConfig);

EXTERNC unsigned long __stdcall VCI_ReadBoardInfo(unsigned long DeviceType,unsigned long DeviceInd,PVCI_BOARD_INFO pInfo);
EXTERNC unsigned short __stdcall VCI_ReadErrInfo(unsigned short DeviceType,unsigned short DeviceInd,unsigned short CANInd,PVCI_ERR_INFO pErrInfo);
EXTERNC unsigned short __stdcall VCI_ReadCANStatus(unsigned short DeviceType,unsigned short DeviceInd,unsigned short CANInd,PVCI_CAN_STATUS pCANStatus);

EXTERNC unsigned long __stdcall VCI_SetReference(unsigned long DeviceType,unsigned long DeviceInd,unsigned long CANInd,unsigned long RefType,void * pData);
//EXTERNC unsigned short __stdcall VCI_SetReference(unsigned short DeviceType,unsigned short DeviceInd,unsigned short CANInd,unsigned short RefType,void * pData);

EXTERNC unsigned long __stdcall VCI_GetReceiveNum(unsigned long DeviceType,unsigned long DeviceInd,unsigned long CANInd);
EXTERNC unsigned long __stdcall VCI_ClearBuffer(unsigned long DeviceType,unsigned long DeviceInd,unsigned long CANInd);

EXTERNC unsigned long __stdcall VCI_StartCAN(unsigned long DeviceType,unsigned long DeviceInd,unsigned long CANInd);
EXTERNC unsigned long __stdcall VCI_ResetCAN(unsigned long DeviceType,unsigned long DeviceInd,unsigned long CANInd);

EXTERNC unsigned long __stdcall VCI_Transmit(unsigned long DeviceType,unsigned long DeviceInd,unsigned long CANInd,PVCI_CAN_OBJ pSend,unsigned long Len);
EXTERNC unsigned long __stdcall VCI_Receive(unsigned long DeviceType,unsigned long DeviceInd,unsigned long CANInd,PVCI_CAN_OBJ pReceive,unsigned long Len,int WaitTime);


#endif
