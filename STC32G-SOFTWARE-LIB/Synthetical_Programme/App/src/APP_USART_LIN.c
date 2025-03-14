/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序            */
/*---------------------------------------------------------------------*/

#include	"APP_USART_LIN.h"
#include	"STC32G_USART_LIN.h"
#include	"STC32G_GPIO.h"
#include	"STC32G_UART.h"
#include	"STC32G_NVIC.h"
#include	"STC32G_Switch.h"

/*************   功能说明   ***************

Lin主机模式收发测试：
按一下P32口按键, 主机发送完整一帧数据.
按一下P33口按键, 主机发送帧头并获取从机应答数据（合并成一串完整的帧）.

Lin从机模式收发测试：
收到一个非本机应答的完整帧后通过串口2输出.
收到一个本机应答的帧头后(例如：ID=0x12), 发送缓存数据进行应答.
需要修改头文件 "STC32G_UART.h" 里的定义 "#define	PRINTF_SELECT  UART2"，通过串口2打印信息

默认传输速率：9600波特率, 用户可自行修改.

下载时, 选择时钟 24MHz (用户可在"config.h"修改频率).

******************************************/

sbit SLP_N  = P5^2;     //0: Sleep

//========================================================================
//                               本地常量声明	
//========================================================================

#define	USART1_LIN_MASTER_MODE     1    //0: 从机模式; 1: 主机模式

//========================================================================
//                               本地变量声明
//========================================================================

u8 ULin_ID;
u8 USART_BUF[8];

//========================================================================
//                               本地函数声明
//========================================================================


//========================================================================
//                            外部函数和变量声明
//========================================================================

extern bit B_ULinRX1_Flag;

extern u8 Key1_cnt;
extern u8 Key2_cnt;
extern bit Key1_Flag;
extern bit Key2_Flag;

//========================================================================
// 函数: LIN_init
// 描述: 用户初始化程序.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022-03-27
//========================================================================
void USART_LIN_init(void)
{
	USARTx_LIN_InitDefine	LIN_InitStructure;				//结构定义
	COMx_InitDefine COMx_InitStructure;				//结构定义

	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,   UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
//	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//选择波特率发生器, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2, 所以不用选择)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率,     110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE 或 DISABLE
	UART_Configuration(UART2, &COMx_InitStructure);		//初始化串口2 UART1,UART2,UART3,UART4
	NVIC_UART2_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3

#if(USART1_LIN_MASTER_MODE==1)
	LIN_InitStructure.LIN_Mode = LinMasterMode;	//LIN总线模式  	LinMasterMode,LinSlaveMode
	LIN_InitStructure.LIN_AutoSync = DISABLE;		//自动同步使能  	ENABLE,DISABLE
#else
	LIN_InitStructure.LIN_Mode = LinSlaveMode;	//LIN总线模式  	LinMasterMode,LinSlaveMode
	LIN_InitStructure.LIN_AutoSync = ENABLE;		//自动同步使能  	ENABLE,DISABLE
#endif
	LIN_InitStructure.LIN_Enable   = ENABLE;		//LIN功能使能  	ENABLE,DISABLE
	LIN_InitStructure.LIN_Baudrate = 9600;			//LIN波特率
	UASRT_LIN_Configuration(USART1,&LIN_InitStructure);						//LIN 初始化

	NVIC_UART1_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3

	P4_MODE_IO_PU(GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_7);		//P4.3,P4.4,P4.6,P4.7 设置为准双向口
	P5_MODE_IO_PU(GPIO_Pin_2);		//P5.2 设置为准双向口
	
	UART1_SW(UART1_SW_P43_P44);		//UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44

	//====初始化数据=====
	SLP_N = 1;
	ULin_ID = 0x32;
	USART_BUF[0] = 0x81;
	USART_BUF[1] = 0x22;
	USART_BUF[2] = 0x33;
	USART_BUF[3] = 0x44;
	USART_BUF[4] = 0x55;
	USART_BUF[5] = 0x66;
	USART_BUF[6] = 0x77;
	USART_BUF[7] = 0x88;
}

//========================================================================
// 函数: Sample_LIN
// 描述: 用户应用程序.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2022-03-27
//========================================================================
void Sample_USART_LIN(void)
{
	u8 i;

#if(USART1_LIN_MASTER_MODE==1)
	if(!P32)
	{
		if(!Key1_Flag)
		{
			Key1_cnt++;
			if(Key1_cnt > 50)
			{
				Key1_Flag = 1;
				UsartLinSendFrame(USART1,ULin_ID, USART_BUF, FRAME_LEN);  //发送一串完整数据
			}
		}
	}
	else
	{
		Key1_cnt = 0;
		Key1_Flag = 0;
	}

	if(!P33)
	{
		if(!Key2_Flag)
		{
			Key2_cnt++;
			if(Key2_cnt > 50)
			{
				Key2_Flag = 1;
				UsartLinSendHeader(USART1,0x13);  //发送帧头，获取数据帧，组成一个完整的帧
			}
		}
	}
	else
	{
		Key2_cnt = 0;
		Key2_Flag = 0;
	}
#else
	if((B_ULinRX1_Flag) && (COM1.RX_Cnt >= 2))
	{
		B_ULinRX1_Flag = 0;

		if((RX1_Buffer[0] == 0x55) && ((RX1_Buffer[1] & 0x3f) == 0x12)) //PID -> ID
		{
			UsartLinSendData(USART1,USART_BUF, FRAME_LEN);
			UsartLinSendChecksum(USART1,USART_BUF, FRAME_LEN);
		}
	}
#endif

	if(COM1.RX_TimeOut > 0)     //超时计数
	{
		if(--COM1.RX_TimeOut == 0)
		{
			printf("Read Cnt = %d.\r\n",COM1.RX_Cnt);
			for(i=0; i<COM1.RX_Cnt; i++)    printf("0x%02x ",RX1_Buffer[i]);    //从串口输出收到的从机数据
			COM1.RX_Cnt  = 0;   //清除字节数
			printf("\r\n");
		}
	}
}

