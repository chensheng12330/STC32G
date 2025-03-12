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
/* ������ʹ��IO�ڳ�ʼ���⺯���������û�LAOXU�ṩ������Ȥ���û��ɲο�ʹ��   */
/* ������Դ��https://www.stcaimcu.com/forum.php?mod=viewthread&tid=818 */
/*---------------------------------------------------------------------*/

#include <STC32G.H> 
#include <STC_GPIO.H> 
#include "stdio.h"

#define MAIN_Fosc     22118400L   //������ʱ��
#define Baudrate      115200L
#define TM            (65536 -(MAIN_Fosc/Baudrate/4))

/******************** ���ڴ�ӡ���� ********************/
void UartInit(void)
{
	SCON = (SCON & 0x3f) | 0x40; 
	T2L  = TM;
	T2H  = TM>>8;
	AUXR |= 0x15;
}

void UartPutc(unsigned char dat)
{
	SBUF = dat; 
	while(TI==0);
	TI = 0;
}

char putchar(char c)
{
	UartPutc(c);
	return c;
}

void main(void)
{  	
//			    ===========�� �� ʵ ��===========
    UartInit();
//                               ------Cortex-M051���1------

//1.void GPIO_SET_MODE(Pn, b7,b6,b5,b4,b3,b2,b1,b0);          // ����IO���������ģʽ(n=0-7)    

//  ʹ�÷�ʽ��
    GPIO_SET_MODE(P1, GPIO_PullUp,GPIO_HighZ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_PP,GPIO_OUT_OD,GPIO_OUT_PP,GPIO_OUT_OD);
                 // ����P1�ڵ�bit.7-bit.0λ������ΪPullUp,HighZ,PullUp,HighZ,OUT_PP,OUT_OD,OUT_PP,OUT_ODģʽ 
	//  P1M1 = 0x55;  // Bin(0101,0101);  0: PullUp, 0: OUT_PP, 1: HighZ; 1: OUT_OD
	//  P1M0 = 0x0f;  // Bin(0000,1111);  0:         1:         0:        1:
    printf("1.P1M1=0x%02x,P1M0=0x%02x\r\n",P1M1,P1M0);

//2.void GPIO_SET_PIN_MODE(Port, Pin_Mode);                 // ����IO������1λ����λ�������ģʽ(N=0-7,i=0-7)  

//  ���磺
    GPIO_SET_PIN_MODE(P1, GPIO_OUT_PP_6);        // ����P1�ڵĵ�bit.6λΪOUT_PPģʽ
		//  P1M0 |= 0x40; P1M1 &= ~0x40; 
		 // P1M0 = 0x4F; P1M1 = 0x15; 
	//  P1M1 = 0x15;  // Bin(0001,0101);  0: PullUp, 0: OUT_PP, 1: HighZ; 1: OUT_OD
	//  P1M0 = 0x4f;  // Bin(0100,1111);  0:         1:         0:        1:
    GPIO_SET_PIN_MODE(P2, GPIO_PullUp_7 | GPIO_OUT_PP_2 | GPIO_HighZ_0);        // ����P2�ڵĵ�bit.5λΪPullUpģʽ,��bit.2λΪOUT_PPģʽ,��bit.0λΪHighZģʽ 
		// P2M0 = (P2M0 & ~0x81) | 0x04; P2M1 = (P2M1 & ~0x84) | 0x01; 
	//  P2M1 = 0x7B;  // Bin(0111,1011);  0: PullUp, 0: OUT_PP, 1: HighZ; 1: OUT_OD
	//  P2M0 = 0x04;  // Bin(0000,0100);  0:         1:         0:        1:
    printf("2.P1M1=0x%02x,P1M0=0x%02x\r\n",P1M1,P1M0);
    printf("2.P2M1=0x%02x,P2M0=0x%02x\r\n",P2M1,P2M0);

//                              ------Cortex-M051���2------

//3.void Pn_SET_MODE(b7,b6,b5,b4,b3,b2,b1,b0);          // ����Pn IO���������ģʽ(n=0-4)    

//  ʹ�÷�ʽ��
    P1_SET_MODE(GPIO_OUT_PP,GPIO_PullUp,GPIO_OUT_OD,GPIO_HighZ,GPIO_OUT_PP,GPIO_OUT_OD,GPIO_PullUp,GPIO_PullUp);
                 // ����P1�ڵ�bit.7-bit.0λ������ΪPullUp,HighZ,PullUp,HighZ,OUT_PP,OUT_OD,PullUp,PullUpģʽ 
	//  P1M1 = 0x34;  // Bin(0011,0100);  0: PullUp, 0: OUT_PP, 1: HighZ; 1: OUT_OD
	//  P1M0 = 0xAC;  // Bin(1010,1100);  0:         1:         0:        1:
    printf("3.P1M1=0x%02x,P1M0=0x%02x\r\n",P1M1,P1M0);


//4.void Pn_SET_PIN_MODE(Pin_Mode);                 // ����Pn IO������1λ����λ�������ģʽ(N=0-7,i=0-7)  

//  ���磺
    P1_SET_PIN_MODE(GPIO_OUT_OD_5);        // ����P1�ڵĵ�bit.5λΪOUT_ODģʽ
		 // P1M0 |= 0x20; P1M1 &= ~0x20; 
	//  P1M1 = 0x34;  // Bin(0011,0100);  0: PullUp, 0: OUT_PP, 1: HighZ; 1: OUT_OD
	//  P1M0 = 0xAC;  // Bin(1010,1100);  0:         1:         0:        1:
    P2_SET_PIN_MODE(GPIO_PullUp_7 | GPIO_OUT_PP_2 | GPIO_HighZ_4);        // ����P2�ڵĵ�bit.5λΪPullUpģʽ,��bit.2λΪOUT_PPģʽ,��bit.4λΪHighZģʽ 
     // P2M0 = (P2M0 & ~0x90) | 0x04; P2M1 = (P2M1 & ~0x84) | 0x10; 
	//  P2M1 = 0x7B;  // Bin(0111,1011);  0: PullUp, 0: OUT_PP, 1: HighZ; 1: OUT_OD
	//  P2M0 = 0x04;  // Bin(0000,0100);  0:         1:         0:        1:
    printf("4.P1M1=0x%02x,P1M0=0x%02x\r\n",P1M1,P1M0);
    printf("4.P2M1=0x%02x,P2M0=0x%02x\r\n",P2M1,P2M0);

//                               -------51ϵ�з��-------

//5.u8 GPIO_Mode(b7,b6,b5,b4,b3,b2,b1,b0);  ����IO���������ģʽ  

//  ʹ�÷�ʽ��
//  PnMode = GPIO_Mode(b7,b6,b5,b4,b3,b2,b1,b0);  ����IO���������ģʽ 

    P1Mode = GPIO_Mode(GPIO_PullUp,GPIO_HighZ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_PP,GPIO_OUT_OD,GPIO_OUT_PP,GPIO_OUT_OD);
                 // ����P1�ڵ�bit.7-bit.0λ������ΪPullUp,HighZ,PullUp,HighZ,OUT_PP,OUT_OD,OUT_PP,OUT_ODģʽ 
	//  P1M1 = 0x55;  // Bin(0101,0101);  0: PullUp, 0: OUT_PP, 1: HighZ; 1: OUT_OD
	//  P1M0 = 0x0f;  // Bin(0000,1111);  0:         1:         0:        1:
    printf("5.P1M1=0x%02x,P1M0=0x%02x\r\n",P1M1,P1M0);

							 
    while(1);
}