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
#include	"STC32G_Exti.h"
#include	"STC32G_GPIO.h"
#include	"STC32G_UART.h"
#include	"STC32G_NVIC.h"
#include	"STC32G_Delay.h"
#include	"STC32G_Switch.h"

/*************	����˵��	**************

��ʾINT0~INT4 5������Դ��MCU�����߻���.

�Ӵ����������Դ�����Ѵ�����115200,N,8,1.

����ʱ, ѡ��ʱ�� 22.1184MHz (�û�����"config.h"�޸�Ƶ��).

******************************************/

/*************	���س�������	**************/

sbit INT0 = P3^2;
sbit INT1 = P3^3;
sbit INT2 = P3^6;
sbit INT3 = P3^7;
sbit INT4 = P3^0;

/*************	���ر�������	**************/

u8 WakeUpCnt;

/*************	���غ�������	**************/



/*************  �ⲿ�����ͱ������� *****************/



/******************** IO������ ********************/
void GPIO_config(void)
{
	P3_MODE_IO_PU(GPIO_Pin_All);		//P3.0~P3.7 ����Ϊ׼˫���
	P3_PULL_UP_ENABLE(GPIO_Pin_All);//P3 ���ڲ���������ʹ��
	P4_MODE_IO_PU(GPIO_Pin_6 | GPIO_Pin_7);	//P4.6,P4.7 ����Ϊ׼˫���
}

/******************** INT���� ********************/
void Exti_config(void)
{
	EXTI_InitTypeDef	Exti_InitStructure;							//�ṹ����

	Exti_InitStructure.EXTI_Mode      = EXT_MODE_Fall;//�ж�ģʽ,   EXT_MODE_RiseFall,EXT_MODE_Fall
	Ext_Inilize(EXT_INT0,&Exti_InitStructure);				//��ʼ��
	NVIC_INT0_Init(ENABLE,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

	Exti_InitStructure.EXTI_Mode      = EXT_MODE_Fall;//�ж�ģʽ,   EXT_MODE_RiseFall,EXT_MODE_Fall
	Ext_Inilize(EXT_INT1,&Exti_InitStructure);				//��ʼ��
	NVIC_INT1_Init(ENABLE,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

	NVIC_INT2_Init(ENABLE,NULL);		//�ж�ʹ��, ENABLE/DISABLE; �����ȼ�
	NVIC_INT3_Init(ENABLE,NULL);		//�ж�ʹ��, ENABLE/DISABLE; �����ȼ�
//	NVIC_INT4_Init(ENABLE,NULL);		//�ж�ʹ��, ENABLE/DISABLE; �����ȼ�
}

/****************  ���ڳ�ʼ������ *****************/
void UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,   UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//ѡ�����ʷ�����, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//������,     110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	UART_Configuration(UART1, &COMx_InitStructure);		//��ʼ������ UART1,UART2,UART3,UART4
	NVIC_UART1_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

//	UART2_SW(UART2_SW_P46_P47);		//UART2_SW_P10_P11,UART2_SW_P46_P47
}

/******************** ������***********************/
void main(void)
{
	WTST = 0;		//���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
	EAXSFR();		//��չSFR(XFR)����ʹ�� 
	CKCON = 0;      //��߷���XRAM�ٶ�

	GPIO_config();
	UART_config();
	Exti_config();
	EA  = 1;		//Enable all interrupt

	PrintString1("STC32G EXINT Wakeup Test Programme!\r\n");	//UART����һ���ַ���
	
	while(1)
	{
		while(!INT0);	//�ȴ����ж�Ϊ�ߵ�ƽ
		while(!INT1);	//�ȴ����ж�Ϊ�ߵ�ƽ
		while(!INT2);	//�ȴ����ж�Ϊ�ߵ�ƽ
		while(!INT3);	//�ȴ����ж�Ϊ�ߵ�ƽ
//		while(!INT4);	//�ȴ����ж�Ϊ�ߵ�ƽ
		delay_ms(10);	//delay 10ms

		while(!INT0);	//�ȴ����ж�Ϊ�ߵ�ƽ
		while(!INT1);	//�ȴ����ж�Ϊ�ߵ�ƽ
		while(!INT2);	//�ȴ����ж�Ϊ�ߵ�ƽ
		while(!INT3);	//�ȴ����ж�Ϊ�ߵ�ƽ
//		while(!INT4);	//�ȴ����ж�Ϊ�ߵ�ƽ

		WakeUpSource = 0;
		PrintString1("MCU��������״̬��\r\n");

		PD = 1;		//Sleep
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		
		if(WakeUpSource == 1)	PrintString1("���ж�INT0����  ");
		if(WakeUpSource == 2)	PrintString1("���ж�INT1����  ");
		if(WakeUpSource == 3)	PrintString1("���ж�INT2����  ");
		if(WakeUpSource == 4)	PrintString1("���ж�INT3����  ");
		if(WakeUpSource == 5)	PrintString1("���ж�INT4����  ");
		
		WakeUpCnt++;
		TX1_write2buff((u8)(WakeUpCnt/100+'0'));
		TX1_write2buff((u8)(WakeUpCnt%100/10+'0'));
		TX1_write2buff((u8)(WakeUpCnt%10+'0'));
		PrintString1("�λ���\r\n");
	}

}

