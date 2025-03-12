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

#include	"STC32G_LIN.h"
#include	"STC32G_GPIO.h"
#include	"STC32G_NVIC.h"
#include	"STC32G_UART.h"
#include	"STC32G_Delay.h"
#include	"STC32G_Switch.h"

/*************   ����˵��   ***************

Lin����ģʽ�շ����ԣ�
��һ��P32�ڰ���, ������������һ֡����.
��һ��P33�ڰ���, ��������֡ͷ����ȡ�ӻ�Ӧ�����ݣ��ϲ���һ��������֡��.

Lin�ӻ�ģʽ�շ����ԣ�
�յ�һ���Ǳ���Ӧ�������֡��ͨ������2���, �����浽���ݻ���.
�յ�һ������Ӧ���֡ͷ��(���磺ID=0x12), ���ͻ������ݽ���Ӧ��.

��Ҫ�޸�ͷ�ļ� "STC32G_UART.h" ��Ķ��� "#define	PRINTF_SELECT  UART1"��ͨ������1��ӡ��Ϣ

Ĭ�ϴ������ʣ�9600������, �û��������޸�.

����ʱ, ѡ��ʱ�� 24MHz (�û�����"config.h"�޸�Ƶ��).

******************************************/

sbit SLP_N  = P5^2;     //0: Sleep

/*************	���س�������	**************/

#define	LIN_MASTER_MODE		1    //0: �ӻ�ģʽ; 1: ����ģʽ

/*************	���ر�������	**************/

bit Key1_Flag;
bit Key2_Flag;

u8 Lin_ID;
u8 TX_BUF[8];
u8 RX_BUF[8];

u8 Key1_cnt;
u8 Key2_cnt;

/*************	���غ�������	**************/


/*************  �ⲿ�����ͱ������� *****************/

extern bit LinRxFlag;

/******************** IO������ ********************/
void GPIO_config(void)
{
	P3_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1);		//P3.0,P3.1 ����Ϊ׼˫���
	P4_MODE_IO_PU(GPIO_Pin_6 | GPIO_Pin_7);		//P4.6,P4.7 ����Ϊ׼˫���
	P5_MODE_IO_PU(GPIO_Pin_2);		//P5.2 ����Ϊ׼˫���
	
	LIN_SW(LIN_P46_P47);					//LIN_P02_P03,LIN_P52_P53,LIN_P46_P47,LIN_P72_P73
	UART1_SW(UART1_SW_P30_P31);		//UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

/******************** LIN ���� ********************/
void LIN_config(void)
{
	LIN_InitTypeDef	LIN_InitStructure;				//�ṹ����

#if(LIN_MASTER_MODE==0)
	LIN_InitStructure.LIN_IE       = LIN_ALLIE;	//LIN�ж�ʹ��  	LIN_LIDE/LIN_RDYE/LIN_ERRE/LIN_ABORTE/LIN_ALLIE,DISABLE
#else
	LIN_InitStructure.LIN_IE       = DISABLE;	//LIN�ж�ʹ��  	LIN_LIDE/LIN_RDYE/LIN_ERRE/LIN_ABORTE/LIN_ALLIE,DISABLE
#endif
	LIN_InitStructure.LIN_Enable   = ENABLE;	//LIN����ʹ��  	ENABLE,DISABLE
	LIN_InitStructure.LIN_Baudrate = 9600;		//LIN������
	LIN_InitStructure.LIN_HeadDelay = 1;			//֡ͷ��ʱ����  	0~(65535*1000)/MAIN_Fosc
	LIN_InitStructure.LIN_HeadPrescaler = 1;	//֡ͷ��ʱ��Ƶ  	0~63
	LIN_Inilize(&LIN_InitStructure);					//LIN ��ʼ��
	
	NVIC_LIN_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}

/******************** UART ���� ********************/
void UART_config(void)
{
	COMx_InitDefine COMx_InitStructure;				//�ṹ����

	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,   UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//ѡ�����ʷ�����, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2, ���Բ���ѡ��)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//������,     110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE �� DISABLE
	UART_Configuration(UART1, &COMx_InitStructure);		//��ʼ������1 UART1,UART2,UART3,UART4
	NVIC_UART1_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}

/**********************************************/
void main(void)
{
	u8 i;

	WTST = 0;		//���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
	EAXSFR();		//��չSFR(XFR)����ʹ�� 
	CKCON = 0;      //��߷���XRAM�ٶ�

	GPIO_config();
	UART_config();
	LIN_config();
	EA = 1;
	//====��ʼ������=====
	SLP_N = 1;
	Lin_ID = 0x32;
	TX_BUF[0] = 0x81;
	TX_BUF[1] = 0x22;
	TX_BUF[2] = 0x33;
	TX_BUF[3] = 0x44;
	TX_BUF[4] = 0x55;
	TX_BUF[5] = 0x66;
	TX_BUF[6] = 0x77;
	TX_BUF[7] = 0x88;

	while (1)
	{
#if(LIN_MASTER_MODE==1)
        delay_ms(1);    //��ѭ��1msѭ��һ��
		if(!P32)
		{
			if(!Key1_Flag)
			{
				Key1_cnt++;
				if(Key1_cnt > 50)
				{
					Key1_Flag = 1;
					LinSendFrame(Lin_ID, TX_BUF);  //����һ֡��������
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
					LinSendHeaderRead(0x13,RX_BUF);  //����֡ͷ����ȡ����֡�����һ��������֡
					printf("�������£�\r\n");
					for(i=0; i<FRAME_LEN; i++)    printf("%02x ", RX_BUF[i]);
					printf("\r\n");
				}
			}
		}
		else
		{
			Key2_cnt = 0;
			Key2_Flag = 0;
		}
#else
		u8 isr;

		if(LinRxFlag == 1)
		{
			LinRxFlag = 0;
			isr = LinReadReg(LID);
			if(isr == 0x12)		//�ж��Ƿ�ӻ���ӦID
			{
				LinTxResponse(TX_BUF);	//������Ӧ����
			}
			else
			{
				LinReadFrame(RX_BUF);			//����Lin��������
				printf("ID=0x%02X, �������ݣ�\r\n",isr);
				for(i=0; i<FRAME_LEN; i++)    printf("%02x ", RX_BUF[i]);
				printf("\r\n");
			}
		}
#endif
	}
}
