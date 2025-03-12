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
#include	"STC32G_RTC.h"
#include	"STC32G_GPIO.h"
#include	"STC32G_UART.h"
#include	"STC32G_NVIC.h"
#include	"STC32G_Timer.h"
#include	"STC32G_Switch.h"

/*************	����˵��	**************

��доƬ�ڲ����ɵ�RTCģ��.

��STC��MCU��IO��ʽ����8λ����ܡ�

ʹ��Timer0��16λ�Զ���װ������1ms����,�������������������, �û��޸�MCU��ʱ��Ƶ��ʱ,�Զ���ʱ��1ms.

8λ�������ʾʱ��(Сʱ-����-��).

ͨ������1��P3.0,P3.1����ӡʱ��(��-��-��-ʱ-��-��).

����ɨ�谴������Ϊ25~32.

����ֻ֧�ֵ�������, ��֧�ֶ��ͬʱ����, ���������в���Ԥ֪�Ľ��.

�����³���1���,����10��/����ٶ��ṩ�ؼ����. �û�ֻ��Ҫ���KeyCode�Ƿ��0���жϼ��Ƿ���.

����ʱ���:
����25: Сʱ+.
����26: Сʱ-.
����27: ����+.
����28: ����-.

����ʱ, ѡ��ʱ�� 24MHz (�����������ļ�"config.h"���޸�).

******************************************/

/*************	���س�������	**************/

#define DIS_DOT     0x20
#define DIS_BLACK   0x10
#define DIS_        0x11

u8 code t_display[]={                       //��׼�ֿ�
//   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
//black  -     H    J    K    L    N    o   P    U     t    G    Q    r   M    y
    0x00,0x40,0x76,0x1E,0x70,0x38,0x37,0x5C,0x73,0x3E,0x78,0x3d,0x67,0x50,0x37,0x6e,
    0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0x46};    //0. 1. 2. 3. 4. 5. 6. 7. 8. 9. -1

u8 code T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};      //λ��

#define SleepModeSet  0        //0:��������ģʽ��ʹ���������ʾʱ���ܽ�����; 1:ʹ������ģʽ

/*************	���ر�������	**************/

u8  LED8[8];        //��ʾ����
u8  display_index;  //��ʾλ����

u8  IO_KeyState, IO_KeyState1, IO_KeyHoldCnt;    //���м��̱���
u8  KeyHoldCnt; //�����¼�ʱ
u8  KeyCode;    //���û�ʹ�õļ���
u8  cnt50ms;

u8  hour,minute,second; //RTC����
u16 msecond;

/*************	���غ�������	**************/

void IO_KeyScan(void);   //50ms call
void WriteRTC(void);
void DisplayRTC(void);
void DisplayScan(void);

/*************  �ⲿ�����ͱ������� *****************/

extern bit B_1S;
extern bit B_Alarm;

/******************** IO������ ********************/
void	GPIO_config(void)
{
	P0_MODE_IO_PU(GPIO_Pin_All);		//P0 ����Ϊ׼˫���
	P3_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1);	//P3.0,P3.1 ����Ϊ׼˫���
	P6_MODE_IO_PU(GPIO_Pin_All);		//P6 ����Ϊ׼˫���
	P7_MODE_IO_PU(GPIO_Pin_All);		//P7 ����Ϊ׼˫���
}

/************************ ��ʱ������ ****************************/
void	Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;						//�ṹ����
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;		//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = (u16)(65536UL - (MAIN_Fosc / 1000UL));		//�ж�Ƶ��, 1000��/��
	TIM_InitStructure.TIM_PS        = 0;					//8λԤ��Ƶ��(n+1), 0~255
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//��ʼ��Timer0	  Timer0,Timer1,Timer2,Timer3,Timer4
	NVIC_Timer0_Init(ENABLE,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}

/***************  ���ڳ�ʼ������ *****************/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����

	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,   UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//ѡ�����ʷ�����, BRT_Timer1,BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//������,     110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	UART_Configuration(UART1, &COMx_InitStructure);		//��ʼ������2 USART1,USART2,USART3,USART4
	NVIC_UART1_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

	UART1_SW(UART1_SW_P30_P31);		//UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

/****************  RTC��ʼ������ *****************/
void	RTC_config(void)
{
	RTC_InitTypeDef		RTC_InitStructure;
	RTC_InitStructure.RTC_Clock  = RTC_X32KCR;	//RTC ʱ��, RTC_IRC32KCR, RTC_X32KCR
	RTC_InitStructure.RTC_Enable = ENABLE;			//RTC����ʹ��,   ENABLE, DISABLE
	RTC_InitStructure.RTC_Year   = 21;					//RTC ��, 00~99, ��Ӧ2000~2099��
	RTC_InitStructure.RTC_Month  = 12;					//RTC ��, 01~12
	RTC_InitStructure.RTC_Day    = 31;					//RTC ��, 01~31
	RTC_InitStructure.RTC_Hour   = 23;					//RTC ʱ, 00~23
	RTC_InitStructure.RTC_Min    = 59;					//RTC ��, 00~59
	RTC_InitStructure.RTC_Sec    = 55;					//RTC ��, 00~59
	RTC_InitStructure.RTC_Ssec   = 00;					//RTC 1/128��, 00~127

	RTC_InitStructure.RTC_ALAHour= 00;					//RTC ����ʱ, 00~23
	RTC_InitStructure.RTC_ALAMin = 00;					//RTC ���ӷ�, 00~59
	RTC_InitStructure.RTC_ALASec = 00;					//RTC ������, 00~59
	RTC_InitStructure.RTC_ALASsec= 00;					//RTC ����1/128��, 00~127
	RTC_Inilize(&RTC_InitStructure);
	NVIC_RTC_Init(RTC_ALARM_INT|RTC_SEC_INT,Priority_0);		//�ж�ʹ��, RTC_ALARM_INT/RTC_DAY_INT/RTC_HOUR_INT/RTC_MIN_INT/RTC_SEC_INT/RTC_SEC2_INT/RTC_SEC8_INT/RTC_SEC32_INT/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}

/******************** task A **************************/
void main(void)
{
	u8  i;
	
	WTST = 0;		//���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
	EAXSFR();		//��չSFR(XFR)����ʹ�� 
	CKCON = 0;      //��߷���XRAM�ٶ�

	GPIO_config();
	Timer_config();
	UART_config();
	RTC_config();
	EA = 1;

	display_index = 0;
	
	for(i=0; i<8; i++)  LED8[i] = 0x10; //�ϵ�����

	DisplayRTC();
	LED8[2] = DIS_;
	LED8[5] = DIS_;

	KeyHoldCnt = 0; //�����¼�ʱ
	KeyCode = 0;    //���û�ʹ�õļ���

	IO_KeyState = 0;
	IO_KeyState1 = 0;
	IO_KeyHoldCnt = 0;
	cnt50ms = 0;

	while (1)
	{
		if(B_1S)
		{
			B_1S = 0;
			DisplayRTC();
			printf("Year=20%d,Month=%d,Day=%d,Hour=%d,Minute=%d,Second=%d\r\n",YEAR,MONTH,DAY,HOUR,MIN,SEC);
		}

		if(B_Alarm)
		{
			B_Alarm = 0;
			printf("RTC Alarm!\r\n");
		}

		if(T0_1ms)
		{
			T0_1ms = 0;
		#if(SleepModeSet == 1)
			_nop_();
			_nop_();
			PD = 1;  //STC32G оƬʹ���ڲ�32Kʱ�ӣ������޷�����
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			_nop_();
		#else
			DisplayScan();
			
			if(++cnt50ms >= 50)     //50msɨ��һ�����м���
			{
				cnt50ms = 0;
				IO_KeyScan();
			}
			
			if(KeyCode != 0)        //�м�����
			{
				if(KeyCode == 25)   //hour +1
				{
					if(++hour >= 24)    hour = 0;
					WriteRTC();
					DisplayRTC();
				}
				if(KeyCode == 26)   //hour -1
				{
					if(--hour >= 24)    hour = 23;
					WriteRTC();
					DisplayRTC();
				}
				if(KeyCode == 27)   //minute +1
				{
					second = 0;
					if(++minute >= 60)  minute = 0;
					WriteRTC();
					DisplayRTC();
				}
				if(KeyCode == 28)   //minute -1
				{
					second = 0;
					if(--minute >= 60)  minute = 59;
					WriteRTC();
					DisplayRTC();
				}

				KeyCode = 0;
			}
		#endif
		}
	}
}

/********************** ��ʾʱ�Ӻ��� ************************/
void DisplayRTC(void)
{
    hour = HOUR;
    minute = MIN;

    if(HOUR >= 10)  LED8[0] = HOUR / 10;
    else            LED8[0] = DIS_BLACK;
    LED8[1] = HOUR % 10;
    LED8[2] = DIS_;
    LED8[3] = MIN / 10;
    LED8[4] = MIN % 10;
    LED8[5] = DIS_;
    LED8[6] = SEC / 10;
    LED8[7] = SEC % 10;
}

/********************** дRTC���� ************************/
void WriteRTC(void)
{
    INIYEAR = YEAR;   //�̳е�ǰ������
    INIMONTH = MONTH;
    INIDAY = DAY;

    INIHOUR = hour;   //�޸�ʱ����
    INIMIN = minute;
    INISEC = 0;
    INISSEC = 0;
    RTCCFG |= 0x01;   //����RTC�Ĵ�����ʼ��
}


/*****************************************************
    ���м�ɨ�����
    ʹ��XY����4x4���ķ�����ֻ�ܵ������ٶȿ�

   Y     P04      P05      P06      P07
          |        |        |        |
X         |        |        |        |
P00 ---- K00 ---- K01 ---- K02 ---- K03 ----
          |        |        |        |
P01 ---- K04 ---- K05 ---- K06 ---- K07 ----
          |        |        |        |
P02 ---- K08 ---- K09 ---- K10 ---- K11 ----
          |        |        |        |
P03 ---- K12 ---- K13 ---- K14 ---- K15 ----
          |        |        |        |
******************************************************/


u8 code T_KeyTable[16] = {0,1,2,0,3,0,0,0,4,0,0,0,0,0,0,0};

void IO_KeyDelay(void)
{
    u8 i;
    i = 60;
    while(--i)  ;
}

void IO_KeyScan(void)    //50ms call
{
    u8  j;

    j = IO_KeyState1;   //������һ��״̬

    P0 = 0xf0;  //X�ͣ���Y
    IO_KeyDelay();
    IO_KeyState1 = P0 & 0xf0;

    P0 = 0x0f;  //Y�ͣ���X
    IO_KeyDelay();
    IO_KeyState1 |= (P0 & 0x0f);
    IO_KeyState1 ^= 0xff;   //ȡ��
    
    if(j == IO_KeyState1)   //�������ζ����
    {
        j = IO_KeyState;
        IO_KeyState = IO_KeyState1;
        if(IO_KeyState != 0)    //�м�����
        {
            F0 = 0;
            if(j == 0)  F0 = 1; //��һ�ΰ���
            else if(j == IO_KeyState)
            {
                if(++IO_KeyHoldCnt >= 20)   //1����ؼ�
                {
                    IO_KeyHoldCnt = 18;
                    F0 = 1;
                }
            }
            if(F0)
            {
                j = T_KeyTable[IO_KeyState >> 4];
                if((j != 0) && (T_KeyTable[IO_KeyState& 0x0f] != 0)) 
                    KeyCode = (j - 1) * 4 + T_KeyTable[IO_KeyState & 0x0f] + 16;    //������룬17~32
            }
        }
        else    IO_KeyHoldCnt = 0;
    }
    P0 = 0xff;
}

void DisplayScan(void)
{   
    P7 = ~T_COM[7-display_index];
    P6 = ~t_display[LED8[display_index]];
    if(++display_index >= 8)    display_index = 0;  //8λ������0
}
