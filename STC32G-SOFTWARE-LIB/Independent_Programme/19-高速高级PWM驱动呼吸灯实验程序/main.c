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

#include	"STC32G_PWM.h"
#include	"STC32G_GPIO.h"
#include	"STC32G_NVIC.h"
#include	"STC32G_Delay.h"
#include	"STC32G_Clock.h"

/*************   ����˵��   ***************

���ٸ߼�PWM��ʱ�� PWM1P/PWM1N,PWM2P/PWM2N,PWM3P/PWM3N,PWM4P/PWM4N ÿ��ͨ�����ɶ���ʵ��PWM������������������Գ����.

8��ͨ��PWM���ö�ӦP6��8���˿�.

ͨ��P6�������ӵ�8��LED�ƣ�����PWMʵ�ֺ�����Ч��.

�߼�PWM��ʱ�� PWM5,PWM6,PWM7,PWM8 ÿ��ͨ�����ɶ���ʵ��PWM���.

4��ͨ��PWM������Ҫ���ö�Ӧ����ڣ���ͨ��ʾ�����۲�������ź�.

PWM���ں�ռ�ձȿ��Ը�����Ҫ�������ã���߿ɴ�65535.

����ʱ, ѡ��ʱ�� 24MHz (�û�����"config.h"�޸�Ƶ��).

******************************************/


/*************	���س�������	**************/


/*************	���ر�������	**************/

PWMx_Duty PWMA_Duty;
bit PWM1_Flag;
bit PWM2_Flag;
bit PWM3_Flag;
bit PWM4_Flag;

PWMx_Duty PWMB_Duty;
bit PWM5_Flag;
bit PWM6_Flag;
bit PWM7_Flag;
bit PWM8_Flag;

/*************	���غ�������	**************/


/*************  �ⲿ�����ͱ������� *****************/


/******************** IO������ ********************/
void GPIO_config(void)
{
	PWM1_USE_P60P61();
	PWM2_USE_P62P63();
	PWM3_USE_P64P65();
	PWM4_USE_P66P67();

	PWM5_USE_P74();
	PWM6_USE_P75();
	PWM7_USE_P76();
	PWM8_USE_P77();
	
	P4_MODE_IO_PU(GPIO_Pin_0);			//P4.0 ����Ϊ׼˫���
	P40 = 0;		//��ʵ����LED��Դ
}

/******************** SPI ���� ********************/
void HSPWM_config(void)
{
	HSPWMx_InitDefine		PWMx_InitStructure;

	PWMx_InitStructure.PWM_EnoSelect= ENO1P|ENO1N|ENO2P|ENO2N|ENO3P|ENO3N|ENO4P|ENO4N;	//���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWMx_InitStructure.PWM_Period   = 2047;							//����ʱ��,   0~65535
	PWMx_InitStructure.PWM_DeadTime = 0;								//��������������, 0~255
	PWMx_InitStructure.PWM_MainOutEnable= ENABLE;				//�����ʹ��, ENABLE,DISABLE
	PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;				//ʹ�ܼ�����, ENABLE,DISABLE
	HSPWM_Configuration(PWMA, &PWMx_InitStructure, &PWMA_Duty);				//��ʼ��PWMͨ�üĴ���,  PWMA,PWMB
	PWMx_InitStructure.PWM_EnoSelect= ENO5P|ENO6P|ENO7P|ENO8P;	//���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	HSPWM_Configuration(PWMB, &PWMx_InitStructure, &PWMB_Duty);				//��ʼ��PWMͨ�üĴ���,  PWMA,PWMB

	HSPllClkConfig(MCLKSEL_HIRC,PLL_96M,0);    //ϵͳʱ��ѡ��,PLLʱ��ѡ��,ʱ�ӷ�Ƶϵ��
	NVIC_PWM_Init(PWMA,DISABLE,Priority_0);
	NVIC_PWM_Init(PWMB,DISABLE,Priority_0);
}

/**********************************************/
void main(void)
{
	WTST = 0;		//���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
	EAXSFR();		//��չSFR(XFR)����ʹ�� 
	CKCON = 0;      //��߷���XRAM�ٶ�

	GPIO_config();
	HSPWM_config();
	EA = 1;

	PWMA_Duty.PWM1_Duty = 128;
	PWMA_Duty.PWM2_Duty = 256;
	PWMA_Duty.PWM3_Duty = 512;
	PWMA_Duty.PWM4_Duty = 1024;

	PWMB_Duty.PWM5_Duty = 128;
	PWMB_Duty.PWM6_Duty = 256;
	PWMB_Duty.PWM7_Duty = 512;
	PWMB_Duty.PWM8_Duty = 1024;
	
	while (1)
	{
		delay_ms(1);
		
		if(!PWM1_Flag)
		{
			PWMA_Duty.PWM1_Duty++;
			if(PWMA_Duty.PWM1_Duty >= 2047) PWM1_Flag = 1;
		}
		else
		{
			PWMA_Duty.PWM1_Duty--;
			if(PWMA_Duty.PWM1_Duty <= 0) PWM1_Flag = 0;
		}

		if(!PWM2_Flag)
		{
			PWMA_Duty.PWM2_Duty++;
			if(PWMA_Duty.PWM2_Duty >= 2047) PWM2_Flag = 1;
		}
		else
		{
			PWMA_Duty.PWM2_Duty--;
			if(PWMA_Duty.PWM2_Duty <= 0) PWM2_Flag = 0;
		}

		if(!PWM3_Flag)
		{
			PWMA_Duty.PWM3_Duty++;
			if(PWMA_Duty.PWM3_Duty >= 2047) PWM3_Flag = 1;
		}
		else
		{
			PWMA_Duty.PWM3_Duty--;
			if(PWMA_Duty.PWM3_Duty <= 0) PWM3_Flag = 0;
		}

		if(!PWM4_Flag)
		{
			PWMA_Duty.PWM4_Duty++;
			if(PWMA_Duty.PWM4_Duty >= 2047) PWM4_Flag = 1;
		}
		else
		{
			PWMA_Duty.PWM4_Duty--;
			if(PWMA_Duty.PWM4_Duty <= 0) PWM4_Flag = 0;
		}
		
		if(!PWM5_Flag)
		{
			PWMB_Duty.PWM5_Duty++;
			if(PWMB_Duty.PWM5_Duty >= 2047) PWM5_Flag = 1;
		}
		else
		{
			PWMB_Duty.PWM5_Duty--;
			if(PWMB_Duty.PWM5_Duty <= 0) PWM5_Flag = 0;
		}

		if(!PWM6_Flag)
		{
			PWMB_Duty.PWM6_Duty++;
			if(PWMB_Duty.PWM6_Duty >= 2047) PWM6_Flag = 1;
		}
		else
		{
			PWMB_Duty.PWM6_Duty--;
			if(PWMB_Duty.PWM6_Duty <= 0) PWM6_Flag = 0;
		}

		if(!PWM7_Flag)
		{
			PWMB_Duty.PWM7_Duty++;
			if(PWMB_Duty.PWM7_Duty >= 2047) PWM7_Flag = 1;
		}
		else
		{
			PWMB_Duty.PWM7_Duty--;
			if(PWMB_Duty.PWM7_Duty <= 0) PWM7_Flag = 0;
		}

		if(!PWM8_Flag)
		{
			PWMB_Duty.PWM8_Duty++;
			if(PWMB_Duty.PWM8_Duty >= 2047) PWM8_Flag = 1;
		}
		else
		{
			PWMB_Duty.PWM8_Duty--;
			if(PWMB_Duty.PWM8_Duty <= 0) PWM8_Flag = 0;
		}
		
		UpdateHSPwm(PWMA, &PWMA_Duty);
		UpdateHSPwm(PWMB, &PWMB_Duty);
	}
}
