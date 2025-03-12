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

#include "STC32G_DMA.h"

bit	DmaTx1Flag;
bit	DmaRx1Flag;
bit	DmaTx2Flag;
bit	DmaRx2Flag;
bit	DmaTx3Flag;
bit	DmaRx3Flag;
bit	DmaTx4Flag;
bit	DmaRx4Flag;

//========================================================================
// ����: DMA_UART1TX_ISR_Handler
// ����: DMA UART1 TX �жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2022-03-23
//========================================================================
void DMA_UART1TX_ISR_Handler (void) interrupt DMA_UR1T_VECTOR
{
	// TODO: �ڴ˴�����û�����
	if (DMA_UR1T_STA & 0x01)	//�������
	{
		DMA_UR1T_STA &= ~0x01;	//���־λ
		DmaTx1Flag = 1;
	}
	if (DMA_UR1T_STA & 0x04)	//���ݸ���
	{
		DMA_UR1T_STA &= ~0x04;	//���־λ
	}
}

//========================================================================
// ����: DMA_UART1RX_ISR_Handler
// ����: DMA UART1 RX �жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2022-03-23
//========================================================================
void DMA_UART1RX_ISR_Handler (void) interrupt DMA_UR1R_VECTOR
{
	// TODO: �ڴ˴�����û�����
	if (DMA_UR1R_STA & 0x01)	//�������
	{
		DMA_UR1R_STA &= ~0x01;	//���־λ
		DmaRx1Flag = 1;
	}
	if (DMA_UR1R_STA & 0x02)	//���ݶ���
	{
		DMA_UR1R_STA &= ~0x02;	//���־λ
	}
}

//========================================================================
// ����: DMA_UART2TX_ISR_Handler
// ����: DMA UART2 TX �жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2022-03-23
//========================================================================
void DMA_UART2TX_ISR_Handler (void) interrupt DMA_UR2T_VECTOR
{
	// TODO: �ڴ˴�����û�����
	if (DMA_UR2T_STA & 0x01)	//�������
	{
		DMA_UR2T_STA &= ~0x01;	//���־λ
		DmaTx2Flag = 1;
	}
	if (DMA_UR2T_STA & 0x04)	//���ݸ���
	{
		DMA_UR2T_STA &= ~0x04;	//���־λ
	}
}

//========================================================================
// ����: DMA_UART2RX_ISR_Handler
// ����: DMA UART2 RX �жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2022-03-23
//========================================================================
void DMA_UART2RX_ISR_Handler (void) interrupt DMA_UR2R_VECTOR
{
	// TODO: �ڴ˴�����û�����
	if (DMA_UR2R_STA & 0x01)	//�������
	{
		DMA_UR2R_STA &= ~0x01;	//���־λ
		DmaRx2Flag = 1;
	}
	if (DMA_UR2R_STA & 0x02)	//���ݶ���
	{
		DMA_UR2R_STA &= ~0x02;	//���־λ
	}
}

//========================================================================
// ����: DMA_UART3TX_ISR_Handler
// ����: DMA UART3 TX �жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2022-03-23
//========================================================================
void DMA_UART3TX_ISR_Handler (void) interrupt DMA_UR3T_VECTOR
{
	// TODO: �ڴ˴�����û�����
	if (DMA_UR3T_STA & 0x01)	//�������
	{
		DMA_UR3T_STA &= ~0x01;	//���־λ
		DmaTx3Flag = 1;
	}
	if (DMA_UR3T_STA & 0x04)	//���ݸ���
	{
		DMA_UR3T_STA &= ~0x04;	//���־λ
	}
}

//========================================================================
// ����: DMA_UART3RX_ISR_Handler
// ����: DMA UART3 RX �жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2022-03-23
//========================================================================
void DMA_UART3RX_ISR_Handler (void) interrupt DMA_UR3R_VECTOR
{
	// TODO: �ڴ˴�����û�����
	if (DMA_UR3R_STA & 0x01)	//�������
	{
		DMA_UR3R_STA &= ~0x01;	//���־λ
		DmaRx3Flag = 1;
	}
	if (DMA_UR3R_STA & 0x02)	//���ݶ���
	{
		DMA_UR3R_STA &= ~0x02;	//���־λ
	}
}

//========================================================================
// ����: DMA_UART4TX_ISR_Handler
// ����: DMA UART4 TX �жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2022-03-23
//========================================================================
void DMA_UART4TX_ISR_Handler (void) interrupt DMA_UR4T_VECTOR
{
	// TODO: �ڴ˴�����û�����
	if (DMA_UR4T_STA & 0x01)	//�������
	{
		DMA_UR4T_STA &= ~0x01;	//���־λ
		DmaTx4Flag = 1;
	}
	if (DMA_UR4T_STA & 0x04)	//���ݸ���
	{
		DMA_UR4T_STA &= ~0x04;	//���־λ
	}
}

//========================================================================
// ����: DMA_UART4RX_ISR_Handler
// ����: DMA UART4 RX �жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2022-03-23
//========================================================================
void DMA_UART4RX_ISR_Handler (void) interrupt DMA_UR4R_VECTOR
{
	// TODO: �ڴ˴�����û�����
	if (DMA_UR4R_STA & 0x01)	//�������
	{
		DMA_UR4R_STA &= ~0x01;	//���־λ
		DmaRx4Flag = 1;
	}
	if (DMA_UR4R_STA & 0x02)	//���ݶ���
	{
		DMA_UR4R_STA &= ~0x02;	//���־λ
	}
}

//========================================================================
// ����: DMA_ISR_Handler
// ����: DMA�жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2021-05-25
//========================================================================
void DMA_ISR_Handler (void) interrupt 13
{
	// TODO: �ڴ˴�����û�����
	
	//---------- DMA UART1 -------------
	if (DMA_UR1T_STA & 0x01)	//�������
	{
		DMA_UR1T_STA &= ~0x01;	//���־λ
		DmaTx1Flag = 1;
	}
	if (DMA_UR1T_STA & 0x04)	//���ݸ���
	{
		DMA_UR1T_STA &= ~0x04;	//���־λ
	}
	
	if (DMA_UR1R_STA & 0x01)	//�������
	{
		DMA_UR1R_STA &= ~0x01;	//���־λ
		DmaRx1Flag = 1;
	}
	if (DMA_UR1R_STA & 0x02)	//���ݶ���
	{
		DMA_UR1R_STA &= ~0x02;	//���־λ
	}
	//---------- DMA UART2 -------------
	if (DMA_UR2T_STA & 0x01)	//�������
	{
		DMA_UR2T_STA &= ~0x01;	//���־λ
		DmaTx2Flag = 1;
	}
	if (DMA_UR2T_STA & 0x04)	//���ݸ���
	{
		DMA_UR2T_STA &= ~0x04;	//���־λ
	}
	
	if (DMA_UR2R_STA & 0x01)	//�������
	{
		DMA_UR2R_STA &= ~0x01;	//���־λ
		DmaRx2Flag = 1;
	}
	if (DMA_UR2R_STA & 0x02)	//���ݶ���
	{
		DMA_UR2R_STA &= ~0x02;	//���־λ
	}
	//---------- DMA UART3 -------------
	if (DMA_UR3T_STA & 0x01)	//�������
	{
		DMA_UR3T_STA &= ~0x01;	//���־λ
		DmaTx3Flag = 1;
	}
	if (DMA_UR3T_STA & 0x04)	//���ݸ���
	{
		DMA_UR3T_STA &= ~0x04;	//���־λ
	}
	
	if (DMA_UR3R_STA & 0x01)	//�������
	{
		DMA_UR3R_STA &= ~0x01;	//���־λ
		DmaRx3Flag = 1;
	}
	if (DMA_UR3R_STA & 0x02)	//���ݶ���
	{
		DMA_UR3R_STA &= ~0x02;	//���־λ
	}
	//---------- DMA UART4 -------------
	if (DMA_UR4T_STA & 0x01)	//�������
	{
		DMA_UR4T_STA &= ~0x01;	//���־λ
		DmaTx4Flag = 1;
	}
	if (DMA_UR4T_STA & 0x04)	//���ݸ���
	{
		DMA_UR4T_STA &= ~0x04;	//���־λ
	}
	
	if (DMA_UR4R_STA & 0x01)	//�������
	{
		DMA_UR4R_STA &= ~0x01;	//���־λ
		DmaRx4Flag = 1;
	}
	if (DMA_UR4R_STA & 0x02)	//���ݶ���
	{
		DMA_UR4R_STA &= ~0x02;	//���־λ
	}
}
