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
#include	"STC32G_ADC.h"
#include	"STC32G_UART.h"
#include	"STC32G_NVIC.h"
#include	"STC32G_Delay.h"
#include	"STC32G_Switch.h"

/*************	����˵��	**************

������ʾ��·ADC��ѯ������ͨ������2(P4.6,P4.7)���͸���λ����������115200,N,8,1��

˵��:
    ADC�ĵ�15ͨ�������������ڲ�BandGap�ο���ѹ��,�����ڲ�BandGap�ο���
ѹ���ȶ�,������оƬ�Ĺ�����ѹ�ĸı���仯,���Կ���ͨ�������ڲ�BandGap
�ο���ѹ,Ȼ��ͨ��ADC��ֵ��ɷ��Ƴ�VCC�ĵ�ѹ,�Ӷ��û�����ʵ���Լ��ĵ�
ѹ��⹦��.
    ADC�ĵ�15ͨ���Ĳ�������:ͨ��ADC�ĵ�15ͨ����ȡ��ǰ�ڲ�BandGap�ο���ѹֵ.
    �û�ʵ���Լ��ĵ�ѹ��⹦�ܵ�ʵ�ַ���:�����û���Ҫ��VCC�ܾ�׼�������
(����5.0V),�������ڲ�BandGap�ο���ѹ��ADCת��ֵ(����ΪBGV5),�����ֵ����
��EEPROM��,Ȼ���ڵ�ѹ���Ĵ�����,��ʵ��VCC�仯��,�����������ڲ�BandGap
�ο���ѹ��ADCת��ֵ(����ΪBGVx),ͨ�����㹫ʽ: ʵ��VCC = 5.0V * BGV5 / BGVx,
���ɼ����ʵ�ʵ�VCC��ѹֵ,��Ҫע�����,��һ����BGV5�Ļ�׼����һ��Ҫ��ȷ.

�궨�ڲ���׼Vref, �ṩһ���ȶ��Ĺ�����ѹVcc, ���ڲ���׼Nref. 
�����ڲ���׼(10λAD) Vref = Nref * Vcc / 1024.
�����ڲ���׼(12λAD) Vref = Nref * Vcc / 4096.

����ĳ����ѹ, ��ADCֵNx, ���ѹ Ux = Vref * Nx / Nref. һ��Vref = 1190mV.

����ʱ, ѡ��ʱ�� 22.1184MHz (�����������ļ�"config.h"���޸�).

******************************************/

/*************	���س�������	**************/


/*************	���ر�������	**************/

u16	Nref;

/*************	���غ�������	**************/


/*************  �ⲿ�����ͱ������� *****************/


/******************* IO���ú��� *******************/
void	GPIO_config(void)
{
	P1_MODE_IN_HIZ(GPIO_Pin_LOW);		//P1.0~P1.4 ����Ϊ��������
	P4_MODE_IO_PU(GPIO_Pin_6 | GPIO_Pin_7);	//P4.6,P4.7 ����Ϊ׼˫���
}

/******************* AD���ú��� *******************/
void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;		//�ṹ����
	ADC_InitStructure.ADC_SMPduty   = 31;		//ADC ģ���źŲ���ʱ�����, 0~31��ע�⣺ SMPDUTY һ����������С�� 10��
	ADC_InitStructure.ADC_CsSetup   = 0;		//ADC ͨ��ѡ��ʱ����� 0(Ĭ��),1
	ADC_InitStructure.ADC_CsHold    = 1;		//ADC ͨ��ѡ�񱣳�ʱ����� 0,1(Ĭ��),2,3
	ADC_InitStructure.ADC_Speed     = ADC_SPEED_2X16T;		//���� ADC ����ʱ��Ƶ��	ADC_SPEED_2X1T~ADC_SPEED_2X16T
	ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED;	//ADC�������,	ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
	ADC_Inilize(&ADC_InitStructure);		//��ʼ��
	ADC_PowerControl(ENABLE);						//ADC��Դ����, ENABLE��DISABLE
	NVIC_ADC_Init(DISABLE,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}

/***************  ���ڳ�ʼ������ *****************/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,   UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//ѡ�����ʷ�����, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//������,     110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	UART_Configuration(UART2, &COMx_InitStructure);		//��ʼ������ UART1,UART2,UART3,UART4
	NVIC_UART2_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

	UART2_SW(UART2_SW_P46_P47);		//UART2_SW_P10_P11,UART2_SW_P46_P47
}

/**********************************************/
void main(void)
{
	u8	i;
	u16	j;

	WTST = 0;		//���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
	EAXSFR();		//��չSFR(XFR)����ʹ�� 
	CKCON = 0;      //��߷���XRAM�ٶ�

	GPIO_config();
	UART_config();
	ADC_config();
	EA = 1;

	printf("STC32G AD to UART Test Programme!\r\n");	//UART����һ���ַ���
	j = Get_ADCResult(15);	//���ڲ���׼��ѹ, ��15ͨ��
	Nref = j;

	while (1)
	{
		for(i=0; i<4; i++)
		{
			delay_ms(250);		//Ϊ���÷��͵��ٶ���һ�㣬��ʱ250ms

			if(i < 3)	//ADC0~ADC2
			{
				j = Get_ADCResult(i);	//����0~14,��ѯ��ʽ��һ��ADC, ����ֵ���ǽ��, == 4096 Ϊ����
				printf("AD%d=%04d  ",i,j);
			}
			else		//�ڻ�׼
			{
				j = Get_ADCResult(15);	//���ڲ���׼��ѹ, ��15ͨ��
				Nref = j;
				printf("ADref=%04d  ",j);
			}
			printf("V=%04.3f  ",((float)1.190 * j) / Nref);
		}
		printf("\r\n");
	}
}



