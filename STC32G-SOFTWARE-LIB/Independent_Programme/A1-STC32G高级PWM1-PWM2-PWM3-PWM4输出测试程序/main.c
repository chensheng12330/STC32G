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
#include	"stc32g_pwma.h"
//#include	"stc32g_pwmb.h"

/*************	����˵��	**************

�����̻���STC32G12K128Ϊ����оƬ��ʵ������б�д����.

�߼�PWM��ʱ�� PWM1P/PWM1N,PWM2P/PWM2N,PWM3P/PWM3N,PWM4P/PWM4N ÿ��ͨ�����ɶ���ʵ��PWM������������������Գ����.

8��ͨ��PWM���ö�ӦP6��8���˿�.

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
    PWMA_DeInit();

    /* PWM Base configuration */
    /*
    PWMA_Period = 4095
    PWMA_Prescaler = 0
    PWMA_CounterMode = PWMA_COUNTERMODE_UP
    PWMA_RepetitionCounter = 0
    */
    PWMA_ENO = ENO1P | ENO1N | ENO2P | ENO2N | ENO3P | ENO3N | ENO4P | ENO4N;		
    PWMA_PS = PWM1_SW_P60_P61| PWM2_SW_P62_P63 | PWM3_SW_P64_P65 | PWM4_SW_P66_P67;	//�л��˿�,		PWM1_SW_P10_P11,PWM1_SW_P20_P21,PWM1_SW_P60_P61
    PWMA_TimeBaseInit(0, PWMA_COUNTERMODE_UP, 2047, 0);


    /* Channel 1, 2,3 and 4 Configuration in PWM mode */

    /*
    PWMA_OCMode = PWMA_OCMODE_PWM2
    PWMA_OutputState = PWMA_OUTPUTSTATE_ENABLE
    PWMA_OutputNState = PWMA_OUTPUTNSTATE_ENABLE
    PWMA_Pulse = CCR1_Val
    PWMA_OCPolarity = PWMA_OCPOLARITY_LOW
    PWMA_OCNPolarity = PWMA_OCNPOLARITY_HIGH
    PWMA_OCIdleState = PWMA_OCIDLESTATE_SET
    PWMA_OCNIdleState = PWMA_OCIDLESTATE_RESET

    */
    PWMA_OC1Init(PWMA_OCMODE_PWM2, PWMA_OUTPUTSTATE_ENABLE, PWMA_OUTPUTNSTATE_ENABLE,
               CCR1_Val, PWMA_OCPOLARITY_LOW, PWMA_OCNPOLARITY_HIGH, PWMA_OCIDLESTATE_SET,
               PWMA_OCNIDLESTATE_RESET); 

    /*PWMA_Pulse = CCR2_Val*/
    PWMA_OC2Init(PWMA_OCMODE_PWM2, PWMA_OUTPUTSTATE_ENABLE, PWMA_OUTPUTNSTATE_ENABLE, CCR2_Val,
               PWMA_OCPOLARITY_LOW, PWMA_OCNPOLARITY_HIGH, PWMA_OCIDLESTATE_SET, 
               PWMA_OCNIDLESTATE_RESET);

    /*PWMA_Pulse = CCR3_Val*/
    PWMA_OC3Init(PWMA_OCMODE_PWM2, PWMA_OUTPUTSTATE_ENABLE, PWMA_OUTPUTNSTATE_ENABLE,
               CCR3_Val, PWMA_OCPOLARITY_LOW, PWMA_OCNPOLARITY_HIGH, PWMA_OCIDLESTATE_SET,
               PWMA_OCNIDLESTATE_RESET);

    /*PWMA_Pulse = CCR4_Val*/
    PWMA_OC4Init(PWMA_OCMODE_PWM2, PWMA_OUTPUTSTATE_ENABLE, PWMA_OUTPUTNSTATE_ENABLE,
               CCR4_Val, PWMA_OCPOLARITY_LOW, PWMA_OCNPOLARITY_HIGH, PWMA_OCIDLESTATE_SET,
               PWMA_OCNIDLESTATE_RESET);

    /* PWMA counter enable */
    PWMA_Cmd(ENABLE);

    /* PWMA Main Output Enable */
    PWMA_CtrlPWMOutputs(ENABLE);
}

/******************** ������**************************/
void main(void)
{
    EAXSFR();       //��չ�Ĵ�������ʹ��
	PWM_config();

	while (1);
}



