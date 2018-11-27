#ifndef CONTROLCAN_H
#define CONTROLCAN_H

////文件版本：v2.00 20150920
//#include <cvidef.h>	//使用CVI平台开发，请使用该语句。

//接口卡类型定义

#define VCI_USBCAN1		3
#define VCI_USBCAN2		4
#define VCI_USBCAN2A		4

#define VCI_USBCAN_E_U 		20
#define VCI_USBCAN_2E_U 	21

//函数调用返回状态值
#define	STATUS_OK					1
#define STATUS_ERR					0
	
/*------------------------------------------------兼容ZLG的函数及数据类型------------------------------------------------*/
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

//3.定义初始化CAN的数据类型
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
 
#define EXTERNC		extern "C"

EXTERNC unsigned long __stdcall VCI_OpenDevice(unsigned long DeviceType,unsigned long DeviceInd,unsigned long Reserved);
EXTERNC unsigned long __stdcall VCI_CloseDevice(unsigned long DeviceType,unsigned long DeviceInd);
EXTERNC unsigned long __stdcall VCI_InitCAN(unsigned long DeviceType, unsigned long DeviceInd, unsigned long CANInd, PVCI_INIT_CONFIG pInitConfig);

EXTERNC unsigned long __stdcall VCI_ReadBoardInfo(unsigned long DeviceType,unsigned long DeviceInd,PVCI_BOARD_INFO pInfo);

EXTERNC unsigned long __stdcall VCI_SetReference(unsigned long DeviceType,unsigned long DeviceInd,unsigned long CANInd,unsigned long RefType,void * pData);

EXTERNC unsigned long __stdcall VCI_GetReceiveNum(unsigned long DeviceType,unsigned long DeviceInd,unsigned long CANInd);
EXTERNC unsigned long __stdcall VCI_ClearBuffer(unsigned long DeviceType,unsigned long DeviceInd,unsigned long CANInd);

EXTERNC unsigned long __stdcall VCI_StartCAN(unsigned long DeviceType,unsigned long DeviceInd,unsigned long CANInd);
EXTERNC unsigned long __stdcall VCI_ResetCAN(unsigned long DeviceType,unsigned long DeviceInd,unsigned long CANInd);

EXTERNC unsigned long __stdcall VCI_Transmit(unsigned long DeviceType,unsigned long DeviceInd,unsigned long CANInd,PVCI_CAN_OBJ pSend,unsigned long Len);
EXTERNC unsigned long __stdcall VCI_Receive(unsigned long DeviceType,unsigned long DeviceInd,unsigned long CANInd,PVCI_CAN_OBJ pReceive,unsigned long Len,int WaitTime);


/*------------------------------------------------其他补充函数及数据结构描述------------------------------------------------*/

//USB-CAN总线适配器板卡信息的数据类型1，该类型为VCI_FindUsbDevice函数的返回参数。
typedef  struct  _VCI_BOARD_INFO1{
    unsigned short	hw_Version;
    unsigned short	fw_Version;
    unsigned short	dr_Version;
    unsigned short	in_Version;
    unsigned short	irq_Num;
    char	can_Num;
    char	Reserved;
    char	str_Serial_Num[8];
    char	str_hw_Type[16];
    char	str_Usb_Serial[4][4];
} VCI_BOARD_INFO1,*PVCI_BOARD_INFO1;

//USB-CAN总线适配器板卡信息的数据类型2，该类型为VCI_FindUsbDevice函数的返回参数。为扩展更多的设备
typedef  struct  _VCI_BOARD_INFO2{
    unsigned short	hw_Version;
    unsigned short	fw_Version;
    unsigned short	dr_Version;
    unsigned short	in_Version;
    unsigned short	irq_Num;
    char	can_Num;
    char	Reserved;
    char	str_Serial_Num[8];
    char	str_hw_Type[16];
    char	str_Usb_Serial[10][4];
} VCI_BOARD_INFO2,*PVCI_BOARD_INFO2;


#define EXTERNC		extern "C"

EXTERNC unsigned long __stdcall VCI_GetReference2(unsigned long DevType,unsigned long DevIndex,unsigned long CANIndex,unsigned long Reserved,char *pData);
EXTERNC unsigned long __stdcall VCI_SetReference2(unsigned long DevType,unsigned long DevIndex,unsigned long CANIndex,unsigned long RefType,char *pData);

EXTERNC unsigned long __stdcall VCI_ConnectDevice(unsigned long DevType,unsigned long DevIndex);
EXTERNC unsigned long __stdcall VCI_UsbDeviceReset(unsigned long DevType,unsigned long DevIndex,unsigned long Reserved);
EXTERNC unsigned long __stdcall VCI_FindUsbDevice(PVCI_BOARD_INFO1 pInfo);
EXTERNC unsigned long __stdcall VCI_FindUsbDevice2(PVCI_BOARD_INFO2 pInfo);

#endif
