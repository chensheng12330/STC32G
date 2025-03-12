/*---------------------------------------------------------------------*/
/* --- STC AI Limited -------------------------------------------------*/
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

#include	"stc32g.h"
//#include	"stc32g_pwma.h"
#include	"stc32g_pwmb.h"

/*************	����˵��	**************

�����̻���STC32G12K128Ϊ����оƬ��ʵ������б�д����.

�߼�PWM��ʱ�� PWM5,PWM6,PWM7,PWM8 ÿ��ͨ�����ɶ���ʵ��PWM���.

4��ͨ��PWM������Ҫ���ö�Ӧ����ڣ���ͨ��ʾ�����۲�������ź�.

PWM���ں�ռ�ձȿ��Ը�����Ҫ�������ã���߿ɴ�65535.

******************************************/

/*************	���س�������	**************/

#define CCR1_Val  ((uint16_t)1024)
#define CCR2_Val  ((uint16_t)512)
#define CCR3_Val  ((uint16_t)256)
#define CCR4_Val  ((uint16_t)128)

/*************	���ر�������	**************/


/*************	���غ�������	**************/



/*************  �ⲿ�����ͱ������� *****************/



/***************  PWM��ʼ������ *****************/
void PWM_config(void)
{
    PWMB_DeInit();

    /* PWM Base configuration */
    /*
    PWMB_Period = 4095
    PWMB_Prescaler = 0
    PWMB_CounterMode = PWMB_COUNTERMODE_UP
    PWMB_RepetitionCounter = 0
    */

    PWMB_ENO   = ENO5P | ENO6P | ENO7P | ENO8P;	//���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWMB_PS      = PWM5_SW_P20| PWM6_SW_P21 | PWM7_SW_P22 | PWM8_SW_P23;	//�л��˿�,		PWM5_SW_P20,PWM5_SW_P17,PWM5_SW_P00,PWM5_SW_P74

    PWMB_TimeBaseInit(0, PWMB_COUNTERMODE_UP, 2047, 0);



    /* Channel 1, 2,3 and 4 Configuration in PWM mode */

    /*
    PWMB_OCMode = PWMB_OCMODE_PWM2
    PWMB_OutputState = PWMB_OUTPUTSTATE_ENABLE
    PWMB_OutputNState = PWMB_OUTPUTNSTATE_ENABLE
    PWMB_Pulse = CCR1_Val
    PWMB_OCPolarity = PWMB_OCPOLARITY_LOW
    PWMB_OCNPolarity = PWMB_OCNPOLARITY_HIGH
    PWMB_OCIdleState = PWMB_OCIDLESTATE_SET
    PWMB_OCNIdleState = PWMB_OCIDLESTATE_RESET

    */
    PWMB_OC5Init(PWMB_OCMODE_PWM2, PWMB_OUTPUTSTATE_ENABLE, CCR1_Val, PWMB_OCPOLARITY_LOW,
               PWMB_OCIDLESTATE_SET); 

    /*PWMB_Pulse = CCR2_Val*/
    PWMB_OC6Init(PWMB_OCMODE_PWM2, PWMB_OUTPUTSTATE_ENABLE, CCR2_Val, PWMB_OCPOLARITY_LOW,
               PWMB_OCIDLESTATE_SET);

    /*PWMB_Pulse = CCR3_Val*/
    PWMB_OC7Init(PWMB_OCMODE_PWM2, PWMB_OUTPUTSTATE_ENABLE, CCR3_Val, PWMB_OCPOLARITY_LOW,
               PWMB_OCIDLESTATE_SET);

    /*PWMB_Pulse = CCR4_Val*/
    PWMB_OC8Init(PWMB_OCMODE_PWM2, PWMB_OUTPUTSTATE_ENABLE, CCR4_Val, PWMB_OCPOLARITY_LOW,
               PWMB_OCIDLESTATE_SET);

    /* TIM1 counter enable */
    PWMB_Cmd(ENABLE);

    /* TIM1 Main Output Enable */
    PWMB_CtrlPWMOutputs(ENABLE);
}

/******************** ������**************************/
void main(void)
{
    EAXSFR();       //��չ�Ĵ�������ʹ��
	PWM_config();

	while (1);
}



