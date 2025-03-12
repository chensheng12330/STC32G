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
#include	"STC32G_Timer.h"
#include	"STC32G_GPIO.h"
#include	"STC32G_NVIC.h"

/*************	����˵��	**************

������ʾ5����ʱ����ʹ��, ��ʹ��16λ�Զ���װ.

��ʱ��0��16λ�Զ���װ, �ж�Ƶ��Ϊ100000Hz���жϺ�����P6.7ȡ�����50KHz�����ź�.

��ʱ��1��16λ�Զ���װ, �ж�Ƶ��Ϊ10000Hz���жϺ�����P6.6ȡ�����5KHz�����ź�.

��ʱ��2��16λ�Զ���װ, �ж�Ƶ��Ϊ1000Hz���жϺ�����P6.5ȡ�����500Hz�����ź�.

��ʱ��3��16λ�Զ���װ, �ж�Ƶ��Ϊ100Hz���жϺ�����P6.4ȡ�����50Hz�����ź�.

��ʱ��4��16λ�Զ���װ, �ж�Ƶ��Ϊ50Hz���жϺ�����P6.3ȡ�����25Hz�����ź�.

����ʱ, ѡ��ʱ�� 24MHz (�����������ļ�"config.h"���޸�).

******************************************/

/*************	���س�������	**************/


/*************	���ر�������	**************/


/*************	���غ�������	**************/



/*************  �ⲿ�����ͱ������� *****************/



/************************ IO������ ****************************/
void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;				//�ṹ����

	GPIO_InitStructure.Pin  = GPIO_Pin_HIGH | GPIO_Pin_3;			//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_PullUp;			//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P6,&GPIO_InitStructure);	//��ʼ��

	GPIO_InitStructure.Pin  = GPIO_Pin_0;				//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_PullUp;			//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P4,&GPIO_InitStructure);	//��ʼ��
}

/************************ ��ʱ������ ****************************/
void	Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//�ṹ����
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;		//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = (u16)(65536UL - (MAIN_Fosc / 100000UL));		//��ֵ,
	TIM_InitStructure.TIM_PS        = 0;					//8λԤ��Ƶ��(n+1), 0~255
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//��ʼ��Timer0	  Timer0,Timer1,Timer2,Timer3,Timer4
	NVIC_Timer0_Init(ENABLE,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;		//ָ��ʱ��Դ, TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = (u16)(65536UL - (MAIN_Fosc / 10000));			//��ֵ,
	TIM_InitStructure.TIM_PS        = 0;					//8λԤ��Ƶ��(n+1), 0~255
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer1,&TIM_InitStructure);				//��ʼ��Timer1	  Timer0,Timer1,Timer2,Timer3,Timer4
	NVIC_Timer1_Init(ENABLE,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;		//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = (u16)(65536UL - (MAIN_Fosc / 1000));				//��ֵ
	TIM_InitStructure.TIM_PS        = 0;					//8λԤ��Ƶ��(n+1), 0~255
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer2,&TIM_InitStructure);				//��ʼ��Timer2	  Timer0,Timer1,Timer2,Timer3,Timer4
	NVIC_Timer2_Init(ENABLE,NULL);		//�ж�ʹ��, ENABLE/DISABLE; �����ȼ�

	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;	//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = ENABLE;					//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = (u16)(65536UL - (MAIN_Fosc / (100*12)));		//��ֵ
	TIM_InitStructure.TIM_PS        = 0;					//8λԤ��Ƶ��(n+1), 0~255
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer3,&TIM_InitStructure);				//��ʼ��Timer3	  Timer0,Timer1,Timer2,Timer3,Timer4
	NVIC_Timer3_Init(ENABLE,NULL);		//�ж�ʹ��, ENABLE/DISABLE; �����ȼ�

	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;	//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = ENABLE;					//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = (u16)(65536UL - (MAIN_Fosc / (50*12)));		//��ֵ
	TIM_InitStructure.TIM_PS        = 0;					//8λԤ��Ƶ��(n+1), 0~255
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer4,&TIM_InitStructure);				//��ʼ��Timer4	  Timer0,Timer1,Timer2,Timer3,Timer4
	NVIC_Timer4_Init(ENABLE,NULL);		//�ж�ʹ��, ENABLE/DISABLE; �����ȼ�
}


/******************** ������**************************/
void main(void)
{
	WTST = 0;		//���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
	EAXSFR();		//��չSFR(XFR)����ʹ�� 
	CKCON = 0;      //��߷���XRAM�ٶ�

	GPIO_config();
	Timer_config();
	EA = 1;
	P40 = 0;		//��ʵ����LED��Դ

	while (1);
}



