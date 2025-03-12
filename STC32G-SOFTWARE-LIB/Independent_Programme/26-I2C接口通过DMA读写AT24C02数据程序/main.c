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

#include	"STC32G_GPIO.h"
#include	"STC32G_DMA.h"
#include	"STC32G_I2C.h"
#include	"STC32G_NVIC.h"
#include	"STC32G_UART.h"
#include	"STC32G_Delay.h"
#include	"STC32G_Switch.h"

/*************   ����˵��   ***************

ͨ������1(P3.0 P3.1)��ָ��ͨ��I2C DMA��дAT24C02����.

Ĭ�ϲ�����:  115200,N,8,1. 

������������: (������ĸ�����ִ�Сд)
    W 0x10 12345678 --> д�����  ʮ�����Ƶ�ַ  д������.
    R 0x10 8        --> ��������  ʮ�����Ƶ�ַ  �����ֽ���.

24CXX����д����ֽ�����ͬ, 1\2Kbit 8-Byte/Page, 4\8\16Kbit 16-Byte/Page.
����д��Ļ�����ַ+���ݳ��Ȳ�Ҫ����һ��PAGE��Χ.

����ʱ, ѡ��ʱ�� 22.1184MHz (�����������ļ�"config.h"���޸�).

******************************************/

/*************	���س�������	**************/

#define SLAW    0xA0
#define SLAR    0xA1

#define EE_BUF_LENGTH       255      //���û�������������

/*************	���ر�������	**************/

u8 EEPROM_addr;
u8 xdata I2cTxBuffer[EE_BUF_LENGTH+1];
u8 xdata I2cRxBuffer[EE_BUF_LENGTH+1];

/*************	���غ�������	**************/

void WriteNbyte(u8 addr, u8 number);
void ReadNbyte( u8 addr, u8 number);

/*************  �ⲿ�����ͱ������� *****************/


/******************** IO������ ********************/
void	GPIO_config(void)
{
	P2_MODE_OUT_OD(GPIO_Pin_4 | GPIO_Pin_5);		//P2.4,P2.5 ����Ϊ׼˫���
	I2C_SW(I2C_P24_P25);					//I2C_P14_P15,I2C_P24_P25,I2C_P76_P77,I2C_P33_P32
}

/******************** I2C ���� ********************/
void	I2C_config(void)
{
	I2C_InitTypeDef		I2C_InitStructure;

	I2C_InitStructure.I2C_Mode      = I2C_Mode_Master;	//����ѡ��   I2C_Mode_Master, I2C_Mode_Slave
	I2C_InitStructure.I2C_Enable    = ENABLE;			//I2C����ʹ��,   ENABLE, DISABLE
	I2C_InitStructure.I2C_MS_WDTA   = DISABLE;			//����ʹ���Զ�����,  ENABLE, DISABLE
	I2C_InitStructure.I2C_Speed     = 63;				//�����ٶ�=Fosc/2/(Speed*2+4),      0~63
	I2C_Init(&I2C_InitStructure);
	NVIC_I2C_Init(I2C_Mode_Master,DISABLE,Priority_0);		//����ģʽ, I2C_Mode_Master, I2C_Mode_Slave; �ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}

/******************** UART ���� ********************/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;		//�ṹ����

	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//ģʽ, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//ѡ�����ʷ�����, BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//������, һ�� 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
	UART_Configuration(UART1, &COMx_InitStructure);		//��ʼ������1 UART1,UART2,UART3,UART4
	NVIC_UART1_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}

/******************** DMA ���� ********************/
void	DMA_config(void)
{
	DMA_I2C_InitTypeDef		DMA_I2C_InitStructure;		//�ṹ����

	DMA_I2C_InitStructure.DMA_TX_Length = EE_BUF_LENGTH;	//DMA�������ֽ���  	(0~65535) + 1
	DMA_I2C_InitStructure.DMA_TX_Buffer = (u16)I2cTxBuffer;	//�������ݴ洢��ַ
	DMA_I2C_InitStructure.DMA_RX_Length = EE_BUF_LENGTH;	//DMA�������ֽ���  	(0~65535) + 1
	DMA_I2C_InitStructure.DMA_RX_Buffer = (u16)I2cRxBuffer;	//�������ݴ洢��ַ
	DMA_I2C_InitStructure.DMA_TX_Enable = ENABLE;		//DMAʹ��  	ENABLE,DISABLE
	DMA_I2C_InitStructure.DMA_RX_Enable = ENABLE;		//DMAʹ��  	ENABLE,DISABLE
	DMA_I2C_Inilize(&DMA_I2C_InitStructure);	//��ʼ��

	NVIC_DMA_I2CT_Init(ENABLE,Priority_0,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0~Priority_3; �������ȼ�(�͵���) Priority_0~Priority_3
	NVIC_DMA_I2CR_Init(ENABLE,Priority_0,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0~Priority_3; �������ȼ�(�͵���) Priority_0~Priority_3
	DMA_I2CR_CLRFIFO();		//��� DMA FIFO
}

//========================================================================
// ����: I2cCheckData
// ����: ����У�麯��.
// ����: None.
// ����: None.
// �汾: V1.0, 2020-09-28
//========================================================================
u8 I2cCheckData(u8 dat)
{
	if((dat >= '0') && (dat <= '9'))		return (dat-'0');
	if((dat >= 'A') && (dat <= 'F'))		return (dat-'A'+10);
	if((dat >= 'a') && (dat <= 'f'))		return (dat-'a'+10);
	return 0xff;
}

//========================================================================
// ����: I2cGetAddress
// ����: ����������뷽ʽ�ĵ�ַ.
// ����: ��.
// ����: 8λEEPROM��ַ.
// �汾: V1.0, 2013-6-6
//========================================================================
u8 I2cGetAddress(void)
{
    u8 address;
    u8  i,j;
    
    address = 0;
    if((RX1_Buffer[2] == '0') && (RX1_Buffer[3] == 'X'))
    {
        for(i=4; i<6; i++)
        {
            j = I2cCheckData(RX1_Buffer[i]);
            if(j >= 0x10)   return 0;  //error
            address = (address << 4) + j;
        }
        return (address);
    }
    return  0; //error
}

//========================================================================
// ����: I2cGetDataLength
// ����: ��ȡҪ�������ݵ��ֽ���.
// ����: ��.
// ����: 1Ҫ�������ݵ��ֽ���.
// �汾: V1.0, 2013-6-6
//========================================================================
u8 I2cGetDataLength(void)
{
	u8  i;
	u8  length;
	
	length = 0;
	for(i=7; i<COM1.RX_Cnt; i++)
	{
		if(I2cCheckData(RX1_Buffer[i]) >= 10)  break;
		length = length * 10 + I2cCheckData(RX1_Buffer[i]);
	}
	return (length);
}

/**********************************************/
void main(void)
{
	u8  i,j;
	u8  addr;
	u8  status;

	WTST = 0;		//���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
	EAXSFR();		//��չSFR(XFR)����ʹ�� 
	CKCON = 0;      //��߷���XRAM�ٶ�

	GPIO_config();
	I2C_config();
	DMA_config();
	UART_config();
	EA = 1;
	printf("��������:\r\n");
	printf("W 0x10 12345678 --> д�����  ʮ�����Ƶ�ַ  д������\r\n");
	printf("R 0x10 8        --> ��������  ʮ�����Ƶ�ַ  �����ֽ�����\r\n");

	while (1)
	{
		delay_ms(1);
		if(COM1.RX_TimeOut > 0)		//��ʱ����
		{
			if(--COM1.RX_TimeOut == 0)
			{
	//			printf("�յ��������£� ");
	//			for(i=0; i<COM1.RX_Cnt; i++)    printf("%c", RX1_Buffer[i]);    //���յ�������ԭ������,���ڲ���
	//			printf("\r\n");

				status = 0xff;  //״̬��һ����0ֵ
				if((COM1.RX_Cnt >= 8) && (RX1_Buffer[1] == ' ')) //�������Ϊ8���ֽ�
				{
					for(i=0; i<6; i++)
					{
						if((RX1_Buffer[i] >= 'a') && (RX1_Buffer[i] <= 'z'))    RX1_Buffer[i] = RX1_Buffer[i] - 'a' + 'A';  //Сдת��д
					}
					addr = I2cGetAddress();
					//if(addr <= 255)    //���Ƶ�ַ��Χ
					{
						if((RX1_Buffer[0] == 'W')&& (RX1_Buffer[6] == ' '))   //д��N���ֽ�
						{
							j = COM1.RX_Cnt - 7;
							if(j > EE_BUF_LENGTH)  j = EE_BUF_LENGTH; //Խ����

							for(i=0; i<j; i++)  I2cTxBuffer[i+2] = RX1_Buffer[i+7];
							WriteNbyte(addr, j);     //дN���ֽ� 
							printf("��д��%d�ֽ�����!\r\n",j);
							delay_ms(5);

							ReadNbyte(addr, j);
							printf("����%d���ֽ��������£�\r\n",j);
							for(i=0; i<j; i++)    printf("%c", I2cRxBuffer[i]);
							printf("\r\n");

							status = 0; //������ȷ
						}
						else if((RX1_Buffer[0] == 'R') && (RX1_Buffer[6] == ' '))   //����N���ֽ�
						{
							j = I2cGetDataLength();
							if(j > EE_BUF_LENGTH)  j = EE_BUF_LENGTH; //Խ����
							if(j > 0)
							{
								ReadNbyte(addr, j);
								printf("����%d���ֽ��������£�\r\n",j);
								for(i=0; i<j; i++)    printf("%c", I2cRxBuffer[i]);
								printf("\r\n");

								status = 0; //������ȷ
							}
						}
					}
				}
				if(status != 0) printf("�������\r\n");
				COM1.RX_Cnt = 0;
			}
		}
	}
}

void WriteNbyte(u8 addr, u8 number)  /*  WordAddress,First Data Address,Byte lenth   */
{
	while(Get_MSBusy_Status());    //���I2C������æµ״̬

	DmaI2CTFlag = 1;
	I2cTxBuffer[0] = SLAW;
	I2cTxBuffer[1] = addr;

	I2C_MSCMD(0x89);			//��ʼ����+��������+����ACK
	I2C_DMA_Enable();
	SET_I2CT_DMA_LEN(number+1);	//���ô������ֽ�����n+1
	SET_I2C_DMA_ST(number+1);		//������Ҫ�����ֽ�����n+1
	DMA_I2CT_TRIG();

	while(DmaI2CTFlag);         //DMAæ���
	I2C_DMA_Disable();
}

void ReadNbyte(u8 addr, u8 number)   /*  WordAddress,First Data Address,Byte lenth   */
{
	while(Get_MSBusy_Status());    //���I2C������æµ״̬
	I2C_DMA_Disable();

	//������ʼ�ź�+�豸��ַ+д�ź�
	SendCmdData(0x09,SLAW);

	//���ʹ洢����ַ
	SendCmdData(0x0a,addr);
    
	//������ʼ�ź�+�豸��ַ+���ź�
	SendCmdData(0x09,SLAR);

	DmaI2CRFlag = 1;

	I2C_MSCMD(0x8b);			//��ʼ����+��������+����ACK
	I2C_DMA_Enable();
	SET_I2CR_DMA_LEN(number-1);	//���ô������ֽ�����n+1
	SET_I2C_DMA_ST(number-1);		//������Ҫ�����ֽ�����n+1
	DMA_I2CR_TRIG();

	while(DmaI2CRFlag);         //DMAæ���
	I2C_DMA_Disable();
}
