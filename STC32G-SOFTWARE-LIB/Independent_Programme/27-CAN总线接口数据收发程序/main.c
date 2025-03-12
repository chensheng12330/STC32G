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

#include	"STC32G_CAN.h"
#include	"STC32G_GPIO.h"
#include	"STC32G_NVIC.h"
#include	"STC32G_UART.h"
#include	"STC32G_Timer.h"
#include	"STC32G_Switch.h"

/*************   ����˵��   ***************

CAN1��CAN2����ͬʱʹ���շ���������.

DCAN��һ��֧��CAN2.0BЭ��Ĺ��ܵ�Ԫ��

�յ�һ֡���ݺ�, ͨ������1��P3.0,P3.1�����ͳ�ȥ�����ڲ�����115200,N,8,1.

��Ҫ��"STC32G_UART.h"�����ã� #define	PRINTF_SELECT  UART1

MCUÿ���Ӵ�CAN1��CAN2����һ֡�̶����ݣ�ÿ��һ֡ID�ż�1.

CAN2���õ��˲�ģʽ��ֻ����IDΪ 0x280~0x2ff �ı�׼֡����

Ĭ��CAN���߲�����500KHz, �û��������޸�.

����ʱ, ѡ��ʱ�� 24MHz (�û�����"config.h"�޸�Ƶ��).

******************************************/

/*************	���س�������	**************/


/*************	���ر�������	**************/

u16 msecond;

/*************	���غ�������	**************/


/*************  �ⲿ�����ͱ������� *****************/


/******************** IO������ ********************/
void GPIO_config(void)
{
	P3_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1);		//P3.0,P3.1 ����Ϊ׼˫���
	P5_MODE_IO_PU(GPIO_Pin_LOW);		//P5.0~P5.3 ����Ϊ׼˫���

	CAN1_SW(CAN1_P50_P51);				//CAN1_P00_P01,CAN1_P50_P51,CAN1_P42_P45,CAN1_P70_P71
	CAN2_SW(CAN2_P52_P53);				//CAN2_P02_P03,CAN2_P52_P53,CAN2_P46_P47,CAN2_P72_P73
}

/************************ ��ʱ������ ****************************/
void Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;						//�ṹ����
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;		//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = (u16)(65536UL - (MAIN_Fosc / 1000UL));		//��ֵ, 1�����ж�1000��
	TIM_InitStructure.TIM_PS        = 0;					//8λԤ��Ƶ��(n+1), 0~255
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//��ʼ��Timer0	  Timer0,Timer1,Timer2,Timer3,Timer4
	NVIC_Timer0_Init(ENABLE,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}

/***************  ���ڳ�ʼ������ *****************/
void UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����

	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,   UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//ѡ�����ʷ�����, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2, ���Բ���ѡ��)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//������,     110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	UART_Configuration(UART1, &COMx_InitStructure);		//��ʼ������ USART1,USART2,USART3,USART4
	NVIC_UART1_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

	UART1_SW(UART1_SW_P30_P31);		//UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

/******************** CAN ���� ********************/
void CAN_config(void)
{
    CAN_InitTypeDef	CAN_InitStructure;			//�ṹ����

    CAN_InitStructure.CAN_Enable = ENABLE;		//CAN����ʹ��   ENABLE��DISABLE
    CAN_InitStructure.CAN_IMR    = CAN_ALLIM;	//CAN�жϼĴ��� 	CAN_DOIM,CAN_BEIM,CAN_TIM,CAN_RIM,CAN_EPIM,CAN_EWIM,CAN_ALIM,CAN_ALLIM,DISABLE
    CAN_InitStructure.CAN_SJW    = 0;			//����ͬ����Ծ���  0~3
    CAN_InitStructure.CAN_SAM    = 0;			//���ߵ�ƽ��������  0:����1��; 1:����3��

    //CAN���߲�����=Fclk/((1+(TSG1+1)+(TSG2+1))*(BRP+1)*2)
    CAN_InitStructure.CAN_TSG1   = 2;			//ͬ��������1       0~15
    CAN_InitStructure.CAN_TSG2   = 1;			//ͬ��������2       1~7 (TSG2 ��������Ϊ0)
    CAN_InitStructure.CAN_BRP    = 3;			//�����ʷ�Ƶϵ��    0~63
    //24000000/((1+3+2)*4*2)=500KHz

    CAN_InitStructure.CAN_ListenOnly = DISABLE;	//Listen Onlyģʽ   ENABLE,DISABLE
    CAN_InitStructure.CAN_Filter  = SINGLE_FILTER;//�˲�ѡ��  DUAL_FILTER(˫�˲�),SINGLE_FILTER(���˲�)
    CAN_InitStructure.CAN_ACR0    = 0x00;		//�������մ���Ĵ��� 0~0xFF
    CAN_InitStructure.CAN_ACR1    = 0x00;
    CAN_InitStructure.CAN_ACR2    = 0x00;
    CAN_InitStructure.CAN_ACR3    = 0x00;
    CAN_InitStructure.CAN_AMR0    = 0xff;		//�����������μĴ��� 0~0xFF
    CAN_InitStructure.CAN_AMR1    = 0xff;
    CAN_InitStructure.CAN_AMR2    = 0xff;
    CAN_InitStructure.CAN_AMR3    = 0xff;
    CAN_Inilize(CAN1,&CAN_InitStructure);		//CAN1 ��ʼ��

    //ֻ���� ID Ϊ 0x280 ~ 0x2ff �ı�׼֡����
    CAN_InitStructure.CAN_ACR0    = 0x50;		//�������մ���Ĵ��� 0~0xFF
    CAN_InitStructure.CAN_ACR1    = 0x00;
    CAN_InitStructure.CAN_ACR2    = 0x00;
    CAN_InitStructure.CAN_ACR3    = 0x00;
    CAN_InitStructure.CAN_AMR0    = 0x0f;		//�����������μĴ��� 0~0xFF
    CAN_InitStructure.CAN_AMR1    = 0xef;
    CAN_InitStructure.CAN_AMR2    = 0xff;
    CAN_InitStructure.CAN_AMR3    = 0xff;
    CAN_Inilize(CAN2,&CAN_InitStructure);		//CAN2 ��ʼ��

    NVIC_CAN_Init(CAN1,ENABLE,Priority_1);		//�ж�ʹ��, CAN1/CAN2; ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
    NVIC_CAN_Init(CAN2,ENABLE,Priority_1);		//�ж�ʹ��, CAN1/CAN2; ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}

/**********************************************/
void main(void)
{
	u8 n,i,j;
	u8 sr;

	WTST = 0;		//���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
	EAXSFR();		//��չSFR(XFR)����ʹ�� 
	CKCON = 0;      //��߷���XRAM�ٶ�

	GPIO_config();
    Timer_config();
    UART_config();
	CAN_config();
	EA = 1;
	//====��ʼ������=====
	CAN1_Tx.FF = STANDARD_FRAME;    //��׼֡
	CAN1_Tx.RTR = 0;                //0������֡��1��Զ��֡
	CAN1_Tx.DLC = 0x08;             //���ݳ���
//	CAN1_Tx.ID = 0x02f8;            //CAN ID
	CAN1_Tx.ID = 0x0278;            //CAN ID
	CAN1_Tx.DataBuffer[0] = 0x11;   //��������
	CAN1_Tx.DataBuffer[1] = 0x12;
	CAN1_Tx.DataBuffer[2] = 0x13;
	CAN1_Tx.DataBuffer[3] = 0x14;
	CAN1_Tx.DataBuffer[4] = 0x15;
	CAN1_Tx.DataBuffer[5] = 0x16;
	CAN1_Tx.DataBuffer[6] = 0x17;
	CAN1_Tx.DataBuffer[7] = 0x18;
	
	CAN2_Tx.FF = EXTENDED_FRAME;    //��չ֡
	CAN2_Tx.RTR = 0;                //0������֡��1��Զ��֡
	CAN2_Tx.DLC = 0x08;             //���ݳ���
	CAN2_Tx.ID = 0x03456789;        //CAN ID
	CAN2_Tx.DataBuffer[0] = 0x21;   //��������
	CAN2_Tx.DataBuffer[1] = 0x22;
	CAN2_Tx.DataBuffer[2] = 0x23;
	CAN2_Tx.DataBuffer[3] = 0x24;
	CAN2_Tx.DataBuffer[4] = 0x25;
	CAN2_Tx.DataBuffer[5] = 0x26;
	CAN2_Tx.DataBuffer[6] = 0x27;
	CAN2_Tx.DataBuffer[7] = 0x28;

	while (1)
	{
        if(T0_1ms)
        {
            T0_1ms = 0;
            //------------------����CAN1ģ��-----------------------
            if(++msecond >= 1000)   //1�뵽
            {
                msecond = 0;

                CANSEL = CAN1;		//ѡ��CAN1ģ��
                sr = CanReadReg(SR);

                if(sr & 0x01)		//�ж��Ƿ��� BS:BUS-OFF״̬
                {
                    CANAR = MR;
                    CANDR &= ~0x04;  //��� Reset Mode, ��BUS-OFF״̬�˳�
                }
                else
                {
                    CanSendMsg(&CAN1_Tx);   //����һ֡����
                    CAN1_Tx.ID += 0x01;        //CAN ID
                }

                //------------------����CAN2ģ��-----------------------
                CANSEL = CAN2;		//ѡ��CAN2ģ��
                sr = CanReadReg(SR);

                if(sr & 0x01)		//�ж��Ƿ��� BS:BUS-OFF״̬
                {
                    CANAR = MR;
                    CANDR &= ~0x04;  //��� Reset Mode, ��BUS-OFF״̬�˳�
                }
                else
                {
                    CanSendMsg(&CAN2_Tx);   //����һ֡����
                    CAN2_Tx.ID += 0x0001;        //CAN ID
                }
            }
        }

		if(B_Can1Read)      //�ж� CAN1 �Ƿ���յ�����
		{
			B_Can1Read = 0;
			
			CANSEL = CAN1;		//ѡ��CAN1ģ��
            n = CanReadMsg(CAN1_Rx);    //��ȡ��������
            if(n>0)
            {
                for(i=0;i<n;i++)
                {
//                    CanSendMsg(&CAN1_Rx[i]);  //CAN����ԭ������
                    printf("CAN1 ID=0x%08lX DLC=%d FF=%d RTR=%d ",CAN1_Rx[i].ID,CAN1_Rx[i].DLC,CAN1_Rx[i].FF,CAN1_Rx[i].RTR);   //���ڴ�ӡ֡��Ϣ
                    for(j=0;j<CAN1_Rx[i].DLC;j++)
                    {
                        printf("0x%02X ",CAN1_Rx[i].DataBuffer[j]);    //�Ӵ�������յ�������
                    }
                    printf("\r\n");
                }
            }
		}

		if(B_Can2Read)      //�ж� CAN2 �Ƿ���յ�����
		{
			B_Can2Read = 0;
			
			CANSEL = CAN2;		//ѡ��CAN2ģ��
            n = CanReadMsg(CAN2_Rx);    //��ȡ��������
            if(n>0)
            {
                for(i=0;i<n;i++)
                {
//                    CanSendMsg(&CAN2_Rx[i]);  //CAN����ԭ������
                    printf("CAN2 ID=0x%08lX DLC=%d FF=%d RTR=%d ",CAN2_Rx[i].ID,CAN2_Rx[i].DLC,CAN2_Rx[i].FF,CAN2_Rx[i].RTR);   //���ڴ�ӡ֡��Ϣ
                    for(j=0;j<CAN2_Rx[i].DLC;j++)
                    {
                        printf("0x%02X ",CAN2_Rx[i].DataBuffer[j]);    //�Ӵ�������յ�������
                    }
                    printf("\r\n");
                }
            }
		}
	}
}
