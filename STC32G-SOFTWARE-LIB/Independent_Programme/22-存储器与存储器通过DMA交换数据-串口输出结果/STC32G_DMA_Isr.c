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
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序            */
/*---------------------------------------------------------------------*/

#include "STC32G_DMA.h"

bit DmaM2MFlag = 0;

//========================================================================
// 函数: DMA_M2M_ISR_Handler
// 描述: DMA M2M 中断函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2022-03-23
//========================================================================
void DMA_M2M_ISR_Handler (void) interrupt DMA_M2M_VECTOR
{
	// TODO: 在此处添加用户代码
	if(DMA_M2M_STA & 0x01)	//M2M传输完成
	{
		DMA_M2M_STA &= ~0x01;	//清标志位
		DmaM2MFlag = 1;
	}
}

//========================================================================
// 函数: DMA_ISR_Handler
// 描述: DMA中断函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2021-05-25
//========================================================================
void DMA_ISR_Handler (void) interrupt 13
{
	// TODO: 在此处添加用户代码
	
	//----------- DMA M2M --------------
	if(DMA_M2M_STA & 0x01)	//M2M传输完成
	{
		DMA_M2M_STA &= ~0x01;	//清标志位
		DmaM2MFlag = 1;
	}
}
