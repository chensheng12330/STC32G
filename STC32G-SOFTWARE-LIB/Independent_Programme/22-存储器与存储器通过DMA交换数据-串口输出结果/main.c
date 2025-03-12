/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCAI.com ---------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- BBS: www.STCAIMCU.com  -----------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����            */
/*---------------------------------------------------------------------*/

#include	"config.h"
#include	"STC32G_GPIO.h"
#include	"STC32G_UART.h"
#include	"STC32G_DMA.h"
#include	"STC32G_NVIC.h"
#include	"STC32G_Switch.h"

/*************   ����˵��   ***************

��������ʾDMA Memory to Memory����ת��.

����2���洢���ռ䣬һ������һ�����գ��ֱ��ʼ��2���洢���ռ�����.

����M2M DMA���ϵ���Զ������ʹ洢�������д�뵽���մ洢���ռ�.

���ò�ͬ�Ķ�ȡ˳��д��˳�򣬽��յ���ͬ�����ݽ��.

ͨ������2(P4.6 P4.7)��ӡ���մ洢������(�ϵ��ӡһ��).

�ö�ʱ���������ʷ�����������ʹ��1Tģʽ(���ǵͲ�������12T)����ѡ��ɱ�������������ʱ��Ƶ�ʣ�����߾���.

����ʱ, ѡ��ʱ�� 22.1184MHz (�����������ļ�"config.h"���޸�).

******************************************/

/*************	���س�������	**************/


/*************	���ر�������	**************/

u8 xdata DmaTxBuffer[256];
u8 xdata DmaRxBuffer[256];

/*************	���غ�������	**************/


/*************  �ⲿ�����ͱ������� *****************/


/******************** IO������ ********************/
void	GPIO_config(void)
{
	P4_MODE_IO_PU(GPIO_Pin_6 | GPIO_Pin_7);		//P4.6,P4.7 ����Ϊ׼˫���
}

/******************** UART���� ********************/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;		//�ṹ����

	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,   UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
//	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//ѡ�����ʷ�����, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2, ���Բ���ѡ��)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//������,     110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE �� DISABLE
	UART_Configuration(UART2, &COMx_InitStructure);		//��ʼ������2 UART1,UART2,UART3,UART4
	NVIC_UART2_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

	UART2_SW(UART2_SW_P46_P47);		//UART2_SW_P10_P11,UART2_SW_P46_P47
}

/******************** DMA ���� ********************/
void	DMA_config(void)
{
	DMA_M2M_InitTypeDef		DMA_M2M_InitStructure;		//�ṹ����

	DMA_M2M_InitStructure.DMA_Enable = ENABLE;			//DMAʹ��  	ENABLE,DISABLE
	DMA_M2M_InitStructure.DMA_Length = 127;					//DMA�������ֽ���  	(0~255) + 1

	DMA_M2M_InitStructure.DMA_Tx_Buffer = (u16)DmaTxBuffer;	//�������ݴ洢��ַ
	DMA_M2M_InitStructure.DMA_Rx_Buffer = (u16)DmaRxBuffer;	//�������ݴ洢��ַ
	DMA_M2M_InitStructure.DMA_SRC_Dir = M2M_ADDR_INC;		//����Դ��ַ�ı䷽��  	M2M_ADDR_INC,M2M_ADDR_DEC
	DMA_M2M_InitStructure.DMA_DEST_Dir = M2M_ADDR_INC;	//����Ŀ���ַ�ı䷽�� 	M2M_ADDR_INC,M2M_ADDR_DEC

//	DMA_M2M_InitStructure.DMA_Tx_Buffer = (u16)&DmaTxBuffer[255];	//�������ݴ洢��ַ
//	DMA_M2M_InitStructure.DMA_Rx_Buffer = (u16)DmaRxBuffer;	//�������ݴ洢��ַ
//	DMA_M2M_InitStructure.DMA_SRC_Dir = M2M_ADDR_DEC;		//����Դ��ַ�ı䷽��  	M2M_ADDR_INC,M2M_ADDR_DEC
//	DMA_M2M_InitStructure.DMA_DEST_Dir = M2M_ADDR_INC;	//����Ŀ���ַ�ı䷽�� 	M2M_ADDR_INC,M2M_ADDR_DEC

//	DMA_M2M_InitStructure.DMA_Tx_Buffer = (u16)DmaTxBuffer;	//�������ݴ洢��ַ
//	DMA_M2M_InitStructure.DMA_Rx_Buffer = (u16)&DmaRxBuffer[255];	//�������ݴ洢��ַ
//	DMA_M2M_InitStructure.DMA_SRC_Dir = M2M_ADDR_INC;		//����Դ��ַ�ı䷽��  	M2M_ADDR_INC,M2M_ADDR_DEC
//	DMA_M2M_InitStructure.DMA_DEST_Dir = M2M_ADDR_DEC;	//����Ŀ���ַ�ı䷽�� 	M2M_ADDR_INC,M2M_ADDR_DEC

//	DMA_M2M_InitStructure.DMA_Tx_Buffer = (u16)&DmaTxBuffer[255];	//�������ݴ洢��ַ
//	DMA_M2M_InitStructure.DMA_Rx_Buffer = (u16)&DmaRxBuffer[255];	//�������ݴ洢��ַ
//	DMA_M2M_InitStructure.DMA_SRC_Dir = M2M_ADDR_DEC;		//����Դ��ַ�ı䷽��  	M2M_ADDR_INC,M2M_ADDR_DEC
//	DMA_M2M_InitStructure.DMA_DEST_Dir = M2M_ADDR_DEC;	//����Ŀ���ַ�ı䷽�� 	M2M_ADDR_INC,M2M_ADDR_DEC

	DMA_M2M_Inilize(&DMA_M2M_InitStructure);		//��ʼ��
	NVIC_DMA_M2M_Init(ENABLE,Priority_0,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0~Priority_3; �������ȼ�(�͵���) Priority_0~Priority_3
}

/******************** task A **************************/
void main(void)
{
	u16	i;

	WTST = 0;		//���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
	EAXSFR();		//��չSFR(XFR)����ʹ�� 
	CKCON = 0;      //��߷���XRAM�ٶ�

	GPIO_config();
	UART_config();
	DMA_config();
	EA = 1;

	printf("STC32G Memory to Memory DMA Test Programme!\r\n");  //UART����һ���ַ���
	
	for(i=0; i<256; i++)
	{
		DmaTxBuffer[i] = i;
		DmaRxBuffer[i] = 0;
	}
	DMA_M2M_TRIG();		//��������ת��
	
	while (1)
	{
		if(DmaM2MFlag)
		{
			DmaM2MFlag = 0;

			for(i=0; i<256; i++)
			{
				printf("%02X ", DmaRxBuffer[i]);
				if((i & 0x0f) == 0x0f)
                {
					printf("\r\n");
                }
			}
		}
	}
}



