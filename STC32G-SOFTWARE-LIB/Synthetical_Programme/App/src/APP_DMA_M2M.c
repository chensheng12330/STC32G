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

#include	"APP_DMA_M2M.h"
#include	"STC32G_GPIO.h"
#include	"STC32G_UART.h"
#include	"STC32G_DMA.h"
#include	"STC32G_NVIC.h"


/*************	功能说明	**************

本程序演示DMA Memory to Memory数据转移.

设置2个存储器空间，一个发送一个接收，分别初始化2个存储器空间内容.

设置M2M DMA，上电后自动将发送存储里的内容写入到接收存储器空间.

根据不同的读取顺序、写入顺序，接收到不同的数据结果.

通过串口2(P4.6 P4.7)打印接收存储器数据(上电打印一次).

用定时器做波特率发生器，建议使用1T模式(除非低波特率用12T)，并选择可被波特率整除的时钟频率，以提高精度.

下载时, 选择时钟 24MHz (可以在配置文件"config.h"中修改).

******************************************/


//========================================================================
//                               本地常量声明	
//========================================================================


//========================================================================
//                               本地变量声明
//========================================================================

u8 xdata DmaTxBuffer[256];
u8 xdata DmaRxBuffer[256];

//========================================================================
//                               本地函数声明
//========================================================================


//========================================================================
//                            外部函数和变量声明
//========================================================================


//========================================================================
// 函数: DMA_M2M_init
// 描述: 用户初始化程序.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2020-09-28
//========================================================================
void DMA_M2M_init(void)
{
	u16	i;
	COMx_InitDefine		COMx_InitStructure;		//结构定义
	DMA_M2M_InitTypeDef		DMA_M2M_InitStructure;		//结构定义

	//----------------------------------------------
	P4_MODE_IO_PU(GPIO_Pin_6 | GPIO_Pin_7);		//P4.6,P4.7 设置为准双向口
	
	//----------------------------------------------
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,   UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
//	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//选择波特率发生器, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2, 所以不用选择)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率,     110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE 或 DISABLE
	UART_Configuration(UART2, &COMx_InitStructure);		//初始化串口2 UART1,UART2,UART3,UART4
	NVIC_UART2_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
	printf("STC32G Memory to Memory DMA Test Programme!\r\n");  //UART发送一个字符串

	//----------------------------------------------
	DMA_M2M_InitStructure.DMA_Enable = ENABLE;			//DMA使能  	ENABLE,DISABLE
	DMA_M2M_InitStructure.DMA_Length = 127;					//DMA传输总字节数  	(0~255) + 1

//	DMA_M2M_InitStructure.DMA_Tx_Buffer = (u16)DmaTxBuffer;	//发送数据存储地址
//	DMA_M2M_InitStructure.DMA_Rx_Buffer = (u16)DmaRxBuffer;	//接收数据存储地址
//	DMA_M2M_InitStructure.DMA_SRC_Dir = M2M_ADDR_INC;		//数据源地址改变方向  	M2M_ADDR_INC,M2M_ADDR_DEC
//	DMA_M2M_InitStructure.DMA_DEST_Dir = M2M_ADDR_INC;	//数据目标地址改变方向 	M2M_ADDR_INC,M2M_ADDR_DEC

//	DMA_M2M_InitStructure.DMA_Tx_Buffer = (u16)&DmaTxBuffer[255];	//发送数据存储地址
//	DMA_M2M_InitStructure.DMA_Rx_Buffer = (u16)DmaRxBuffer;	//接收数据存储地址
//	DMA_M2M_InitStructure.DMA_SRC_Dir = M2M_ADDR_DEC;		//数据源地址改变方向  	M2M_ADDR_INC,M2M_ADDR_DEC
//	DMA_M2M_InitStructure.DMA_DEST_Dir = M2M_ADDR_INC;	//数据目标地址改变方向 	M2M_ADDR_INC,M2M_ADDR_DEC

//	DMA_M2M_InitStructure.DMA_Tx_Buffer = (u16)DmaTxBuffer;	//发送数据存储地址
//	DMA_M2M_InitStructure.DMA_Rx_Buffer = (u16)&DmaRxBuffer[255];	//接收数据存储地址
//	DMA_M2M_InitStructure.DMA_SRC_Dir = M2M_ADDR_INC;		//数据源地址改变方向  	M2M_ADDR_INC,M2M_ADDR_DEC
//	DMA_M2M_InitStructure.DMA_DEST_Dir = M2M_ADDR_DEC;	//数据目标地址改变方向 	M2M_ADDR_INC,M2M_ADDR_DEC

	DMA_M2M_InitStructure.DMA_Tx_Buffer = (u16)&DmaTxBuffer[255];	//发送数据存储地址
	DMA_M2M_InitStructure.DMA_Rx_Buffer = (u16)&DmaRxBuffer[255];	//接收数据存储地址
	DMA_M2M_InitStructure.DMA_SRC_Dir = M2M_ADDR_DEC;		//数据源地址改变方向  	M2M_ADDR_INC,M2M_ADDR_DEC
	DMA_M2M_InitStructure.DMA_DEST_Dir = M2M_ADDR_DEC;	//数据目标地址改变方向 	M2M_ADDR_INC,M2M_ADDR_DEC

	DMA_M2M_Inilize(&DMA_M2M_InitStructure);		//初始化
	NVIC_DMA_M2M_Init(ENABLE,Priority_0,Priority_0);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0~Priority_3; 总线优先级(低到高) Priority_0~Priority_3

	for(i=0; i<256; i++)
	{
		DmaTxBuffer[i] = i;
		DmaRxBuffer[i] = 0;
	}
	DMA_M2M_TRIG();		//触发启动转换
}

//========================================================================
// 函数: Sample_DMA_M2M
// 描述: 用户应用程序.
// 参数: None.
// 返回: None.
// 版本: V1.0, 2020-09-24
//========================================================================
void Sample_DMA_M2M(void)
{
	u16	i;

	if(DmaM2MFlag)
	{
		DmaM2MFlag = 0;

		for(i=0; i<256; i++)
		{
			printf("%02X ", DmaRxBuffer[i]);
			if((i & 0x0f) == 0x0f)
				printf("\r\n");
		}
	}
}

